
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/Math/interface/deltaR.h"


using namespace edm;
using namespace std;

namespace flashgg {

    class VertexMapValidator : public EDProducer
    {

    public:
        VertexMapValidator( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<reco::Vertex> > vertexTokenAOD_;
        EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
        edm::EDGetTokenT<edm::Association<pat::PackedCandidateCollection> > map_;
        bool useMiniAODTrackVertexAssociation_;
        bool doTextDebug_; // Not compatible with actually outputting results!
        //			double maxAllowedDz_;
        //			bool useEachTrackOnce_;
    };

    VertexMapValidator::VertexMapValidator( const ParameterSet &iConfig ) :
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        vertexTokenAOD_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTagAOD" ) ) ),
        pfcandidateToken_( consumes<View<pat::PackedCandidate> >( iConfig.getParameter<InputTag> ( "PFCandidatesTag" ) ) ),
        map_( consumes<edm::Association<pat::PackedCandidateCollection> >( iConfig.getParameter<InputTag> ( "PFCandidatesTag" ) ) ),
        //		maxAllowedDz_(iConfig.getParameter<double>("MaxAllowedDz")), // in cm
        //		useEachTrackOnce_(iConfig.getUntrackedParameter<bool>("UseEachTrackOnce",true))
        useMiniAODTrackVertexAssociation_( iConfig.getParameter<bool>( "UseMiniAODTrackVertexAssociation" ) ),
        doTextDebug_( iConfig.getUntrackedParameter<bool>( "DoTextDebug", false ) )
    {
        produces<VertexCandidateMap>();
    }

