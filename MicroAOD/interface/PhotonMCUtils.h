#ifndef _PhotonMCUtils_h_
#define _PhotonMCUtils_h_

#include "DataFormats/Math/interface/deltaR.h"
#include "flashgg/DataFormats/interface/Photon.h"

namespace flashgg {

	class PhotonMCUtils {
		
	public:
		template<class GenT, class GenCollT> static float isoSum(const GenT & genp, const GenCollT & coll, float dRMax) {
			float sum = 0.;
			for(auto & part : coll) {
				if( reco::deltaR(genp,part) >= dRMax || part.p4() == genp.p4() ) { continue; }
				sum += part.et();
			}
			return sum;
		}
		
		template<class GenT, class GenCollT> static bool frixioneIso(const GenT & genp, const GenCollT & coll, float delta0, float eps0, float n0) {
			// Phys.Lett. B429 (1998) 369-374 - arXiv:hep-ph/9801442
			std::map<float,float> slices;
			for(auto & part : coll) {
				float dR = reco::deltaR(genp,part);
				if(dR  >= delta0 || part.p4() == genp.p4() ) { continue; }
				slices[dR] += part.et();
			}
			float sum = 0.;
			const float eTimesEps        = genp.et()*eps0;
			const float oneMinusCosDelta0 = 1 - cos(delta0);
			for( auto & slice : slices ) {
				auto& delta = slice.first;
				sum  += slice.second;
				const float chi = eTimesEps*pow( (1-cos(delta))/oneMinusCosDelta0, n0 );
				if( sum >= chi ) { return false; }
			}
			return true;
		}

		static void determineMatchType(flashgg::Photon & pho, flashgg::Photon::mcMatch_t defaultType=flashgg::Photon::kFake);
		template<class GenT> static flashgg::Photon::mcMatch_t determineMatchType(const GenT & gen, flashgg::Photon::mcMatch_t defaultType=flashgg::Photon::kFake) {
			auto matchType = defaultType;
			for(size_t imom=0; imom<gen.numberOfMothers(); ++imom) {
				int mstat = gen.mother(imom)->status();
				int mpdgId = abs(gen.mother(imom)->pdgId());
				if( mpdgId > 0 && mpdgId <= 25 && (mstat == 3 || mstat == 23 || mstat == 22) ) {
					matchType = flashgg::Photon::kPrompt;
				}
				if( mstat == 2 && mpdgId != 22 ) {
					matchType = flashgg::Photon::kFake;
				}
			}
			return matchType;
		}

		
	};


}


#endif // _PhotonMCUtils_h_
