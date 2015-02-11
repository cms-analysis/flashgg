#ifndef flashgg_PhotonDumpers_h
#define flashgg_PhotonDumpers_h

#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"

#include "flashgg/TagAlgos/interface/CollectionDumper.h"

/// #include "PhysicsTools/UtilAlgos/interface/FWLiteAnalyzerWrapper.h"

namespace flashgg 
{ 
	typedef CollectionDumper<std::vector<Photon> > PhotonDumper;
	typedef CollectionDumper<std::vector<Photon>,
				 Photon,
				 CutBasedClassifier<Photon> > CutBasedPhotonDumper;
	typedef CollectionDumper<std::vector<DiPhotonCandidate> > DiPhotonDumper;
	typedef CollectionDumper<std::vector<DiPhotonCandidate>,
				 DiPhotonCandidate,
				 CutBasedClassifier<DiPhotonCandidate> > CutBasedDiPhotonDumper;
	typedef CollectionDumper<edm::OwnVector<DiPhotonTagBase>,
				 DiPhotonTagBase,
				 ClassNameClassifier<DiPhotonTagBase> > DiPhotonTagDumper;

	//// namespace fwlite {
	//// 	typedef ::fwlite::AnalyzerWrapper<DiPhotonDumper> DiPhotonDumper;
	//// 	typedef ::fwlite::AnalyzerWrapper<PhotonDumper> PhotonDumper;
	//// 	typedef ::fwlite::AnalyzerWrapper<DiPhotonTagDumper> DiPhotonTagDumper;
	//// }
	
}

#endif // flashgg_PhotonDumpers_h
