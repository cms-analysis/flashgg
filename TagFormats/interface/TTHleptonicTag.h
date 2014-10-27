#ifndef FLASHgg_TTHleptonicTag_h
#define FLASHgg_TTHleptonicTag_h

#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

namespace flashgg {

	class TTHleptonicTag: public DiPhotonTagBase {
		public:
			TTHleptonicTag();
                        TTHleptonicTag(edm::Ptr<pat::Muon>);
			~TTHleptonicTag();

                const edm::Ptr<pat::Muon> getMuon() const { return theMuon_;} 

                private:
                edm::Ptr<pat::Muon> theMuon_;  
	};
} 

#endif 

