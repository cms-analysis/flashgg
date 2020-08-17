{
// Set up FW Lite for automatic loading of CMS libraries
    // and data formats.   As you may have other user-defined setup
    // in your rootlogon.C, the CMS setup is executed only if the CMS
    // environment is set up.
    //
    TString cmsswbase = getenv( "CMSSW_BASE" );
    if( cmsswbase.Length() > 0 )
    {
        //
        // The CMSSW environment is defined (this is true even for FW Lite)
        // so set up the rest.
        //
        cout << "Loading FW Lite setup." << endl;
        gSystem->Load( "libFWCoreFWLite.so" );
        AutoLibraryLoader::enable();
        gSystem->Load( "libDataFormatsFWLite.so" );
        gSystem->Load( "libDataFormatsPatCandidates.so" );
        gSystem->Load( "libflashggMicroAODFormats.so" );

    }

    TFile f( "myOutputFile.root" );
    TTree *Events = f.Get( "Events" );
//  Events->Print();
    Events->SetScanField( 0 );
    //  Events->Scan("flashggDiPhotonCandidates_flashggDiPhotons__FLASHggTEST.obj.pt_:flashggPhotons_flashggPhotons__FLASHggTEST.obj.zernike42:flashggPhotons_flashggPhotons__FLASHggTEST.obj.hcalTowersBehindClusters[0]");


#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "flashgg/DataFormats/interface/DiphotonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#endif

    TH1F *pho_hist0 = new TH1F( "phoPt", "phoPt", 70, 0, 180 );

    fwlite::Event ev( &f );


    for( ev.toBegin(); ! ev.atEnd(); ++ev )
    {
        fwlite::Handle<std::vector<flashgg::Photon> > objs_pho;
        fwlite::Handle<std::vector<flashgg::DiPhotonCandidate> > objs_dipho;
        fwlite::Handle<std::vector<reco::Vertex> > objs_vertex;
        fwlite::Handle<std::vector<pat::PackedCandidate> > objs_packcand;
        objs_pho.getByLabel( ev, "flashggPhotons" );
        objs_dipho.getByLabel( ev, "flashggDiPhotons" );
        objs_vertex.getByLabel( ev, "offlineSlimmedPrimaryVertices" );
//    objs_packcand.getByLabel(ev,"")
        std::cout  << "------------------------------------------------------------------------" << std::endl;
        // now can access data
        for( int i = 0; i < objs_pho->size() ; i++ ) {
            std::vector<flashgg::Photon> const &pho = *objs_pho;
            std::cout << "pt" <<   "   " << pho[i].pt() << std::endl;
            std::cout << "eta" <<  "   " << pho[i].eta() << std::endl;
            std::cout << "phi" <<  "   " << pho[i].phi() << std::endl;
            std::cout << "r9 " << "   " << pho[i].r9() << std::endl;
//    std::cout << "e1x5" << "   " << pho[i].e1x5() << std::endl;
//    std::cout << "e2x5" << "   " << pho[i].e2x5() << std::endl;
//    std::cout << "e5x5" << "   " << pho[i].e5x5() << std::endl;
//    std::cout << "e3x3" << "   " << pho[i].e3x3() << std::endl;
//    std::cout << "e1x3" << "   " << pho[i].getE1x3() << std::endl;
//    std::cout << "ebottom" << "   " << pho[i].getEbottom() << std::endl;
//    std::cout << "etop" << "   " << pho[i].getEtop() << std::endl;
//    std::cout << "eright" << "   " << pho[i].getEright() << std::endl;
//    std::cout << "e2x5bottom" << "   " << pho[i].getE2x5bottom() << std::endl;
//    std::cout << "e2x5top" << "   " << pho[i].getE2x5top() << std::endl;
//    std::cout << "e2x5left" << "   " << pho[i].getE2x5left() << std::endl;
//    std::cout << "e2x5right" << "   " << pho[i].getEright() << std::endl;
//    std::cout << "e2x5max" << "   " << pho[i].getE2x5max() << std::endl;
//    std::cout << "e2nd" << "   " << pho[i].getE2nd() << std::endl;
//    std::cout << "zernike42" << "   " << pho[i].getZernike42() << std::endl;
//    std::cout << "zernike20" << "   " << pho[i].getZernike20() << std::endl;
//    std::cout << "sieip" << "   " << pho[i].getSieip() << std::endl;
//    std::cout << "sipip" << "   " << pho[i].getSipip() << std::endl;
//    std::cout << "hcalDepth2OverEcalBc" << "   " << pho[i].hadTowDepth2OverEm() << std::endl;
//    std::cout << "hcalDepth1OverEcalBc" << "   " << pho[i].hadTowDepth1OverEm() << std::endl;
//    std::cout << "hcalDepth2OverEcal" << "   " << pho[i].hadronicDepth2OverEm() << std::endl;
//    std::cout << "hcalDepth1OverEcal" << "   " << pho[i].hadronicDepth1OverEm() << std::endl;
//    std::cout << "sigmaIetaIeta" << "   " << pho[i].sigmaIetaIeta() << std::endl;
//    std::cout << "sigmaEtaEta"   << "   " << pho[i].sigmaEtaEta() << std::endl;
//    std::cout << "maxEnergyXtal" << "   " << pho[i].maxEnergyXtal() << std::endl;

            pho_hist0->Fill( pho[i].pt() );

        }

        for( int i = 0; i < objs_dipho.ptr()->size(); i++ ) {
            std::vector<flashgg::DiPhotonCandidate> const &dipho = *objs_dipho;
            std::cout << "pt dipho" <<   "   " << dipho[i].pt() << std::endl;
//    std::cout << "eta dipho" <<  "   " << dipho[i].eta() << std::endl;
            std::cout << "phi dipho" <<  "   " << dipho[i].phi() << std::endl;
            std::cout << "ptasym dipho" <<  "   " << dipho[i].getPtAsym() << std::endl;
        }

        for( int i = 0; i < objs_vertex.ptr()->size(); i++ ) {
            std::vector<reco::Vertex> const &vertex = *objs_vertex;
//    std::cout << "chi2" <<   "   " << vertex[i].chi2() << std::endl;
//    std::cout << "x" <<   "   " << vertex[i].x() << std::endl;
//    std::cout << "y" <<   "   " << vertex[i].y() << std::endl;
//    std::cout << "z" <<   "   " << vertex[i].z() << std::endl;
//    std::cout << "xerror" << "   " <<  vertex[i].xError() << std::endl;
//    std::cout << "yerror" << "   " <<  vertex[i].yError() << std::endl;
//    std::cout << "zerror" << "   " <<  vertex[i].zError() << std::endl;
//    std::cout << "ndof" << "   " <<  vertex[i].ndof() << std::endl;

        }

    }

    TCanvas *c3 = new TCanvas();
    pho_hist0->Draw();
    c3->SaveAs( "pho_hist0.pdf" );

}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

