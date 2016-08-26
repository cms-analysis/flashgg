#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Met.h"
//#include "DataFormats/PatCandidates/interface/MET.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"


namespace flashgg{

  class MetSmear: public BaseSystMethod<flashgg::Met, int>
  {


  public:
    typedef StringCutObjectSelector<flashgg::Met, true> selector_type;
    MetSmear( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv );

    void applyCorrection( flashgg::Met &y, int syst_shift ) override;
    std::string shiftLabel( int ) const override;
    void eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) override;
  private:
    selector_type overall_range_;
    bool debug_;
  };
  
  void MetSmear::eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) 
  {
    
  }
  MetSmear::MetSmear(const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv) :
    BaseSystMethod( conf, std::forward<edm::ConsumesCollector>(iC) ),
    overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
    debug_( conf.getUntrackedParameter<bool>("Debug", false) )
  {
  }
  
  std::string MetSmear::shiftLabel( int syst_value ) const
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

  void MetSmear::applyCorrection( flashgg::Met &y, int syst_shift )
  {
      //pat::METCorrectionLevel level = Type1;
      
      if ( debug_ && syst_shift==-1) 
          {
              std::cout << "        unshifted" << ": Met has et=" << y.getCorPt() << " phi=" << y.getCorPhi() << std::endl;
          }
      if( overall_range_( y ) ) 
          {
              //y.setCorPx(); y.setCorPy(); y.setCorPhi();
              //pat::MET::METUncertainty shift;
              //shift = JetResUp;
              if(syst_shift==0)
                  {//do nothing
                  }
              
              else if(syst_shift==1)
                  {
                      //std::cout << "+1 sigma for MET shift" << std::endl;
                      //std::cout << "previous px value: " << y.corPx() <<  "   shifted up by jet:  " << y.shiftedPx((pat::MET::METUncertainty) 2) << std::endl;
                      //std::cout << "previous px value: " << y.corPx() <<  "   shifted up by mu :  " << y.shiftedPx((pat::MET::METUncertainty) 4) << std::endl;
                      //std::cout << "previous px value: " << y.corPx() <<  "   shifted up by ele:  " << y.shiftedPx((pat::MET::METUncertainty) 6) << std::endl;
                      //std::cout << "previous px value: " << y.corPx() <<  "   shifted up by tau:  " << y.shiftedPx((pat::MET::METUncertainty) 8) << std::endl;
                      //std::cout << "previous px value: " << y.corPx() <<  "   shifted up by unc:  " << y.shiftedPx((pat::MET::METUncertainty) 10) << std::endl;
                      //std::cout << "previous px value: " << y.corPx() <<  "   shifted up by pho:  " << y.shiftedPx((pat::MET::METUncertainty) 12) << std::endl;
                      //std::cout << "previous px value: " << y.corPx() <<  "   unshifted        :  " << y.shiftedPx((pat::MET::METUncertainty) 14) << std::endl;
                      
                      y.setCorPx(y.shiftedPx((pat::MET::METUncertainty) 10));
                      y.setCorPy(y.shiftedPy((pat::MET::METUncertainty) 10));
                      
                      //std::cout << "  shifted up:  " << y.getCorPt() << std::endl;
                  }
              else if(syst_shift==-1)
                  {
                      //std::cout << "-1 sigma for MET shift" << std::endl;
                      y.setCorPx(y.shiftedPx((pat::MET::METUncertainty) 11));
                      y.setCorPy(y.shiftedPy((pat::MET::METUncertainty) 11));
                      //std::cout << "  shifted down:  " << y.getCorPt() << std::endl;
                  }
              else
                  {
                      std::cout << std::endl;
                      //std::cout << "central value for MET shift" << std::endl;
                      throw cms::Exception("UnsupportedMET smear") << " syst_shift=" << syst_shift << " is not supported";
                  }
              if ( debug_ && syst_shift==-1) 
                  {
                      std::cout << "  " << shiftLabel( syst_shift ) << ": Met has et=" << y.getCorPt() << " phi=" << y.getCorPhi() << std::endl;
                  }
          }
      //from python call
      //NSigmas = cms.vint32(-1,1)
      //ApplyCentralValue = cms.bool(False)
  }  
}
DEFINE_EDM_PLUGIN( FlashggSystematicMetMethodsFactory,
                   flashgg::MetSmear,
                   "FlashggMetSmear" );
// Local Variables:
// mode:c++        
// indent-tabs-mode:nil
// tab-width:4         
// c-basic-offset:4    
// End:                
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4 
