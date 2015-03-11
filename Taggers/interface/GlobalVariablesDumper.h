#ifndef flashgg_GlobalVariablesDumper_h
#define flashgg_GlobalVariablesDumper_h

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TTree;

namespace flashgg {
	
	class GlobalVariablesDumper {
	public:
		struct DumpCache {
			float rho;
			int   nvtx;
		};
		
		GlobalVariablesDumper(const edm::ParameterSet & cfg);
		~GlobalVariablesDumper();
		
		void bookTreeVariables(TTree * target, const std::map<std::string,std::string> & replacements);

		void fill(const edm::EventBase & event);
		
	private:
		edm::InputTag rhoTag_, vtxTag_;
		DumpCache cache_;
	};	

}

#endif  // flashgg_GlobalVariablesDumper_h
