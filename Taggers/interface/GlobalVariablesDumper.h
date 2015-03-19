#ifndef flashgg_GlobalVariablesDumper_h
#define flashgg_GlobalVariablesDumper_h

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "flashgg/MicroAOD/interface/GlobalVariablesComputer.h"

class TTree;

namespace flashgg {
	
	class GlobalVariablesDumper : public GlobalVariablesComputer {
	public:
		GlobalVariablesDumper(const edm::ParameterSet & cfg);
		~GlobalVariablesDumper();
		
		void bookTreeVariables(TTree * target, const std::map<std::string,std::string> & replacements);
		
		void fill(const edm::EventBase & event);
	};	

}

#endif  // flashgg_GlobalVariablesDumper_h
