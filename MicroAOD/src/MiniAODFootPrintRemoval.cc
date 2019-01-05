// Copied from https://raw.githubusercontent.com/musella/diphotons/693a5a0413ba71a3f1f81edf09bf6963bb4613dc/SCFootprintRemoval/src/MiniAODFootPrintRemoval.cc

// -*- C++ -*-
//
// Package:    MiniAODFootprintRemoval
// Class:      MiniAODFootprintRemoval
//
/**\class MiniAODFootprintRemoval MiniAODFootprintRemoval.cc PFIsolation/MiniAODFootprintRemoval/plugins/MiniAODFootprintRemoval.cc

   Description: Implements the algo for removal of PF clusters from the SC footprint

   Implementation:
   Runs on AOD.
*/
//
// Original Author:  Marco Peruzzi,32 4-C16,+41227676829,
//         Created:  Sat Sep 29 17:58:21 CEST 2012
// $Id: MiniAODFootprintRemoval.cc,v 1.10 2013/05/15 12:57:16 peruzzi Exp $
//
//


#include "../interface/MiniAODFootPrintRemoval.h"

/// #include "flashgg/MicroAOD/interface/PhotonIdUtils.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalEndcapGeometry.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "TRandom3.h"
#include "TMath.h"
#include "TRotation.h"
#include "TString.h"

using namespace std;
using namespace diphotons;

//
// constructors and destructor
//
MiniAODFootprintRemoval::MiniAODFootprintRemoval( edm::ParameterSet iConfig )
{
    //now do what ever initialization is needed
    rotation_phi = 0.;

    eegeom = TGeoPara( 1, 1, 1, 0, 0, 0 );

    global_linkbyrechit_enlargement = iConfig.getParameter<double>( "rechitLinkEnlargement" );
    removePhotonsInMap_ = iConfig.getParameter<int>( "removePhotonsInMap" );

}

void MiniAODFootprintRemoval::setup( const pat::Photon &photon, const edm::Event &iEvent, const edm::EventSetup &iSetup )
{
    edm::ESHandle<CaloGeometry> geometry ;
    iSetup.get<CaloGeometryRecord>().get( geometry );
    barrelGeometry = ( CaloSubdetectorGeometry * )( geometry->getSubdetectorGeometry( DetId::Ecal, EcalBarrel ) );
    endcapGeometry = ( CaloSubdetectorGeometry * )( geometry->getSubdetectorGeometry( DetId::Ecal, EcalEndcap ) );

    edm::ESHandle<MagneticField> magneticField;
    iSetup.get<IdealMagneticFieldRecord>().get( magneticField );
    magField = ( MagneticField * )( magneticField.product() );

    // compute supe-cluster foot-print
    infos = getSCXtalInfo( photon.superCluster() );

    // use association map to recover link by super-cluster ref
    if( removePhotonsInMap_ > 0 ) {
        edm::RefVector<pat::PackedCandidateCollection> associated =  photon.associatedPackedPFCandidates();
        if( ! associated.empty() ) {
            std::map<float, edm::Ptr<pat::PackedCandidate> > sorted;
            for( unsigned int ipc = 0; ipc < associated.size(); ipc++ ) {
                auto &ref = associated[ipc];
                edm::Ptr<pat::PackedCandidate> ptr = edm::refToPtr( ref );
                sorted[ reco::deltaR( *ptr, photon ) ] = ptr;
            }
            auto itr = sorted.begin();
            while( itr != sorted.end() && ( int )associatedCandidates.size() < removePhotonsInMap_ ) {
                if( itr->second->pdgId() == 22 ) {
                    associatedCandidates.push_back( itr->second );
                }
            }
        }
    }
}

MiniAODFootprintRemoval::~MiniAODFootprintRemoval()
{
}


//
// member functions
//


