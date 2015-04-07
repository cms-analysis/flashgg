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

// [0]*(1+TMath::Sign(1.0,[2])*exp([1]*x))
Double_t JetResponseFun( Double_t *x, Double_t *par )
{
    Double_t _x    = x[0];
    Double_t R_0   = par[0];
    Double_t tau   = par[1];
    Double_t s     = par[2];
    Double_t x_0     = par[3];

    return R_0 * ( 1 - TMath::Sign( 1.0, s ) * TMath::Exp( -( _x - x_0 ) * tau ) );
}


void GenJetvsTrueJet( int cat = 1 )
{
    // get the trees
    TString pt_cut;
    TString pt_cut_all;
    TString com_cut = "&& (bestPt/genJetPt) < 2.0" ;
    std::pair<TString, TString> label ;

    pt_cut_all = "bestPt > 20" + com_cut;

    if( cat == 1 ) {
        label  = std::make_pair( TString( "category_1" ), TString( " 20 < p_{t} < 30 GeV | Res_{jet} < 2.0" ) );
        pt_cut = "bestPt>20 && pt<30 && genJetMatch==1" + com_cut;
    } else if( cat == 2 ) {
        pt_cut = "bestPt>30 && pt<50 && genJetMatch==1" + com_cut;
        label = std::make_pair( TString( "category_2" ), TString( "30 < p_{t} < 50 GeV | Res_{jet} < 2.0" ) );
    } else if( cat == 3 ) {
        pt_cut = "bestPt>50 && pt<80 && genJetMatch==1" + com_cut;
        label = std::make_pair( TString( "category_3" ), TString( "50 < p_{t} < 80 GeV | Res_{jet} < 2.0" ) );
    } else if( cat == 4 ) {
        pt_cut = "bestPt>80 && genJetMatch==1" + com_cut;;
        label = std::make_pair( TString( "category_4" ), TString( "p_{t}> 80 GeV" ) );
    }

    std::map<TString, TCanvas *> Canv;

    std::map<TString, TTree *>     JetTree_PU20bx25;
    std::map<TString, TTree *>     JetTree_PU40bx50;

    std::map<TString, TH1F *>      h_PU20bx25_ptOffset;
    std::map<TString, TH1F *>      h_PU40bx50_ptOffset;



    std::map<TString, TH1F *>      h_PU20bx25_ptOffset_etacut;
    std::map<TString, TH1F *>      h_PU40bx50_ptOffset_etacut;

    std::map<TString, TH2F *>      h_PU20bx25_ptOffset_eta;
    std::map<TString, TH2F *>      h_PU40bx50_ptOffset_eta;

    std::map<TString, TProfile *>  p_PU20bx25_ptOffset_eta;
    std::map<TString, TProfile *>  p_PU40bx50_ptOffset_eta;

    std::map<TString, TH2F *>      h_PU20bx25_res_pt_cat1;
    std::map<TString, TH2F *>      h_PU40bx50_res_pt_cat1;
    std::map<TString, TH2F *>      h_PU20bx25_res_pt_cat2;
    std::map<TString, TH2F *>      h_PU40bx50_res_pt_cat2;
    std::map<TString, TH2F *>      h_PU20bx25_res_pt_cat3;
    std::map<TString, TH2F *>      h_PU40bx50_res_pt_cat3;

    std::map<TString, TProfile *>      p_PU20bx25_res_pt_cat1;
    std::map<TString, TProfile *>      p_PU40bx50_res_pt_cat1;
    std::map<TString, TProfile *>      p_PU20bx25_res_pt_cat2;
    std::map<TString, TProfile *>      p_PU40bx50_res_pt_cat2;
    std::map<TString, TProfile *>      p_PU20bx25_res_pt_cat3;
    std::map<TString, TProfile *>      p_PU40bx50_res_pt_cat3;

    std::map<TString, TF1 *>      f_PU20bx25_res_pt_cat1;
    std::map<TString, TF1 *>      f_PU40bx50_res_pt_cat1;
    std::map<TString, TF1 *>      f_PU20bx25_res_pt_cat2;
    std::map<TString, TF1 *>      f_PU40bx50_res_pt_cat2;
    std::map<TString, TF1 *>      f_PU20bx25_res_pt_cat3;
    std::map<TString, TF1 *>      f_PU40bx50_res_pt_cat3;


    std::map<TString, TH2F *>      h_PU20bx25_area_eta;
    std::map<TString, TH2F *>      h_PU40bx50_area_eta;

    std::map<TString, TProfile *>      p_PU20bx25_area_eta;
    std::map<TString, TProfile *>      p_PU40bx50_area_eta;

    //TFile *f_PU20bx25           = TFile::Open("data/jetValidationCollection_PU20bx25_new.root");
    //TFile *f_PU20bx25           = TFile::Open("/afs/cern.ch/work/s/sethzenz/public/72x_jetValidationCollection_PU20bx25.root");
    TFile *f_PU20bx25           = TFile::Open( "/afs/cern.ch/user/l/lcorpe/public/Yacine/jetValidationCollection_ggH.root" );
    JetTree_PU20bx25["PF"]      = ( TTree * )f_PU20bx25->Get( "flashggJetValidationTreeMaker/jetTree_PF" );
    JetTree_PU20bx25["PFCHS0"]  = ( TTree * )f_PU20bx25->Get( "flashggJetValidationTreeMakerPFCHS0/jetTree_PFCHS0" );
    JetTree_PU20bx25["PFCHSLeg"] = ( TTree * )f_PU20bx25->Get( "flashggJetValidationTreeMakerPFCHSLeg/jetTree_PFCHSLeg" );
    JetTree_PU20bx25["PUPPI0"]  = ( TTree * )f_PU20bx25->Get( "flashggJetValidationTreeMakerPUPPI0/jetTree_PUPPI0" );
    JetTree_PU20bx25["PUPPILeg"] = ( TTree * )f_PU20bx25->Get( "flashggJetValidationTreeMakerPUPPILeg/jetTree_PUPPILeg" );
    //  TFile *f_PU40bx50           = TFile::Open("data/jetValidationCollection_PU40bx50_new.root");


    TFile *f_PU40bx50           = TFile::Open( "/afs/cern.ch/user/l/lcorpe/public/Yacine/jetValidationCollection_ggH_JEC.root" );
    //TFile *f_PU40bx50           = TFile::Open("/afs/cern.ch/work/s/sethzenz/public/72x_jetValidationCollection_PU40bx50.root");
    //TFile *f_PU40bx50           = TFile::Open("data/jetValidationCollection_PU40bx50_new.root");
    JetTree_PU40bx50["PF"]      = ( TTree * )f_PU40bx50->Get( "flashggJetValidationTreeMaker/jetTree_PF" );
    JetTree_PU40bx50["PFCHS0"]  = ( TTree * )f_PU40bx50->Get( "flashggJetValidationTreeMakerPFCHS0/jetTree_PFCHS0" );
    JetTree_PU40bx50["PFCHSLeg"] = ( TTree * )f_PU40bx50->Get( "flashggJetValidationTreeMakerPFCHSLeg/jetTree_PFCHSLeg" );
    JetTree_PU40bx50["PUPPI0"]  = ( TTree * )f_PU40bx50->Get( "flashggJetValidationTreeMakerPUPPI0/jetTree_PUPPI0" );
    JetTree_PU40bx50["PUPPILeg"] = ( TTree * )f_PU40bx50->Get( "flashggJetValidationTreeMakerPUPPILeg/jetTree_PUPPILeg" );

    // ===> Only for def
    for( std::map<TString, TTree *>::iterator it = JetTree_PU20bx25.begin(); it != JetTree_PU20bx25.end(); ++it ) {
        h_PU20bx25_ptOffset[it->first] = new TH1F( Form( "h_PU20bx25_ptOffset_%s", it->first.Data() ), Form( "%s  %s;p^{reco}_{t}/p^{gen}_{t}", it->first.Data(),
                label.second.Data() ), 200, 0, 10 );
        h_PU40bx50_ptOffset[it->first] = new TH1F( Form( "h_PU40bx50_ptOffset_%s", it->first.Data() ), Form( "%s  %s;p^{reco}_{t}/p^{gen}_{t}", it->first.Data(),
                label.second.Data() ), 200, 0, 10 );

        h_PU20bx25_ptOffset_etacut[it->first] = new TH1F( Form( "h_PU20bx25_ptOffset_etacut_%s", it->first.Data() ), Form( "%s  %s;p^{reco}_{t}/p^{gen}_{t}",
                it->first.Data(), label.second.Data() ), 200, 0, 10 );
        h_PU40bx50_ptOffset_etacut[it->first] = new TH1F( Form( "h_PU40bx50_ptOffset_etacut_%s", it->first.Data() ), Form( "%s  %s;p^{reco}_{t}/p^{gen}_{t}",
                it->first.Data(), label.second.Data() ), 200, 0, 10 );

        h_PU40bx50_ptOffset_eta[it->first] = new TH2F( Form( "h_PU40bx50_ptOffset_eta_%s", it->first.Data() ), Form( "%s  %s;#eta;p^{reco}_{t}/p^{gen}_{t}",
                it->first.Data(), label.second.Data() ), 150, -6, 6, 200, 0, 10 );
        h_PU20bx25_ptOffset_eta[it->first] = new TH2F( Form( "h_PU20bx25_ptOffset_eta_%s", it->first.Data() ), Form( "%s  %s;#eta;p^{reco}_{t}/p^{gen}_{t}",
                it->first.Data(), label.second.Data() ), 150, -6, 6, 200, 0, 10 );

        h_PU40bx50_area_eta[it->first] = new TH2F( Form( "h_PU40bx50_area_eta_%s", it->first.Data() ), Form( "%s  %s;#eta;JetArea_{FastJet}", it->first.Data(),
                label.second.Data() ), 150, -6, 6, 200, 0, 10 );
        h_PU20bx25_area_eta[it->first] = new TH2F( Form( "h_PU20bx25_area_eta_%s", it->first.Data() ), Form( "%s  %s;#eta;JetArea_{FastJet}", it->first.Data(),
                label.second.Data() ), 150, -6, 6, 200, 0, 10 );

        // response vs pt / cathogories
        h_PU40bx50_res_pt_cat1[it->first] = new TH2F( Form( "h_PU40bx50_res_pt_cat1_%s", it->first.Data() ), Form( "%s  %s;p_{t};p^{reco}_{t}/p^{gen}_{t}",
                it->first.Data(), "PU40bx50" ), 60, 0, 500, 200, 0, 10 );
        h_PU20bx25_res_pt_cat1[it->first] = new TH2F( Form( "h_PU20bx25_res_pt_cat1_%s", it->first.Data() ), Form( "%s  %s;p_{t};p^{reco}_{t}/p^{gen}_{t}",
                it->first.Data(), "PU20bx25" ), 60, 0, 500, 200, 0, 10 );
        h_PU40bx50_res_pt_cat2[it->first] = new TH2F( Form( "h_PU40bx50_res_pt_cat2_%s", it->first.Data() ), Form( "%s  %s;p_{t};p^{reco}_{t}/p^{gen}_{t}",
                it->first.Data(), "PU40bx50" ), 60, 0, 500, 200, 0, 10 );
        h_PU20bx25_res_pt_cat2[it->first] = new TH2F( Form( "h_PU20bx25_res_pt_cat2_%s", it->first.Data() ), Form( "%s  %s;p_{t};p^{reco}_{t}/p^{gen}_{t}",
                it->first.Data(), "PU20bx25" ), 60, 0, 500, 200, 0, 10 );
        h_PU40bx50_res_pt_cat3[it->first] = new TH2F( Form( "h_PU40bx50_res_pt_cat3_%s", it->first.Data() ), Form( "%s  %s;p_{t};p^{reco}_{t}/p^{gen}_{t}",
                it->first.Data(), "PU40bx50" ), 60, 0, 500, 200, 0, 10 );
        h_PU20bx25_res_pt_cat3[it->first] = new TH2F( Form( "h_PU20bx25_res_pt_cat3_%s", it->first.Data() ), Form( "%s  %s;p_{t};p^{reco}_{t}/p^{gen}_{t}",
                it->first.Data(), "PU20bx25" ), 60, 0, 500, 200, 0, 10 );

        f_PU40bx50_res_pt_cat1[it->first] = new TF1( Form( "f_PU40bx50_res_pt_cat1_%s", it->first.Data() ), JetResponseFun, 20, 500, 4 );
        f_PU20bx25_res_pt_cat1[it->first] = new TF1( Form( "f_PU20bx25_res_pt_cat1_%s", it->first.Data() ), JetResponseFun, 20, 500, 4 );
        f_PU40bx50_res_pt_cat2[it->first] = new TF1( Form( "f_PU40bx50_res_pt_cat2_%s", it->first.Data() ), JetResponseFun, 20, 500, 4 );
        f_PU20bx25_res_pt_cat2[it->first] = new TF1( Form( "f_PU20bx25_res_pt_cat2_%s", it->first.Data() ), JetResponseFun, 20, 500, 4 );
        f_PU40bx50_res_pt_cat3[it->first] = new TF1( Form( "f_PU40bx50_res_pt_cat3_%s", it->first.Data() ), JetResponseFun, 20, 500, 4 );
        f_PU20bx25_res_pt_cat3[it->first] = new TF1( Form( "f_PU20bx25_res_pt_cat3_%s", it->first.Data() ), JetResponseFun, 20, 500, 4 );

        f_PU40bx50_res_pt_cat1[it->first]->SetParameters( 1, 0.01, 1, 0 );
        f_PU20bx25_res_pt_cat1[it->first]->SetParameters( 1, 0.01, 1, 0 );
        f_PU40bx50_res_pt_cat2[it->first]->SetParameters( 1, 0.01, 1, 0 );
        f_PU20bx25_res_pt_cat2[it->first]->SetParameters( 1, 0.01, 1, 0 );
        f_PU40bx50_res_pt_cat3[it->first]->SetParameters( 1, 0.01, 1, 0 );
        f_PU20bx25_res_pt_cat3[it->first]->SetParameters( 1, 0.01, 1, 0 );


        JetTree_PU20bx25[it->first]->Draw( Form( "(bestPt/genJetPt):bestPt>>h_PU20bx25_res_pt_cat1_%s", it->first.Data() ), Form( "%s && abs(eta) <2.5",
                                           pt_cut_all.Data() ), "PU40bx50" );
        JetTree_PU40bx50[it->first]->Draw( Form( "(bestPt/genJetPt):bestPt>>h_PU40bx50_res_pt_cat1_%s", it->first.Data() ), Form( "%s && abs(eta) <2.5",
                                           pt_cut_all.Data() ), "PU20bx25" );
        p_PU40bx50_res_pt_cat1[it->first] = h_PU40bx50_res_pt_cat1[it->first]->ProfileX();
        p_PU20bx25_res_pt_cat1[it->first] = h_PU20bx25_res_pt_cat1[it->first]->ProfileX();
        p_PU40bx50_res_pt_cat1[it->first]->SetTitle( h_PU40bx50_res_pt_cat1[it->first]->GetTitle() ) ;
        p_PU20bx25_res_pt_cat1[it->first]->SetTitle( h_PU20bx25_res_pt_cat1[it->first]->GetTitle() ) ;

        JetTree_PU20bx25[it->first]->Draw( Form( "(bestPt/genJetPt):bestPt>>h_PU20bx25_res_pt_cat2_%s", it->first.Data() ), Form( "%s && abs(eta)>2.5 && abs(eta)<3.0",
                                           pt_cut_all.Data() ), "" );
        JetTree_PU40bx50[it->first]->Draw( Form( "(bestPt/genJetPt):bestPt>>h_PU40bx50_res_pt_cat2_%s", it->first.Data() ), Form( "%s && abs(eta)>2.5 && abs(eta)<3.0",
                                           pt_cut_all.Data() ), "" );
        p_PU40bx50_res_pt_cat2[it->first] = h_PU40bx50_res_pt_cat2[it->first]->ProfileX();
        p_PU20bx25_res_pt_cat2[it->first] = h_PU20bx25_res_pt_cat2[it->first]->ProfileX();
        p_PU40bx50_res_pt_cat2[it->first]->SetTitle( h_PU40bx50_res_pt_cat2[it->first]->GetTitle() ) ;
        p_PU20bx25_res_pt_cat2[it->first]->SetTitle( h_PU20bx25_res_pt_cat2[it->first]->GetTitle() ) ;

        JetTree_PU20bx25[it->first]->Draw( Form( "(bestPt/genJetPt):bestPt>>h_PU20bx25_res_pt_cat3_%s", it->first.Data() ), Form( "%s && abs(eta)>3.0 && abs(eta)<5.0",
                                           pt_cut_all.Data() ), "" );
        JetTree_PU40bx50[it->first]->Draw( Form( "(bestPt/genJetPt):bestPt>>h_PU40bx50_res_pt_cat3_%s", it->first.Data() ), Form( "%s && abs(eta)>3.0 && abs(eta)<5.0",
                                           pt_cut_all.Data() ), "" );
        p_PU40bx50_res_pt_cat3[it->first] = h_PU40bx50_res_pt_cat3[it->first]->ProfileX();
        p_PU20bx25_res_pt_cat3[it->first] = h_PU20bx25_res_pt_cat3[it->first]->ProfileX();
        p_PU40bx50_res_pt_cat3[it->first]->SetTitle( h_PU40bx50_res_pt_cat3[it->first]->GetTitle() ) ;
        p_PU20bx25_res_pt_cat3[it->first]->SetTitle( h_PU20bx25_res_pt_cat3[it->first]->GetTitle() ) ;

        // fill the histograms
        JetTree_PU20bx25[it->first]->Draw( Form( "(bestPt/genJetPt)>>h_PU20bx25_ptOffset_%s", it->first.Data() ), pt_cut.Data(), "" );
        JetTree_PU40bx50[it->first]->Draw( Form( "(bestPt/genJetPt)>>h_PU40bx50_ptOffset_%s", it->first.Data() ), pt_cut.Data(), "" );

        JetTree_PU20bx25[it->first]->Draw( Form( "(bestPt/genJetPt)>>h_PU20bx25_ptOffset_etacut_%s", it->first.Data() ), Form( "%s && abs(eta) <2.5", pt_cut.Data() ),
                                           "" );
        JetTree_PU40bx50[it->first]->Draw( Form( "(bestPt/genJetPt)>>h_PU40bx50_ptOffset_etacut_%s", it->first.Data() ), Form( "%s && abs(eta) <2.5", pt_cut.Data() ),
                                           "" );

        JetTree_PU20bx25[it->first]->Draw( Form( "(bestPt/genJetPt):eta>>h_PU20bx25_ptOffset_eta_%s", it->first.Data() ), pt_cut.Data(), "" );
        JetTree_PU40bx50[it->first]->Draw( Form( "(bestPt/genJetPt):eta>>h_PU40bx50_ptOffset_eta_%s", it->first.Data() ), pt_cut.Data(), "" );

        p_PU40bx50_ptOffset_eta[it->first] = h_PU40bx50_ptOffset_eta[it->first]->ProfileX();
        p_PU20bx25_ptOffset_eta[it->first] = h_PU20bx25_ptOffset_eta[it->first]->ProfileX();

        p_PU40bx50_ptOffset_eta[it->first]->SetTitle( h_PU40bx50_ptOffset_eta[it->first]->GetTitle() ) ;
        p_PU20bx25_ptOffset_eta[it->first]->SetTitle( h_PU20bx25_ptOffset_eta[it->first]->GetTitle() ) ;
    }

    for( std::map<TString, TTree *>::iterator it = JetTree_PU20bx25.begin(); it != JetTree_PU20bx25.end(); ++it ) {
        Canv["ptOffset_" + it->first] = new TCanvas( Form( "c_eta_%s", it->first.Data() ), "", 500, 500 );
        Canv["ptOffset_" + it->first]->cd();
        gPad->SetLogy();

        h_PU20bx25_ptOffset[it->first]->SetLineColor( 1 );
        h_PU40bx50_ptOffset[it->first]->SetLineColor( 2 );
        h_PU20bx25_ptOffset[it->first]->SetMarkerColor( 1 );
        h_PU40bx50_ptOffset[it->first]->SetMarkerColor( 2 );
        h_PU20bx25_ptOffset_etacut[it->first]->SetLineColor( 1 );
        h_PU40bx50_ptOffset_etacut[it->first]->SetLineColor( 2 );
        h_PU20bx25_ptOffset_etacut[it->first]->SetLineStyle( 2 );
        h_PU40bx50_ptOffset_etacut[it->first]->SetLineStyle( 2 );
        h_PU20bx25_ptOffset_etacut[it->first]->SetMarkerColor( 1 );
        h_PU40bx50_ptOffset_etacut[it->first]->SetMarkerColor( 2 );
        //h_PU20bx25_ptOffset[it->first]->GetYaxis()->SetRangeUser(0,500);

        h_PU20bx25_ptOffset[it->first]->Draw();
        h_PU20bx25_ptOffset_etacut[it->first]->Draw( "same" );
        h_PU40bx50_ptOffset[it->first]->Draw( "same" );
        h_PU40bx50_ptOffset_etacut[it->first]->Draw( "same" );

        Draw2Legend( h_PU20bx25_ptOffset[it->first],
                     h_PU40bx50_ptOffset[it->first],
                     "PU20bx25",
                     "PU40bx50" );

        Draw2Legend( h_PU20bx25_ptOffset[it->first],
                     h_PU20bx25_ptOffset_etacut[it->first],
                     "no #eta cut",
                     "|#eta |<2.5", "2" );


        Canv["res_pt_" + it->first] = new TCanvas( Form( "res_pt_%s", it->first.Data() ), "", 1000, 500 );
        Canv["res_pt_" + it->first]->Divide( 2, 1 );
        std::cout << Form( "res_pt_%s", it->first.Data() ) << std::endl;
        Canv["res_pt_" + it->first]->cd( 1 );
        //gPad->SetLogy();

        p_PU20bx25_res_pt_cat1[it->first]->SetLineColor( 55 );
        p_PU20bx25_res_pt_cat1[it->first]->SetMarkerColor( 55 );
        p_PU20bx25_res_pt_cat2[it->first]->SetLineColor( 65 );
        p_PU20bx25_res_pt_cat2[it->first]->SetMarkerColor( 65 );
        p_PU20bx25_res_pt_cat3[it->first]->SetLineColor( 95 );
        p_PU20bx25_res_pt_cat3[it->first]->SetMarkerColor( 95 );
        p_PU20bx25_res_pt_cat1[it->first]->GetYaxis()->SetRangeUser( 0, 2 );

        p_PU20bx25_res_pt_cat1[it->first]->Fit( Form( "f_PU20bx25_res_pt_cat1_%s", it->first.Data() ) );
        p_PU20bx25_res_pt_cat2[it->first]->Fit( Form( "f_PU20bx25_res_pt_cat2_%s", it->first.Data() ) );
        p_PU20bx25_res_pt_cat3[it->first]->Fit( Form( "f_PU20bx25_res_pt_cat3_%s", it->first.Data() ) );

        p_PU20bx25_res_pt_cat1[it->first]->Draw( "" );
        p_PU20bx25_res_pt_cat2[it->first]->Draw( "same" );
        p_PU20bx25_res_pt_cat3[it->first]->Draw( "same" );

        Draw3Legend( p_PU20bx25_res_pt_cat1[it->first],
                     p_PU20bx25_res_pt_cat2[it->first],
                     p_PU20bx25_res_pt_cat3[it->first],
                     "|#eta|<2.5",
                     "2.5<|#eta|<3.0",
                     "3.0<|#eta|<5.0" );

        Canv["res_pt_" + it->first]->cd( 2 );
        //gPad->SetLogy();

        p_PU40bx50_res_pt_cat1[it->first]->SetLineColor( 55 );
        p_PU40bx50_res_pt_cat1[it->first]->SetMarkerColor( 55 );
        p_PU40bx50_res_pt_cat2[it->first]->SetLineColor( 65 );
        p_PU40bx50_res_pt_cat2[it->first]->SetMarkerColor( 65 );
        p_PU40bx50_res_pt_cat3[it->first]->SetLineColor( 95 );
        p_PU40bx50_res_pt_cat3[it->first]->SetMarkerColor( 95 );
        p_PU40bx50_res_pt_cat1[it->first]->GetYaxis()->SetRangeUser( 0, 2 );

        p_PU40bx50_res_pt_cat1[it->first]->Fit( Form( "f_PU40bx50_res_pt_cat1_%s", it->first.Data() ) );
        p_PU40bx50_res_pt_cat2[it->first]->Fit( Form( "f_PU40bx50_res_pt_cat2_%s", it->first.Data() ) );
        p_PU40bx50_res_pt_cat3[it->first]->Fit( Form( "f_PU40bx50_res_pt_cat3_%s", it->first.Data() ) );

        p_PU40bx50_res_pt_cat1[it->first]->Draw( "" );
        p_PU40bx50_res_pt_cat2[it->first]->Draw( "same" );
        p_PU40bx50_res_pt_cat3[it->first]->Draw( "same" );

        Draw3Legend( p_PU40bx50_res_pt_cat1[it->first],
                     p_PU40bx50_res_pt_cat2[it->first],
                     p_PU40bx50_res_pt_cat3[it->first],
                     "|#eta|<2.5",
                     "2.5<|#eta|<3.0",
                     "3.0<|#eta|<5.0" );


        Canv["res_pt_" + it->first]->SaveAs( Form( "plots/pt_res_%s_%s.C", it->first.Data(), label.first.Data() ) );
        Canv["res_pt_" + it->first]->SaveAs( Form( "plots/pt_res_%s_%s.pdf", it->first.Data(), label.first.Data() ) );

        Canv["ptOffset_eta_" + it->first] = new TCanvas( Form( "c_eta_corr_%s", it->first.Data() ), "", 500, 500 );
        Canv["ptOffset_eta_" + it->first]->cd();

        p_PU20bx25_ptOffset_eta[it->first]->SetLineColor( 1 );
        p_PU40bx50_ptOffset_eta[it->first]->SetLineColor( 2 );
        p_PU20bx25_ptOffset_eta[it->first]->SetMarkerColor( 1 );
        p_PU40bx50_ptOffset_eta[it->first]->SetMarkerColor( 2 );
        p_PU20bx25_ptOffset_eta[it->first]->GetYaxis()->SetRangeUser( 0, 4 );
        p_PU40bx50_ptOffset_eta[it->first]->GetYaxis()->SetRangeUser( 0, 4 );
        p_PU20bx25_ptOffset_eta[it->first]->GetYaxis()->SetTitle( "p^{reco}_{t}/p^{gen}_{t}" );
        p_PU40bx50_ptOffset_eta[it->first]->GetYaxis()->SetTitle( "p^{reco}_{t}/p^{gen}_{t}" );
        p_PU20bx25_ptOffset_eta[it->first]->Draw();
        p_PU40bx50_ptOffset_eta[it->first]->Draw( "same" );
        Draw2Legend( p_PU20bx25_ptOffset_eta[it->first],
                     p_PU40bx50_ptOffset_eta[it->first],
                     "PU20bx25",
                     "PU40bx50" );

        Canv["ptOffset_eta_" + it->first]->SaveAs( Form( "plots/profile_eta_pt_offset_%s_%s.C", it->first.Data(), label.first.Data() ) );
        Canv["ptOffset_eta_" + it->first]->SaveAs( Form( "plots/profile_eta_pt_offset_%s_%s.pdf", it->first.Data(), label.first.Data() ) );


        Canv["2ddist_" + it->first] = new TCanvas( Form( "2ddist_%s", it->first.Data() ), "", 1000, 500 );
        Canv["2ddist_" + it->first]->Divide( 2, 1 );

        Canv["2ddist_" + it->first]->cd( 1 );
        gPad->SetLogz();
        h_PU40bx50_ptOffset_eta[it->first]->Draw( "colz" );
        p_PU40bx50_ptOffset_eta[it->first]->Draw( "same" );
        Canv["2ddist_" + it->first]->cd( 2 );
        gPad->SetLogz();
        h_PU20bx25_ptOffset_eta[it->first]->Draw( "colz" );
        p_PU20bx25_ptOffset_eta[it->first]->Draw( "same" );

        Canv["2ddist_" + it->first]->SaveAs( Form( "plots/scatter_eta_pt_offset_%s_%s.C", it->first.Data(), label.first.Data() ) );
        Canv["2ddist_" + it->first]->SaveAs( Form( "plots/scatter_eta_pt_offset_%s_%s.pdf", it->first.Data(), label.first.Data() ) );
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

