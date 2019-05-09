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
#include "flashgg/MicroAOD/interface/MVAComputer.h"

#include "TFormula.h"

using namespace std;
using namespace edm;
using namespace reco;

namespace flashgg {

    class DifferentialPhoIdInputsCorrector : public edm::EDProducer
    {
    public:
        DifferentialPhoIdInputsCorrector( const edm::ParameterSet & );
        void produce( edm::Event &, const edm::EventSetup & ) override;

        void storePhotonRegressions(flashgg::DiPhotonCandidate* dipho, const std::string & label);
        void updatePhotonRegressions(flashgg::DiPhotonCandidate* dipho);

    private:
        void correctPhoton( flashgg::Photon & ph );
        void correctPhoton_non5x5( flashgg::Photon & ph ) ;
        void storeRegression(flashgg::Photon & cand, const std::string & label);

        //---inputs
        edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diphoToken_;
        edm::EDGetTokenT<double> rhoToken_;

        //---photon id        
        PhotonIdUtils phoTools_;
        ConsumesCollector cc_;
        GlobalVariablesComputer globalVariablesComputer_;
        map<string, MVAComputer<Photon, StringObjectFunction<Photon, true>, true> > correctionsEB_;
        map<string, TFormula> correctionScalingsEB_;
        map<string, MVAComputer<Photon, StringObjectFunction<Photon, true>, true> > correctionsEE_;
        map<string, TFormula> correctionScalingsEE_;
        // static list of variables to be corrected
        static vector<string> showerShapes_;

        //---energy regression
        shared_ptr<ModifyObjectValueBase> regress_;
        bool reRunRegression_;
        bool correctInputs_;        
    };
    
    vector<string> DifferentialPhoIdInputsCorrector::showerShapes_ = {"r9", "s4", "sieie", "sieip", "etaWidth", "phiWidth"};

    DifferentialPhoIdInputsCorrector::DifferentialPhoIdInputsCorrector( const edm::ParameterSet &pSet ) :
        diphoToken_(consumes<edm::View<flashgg::DiPhotonCandidate> >(pSet.getParameter<edm::InputTag>("diphotonSrc"))),
        rhoToken_( consumes<double>( pSet.getParameter<edm::InputTag>( "rhoFixedGridCollection" ) ) ),
        cc_( consumesCollector() ),
        globalVariablesComputer_(pSet.getParameter<edm::ParameterSet>("globalVariables"), cc_),
        regress_(0)
    {
        reRunRegression_ = pSet.getParameter<bool>("reRunRegression");
        if( reRunRegression_ ) 
            regress_.reset(ModifyObjectValueFactory::get()->create( "EGRegressionModifierV2", pSet.getParameter<edm::ParameterSet>("regressionConfig"), cc_ )); 

        for(auto& ss_var : showerShapes_)
        {
            //---EB
            auto xgb_config = pSet.getParameter<edm::ParameterSet>(ss_var+"_corrector_config_EB"); 
            correctionsEB_[ss_var] = MVAComputer<Photon, StringObjectFunction<Photon, true>, true>(xgb_config, &globalVariablesComputer_);
            correctionScalingsEB_[ss_var] = TFormula("", xgb_config.getParameter<string>("regr_output_scaling").c_str());

            //---EE
            xgb_config = pSet.getParameter<edm::ParameterSet>(ss_var+"_corrector_config_EE"); 
            correctionsEE_[ss_var] = MVAComputer<Photon, StringObjectFunction<Photon, true>, true>(xgb_config, &globalVariablesComputer_);
            correctionScalingsEE_[ss_var] = TFormula("", xgb_config.getParameter<string>("regr_output_scaling").c_str());
        }

        produces<std::vector<flashgg::DiPhotonCandidate> >();
    }

    void DifferentialPhoIdInputsCorrector::updatePhotonRegressions(flashgg::DiPhotonCandidate* dipho)
    {
        regress_->modifyObject(dipho->getLeadingPhoton());
        regress_->modifyObject(dipho->getSubLeadingPhoton());
    }