TVector3 MiniAODFootprintRemoval::propagatePFCandToEcal( const edm::Ptr<pat::PackedCandidate> &cand, float position, bool isbarrel )
{
    // WARNING: this propagates until EE+ or EE- at the given TMath::Abs(position.z()) for isbarrel=0, depending on where the candidate is pointing.

    int type = findPFCandType( cand->pdgId() );

    if( cand->pt() <= 0 ) {
        std::cout << "Warning: called propagation to ECAL for object with negative or zero pt. Returning TVector3(0,0,1e10)." << std::endl;
        return TVector3( 0, 0, 1e10 );
    }

    if( type > 2 ) {
        std::cout << "Asking propagation for lepton, not implemented. Returning TVector3(0,0,1e10)." << std::endl;
        return TVector3( 0, 0, 1e10 );
    }

    TVector3 pfvertex( cand->vx(), cand->vy(), cand->vz() );
    TVector3 pfmomentum( cand->px(), cand->py(), cand->pz() );
    pfmomentum = pfmomentum.Unit();

    TVector3 ecalpfhit( 0, 0, 0 );
    if( isbarrel ) {
        double p[3] = {pfvertex.x(), pfvertex.y(), pfvertex.z()};
        double d[3] = {pfmomentum.x(), pfmomentum.y(), pfmomentum.z()};
        double dist = TGeoTube::DistFromInsideS( p, d, 0, position, 1e+10 );
        ecalpfhit = pfvertex + dist * pfmomentum;
    } else { // EE
        double dim[6] = {1e+10, 1e+10, fabs( position ), 0, 0, 0};
        double p[3] = {pfvertex.x(), pfvertex.y(), pfvertex.z()};
        double d[3] = {pfmomentum.x(), pfmomentum.y(), pfmomentum.z()};
        eegeom.SetDimensions( dim );
        double dist = eegeom.DistFromInside( p, d );
        ecalpfhit = pfvertex + dist * pfmomentum;
    }

    if( type == 1 ) { // APPROXIMATE helix propagation
        float field = magField->inTesla( GlobalPoint( 0., 0., 0. ) ).z();
        float charge = cand->charge();
        float curvature = ( cand->pt() ) / ( 0.3 * field * charge ) * 100; // curvature radius in cm
        float final_radius = ecalpfhit.Perp();
        float addphi = -TMath::ASin( final_radius / curvature / 2 );

        TRotation r;
        r.RotateZ( addphi );
        ecalpfhit *= r;

    }

    return ecalpfhit;
}

sc_xtal_information MiniAODFootprintRemoval::getSCXtalInfo( reco::SuperClusterRef sc )
{
    sc_xtal_information out;

    if( ! sc.isAvailable() ) {
        cout << "Warning : unavaliable super cluster " << endl;
        return out;
    }

    std::map<DetId,float> hitsAndFractions;    
    if( sc->clusters().isAvailable() ) {
        for( reco::CaloCluster_iterator bc = sc->clustersBegin(); bc != sc->clustersEnd(); ++bc ) {
            if( ! bc->isAvailable() ) { continue; }
            const std::vector< std::pair<DetId, float> > &seedrechits = ( *bc )->hitsAndFractions();
            for( unsigned int i = 0; i < seedrechits.size(); i++ ) { 
                hitsAndFractions[seedrechits[i].first] += seedrechits[i].second; 
                // crystals.push_back( seedrechits[i].first ); 
            }
        }
    } else if( sc->seed().isAvailable() ) {
        /// cout << "Warning : only seed cluster available " << endl;
        const std::vector< std::pair<DetId, float> > &seedrechits = sc->seed()->hitsAndFractions();
        for( unsigned int i = 0; i < seedrechits.size(); i++ ) { 
            hitsAndFractions[seedrechits[i].first] += seedrechits[i].second; 
            // crystals.push_back( seedrechits[i].first ); 
        }
    } else {
        cout << "Warning : no basic clusters available " << endl;
        return out;
    }

    std::vector<DetId> crystals;    
    for( auto hi : hitsAndFractions ) {
        if( hi.second > 0.2 ) {
            crystals.push_back( hi.first ); 
        }
    }
    
    sort( crystals.begin(), crystals.end() );
    std::vector<DetId>::iterator it;
    it = unique( crystals.begin(), crystals.end() );
    crystals.resize( it - crystals.begin() );

    unsigned int i = 0;
    for( i = 0; i < crystals.size(); i++ ) {

        if( crystals.at( i ).subdetId() != EcalBarrel && crystals.at( i ).subdetId() != EcalEndcap ) { continue; }
        bool isbarrel = ( crystals.at( i ).subdetId() == EcalBarrel );

        const CaloCellGeometry *cellGeometry = NULL;
        EBDetId ebDetId;
        EEDetId eeDetId;
        if( isbarrel ) {
            ebDetId = crystals.at( i );
            cellGeometry = ( barrelGeometry->getGeometry( ebDetId ) ).get();
        } else {
            eeDetId = crystals.at( i );
            cellGeometry = ( endcapGeometry->getGeometry( eeDetId ) ).get();
        }
        TVector3 xtal_position( cellGeometry->getPosition().x(), cellGeometry->getPosition().y(), cellGeometry->getPosition().z() );

        float dphi;
        float deta;
        if( isbarrel ) {
            dphi = ( dynamic_cast<const EcalBarrelGeometry *>( barrelGeometry ) )->deltaPhi( ebDetId );
            deta = ( dynamic_cast<const EcalBarrelGeometry *>( barrelGeometry ) )->deltaEta( ebDetId );
        } else {
            dphi = ( dynamic_cast<const EcalEndcapGeometry *>( endcapGeometry ) )->deltaPhi( eeDetId );
            deta = ( dynamic_cast<const EcalEndcapGeometry *>( endcapGeometry ) )->deltaEta( eeDetId );
        }

        const CaloCellGeometry::CornersVec &cellCorners( cellGeometry->getCorners() );
        std::vector<TVector3> corners;
        for( int k = 0; k < 4; k++ ) {
            TVector3 thiscorner( ( float )( cellCorners[k].x() ), ( float )( cellCorners[k].y() ), ( float )( cellCorners[k].z() ) );
            corners.push_back( thiscorner );
        }
        out.xtalposition.push_back( xtal_position );
        out.xtaletawidth.push_back( deta );
        out.xtalphiwidth.push_back( dphi );
        out.xtalcorners.push_back( corners );
    }
    out.nxtals = i;

    assert( out.nxtals == ( int )out.xtalposition.size() );

    return out;
}


