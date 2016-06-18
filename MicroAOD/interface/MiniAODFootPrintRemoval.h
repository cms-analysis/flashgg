// -*- C++ -*-
//
// Ported to miniAOD from https://github.com/peruzzim/SCFootprintRemoval.git
//
// copied  from https://raw.githubusercontent.com/musella/diphotons/693a5a0413ba71a3f1f81edf09bf6963bb4613dc/SCFootprintRemoval/interface/MiniAODFootPrintRemoval.h

#ifndef __MINIAODFOOTPRINTREMOVAL__HH__
#define __MINIAODFOOTPRINTREMOVAL__HH__


// system include files
#include <memory>
#include <iostream>
#include <cmath>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "MagneticField/Engine/interface/MagneticField.h"

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "TVector3.h"
#include "TGeoTube.h"
#include "TGeoPara.h"


namespace diphotons {
    //
    // constants, enums and typedefs
    //
    typedef struct {
        int nxtals;
        std::vector<TVector3> xtalposition;
        std::vector<float> xtaletawidth;
        std::vector<float> xtalphiwidth;
        std::vector<std::vector<TVector3> > xtalcorners;
    } sc_xtal_information;

    //
    // class declaration
    //

    class MiniAODFootprintRemoval   // : public flashgg::OverlapRemovalAlgo {
    {

    public:

        //    The constructor can take the following arguments from configuration (default value):
        //
        //    isolation_cone_size_forSCremoval (0.4) : isolation cone used for PF Iso calculation
        //    tag_pfCandidates_forSCremoval ("particleFlow") : collection of PF candidates to use
        //    tag_jets ("ak5PFJets") : collection of PF jets to use for vetoing around random cone direction
        //    rechit_link_enlargement_forSCremoval (0.25) : enlargement of linear dimension of xtals for rechit matching

        MiniAODFootprintRemoval( edm::ParameterSet iConfig );
        ~MiniAODFootprintRemoval();

        void setup( const pat::Photon &photon, const edm::Event &iEvent, const edm::EventSetup &iSetup );

        bool checkPFCandInFootprint( const pat::Photon &photon, const edm::Ptr<pat::PackedCandidate> &cand );
        /// virtual bool operator()(const pat::Photon& photon, const edm::Ptr<pat::PackedCandidate> & pfcand);

        void setRotatioPhi( float x ) { rotation_phi = x; };
    private:
        std::vector<edm::Ptr<pat::PackedCandidate> > associatedCandidates;

        TVector3 propagatePFCandToEcal( const edm::Ptr<pat::PackedCandidate> &cand, float position, bool isbarrel );
        sc_xtal_information getSCXtalInfo( reco::SuperClusterRef sc );
        bool checkMatchedPFCandidate( const edm::Ptr<pat::PackedCandidate> &pfcandindex );

        CaloSubdetectorGeometry *barrelGeometry;
        CaloSubdetectorGeometry *endcapGeometry;
        TGeoPara eegeom;
        MagneticField *magField;

        reco::SuperClusterRef sc;
        sc_xtal_information infos;

        double global_linkbyrechit_enlargement;
        float rotation_phi;
        int removePhotonsInMap_;

        static int findPFCandType( int id );

    };

}

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

