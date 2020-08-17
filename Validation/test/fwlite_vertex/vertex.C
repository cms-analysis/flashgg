{
    // Set up FW Lite for automatic loading of CMS libraries
    // and data formats.   As you may have other user-defined setup
    // in your rootlogon.C, the CMS setup is executed only if the CMS
    // environment is set up.
    //

    using namespace std;

    TString cmsswbase = getenv( "CMSSW_BASE" );
    if( cmsswbase.Length() > 0 )
    {
        //
        // The CMSSW environment is defined (this is true even for FW Lite)
        // so set up the rest.
        //
        std::cout << "Loading FW Lite setup." << std::endl;
        gSystem->Load( "libFWCoreFWLite.so" );
        AutoLibraryLoader::enable();
        gSystem->Load( "libDataFormatsFWLite.so" );
        gSystem->Load( "libDataFormatsPatCandidates.so" );
        gSystem->Load( "libflashggMicroAODFormats.so" );

    }

    //  TFile f("../../MicroAODProducers/test/myOutputFile.root");
    TFile f( "/tmp/carrillo/myOutputFileBig.root" );
    TTree *Events = f.Get( "Events" );
    //Events->Print();
    Events->SetScanField( 0 );
    //Events->Scan("flashggDiPhotonCandidates_flashggDiPhotons__FLASHggTEST.obj.pt_:flashggPhotons_flashggPhotons__FLASHggTEST.obj.zernike42:flashggPhotons_flashggPhotons__FLASHggTEST.obj.hcalTowersBehindClusters[0]");


#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "flashgg/DataFormats/interface/DiphotonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#endif

    gStyle->SetOptStat( 0 );

    TH1F *histo_sumpt2 = new TH1F( "sumpt2 in", "sumpt2", 100, 0., 15. );
    TH1F *histo_ptbal = new TH1F( "ptbal normalized", "ptbal normalized", 100, -200., 350. );
    TH1F *histo_ptasym = new TH1F( "ptasym normalized", "ptasym normalized", 100, -1, 1 );
    TH1F *histo_pull_conv = new TH1F( "pull conv", "pull conv", 100, 0., 100. );
    TH1F *histo_vtxprobmva = new TH1F( "vtxprobmva", "vtxprobmva", 20, -1., 1. );
    TH1F *histo_vtxeff_hpt = new TH1F( "vtxeff vs higgs pt", "vtxeff vs higgs pt", 100, 0, 250. );

    TH1F *histo_gen_matched_sumpt2 = new TH1F( "genmatch_sumpt2", "sumpt2", 100, -5., 15. );
    TH1F *histo_gen_matched_ptbal = new TH1F( "genmatch_ptbal_normalized", "ptbal normalized", 100, -200., 350. );
    TH1F *histo_gen_matched_ptasym = new TH1F( "genmatch_ptasym_normalized", "ptasym normalized", 100, -1., 1. );
    TH1F *histo_gen_matched_pull_conv = new TH1F( "genmatch_pullconv", "pull conv", 100, 0., 100. );
    TH1F *histo_gen_matched_vtxprobmva = new TH1F( "genmatch_vtxprobmva", "vtxprobmva", 20, -1., 1. );
    TH1F *histo_gen_matched_vtxeff_hpt = new TH1F( "genmatch_vtxeffVsHiggsPt", "vtxeff vs higgs pt", 100, 0., 250. );

    TH1F *histo_nconv = new TH1F( "nconv", "nconv", 3, -0.5, 2.5 );
    TH1F *histo_ndiphotons = new TH1F( "ndiphotons", "ndiphotons", 31, -0.5, 30.5 );

    TH1F *histo_residuals_z_nconv_0 = new TH1F( "residuals_z_0_conv", "residuals_z_0_conv", 100, -50., 50. );
    TH1F *histo_residuals_z_nconv_1 = new TH1F( "residuals_z_1_conv", "residuals_z_1_conv", 100, -50., 50. );
    TH1F *histo_residuals_z_nconv_2 = new TH1F( "residuals_z_2_conv", "residuals_z_2_conv", 100, -50., 50. );

    int contador = 0;

    fwlite::Event ev( &f );

    for( ev.toBegin(); ! ev.atEnd(); ++ev )
    {
        fwlite::Handle<vector<flashgg::Photon> > objs_pho;
        fwlite::Handle<vector<flashgg::DiPhotonCandidate> > objs_dipho;
        fwlite::Handle<vector<reco::Vertex> > objs_vertex;
        fwlite::Handle<vector<reco::GenParticle> > objs_genpart;

        objs_pho.getByLabel( ev, "flashggPhotons" );
        objs_dipho.getByLabel( ev, "flashggDiPhotons" );
        objs_vertex.getByLabel( ev, "offlineSlimmedPrimaryVertices" );
        objs_genpart.getByLabel( ev, "flashggPrunedGenParticles" );

        vector<reco::GenParticle> const &genpart = *objs_genpart;


        contador++;
        /*if(contador>=3000){
          std::cout<<"imprimo finished"<<std::endl;
          std::cout<<contador<<std::endl;
          break;
          }*/



        std::cout << contador << std::endl;

        float z_higgs = 0;
        for( int k = 0; k < objs_genpart.ptr()->size(); k++ ) {
            if( genpart[k].pdgId() == 25 ) { z_higgs = genpart[k].vz(); }
        }

        vector<flashgg::DiPhotonCandidate> const &dipho = *objs_dipho;


        bool gen_matched = false;

        histo_ndiphotons->Fill( int( objs_dipho.ptr()->size() ) );
        for( int i = 0; i < objs_dipho.ptr()->size(); i++ ) {
            histo_nconv->Fill( dipho[i].getNConv() );
            if( dipho[i].getNConv() == 0 ) {
                histo_residuals_z_nconv_0->Fill( dipho[i].getVertex()->position().z() - z_higgs );
            } else if( dipho[i].getNConv() == 1 ) {
                histo_residuals_z_nconv_1->Fill( dipho[i].getVertex()->position().z() - z_higgs );
            } else if( dipho[i].getNConv() == 2 ) {
                histo_residuals_z_nconv_2->Fill( dipho[i].getVertex()->position().z() - z_higgs );
            }
            if( fabs( dipho[i].getVertex()->position().z() - z_higgs ) <= 1. )
            { gen_matched = true; }
            else
            { gen_matched = false; }

            histo_vtxeff_hpt->Fill( dipho[i].mass() );
            histo_vtxprobmva->Fill( dipho[i].getVtxProbMVA() );
            if( gen_matched ) {
                histo_gen_matched_sumpt2->Fill( dipho[i].getLogSumPt2() );
                histo_gen_matched_ptbal->Fill( dipho[i].getPtBal() );
                histo_gen_matched_ptasym->Fill( dipho[i].getPtAsym() );
                histo_gen_matched_vtxeff_hpt->Fill( dipho[i].mass() );
                if( dipho[i].getNConv() != 0 ) {
                    histo_gen_matched_pull_conv->Fill( dipho[i].getPullConv() );
                }
                histo_gen_matched_vtxprobmva->Fill( dipho[i].getVtxProbMVA() );

                /*
                  if(fabs(pho->superCluster()->eta()<1.5)) {
                  if (perp<=15.0) {
                  //Pixel Barrel
                  } else if  (perp>15 && perp<=60.0) {
                  //Tracker Inner Barrel
                  }else{
                  //Tracker Outer Barrel
                  }
                  }else{
                  if(fabs(conversion->conversionVertex().z())<=50.0){
                  //Pixel Forward
                  }else if(fabs(conversion->conversionVertex().z())>50.0 && fabs(conversion->conversionVertex().z())<=100.0){
                  //Tracker Inner Disk
                  }else{
                  //Track EndCap
                  }
                */

            } else {
                histo_sumpt2->Fill( dipho[i].getLogSumPt2() );
                histo_ptbal->Fill( dipho[i].getPtBal() );
                histo_ptasym->Fill( dipho[i].getPtAsym() );
                if( dipho[i].getNConv() != 0 ) {
                    histo_pull_conv->Fill( dipho[i].getPullConv() );
                }
            }
        }
        dipho.clear();
        genpart.clear();
    }

    theFileOut = new TFile( "output_vertex.root", "RECREATE" );
    theFileOut->cd();

    histo_gen_matched_sumpt2->SetLineColor( kBlue );
    histo_gen_matched_ptbal->SetLineColor( kBlue );
    histo_gen_matched_ptasym->SetLineColor( kBlue );
    histo_gen_matched_pull_conv->SetLineColor( kBlue );

    histo_sumpt2->SetLineColor( kBlack );
    histo_ptbal->SetLineColor( kBlack );
    histo_ptasym->SetLineColor( kBlack );
    histo_pull_conv->SetLineColor( kBlack );

    histo_residuals_z_nconv_0->SetLineWidth( 3 );
    histo_residuals_z_nconv_1->SetLineWidth( 3 );
    histo_residuals_z_nconv_2->SetLineWidth( 3 );

    histo_gen_matched_sumpt2->SetLineWidth( 3 );
    histo_gen_matched_ptbal->SetLineWidth( 3 );
    histo_gen_matched_ptasym->SetLineWidth( 3 );
    histo_gen_matched_pull_conv->SetLineWidth( 3 );

    histo_sumpt2->SetLineWidth( 3 );
    histo_ptbal->SetLineWidth( 3 );
    histo_ptasym->SetLineWidth( 3 );
    histo_pull_conv->SetLineWidth( 3 );

    TLegend *leg = new TLegend( 0.55, 0.88, 0.98, 0.7 );
    leg->AddEntry( histo_gen_matched_sumpt2, "higgs vertex match (dz<1.cm)", "lp" );
    leg->AddEntry( histo_sumpt2, "the rest of vertices", "lp" );
    leg->SetFillColor( 0 );

    TCanvas *Ca0 = new TCanvas( "Ca0", "Canvas", 1200, 800 );
    Ca0->Divide( 2, 2 );

    Ca0->cd( 1 );
    histo_sumpt2->GetXaxis()->SetTitle( "log sumpt2" );
    histo_gen_matched_sumpt2->GetXaxis()->SetTitle( "log sumpt2" );
    histo_sumpt2->Draw();
    histo_gen_matched_sumpt2->Draw( "same" );
    leg->Draw( "same" );
    Ca0_1->SetLogy();

    Ca0->cd( 2 );
    histo_ptbal->GetXaxis()->SetTitle( "GeV" );
    histo_gen_matched_ptbal->GetXaxis()->SetTitle( "GeV" );
    histo_ptbal->DrawNormalized();
    histo_gen_matched_ptbal->DrawNormalized( "same" );
    leg->Draw( "same" );
    Ca0_2->SetLogy();

    Ca0->cd( 3 );
    histo_ptasym->GetXaxis()->SetTitle( "ptasym" );
    histo_gen_matched_ptasym->GetXaxis()->SetTitle( "ptasym" );
    histo_gen_matched_ptasym->DrawNormalized( "" );
    histo_ptasym->DrawNormalized( "same" );
    leg->Draw( "same" );

    Ca0->cd( 4 );
    histo_gen_matched_pull_conv->Draw();
    histo_pull_conv->Draw( "same" );
    leg->Draw( "same" );
    histo_pull_conv->GetXaxis()->SetTitle( "pull_conv" );
    histo_gen_matched_pull_conv->GetXaxis()->SetTitle( "pull_conv" );
    Ca0_4->SetLogy();

    Ca0->SaveAs( "mva_plots.png" );

    TCanvas *Ca20 = new TCanvas( "Ca20", "Canvas", 1200, 800 );
    Ca20->Divide( 2, 2 );

    Ca20->cd( 1 );
    histo_residuals_z_nconv_0->SetTitle( "0 conv." );
    histo_residuals_z_nconv_0->GetXaxis()->SetTitle( "0 conv. #Delta z (cm)" );
    histo_residuals_z_nconv_0->Draw();

    Ca20->cd( 2 );
    histo_residuals_z_nconv_1->SetTitle( "1 conv." );
    histo_residuals_z_nconv_1->GetXaxis()->SetTitle( "1 conv. #Delta z (cm)" );
    histo_residuals_z_nconv_1->Draw();

    Ca20->cd( 3 );
    histo_residuals_z_nconv_2->SetTitle( "2 conv." );
    histo_residuals_z_nconv_2->GetXaxis()->SetTitle( "2 conv. #Delta z (cm)" );
    histo_residuals_z_nconv_2->Draw();

    Ca20->cd( 4 );
    histo_nconv->SetTitle( "n conv." );
    histo_nconv->GetXaxis()->SetTitle( "n conv" );
    histo_nconv->Draw();

    Ca20->SetLogy();
    Ca20->SaveAs( "residuals.png" );

    TCanvas *Ca1 = new TCanvas( "Ca1", "Canvas", 1200, 800 );
    TGraphAsymmErrors *efficiency = new TGraphAsymmErrors( histo_gen_matched_vtxprobmva, histo_vtxprobmva );
    efficiency->SetTitle( "Vertex Assignment probability < 1.cm w.r.t GEN higgs vs vertex probability MVA" );
    efficiency->GetXaxis()->SetTitle( "MVA_{prob}" );
    efficiency->GetYaxis()->SetTitle( "#epsilon" );
    efficiency->SetMarkerColor( kRed );
    efficiency->SetMarkerStyle( 23 );
    efficiency->SetMarkerSize( 2 );
    efficiency->GetXaxis()->SetRangeUser( -1., 1. );
    efficiency->GetYaxis()->SetRangeUser( 0., 1. );
    TF1 *line = new TF1( "line", "[1]*x+[0]", -1, 1 );
    efficiency->Fit( "line" );
    efficiency->Draw( "AP" );
    line->Draw( "same" );
    Ca1->SaveAs( "vtxprobmva.png" );

    TCanvas *Ca2 = new TCanvas( "Ca2", "Canvas", 1200, 800 );
    TGraphAsymmErrors *efficiency0 = new TGraphAsymmErrors( histo_gen_matched_vtxeff_hpt, histo_vtxeff_hpt );
    efficiency0->SetTitle( "Vertex Assignment efficiency" );
    efficiency0->GetXaxis()->SetTitle( "higgs pt" );
    efficiency0->GetYaxis()->SetTitle( "#epsilon" );
    efficiency0->SetMarkerColor( kRed );
    efficiency0->SetMarkerStyle( 23 );
    efficiency0->SetMarkerSize( 2 );
    efficiency0->GetXaxis()->SetRangeUser( 0, 250. );
    efficiency0->GetYaxis()->SetRangeUser( 0., 1. );
    efficiency0->Draw( "AP" );
    Ca2->SaveAs( "vtxeff_higgs_pt.png" );

    TCanvas *Ca3 = new TCanvas( "Ca3", "Canvas", 1200, 800 );
    histo_ndiphotons->SetTitle( "n diphotons" );
    histo_ndiphotons->GetXaxis()->SetTitle( "n diphotons" );
    histo_ndiphotons->SetMarkerColor( kRed );
    Ca3->SaveAs( "ndiphotons.png" );

    theFileOut->Close();
    exit( 0 );

}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

