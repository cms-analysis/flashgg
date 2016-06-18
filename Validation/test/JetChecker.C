#include <map>
#include <iostream>
#include <TTree.h>
#include <TH1F.h>
#include <TFile.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TCanvas.h>

void JetChecker()
{
    // get the trees
    TString pt_cut = "2*(pt>20)";

    std::map<TString, TCanvas *> Canv;

    std::map<TString, TTree *> JetTree_PU20bx25;
    std::map<TString, TTree *> JetTree_PU40bx50;

    std::map<TString, TH1F *>  h_PU20bx25_eta;
    std::map<TString, TH1F *>  h_PU20bx25_pt;
    std::map<TString, TH1F *>  h_PU20bx25_corr;
    std::map<TString, TH1F *>  h_PU20bx25_energy;
    std::map<TString, TH1F *>  h_PU20bx25_mass;

    std::map<TString, TH1F *>  h_PU40bx50_eta;
    std::map<TString, TH1F *>  h_PU40bx50_pt;
    std::map<TString, TH1F *>  h_PU40bx50_corr;
    std::map<TString, TH1F *>  h_PU40bx50_energy;
    std::map<TString, TH1F *>  h_PU40bx50_mass;

    std::map<TString, TH2F *>  h_PU20bx25_eta_corr;
    std::map<TString, TH2F *>  h_PU40bx50_eta_corr;

    std::map<TString, TProfile *>  p_PU20bx25_eta_corr;
    std::map<TString, TProfile *>  p_PU40bx50_eta_corr;

    //TFile *f_PU20bx25           = TFile::Open("jetValidationCollection_PU20bx25.root");
    //TFile *f_PU20bx25           = TFile::Open("/afs/cern.ch/work/s/sethzenz/public/72x_jetValidationCollection_PU20bx25.root");
    //TFile *f_PU20bx25           = TFile::Open("jetValidationCollection_PU20bx25_GluGluToHToGG_70X.root");
    TFile *f_PU20bx25           = TFile::Open( "/afs/cern.ch/user/l/lcorpe/public/Yacine/jetValidationCollection_ggH.root" );
    JetTree_PU20bx25["PF"]      = ( TTree * )f_PU20bx25->Get( "flashggJetValidationTreeMaker/jetTree_PF" );
    JetTree_PU20bx25["PFCHS0"]  = ( TTree * )f_PU20bx25->Get( "flashggJetValidationTreeMakerPFCHS0/jetTree_PFCHS0" );
    JetTree_PU20bx25["PFCHSLeg"] = ( TTree * )f_PU20bx25->Get( "flashggJetValidationTreeMakerPFCHSLeg/jetTree_PFCHSLeg" );
    JetTree_PU20bx25["PUPPI0"]  = ( TTree * )f_PU20bx25->Get( "flashggJetValidationTreeMakerPUPPI0/jetTree_PUPPI0" );
    JetTree_PU20bx25["PUPPILeg"] = ( TTree * )f_PU20bx25->Get( "flashggJetValidationTreeMakerPUPPILeg/jetTree_PUPPILeg" );

    //TFile *f_PU40bx50           = TFile::Open("jetValidationCollection_PU40bx50.root");
    //TFile *f_PU40bx50           = TFile::Open("jetValidationCollection_PU20bx25_GluGluToHToGG_72X.root");
    TFile *f_PU40bx50           = TFile::Open( "/afs/cern.ch/user/l/lcorpe/public/Yacine/jetValidationCollection_ggH_JEC.root" );
    JetTree_PU40bx50["PF"]      = ( TTree * )f_PU40bx50->Get( "flashggJetValidationTreeMaker/jetTree_PF" );
    JetTree_PU40bx50["PFCHS0"]  = ( TTree * )f_PU40bx50->Get( "flashggJetValidationTreeMakerPFCHS0/jetTree_PFCHS0" );
    JetTree_PU40bx50["PFCHSLeg"] = ( TTree * )f_PU40bx50->Get( "flashggJetValidationTreeMakerPFCHSLeg/jetTree_PFCHSLeg" );
    JetTree_PU40bx50["PUPPI0"]  = ( TTree * )f_PU40bx50->Get( "flashggJetValidationTreeMakerPUPPI0/jetTree_PUPPI0" );
    JetTree_PU40bx50["PUPPILeg"] = ( TTree * )f_PU40bx50->Get( "flashggJetValidationTreeMakerPUPPILeg/jetTree_PUPPILeg" );

    // ===> Only for def
    for( std::map<TString, TTree *>::iterator it = JetTree_PU20bx25.begin(); it != JetTree_PU20bx25.end(); ++it ) {
        h_PU20bx25_eta   [it->first] = new TH1F( Form( "h_PU20bx25_eta_%s" , it->first.Data() ), Form( "%s;eta", it->first.Data() ), 300, -6, 6 );
        h_PU20bx25_pt    [it->first] = new TH1F( Form( "h_PU20bx25_pt_%s"  , it->first.Data() ), Form( "%s;pt_{t}", it->first.Data() ), 500, 0, 500 );
        h_PU20bx25_mass  [it->first] = new TH1F( Form( "h_PU20bx25_mass_%s"  , it->first.Data() ), Form( "%s;m_{jet}", it->first.Data() ), 200, 0, 100 );
        h_PU20bx25_energy[it->first] = new TH1F( Form( "h_PU20bx25_energy_%s"  , it->first.Data() ), Form( "%s;E_{jet}", it->first.Data() ), 1000, 0, 1000 );
        h_PU20bx25_corr  [it->first] = new TH1F( Form( "h_PU20bx25_corr_%s", it->first.Data() ), Form( "%s;(p_{t}-p_{t}^{raw})", it->first.Data() ), 200, -40, 40 );

        h_PU40bx50_eta   [it->first] = new TH1F( Form( "h_PU40bx50_eta_%s" , it->first.Data() ), Form( "%s;eta", it->first.Data() ), 300, -6, 6 );
        h_PU40bx50_pt    [it->first] = new TH1F( Form( "h_PU40bx50_pt_%s"  , it->first.Data() ), Form( "%s;pt_{t}", it->first.Data() ), 500, 0, 500 );
        h_PU40bx50_mass  [it->first] = new TH1F( Form( "h_PU40bx50_mass_%s"  , it->first.Data() ), Form( "%s;m_{jet}", it->first.Data() ), 200, 0, 100 );
        h_PU40bx50_energy[it->first] = new TH1F( Form( "h_PU40bx50_energy_%s"  , it->first.Data() ), Form( "%s;E_{jet}", it->first.Data() ), 1000, 0, 1000 );
        h_PU40bx50_corr  [it->first] = new TH1F( Form( "h_PU40bx50_corr_%s", it->first.Data() ), Form( "%s;(pt_{t}-p_{t}^{raw})", it->first.Data() ), 200, -40, 40 );

        h_PU40bx50_eta_corr[it->first] = new TH2F( Form( "h_PU40bx50_eta_corr_%s", it->first.Data() ), Form( "%s;#eta;#Delta p_{t}", it->first.Data() ), 300, -6, 6,
                200, -40, 40 );
        h_PU20bx25_eta_corr[it->first] = new TH2F( Form( "h_PU20bx25_eta_corr_%s", it->first.Data() ), Form( "%s;#eta;#Delta p_{t}", it->first.Data() ), 300, -6, 6,
                200, -40, 40 );

        JetTree_PU20bx25[it->first]->Draw( Form( "eta>>h_PU20bx25_eta_%s", it->first.Data() ), pt_cut.Data(), "" );
        JetTree_PU40bx50[it->first]->Draw( Form( "eta>>h_PU40bx50_eta_%s", it->first.Data() ), pt_cut.Data(), "" );

        JetTree_PU20bx25[it->first]->Draw( Form( "pt>>h_PU20bx25_pt_%s", it->first.Data() ), "", "" );
        JetTree_PU40bx50[it->first]->Draw( Form( "pt>>h_PU40bx50_pt_%s", it->first.Data() ), "", "" );

        JetTree_PU20bx25[it->first]->Draw( Form( "(pt-rawPt)>>h_PU20bx25_corr_%s", it->first.Data() ), pt_cut.Data(), "" );
        JetTree_PU40bx50[it->first]->Draw( Form( "(pt-rawPt)>>h_PU40bx50_corr_%s", it->first.Data() ), pt_cut.Data(), "" );

        JetTree_PU20bx25[it->first]->Draw( Form( "energy>>h_PU20bx25_energy_%s", it->first.Data() ), pt_cut.Data(), "" );
        JetTree_PU40bx50[it->first]->Draw( Form( "energy>>h_PU40bx50_energy_%s", it->first.Data() ), pt_cut.Data(), "" );

        JetTree_PU20bx25[it->first]->Draw( Form( "mass>>h_PU20bx25_mass_%s", it->first.Data() ), pt_cut.Data(), "" );
        JetTree_PU40bx50[it->first]->Draw( Form( "mass>>h_PU40bx50_mass_%s", it->first.Data() ), pt_cut.Data(), "" );

        JetTree_PU20bx25[it->first]->Draw( Form( "(rawPt-pt):eta>>h_PU20bx25_eta_corr_%s", it->first.Data() ), pt_cut.Data(), "" );
        JetTree_PU40bx50[it->first]->Draw( Form( "(rawPt-pt):eta>>h_PU40bx50_eta_corr_%s", it->first.Data() ), pt_cut.Data(), "" );

        p_PU40bx50_eta_corr[it->first] = h_PU40bx50_eta_corr[it->first]->ProfileX();
        p_PU20bx25_eta_corr[it->first] = h_PU20bx25_eta_corr[it->first]->ProfileX();

        p_PU40bx50_eta_corr[it->first]->SetTitle( h_PU40bx50_eta_corr[it->first]->GetTitle() ) ;
        p_PU20bx25_eta_corr[it->first]->SetTitle( h_PU20bx25_eta_corr[it->first]->GetTitle() ) ;
    }

    for( std::map<TString, TTree *>::iterator it = JetTree_PU20bx25.begin(); it != JetTree_PU20bx25.end(); ++it ) {
        Canv["eta_" + it->first] = new TCanvas( Form( "c_eta_%s", it->first.Data() ), "", 700, 500 );
        Canv["eta_" + it->first]->cd();

        h_PU20bx25_eta[it->first]->SetLineColor( 1 );
        h_PU40bx50_eta[it->first]->SetLineColor( 2 );
        h_PU20bx25_eta[it->first]->SetMarkerColor( 1 );
        h_PU40bx50_eta[it->first]->SetMarkerColor( 2 );
        h_PU20bx25_eta[it->first]->GetYaxis()->SetRangeUser( 0, 750 );
        h_PU20bx25_eta[it->first]->Draw();
        h_PU40bx50_eta[it->first]->Draw( "same" );

        Canv["eta_" + it->first]->SaveAs( Form( "plots/eta_%s.C", it->first.Data() ) );
        Canv["eta_" + it->first]->SaveAs( Form( "plots/eta_%s.pdf", it->first.Data() ) );

        Canv["eta_corr_" + it->first] = new TCanvas( Form( "c_eta_corr_%s", it->first.Data() ), "", 700, 500 );
        Canv["eta_corr_" + it->first]->cd();

        p_PU20bx25_eta_corr[it->first]->SetLineColor( 1 );
        p_PU40bx50_eta_corr[it->first]->SetLineColor( 2 );
        p_PU20bx25_eta_corr[it->first]->SetMarkerColor( 1 );
        p_PU40bx50_eta_corr[it->first]->SetMarkerColor( 2 );
        p_PU20bx25_eta_corr[it->first]->GetYaxis()->SetRangeUser( -30, 30 );
        p_PU40bx50_eta_corr[it->first]->GetYaxis()->SetRangeUser( -30, 30 );
        p_PU20bx25_eta_corr[it->first]->GetYaxis()->SetTitle( "#Delta p_{t}" );
        p_PU40bx50_eta_corr[it->first]->GetYaxis()->SetTitle( "#Delta p_{t}" );
        p_PU20bx25_eta_corr[it->first]->Draw();
        p_PU40bx50_eta_corr[it->first]->Draw( "same" );

        Canv["eta_corr_" + it->first]->SaveAs( Form( "plots/profile_eta_corr_%s.C", it->first.Data() ) );
        Canv["eta_corr_" + it->first]->SaveAs( Form( "plots/profile_eta_corr_%s.pdf", it->first.Data() ) );


        Canv["corr_" + it->first] = new TCanvas( Form( "c_corr_%s", it->first.Data() ), "", 700, 500 );
        Canv["corr_" + it->first]->cd();

        h_PU20bx25_corr[it->first]->SetLineColor( 1 );
        h_PU40bx50_corr[it->first]->SetLineColor( 2 );
        h_PU20bx25_corr[it->first]->SetMarkerColor( 1 );
        h_PU40bx50_corr[it->first]->SetMarkerColor( 2 );
        //h_PU20bx25_corr[it->first]->GetYaxis()->SetRangeUser(0,0.08);
        //h_PU40bx50_corr[it->first]->GetYaxis()->SetRangeUser(0,0.08);
        h_PU20bx25_corr[it->first]->DrawNormalized();
        h_PU40bx50_corr[it->first]->DrawNormalized( "same" );

        Canv["corr_" + it->first]->SaveAs( Form( "plots/corr_%s.C", it->first.Data() ) );
        Canv["corr_" + it->first]->SaveAs( Form( "plots/corr_%s.pdf", it->first.Data() ) );

        Canv["pt_" + it->first] = new TCanvas( Form( "c_pt_%s", it->first.Data() ), "", 700, 500 );
        Canv["pt_" + it->first]->cd();
        gPad->SetLogy();
        h_PU20bx25_pt[it->first]->SetLineColor( 1 );
        h_PU40bx50_pt[it->first]->SetLineColor( 2 );
        h_PU20bx25_pt[it->first]->SetMarkerColor( 1 );
        h_PU40bx50_pt[it->first]->SetMarkerColor( 2 );
        h_PU20bx25_pt[it->first]->Draw();
        h_PU40bx50_pt[it->first]->Draw( "same" );

        Canv["pt_" + it->first]->SaveAs( Form( "plots/pt_%s.pdf", it->first.Data() ) );
        Canv["pt_" + it->first]->SaveAs( Form( "plots/pt_%s.C", it->first.Data() ) );


        Canv["mass_" + it->first] = new TCanvas( Form( "c_mass_%s", it->first.Data() ), "", 700, 500 );
        Canv["mass_" + it->first]->cd();
        gPad->SetLogy();
        h_PU20bx25_mass[it->first]->SetLineColor( 1 );
        h_PU40bx50_mass[it->first]->SetLineColor( 2 );
        h_PU20bx25_mass[it->first]->SetMarkerColor( 1 );
        h_PU40bx50_mass[it->first]->SetMarkerColor( 2 );
        h_PU20bx25_mass[it->first]->Draw();
        h_PU40bx50_mass[it->first]->Draw( "same" );

        Canv["mass_" + it->first]->SaveAs( Form( "plots/mass_%s.pdf", it->first.Data() ) );
        Canv["mass_" + it->first]->SaveAs( Form( "plots/mass_%s.C", it->first.Data() ) );

        Canv["energy_" + it->first] = new TCanvas( Form( "c_energy_%s", it->first.Data() ), "", 700, 500 );
        Canv["energy_" + it->first]->cd();
        gPad->SetLogy();
        h_PU20bx25_energy[it->first]->SetLineColor( 1 );
        h_PU40bx50_energy[it->first]->SetLineColor( 2 );
        h_PU20bx25_energy[it->first]->SetMarkerColor( 1 );
        h_PU40bx50_energy[it->first]->SetMarkerColor( 2 );
        h_PU20bx25_energy[it->first]->Draw();
        h_PU40bx50_energy[it->first]->Draw( "same" );

        Canv["energy_" + it->first]->SaveAs( Form( "plots/energy_%s.pdf", it->first.Data() ) );
        Canv["energy_" + it->first]->SaveAs( Form( "plots/energy_%s.C", it->first.Data() ) );

    }


    //TH1F* h_eta_PF      = new TH1F("h_eta_PF"      ,";eta",300,-5,5);
    //TH1F* h_eta_PFCHS0  = new TH1F("h_eta_PFCHS0"  ,";eta",300,-5,5);
    //TH1F* h_eta_PFCHSLeg= new TH1F("h_eta_PFCHSLeg",";eta",300,-5,5);
    //TH1F* h_eta_PUPPI0  = new TH1F("h_eta_PUPPI0"  ,";eta",300,-5,5);
    //TH1F* h_eta_PUPPILeg= new TH1F("h_eta_PUPPILeg",";eta",300,-5,5);
    //
    //TH1F* h_corr_PF      = new TH1F("h_corr_PF"      ,";#eta",300,-5,5);
    //TH1F* h_corr_PFCHS0  = new TH1F("h_corr_PFCHS0"  ,";#eta",300,-5,5);
    //TH1F* h_corr_PFCHSLeg= new TH1F("h_corr_PFCHSLeg",";#eta",300,-5,5);
    //TH1F* h_corr_PUPPI0  = new TH1F("h_corr_PUPPI0"  ,";#eta",300,-5,5);
    //TH1F* h_corr_PUPPILeg= new TH1F("h_corr_PUPPILeg",";#eta",300,-5,5);
    //
    //h_corr_PF      ->SetLineColor(1) ;
    //h_corr_PFCHS0  ->SetLineColor(2) ;
    //h_corr_PFCHSLeg->SetLineColor(3) ;
    //h_corr_PUPPI0  ->SetLineColor(4) ;
    //h_corr_PUPPILeg->SetLineColor(6) ;
    //
    //
    //
    //TH1F* h_pt_PF      = new TH1F("h_pt_PF"      ,";p_{t}[GeV]",500,-50,50);
    //TH1F* h_pt_PFCHS0  = new TH1F("h_pt_PFCHS0"  ,";p_{t}[GeV]",500,-50,50);
    //TH1F* h_pt_PFCHSLeg= new TH1F("h_pt_PFCHSLeg",";p_{t}[GeV]",500,-50,50);
    //TH1F* h_pt_PUPPI0  = new TH1F("h_pt_PUPPI0"  ,";p_{t}[GeV]",500,-50,50);
    //TH1F* h_pt_PUPPILeg= new TH1F("h_pt_PUPPILeg",";p_{t}[GeV]",500,-50,50);
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

