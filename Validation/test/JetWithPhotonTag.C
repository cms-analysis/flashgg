#include <map>
#include <iostream>
#include <TTree.h>
#include <TH1F.h>
#include <TFile.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TCanvas.h>
#include "../roomacro.C"
#include <TMath.h>

void JetWithPhotonTag()
{
    // get the trees
    TString cut = "" ;
    std::pair<TString, TString> label = std::make_pair( TString( "w/o photon removal" ), TString( "w photon removal" ) );

    TString photon_tag_veto = "!(photonMatch==1 && GenPhotonPt/bestPt > 0.5 && photondRmin < 0.2)";

    std::map<TString, TCanvas *> Canv;

    std::map<TString, TTree *>     JetTree;

    std::map<TString, TH1F *>      h_ptOffset_tag;
    std::map<TString, TH1F *>      h_ptOffset_notag;

    std::map<TString, TH1F *>      h_DR;
    std::map<TString, TH1F *>      h_ratioPt;

    std::map<TString, TH1F *>      h_eta_tag;
    std::map<TString, TH1F *>      h_eta_notag;

    std::map<TString, TProfile *>  p_ptOffset_eta_tag;
    std::map<TString, TProfile *>  p_ptOffset_eta_notag;

    TFile *file = TFile::Open( "/afs/cern.ch/work/y/yhaddad/jetValidationCollection_PU40bx50_ptag.root" );
    JetTree["PF"]      = ( TTree * )file->Get( "flashggJetValidationTreeMaker/jetTree_PF" );
    JetTree["PFCHS0"]  = ( TTree * )file->Get( "flashggJetValidationTreeMakerPFCHS0/jetTree_PFCHS0" );
    JetTree["PFCHSLeg"] = ( TTree * )file->Get( "flashggJetValidationTreeMakerPFCHSLeg/jetTree_PFCHSLeg" );
    JetTree["PUPPI0"]  = ( TTree * )file->Get( "flashggJetValidationTreeMakerPUPPI0/jetTree_PUPPI0" );
    JetTree["PUPPILeg"] = ( TTree * )file->Get( "flashggJetValidationTreeMakerPUPPILeg/jetTree_PUPPILeg" );

    // ===> Only for def
    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        h_ptOffset_tag  [it->first] = new TH1F( Form( "h_ptOffset_tag_%s"  , it->first.Data() ), Form( "%s;p^{reco}_{t}/p^{gen}_{t}", it->first.Data() ), 200, 0, 10 );
        h_ptOffset_notag[it->first] = new TH1F( Form( "h_ptOffset_notag_%s", it->first.Data() ), Form( "%s;p^{reco}_{t}/p^{gen}_{t}", it->first.Data() ) , 200, 0, 10 );


        h_eta_tag  [it->first] = new TH1F( Form( "h_eta_tag_%s"  , it->first.Data() ), Form( "%s;#eta", it->first.Data() ), 200, -6, 6 );
        h_eta_notag[it->first] = new TH1F( Form( "h_eta_notag_%s", it->first.Data() ), Form( "%s;#eta", it->first.Data() ) , 200, -6, 6 );


        h_ratioPt  [it->first] = new TH1F( Form( "h_ratioPt_%s"  , it->first.Data() ), Form( "%s;p^{Jet}_{t}/p^{#gamma}_{t}", it->first.Data() ), 200, 0, 5 );
        h_DR       [it->first] = new TH1F( Form( "h_DR_%s"       , it->first.Data() ), Form( "%s;min{#Delta R (#gamma^{gen}, Jet)}", it->first.Data() ), 200, 0, 1 );

        JetTree[it->first]->Draw( Form( "(bestPt/genJetPt)>>h_ptOffset_tag_%s"  , it->first.Data() ), photon_tag_veto + "&& bestPt > 20", "" );
        JetTree[it->first]->Draw( Form( "(bestPt/genJetPt)>>h_ptOffset_notag_%s", it->first.Data() ), "bestPt>20", "" );

        JetTree[it->first]->Draw( Form( "eta>>h_eta_tag_%s"  , it->first.Data() ), photon_tag_veto + "&& bestPt>20 ", "" );
        JetTree[it->first]->Draw( Form( "eta>>h_eta_notag_%s", it->first.Data() ), "bestPt>20", "" );

        JetTree[it->first]->Draw( Form( "(bestPt/GenPhotonPt)>>h_ratioPt_%s", it->first.Data() ), photon_tag_veto + "&& bestPt>20 ", "" );
        JetTree[it->first]->Draw( Form( " photondRmin >>h_DR_%s", it->first.Data() ), photon_tag_veto + "&& bestPt>20 ", "" );
    }

    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        Canv["ptOffset_" + it->first] = new TCanvas( Form( "c_ptOffset_%s", it->first.Data() ), "", 500, 500 );
        Canv["ptOffset_" + it->first]->cd();
        gPad->SetLogy();

        h_ptOffset_tag  [it->first]->SetLineColor( 1 );
        h_ptOffset_notag[it->first]->SetLineColor( 2 );
        h_ptOffset_tag  [it->first]->SetMarkerColor( 1 );
        h_ptOffset_notag[it->first]->SetMarkerColor( 2 );

        h_ptOffset_notag[it->first]->Draw();
        h_ptOffset_tag  [it->first]->Draw( "same" );

        Draw2Legend( h_ptOffset_tag  [it->first],
                     h_ptOffset_notag[it->first],
                     "w/o  photons",
                     "with photons" );

        Canv["ptOffset_" + it->first]->SaveAs( Form( "plots/ptOffset_%s.C"  , it->first.Data() ) );
        Canv["ptOffset_" + it->first]->SaveAs( Form( "plots/ptOffset_%s.pdf", it->first.Data() ) );

        Canv["eta_" + it->first] = new TCanvas( Form( "c_eta_%s", it->first.Data() ), "", 500, 500 );
        Canv["eta_" + it->first]->cd();
        gPad->SetLogy();

        h_eta_tag  [it->first]->SetLineColor( 1 );
        h_eta_notag[it->first]->SetLineColor( 2 );
        h_eta_tag  [it->first]->SetMarkerColor( 1 );
        h_eta_notag[it->first]->SetMarkerColor( 2 );

        h_eta_notag[it->first]->Draw();
        h_eta_tag  [it->first]->Draw( "same" );

        Draw2Legend( h_eta_tag  [it->first],
                     h_eta_notag[it->first],
                     "w/o  photons",
                     "with photons" );

        Canv["eta_" + it->first]->SaveAs( Form( "plots/eta_%s.C"  , it->first.Data() ) );
        Canv["eta_" + it->first]->SaveAs( Form( "plots/eta_%s.pdf", it->first.Data() ) );
    }

    TCanvas *c_dr  = new TCanvas( "c_dr", "", 500, 500 );
    c_dr->cd();
    int count = 0;
    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        h_DR[it->first]->SetLineColor( int( 50 + count * 10 ) );
        if( count == 0 ) {
            h_DR[it->first]->Draw();
        } else {
            h_DR[it->first]->Draw( "same" );
        }
        count++;
    }

    TCanvas *c_ptratio  = new TCanvas( "c_ptratio", "", 500, 500 );
    c_ptratio->cd();
    count = 0;
    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        h_ratioPt[it->first]->SetLineColor( int( 55 + count * 10 ) );
        if( count == 0 ) {
            h_ratioPt[it->first]->Draw();
        } else {
            h_ratioPt[it->first]->Draw( "same" );
        }
        count++;
    }

    //
    //
    //TH1F* h_rawPt_PF      = new TH1F("h_rawPt_PF"      ,"p^{raw}_{t}[GeV]",500,0,500);
    //TH1F* h_rawPt_PFCHS0  = new TH1F("h_rawPt_PFCHS0"  ,"p^{raw}_{t}[GeV]",500,0,500);
    //TH1F* h_rawPt_PFCHSLeg= new TH1F("h_rawPt_PFCHSLeg","p^{raw}_{t}[GeV]",500,0,500);
    //TH1F* h_rawPt_PUPPI0  = new TH1F("h_rawPt_PUPPI0"  ,"p^{raw}_{t}[GeV]",500,0,500);
    //TH1F* h_rawPt_PUPPILeg= new TH1F("h_rawPt_PUPPILeg","p^{raw}_{t}[GeV]",500,0,500);
    //
    //h_pt_PF      ->SetLineColor(1) ;
    //h_pt_PFCHS0  ->SetLineColor(2) ;
    //h_pt_PFCHSLeg->SetLineColor(3) ;
    //h_pt_PUPPI0  ->SetLineColor(4) ;
    //h_pt_PUPPILeg->SetLineColor(6) ;
    //
    //
    //
    //
    //JetTree_PF      ->Draw("eta>>h_eta_PF"      ,pt_cut.Data(),"");
    //JetTree_PFCHS0  ->Draw("eta>>h_eta_PFCHS0"  ,pt_cut.Data(),"");
    //JetTree_PFCHSLeg->Draw("eta>>h_eta_PFCHSLeg",pt_cut.Data(),"");
    //JetTree_PUPPI0  ->Draw("eta>>h_eta_PUPPI0"  ,pt_cut.Data(),"");
    //Jettree_PUPPILeg->Draw("eta>>h_eta_PUPPILeg",pt_cut.Data(),"");
    //
    //JetTree_PF      ->Draw("rawPt-pt>>h_pt_PF"      ,"pt>20 && abs(eta)>3 ","");
    //JetTree_PFCHS0  ->Draw("rawPt-pt>>h_pt_PFCHS0"  ,"pt>20 && abs(eta)>3 ","");
    //JetTree_PFCHSLeg->Draw("rawPt-pt>>h_pt_PFCHSLeg","pt>20 && abs(eta)>3 ","");
    //JetTree_PUPPI0  ->Draw("rawPt-pt>>h_pt_PUPPI0"  ,"pt>20 && abs(eta)>3 ","");
    //Jettree_PUPPILeg->Draw("rawPt-pt>>h_pt_PUPPILeg","pt>20 && abs(eta)>3 ","");
    //
    //
    //TH2F* h2_eta_corr_PF      = new TH2F("h2_eta_corr_PF"      ,";#eta;(p_{t}-p_{t}^{raw})",120,-6,6,200,-40,40);
    //JetTree_PF      ->Draw("(rawPt-pt):eta>>h2_eta_corr_PF"    ,"pt>20","colz");
    //
    //
    //TCanvas *c = new TCanvas("c","eta",500,500);
    //c->cd();
    //h_eta_PF      ->Draw();
    //h_eta_PFCHS0  ->Draw("same");
    //h_eta_PFCHSLeg->Draw("same");
    //h_eta_PUPPI0  ->Draw("same");
    //h_eta_PUPPILeg->Draw("same");
    //
    //c->BuildLegend();
    //
    //TCanvas *c_corr = new TCanvas("c_corr","eta",500,500);
    //c_corr->cd();
    //h_pt_PF      ->Draw();
    //h_pt_PFCHS0  ->Draw("same");
    //h_pt_PFCHSLeg->Draw("same");
    //h_pt_PUPPI0  ->Draw("same");
    //h_pt_PUPPILeg->Draw("same");
    //
    //c_corr->BuildLegend();
    //
    //TCanvas *c_corr2 = new TCanvas("c_corr2","corrections",500,500);
    //h2_eta_corr_PF      ->Draw("colz");
    //TProfile *pfx_eta_corr_PF = h2_eta_corr_PF->ProfileX("pfx_eta_corr_PF");
    //pfx_eta_corr_PF->SetMarkerColor(kRed);
    //pfx_eta_corr_PF->SetLineColor(kRed);
    //pfx_eta_corr_PF     ->Draw("same");
    //

}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

