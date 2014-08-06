
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/MicroAODFormats/interface/VertexCandidateMap.h"
#include <fstream>

using namespace edm;
using namespace std;

namespace flashgg {

	class DzVertexMapValidator : public EDProducer 
	{

		public:
			DzVertexMapValidator( const ParameterSet & );
		private:
			void produce( Event &, const EventSetup & ) override;
			EDGetTokenT<View<reco::Vertex> > vertexToken_;
			EDGetTokenT<View<reco::Vertex> > vertexTokenAOD_;
			EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
			EDGetTokenT<View<pat::PackedCandidate> > lostTrackToken_;
			double maxAllowedDz_;
			bool useEachTrackOnce_;
	};

	DzVertexMapValidator::DzVertexMapValidator(const ParameterSet & iConfig) :
		vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
		vertexTokenAOD_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTagAOD", InputTag("offlinePrimaryVertices")))),
		pfcandidateToken_(consumes<View<pat::PackedCandidate> >(iConfig.getUntrackedParameter<InputTag> ("PFCandidatesTag", InputTag("packedPFCandidates")))),
		lostTrackToken_(consumes<View<pat::PackedCandidate> >(iConfig.getUntrackedParameter<InputTag> ("lostTrackTag", InputTag("lostTracks")))),
		maxAllowedDz_(iConfig.getParameter<double>("MaxAllowedDz")), // in cm
		useEachTrackOnce_(iConfig.getUntrackedParameter<bool>("UseEachTrackOnce",true))
	{
		produces<VertexCandidateMap>();
	}

