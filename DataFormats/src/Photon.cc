#include "flashgg/DataFormats/interface/Photon.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <limits>

using namespace flashgg;

Photon::Photon()
{
    sipip_ = 0.;
    sieip_ = 0.;
    zernike20_ = 0.;
    zernike42_ = 0.;
    e2nd_ = 0.;
    e2x5right_ = 0.;
    e2x5left_ = 0.;
    e2x5top_ = 0.;
    e2x5bottom_ = 0.;
    e2x5max_ = 0.;
    eright_ = 0.;
    eleft_ = 0.;
    etop_ = 0.;
    ebottom_ = 0.;
    e1x3_ = 0.;
    S4_ = 0.;
    pfPhoIso04_ = 0.;
    pfPhoIso03_ = 0.;
    pfChgIsoWrtWorstVtx04_ = 0.;
    pfChgIsoWrtWorstVtx03_ = 0.;
    pfChgIsoWrtChosenVtx02_ = 0.;
    ESEffSigmaRR_ = 0.;
    sigEOverE_ = 0.;
    pfChgIso03_.clear();
    pfChgIso02_.clear();
    phoIdMvaD_.clear();
    passElecVeto_ = false;
}
Photon::Photon( const pat::Photon &aPhoton ) : pat::Photon( aPhoton ) {}
Photon::~Photon() {}

Photon *Photon::clone() const { return new Photon( *this ); }

// Very simple functions now, but we want to be smarter about them later
void Photon::setEnergyAtStep( std::string key, float val )
{
    addUserFloat( key, val );
}
float const Photon::energyAtStep( std::string key ) const
{
    if( key == "initial" && !hasEnergyAtStep( "initial" ) ) {
        return energy(); // "initial" is always set whenever any other value is set
        // So if it's not present we can skip this mechanism for now
    }
    return userFloat( key );
}
bool Photon::hasEnergyAtStep( std::string key ) const
{
    return hasUserData( key );
}


float const Photon::findVertex0Float( const std::map<edm::Ptr<reco::Vertex>, float> &mp ) const
{
    for( std::map<edm::Ptr<reco::Vertex>, float>::const_iterator it = mp.begin(); it != mp.end(); ++it ) {
        if( it->first.key() == 0 ) {
            return  it->second;
        }
    }

    throw cms::Exception( "Missing Data" ) << "could not find value for vertex 0\n";;

    return 0.;
}

float const Photon::findVertexFloat( const edm::Ptr<reco::Vertex> &vtx, const std::map<edm::Ptr<reco::Vertex>, float> &mp, bool lazy ) const
{
    lazy = lazy && ( vtx.id() == edm::ProductID( 0, 0 ) );
    for( std::map<edm::Ptr<reco::Vertex>, float>::const_iterator it = mp.begin(); it != mp.end(); ++it ) {
        if( ( lazy && it->first.key() == vtx.key() ) || it->first == vtx ) {
            return  it->second;
        }
    }

    throw cms::Exception( "Missing Data" ) << "could not find value for vertex " << vtx.key() << " " << vtx.id() << " lazy search: " << lazy <<  "\n";;

    return 0.;
}

float const Photon::findWorstIso( const std::map<edm::Ptr<reco::Vertex>, float> &mp ) const
{
    float ret = std::numeric_limits<float>::min();
    for( auto it : mp ) {
        ret = std::max( ret, it.second );
    }
    return ret;
}


void Photon::updateEnergy( std::string key, float val )
{

    // Current energy saved when updated, unless we're still at the initial step
    if( !hasEnergyAtStep( "initial" ) ) {
        setEnergyAtStep( "initial", energy() );
    }

    // Update new energy
    setEnergyAtStep( key, val );

    // Update 4-vector
    setP4( ( val / energy() )*p4() );
}

float const Photon::sigEOverE() const
{
    // Use uncertainty and error stored from reco because we want this fraction to be constant
    return ( getCorrectedEnergyError( regression_type ) / getCorrectedEnergy( regression_type ) );
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

