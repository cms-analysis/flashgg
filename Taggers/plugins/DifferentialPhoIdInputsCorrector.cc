#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/CandAlgos/interface/ModifyObjectValueBase.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"

#include "XGBoostCMSSW/XGBoostInterface/interface/XGBComputer.h"

#include "TFile.h"
#include "TGraph.h"

using namespace std;
using namespace edm;
using namespace reco;

namespace flashgg {

    class DifferentialPhoIdInputsCorrector : public edm::EDProducer
    {
    public:
        DifferentialPhoIdInputsCorrector( const edm::ParameterSet & );
        void produce( edm::Event &, const edm::EventSetup & ) override;

        void storePhotonRegressions(flashgg::DiPhotonCandidate & diph, const std::string & label);
        void updatePhotonRegressions(flashgg::DiPhotonCandidate & diph);

    private:
        void correctPhoton( flashgg::Photon & ph );
        void correctPhoton_non5x5( flashgg::Photon & ph ) ;
        void storeRegression(flashgg::Photon & cand, const std::string & label);

        //---inputs
        edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diphoToken_;
        edm::EDGetTokenT<double> rhoToken_;

        //---photon id        
        PhotonIdUtils phoTools_;
        //vector<StringObjectFunction<flashgg::Photon, true> > showerShapes_;
        XGBComputer::mva_variables correctionVars_;
        map<string, XGBComputer> corrections_;

        //---energy regression
        shared_ptr<ModifyObjectValueBase> regress_;
        bool reRunRegression_;
        bool correctInputs_;
    };

    DifferentialPhoIdInputsCorrector::DifferentialPhoIdInputsCorrector( const edm::ParameterSet &pSet ) :
        diphoToken_(consumes<edm::View<flashgg::DiPhotonCandidate> >(pSet.getParameter<edm::InputTag>("diphotonSrc"))),
        rhoToken_( consumes<double>( pSet.getParameter<edm::InputTag>( "rhoFixedGridCollection" ) ) ),
        regress_(0)
    {
        edm::ConsumesCollector cc(consumesCollector());
        reRunRegression_ = pSet.getParameter<bool>("reRunRegression");
        if( reRunRegression_ ) 
            regress_.reset(ModifyObjectValueFactory::get()->create( "EGRegressionModifierV2", pSet.getParameter<edm::ParameterSet>("regressionConfig"), cc )); 
        
        corrections_["R9"] = XGBComputer(&correctionVars_, pSet.getParameter<edm::FileInPath>("r9_corrector_weights").fullPath());

        produces<std::vector<flashgg::DiPhotonCandidate> >();
    }

    void DifferentialPhoIdInputsCorrector::updatePhotonRegressions(flashgg::DiPhotonCandidate & dipho)
    {
        regress_->modifyObject(dipho.getLeadingPhoton());
        regress_->modifyObject(dipho.getSubLeadingPhoton());
    }

    void DifferentialPhoIdInputsCorrector::storePhotonRegressions(flashgg::DiPhotonCandidate & dipho, const std::string & label)
    {
        storeRegression(dipho.getLeadingPhoton(), label);
        storeRegression(dipho.getSubLeadingPhoton(), label);
    }

    void DifferentialPhoIdInputsCorrector::storeRegression(flashgg::Photon & ph, const std::string & label)
    {
        ph.addUserFloat(label + "_regr_E", ph.energyCorrections().regression2Energy);
        ph.addUserFloat(label + "_regr_E_err", ph.energyCorrections().regression2EnergyError);
    }

