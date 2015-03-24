#ifndef flashgg_GlobalVariablesComputer_h
#define flashgg_GlobalVariablesComputer_h

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace flashgg {

	class GlobalVariablesComputer {
	public:
		struct cache_t {
			float rho;
			int   nvtx;
		};
		
		GlobalVariablesComputer(const edm::ParameterSet & cfg);
		
		void update(const edm::EventBase & event);
		
		float * addressOf(const std::string & varName);
		
	protected:
		edm::InputTag rhoTag_, vtxTag_;
		cache_t cache_;
	};	
}

#endif // flashgg_GlobalVariablesComputer_h