	void DzVertexMapValidator::produce( Event & evt , const EventSetup & ) 
	{

		// Primary Vertices from original AOD  (can access tracks using tracks_begin() etc)
		Handle<View<reco::Vertex> > primaryVerticesAOD;
		evt.getByToken(vertexTokenAOD_,primaryVerticesAOD);
		const PtrVector<reco::Vertex>& pvPtrsAOD = primaryVerticesAOD->ptrVector();

		// Primary Vertices from miniAOD after AOD has been processed
		Handle<View<reco::Vertex> > primaryVertices;
		evt.getByToken(vertexToken_,primaryVertices);
		const PtrVector<reco::Vertex>& pvPtrs = primaryVertices->ptrVector();

		// Tracks from miniAOD
		Handle<View<pat::PackedCandidate> > pfCandidates;
		evt.getByToken(pfcandidateToken_,pfCandidates);
		const PtrVector<pat::PackedCandidate>& pfPtrs = pfCandidates->ptrVector();

		Handle<View<pat::PackedCandidate> > lostTracks;
		evt.getByToken(lostTrackToken_,lostTracks);
		const PtrVector<pat::PackedCandidate>& lostTrks = lostTracks->ptrVector();

		std::ofstream out ;
		out.open("out.txt",  std::ofstream::app);

		// Track number is used to know how many valid AOD tracks exist 
		int trackNumber =0;
	//	int lostCounter=0;
		for ( unsigned int i=0 ; i < pvPtrsAOD.size() ;i++)
		{
			//	int nTracks    = pvPtrsAOD[i]->nTracks()   ;
			//	int tracksSize = pvPtrsAOD[i]->tracksSize();
			//	std:: cout <<  nTracks   << std::endl;
			//	std:: cout <<  tracksSize<< std::endl;
			trackNumber = trackNumber + pvPtrsAOD[i]->tracksSize();
		}
		

		// Track number is used to know how many valid miniAOD tracks exist 
		int trackNumberMiniAOD =0;
		for (unsigned int i = 0 ; i < pfPtrs.size() ; i++) 
		{
			Ptr<pat::PackedCandidate> cand = pfPtrs[i];
			if (cand->charge() == 0) continue; // skip neutrals
			trackNumberMiniAOD++;
		}

		//used to store the final association from miniAOD vertice to miniAOD tracks (packed candidates)
		std::auto_ptr<VertexCandidateMap> assoc(new VertexCandidateMap);

		// print ifno for debugging
		std::cout << "AOD vtxs:  "<<  pvPtrsAOD.size() << ", miniAOD vtxs: " << pvPtrs.size() << ", AOD tracks: " << trackNumber <<", miniAOD tracks: " << trackNumberMiniAOD <<  std::endl;
		std::cout << "Lost :" << lostTrks.size() << std::endl;


		// ***************************************************************************
		// *** Begin creation of map between miniAOD Primary Vertices and AOD PVs. ***
		// ***************************************************************************

		// pvMap is a map between vertices from miniAOD to AOD in format: pvMap(miniAOD_PV_index)= AOD_PV_index;
		// Use int rather than unsigned int so that index can be -1 if no match.
		int pvMap[pvPtrs.size()]; 

		// x-,y-,zLim represent the minimum proximity for each coordiante for the PVs to be considered the same.	
		double xLim = 0.01 ;
		double yLim = 0.01 ;
		double zLim = 0.01 ;


//		std::cout << " ***** <VERTEX MATCHING> *****" << std:: endl;

		for (unsigned int i=0; i< pvPtrs.size() ; i++)
		{
			double xMiniAOD = pvPtrs[i]->x();
			double yMiniAOD = pvPtrs[i]->y();
			double zMiniAOD = pvPtrs[i]->z();

			// If no match is found, the index of corresponding entry is -1
			pvMap[i]= -1;

			for (unsigned int j=0; j< pvPtrsAOD.size(); j++)
			{
				double xAOD = pvPtrsAOD[j]->x();
				double yAOD = pvPtrsAOD[j]->y();
				double zAOD = pvPtrsAOD[j]->z();

				// require x,y, and z to simulatenously be within the limits for a match
				if( fabs(xAOD-xMiniAOD) < xLim && fabs(yAOD-yMiniAOD) < yLim && fabs(zAOD-zMiniAOD) < zLim )
				{
					//set index if there is a match
					pvMap[i]=j;

					if (pvMap[i]==-1){ std::cout << "no match" << std::endl;}
					//cout for debugging
			//			std::cout << "miniAOD PV index: " << i << " , AOD PV index: " << pvMap[i] << std::endl;
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
		int trkCounter =0;

		// myMap will be used to store association from AOD vertices to miniAOD packed candidate tracks
		std::multimap<Ptr<reco::Vertex>, Ptr<pat::PackedCandidate>> myMap;

		// eta-,phi-,ptLim represent the minimum proximity for each criterion for the tracks to be considered the same.	
		double etaLim = 0.001 ;
		double phiLim = 0.001 ;
		double ptLim = 0.001 ;

		for (unsigned int i =0; i< pvPtrsAOD.size() ; i++)
		{
			for(auto trackAOD=pvPtrsAOD[i]->tracks_begin(); trackAOD != pvPtrsAOD[i]->tracks_end(); trackAOD++)		
			{
		

				double etaAOD = (**trackAOD).eta();
				double phiAOD = (**trackAOD).phi();
				double ptAOD = (**trackAOD).pt();


				//	std::cout << "eta "<< etaAOD << " , phi " << phiAOD << " , pt " << ptAOD << std::endl;
				// If no match is found, the index of correcsponding entry is -1
				trkCounter++;
				trkMap[trkCounter]= -1;

				for (unsigned int j=0; j< pfPtrs.size(); j++)
				{
					double	etaMiniAOD = pfPtrs[j]->eta();
					double	phiMiniAOD = pfPtrs[j]->phi();
					double	ptMiniAOD = pfPtrs[j]->pt();

					// require x,y, and z to simulatenously be within the limits for a match
					if( fabs(etaAOD-etaMiniAOD) < etaLim && fabs(phiAOD-phiMiniAOD) < phiLim && fabs(ptAOD-ptMiniAOD) < ptLim )
					{
						//set index if there is a match
							trkMap[trkCounter]=j;
						//std::cout << " Track : " << trkCounter << " MATCH to " << j << std::endl;
						//std::cout << etaAOD << " | " << etaMiniAOD << " - " << phiAOD << " | " <<phiMiniAOD << " - " << ptAOD << " | " << ptMiniAOD << std::endl;


						//break loop if there is a match
						myMap.insert(std::pair<Ptr<reco::Vertex>,Ptr<pat::PackedCandidate>>(pvPtrsAOD[i],pfPtrs[j]));
						break;
					}
				}
					if ( trkMap[trkCounter]==-1) {std::cout << "Track : " << trkCounter << " NO MATCH "<< std::endl;} 
			}
		}

		// ************************************************************************************
		// *** create map between miniAOD vertices and Packed Candidate Tracks via AOD info ***
		// ************************************************************************************

		double matchCounter=0;

		// loop over miniAOD PVs
		for (unsigned int i = 0 ; i < pvPtrs.size() ; i++) 
		{

			// Grab ith PV in miniAOD
			Ptr<reco::Vertex> vtx = pvPtrs[i];
			// use map created above to find corresponding PV index in AOD;
			unsigned int index = pvMap[i];

			// define a range to be accesses by the multimap: the range returns an interator to loop over all elements corresponding to a goiven key, in this case the AOD vertex.
			std::pair <std::multimap<Ptr<reco::Vertex>, Ptr<pat::PackedCandidate>>::iterator,std::multimap<Ptr<reco::Vertex>, Ptr<pat::PackedCandidate>>::iterator> range;
			// get range by feeding it the AOD vertex.
			range = myMap.equal_range(pvPtrsAOD[index]);
			matchCounter = matchCounter + myMap.count(pvPtrsAOD[index]);
		//	{std::cout << myMap.count(pvPtrsAOD[index]) << "	" << myMap.count(pvPtrsAOD[i]) << "	" << pvPtrsAOD[index]->tracksSize() <<std::endl;}

			PtrVector<pat::PackedCandidate> finalTracks;
			int tempCounter =0;
			// use iterators to loop over this range.
			for (std::multimap<Ptr<reco::Vertex>,Ptr<pat::PackedCandidate>>::iterator fillLoop = range.first; fillLoop != range.second; fillLoop++)
			{
				tempCounter++;	
				finalTracks.push_back(fillLoop->second);
			}
			// finally, fill the assoc with the pair < vertex, track > for each corresponding track;
			assoc->insert (std::make_pair(vtx,finalTracks));
			//assoc->insert (std::pair<<Ptr<reco::Vertex>,PtrVector<pat::PackedCandidate>>(vtx,finalTracks));
		}


		std:: cout << "matched tracks : " << matchCounter  << std:: endl;

		//		if (trkCounter > trackNumber) { std::cout << " [ISSUE] " << std::endl;}
		evt.put(assoc);

		out << trackNumber << "	" << trackNumberMiniAOD << "	" << matchCounter << std::endl;
	}
}

typedef flashgg::DzVertexMapValidator FlashggDzVertexMapValidator;
DEFINE_FWK_MODULE(FlashggDzVertexMapValidator);
