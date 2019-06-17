#include <TSystem.h>
#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"

//For medium working point

BTagCalibrationReader readerMedB(BTagEntry::OP_MEDIUM, "central", {"up", "down"}); //readerMedB.load(calib, BTagEntry::FLAV_B, "comb"); 

BTagCalibrationReader readerMedC(BTagEntry::OP_MEDIUM, "central", {"up", "down"}); //readerMedC.load(calib, BTagEntry::FLAV_C, "comb");   // operating point
                               // "central",               // central sys type
                               // {"up", "down"});        // other systematics type
BTagCalibrationReader readerMedUDSG(BTagEntry::OP_MEDIUM,  "central", {"up", "down"}); //readerMedUDSG.load(calib, BTagEntry::FLAV_UDSG, "comb");  // operating point
                               // "central",               // central sys type
                               // {"up", "down"});        // other systematics type

bool isloaded = false;


namespace flashgg {

    //typedef ObjectWeight<flashgg::Jet, int> BTagWeight;
    class JetBTagWeight: public ObjectSystMethodBinnedByFunctor<flashgg::Jet, int>
    {

    public:
        typedef StringCutObjectSelector<Jet, true> selector_type;

        JetBTagWeight( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv );
        float makeWeight( const flashgg::Jet &y, int syst_shift ) override;
        std::string shiftLabel( int syst_shift ) const override;

    private:
        selector_type overall_range_;
        bool debug_;
        std::string bTag_;
        double bDiscriminator_;
        bool btagSFreshape_;
        BTagCalibration calibReshape_;
    };

