#include <TSystem.h>
#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"

//For reshaping BTag shape

BTagCalibrationReader readerShapeB(BTagEntry::OP_RESHAPING,  // operating point
                                   "central",               // central sys type
                                   {"up_jes", "down_jes", "up_lf", "down_lf", "up_hfstats1", "down_hfstats1", "up_hfstats2", "down_hfstats2"});        // other systematics type

BTagCalibrationReader readerShapeC(BTagEntry::OP_RESHAPING,  // operating point
                                   "central",               // central sys type
                                   {"up_cferr1", "down_cferr1", "up_cferr2", "down_cferr2"});        // other systematics type

BTagCalibrationReader readerShapeUDSG(BTagEntry::OP_RESHAPING,  // operating point
                                      "central",               // central sys type
                                      {"up_jes", "down_jes", "up_hf", "down_hf", "up_lfstats1", "down_lfstats1", "up_lfstats2", "down_lfstats2"});        // other systematics type

bool isloadedReshape = false;

namespace flashgg {

    //typedef ObjectWeight<flashgg::Jet, int> BTagWeight;
    class JetBTagReshapeWeight: public ObjectSystMethodBinnedByFunctor<flashgg::Jet, int>
    {

    public:
        typedef StringCutObjectSelector<Jet, true> selector_type;

        JetBTagReshapeWeight( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv );
        float makeWeight( const flashgg::Jet &y, int syst_shift ) override;
        std::string shiftLabel( int syst_shift ) const override;

    private:
        selector_type overall_range_;
        bool debug_;
        std::string bTag_;
        bool btagSFreshape_;
        int bTagReshapeSystOption_;
        BTagCalibration calibReshape_;
    };

