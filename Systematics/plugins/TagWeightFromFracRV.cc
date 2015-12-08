#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace flashgg {

    class TagWeightFromFracRV: public ObjectSystMethodBinnedByFunctor<flashgg::DiPhotonTagBase, int>
    {

    public:
        typedef StringCutObjectSelector<DiPhotonTagBase, true> selector_type;

        TagWeightFromFracRV( const edm::ParameterSet &conf );
        float makeWeight( const flashgg::DiPhotonTagBase &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;

    private:
        selector_type overall_range_;
        bool debug_;
    };

    TagWeightFromFracRV::TagWeightFromFracRV( const edm::ParameterSet &conf ) :
        ObjectSystMethodBinnedByFunctor( conf ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) )
    {
    }

    std::string TagWeightFromFracRV::shiftLabel( int syst_value ) const
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

    float TagWeightFromFracRV::makeWeight( const DiPhotonTagBase &obj, int syst_shift )
    {
        float theWeight = 1.;
        if( overall_range_( obj ) ) {
            auto val_err = this->binContents( obj );

            // Do the interpretation here!  See ObjectWeight for an example

            if( this->debug_ ) {
                std::cout << "  " << shiftLabel( syst_shift ) << ": Tag has diphoton pt " << obj.diPhoton()->pt() 
                          << " and we apply a weight of " << theWeight << std::endl;
            }
        }
        return theWeight;
    }


}

DEFINE_EDM_PLUGIN( FlashggSystematicTagMethodsFactory,
                   flashgg::TagWeightFromFracRV,
                   "FlashggTagWeightFromFracRV" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

