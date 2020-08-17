#include "TH1.h"
#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TString.h"
#include <sstream>

TGraph *makeROC2( TH1 *sig, TH1 *bkg, TLegend *leg, int tc )
{

    gStyle->SetOptStat( 0 );
    sig->Sumw2( 0 );
    bkg->Sumw2( 0 );
    TGraph *g_e = new TGraph();

    g_e->SetMarkerColor( tc );
    g_e->SetLineColor( tc );
    g_e->SetMarkerSize( 1 );
    //g_e->SetMarkerStyle(21);

    bool printFlag = 0;
    int N0 = bkg->GetNbinsX();
    const unsigned int N = N0;
    float y[N];
    float signal[N];
    int i = 0;

    for( unsigned int n = 0; n < N; n++ ) {
        signal[n] =  sig->Integral( 0, n ) / sig->Integral( 0, N );
        y[n]     = ( bkg->Integral( 0, n ) / bkg->Integral( 0, N ) );

        if( y[n] == 0 ) {
            i++;
            continue;
        }

        if( signal[n] > 0.98 && printFlag == 0 ) {
            std::cout << "Better than 97 pc eff.("
                      << signal[n] << "). Bin "
                      << n << ", x value "
                      << n *sig->GetBinWidth( n ) << std::endl;

            printFlag = 1;
        }
        //g_e->SetPoint(n-i,1-signal[n], y[n]);
        g_e->SetPoint( n - i, 1 - y[n], 1 - signal[n] );

        //use 1-n for signal because I am integrating from bin 0 to bin N, which is techincally asking how to reject signal.
    }
    g_e->SetTitle( Form( "ROCs_%i", tc ) );


    //g_e->GetXaxis()->SetRangeUser(0,1);
    //g_e->GetXaxis()->SetTitle("background efficiency");
    //g_e->GetYaxis()->SetRangeUser(0,1);
    //g_e->GetYaxis()->SetTitle("signal efficiency");
    //
    //if (tc ==1){
    //  g_e->Draw("apl same");
    //} else {
    //  g_e->Draw("pl same");
    //}
    //g_g->Draw("pl same");:w
    //
    //g_eg->Draw("pl same");
    //std::string name2 = name.str().replace(name.str().find("_signal_h"), 6, "");

    if( tc == 1 ) {
        leg->AddEntry( g_e, "current FLASHgg MVA", "lep" );
    } else {
        leg->AddEntry( g_e, "retrained MVA", "lep" );
    }
    return g_e;
    //leg->Draw();
}

TGraph *makeROC( TH1 *sig, TH1 *bkg, TLegend *leg, int tc )
{

    gStyle->SetOptStat( 0 );
    sig->Sumw2( 0 );
    bkg->Sumw2( 0 );
    TGraph *g_e = new TGraph();

    g_e->SetMarkerColor( tc );
    g_e->SetLineColor( tc );
    g_e->SetMarkerSize( 1 );
    //g_e->SetMarkerStyle(21);

    bool printFlag = 0;
    int N0 = bkg->GetNbinsX();
    const unsigned int N = N0;
    float y[N];
    float signal[N];
    int i = 0;
    for( unsigned int n = 0; n < N; n++ ) {


        signal[n] = sig->Integral( 0, n ) / sig->Integral( 0, N );

        y[n] = ( bkg->Integral( 0, n ) / bkg->Integral( 0, N ) );

        if( y[n] == 0 ) {
            i++;
            continue;
        }
        if( signal[n] > 0.98 && printFlag == 0 ) {

            std::cout << "Better than 97 pc eff.(" << signal[n] << "). Bin " << n << ", x value " << n *sig->GetBinWidth( n ) << std::endl;
            printFlag = 1;
        }
        g_e->SetPoint( n - i, 1 - signal[n], y[n] );
        //use 1-n for signal because I am integrating from bin 0 to bin N, which is techincally asking how to reject signal.


    }
    g_e->SetTitle( "ROCs" );



    g_e->GetXaxis()->SetTitle( "signal efficiency" );
    g_e->GetXaxis()->SetRangeUser( 0, 1 );
    g_e->GetYaxis()->SetRangeUser( 0, 1 );
    g_e->GetYaxis()->SetTitle( "background rejection" );
    if( tc == 1 ) {
        g_e->Draw( "apl same" );
    } else {
        g_e->Draw( "pl same" );

    }
    //g_g->Draw("pl same");:w
    //
    //g_eg->Draw("pl same");
    //std::string name2 = name.str().replace(name.str().find("_signal_h"), 6, "");

    if( tc == 1 ) {
        leg->AddEntry( g_e, "current FLASHgg MVA", "lep" );
    } else {
        leg->AddEntry( g_e, "retrained MVA", "lep" );
    }

    //leg->Draw();
    return g_e;
}


