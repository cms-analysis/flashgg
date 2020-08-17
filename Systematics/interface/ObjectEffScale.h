#ifndef FLASHgg_ObjectEffScale_h
#define FLASHgg_ObjectEffScale_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"

namespace flashgg {

    template <typename flashgg_object, typename param_var>
    class ObjectEffScale : public ObjectSystMethodBinnedByFunctor<flashgg_object, param_var>
    {
    public:

        ObjectEffScale( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        float makeWeight( const flashgg_object &obj, param_var syst_shift ) override;
        std::string shiftLabel( param_var syst_shift ) const override;
    };

    template<typename flashgg_object, typename param_var>
    ObjectEffScale<flashgg_object, param_var>::ObjectEffScale( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv )
        : ObjectSystMethodBinnedByFunctor<flashgg_object, param_var>( conf, std::forward<edm::ConsumesCollector>(iC), gv )
    {
        this->setMakesWeight( true );
    }

    template<typename flashgg_object, typename param_var>
    std::string ObjectEffScale<flashgg_object, param_var>::shiftLabel( param_var syst_value ) const
    {
        std::string result;
        if( syst_value == 0 ) {
            result = Form( "%sCentral", this->label().c_str() );
        } else if( syst_value > 0 ) {
            result = Form( "%sUp%.2dsigma", this->label().c_str(), syst_value );
        } else {
            result = Form( "%sDown%.2dsigma", this->label().c_str(), -1 * syst_value );
        }
        return result;
    }

    template<typename flashgg_object, typename param_var>
    float ObjectEffScale<flashgg_object, param_var>::makeWeight( const flashgg_object &obj, param_var syst_shift )
    {
        if( this->debug_ ) { std::cout << "  Start of ObjectEffScale<flashgg_object, param_var>::makeWeight" << std::endl; }

        typedef typename ObjectSystMethodBinnedByFunctor<flashgg_object, param_var>::Bin BaseBin;
        std::pair<std::vector<int>, std::vector<BaseBin> > myBins = ObjectSystMethodBinnedByFunctor<flashgg_object, param_var>::adjacentBins( obj );

        double var_value = ObjectSystMethodBinnedByFunctor<flashgg_object, param_var>::functors_[0]->eval(
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
        if (!this->applyCentralValue()) theWeight = 1.;
        double Weight = theWeight + ( theError * syst_shift );
        //        lep.setWeight( shiftLabel(syst_shift), Weight );
        //std::cout << "Weight " << lep.weight() << std::endl;
        if( this->debug_ ) { std::cout << "  end of ObjectEffScale<flashgg_object, param_var>::makeWeight - returning " << Weight << std::endl; }

        

        return Weight;
    }

}
#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
