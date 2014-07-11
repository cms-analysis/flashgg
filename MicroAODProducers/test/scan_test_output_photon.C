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
}

  TFile f("myOutputFile.root");
  TTree *Events = f.Get("Events");
  Events->Print();
  Events->SetScanField(0);
  Events->Scan("flashggDiPhotonCandidates_flashggDiPhotons__FLASHggTEST.obj.pt_:flashggPhotons_flashggPhotons__FLASHggTEST.obj.pt_:flashggPhotons_flashggPhotons__FLASHggTEST.obj.full5x5_showerShape_.sigmaIetaIeta:recoVertexs_offlineSlimmedPrimaryVertices__PAT.obj.chi2_");
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include <typeinfo>
#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "flashgg/MicroAODFormats/interface/DiphotonCandidate.h"  
#include "flashgg/MicroAODFormats/interface/Photon.h"  
#endif

  fwlite::Event ev(&f);

  int n = 0;

for( ev.toBegin(); ! ev.atEnd(); ++ev) {
    fwlite::Handle<std::vector<flashgg::Photon> > objs_pho;
    fwlite::Handle<std::vector<flashgg::DiPhotonCandidate> > objs_dipho;
    fwlite::Handle<std::vector<reco::Vertex> > objs_vertex;
    fwlite::Handle<std::vector<pat::PackedCandidate> > objs_packcand;
    objs_pho.getByLabel(ev,"flashggPhotons");
    objs_dipho.getByLabel(ev,"flashggDiPhotons");
    objs_vertex.getByLabel(ev,"offlineSlimmedPrimaryVertices"); 
    objs_packcand.getByLabel(ev,"flashggVertexMapNonUnique"); 

    std::cout << "-----------------------" << std::endl;
   
    // now can access data
    for (int i=0; i < objs_pho.ptr()->size();i++ ){
    std::vector<flashgg::Photon> const & pho = *objs_pho;
    std::cout << "pt" <<   "   " << pho[i].pt() << std::endl;
    std::cout << "eta" <<  "   " << pho[i].eta() << std::endl;
    std::cout << "phi" <<  "   " << pho[i].phi() << std::endl;
    std::cout << "e1x5" << "   " << pho[i].e1x5() << std::endl;
    std::cout << "e2x5" << "   " << pho[i].e2x5() << std::endl;
    std::cout << "e5x5" << "   " << pho[i].e5x5() << std::endl;
    std::cout << "e3x3" << "   " << pho[i].e3x3() << std::endl;
    std::cout << "sigmaIetaIeta" << "   " << pho[i].sigmaIetaIeta() << std::endl;
    std::cout << "sigmaEtaEta"   << "   " << pho[i].sigmaEtaEta() << std::endl;
    std::cout << "maxEnergyXtal" << "   " << pho[i].maxEnergyXtal() << std::endl;
       }
    for (int i=0; i < objs_dipho.ptr()->size();i++ ){
    std::vector<flashgg::DiPhotonCandidate> const & dipho = *objs_dipho;
    std::cout << "pt dipho" <<   "   " << dipho[i].pt() << std::endl;
    std::cout << "eta dipho" <<  "   " << dipho[i].eta() << std::endl;
    std::cout << "phi dipho" <<  "   " << dipho[i].phi() << std::endl;
    }
    for (int i=0; i < objs_vertex.ptr()->size();i++ ){
    std::vector<reco::Vertex> const & vertex = *objs_vertex;
    std::cout << "chi2" <<   "   " << vertex[i].chi2() << std::endl;
     } 







    }

}

