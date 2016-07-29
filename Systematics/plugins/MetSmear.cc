#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"


namespace flashgg{

  class MetSmear: public BaseSystMethod<pat::MET, int>
  {


  public:
    typedef StringCutObjectSelector<pat::MET, true> selector_type;
    MetSmear( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv );

    void applyCorrection( pat::MET &y, int syst_shift ) override;
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

  void MetSmear::applyCorrection( pat::MET &y, int syst_shift )
  {
    if( overall_range_( y ) ) 
      {
	if ( debug_ ) 
	  {
	    std::cout << "  " << shiftLabel( syst_shift ) << ": Met has et=" << y.corPt() << " phi=" << y.corPhi() << std::endl;
	  }
	
	//      y.setCorPx(); y.setCorPy(); y.setCorPhi();
	if(syst_shift==1)
	  {
	    //y.setCorPx(y.corPx()+y.uncertainties->dpx());
	    //y.setCorPy(y.corPy()+y.uncertainties->dpy());
	    //y.setCorPt(sqrt(y.corPy()*y.corPy()+y.corPx()*y.corPx()));
	    
	  }
	else if(syst_shift==-1)
	  {
	    //y.setCorPx(y.corPx()-y.uncertainties->dpx());
	    //y.setCorPy(y.corPy()-y.uncertainties->dpy());
	    //y.setCorPt(sqrt(y.corPy()*y.corPy()+y.corPx()*y.corPx()));
	  }
	else
	  throw cms::Exception("UnsupportedMET smear") << " syst_shift=" << syst_shift << " is not supported";
      }
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
