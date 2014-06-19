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
  }

  TFile f("myOutputFile.root");
  TTree *Events = f.Get("Events");
  Events->Print();
  Events->SetScanField(0);
  Events->Scan("flashggDiPhotonCandidates_flashggDiPhotons__FLASHggTEST.obj.pt_:flashggPhotons_flashggPhotons__FLASHggTEST.obj.pt_:flashggPhotons_flashggPhotons__FLASHggTEST.obj.e1x3");

#include "DataFormats/FWLite/interface/Handle.h"
  
  fwlite::Event ev(&f);

  int n = 0;

  for( ev.toBegin(); ! ev.atEnd(); ++ev) {
    fwlite::Handle<std::vector<flashgg::DiPhotonCandidate> > objs;
    objs.getByLabel(ev,"flashggDiPhotons");
    // now can access data
    cout << "We have " << objs.ptr().size() << " diPhotons in event " << n++ << endl;

    // Actually we can't access very much data
    // FWLite doesn't really know about flashgg objects, so the code below doesn't really work.
    // We'll leave figuring this out for another day

    //    for(std::vector<DiPhotonCandidate>::const_iterator dipho=objs->begin(); dipho != objs->end(); ++dipho) {
    //      cout << "  DiPhoton with pt " << dipho->pt() << endl;
    //    }
  }

}
