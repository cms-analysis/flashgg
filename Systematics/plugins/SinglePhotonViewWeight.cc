#include "flashgg/Systematics/interface/ObjectWeight.h"

namespace flashgg {

    typedef ObjectWeight<flashgg::SinglePhotonView, int> SinglePhotonViewWeight;

    template <>
    float ObjectWeight<flashgg::SinglePhotonView, int>::makeWeight( const flashgg::SinglePhotonView &obj, int syst_shift )
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
                std::cout << "  " << shiftLabel( syst_shift ) << ": Object has e= " << obj.photon()->energy() << " and eta=" << obj.photon()->superCluster()->eta() 
                          << " and we apply a weight of " << theWeight << " " << errdown << " " << errup << " " << syst_shift<< std::endl;
            }
        }
        return theWeight;
    }   
}




DEFINE_EDM_PLUGIN( FlashggSystematicSinglePhotonViewMethodsFactory,
                   flashgg::SinglePhotonViewWeight,
                   "FlashggSinglePhotonViewWeight" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
