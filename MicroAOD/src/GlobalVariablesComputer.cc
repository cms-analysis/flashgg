#include "flashgg/MicroAOD/interface/GlobalVariablesComputer.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

using namespace edm;
using namespace reco;

namespace flashgg {


	GlobalVariablesComputer::GlobalVariablesComputer(const edm::ParameterSet & cfg) : 
		rhoTag_(cfg.getParameter<edm::InputTag>("rho")),
		vtxTag_(cfg.getParameter<edm::InputTag>("vertexes"))
	{
	}
	
	float * GlobalVariablesComputer::addressOf(const std::string & varName)
	{
		if( varName == "rho" ) { return &cache_.rho; }
		return 0;
	}

	void GlobalVariablesComputer::update(const EventBase & evt)
	{
		Handle<double> rhoHandle;
		evt.getByLabel(rhoTag_,rhoHandle);
		
		Handle<VertexCollection> vertices;
		evt.getByLabel(vtxTag_,vertices);

		
		cache_.rho = *rhoHandle;
		cache_.nvtx = vertices->size();
	}
}