bool MiniAODFootprintRemoval::checkMatchedPFCandidate( const edm::Ptr<pat::PackedCandidate> &cand )
{
    if( ( cand->pdgId() == 22 ) &&
            ( find( associatedCandidates.begin(), associatedCandidates.end(), cand ) != associatedCandidates.end() ) ) {
        return true;
    }

    return false;
}

/// bool MiniAODFootprintRemoval::operator()(const pat::Photon& photon, const edm::Ptr<pat::PackedCandidate> & pfcand)
/// {
/// }

bool MiniAODFootprintRemoval::checkPFCandInFootprint( const pat::Photon &pho, const edm::Ptr<pat::PackedCandidate> &cand )
{
    bool isbarrel = ( fabs( pho.superCluster()->eta() ) < 1.5 );

    if( checkMatchedPFCandidate( cand ) ) { return true; }

    if( cand->pt() <= 0 ) { return false; }

    int type = findPFCandType( cand->pdgId() );
    if( type > 2 ) { return false; }

    bool inside = false;

    for( int j = 0; j < infos.nxtals; j++ ) {

        TVector3 xtal_position = infos.xtalposition[j];
        if( rotation_phi != 0 ) {
            TRotation r;
            r.RotateZ( rotation_phi );
            xtal_position *= r;
        }

        TVector3 ecalpfhit = propagatePFCandToEcal( cand, isbarrel ? xtal_position.Perp() : xtal_position.z(), isbarrel );

        if( ecalpfhit.Perp() == 0 ) { continue; }

        if( isbarrel ) {
            float xtalEtaWidth = infos.xtaletawidth[j] * ( 1.0 + global_linkbyrechit_enlargement );
            float xtalPhiWidth = infos.xtalphiwidth[j] * ( 1.0 + global_linkbyrechit_enlargement );
            float deta = ecalpfhit.Eta() - xtal_position.Eta();
            float dphi = reco::deltaPhi( ecalpfhit.Phi(), xtal_position.Phi() );
            if( fabs( deta ) < xtalEtaWidth / 2 && fabs( dphi ) < xtalPhiWidth / 2 ) { inside = true; }
        } else { // EE
            if( ecalpfhit.z()*xtal_position.z() > 0 ) {
                TVector3 xtal_corners[4];
                for( int k = 0; k < 4; k++ ) { xtal_corners[k] = infos.xtalcorners[j][k]; }
                if( rotation_phi != 0 ) {
                    TRotation r;
                    r.RotateZ( rotation_phi );
                    for( int k = 0; k < 4; k++ ) { xtal_corners[k] *= r; }
                }
                float hitx = ecalpfhit.x();
                float hity = ecalpfhit.y();
                float polx[5];
                float poly[5];
                for( int k = 0; k < 4; k++ ) { polx[k] = xtal_corners[k].x(); }
                for( int k = 0; k < 4; k++ ) { poly[k] = xtal_corners[k].y(); }
                polx[4] = polx[0];
                poly[4] = poly[0]; // closed polygon
                float centerx = ( polx[0] + polx[1] + polx[2] + polx[3] ) / 4;
                float centery = ( poly[0] + poly[1] + poly[2] + poly[3] ) / 4;
                hitx = centerx + ( hitx - centerx ) / ( 1.0 + global_linkbyrechit_enlargement );
                hity = centery + ( hity - centery ) / ( 1.0 + global_linkbyrechit_enlargement );
                if( TMath::IsInside( hitx, hity, 5, polx, poly ) ) { inside = true; }
            }
        }

    }

    return inside;
}

int MiniAODFootprintRemoval::findPFCandType( int id )
{

    int type = -1;

    if( id == 111 || id == 130 || id == 310 || id == 2112 ) { type = 0; } //neutral hadrons
    if( fabs( id ) == 211 || fabs( id ) == 321 || id == 999211 || fabs( id ) == 2212 ) { type = 1; } //charged hadrons
    if( id == 22 ) { type = 2; } //photons
    if( fabs( id ) == 11 ) { type = 3; } //electrons
    if( fabs( id ) == 13 ) { type = 4; } //muons

    return type;
}



// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

