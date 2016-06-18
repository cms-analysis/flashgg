#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace flashgg {

    class DiPhotonWeightFromFracRV: public ObjectSystMethodBinnedByFunctor<flashgg::DiPhotonCandidate, int>
    {

    public:
        typedef StringCutObjectSelector<DiPhotonCandidate, true> selector_type;

        DiPhotonWeightFromFracRV( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv );
        float makeWeight( const flashgg::DiPhotonCandidate &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;

    private:
        selector_type overall_range_;
        bool debug_;
    };

    DiPhotonWeightFromFracRV::DiPhotonWeightFromFracRV( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer * gv ) :
        ObjectSystMethodBinnedByFunctor( conf, std::forward<edm::ConsumesCollector>(iC), gv  ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) )
    {
        this->setMakesWeight( true );
    }

    std::string DiPhotonWeightFromFracRV::shiftLabel( int syst_value ) const
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

    float DiPhotonWeightFromFracRV::makeWeight( const DiPhotonCandidate &obj, int syst_shift )
    {
        float theWeight = 1.;
        if( overall_range_( obj ) ) {
            auto val_err = this->binContents( obj );
	    if( val_err.first.size() == 2 ){
	      	      
	      // Do the interpretation here!  See ObjectWeight for an example
	      float dz=fabs(obj.genPV().z()-obj.vtx()->z());
	      float sign = 1.;
	      int index = 0;
	      if(dz>1){
		sign=-1.;
		index=1;
	      }
	      
	      float weight = val_err.first[index];
          if (!applyCentralValue()) weight = 1.;
	      float eUp, eDown;
	      if(val_err.second.size() == 4){
		eUp= val_err.second[index*2];
		eDown= val_err.second[index*2+1];
	      }else{
		eUp=eDown= val_err.second[index];
	      }

	      float error;
	      if(syst_shift*sign>0) error=eUp;
	      else error = eDown;
	      
	      theWeight = weight + ( error * syst_shift * sign);
	      
	      if( this->debug_ ) {
                std::cout << "  " << shiftLabel( syst_shift ) << ": Diphoton has pt " << obj.pt() 
                          << " and we apply a weight of " << theWeight << std::endl;
	      }
	    }
	}else{
	  cout<<" Wrong FracRV input size"<< endl;
    }

        return theWeight;
    }


}

/*

EXAMPLE1:


        double var_value = ObjectSystMethodBinnedByFunctor<flashgg_object, param_var>::functors_[0](
                               obj ); //value of objton parameter, most probably eithr lep.pt() or lep.eta()

        int myLowerIndex = myBins.first[0];
        int myUpperIndex = myBins.first[1];
        //std::cout << "myLowerBin " << myLowerIndex << std::endl;
        //std::cout << "myUpperBin " << myUpperIndex << std::endl;

        double xLow = myBins.second[0].min[0];//lower limit of lower bin   *|_|_|
        double xHigh = myBins.second[1].max[0];//upper limit of upper bin   |_|_|*
        double yLow = myBins.second[0].val[0];//scale factor value from lower bin
        double yHigh = myBins.second[1].val[0];//scale factor value from upper bin

        double errLowYup = myBins.second[0].unc[0];//upper error of lower bin
        double errLowYdown = myBins.second[0].unc[1];//lower error of lower bin
        double errHighYup = myBins.second[1].unc[0];//upper error of upper bin
        double errHighYdown = myBins.second[1].unc[1];//lower error of upper bin

        bool atBoundary = false;

        if( myLowerIndex == myUpperIndex ) {
            atBoundary = true; //checking if at the boundary
        }

        double theErrorLow;
        double theErrorHigh;

        if( syst_shift > 0 ) {
            theErrorLow = errLowYup;//positive syst_shift values
            theErrorHigh = errHighYup;
        } else {
            theErrorLow = errLowYdown; //negative syst_shift values
            theErrorHigh = errHighYdown;
        }

        double theWeight = 1;
        double  theError = 0;

        if( !atBoundary ) {
            //std::cout << "it reaches this level " << std::endl;
            theWeight = yLow + ( yHigh - yLow ) / ( xHigh - xLow ) * ( var_value - xLow );
            //std::cout << "theWeight " << theWeight << std::endl;
            theError  = theErrorLow + ( theErrorHigh - theErrorLow ) / ( xHigh - xLow ) * ( var_value - xLow );
        } else {

            if( myLowerIndex == 0 && myUpperIndex == 0 ) {
                theWeight = yLow;
                theError = theErrorLow;
                //std::cout << "at the low boundary " << std::endl;
            } else if( myLowerIndex > 0 && myLowerIndex == myUpperIndex ) {
                theWeight = yHigh;
                theError = theErrorHigh;
                //std::cout << "at the high boundary " << std::endl;
            } else {

                std::cout <<
                          " ** you claim to be at boundary of the efficiency range! but you are simply not in the same Set of the range, you probably have a negative bin value " <<
                          std::endl;

            }
        }
        double Weight = theWeight + ( theError * syst_shift );
        //        lep.setWeight( shiftLabel(syst_shift), Weight );
        //std::cout << "Weight " << lep.weight() << std::endl;
        if( this->debug_ ) { std::cout << "  end of ObjectEffScale<flashgg_object, param_var>::makeWeight - returning " << Weight << std::endl; }

        return Weight;
    }

EXAMPLE2:

        if( overall_range_( y ) ) {
            auto val_err = binContents( y );
            if( val_err.first.size() == 1 && val_err.second.size() == 1 ) { // otherwise no-op because we don't have an entry
                float sigma_smearing = val_err.first[0];
                float sigma_smearing_err = val_err.second[0];
                if( exaggerateShiftUp_ && syst_shift == 1 ) { sigma_smearing_err  = 9 * sigma_smearing; }
                float sigma = sigma_smearing + syst_shift * sigma_smearing_err;
                float newe = CLHEP::RandGauss::shoot( RandomEngine(), y.energy(), sigma * y.energy() );
                if( debug_ ) {
                    std::cout << "  " << shiftLabel( syst_shift ) << ": Photon has energy= " << y.energy() << " eta=" << y.eta()
                              << " and we apply a smearing with sigma " << ( 100 * sigma ) << "% to get new energy=" << newe << std::endl;
                }
                y.updateEnergy( shiftLabel( syst_shift ), newe );
 */

DEFINE_EDM_PLUGIN( FlashggSystematicDiPhotonMethodsFactory,
                   flashgg::DiPhotonWeightFromFracRV,
                   "FlashggDiPhotonWeightFromFracRV" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

