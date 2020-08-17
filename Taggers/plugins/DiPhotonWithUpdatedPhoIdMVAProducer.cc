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
// #include "RecoEgamma/EgammaTools/plugins/EGExtraInfoModifierFromDB.cc"
#include "CommonTools/CandAlgos/interface/ModifyObjectValueBase.h"
#include "flashgg/Taggers/src/IsolationCorrection.C"

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

        void storePhotonRegressions(flashgg::DiPhotonCandidate & diph, const std::string & label);
        void updatePhotonRegressions(flashgg::DiPhotonCandidate & diph);

    private:
        float correctPhoton( flashgg::Photon & ph );
        void correctPhoton_non5x5( flashgg::Photon & ph ) ;
        void storeRegression(flashgg::Photon & cand, const std::string & label);

        edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > token_;
        edm::EDGetTokenT<double> rhoToken_;
        PhotonIdUtils phoTools_;
        edm::FileInPath phoIdMVAweightfileEB_, phoIdMVAweightfileEE_, correctionFile_, non5x5correctionFile_;
        shared_ptr<ModifyObjectValueBase> regress_;
        bool reRunRegression_, reRunRegressionOnData_;
        bool debug_;
        //        std::vector<TGraph*> corrections_;
        bool correctInputs_;
        std::vector<std::unique_ptr<TGraph> > corrections_;

        bool doNon5x5transformation_;
        std::vector<std::unique_ptr<TGraph> > non5x5corrections_;

        bool useNewPhoId_;
        bool is2017_;

        EffectiveAreas _effectiveAreas;
        vector<double> _phoIsoPtScalingCoeff;
        double _phoIsoCutoff;

        bool keepInitialEnergy_;
        bool _doIsoCorrection;
        unique_ptr<IsolationCorrection> _isoCorrector;
        // edm::EDGetTokenT<edm::View<vector<reco::SuperCluster>> > SCtoken_;
        // vector<reco::SuperCluster> _SuperClusters; 
    };

    DiPhotonWithUpdatedPhoIdMVAProducer::DiPhotonWithUpdatedPhoIdMVAProducer( const edm::ParameterSet &ps ) :
        token_(consumes<edm::View<flashgg::DiPhotonCandidate> >(ps.getParameter<edm::InputTag>("src"))),
        rhoToken_( consumes<double>( ps.getParameter<edm::InputTag>( "rhoFixedGridCollection" ) ) ),
        regress_(0),
        debug_( ps.getParameter<bool>( "Debug" ) ),
        correctInputs_(ps.getParameter<bool>("do5x5correction")),
        doNon5x5transformation_(ps.getParameter<bool>("doNon5x5transformation")),
        _effectiveAreas((ps.getParameter<edm::FileInPath>("effAreasConfigFile")).fullPath()),
        _phoIsoPtScalingCoeff(ps.getParameter<std::vector<double >>("phoIsoPtScalingCoeff")),
        _phoIsoCutoff(ps.getParameter<double>("phoIsoCutoff")),
        keepInitialEnergy_(ps.getParameter<bool>("keepInitialEnergy")),
        _doIsoCorrection(ps.getParameter<bool>("doIsoCorrection"))
        // SCtoken_(ps.getParameter<vector<reco::SuperCluster>>("SuperClusterTag"))
        // _SuperClusters(ps.getParameter<vector<reco::SuperCluster>>("SuperClusterTag")) // need absence of comma for last entry 
        
    {
        if (_doIsoCorrection) {
            _isoCorrector = make_unique<IsolationCorrection>(ps.getParameter<edm::FileInPath>("isoCorrectionFile").fullPath().c_str());
        }

        useNewPhoId_ = ps.getParameter<bool>( "useNewPhoId" );
        is2017_ = ps.getParameter<bool>( "is2017" );

        phoIdMVAweightfileEB_ = ps.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EB" );
        phoIdMVAweightfileEE_ = ps.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EE" );
        if(is2017_){
            phoIdMVAweightfileEB_ = ps.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EB_2017" );
            phoIdMVAweightfileEE_ = ps.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EE_2017" );
        }
        else if(useNewPhoId_){
            phoIdMVAweightfileEB_ = ps.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EB_new" );
            phoIdMVAweightfileEE_ = ps.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EE_new" );
        }
        phoTools_.setupMVA( phoIdMVAweightfileEB_.fullPath(), phoIdMVAweightfileEE_.fullPath(), useNewPhoId_ , is2017_);

        correctInputs_ = ps.getParameter<bool>("do5x5correction");
        if (correctInputs_) {
            correctionFile_ = ps.getParameter<edm::FileInPath>( "correctionFile" );
            TFile* f = TFile::Open(correctionFile_.fullPath().c_str());
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transffull5x5R9EB"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transfEtaWidthEB"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transfS4EB"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transffull5x5sieieEB"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transffull5x5R9EE"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transfEtaWidthEE"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transfS4EE"))->Clone() );
            corrections_.emplace_back((TGraph*)((TGraph*) f->Get("transffull5x5sieieEE"))->Clone() );
            f->Close();
        }

        doNon5x5transformation_ = ps.getParameter<bool>( "doNon5x5transformation" );
        if (doNon5x5transformation_) {
            non5x5correctionFile_ = ps.getParameter<edm::FileInPath>( "non5x5correctionFile" );
            TFile* non5x5_f = TFile::Open(non5x5correctionFile_.fullPath().c_str());
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfr9EB"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfsieieEB"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfsipipEB"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfsieipEB"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfr9EE"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfsieieEE"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfsipipEE"))->Clone() );
            non5x5corrections_.emplace_back((TGraph*)((TGraph*) non5x5_f->Get("transfsieipEE"))->Clone() );
            non5x5_f->Close();
        }

        edm::ConsumesCollector sumes(consumesCollector());
        reRunRegression_ = ps.getParameter<bool>("reRunRegression");
        if( reRunRegression_ ) {
            reRunRegressionOnData_ = ps.getParameter<bool>("reRunRegressionOnData");
            // regress_ = new EGExtraInfoModifierFromDB(ps.getParameter<edm::ParameterSet>("regressionConfig"));
            regress_.reset(ModifyObjectValueFactory::get()->create( "EGRegressionModifierV2", ps.getParameter<edm::ParameterSet>("regressionConfig"), sumes )); 
        }

        produces<std::vector<flashgg::DiPhotonCandidate> >();
    }

    void DiPhotonWithUpdatedPhoIdMVAProducer::updatePhotonRegressions(flashgg::DiPhotonCandidate & diph)
    {
            regress_->modifyObject(diph.getLeadingPhoton());
            regress_->modifyObject(diph.getSubLeadingPhoton());
    }

    void DiPhotonWithUpdatedPhoIdMVAProducer::storePhotonRegressions(flashgg::DiPhotonCandidate & diph, const std::string & label)
    {
            storeRegression(diph.getLeadingPhoton(), label);
            storeRegression(diph.getSubLeadingPhoton(), label);
    }

    void DiPhotonWithUpdatedPhoIdMVAProducer::storeRegression(flashgg::Photon & ph, const std::string & label)
    {
        ph.addUserFloat(label + "_regr_E", ph.energyCorrections().regression2Energy);
        ph.addUserFloat(label + "_regr_E_err", ph.energyCorrections().regression2EnergyError);
    }

    float DiPhotonWithUpdatedPhoIdMVAProducer::correctPhoton( flashgg::Photon & ph ) 
    {
        if (this->debug_) {
            std::cout << ph.full5x5_r9() << std::endl;
            std::cout << ph.old_r9() << std::endl;
        }
        size_t corr_index =  ph.isEB() ? 0 : 4;
        reco::Photon::ShowerShape newShowerShapes = ph.full5x5_showerShapeVariables();
        ph.addUserFloat("uncorr_r9",ph.full5x5_r9());
        ph.addUserFloat("uncorr_etaWidth",ph.superCluster()->etaWidth());
        ph.addUserFloat("uncorr_s4",ph.s4());
        ph.addUserFloat("uncorr_sigmaIetaIeta",ph.full5x5_sigmaIetaIeta());

        newShowerShapes.e3x3 = corrections_[corr_index+0]->Eval(ph.full5x5_r9())*ph.superCluster()->rawEnergy();
        newShowerShapes.sigmaIetaIeta = corrections_[corr_index+3]->Eval(ph.full5x5_sigmaIetaIeta());

        float correctedEtaWidth = corrections_[corr_index+1]->Eval(ph.superCluster()->etaWidth());
        ph.getSuperCluster()->setEtaWidth(correctedEtaWidth);
        ph.setS4(corrections_[corr_index+2]->Eval(ph.s4()));

        ph.full5x5_setShowerShapeVariables(newShowerShapes);
        return correctedEtaWidth;
    }

    void DiPhotonWithUpdatedPhoIdMVAProducer::correctPhoton_non5x5( flashgg::Photon & ph ) 
    {
        // if (this->debug_) {
        //     std::cout << ph.full5x5_r9() << std::endl;
        //     std::cout << ph.old_r9() << std::endl;
        // }
        size_t corr_index =  ph.isEB() ? 0 : 4;
        reco::Photon::ShowerShape non5x5ShowerShapes = ph.showerShapeVariables();

        ph.addUserFloat("uncorr_non5x5_r9",ph.old_r9());
        ph.addUserFloat("uncorr_non5x5_sigmaIetaIeta",non5x5ShowerShapes.sigmaIetaIeta);
        ph.addUserFloat("uncorr_non5x5_sigmaIphiIphi",non5x5ShowerShapes.sigmaIphiIphi);
        ph.addUserFloat("uncorr_non5x5_sigmaIetaIphi",non5x5ShowerShapes.sigmaIetaIphi);

        non5x5ShowerShapes.e3x3          = non5x5corrections_[corr_index+0]->Eval(ph.old_r9())*ph.superCluster()->rawEnergy();
        non5x5ShowerShapes.sigmaIetaIeta = non5x5corrections_[corr_index+1]->Eval(non5x5ShowerShapes.sigmaIetaIeta);
        non5x5ShowerShapes.sigmaIphiIphi = non5x5corrections_[corr_index+2]->Eval(non5x5ShowerShapes.sigmaIphiIphi);
        non5x5ShowerShapes.sigmaIetaIphi = non5x5corrections_[corr_index+3]->Eval(non5x5ShowerShapes.sigmaIetaIphi);

        ph.setShowerShapeVariables(non5x5ShowerShapes);
        return;
    }

    void DiPhotonWithUpdatedPhoIdMVAProducer::produce( edm::Event &evt, const edm::EventSetup & es)
    {
        edm::Handle<edm::View<flashgg::DiPhotonCandidate> > objects;
        evt.getByToken( token_, objects );

        edm::Handle<double> rhoHandle;
        evt.getByToken( rhoToken_, rhoHandle );
        const double rhoFixedGrd = *( rhoHandle.product() );

        // For updated photon MVA study 
        // Trying to add supercluster information 
        // particleFlowSuperClusterECAL
        // vector<reco::SuperCluster> 
        
        // edm::Handle<edm::View<vector<reco::SuperCluster>> > SuperClusters;
        // evt.getByToken( SCtoken_, SuperClusters );        

        if( reRunRegression_ ) {
            if( reRunRegressionOnData_ || ! evt.isRealData() ) {
                regress_->setEvent(evt);
                regress_->setEventContent(es);
            }
        }
        
        unique_ptr<std::vector<flashgg::DiPhotonCandidate> > out_obj( new std::vector<flashgg::DiPhotonCandidate>() );

        for (const auto & obj : *objects) {
            flashgg::DiPhotonCandidate *new_obj = obj.clone();
            new_obj->makePhotonsPersistent();
            // store reco energy for safety
            new_obj->getLeadingPhoton().addUserFloat("reco_E", new_obj->getLeadingPhoton().energy());
            new_obj->getSubLeadingPhoton().addUserFloat("reco_E", new_obj->getSubLeadingPhoton().energy());
            // store reco regression
            float leadE = 0., subleadE = 0.;
            if( reRunRegression_ ) {
                if( keepInitialEnergy_ ) {
                    leadE = new_obj->leadingPhoton()->energy();
                    subleadE = new_obj->subLeadingPhoton()->energy();
                }
                storePhotonRegressions(*new_obj, "reco");
                if( reRunRegressionOnData_ || ! evt.isRealData() ) { updatePhotonRegressions(*new_obj); }
                storePhotonRegressions(*new_obj, "beforeShShTransf");
            }
            
            double leadCorrectedEtaWidth = 0., subLeadCorrectedEtaWidth = 0.;
            if (not evt.isRealData() and correctInputs_) { 
                leadCorrectedEtaWidth = correctPhoton(new_obj->getLeadingPhoton());
                subLeadCorrectedEtaWidth = correctPhoton(new_obj->getSubLeadingPhoton());
            }
            if (not evt.isRealData() and doNon5x5transformation_) { 
                correctPhoton_non5x5(new_obj->getLeadingPhoton());
                correctPhoton_non5x5(new_obj->getSubLeadingPhoton());
            }
            if (not evt.isRealData() and  _doIsoCorrection ) {
                float lead_iso = new_obj->getLeadingPhoton().pfPhoIso03();
                float sublead_iso = new_obj->getSubLeadingPhoton().pfPhoIso03();
                float lead_eta = new_obj->getLeadingPhoton().superCluster()->eta();
                float sublead_eta = new_obj->getSubLeadingPhoton().superCluster()->eta();
                if (this->debug_) {
                    std::cout << "Doing Iso correction to lead (sublead) photon with eta,rho,iso: " << lead_eta << ", " << rhoFixedGrd << ", " << lead_iso;
                    std::cout << " (" << sublead_eta << ", " << rhoFixedGrd << ", " << sublead_iso << ")" << std::endl;
                }
                float extra_lead = _isoCorrector->getExtra(fabs(lead_eta),rhoFixedGrd);
                float extra_sublead = _isoCorrector->getExtra(fabs(sublead_eta),rhoFixedGrd);
                new_obj->getLeadingPhoton().setpfPhoIso03(lead_iso+extra_lead);
                new_obj->getSubLeadingPhoton().setpfPhoIso03(sublead_iso+extra_sublead);
                if (this->debug_) {
                    std::cout << " Final iso value for lead (sublead) photon: " << new_obj->getLeadingPhoton().pfPhoIso03() << " (" 
                              << new_obj->getSubLeadingPhoton().pfPhoIso03() << ")" << std::endl;
                }
            }
            double eA_leadPho = _effectiveAreas.getEffectiveArea( abs(new_obj->getLeadingPhoton().superCluster()->eta()) );
            double eA_subLeadPho = _effectiveAreas.getEffectiveArea( abs(new_obj->getSubLeadingPhoton().superCluster()->eta()) );
            
            float leadpfPhoIso03Corr = phoTools_.computeCorrectPhoIso( new_obj->getLeadingPhoton(), rhoFixedGrd,  eA_leadPho, _phoIsoPtScalingCoeff, _phoIsoCutoff);
            new_obj->getLeadingPhoton().setpfPhoIso03Corr(leadpfPhoIso03Corr);
            float subleadpfPhoIso03Corr = phoTools_.computeCorrectPhoIso( new_obj->getSubLeadingPhoton(), rhoFixedGrd,  eA_subLeadPho, _phoIsoPtScalingCoeff, _phoIsoCutoff);
            new_obj->getSubLeadingPhoton().setpfPhoIso03Corr(subleadpfPhoIso03Corr);
            if (this->debug_) {
                std::cout << "Isolation notcorr (corr) for lead, sublead" << new_obj->getLeadingPhoton().pfPhoIso03() << "," << new_obj->getSubLeadingPhoton().pfPhoIso03() << "(" << new_obj->getLeadingPhoton().pfPhoIso03Corr() << "," << new_obj->getSubLeadingPhoton().pfPhoIso03Corr() << ")" << std::endl;
            }
            if (this->debug_) {
                std::cout << " Input DiPhoton lead (sublead) MVA: " << obj.leadPhotonId() << " " << obj.subLeadPhotonId() << std::endl;
            }
            if( reRunRegression_ ) {            
                if( reRunRegressionOnData_ || ! evt.isRealData() ) { updatePhotonRegressions(*new_obj); }
                storePhotonRegressions(*new_obj, "afterShShTransf");
                if( keepInitialEnergy_ ) {
                    new_obj->getLeadingPhoton().setCorrectedEnergy(reco::Photon::P4type::regression2, leadE, new_obj->leadingPhoton()->sigEOverE()*leadE, true);
                    new_obj->getSubLeadingPhoton().setCorrectedEnergy(reco::Photon::P4type::regression2, subleadE, new_obj->subLeadingPhoton()->sigEOverE()*subleadE, true);
                }
            }
            
            
            new_obj->getLeadingPhoton().addUserFloat("etaWidth", leadCorrectedEtaWidth);
            float newleadmva = phoTools_.computeMVAWrtVtx( new_obj->getLeadingPhoton(), new_obj->vtx(), rhoFixedGrd, eA_leadPho, _phoIsoPtScalingCoeff, _phoIsoCutoff );
            new_obj->getLeadingPhoton().setPhoIdMvaWrtVtx( new_obj->vtx(), newleadmva);
            new_obj->getSubLeadingPhoton().addUserFloat("etaWidth", subLeadCorrectedEtaWidth);
            float newsubleadmva = phoTools_.computeMVAWrtVtx( new_obj->getSubLeadingPhoton(), new_obj->vtx(), rhoFixedGrd, eA_subLeadPho, _phoIsoPtScalingCoeff, _phoIsoCutoff );
            new_obj->getSubLeadingPhoton().setPhoIdMvaWrtVtx( new_obj->vtx(), newsubleadmva);
            if (this->debug_) {
                std::cout << " Output DiPhoton lead (sublead) MVA: " << new_obj->leadPhotonId() << " " << new_obj->subLeadPhotonId() << std::endl;
            }
            out_obj->push_back(*new_obj);
            delete new_obj;
        }
        evt.put( std::move(out_obj) );
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
