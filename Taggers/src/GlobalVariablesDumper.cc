#include "flashgg/TagAlgos/interface/GlobalVariablesDumper.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "TTree.h"

using namespace edm;
using namespace reco;

namespace flashgg {


	GlobalVariablesDumper::GlobalVariablesDumper(const edm::ParameterSet & cfg) : 
		rhoTag_(cfg.getParameter<edm::InputTag>("rho")),
		vtxTag_(cfg.getParameter<edm::InputTag>("vertexes"))
	{
	}
	
	GlobalVariablesDumper::~GlobalVariablesDumper()
	{}

	void GlobalVariablesDumper::bookTreeVariables(TTree *tree, const std::map<std::string,std::string> & replacements)
	{
		tree->Branch("rho",&cache_.rho);
		tree->Branch("nvtx",&cache_.nvtx);
	}

	void GlobalVariablesDumper::fill(const EventBase & evt)
	{
		Handle<double> rhoHandle;
		evt.getByLabel(rhoTag_,rhoHandle);
		
		Handle<VertexCollection> vertices;
		evt.getByLabel(vtxTag_,vertices);

		
		cache_.rho = *rhoHandle;
		cache_.nvtx = vertices->size();
	}


}
