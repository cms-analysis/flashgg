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

			float getNonTrigMVA() const {return nontrigmva_;}
			void setNonTrigMVA(float val) { nontrigmva_ = val; }

			float getStandardHggIso() const { return PfRhoAreaCorrectedIso_; }
			void setStandardHggIso( float val ) { PfRhoAreaCorrectedIso_ = val; }

		private:
			float nontrigmva_;
			float PfRhoAreaCorrectedIso_;
	};
}	

#endif
