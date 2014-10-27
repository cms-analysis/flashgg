{
	// Set up FW Lite for automatic loading of CMS libraries
	// and data formats.   As you may have other user-defined setup
	// in your rootlogon.C, the CMS setup is executed only if the CMS
	// environment is set up.
	//

	TString cmsswbase = getenv("CMSSW_BASE");
	if (cmsswbase.Length() > 0) {
		//
		// The CMSSW environment is defined (this is true even for FW Lite)
		// so set up the rest.
		//
		cout << "Loading FW Lite setup." << endl;
		gSystem->Load("libFWCoreFWLite.so");
		AutoLibraryLoader::enable();
		gSystem->Load("libDataFormatsFWLite.so");
		gSystem->Load("libDataFormatsPatCandidates.so");
		gSystem->Load("libflashggMicroAODFormats.so"); 
                gSystem->Load("libflashggTagFormats.so");
	}

	TFile f("myOutputFile.root");

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "flashgg/MicroAODFormats/interface/DiphotonCandidate.h"  
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/TagFormats/interface/TTHleptonicTag.h"
#endif

	fwlite::Event ev(&f);
	int eventsCount = 0;

std::cout << "before loop " << std::endl;

	for( ev.toBegin(); ! ev.atEnd(); ++ev) 
{
		fwlite::Handle<std::vector<flashgg::Photon> > objs_pho;
		fwlite::Handle<std::vector<flashgg::DiPhotonCandidate> > objs_dipho;
		fwlite::Handle<std::vector<flashgg::TTHleptonicTag> > objs_tthltag;

		objs_pho.getByLabel(ev,"flashggPhotons");
		objs_dipho.getByLabel(ev,"flashggDiPhotons");
		objs_tthltag.getByLabel(ev,"flashggTTHleptonicTag");

		std::vector<flashgg::TTHleptonicTag> const & tthltag = *objs_tthltag;


	if (objs_tthltag.ptr()->size() != 0)
	{
		eventsCount++;

		std::cout << "objs_tthltag.ptr()->size() = " << objs_tthltag.ptr()->size() << std::endl;



		for(int i = 0; i< objs_tthltag.ptr()->size(); i++)
		{

			std::cout << tthltag[i]->getMuon()->pt() << std::endl;

		}
	}

} 
std::cout << "Events with TTHleptonicTag = " << eventsCount << std::endl;
}

