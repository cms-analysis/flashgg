#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "flashgg/MicroAODFormats/interface/Electron.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "EgammaAnalysis/ElectronTools/interface/EGammaMvaEleEstimator.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/MicroAODFormats/interface/VertexCandidateMap.h"


using namespace std;
using namespace edm;

namespace flashgg {

		class ElectronProducer : public EDProducer {

		public:
		ElectronProducer( const ParameterSet & );
		private:
		void produce( Event &, const EventSetup & ) override;
		EDGetTokenT<View<pat::Electorn> > ElectronToken_;
		EDGetTokenT<View<