    void VertexMapValidator::produce( Event &evt , const EventSetup & )
    {

        // Primary Vertices from original AOD  (can access tracks using tracks_begin() etc)
        Handle<View<reco::Vertex> > primaryVerticesAOD;
        evt.getByToken( vertexTokenAOD_, primaryVerticesAOD );
        // 	const PtrVector<reco::Vertex>& pvPtrsAOD = primaryVerticesAOD->ptrVector();

        // Primary Vertices from miniAOD after AOD has been processed
        Handle<View<reco::Vertex> > primaryVertices;
        evt.getByToken( vertexToken_, primaryVertices );
        //	const PtrVector<reco::Vertex>& primaryVertices = primaryVertices->ptrVector();

        // Tracks from miniAOD
        Handle<View<pat::PackedCandidate> > pfCandidates;
        evt.getByToken( pfcandidateToken_, pfCandidates );
        //	const PtrVector<pat::PackedCandidate>& pfPtrs = pfCandidates->ptrVector();

        // Association between tracks and PackedCandidates
        // Treatment from https://github.com/cms-sw/cmssw/blob/743bde9939bbc4f34c2b4fd022cfb980314f912a/PhysicsTools/PatAlgos/plugins/PATSecondaryVertexSlimmer.cc
        // Comes from same producer as packed candidates themselves, so map_ is actually the same as pfcandidateToken_
        edm::Handle<edm::Association<pat::PackedCandidateCollection> > pf2pc;
        evt.getByToken( map_, pf2pc );

        // Track number is used to know how many valid AOD tracks exist
        int trackNumber = 0;
        //	int lostCounter=0;
        for( unsigned int i = 0 ; i < primaryVerticesAOD->size() ; i++ ) {
            //	int nTracks    = primaryVerticesAOD->ptrAt(i)->nTracks()   ;
            //	int tracksSize = primaryVerticesAOD->ptrAt(i)->tracksSize();
            //	std:: cout <<  nTracks   << std::endl;
            //	std:: cout <<  tracksSize<< std::endl;
            trackNumber = trackNumber + primaryVerticesAOD->ptrAt( i )->tracksSize();
        }


        // Track number is used to know how many valid miniAOD tracks exist
        int trackNumberMiniAOD = 0;
        for( unsigned int i = 0 ; i < pfCandidates->size() ; i++ ) {
            Ptr<pat::PackedCandidate> cand = pfCandidates->ptrAt( i );
            if( cand->charge() == 0 ) { continue; } // skip neutrals
            //			if (abs(cand->pdgId()) != 211) continue; // skip neutrals
            trackNumberMiniAOD++;
        }

        //used to store the final association from miniAOD vertice to miniAOD tracks (packed candidates)
        std::unique_ptr<VertexCandidateMap> assoc( new VertexCandidateMap );

        // print ifno for debugging
        std::cout << "AOD vtxs:  " <<  primaryVerticesAOD->size() << ", miniAOD vtxs: " << primaryVertices->size() << ", AOD tracks: " << trackNumber <<
                  ", miniAOD tracks: " << trackNumberMiniAOD <<  std::endl;


        // ***************************************************************************
        // *** Begin creation of map between miniAOD Primary Vertices and AOD PVs. ***
        // ***************************************************************************

        // pvMap is a map between vertices from miniAOD to AOD in format: pvMap(miniAOD_PV_index)= AOD_PV_index;
        // Use int rather than unsigned int so that index can be -1 if no match.
        int pvMap[primaryVertices->size()];

        // x-,y-,zLim represent the minimum proximity for each coordiante for the PVs to be considered the same.
        double xLim = 0.01 ;
        double yLim = 0.01 ;
        double zLim = 0.01 ;

        for( unsigned int i = 0; i < primaryVertices->size() ; i++ ) {
            double xMiniAOD = primaryVertices->ptrAt( i )->x();
            double yMiniAOD = primaryVertices->ptrAt( i )->y();
            double zMiniAOD = primaryVertices->ptrAt( i )->z();

            // If no match is found, the index of corresponding entry is -1
            pvMap[i] = -1;

            for( unsigned int j = 0; j < primaryVerticesAOD->size(); j++ ) {
                double xAOD = primaryVerticesAOD->ptrAt( j )->x();
                double yAOD = primaryVerticesAOD->ptrAt( j )->y();
                double zAOD = primaryVerticesAOD->ptrAt( j )->z();

                // require x,y, and z to simulatenously be within the limits for a match
                if( fabs( xAOD - xMiniAOD ) < xLim && fabs( yAOD - yMiniAOD ) < yLim && fabs( zAOD - zMiniAOD ) < zLim ) {
                    //set index if there is a match
                    pvMap[i] = j;

                    if( pvMap[i] == -1 ) { std::cout << "no match" << std::endl;}
                    //cout for debugging
                    //		std::cout << "miniAOD PV index: " << i << " , AOD PV index: " << pvMap[i] << std::endl;
                    //std::cout << " ("<< xAOD <<", "<<yAOD<<", "<<zAOD<<")"<< std::endl;
                    //std::cout << " ("<< xMiniAOD <<", "<<yMiniAOD<<", "<<zMiniAOD<<")"<< std::endl;
                    //break loop if there is a match
                    break;
                }
            }

        }
        //std::cout << " ***** </VERTEX MATCHING> *****" << std:: endl;
        // ********************************************************
        // *** create map between AOD vertices and miniAOD tracks ***
        // ********************************************************
        // could eventually be conbined with above step in future.

        // trkMap is a map between tracks from AOD to tracks in miniAOD.
        // to keep approx same nomenclature as in pvMap, use number of tracks in AOD as size of map.
        //  MIGHT NOT BE USED IN FINAL VERSION, see multimap below
        int trkMap[trackNumber];
        int trkCounter = 0;

        // myMap will be used to store association from AOD vertices to miniAOD packed candidate tracks
        std::multimap<Ptr<reco::Vertex>, Ptr<pat::PackedCandidate>> myMap;
        myMap.clear();

        // eta-,phi-,ptLim represent the minimum proximity for each criterion for the tracks to be considered the same.
        double etaLim = 0.01 ;
        double phiLim = 0.01 ;
        double ptLim = 0.01 ;
        double matchCounter = 0;

        for( unsigned int i = 0; i < primaryVerticesAOD->size() ; i++ ) {
            if( doTextDebug_ ) { std::cout << "Vertex " << i << std::endl; }
            for( auto trackAOD = primaryVerticesAOD->ptrAt( i )->tracks_begin(); trackAOD != primaryVerticesAOD->ptrAt( i )->tracks_end(); trackAOD++ ) {
                double etaAOD = ( **trackAOD ).eta();
                double phiAOD = ( **trackAOD ).phi();
                double ptAOD = ( **trackAOD ).pt();


                // If no match is found, the index of correcsponding entry is -1
                trkCounter++;
                trkMap[trkCounter] = -1;

                if( doTextDebug_ ) { std::cout << "  Track : " << trkCounter << " eta " << etaAOD << " , phi " << phiAOD << " , pt " << ptAOD << std::endl; }

                if( useMiniAODTrackVertexAssociation_ ) { // Seth's adaptation to use pf2pc
                    if( ( *pf2pc )[*trackAOD].isNonnull() && ( *pf2pc )[*trackAOD]->charge() != 0 ) {

                        edm::Ptr<pat::PackedCandidate> pfPtr = edm::refToPtr<pat::PackedCandidateCollection>( ( *pf2pc )[*trackAOD] );

                        //				    trkMap[trkCounter] = j; // Doesn't do anything in this case - is it needed later?

                        if( doTextDebug_ ) {
                            double  etaMiniAOD = ( *pf2pc )[*trackAOD]->eta();
                            double  phiMiniAOD = ( *pf2pc )[*trackAOD]->phi();
                            double  ptMiniAOD = ( *pf2pc )[*trackAOD]->pt();
                            std::cout << "    Track : " << trkCounter << " pf2pc MATCH eta " << etaMiniAOD << " , phi " << phiMiniAOD << " , pt " << ptMiniAOD << std::endl;
                        }

                        myMap.insert( std::pair<Ptr<reco::Vertex>, Ptr<pat::PackedCandidate>>( primaryVerticesAOD->ptrAt( i ), pfPtr ) );
                        matchCounter++;

                    } else {
                        std::cout << "    Track : " << trkCounter << " NO pf2pc MATCH " << std::endl;
                    }
                }
                if( !useMiniAODTrackVertexAssociation_ || doTextDebug_ ) {
                    // Louie's original version
                    for( unsigned int j = 0; j < pfCandidates->size(); j++ ) {
                        double	etaMiniAOD = pfCandidates->ptrAt( j )->eta();
                        double	phiMiniAOD = pfCandidates->ptrAt( j )->phi();
                        double	ptMiniAOD = pfCandidates->ptrAt( j )->pt();

                        // require x,y, and z to simulatenously be within the limits for a match
                        if( pfCandidates->ptrAt( j )->charge() != 0 && // abs(pfCandidates->ptraT(j)->pdgId()) == 211 &&
                                fabs( etaAOD - etaMiniAOD ) < etaLim && reco::deltaPhi( **trackAOD, *pfCandidates->ptrAt( j ) ) < phiLim && fabs( ptAOD - ptMiniAOD ) < ptLim ) {
                            //set index if there is a match
                            trkMap[trkCounter] = j;
                            matchCounter++;
                            if( doTextDebug_ ) {
                                std::cout << "    Track : " << trkCounter << " eta/phi/pt MATCH to " << j << " :";
                                std::cout << "      " << etaMiniAOD << " , " << phiMiniAOD << " , " << ptMiniAOD << std::endl;
                            } else {
                                //break loop if there is a match, unless we're doing text debugging
                                // Then don't store any output at all!
                                myMap.insert( std::pair<Ptr<reco::Vertex>, Ptr<pat::PackedCandidate>>( primaryVerticesAOD->ptrAt( i ), pfCandidates->ptrAt( j ) ) );
                                break;
                            }
                        }
                    }
                    if( trkMap[trkCounter] == -1 ) {std::cout << "    Track : " << trkCounter << " NO eta/phi/pt MATCH " << std::endl;}
                }
            }
        }
        // ************************************************************************************
        // *** create map between miniAOD vertices and Packed Candidate Tracks via AOD info ***
        // ************************************************************************************

        matchCounter = 0;
        // loop over miniAOD PVs
        for( unsigned int i = 0 ; i < primaryVertices->size() ; i++ ) {

            // Grab ith PV in miniAOD
            Ptr<reco::Vertex> vtx = primaryVertices->ptrAt( i );
            // use map created above to find corresponding PV index in AOD;
            unsigned int index = pvMap[i];

            // define a range to be accesses by the multimap: the range returns an interator to loop over all elements corresponding to a goiven key, in this case the AOD vertex.
            std::pair <std::multimap<Ptr<reco::Vertex>, Ptr<pat::PackedCandidate>>::iterator, std::multimap<Ptr<reco::Vertex>, Ptr<pat::PackedCandidate>>::iterator> range;
            // get range by feeding it the AOD vertex.
            range = myMap.equal_range( primaryVerticesAOD->ptrAt( index ) );
            matchCounter = matchCounter + myMap.count( primaryVerticesAOD->ptrAt( index ) );
            //	{std::cout << myMap.count(primaryVerticesAOD->ptrAt(index)) << "	" << myMap.count(primaryVerticesAOD->ptrAt(i)) << "	" << primaryVerticesAOD->ptrAt(index)->tracksSize() <<std::endl;}

            //			PtrVector<pat::PackedCandidate> finalTracks;
            int tempCounter = 0;
            // use iterators to loop over this range.
            for( std::multimap<Ptr<reco::Vertex>, Ptr<pat::PackedCandidate>>::iterator fillLoop = range.first; fillLoop != range.second; fillLoop++ ) {
                tempCounter++;
                //				finalTracks.push_back(fillLoop->second);
                assoc->emplace_back( vtx, fillLoop->second );
            }
            // finally, fill the assoc with the pair < vertex, track > for each corresponding track;
            //			assoc->insert (std::make_pair(vtx,finalTracks));
            //assoc->insert (std::pair<<Ptr<reco::Vertex>,PtrVector<pat::PackedCandidate>>(vtx,finalTracks));
        }


        std:: cout << "matched tracks : " << matchCounter  << std:: endl;

        //		if (trkCounter > trackNumber) { std::cout << " [ISSUE] " << std::endl;}
        std::stable_sort( assoc->begin(), assoc->end(), flashgg::compare_by_vtx() );
        evt.put( std::move( assoc ) );

        cout << trackNumber << "	" << trackNumberMiniAOD << "	" << matchCounter << std::endl;
    }
}

typedef flashgg::VertexMapValidator FlashggVertexMapValidator;
DEFINE_FWK_MODULE( FlashggVertexMapValidator );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

