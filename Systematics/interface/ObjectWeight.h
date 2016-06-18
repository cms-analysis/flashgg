#ifndef FLASHgg_ObjectEffScale_h
#define FLASHgg_ObjectEffScale_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace flashgg {

    template <typename flashgg_object, typename param_var>
    class ObjectWeight : public ObjectSystMethodBinnedByFunctor<flashgg_object, param_var>
    {
    public:
        typedef StringCutObjectSelector<flashgg_object, true> selector_type;

        ObjectWeight( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv=0 );
        float makeWeight( const flashgg_object &obj, param_var syst_shift ) override;
        std::string shiftLabel( param_var syst_shift ) const override;
        
    private:
        selector_type overall_range_;

    };

    template<typename flashgg_object, typename param_var>
    ObjectWeight<flashgg_object, param_var>::ObjectWeight( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv ) : 
        ObjectSystMethodBinnedByFunctor<flashgg_object, param_var>( conf, std::forward<edm::ConsumesCollector>(iC), gv ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) )
    {
        this->setMakesWeight( true );
    }

    template<typename flashgg_object, typename param_var>
    std::string ObjectWeight<flashgg_object, param_var>::shiftLabel( param_var syst_value ) const
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
    float ObjectWeight<flashgg_object, param_var>::makeWeight( const flashgg_object &obj, param_var syst_shift )
    {
        float theWeight = 1.;
        if( overall_range_( obj ) ) {
            auto val_err = this->binContents( obj );
            float central = 1., errup = 0., errdown = 0.;
            if( val_err.first.size() == 1 && val_err.second.size() == 1 ) { // symmetric
                central = val_err.first[0];  
                errup = errdown = val_err.second[0]; 
            } else if ( val_err.first.size() == 1 && val_err.second.size() == 2 ) { // asymmetric
                central = val_err.first[0];
                errup = val_err.second[0];
                errdown = val_err.second[1];
            } else {
                throw cms::Exception("BadConfig") << " We do not recognize the bin format or this object is not in any bin";
            }
            if (this->applyCentralValue()) {
                theWeight = central;
            } else {
                theWeight = 1;
            }
            if ( syst_shift < 0 ) theWeight += syst_shift * errdown;
            if ( syst_shift > 0 ) theWeight += syst_shift * errup;
            if( this->debug_ ) {
                std::cout << "  " << shiftLabel( syst_shift ) << ": Object has e= " << obj.energy() << " eta=" << obj.eta()
                          << " and we apply a weight of " << theWeight << std::endl;
            }
        }
        return theWeight;
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
