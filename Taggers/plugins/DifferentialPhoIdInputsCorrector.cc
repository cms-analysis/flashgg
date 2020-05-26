#include "CLHEP/Random/RandomEngine.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

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
    const bool useXGB = false;
    
    class DifferentialPhoIdInputsCorrector : public edm::EDProducer
    {
    public:
        DifferentialPhoIdInputsCorrector( const edm::ParameterSet & );
        void produce( edm::Event &, const edm::EventSetup & ) override;

        void storePhotonRegressions(flashgg::DiPhotonCandidate* dipho, const std::string & label);
        void updatePhotonRegressions(flashgg::DiPhotonCandidate* dipho);

    private:
        void correctPhoton(flashgg::Photon& ph, CLHEP::HepRandomEngine& engine, double rho);
        void storeRegression(flashgg::Photon & cand, const std::string & label);

        //---inputs
        edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diphoToken_;
        edm::EDGetTokenT<double> rhoToken_;

        //---photon id        
        PhotonIdUtils phoTools_;
        ConsumesCollector cc_;
        GlobalVariablesComputer globalVariablesComputer_;
        bool correctShowerShapes_;
        bool correctIsolations_;
        map<string, MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB> > correctionsEB_;
        map<string, TFormula> correctionScalingsEB_;
        map<string, MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB> > correctionsEE_;
        map<string, TFormula> correctionScalingsEE_;
        // static list of variables to be corrected
        static vector<string> showerShapes_;
        std::string weights_;

        EffectiveAreas effectiveAreas_;
        vector<double> phoIsoPtScalingCoeff_;
        double phoIsoCutoff_;

        //---energy regression
        shared_ptr<ModifyObjectValueBase> regress_;
        bool reRunRegression_;
    };
    
    vector<string> DifferentialPhoIdInputsCorrector::showerShapes_ = {"r9", "s4", "sieie", "sieip", "etaWidth", "phiWidth"};

    DifferentialPhoIdInputsCorrector::DifferentialPhoIdInputsCorrector( const edm::ParameterSet &pSet ) :
        diphoToken_(consumes<edm::View<flashgg::DiPhotonCandidate> >(pSet.getParameter<edm::InputTag>("diphotonSrc"))),
        rhoToken_( consumes<double>( pSet.getParameter<edm::InputTag>( "rhoFixedGridCollection" ) ) ),
        cc_( consumesCollector() ),
        globalVariablesComputer_(pSet.getParameter<edm::ParameterSet>("globalVariables"), cc_),
        correctShowerShapes_(pSet.getParameter<bool>("correctShowerShapes")),
        correctIsolations_(pSet.getParameter<bool>("correctIsolations")),
        effectiveAreas_((pSet.getParameter<edm::FileInPath>("effAreasConfigFile")).fullPath()),
        phoIsoPtScalingCoeff_(pSet.getParameter<std::vector<double >>("phoIsoPtScalingCoeff")),
        phoIsoCutoff_(pSet.getParameter<double>("phoIsoCutoff")),
        regress_(0)
    {
        reRunRegression_ = pSet.getParameter<bool>("reRunRegression");
        if( reRunRegression_ ) 
            regress_.reset(ModifyObjectValueFactory::get()->create( "EGRegressionModifierV2", pSet.getParameter<edm::ParameterSet>("regressionConfig"), cc_ )); 

        phoTools_.setupMVA( pSet.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EB" ).fullPath(), 
                            pSet.getParameter<edm::FileInPath>( "photonIdMVAweightfile_EE" ).fullPath(), 
                            true , pSet.getParameter<bool>("is2017") );

        //---Load shower shapes corrections
        if(correctShowerShapes_)
        {
            for(auto& ss_var : showerShapes_)
            {
                //---EB
                auto xgb_config = pSet.getParameter<edm::ParameterSet>(ss_var+"_corrector_config_EB"); 
                correctionsEB_[ss_var] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(xgb_config, &globalVariablesComputer_);
                correctionScalingsEB_[ss_var] = TFormula("", xgb_config.getParameter<string>("regr_output_scaling").c_str());

                //---EE
                xgb_config = pSet.getParameter<edm::ParameterSet>(ss_var+"_corrector_config_EE");
                correctionsEE_[ss_var] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(xgb_config, &globalVariablesComputer_);
                correctionScalingsEE_[ss_var] = TFormula("", xgb_config.getParameter<string>("regr_output_scaling").c_str());
            }


        }

        //---Load isolation corrections
        if(correctIsolations_)
        {
            //---EB
            // pho iso
            auto iso_config = pSet.getParameter<edm::ParameterSet>("phoIso_corrector_config_EB"); 
            correctionsEB_["phoIsoClfData"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("clf_data"), 
                &globalVariablesComputer_);
            correctionsEB_["phoIsoClfMC"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("clf_mc"), 
                &globalVariablesComputer_);
            correctionsEB_["phoIsoPeak2Tail"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("peak2tail"), 
                &globalVariablesComputer_);
            correctionsEB_["phoIsoMorphing"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("morphing"), 
                &globalVariablesComputer_);
            correctionScalingsEB_["phoIsoMorphing"] = TFormula("", iso_config.getParameter<edm::ParameterSet>("morphing").getParameter<string>("regr_output_scaling").c_str());
            // ch iso
            iso_config = pSet.getParameter<edm::ParameterSet>("chIso_corrector_config_EB"); 
            correctionsEB_["chIsoClfData"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("clf_data"), 
                &globalVariablesComputer_);
            correctionsEB_["chIsoClfMC"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("clf_mc"), 
                &globalVariablesComputer_);
            correctionsEB_["chIsoPeak2Tail"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("chIso_peak2tail"), 
                &globalVariablesComputer_);
            correctionsEB_["chIsoMorphing"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("chIso_morphing"), 
                &globalVariablesComputer_);
            correctionScalingsEB_["chIsoMorphing"] = TFormula("", iso_config.getParameter<edm::ParameterSet>("chIso_morphing").getParameter<string>("regr_output_scaling").c_str());            
            correctionsEB_["chIsoWorstPeak2Tail"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("chIsoWorst_peak2tail"), 
                &globalVariablesComputer_);
            correctionsEB_["chIsoWorstMorphing"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("chIsoWorst_morphing"), 
                &globalVariablesComputer_);
            correctionScalingsEB_["chIsoWorstMorphing"] = TFormula("", iso_config.getParameter<edm::ParameterSet>("chIsoWorst_morphing").getParameter<string>("regr_output_scaling").c_str());            

            //---EE
            // pho iso
            iso_config = pSet.getParameter<edm::ParameterSet>("phoIso_corrector_config_EE"); 
            correctionsEE_["phoIsoClfData"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("clf_data"), 
                &globalVariablesComputer_);
            correctionsEE_["phoIsoClfMC"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("clf_mc"), 
                &globalVariablesComputer_);
            correctionsEE_["phoIsoPeak2Tail"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("peak2tail"), 
                &globalVariablesComputer_);
            correctionsEE_["phoIsoMorphing"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("morphing"), 
                &globalVariablesComputer_);
            correctionScalingsEE_["phoIsoMorphing"] = TFormula("", iso_config.getParameter<edm::ParameterSet>("morphing").getParameter<string>("regr_output_scaling").c_str());
            // ch iso
            iso_config = pSet.getParameter<edm::ParameterSet>("chIso_corrector_config_EE"); 
            correctionsEE_["chIsoClfData"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("clf_data"), 
                &globalVariablesComputer_);
            correctionsEE_["chIsoClfMC"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("clf_mc"), 
                &globalVariablesComputer_);
            correctionsEE_["chIsoPeak2Tail"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("chIso_peak2tail"), 
                &globalVariablesComputer_);
            correctionsEE_["chIsoMorphing"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("chIso_morphing"), 
                &globalVariablesComputer_);
            correctionScalingsEE_["chIsoMorphing"] = TFormula("", iso_config.getParameter<edm::ParameterSet>("chIso_morphing").getParameter<string>("regr_output_scaling").c_str());            
            correctionsEE_["chIsoWorstPeak2Tail"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("chIsoWorst_peak2tail"), 
                &globalVariablesComputer_);
            correctionsEE_["chIsoWorstMorphing"] = MVAComputer<Photon, StringObjectFunction<Photon, true>, useXGB>(
                iso_config.getParameter<edm::ParameterSet>("chIsoWorst_morphing"), 
                &globalVariablesComputer_);
            correctionScalingsEE_["chIsoWorstMorphing"] = TFormula("", iso_config.getParameter<edm::ParameterSet>("chIsoWorst_morphing").getParameter<string>("regr_output_scaling").c_str());            
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

    void DifferentialPhoIdInputsCorrector::correctPhoton( flashgg::Photon & pho, CLHEP::HepRandomEngine& engine, double rho) 
    {
        const auto* corrections = std::abs(pho.superCluster()->eta())<1.5 ? &correctionsEB_ : &correctionsEE_;
        const auto* correctionScalings = std::abs(pho.superCluster()->eta())<1.5 ? &correctionScalingsEB_ : &correctionScalingsEE_;

        if(!(std::isnan(pho.full5x5_r9()) || std::isnan(pho.s4()) || std::isnan(pho.full5x5_sigmaIetaIeta()) || std::isnan(pho.sieip()) || std::isnan(pho.superCluster()->etaWidth()) || std::isnan(pho.superCluster()->phiWidth()) || std::isnan(pho.pfPhoIso03()) || std::isnan(pho.pfChgIsoWrtChosenVtx03()) || std::isnan(pho.pfChgIsoWrtWorstVtx03()) || std::isinf(pho.full5x5_r9()) || std::isinf(pho.s4()) || std::isinf(pho.full5x5_sigmaIetaIeta()) || std::isinf(pho.sieip()) || std::isinf(pho.superCluster()->etaWidth()) || std::isinf(pho.superCluster()->phiWidth()) || std::isinf(pho.pfPhoIso03()) || std::isinf(pho.pfChgIsoWrtChosenVtx03()) || std::isinf(pho.pfChgIsoWrtWorstVtx03())))
        {
            if(correctShowerShapes_)
            {
                reco::Photon::ShowerShape correctedShowerShapes = pho.full5x5_showerShapeVariables();
                pho.addUserFloat("uncorr_r9", pho.full5x5_r9());
                pho.addUserFloat("uncorr_s4", pho.s4());
                pho.addUserFloat("uncorr_sieie", pho.full5x5_sigmaIetaIeta());
                pho.addUserFloat("uncorr_sieip", pho.sieip());
                pho.addUserFloat("uncorr_etaWidth", pho.superCluster()->etaWidth());
                pho.addUserFloat("uncorr_phiWidth", pho.superCluster()->phiWidth());
              
                //---Compute corrections
                // R9 (store it inside e3x3)        
                correctedShowerShapes.e3x3 = (pho.full5x5_r9()+correctionScalings->at("r9").Eval(corrections->at("r9")(pho)[0]))*pho.superCluster()->rawEnergy();                            
                //S4
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
                if (pho.full5x5_r9()<0.)
                    {
                        std::cout << "WARNING: R9<0, Original R9: " << pho.userFloat("uncorr_r9") << " corrected R9: " << pho.full5x5_r9() << std::endl;
                    }
            }

            if(correctIsolations_)
            {
                //---Photon isolation
                pho.addUserFloat("uncorr_pfPhoIso03", pho.pfPhoIso03());

                // peak to tail shift (and viceversa), with conversion from TMVA value [-1,1] to probability [0,1]
                auto p_tail_data = 1./(1.+sqrt(2./(1.+corrections->at("phoIsoClfData")(pho)[0])-1.));
                auto p_tail_mc =  1./(1.+sqrt(2./(1.+corrections->at("phoIsoClfMC")(pho)[0])-1.));
                auto p_peak_data = 1 - p_tail_data;
                auto p_peak_mc = 1 - p_tail_mc;
                auto migration_rnd_value = engine.flat();
            
                double p_move_to_tail = (p_tail_data-p_tail_mc)/p_peak_mc;
                double p_move_to_peak = (p_peak_data-p_peak_mc)/p_tail_mc;
            
                if(pho.pfPhoIso03() == 0 && p_tail_data > p_tail_mc && migration_rnd_value < p_move_to_tail)
                {
                    pho.addUserFloat("peak2tail_rnd", engine.flat()*(0.99-0.01)+0.01);
                    pho.setpfPhoIso03(corrections->at("phoIsoPeak2Tail")(pho)[0]);
                }
                else if(pho.pfPhoIso03() > 0 && p_peak_data > p_peak_mc && migration_rnd_value <= p_move_to_peak)
                    pho.setpfPhoIso03(0.);

                // tail morphing
                if(pho.pfPhoIso03() > 0.)
                {
                    pho.setpfPhoIso03(pho.pfPhoIso03()+correctionScalings->at("phoIsoMorphing").Eval(corrections->at("phoIsoMorphing")(pho)[0]));
                }
                //Make sure pfPhoIso03Corr is same as pfPhoIso03
                pho.setpfPhoIso03Corr(pho.pfPhoIso03());

                
                //---Charge isolations
                // ----------------+-------------------------+
                // ChIsoWorst tail | 01         | 11         |
                // ChIsoWorst peak | 00         | X          |
                // ----------------+-------------------------+
                //                 | ChIso peak | ChIso tail |
                //                 +-------------------------+
                // ChIsoWorst < ChIso is impossible given the definition of the two
                pho.addUserFloat("uncorr_pfChIso03", pho.pfChgIsoWrtChosenVtx03());
                pho.addUserFloat("uncorr_pfChIsoWorst03", pho.pfChgIsoWrtWorstVtx03());
            
                auto p_00_data = corrections->at("chIsoClfData")(pho)[0];
                auto p_01_data = corrections->at("chIsoClfData")(pho)[1];
                auto p_11_data = corrections->at("chIsoClfData")(pho)[2];
                auto p_00_mc = corrections->at("chIsoClfMC")(pho)[0];
                auto p_01_mc = corrections->at("chIsoClfMC")(pho)[1];
                auto p_11_mc = corrections->at("chIsoClfMC")(pho)[2];
                migration_rnd_value = engine.flat();

                pho.addUserFloat("peak2tail_chIso_rnd", engine.flat()*(0.99-0.01)+0.01);
                pho.addUserFloat("peak2tail_chIsoWorst_rnd", engine.flat()*(0.99-0.01)+0.01);
            
                //---often used functions
                auto get_w = [](float p_data, float p_mc){ return 1-p_data/p_mc; };
                auto get_z = [](float p_mc_1, float p_data_1, float p_mc_2, float p_data_2){ return (p_data_1-p_mc_1)/(p_mc_2-p_data_2); };                    
            
                // 00
                if(pho.pfChgIsoWrtChosenVtx03() == 0 && pho.pfChgIsoWrtWorstVtx03() == 0 && p_00_mc > p_00_data && migration_rnd_value <= get_w(p_00_data, p_00_mc))
                {
                    // 00->01
                    if(p_01_mc < p_01_data && p_11_mc > p_11_data)
                        pho.setpfChgIsoWrtWorstVtx03(corrections->at("chIsoWorstPeak2Tail")(pho)[0]);
                    // 00->11
                    else if(p_01_mc > p_01_data && p_11_mc < p_11_data)
                    {
                        pho.setpfChgIsoWrtChosenVtx03(corrections->at("chIsoPeak2Tail")(pho)[0]);
                        pho.setpfChgIsoWrtWorstVtx03(corrections->at("chIsoWorstPeak2Tail")(pho)[0]);
                    }
                    // 00->either 
                    else if(p_01_mc < p_01_data && p_11_mc < p_11_data)
                    {
                        migration_rnd_value = engine.flat();
                        if(migration_rnd_value <= get_z(p_01_mc, p_01_data, p_00_mc, p_00_data))
                            pho.setpfChgIsoWrtWorstVtx03(corrections->at("chIsoWorstPeak2Tail")(pho)[0]);
                        else
                        {
                            pho.setpfChgIsoWrtChosenVtx03(corrections->at("chIsoPeak2Tail")(pho)[0]);
                            pho.setpfChgIsoWrtWorstVtx03(corrections->at("chIsoWorstPeak2Tail")(pho)[0]);
                        }                   
                    }
                }
                // 01
                else if(pho.pfChgIsoWrtChosenVtx03() == 0. && pho.pfChgIsoWrtWorstVtx03() > 0. && p_01_mc > p_01_data && migration_rnd_value <= get_w(p_01_data, p_01_mc))
                {
                    // 01->00
                    if(p_00_mc < p_00_data && p_11_mc > p_11_data)
                    {
                        pho.setpfChgIsoWrtChosenVtx03(0.);
                        pho.setpfChgIsoWrtWorstVtx03(0.);
                    }
                    // 01->11
                    else if(p_00_mc>p_00_data && p_11_mc<p_11_data)
                        pho.setpfChgIsoWrtChosenVtx03(corrections->at("chIsoPeak2Tail")(pho)[0]);
                    // 01->either
                    else if(p_00_mc < p_00_data && p_11_mc < p_11_data)
                    {
                        migration_rnd_value = engine.flat();
                        if(migration_rnd_value <= get_z(p_00_mc, p_00_data, p_01_mc, p_01_data))
                            pho.setpfChgIsoWrtWorstVtx03(0.);
                        else
                            pho.setpfChgIsoWrtChosenVtx03(corrections->at("chIsoPeak2Tail")(pho)[0]);
                    }
                }
                // 11
                else if(pho.pfChgIsoWrtChosenVtx03() > 0. && pho.pfChgIsoWrtWorstVtx03() > 0. && p_11_mc > p_11_data && migration_rnd_value <= get_w(p_11_data, p_11_mc))
                {
                    // 11->00
                    if(p_00_mc < p_00_data && p_01_mc > p_01_data)
                    {
                        pho.setpfChgIsoWrtChosenVtx03(0.);
                        pho.setpfChgIsoWrtWorstVtx03(0.);
                    }
                    // 11->01
                    else if(p_00_mc > p_00_data && p_01_mc < p_01_data)
                        pho.setpfChgIsoWrtChosenVtx03(0.);
                    // 11->either
                    else if(p_00_mc < p_00_data && p_01_mc < p_01_data)
                    {
                        migration_rnd_value = engine.flat();
                        if(migration_rnd_value <= get_z(p_00_mc, p_00_data, p_11_mc, p_11_data))
                        {
                            pho.setpfChgIsoWrtChosenVtx03(0.);
                            pho.setpfChgIsoWrtWorstVtx03(0.);
                        }
                        else
                            pho.setpfChgIsoWrtChosenVtx03(0.);
                    }
                }

                // tail morphing
                if(pho.pfChgIsoWrtChosenVtx03() > 0.)
                    pho.setpfChgIsoWrtChosenVtx03(pho.pfChgIsoWrtChosenVtx03()+correctionScalings->at("chIsoMorphing").Eval(corrections->at("chIsoMorphing")(pho)[0]));
                if(pho.pfChgIsoWrtWorstVtx03() > 0.)
                    pho.setpfChgIsoWrtWorstVtx03(pho.pfChgIsoWrtWorstVtx03()+correctionScalings->at("chIsoWorstMorphing").Eval(corrections->at("chIsoWorstMorphing")(pho)[0]));

            }
        }
    }

    void DifferentialPhoIdInputsCorrector::produce( edm::Event &evt, const edm::EventSetup & es)
    {
        globalVariablesComputer_.update(evt);

        edm::Handle<edm::View<flashgg::DiPhotonCandidate> > orig_diphotons;
        evt.getByToken( diphoToken_, orig_diphotons );

        edm::Handle<double> rhoHandle;
        evt.getByToken( rhoToken_, rhoHandle );
        const double rhoFixedGrd = *( rhoHandle.product() );

        edm::Service<edm::RandomNumberGenerator> rng;
        if( ! rng.isAvailable() ) {
            throw cms::Exception( "Configuration" ) << 
                "FlashggDifferentialPhoIdInputsCorrector requires the RandomNumberGeneratorService  - please add to configuration";
        }
        CLHEP::HepRandomEngine& engine = rng->getEngine(evt.streamID());
        
        unique_ptr<std::vector<flashgg::DiPhotonCandidate> > out_diphotons( new std::vector<flashgg::DiPhotonCandidate>() );

        for (const auto & orig_dipho : *orig_diphotons) {
            
            flashgg::DiPhotonCandidate *dipho = orig_dipho.clone();
            dipho->makePhotonsPersistent();

            //---store ChIso wrt chosen vertex into the actual photon objects
            dipho->getLeadingPhoton().setpfChgIsoWrtChosenVtx03(dipho->leadingView()->pfChIso03WrtChosenVtx());
            dipho->getSubLeadingPhoton().setpfChgIsoWrtChosenVtx03(dipho->subLeadingView()->pfChIso03WrtChosenVtx());

            if( !evt.isRealData() ) {
                
                //---compute corrections
                correctPhoton(dipho->getLeadingPhoton(), engine, rhoFixedGrd);
                correctPhoton(dipho->getSubLeadingPhoton(), engine, rhoFixedGrd);

                auto leadChgIsos = dipho->getLeadingPhoton().pfChgIso03();
                leadChgIsos[dipho->vtx()] = dipho->getLeadingPhoton().pfChgIsoWrtChosenVtx03();
                dipho->getLeadingPhoton().setpfChgIso03(leadChgIsos);
                auto subleadChgIsos = dipho->getSubLeadingPhoton().pfChgIso03();
                subleadChgIsos[dipho->vtx()] = dipho->getSubLeadingPhoton().pfChgIsoWrtChosenVtx03();
                dipho->getLeadingPhoton().setpfChgIso03(leadChgIsos);

                if( reRunRegression_ ) {
                    // store original energy 
                    dipho->getLeadingPhoton().addUserFloat("reco_E", dipho->getLeadingPhoton().energy());
                    dipho->getSubLeadingPhoton().addUserFloat("reco_E", dipho->getSubLeadingPhoton().energy());
                    
                    regress_->setEvent(evt);
                    regress_->setEventContent(es);
                    
                    // compute new regression values
                    updatePhotonRegressions(dipho);
                    storePhotonRegressions(dipho, "afterDiffCorr");
                }

            }

            //---Recompute photon id
            dipho->getLeadingPhoton().addUserFloat("original_phoId", dipho->getLeadingPhoton().phoIdMvaDWrtVtx(dipho->vtx()));
            double eA_leadPho = effectiveAreas_.getEffectiveArea( std::abs(dipho->getLeadingPhoton().superCluster()->eta()) );
            dipho->getLeadingPhoton().setPhoIdMvaWrtVtx( dipho->vtx(), phoTools_.computeMVAWrtVtx( dipho->getLeadingPhoton(), dipho->vtx(), rhoFixedGrd, eA_leadPho, phoIsoPtScalingCoeff_, phoIsoCutoff_ ) );

            dipho->getSubLeadingPhoton().addUserFloat("original_phoId", dipho->getSubLeadingPhoton().phoIdMvaDWrtVtx(dipho->vtx()));
            double eA_subLeadPho = effectiveAreas_.getEffectiveArea( std::abs(dipho->getSubLeadingPhoton().superCluster()->eta()) );
            dipho->getSubLeadingPhoton().setPhoIdMvaWrtVtx( dipho->vtx(), phoTools_.computeMVAWrtVtx( dipho->getSubLeadingPhoton(), dipho->vtx(), rhoFixedGrd, eA_subLeadPho, phoIsoPtScalingCoeff_, phoIsoCutoff_ ) );
            
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