    JetBTagWeight::JetBTagWeight( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv ) : 
        ObjectSystMethodBinnedByFunctor( conf, std::forward<edm::ConsumesCollector>(iC), gv  ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),        
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) ),
        bTag_( conf.getParameter<std::string>("bTag") ),
        bDiscriminator_( conf.getParameter<double>("bDiscriminator") ),
        btagSFreshape_ ( conf.getUntrackedParameter<bool>( "btagSFreshape", false ) )
    {


        this->setMakesWeight( true );
    
        // std::string btag_algo = bTag_== "pfDeepJet" ? "DeepJet" : "pfDeepCSV" ? "DeepCSV" : "CSVv2";    //// DeepJet =  DeepFlavour
        std::string btag_algo = bTag_== "pfDeepJet" ? "DeepJet" : "DeepCSV" ;
        calibReshape_ = BTagCalibration(btag_algo, conf.getParameter<edm::FileInPath>("bTagCalibrationFile").fullPath());
    }

    std::string JetBTagWeight::shiftLabel( int syst_value ) const
    {
        std::string result;
        if( syst_value == 0 ) {
            result = Form( "%sCentral", label().c_str() );
        } else if( syst_value > 0 ) {
            result = Form( "%sUp%.2dsigma", label().c_str(), syst_value );
        } else {
            result = Form( "%sDown%.2dsigma", label().c_str(), -1 * syst_value );
        }
        return result;
    }

    float JetBTagWeight::makeWeight( const flashgg::Jet &obj, int syst_shift ) 
    {

        if( this->debug_ ) {
            std::cout<<"In JetBTagProducer "<<std::endl;
        }

        //bool isloaded = false;

        if(!isloaded){

        readerMedB.load(calibReshape_,                // calibration instance
                        BTagEntry::FLAV_B,    // btag flavour
                        "comb");               // measurement type

        readerMedC.load(calibReshape_,                // calibration instance
                        BTagEntry::FLAV_C,    // btag flavour
                        "comb");               // measurement type

        readerMedUDSG.load(calibReshape_,                // calibration instance
                           BTagEntry::FLAV_UDSG,    // btag flavour
                           "incl");               // measurement type
         isloaded = true;
        }

        float theWeight = 1.;
        if( overall_range_( obj ) ) {
            
            if( this->debug_ ) {
                std::cout<<"In JetBTagProducer inside range "<<std::endl;
            }

            float central = 1., errup = 1., errdown = 1.;

            //obtaining efficiencies
            auto val_err = binContents( obj );
            float eff_central = 1.;//, eff_errup = 0., eff_errdown = 0.;
            if( val_err.first.size() == 1 && val_err.second.size() == 1 ) { // symmetric
                eff_central = val_err.first[0];  
                //eff_errup = eff_errdown = val_err.second[0]; 
            } else if ( val_err.first.size() == 1 && val_err.second.size() == 2 ) { // asymmetric
                eff_central = val_err.first[0];
                //eff_errup = val_err.second[0];
                //eff_errdown = val_err.second[1];
            } else {
                throw cms::Exception("BadConfig") << " We do not recognize the bin format or this object is not in any bin";
            }
            
            if( this->debug_ ) {
                std::cout << " In JetBTagWeight after obtaining efficiency : " << shiftLabel( syst_shift ) << ": Object has pt= " << obj.pt() << " eta=" << obj.eta() << " flavour=" << obj.hadronFlavour()
                          << " efficiency of " << eff_central << std::endl;
            }

            //obtaining scale factors

            //https://twiki.cern.ch/twiki/bin/view/CMS/BTagCalibration
            float JetPt = obj.pt();
            float JetEta = obj.eta();
            int JetFlav = obj.hadronFlavour();
            bool JetBTagStatus = false;
            float JetBDiscriminator;
        
            if(bTag_=="pfDeepJet") JetBDiscriminator = obj.bDiscriminator("mini_pfDeepFlavourJetTags:probb")+ obj.bDiscriminator("mini_pfDeepFlavourJetTags:probbb")+ obj.bDiscriminator("mini_pfDeepFlavourJetTags:problepb"); 
            else  JetBDiscriminator = obj.bDiscriminator("pfDeepCSVJetTags:probb")+ obj.bDiscriminator("pfDeepCSVJetTags:probbb"); //JM
            //else JetBDiscriminator = obj.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");

            if(JetBDiscriminator > bDiscriminator_ ) JetBTagStatus = true;


            if( this->debug_ ) {
                std::cout << " In JetBTagWeight before calib reader: " << shiftLabel( syst_shift ) << ": Object has pt= " << obj.pt() << " eta=" << obj.eta() << " flavour=" << obj.hadronFlavour()
                          << " efficiency of " << eff_central << " values for scale factors : "<< JetPt <<" "<< JetEta <<" "<<JetFlav 
                          << " BTag Values : "<< JetBDiscriminator <<" "<< bDiscriminator_<<" "<<JetBTagStatus<<std::endl;
            }

            //get scale factors from calib reader
            double jet_scalefactor = 1.0;
            double jet_scalefactor_up =  1.0;
            double jet_scalefactor_do =  1.0;

            // if(!btagSFreshape_){ //for medium WP

                if(JetFlav == 5){// b jets
                    jet_scalefactor = readerMedB.eval_auto_bounds("central", BTagEntry::FLAV_B, JetEta, JetPt); 
                    jet_scalefactor_up = readerMedB.eval_auto_bounds("up", BTagEntry::FLAV_B, JetEta, JetPt);
                    jet_scalefactor_do = readerMedB.eval_auto_bounds("down", BTagEntry::FLAV_B, JetEta, JetPt);
                } else if(JetFlav == 4){// c jets
                    jet_scalefactor = readerMedC.eval_auto_bounds("central", BTagEntry::FLAV_C, JetEta, JetPt); 
                    jet_scalefactor_up = readerMedC.eval_auto_bounds("up", BTagEntry::FLAV_C, JetEta, JetPt);
                    jet_scalefactor_do = readerMedC.eval_auto_bounds("down", BTagEntry::FLAV_C, JetEta, JetPt);
                } else {// light jets
                    jet_scalefactor = readerMedUDSG.eval_auto_bounds("central", BTagEntry::FLAV_UDSG, JetEta, JetPt); 
                    jet_scalefactor_up =  readerMedUDSG.eval_auto_bounds("up", BTagEntry::FLAV_UDSG, JetEta, JetPt); 
                    jet_scalefactor_do =  readerMedUDSG.eval_auto_bounds("down", BTagEntry::FLAV_UDSG, JetEta, JetPt);
                }
                
                if( this->debug_ ) {
                    std::cout << " In JetBTagWeight after obtaining SF: " << shiftLabel( syst_shift ) << " SF type : " << btagSFreshape_  << ": Object has pt= " << obj.pt() << " eta=" << obj.eta() << " flavour=" << obj.hadronFlavour()
                              << " efficiency of " << eff_central << " scale factors : "<< jet_scalefactor <<" "<< jet_scalefactor_up <<" "<< jet_scalefactor_do << std::endl;
                }
                
                if ( syst_shift < 0 ){
                    jet_scalefactor_do = abs(syst_shift) * (jet_scalefactor_do - jet_scalefactor) + jet_scalefactor;
                }
                if ( syst_shift > 0 ){
                    jet_scalefactor_up = abs(syst_shift) * (jet_scalefactor_up - jet_scalefactor) + jet_scalefactor;
                }
                
                if( this->debug_ ) {
                    std::cout << " In JetBTagWeight : " << shiftLabel( syst_shift ) << " SF type : " << btagSFreshape_ << " : Object has pt= " << obj.pt() << " eta=" << obj.eta() << " flavour=" << obj.hadronFlavour()
                              << " efficiency of " << eff_central << " scale factors : "<< jet_scalefactor <<" "<< jet_scalefactor_up <<" "<< jet_scalefactor_do << std::endl;
                }
                
                //calculating the weights
                //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods#1a_Event_reweighting_using_scale
                double p_mc = 1.0;
                double p_data = 1.0;
                double p_data_down = 1.0;
                double p_data_up = 1.0;
                if(JetBTagStatus){
                    p_mc = eff_central;
                    p_data = jet_scalefactor * eff_central;
                    p_data_down = jet_scalefactor_do * eff_central;
                    p_data_up = jet_scalefactor_up * eff_central;
                } else {
                    p_mc = 1.0 - eff_central;
                    p_data = 1.0 - (jet_scalefactor * eff_central);
                    p_data_down = 1.0 - (jet_scalefactor_do * eff_central);
                    p_data_up = 1.0 - (jet_scalefactor_up * eff_central);
                }
                
                if(fabs(p_mc) > 1e-10){
                    central = p_data / p_mc ;
                    errdown = p_data_down / p_mc ;
                    errup = p_data_up / p_mc ;
                }

            // } else { // Reshaping SFs : https://twiki.cern.ch/twiki/bin/view/CMS/BTagShapeCalibration#Using_csv_files_and_BTagCalibrat
            //     if( JetBDiscriminator < 0.0 ) JetBDiscriminator = -0.05;
            //     if( JetBDiscriminator > 1.0 ) JetBDiscriminator = 1.0;

            //     if(JetFlav == 5){// b jets
            //         jet_scalefactor = readerShapeB.eval_auto_bounds("central", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator); 
            //         jet_scalefactor_up = 
            //             readerShapeB.eval_auto_bounds("up_jes", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeB.eval_auto_bounds("up_lf", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeB.eval_auto_bounds("up_hfstats1", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeB.eval_auto_bounds("up_hfstats2", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator);
            //         jet_scalefactor_do = 
            //             readerShapeB.eval_auto_bounds("down_jes", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeB.eval_auto_bounds("down_lf", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeB.eval_auto_bounds("down_hfstats1", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeB.eval_auto_bounds("down_hfstats2", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator);
            //     } else if(JetFlav == 4){// c jets
            //         jet_scalefactor = readerShapeC.eval_auto_bounds("central", BTagEntry::FLAV_C, JetEta, JetPt, JetBDiscriminator); 
            //         jet_scalefactor_up = 
            //             readerShapeC.eval_auto_bounds("up_jes", BTagEntry::FLAV_C, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeC.eval_auto_bounds("up_cferr1", BTagEntry::FLAV_C, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeC.eval_auto_bounds("up_cferr2", BTagEntry::FLAV_C, JetEta, JetPt, JetBDiscriminator);
            //         jet_scalefactor_do =  
            //             readerShapeC.eval_auto_bounds("down_jes", BTagEntry::FLAV_C, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeC.eval_auto_bounds("down_cferr1", BTagEntry::FLAV_C, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeC.eval_auto_bounds("down_cferr2", BTagEntry::FLAV_C, JetEta, JetPt, JetBDiscriminator);
            //     } else {// light jets
            //         jet_scalefactor = readerShapeUDSG.eval_auto_bounds("central", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator); 
            //         jet_scalefactor_up =  
            //             readerShapeUDSG.eval_auto_bounds("up_jes", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeUDSG.eval_auto_bounds("up_hf", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeUDSG.eval_auto_bounds("up_lfstats1", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeUDSG.eval_auto_bounds("up_lfstats2", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator) ; 
            //         jet_scalefactor_do =   
            //             readerShapeUDSG.eval_auto_bounds("down_jes", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeUDSG.eval_auto_bounds("down_hf", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeUDSG.eval_auto_bounds("down_lfstats1", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator)
            //             * readerShapeUDSG.eval_auto_bounds("down_lfstats2", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator) ; 
            //     }
                
            //     if( this->debug_ ) {
            //         std::cout << " In JetBTagWeight after obtaining SF: " << shiftLabel( syst_shift ) << " SF type : " << btagSFreshape_  << ": Object has pt= " << obj.pt() << " eta=" << obj.eta() << " flavour=" << obj.hadronFlavour()
            //                   << " efficiency of " << eff_central << " scale factors : "<< jet_scalefactor <<" "<< jet_scalefactor_up <<" "<< jet_scalefactor_do << std::endl;
            //     }
                
            //     if ( syst_shift < 0 ){
            //         jet_scalefactor_do = abs(syst_shift) * (jet_scalefactor_do - jet_scalefactor) + jet_scalefactor;
            //     }
            //     if ( syst_shift > 0 ){
            //         jet_scalefactor_up = abs(syst_shift) * (jet_scalefactor_up - jet_scalefactor) + jet_scalefactor;
            //     }
                
            //     if( this->debug_ ) {
            //         std::cout << " In JetBTagWeight : " << shiftLabel( syst_shift ) << " SF type : " << btagSFreshape_ << " : Object has pt= " << obj.pt() << " eta=" << obj.eta() << " flavour=" << obj.hadronFlavour()
            //                   << " efficiency of " << eff_central << " scale factors : "<< jet_scalefactor <<" "<< jet_scalefactor_up <<" "<< jet_scalefactor_do << std::endl;
            //     }
               
            //     central = jet_scalefactor ;
            //     errdown = jet_scalefactor_do ;
            //     errup = jet_scalefactor_up ;
                

            // } // Reshaping SFs
         

            theWeight = central;
            if ( syst_shift < 0 ) theWeight = errdown;
            if ( syst_shift > 0 ) theWeight = errup;

            if( this->debug_ ) {
                std::cout << " In JetBTagWeight : " << shiftLabel( syst_shift ) << " SF type : " << btagSFreshape_ <<  " : Object has pt= " << obj.pt() << " eta=" << obj.eta() << " flavour=" << obj.hadronFlavour()
                          << " and we apply a weight of " << theWeight << std::endl;
            }
        }
        return theWeight;
    }

}

DEFINE_EDM_PLUGIN( FlashggSystematicJetMethodsFactory,
                   flashgg::JetBTagWeight,
                   "FlashggJetBTagWeight" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
