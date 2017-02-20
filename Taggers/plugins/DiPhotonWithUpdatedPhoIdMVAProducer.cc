#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"

#include "TFile.h"
#include "TGraph.h"

using namespace std;
using namespace edm;
using namespace reco;

namespace flashgg {

    class DiPhotonWithUpdatedPhoIdMVAProducer : public edm::EDProducer
    {
    public:
        DiPhotonWithUpdatedPhoIdMVAProducer( const edm::ParameterSet & );
        void produce( edm::Event &, const edm::EventSetup & ) override;

    private:
        edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > token_;
        edm::EDGetTokenT<double> rhoToken_;
        PhotonIdUtils phoTools_;
        edm::FileInPath phoIdMVAweightfileEB_, phoIdMVAweightfileEE_, correctionFile_, non5x5correctionFile_;
        bool correctInputs_;
        bool debug_;
        //        std::vector<TGraph*> corrections_;
        std::vector<std::unique_ptr<TGraph> > corrections_;

        bool doNon5x5transformation_;
        std::vector<std::unique_ptr<TGraph> > non5x5corrections_;

        bool useNewPhoId_;

        EffectiveAreas _effectiveAreas;
        vector<double> _phoIsoPtScalingCoeff;
        double _phoIsoCutoff;
    };

    DiPhotonWithUpdatedPhoIdMVAProducer::DiPhotonWithUpdatedPhoIdMVAProducer( const edm::ParameterSet &ps ) :
        token_(consumes<edm::View<flashgg::DiPhotonCandidate> >(ps.getParameter<edm::InputTag>("src"))),
        rhoToken_( consumes<double>( ps.getParameter<edm::InputTag>( "rhoFixedGridCollection" ) ) ),
        debug_( ps.getParameter<bool>( "Debug" ) ),
        _effectiveAreas((ps.getParameter<edm::FileInPath>("effAreasConfigFile")).fullPath()),
        _phoIsoPtScalingCoeff(ps.getParameter<std::vector<double >>("phoIsoPtScalingCoeff")),
        _phoIsoCutoff(ps.getParameter<double>("phoIsoCutoff"))
    {

        useNewPhoId_ = ps.getParameter<bool>( "useNewPhoId" );
        phoIdMVAweightfileEB_ = ps.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EB" );
        phoIdMVAweightfileEE_ = ps.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EE" );
        if(useNewPhoId_){
            phoIdMVAweightfileEB_ = ps.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EB_new" );
            phoIdMVAweightfileEE_ = ps.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EE_new" );
        }
        phoTools_.setupMVA( phoIdMVAweightfileEB_.fullPath(), phoIdMVAweightfileEE_.fullPath(), useNewPhoId_ );

        correctInputs_ = ps.existsAs<edm::FileInPath>("correctionFile") ? true: false;
        if (correctInputs_) {
            correctionFile_ = ps.getParameter<edm::FileInPath>( "correctionFile" );
            TFile* f = TFile::Open(correctionFile_.fullPath().c_str());
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transffull5x5R9EB"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transfEtaWidthEB"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transfS4EB"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transffull5x5R9EE"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transfEtaWidthEE"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transfS4EE"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transffull5x5sieieEB"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transffull5x5sieieEE"))->Clone() );
            f->Close();
        }

        doNon5x5transformation_ =ps.getParameter<bool>( "doNon5x5transformation" );
        if (doNon5x5transformation_) {
            non5x5correctionFile_ = ps.getParameter<edm::FileInPath>( "non5x5correctionFile" );
            TFile* non5x5_f = TFile::Open(non5x5correctionFile_.fullPath().c_str());
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfr9EB"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfr9EE"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfsieieEB"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfsieieEE"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfsipipEB"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfsipipEE"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfsieipEB"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfsieipEE"))->Clone() );
            non5x5_f->Close();
        }

        produces<std::vector<flashgg::DiPhotonCandidate> >();
    }

    void DiPhotonWithUpdatedPhoIdMVAProducer::produce( edm::Event &evt, const edm::EventSetup & )
    {
        edm::Handle<edm::View<flashgg::DiPhotonCandidate> > objects;
        evt.getByToken( token_, objects );

        edm::Handle<double> rhoHandle;
        evt.getByToken( rhoToken_, rhoHandle );
        const double rhoFixedGrd = *( rhoHandle.product() );

        auto_ptr<std::vector<flashgg::DiPhotonCandidate> > out_obj( new std::vector<flashgg::DiPhotonCandidate>() );

        for (const auto & obj : *objects) {
            flashgg::DiPhotonCandidate *new_obj = obj.clone();
            new_obj->makePhotonsPersistent();
            double leadCorrectedEtaWidth = 0., subLeadCorrectedEtaWidth = 0.;
            if (not evt.isRealData() and correctInputs_) { 
                if (new_obj->getLeadingPhoton().isEB()) {
                    if (this->debug_) {
                        std::cout << new_obj->getLeadingPhoton().full5x5_r9() << std::endl;
                        std::cout << new_obj->getLeadingPhoton().old_r9() << std::endl;
                    }
                    reco::Photon::ShowerShape newShowerShapes = new_obj->getLeadingPhoton().full5x5_showerShapeVariables();
                    newShowerShapes.e3x3 = corrections_[0]->Eval(new_obj->getLeadingPhoton().full5x5_r9())*new_obj->getLeadingPhoton().superCluster()->rawEnergy();
                    newShowerShapes.sigmaIetaIeta = corrections_[6]->Eval(new_obj->getLeadingPhoton().full5x5_sigmaIetaIeta());
                    new_obj->getLeadingPhoton().full5x5_setShowerShapeVariables(newShowerShapes);
                    leadCorrectedEtaWidth = corrections_[1]->Eval(new_obj->getLeadingPhoton().superCluster()->etaWidth());
                    new_obj->getLeadingPhoton().getSuperCluster()->setEtaWidth(leadCorrectedEtaWidth);
                    new_obj->getLeadingPhoton().setS4(corrections_[2]->Eval(new_obj->getLeadingPhoton().s4()));

                    if (this->debug_) {
                        std::cout << new_obj->getLeadingPhoton().full5x5_r9() << std::endl;
                        std::cout << new_obj->getLeadingPhoton().old_r9() << std::endl;
                    }
                }
                
                if (new_obj->getSubLeadingPhoton().isEB()) {
                    reco::Photon::ShowerShape newShowerShapes = new_obj->getSubLeadingPhoton().full5x5_showerShapeVariables();
                    newShowerShapes.e3x3 = corrections_[0]->Eval(new_obj->getSubLeadingPhoton().full5x5_r9())*new_obj->getSubLeadingPhoton().superCluster()->rawEnergy();
                    newShowerShapes.sigmaIetaIeta = corrections_[6]->Eval(new_obj->getSubLeadingPhoton().full5x5_sigmaIetaIeta());
                    new_obj->getSubLeadingPhoton().full5x5_setShowerShapeVariables(newShowerShapes);
                    subLeadCorrectedEtaWidth = corrections_[1]->Eval(new_obj->getSubLeadingPhoton().superCluster()->etaWidth());
                    new_obj->getSubLeadingPhoton().getSuperCluster()->setEtaWidth(subLeadCorrectedEtaWidth);
                    new_obj->getSubLeadingPhoton().setS4(corrections_[2]->Eval(new_obj->getSubLeadingPhoton().s4()));
                }

                if (new_obj->getLeadingPhoton().isEE()) {
                    if (this->debug_) {
                        std::cout << new_obj->getLeadingPhoton().full5x5_r9() << std::endl;
                        std::cout << new_obj->getLeadingPhoton().old_r9() << std::endl;
                    }
                    reco::Photon::ShowerShape newShowerShapes = new_obj->getLeadingPhoton().full5x5_showerShapeVariables();
                    newShowerShapes.e3x3 = corrections_[3]->Eval(new_obj->getLeadingPhoton().full5x5_r9())*new_obj->getLeadingPhoton().superCluster()->rawEnergy();
                    newShowerShapes.sigmaIetaIeta = corrections_[7]->Eval(new_obj->getLeadingPhoton().full5x5_sigmaIetaIeta());
                    new_obj->getLeadingPhoton().full5x5_setShowerShapeVariables(newShowerShapes);
                    leadCorrectedEtaWidth = corrections_[4]->Eval(new_obj->getLeadingPhoton().superCluster()->etaWidth());
                    new_obj->getLeadingPhoton().getSuperCluster()->setEtaWidth(leadCorrectedEtaWidth);
                    new_obj->getLeadingPhoton().setS4(corrections_[5]->Eval(new_obj->getLeadingPhoton().s4()));

                    if (this->debug_) {
                        std::cout << new_obj->getLeadingPhoton().full5x5_r9() << std::endl;
                        std::cout << new_obj->getLeadingPhoton().old_r9() << std::endl;
                    }
                }
                
                if (new_obj->getSubLeadingPhoton().isEE()) {
                    reco::Photon::ShowerShape newShowerShapes = new_obj->getSubLeadingPhoton().full5x5_showerShapeVariables();
                    newShowerShapes.e3x3 = corrections_[3]->Eval(new_obj->getSubLeadingPhoton().full5x5_r9())*new_obj->getSubLeadingPhoton().superCluster()->rawEnergy();
                    newShowerShapes.sigmaIetaIeta = corrections_[7]->Eval(new_obj->getSubLeadingPhoton().full5x5_sigmaIetaIeta());
                    new_obj->getSubLeadingPhoton().full5x5_setShowerShapeVariables(newShowerShapes);
                    subLeadCorrectedEtaWidth = corrections_[4]->Eval(new_obj->getSubLeadingPhoton().superCluster()->etaWidth());
                    new_obj->getSubLeadingPhoton().getSuperCluster()->setEtaWidth(subLeadCorrectedEtaWidth);
                    new_obj->getSubLeadingPhoton().setS4(corrections_[5]->Eval(new_obj->getSubLeadingPhoton().s4()));
                }
            }

            if (not evt.isRealData() and doNon5x5transformation_) { 
                if (new_obj->getLeadingPhoton().isEB()) {
                    if (this->debug_) {
                        std::cout << new_obj->getLeadingPhoton().full5x5_r9() << std::endl;
                        std::cout << new_obj->getLeadingPhoton().old_r9() << std::endl;
                    }
                    reco::Photon::ShowerShape non5x5ShowerShapes = new_obj->getLeadingPhoton().showerShapeVariables();
                    non5x5ShowerShapes.e3x3 = non5x5corrections_[0]->Eval(new_obj->getLeadingPhoton().old_r9())*new_obj->getLeadingPhoton().superCluster()->rawEnergy();
                    non5x5ShowerShapes.sigmaIetaIeta = non5x5corrections_[2]->Eval(new_obj->getLeadingPhoton().sigmaIetaIeta());
                    new_obj->getLeadingPhoton().setShowerShapeVariables(non5x5ShowerShapes);
                    new_obj->getLeadingPhoton().setSipip(non5x5corrections_[4]->Eval(new_obj->getLeadingPhoton().sipip()));
                    new_obj->getLeadingPhoton().setSieip(non5x5corrections_[6]->Eval(new_obj->getLeadingPhoton().sieip()));

                    if (this->debug_) {
                        std::cout << new_obj->getLeadingPhoton().full5x5_r9() << std::endl;
                        std::cout << new_obj->getLeadingPhoton().old_r9() << std::endl;
                    }
                }
                
                if (new_obj->getSubLeadingPhoton().isEB()) {
                    reco::Photon::ShowerShape non5x5ShowerShapes = new_obj->getSubLeadingPhoton().showerShapeVariables();
                    non5x5ShowerShapes.e3x3 = non5x5corrections_[0]->Eval(new_obj->getSubLeadingPhoton().old_r9())*new_obj->getSubLeadingPhoton().superCluster()->rawEnergy();
                    non5x5ShowerShapes.sigmaIetaIeta = non5x5corrections_[2]->Eval(new_obj->getSubLeadingPhoton().sigmaIetaIeta());
                    new_obj->getSubLeadingPhoton().setShowerShapeVariables(non5x5ShowerShapes);
                    new_obj->getSubLeadingPhoton().setSipip(non5x5corrections_[4]->Eval(new_obj->getSubLeadingPhoton().sipip()));
                    new_obj->getSubLeadingPhoton().setSieip(non5x5corrections_[6]->Eval(new_obj->getSubLeadingPhoton().sieip()));
                }

                if (new_obj->getLeadingPhoton().isEE()) {
                    if (this->debug_) {
                        std::cout << new_obj->getLeadingPhoton().full5x5_r9() << std::endl;
                        std::cout << new_obj->getLeadingPhoton().old_r9() << std::endl;
                    }
                    reco::Photon::ShowerShape non5x5ShowerShapes = new_obj->getLeadingPhoton().showerShapeVariables();
                    non5x5ShowerShapes.e3x3 = non5x5corrections_[1]->Eval(new_obj->getLeadingPhoton().old_r9())*new_obj->getLeadingPhoton().superCluster()->rawEnergy();
                    non5x5ShowerShapes.sigmaIetaIeta = non5x5corrections_[3]->Eval(new_obj->getLeadingPhoton().sigmaIetaIeta());
                    new_obj->getLeadingPhoton().setShowerShapeVariables(non5x5ShowerShapes);
                    new_obj->getLeadingPhoton().setSipip(non5x5corrections_[5]->Eval(new_obj->getLeadingPhoton().sipip()));
                    new_obj->getLeadingPhoton().setSieip(non5x5corrections_[7]->Eval(new_obj->getLeadingPhoton().sieip()));

                    if (this->debug_) {
                        std::cout << new_obj->getLeadingPhoton().full5x5_r9() << std::endl;
                        std::cout << new_obj->getLeadingPhoton().old_r9() << std::endl;
                    }
                }
                
                if (new_obj->getSubLeadingPhoton().isEE()) {
                    reco::Photon::ShowerShape non5x5ShowerShapes = new_obj->getSubLeadingPhoton().showerShapeVariables();
                    non5x5ShowerShapes.e3x3 = non5x5corrections_[1]->Eval(new_obj->getSubLeadingPhoton().old_r9())*new_obj->getSubLeadingPhoton().superCluster()->rawEnergy();
                    non5x5ShowerShapes.sigmaIetaIeta = non5x5corrections_[3]->Eval(new_obj->getSubLeadingPhoton().sigmaIetaIeta());
                    new_obj->getSubLeadingPhoton().setShowerShapeVariables(non5x5ShowerShapes);
                    new_obj->getSubLeadingPhoton().setSipip(non5x5corrections_[5]->Eval(new_obj->getSubLeadingPhoton().sipip()));
                    new_obj->getSubLeadingPhoton().setSieip(non5x5corrections_[7]->Eval(new_obj->getSubLeadingPhoton().sieip()));
                }
            }

            if (this->debug_) {
                std::cout << " Input DiPhoton lead (sublead) MVA: " << obj.leadPhotonId() << " " << obj.subLeadPhotonId() << std::endl;
            }
            double eA_leadPho = _effectiveAreas.getEffectiveArea( abs(new_obj->getLeadingPhoton().superCluster()->eta()) );
            double eA_subLeadPho = _effectiveAreas.getEffectiveArea( abs(new_obj->getSubLeadingPhoton().superCluster()->eta()) );
            
            float newleadmva = phoTools_.computeMVAWrtVtx( new_obj->getLeadingPhoton(), new_obj->vtx(), rhoFixedGrd, leadCorrectedEtaWidth, eA_leadPho, _phoIsoPtScalingCoeff, _phoIsoCutoff );
            new_obj->getLeadingPhoton().setPhoIdMvaWrtVtx( new_obj->vtx(), newleadmva);
            float newsubleadmva = phoTools_.computeMVAWrtVtx( new_obj->getSubLeadingPhoton(), new_obj->vtx(), rhoFixedGrd, subLeadCorrectedEtaWidth,eA_subLeadPho, _phoIsoPtScalingCoeff, _phoIsoCutoff );
            new_obj->getSubLeadingPhoton().setPhoIdMvaWrtVtx( new_obj->vtx(), newsubleadmva);
            if (this->debug_) {
                std::cout << " Output DiPhoton lead (sublead) MVA: " << new_obj->leadPhotonId() << " " << new_obj->subLeadPhotonId() << std::endl;
            }
            out_obj->push_back(*new_obj);
            delete new_obj;
        }
        evt.put(out_obj);
    }
}

typedef flashgg::DiPhotonWithUpdatedPhoIdMVAProducer FlashggDiPhotonWithUpdatedPhoIdMVAProducer;
DEFINE_FWK_MODULE( FlashggDiPhotonWithUpdatedPhoIdMVAProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
