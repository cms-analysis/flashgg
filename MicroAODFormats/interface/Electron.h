#ifndef FLASHgg_Electron_h
#define FLASHgg_Electron_h

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"

namespace flashgg {

	class Electron : public pat::Electron {

		public:
			Electron();
			Electron(const pat::Electron& );
			~Electron();

			float nontrigmva;

			float getNonTrigMVA() const {return nontrigmva;}

	};
}	

#endif
