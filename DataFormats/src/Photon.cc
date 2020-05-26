#include "flashgg/DataFormats/interface/Photon.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <limits>

using namespace flashgg;

Photon::Photon() : pat::Photon::Photon()
{
    ZeroVariables();
}

void Photon::ZeroVariables()
{
    sipip_ = 0.;
    sieip_ = 0.;
    //    zernike20_ = 0.;
    //    zernike42_ = 0.;
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
    pfPhoIso03Cor_ = 0.;
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

Photon::Photon( const pat::Photon &aPhoton ) : pat::Photon::Photon( aPhoton )
{
    ZeroVariables();
}

/// Photon::Photon( const Photon &aPhoton ) : pat::Photon::Photon( aPhoton )
/// {
///     sipip_ = aPhoton.sipip();
///     sieip_ = aPhoton.sieip();
///     e2nd_ = aPhoton.e2nd();
///     e2x5right_ = aPhoton.e2x5right();
///     e2x5left_ = aPhoton.e2x5left();
///     e2x5top_ = aPhoton.e2x5top();
///     e2x5bottom_ = aPhoton.e2x5bottom();
///     e2x5max_ = aPhoton.e2x5max();
///     eright_ = aPhoton.eright();
///     eleft_ = aPhoton.eleft();
///     etop_ = aPhoton.etop();
///     ebottom_ = aPhoton.ebottom();
///     e1x3_ = aPhoton.e1x3();
///     S4_ = aPhoton.s4();
///     pfPhoIso04_ = aPhoton.pfPhoIso04();
///     pfPhoIso03_ = aPhoton.pfPhoIso03();
///     pfChgIsoWrtWorstVtx04_ = aPhoton.pfChgIsoWrtWorstVtx04();
///     pfChgIsoWrtWorstVtx03_ = aPhoton.pfChgIsoWrtWorstVtx03();
///     pfChgIsoWrtChosenVtx02_ = aPhoton.pfChgIsoWrtChosenVtx02();
///     ESEffSigmaRR_ = aPhoton.esEffSigmaRR();
///     sigEOverE_ = aPhoton.sigEOverE();
///     pfChgIso04_ = aPhoton.pfChgIso04();
///     pfChgIso03_ = aPhoton.pfChgIso03();
///     pfChgIso02_ = aPhoton.pfChgIso02();
///     phoIdMvaD_ = aPhoton.phoIdMvaD();
///     passElecVeto_ = aPhoton.passElectronVeto();
///     extraChargedIsolations_ = aPhoton.extraChargedIsolations_;
///     extraPhotonIsolations_  = aPhoton.extraPhotonIsolations_;
///     extraNeutralIsolations_ = aPhoton.extraNeutralIsolations_;
/// }

Photon::~Photon() {}

Photon *Photon::clone() const { return new Photon( *this ); }

void Photon::removeVerticesExcept( const std::set<edm::Ptr<reco::Vertex> > &vtxPtrList )
{
    /*
        std::map<edm::Ptr<reco::Vertex>, float> pfChgIso04_;
        std::map<edm::Ptr<reco::Vertex>, float> pfChgIso03_;
        std::map<edm::Ptr<reco::Vertex>, float> pfChgIso02_;
        std::map<edm::Ptr<reco::Vertex>, float> phoIdMvaD_;
        bool passElecVeto_;
        std::map<std::string, std::map<edm::Ptr<reco::Vertex>, float> > extraChargedIsolations_;
    */

    //    std::cout << "Running removeVerticesExcept" << std::endl;
    //    std::cout << "  Size of pfChgIso04_: " << pfChgIso04_.size() << std::endl;
    //    std::cout << "  Size of vtxPtrList: " << vtxPtrList.size() << std::endl;

    std::map<edm::Ptr<reco::Vertex>, float>::iterator map_it;

    for( auto extra_map_it = extraChargedIsolations_.begin() ; extra_map_it != extraChargedIsolations_.end() ; extra_map_it++ ) {
        map_it = extra_map_it->second.begin();
        while( map_it != extra_map_it->second.end() ) {
            bool deleteMe = true;
            for( auto list_it = vtxPtrList.begin() ; list_it != vtxPtrList.end() ; list_it++ ) {
                if( *list_it == map_it->first ) {
                    deleteMe = false;
                    break;
                }
            }
            if( deleteMe ) {
                map_it = extra_map_it->second.erase( map_it );
            } else {
                map_it++;
            }
        }
    }

    map_it = phoIdMvaD_.begin();
    while( map_it != phoIdMvaD_.end() ) {
        bool deleteMe = true;
        for( auto list_it = vtxPtrList.begin() ; list_it != vtxPtrList.end() ; list_it++ ) {
            if( *list_it == map_it->first ) {
                deleteMe = false;
                break;
            }
        }
        if( deleteMe ) {
            map_it = phoIdMvaD_.erase( map_it );
        } else {
            map_it++;
        }
    }

    map_it = pfChgIso04_.begin();
    while( map_it != pfChgIso04_.end() ) {
        bool deleteMe = true;
        for( auto list_it = vtxPtrList.begin() ; list_it != vtxPtrList.end() ; list_it++ ) {
            if( *list_it == map_it->first ) {
                deleteMe = false;
                break;
            }
        }
        if( deleteMe ) {
            map_it = pfChgIso04_.erase( map_it );
        } else {
            map_it++;
        }
    }

    map_it = pfChgIso03_.begin();
    while( map_it != pfChgIso03_.end() ) {
        bool deleteMe = true;
        for( auto list_it = vtxPtrList.begin() ; list_it != vtxPtrList.end() ; list_it++ ) {
            if( *list_it == map_it->first ) {
                deleteMe = false;
                break;
            }
        }
        if( deleteMe ) {
            map_it = pfChgIso03_.erase( map_it );
        } else {
            map_it++;
        }
    }

    map_it = pfChgIso02_.begin();
    while( map_it != pfChgIso02_.end() ) {
        bool deleteMe = true;
        for( auto list_it = vtxPtrList.begin() ; list_it != vtxPtrList.end() ; list_it++ ) {
            if( *list_it == map_it->first ) {
                deleteMe = false;
                break;
            }
        }
        if( deleteMe ) {
            map_it = pfChgIso02_.erase( map_it );
        } else {
            map_it++;
        }
    }


    //    std::cout << "  Size of pfChgIso04_: " << pfChgIso04_.size() << std::endl;
    //    std::cout << "  Size of vtxPtrList: " << vtxPtrList.size() << std::endl;
    //    std::cout << "End of removeVerticesExcept"<< std::endl;
}


// Very simple functions now, but we want to be smarter about them later
void Photon::setEnergyAtStep( std::string key, float val )
{
    addUserFloat( key, val );
}
float const Photon::energyAtStep( std::string key, std::string fallback ) const
{
    if( key == "initial" && !hasEnergyAtStep( "initial" ) ) {
        return energy(); // "initial" is always set whenever any other value is set
        // So if it's not present we can skip this mechanism for now
    }
    if( !fallback.empty() && (! hasEnergyAtStep(key) || userFloat(key) == 0.) ) { return energyAtStep(fallback,""); }
    return userFloat( key );
}
bool Photon::hasEnergyAtStep( std::string key ) const
{
    return hasUserFloat( key );
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

// For systematics
void Photon::shiftAllMvaValuesBy( float val ) {
    std::vector<edm::Ptr<reco::Vertex> > keys;
    for(auto keyval = phoIdMvaD_.begin(); keyval != phoIdMvaD_.end(); ++keyval) { 
        keys.push_back( keyval->first );
    }
    for(auto key = keys.begin() ; key != keys.end() ; ++key ) {
        phoIdMvaD_[*key] += val;
        if (phoIdMvaD_[*key] > 1.) phoIdMvaD_[*key] = 1.;
        if (phoIdMvaD_[*key] < -1.) phoIdMvaD_[*key] = -1.;
    }
}


void Photon::shiftMvaValueBy( float val, edm::Ptr<reco::Vertex> vtx ) {
    phoIdMvaD_[vtx] += val;
    if (phoIdMvaD_[vtx] > 1.) phoIdMvaD_[vtx] = 1.;
    if (phoIdMvaD_[vtx] < -1.) phoIdMvaD_[vtx] = -1.;
}

//sigmaEOverE systematycs
void Photon::shiftSigmaEOverEValueBy( float val, float cutoff ) {
    const LorentzVector pho_p4 = p4(getCandidateP4type());
    float energyError = getCorrectedEnergyError(getCandidateP4type());
    setP4(getCandidateP4type(), pho_p4, energyError + val*(std::max(energyError, cutoff)), false);
}


//sigmaEOverE systematycs
void Photon::smearSigmaEOverEValueBy( float val ) {
    const LorentzVector pho_p4 = p4(getCandidateP4type());
    float energyError = getCorrectedEnergyError(getCandidateP4type());
    float energy = getCorrectedEnergy(getCandidateP4type());
    energyError = sqrt( energyError*energyError + val*val*energy*energy );
    setP4(getCandidateP4type(), pho_p4, energyError, false);
}



//void Photon::setSigEOverE(){
//        sigEOverE_ = getCorrectedEnergyError( getCandidateP4type() ) / getCorrectedEnergy( getCandidateP4type() ) ;
//}
    



float const Photon::sigEOverE() const
{
    // Use uncertainty and error stored from reco because we want this fraction to be constant
    return ( getCorrectedEnergyError( getCandidateP4type() ) / getCorrectedEnergy( getCandidateP4type() ) );
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