    JetBTagReshapeWeight::JetBTagReshapeWeight( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv ) : 
        ObjectSystMethodBinnedByFunctor( conf, std::forward<edm::ConsumesCollector>(iC), gv  ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),        
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) ),
        bTag_( conf.getParameter<std::string>("bTag") ),

        btagSFreshape_ ( conf.getUntrackedParameter<bool>( "btagSFreshape", false ) )
    {
        this->setMakesWeight( true );
        bTagReshapeSystOption_ = conf.getParameter<int>( "bTagReshapeSystOption"); 
        // std::string btag_algo = bTag_=="pfDeepJet" ? "DeepJet" : "pfDeepCSV" ? "DeepCSV" : "CSVv2";  //// DeepJet =  DeepFlavour
        std::string btag_algo = bTag_=="pfDeepJet" ? "DeepJet" : "DeepCSV" ;
        calibReshape_ = BTagCalibration(btag_algo, conf.getParameter<edm::FileInPath>("bTagCalibrationFile").fullPath());
    }

    std::string JetBTagReshapeWeight::shiftLabel( int syst_value ) const
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

    float JetBTagReshapeWeight::makeWeight( const flashgg::Jet &obj, int syst_shift ) 
    {

        if( this->debug_ ) {
            std::cout<<"In JetBTagReshapeProducer and syst_shift="<<  syst_shift <<std::endl;
        }


        if(!isloadedReshape){
        readerShapeB.load(calibReshape_,                // calibration instance
                          BTagEntry::FLAV_B,    // btag flavour
                          "iterativefit");               // measurement type

        readerShapeC.load(calibReshape_,                // calibration instance
                         BTagEntry::FLAV_C,    // btag flavour
                         "iterativefit");               // measurement type
        
        readerShapeUDSG.load(calibReshape_,                // calibration instance
                             BTagEntry::FLAV_UDSG,    // btag flavour
                             "iterativefit");               // measurement type
        isloadedReshape = true;
        }

        float theWeight = 1.;

        if( overall_range_( obj ) ) {
            
            if( this->debug_ ) {
                std::cout<<"In JetBTagReshapeProducer inside range "<<std::endl;
            }

            float central = 1., errup = 1., errdown = 1.;


            //obtaining scale factors

            //https://twiki.cern.ch/twiki/bin/view/CMS/BTagCalibration

            float JetPt = obj.pt();
            float JetEta = fabs(obj.eta());
            int JetFlav = obj.hadronFlavour();
            float JetBDiscriminator;

            if(bTag_=="pfDeepJet") JetBDiscriminator = obj.bDiscriminator("mini_pfDeepFlavourJetTags:probb")+ obj.bDiscriminator("mini_pfDeepFlavourJetTags:probbb")+ obj.bDiscriminator("mini_pfDeepFlavourJetTags:problepb"); 
            else JetBDiscriminator = obj.bDiscriminator("pfDeepCSVJetTags:probb")+ obj.bDiscriminator("pfDeepCSVJetTags:probbb"); //JM
            //   else JetBDiscriminator= obj.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");

            if( this->debug_ ) {
                std::cout << " In JetBTagReshapeWeight before calib reader: " << shiftLabel( syst_shift ) << ": Object has pt= " << obj.pt() << " eta=" << obj.eta() << " flavour=" << obj.hadronFlavour()
                          << " values for scale factors : "<< JetPt <<" "<< JetEta <<" "<<JetFlav 
                          << " b-tagger = " << bTag_<< " BTag Values : "<< JetBDiscriminator <<endl;
            }

            //get scale factors from calib reader
            double jet_scalefactor = 1.0;
            double jet_scalefactor_up =  1.0;
            double jet_scalefactor_do =  1.0;
            
            if( JetBDiscriminator < 0.0 ) JetBDiscriminator = -0.05;
            if( JetBDiscriminator > 1.0 ) JetBDiscriminator = 1.0;

            //for the scale factor up / down variation : have to take each source one ata a time
            
            if(JetFlav == 5){// b jets
                jet_scalefactor = readerShapeB.eval_auto_bounds("central", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator);  
                
                jet_scalefactor_up = jet_scalefactor;
                jet_scalefactor_do = jet_scalefactor;
                if(bTagReshapeSystOption_ == 1){
                    jet_scalefactor_up = readerShapeB.eval_auto_bounds("up_jes", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator);
                    jet_scalefactor_do = readerShapeB.eval_auto_bounds("down_jes", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator);
                    if( this->debug_ )  { std::cout << " In JetBTagReshapeWeight Systematics : "<<" jes : "<<jet_scalefactor<<" "<<jet_scalefactor_up<<" "<<jet_scalefactor_do<<std::endl; }
                } 
                if(bTagReshapeSystOption_ == 2){
                    jet_scalefactor_up = readerShapeB.eval_auto_bounds("up_lf", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator);
                    jet_scalefactor_do = readerShapeB.eval_auto_bounds("down_lf", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator);
                    if( this->debug_ )  { std::cout << " In JetBTagReshapeWeight Systematics : "<<" lf : "<<jet_scalefactor<<" "<<jet_scalefactor_up<<" "<<jet_scalefactor_do<<std::endl; }
                }
                if(bTagReshapeSystOption_ == 3){
                    jet_scalefactor_up = readerShapeB.eval_auto_bounds("up_hfstats1", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator);
                    jet_scalefactor_do = readerShapeB.eval_auto_bounds("down_hfstats1", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator);
                    if( this->debug_ )  { std::cout << " In JetBTagReshapeWeight Systematics : "<<" hfstats1 : "<<jet_scalefactor<<" "<<jet_scalefactor_up<<" "<<jet_scalefactor_do<<std::endl; }
                }
                if(bTagReshapeSystOption_ == 4){
                    jet_scalefactor_up = readerShapeB.eval_auto_bounds("up_hfstats2", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator);
                    jet_scalefactor_do = readerShapeB.eval_auto_bounds("down_hfstats2", BTagEntry::FLAV_B, JetEta, JetPt, JetBDiscriminator);
                    if( this->debug_ )  { std::cout << " In JetBTagReshapeWeight Systematics : "<<" hfstats2 : "<<jet_scalefactor<<" "<<jet_scalefactor_up<<" "<<jet_scalefactor_do<<std::endl; }
                }

            } else if(JetFlav == 4){// c jets
                jet_scalefactor = readerShapeC.eval_auto_bounds("central", BTagEntry::FLAV_C, JetEta, JetPt, JetBDiscriminator); 
                
                jet_scalefactor_up = jet_scalefactor;
                jet_scalefactor_do = jet_scalefactor;
                if(bTagReshapeSystOption_ == 5){
                    jet_scalefactor_up = readerShapeC.eval_auto_bounds("up_cferr1", BTagEntry::FLAV_C, JetEta, JetPt, JetBDiscriminator);
                    jet_scalefactor_do = readerShapeC.eval_auto_bounds("down_cferr1", BTagEntry::FLAV_C, JetEta, JetPt, JetBDiscriminator);
                    if( this->debug_ )  { std::cout << " In JetBTagReshapeWeight Systematics : "<<" cferr1 : "<<jet_scalefactor<<" "<<jet_scalefactor_up<<" "<<jet_scalefactor_do<<std::endl; }
                }
                if(bTagReshapeSystOption_ == 6){
                    jet_scalefactor_up = readerShapeC.eval_auto_bounds("up_cferr2", BTagEntry::FLAV_C, JetEta, JetPt, JetBDiscriminator);
                    jet_scalefactor_do = readerShapeC.eval_auto_bounds("down_cferr2", BTagEntry::FLAV_C, JetEta, JetPt, JetBDiscriminator);
                    if( this->debug_ )  { std::cout << " In JetBTagReshapeWeight Systematics : "<<" cferr2 : "<<jet_scalefactor<<" "<<jet_scalefactor_up<<" "<<jet_scalefactor_do<<std::endl; }
                }

            } else {// light jets
                jet_scalefactor = readerShapeUDSG.eval_auto_bounds("central", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator); 
                
                jet_scalefactor_up = jet_scalefactor;
                jet_scalefactor_do = jet_scalefactor;
                if(bTagReshapeSystOption_ == 1){
                    jet_scalefactor_up = readerShapeUDSG.eval_auto_bounds("up_jes", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator);
                    jet_scalefactor_do = readerShapeUDSG.eval_auto_bounds("down_jes", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator);
                    if( this->debug_ )  { std::cout << " In JetBTagReshapeWeight Systematics : "<<" jes : "<<jet_scalefactor<<" "<<jet_scalefactor_up<<" "<<jet_scalefactor_do<<std::endl; }
                } 
                if(bTagReshapeSystOption_ == 7){
                    jet_scalefactor_up = readerShapeUDSG.eval_auto_bounds("up_hf", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator);
                    jet_scalefactor_do = readerShapeUDSG.eval_auto_bounds("down_hf", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator);
                    if( this->debug_ )  { std::cout << " In JetBTagReshapeWeight Systematics : "<<" hf : "<<jet_scalefactor<<" "<<jet_scalefactor_up<<" "<<jet_scalefactor_do<<std::endl; }
                } 
                if(bTagReshapeSystOption_ == 8){
                    jet_scalefactor_up = readerShapeUDSG.eval_auto_bounds("up_lfstats1", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator);
                    jet_scalefactor_do = readerShapeUDSG.eval_auto_bounds("down_lfstats1", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator);
                    if( this->debug_ )  { std::cout << " In JetBTagReshapeWeight Systematics : "<<" lfstats1 : "<<jet_scalefactor<<" "<<jet_scalefactor_up<<" "<<jet_scalefactor_do<<std::endl; }
                } 
                if(bTagReshapeSystOption_ == 9){
                    jet_scalefactor_up = readerShapeUDSG.eval_auto_bounds("up_lfstats2", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator);
                    jet_scalefactor_do = readerShapeUDSG.eval_auto_bounds("down_lfstats2", BTagEntry::FLAV_UDSG, JetEta, JetPt, JetBDiscriminator);
                    if( this->debug_ )  { std::cout << " In JetBTagReshapeWeight Systematics : "<<" lfstats1 : "<<jet_scalefactor<<" "<<jet_scalefactor_up<<" "<<jet_scalefactor_do<<std::endl; }
                }
            }
            
            if( this->debug_ ) {
                std::cout << " In JetBTagReshapeWeight after obtaining SF: " << shiftLabel( syst_shift ) << " SF type : " << btagSFreshape_  << ": Object has pt= " << obj.pt() << " eta=" << obj.eta() << " flavour=" << obj.hadronFlavour()
                           << " scale factors : "<< jet_scalefactor <<" "<< jet_scalefactor_up <<" "<< jet_scalefactor_do << std::endl;
            }
            
            if ( syst_shift < 0 ){
                jet_scalefactor_do = abs(syst_shift) * (jet_scalefactor_do - jet_scalefactor) + jet_scalefactor;
            }
            if ( syst_shift > 0 ){
                jet_scalefactor_up = abs(syst_shift) * (jet_scalefactor_up - jet_scalefactor) + jet_scalefactor;
            }
            
            if( this->debug_ ) {
                std::cout << " In JetBTagReshapeWeight : " << shiftLabel( syst_shift ) << " SF type : " << btagSFreshape_ << " : Object has pt= " << obj.pt() << " eta=" << obj.eta() << " flavour=" << obj.hadronFlavour()
                          << " scale factors : "<< jet_scalefactor <<" "<< jet_scalefactor_up <<" "<< jet_scalefactor_do << std::endl;
            }
            
            central = jet_scalefactor ;
            errdown = jet_scalefactor_do ;
            errup = jet_scalefactor_up ;
                        
            theWeight = central;
            if ( syst_shift < 0 ) theWeight = errdown;
            if ( syst_shift > 0 ) theWeight = errup;
            
            if( this->debug_ ) {
                std::cout << " In JetBTagReshapeWeight : " << shiftLabel( syst_shift ) << " SF type : " << btagSFreshape_ <<  " : Object has pt= " << obj.pt() << " eta=" << obj.eta() << " flavour=" << obj.hadronFlavour()
                          << " and we apply a weight of " << theWeight << std::endl;
            }
        }
        return theWeight;
    }

}

DEFINE_EDM_PLUGIN( FlashggSystematicJetMethodsFactory,
                   flashgg::JetBTagReshapeWeight,
                   "FlashggJetBTagReshapeWeight" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
