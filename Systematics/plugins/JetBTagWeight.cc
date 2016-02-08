#include <TSystem.h>
#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "flashgg/Systematics/interface/BTagCalibrationStandalone.h"

// setup calibration readers
std::string CMSSW_BASE(getenv("CMSSW_BASE"));
std::string CSVfilename = CMSSW_BASE + std::string("/src/flashgg/Systematics/data/CSVv2.csv");
BTagCalibration calib("CSVv2", CSVfilename);

BTagCalibrationReader reader(&calib,               // calibration instance
                             BTagEntry::OP_MEDIUM,  // operating point
                             "comb",               // measurement type
                             "central");           // systematics type
BTagCalibrationReader reader_up(&calib, BTagEntry::OP_MEDIUM, "comb", "up");  // sys up
BTagCalibrationReader reader_do(&calib, BTagEntry::OP_MEDIUM, "comb", "down");  // sys down

namespace flashgg {

    //typedef ObjectWeight<flashgg::Jet, int> BTagWeight;
    class JetBTagWeight: public ObjectSystMethodBinnedByFunctor<flashgg::Jet, int>
    {

    public:
        typedef StringCutObjectSelector<Jet, true> selector_type;

        JetBTagWeight( const edm::ParameterSet &conf );
        float makeWeight( const flashgg::Jet &y, int syst_shift ) override;
        std::string shiftLabel( int syst_shift ) const override;

    private:
        selector_type overall_range_;
        bool debug_;
        std::string bTag_;
        double bDiscriminator_;
    };

    JetBTagWeight::JetBTagWeight( const edm::ParameterSet &conf ) : 
        ObjectSystMethodBinnedByFunctor( conf ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),        
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) ),
        bTag_( conf.getParameter<std::string>("bTag") ),
        bDiscriminator_( conf.getParameter<double>("bDiscriminator") )
    {
        this->setMakesWeight( true );
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

        float theWeight = 1.;
        if( overall_range_( obj ) ) {
            
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
            
            //obtaining scale factors

            //https://twiki.cern.ch/twiki/bin/view/CMS/BTagCalibration
            float MaxBJetPt = 670., MaxLJetPt = 1000.;
            float JetPt = obj.pt();
            float JetEta = obj.eta();
            int JetFlav = obj.hadronFlavour();
            bool JetBTagStatus = false;
            if(obj.bDiscriminator(bTag_.c_str()) > bDiscriminator_ ) JetBTagStatus = true;
            bool DoubleUncertainty = false;

            if(JetFlav == 5 || JetFlav == 4){ // for b and c jets
                if(JetPt>MaxBJetPt)  {
                    JetPt = MaxBJetPt; 
                    DoubleUncertainty = true;
                }  
            } else { // for light jets
                if(JetPt>MaxLJetPt)  {
                    JetPt = MaxLJetPt; 
                    DoubleUncertainty = true;
                } 
            }

            //get scale factors from calib reader
            double jet_scalefactor = 1.0;
            double jet_scalefactor_up =  1.0;
            double jet_scalefactor_do =  1.0;
            if(JetFlav == 5){// b jets
                jet_scalefactor = reader.eval(BTagEntry::FLAV_B, JetEta, JetPt); 
                jet_scalefactor_up =  reader_up.eval(BTagEntry::FLAV_B, JetEta, JetPt); 
                jet_scalefactor_do =  reader_do.eval(BTagEntry::FLAV_B, JetEta, JetPt);
            } else if(JetFlav == 4){// c jets
                jet_scalefactor = reader.eval(BTagEntry::FLAV_C, JetEta, JetPt); 
                jet_scalefactor_up =  reader_up.eval(BTagEntry::FLAV_C, JetEta, JetPt); 
                jet_scalefactor_do =  reader_do.eval(BTagEntry::FLAV_C, JetEta, JetPt);
            } else {// light jets
                jet_scalefactor = reader.eval(BTagEntry::FLAV_UDSG, JetEta, JetPt); 
                jet_scalefactor_up =  reader_up.eval(BTagEntry::FLAV_UDSG, JetEta, JetPt); 
                jet_scalefactor_do =  reader_do.eval(BTagEntry::FLAV_UDSG, JetEta, JetPt);
            }

            if (DoubleUncertainty) {
                jet_scalefactor_up = 2*(jet_scalefactor_up - jet_scalefactor) + jet_scalefactor; 
                jet_scalefactor_do = 2*(jet_scalefactor_do - jet_scalefactor) + jet_scalefactor; 
            }
           
            if ( syst_shift < 0 ){
                jet_scalefactor_do = abs(syst_shift) * (jet_scalefactor_do - jet_scalefactor) + jet_scalefactor;
            }
            if ( syst_shift > 0 ){
                jet_scalefactor_up = abs(syst_shift) * (jet_scalefactor_up - jet_scalefactor) + jet_scalefactor;
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
         

            theWeight = central;
            if ( syst_shift < 0 ) theWeight = errdown;
            if ( syst_shift > 0 ) theWeight = errup;

            if( this->debug_ ) {
                std::cout << "  " << shiftLabel( syst_shift ) << ": Object has e= " << obj.energy() << " eta=" << obj.eta()
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