    void DifferentialPhoIdInputsCorrector::correctPhoton( flashgg::Photon & pho ) 
    {
        reco::Photon::ShowerShape correctedShowerShapes = pho.full5x5_showerShapeVariables();
        pho.addUserFloat("uncorr_r9", pho.full5x5_r9());
        pho.addUserFloat("uncorr_etaWidth", pho.superCluster()->etaWidth());
        pho.addUserFloat("uncorr_etaWidth", pho.superCluster()->etaWidth());
        pho.addUserFloat("uncorr_s4", pho.s4());
        pho.addUserFloat("uncorr_sigmaIetaIeta", pho.full5x5_sigmaIetaIeta());

        //---Compute corrections
        // R9 (store it inside e3x3)
        //correctedShowerShapes.e3x3 = corrections_["R9"]()*pho.superCluster()->rawEnergy();
        // SiEiE
        //correctedShowerShapes.sigmaIetaIeta = corrections_["sieie"]();
        
    }

    void DifferentialPhoIdInputsCorrector::produce( edm::Event &evt, const edm::EventSetup & es)
    {
        edm::Handle<edm::View<flashgg::DiPhotonCandidate> > orig_diphotons;
        evt.getByToken( diphoToken_, orig_diphotons );

        edm::Handle<double> rhoHandle;
        evt.getByToken( rhoToken_, rhoHandle );
        const double rhoFixedGrd = *( rhoHandle.product() );
        
        unique_ptr<std::vector<flashgg::DiPhotonCandidate> > out_diphotons( new std::vector<flashgg::DiPhotonCandidate>() );

        for (const auto & orig_dipho : *orig_diphotons) {
            flashgg::DiPhotonCandidate *dipho = orig_dipho.clone();
            dipho->makePhotonsPersistent();
            // store original energy 
            dipho->getLeadingPhoton().addUserFloat("reco_E", dipho->getLeadingPhoton().energy());
            dipho->getSubLeadingPhoton().addUserFloat("reco_E", dipho->getSubLeadingPhoton().energy());

            // store reco regression
            // float leadE = 0., subleadE = 0.;
            // if( reRunRegression_ ) {
            //     if( keepInitialEnergy_ ) {
            //         leadE = dipho->leadingPhoton()->energy();
            //         subleadE = dipho->subLeadingPhoton()->energy();
            //     }
            //     storePhotonRegressions(*dipho, "reco");
            //     if( reRunRegressionOnData_ || ! evt.isRealData() ) { updatePhotonRegressions(*dipho); }
            //     storePhotonRegressions(*dipho, "beforeShShTransf");
            // }
            
            correctPhoton(dipho->getLeadingPhoton());
            correctPhoton(dipho->getSubLeadingPhoton());

            if( reRunRegression_ ) {            
                storePhotonRegressions(*dipho, "afterShShTransf");
                // dipho->getLeadingPhoton().setCorrectedEnergy(reco::Photon::P4type::regression2, leadE, dipho->leadingPhoton()->sigEOverE()*leadE, true);
                // dipho->getSubLeadingPhoton().setCorrectedEnergy(reco::Photon::P4type::regression2, subleadE, dipho->subLeadingPhoton()->sigEOverE()*subleadE, true);
            }
            
            // float newleadmva = phoTools_.computeMVAWrtVtx( dipho->getLeadingPhoton(), dipho->vtx(), rhoFixedGrd, leadCorrectedEtaWidth, eA_leadPho, _phoIsoPtScalingCoeff, _phoIsoCutoff );
            // dipho->getLeadingPhoton().setPhoIdMvaWrtVtx( dipho->vtx(), newleadmva);
            // float newsubleadmva = phoTools_.computeMVAWrtVtx( dipho->getSubLeadingPhoton(), dipho->vtx(), rhoFixedGrd, subLeadCorrectedEtaWidth,eA_subLeadPho, _phoIsoPtScalingCoeff, _phoIsoCutoff );
            // dipho->getSubLeadingPhoton().setPhoIdMvaWrtVtx( dipho->vtx(), newsubleadmva);
            
            out_diphotons->push_back(*dipho);
            delete dipho;
        }



        evt.put( std::move(out_diphotons) );
    }
}
            
typedef flashgg::DifferentialPhoIdInputsCorrector FlashggDifferentialPhoIdInputsCorrector;
DEFINE_FWK_MODULE( FlashggDifferentialPhoIdInputsCorrector );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