    void DifferentialPhoIdInputsCorrector::storePhotonRegressions(flashgg::DiPhotonCandidate* dipho, const std::string & label)
    {
        storeRegression(dipho->getLeadingPhoton(), label);
        storeRegression(dipho->getSubLeadingPhoton(), label);
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
        pho.addUserFloat("uncorr_s4", pho.s4());
        pho.addUserFloat("uncorr_sieie", pho.full5x5_sigmaIetaIeta());
        pho.addUserFloat("uncorr_sieip", pho.sieip());
        pho.addUserFloat("uncorr_etaWidth", pho.superCluster()->etaWidth());
        pho.addUserFloat("uncorr_phiWidth", pho.superCluster()->phiWidth());

        //---Compute corrections 
        const auto* corrections = std::abs(pho.superCluster()->eta())<1.5 ? &correctionsEB_ : &correctionsEE_;
        const auto* correctionScalings = std::abs(pho.superCluster()->eta())<1.5 ? &correctionScalingsEB_ : &correctionScalingsEE_;
        // R9 (store it inside e3x3)        
        correctedShowerShapes.e3x3 = (pho.full5x5_r9()+correctionScalings->at("r9").Eval(corrections->at("r9")(pho)[0]))*pho.superCluster()->rawEnergy();
        // S4
        auto s4_corr = pho.s4()+correctionScalings->at("s4").Eval(corrections->at("s4")(pho)[0]);
        // SiEiE
        correctedShowerShapes.sigmaIetaIeta = pho.full5x5_sigmaIetaIeta()+correctionScalings->at("sieie").Eval(corrections->at("sieie")(pho)[0]);
        // SiEiP
        auto sieip_corr = pho.sieip()+correctionScalings->at("sieip").Eval(corrections->at("sieip")(pho)[0]);
        // etaWidth
        pho.addUserFloat("etaWidth", (pho.superCluster()->etaWidth()+correctionScalings->at("etaWidth").Eval(corrections->at("etaWidth")(pho)[0])));
        // phiWidth
        pho.addUserFloat("phiWidth", (pho.superCluster()->phiWidth()+correctionScalings->at("phiWidth").Eval(corrections->at("phiWidth")(pho)[0])));
        
        //---set shower shapes
        pho.setS4(s4_corr);
        pho.setSieip(sieip_corr);
        pho.full5x5_setShowerShapeVariables(correctedShowerShapes);        
    }

    void DifferentialPhoIdInputsCorrector::produce( edm::Event &evt, const edm::EventSetup & es)
    {
        globalVariablesComputer_.update(evt);

        edm::Handle<edm::View<flashgg::DiPhotonCandidate> > orig_diphotons;
        evt.getByToken( diphoToken_, orig_diphotons );

        edm::Handle<double> rhoHandle;
        evt.getByToken( rhoToken_, rhoHandle );
        const double rhoFixedGrd = *( rhoHandle.product() );
        
        unique_ptr<std::vector<flashgg::DiPhotonCandidate> > out_diphotons( new std::vector<flashgg::DiPhotonCandidate>() );

        for (const auto & orig_dipho : *orig_diphotons) {
            flashgg::DiPhotonCandidate *dipho = orig_dipho.clone();
            dipho->makePhotonsPersistent();
            
            correctPhoton(dipho->getLeadingPhoton());
            correctPhoton(dipho->getSubLeadingPhoton());

            if( reRunRegression_ && !evt.isRealData() ) {
                // store original energy 
                dipho->getLeadingPhoton().addUserFloat("reco_E", dipho->getLeadingPhoton().energy());
                dipho->getSubLeadingPhoton().addUserFloat("reco_E", dipho->getSubLeadingPhoton().energy());
                
                regress_->setEvent(evt);
                regress_->setEventContent(es);

                // compute new regression values
                updatePhotonRegressions(dipho);
                storePhotonRegressions(dipho, "afterDiffCorr");
                // dipho->getLeadingPhoton().setCorrectedEnergy(reco::Photon::P4type::regression2, leadE, dipho->leadingPhoton()->sigEOverE()*leadE, true);
                // dipho->getSubLeadingPhoton().setCorrectedEnergy(reco::Photon::P4type::regression2, subleadE, dipho->subLeadingPhoton()->sigEOverE()*subleadE, true);
            }

            //---Recompute photon id
            auto phoIdValues = dipho->getLeadingPhoton().phoIdMvaD();
            dipho->getLeadingPhoton().addUserFloat("original_phoId", dipho->getLeadingPhoton().phoIdMvaDWrtVtx(dipho->vtx()));
            //dipho->getLeadingPhoton().setPhoIdMvaWrtVtx( dipho->vtx(), phoTools_.computeMVAWrtVtx( dipho->getLeadingPhoton(), dipho->vtx(), rhoFixedGrd) );

            phoIdValues = dipho->getSubLeadingPhoton().phoIdMvaD();
            dipho->getSubLeadingPhoton().addUserFloat("original_phoId", dipho->getSubLeadingPhoton().phoIdMvaDWrtVtx(dipho->vtx()));
            //dipho->getSubLeadingPhoton().setPhoIdMvaWrtVtx( dipho->vtx(), phoTools_.computeMVAWrtVtx( dipho->getSubLeadingPhoton(), dipho->vtx(), rhoFixedGrd) );

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