void makeROCsDiPho( TString eventN = "10000", TString Level = "DiPhoton" )
{

    int nevent = std::atoi( eventN.Data() );

    TString path;
    path = "${WORKSPACE}/test_dipho_compare/";

    TFile *f_sig1 = TFile::Open( path + "output_GluGluToHToGG_M-125_13TeV-powheg-pythia6_numEvent" + eventN + "_histos.root" );
    TFile *f_sig2 = TFile::Open( path + "output_TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola_numEvent" + eventN + "_histos.root" );
    TFile *f_sig3 = TFile::Open( path + "output_VBF_HToGG_M-125_13TeV-powheg-pythia6_numEvent" + eventN + "_histos.root" );
    TFile *f_sig4 = TFile::Open( path + "output_WH_ZH_HToGG_M-125_13TeV_pythia6_numEvent" + eventN + "_histos.root" );

    TH1F *old_sig1 = ( TH1F * ) f_sig1->Get( Level + "MVADumper/histograms/ggh_m125_13TeV_All" + Level + "MVAValue" );
    TH1F *old_sig2 = ( TH1F * ) f_sig2->Get( Level + "MVADumper/histograms/tth_m125_13TeV_All" + Level + "MVAValue" );
    TH1F *old_sig3 = ( TH1F * ) f_sig3->Get( Level + "MVADumper/histograms/vbf_m125_13TeV_All" + Level + "MVAValue" );
    TH1F *old_sig4 = ( TH1F * ) f_sig4->Get( Level + "MVADumper/histograms/wzh_m125_13TeV_All" + Level + "MVAValue" );

    float wS[5];
    wS[0] = 0;
    wS[1] = 43.92 * 2.28e-3 / ( nevent ); //ggH
    wS[2] = 3.748 * 2.28e-3 / ( nevent ); //ttH
    wS[3] = 2.2496 * 2.28e-3 / ( nevent ); //VBH
    wS[4] = 0.5608 * 2.28e-3 / ( nevent ); //WZH

    TH1F *old_sig = new TH1F( "old_sig", "old_sig", old_sig1->GetNbinsX(), -1, 1 );

    old_sig->Add( old_sig1, wS[1] );
    old_sig->Add( old_sig2, wS[2] );
    old_sig->Add( old_sig3, wS[3] );
    old_sig->Add( old_sig4, wS[4] );

//    std::cout << Level + "MVADumper/histograms/vbf_m125_13TeV_Good" + Level + "Leg" + Level + "MVAValue" << std::endl ;
    TH1F *new_sig1 = ( TH1F * ) f_sig1->Get( Level + "MVADumperNew/histograms/ggh_m125_13TeV_All" + Level + "MVAValue" );
    TH1F *new_sig2 = ( TH1F * ) f_sig2->Get( Level + "MVADumperNew/histograms/tth_m125_13TeV_All" + Level + "MVAValue" );
    TH1F *new_sig3 = ( TH1F * ) f_sig3->Get( Level + "MVADumperNew/histograms/vbf_m125_13TeV_All" + Level + "MVAValue" );
    TH1F *new_sig4 = ( TH1F * ) f_sig4->Get( Level + "MVADumperNew/histograms/wzh_m125_13TeV_All" + Level + "MVAValue" );

    TH1F *new_sig = new TH1F( "new_sig", "new_sig", new_sig1->GetNbinsX(), -1, 1 );

    new_sig->Add( new_sig1, wS[1] );
    new_sig->Add( new_sig2, wS[2] );
    new_sig->Add( new_sig3, wS[3] );
    new_sig->Add( new_sig4, wS[4] );

    TFile *f_bkg1 = TFile::Open( path + "output_DYJetsToLL_M-50_13TeV-madgraph-pythia8_numEvent" + eventN + "_histos.root" );
    TFile *f_bkg2 = TFile::Open( path + "output_GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6_numEvent" + eventN + "_histos.root" );
    TFile *f_bkg3 = TFile::Open( path + "output_GJet_Pt20to40_doubleEMEnriched_TuneZ2star_13TeV-pythia6_numEvent" + eventN + "_histos.root" );

    TH1F *old_bkg1 = ( TH1F * ) f_bkg1->Get( Level + "MVADumper/histograms/dyJets_13TeV_All" + Level + "MVAValue" );
    TH1F *old_bkg2 = ( TH1F * ) f_bkg2->Get( Level + "MVADumper/histograms/gamJet_13TeV_All" + Level + "MVAValue" );
    TH1F *old_bkg3 = ( TH1F * ) f_bkg3->Get( Level + "MVADumper/histograms/gamJet_13TeV_All" + Level + "MVAValue" );

    TH1F *new_bkg1 = ( TH1F * ) f_bkg1->Get( Level + "MVADumperNew/histograms/dyJets_13TeV_All" + Level + "MVAValue" );
    TH1F *new_bkg2 = ( TH1F * ) f_bkg2->Get( Level + "MVADumperNew/histograms/gamJet_13TeV_All" + Level + "MVAValue" );
    TH1F *new_bkg3 = ( TH1F * ) f_bkg3->Get( Level + "MVADumperNew/histograms/gamJet_13TeV_All" + Level + "MVAValue" );

    float wB[4];

    wB[0] = 0;
    wB[1] = 4746.0 / ( nevent ); // DyJetsToLL
    wB[2] = 17180.0 * 0.0379 / ( nevent ); //gamJets pt>40
    wB[3] = 145400.0 * 0.001776 / ( nevent ); //gamJets pt  in 20->40

    TH1F *old_bkg = new TH1F( "old_bkg", "old_bkg", old_bkg1->GetNbinsX(), -1, 1 );

    old_bkg->Add( old_bkg1, wB[1] );
    old_bkg->Add( old_bkg2, wB[2] );
    old_bkg->Add( old_bkg3, wB[3] );

    TH1F *new_bkg = new TH1F( "new_bkg", "new_bkg", new_bkg2->GetNbinsX(), -1, 1 );

    new_bkg->Add( new_bkg1, wB[1] );
    new_bkg->Add( new_bkg2, wB[2] );
    new_bkg->Add( new_bkg3, wB[3] );

    std::cout << "plotting" << std::endl;
    std::cout << " --> obj::old_bkg::" << old_bkg  << std::endl;
    std::cout << " --> obj::new_bkg::" << new_bkg  << std::endl;
    std::cout << " --> obj::old_sig::" << old_sig  << std::endl;
    std::cout << " --> obj::new_sig::" << new_sig  << std::endl;
    TCanvas *c1 = new TCanvas( "c1", "", 600, 600 );
    c1->cd();
//	gStyle->SetNdivisions(810,"X");
//	gStyle->SetNdivisions(810,"Y");
    c1->SetGridx();
    c1->SetGridy();
    c1->Modified();
    c1->Update();

// old_bkg->Draw();
    std::vector<std::string> xxx;//={"signal","hgg"};
    xxx.push_back( "current MVA" );
    xxx.push_back( "retrained MVA" );

    int itc = 1;
    TLegend *leg = new TLegend( 0.3, 0.3, 0.65, 0.5 );

    TMultiGraph *mul  = new TMultiGraph();

    mul->Add( makeROC2( old_sig, old_bkg, leg, itc ) );
    itc++;
    mul->Add( makeROC2( new_sig, new_bkg, leg, itc ) );
    mul->SetTitle( ";background efficiency;signal efficiency" );
    mul->Draw( "APL" );
    leg->Draw();

    c1->SaveAs( "plots/" + Level + "_ROCs.pdf" );
    c1->SaveAs( "plots/" + Level + "_ROCs.root" );
    //TCanvas *c2 = new TCanvas("c2","",600,600);
    //c2->cd();
    //new_bkg->SetLineColor(kBlue);
    //new_sig->SetLineColor(kRed);
    //new_bkg->Draw();
    //new_sig->Draw("same");


}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
