#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Met.h"
//#include "DataFormats/PatCandidates/interface/MET.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"


namespace flashgg{

  class MetPhoSmear: public BaseSystMethod<flashgg::Met, int>
  {


  public:
    typedef StringCutObjectSelector<flashgg::Met, true> selector_type;
    MetPhoSmear( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv );

    void applyCorrection( flashgg::Met &y, int syst_shift ) override;
    std::string shiftLabel( int ) const override;
    void eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) override;
  private:
    selector_type overall_range_;
    bool debug_;
  };
  
  void MetPhoSmear::eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) 
  {
    
  }
  MetPhoSmear::MetPhoSmear(const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv) :
    BaseSystMethod( conf, std::forward<edm::ConsumesCollector>(iC) ),
    overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
    debug_( conf.getUntrackedParameter<bool>("Debug", false) )
  {
  }
  
  std::string MetPhoSmear::shiftLabel( int syst_value ) const
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

  void MetPhoSmear::applyCorrection( flashgg::Met &mets, int syst_shift )
  {
      //pat::METCorrectionLevel level = Type1;
      
      if ( debug_ && syst_shift==-1) 
          {
              std::cout << "        unshifted" << ": Met has et=" << mets.getCorPt() << " phi=" << mets.getCorPhi() << std::endl;
              std::cout << "        unshifted" << ": Met has et=" << mets.shiftedPt((pat::MET::METUncertainty) 14) << " phi=" << mets.shiftedPhi((pat::MET::METUncertainty) 14) << std::endl;
          }
      if( overall_range_( mets ) ) 
          {
              //mets.setCorPx(); mets.setCorPy(); mets.setCorPhi();
              //pat::MET::METUncertainty shift;
              //shift = JetResUp;
              if(syst_shift==0)
                  {//do nothing
                  }
              
              else if(syst_shift==1)
                  {
                      //std::cout << "+1 sigma for MET shift" << std::endl;
                      //0: JER
                      //2: JEC
                      //4: muon
                      //6: ele
                      //8: tau
                      //10: unc
                      //12: pho
                      //14: unshifted
                      
                      mets.setCorPt(mets.shiftedPt((pat::MET::METUncertainty) 12));
                      mets.setCorPhi(mets.shiftedPhi((pat::MET::METUncertainty) 12));
                  }
              else if(syst_shift==-1)
                  {
                      mets.setCorPt(mets.shiftedPt((pat::MET::METUncertainty) 13));
                      mets.setCorPhi(mets.shiftedPhi((pat::MET::METUncertainty) 13));
                  }
              else
                  {
                      std::cout << std::endl;
                      throw cms::Exception("UnsupportedMET smear") << " syst_shift=" << syst_shift << " is not supported";
                  }
              if ( debug_ && syst_shift!=0) 
                  {
                      std::cout << "               " << shiftLabel( syst_shift ) << ": fullcor has et=" << mets.getCorPt() << " phi=" << mets.getCorPhi() << std::endl;
                  }
          }
      //from python call
      //NSigmas = cms.vint32(-1,1)
      //ApplyCentralValue = cms.bool(False)
  }  
}
DEFINE_EDM_PLUGIN( FlashggSystematicMetMethodsFactory,
                   flashgg::MetPhoSmear,
                   "FlashggMetPhoSmear" );
// Local Variables:
// mode:c++        
// indent-tabs-mode:nil
// tab-width:4         
// c-basic-offset:4    
// End:                
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4 
