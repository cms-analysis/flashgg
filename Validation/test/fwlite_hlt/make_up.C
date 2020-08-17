#include <iostream>
#include "Riostream.h"
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <TEfficiency.h>
#include <TLegend.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraphAsymmErrors.h>
#include "TStyle.h"
#include "TCanvas.h"

//#define Maxselection 1
//#define Nhltpaths 442


TCanvas *Ca0 = new TCanvas( "Ca0", "bit0", 1200, 800 );

void binomialEfficiency1D( TH1F *numerator, TH1F *denominator, string option )
{
    //  TH1F * efficiency = numerator->Clone("efficiency");
    TGraphAsymmErrors *efficiency = new TGraphAsymmErrors( numerator, denominator );
    efficiency->SetTitle( "Efficiency" );
    //efficiency->GetXaxis()->SetTitle(numerator->GetXaxis()->GetTitle());
    efficiency->GetYaxis()->SetTitle( "#epsilon" );


    /*
    for(int j=0;j<=numerator->GetXaxis()->GetNbins() ;j++){
      if(denominator->GetBinContent(j)!=0){
        float eff = numerator->GetBinContent(j)/denominator->GetBinContent(j);
        float err = sqrt(eff*(1-eff)/denominator->GetBinContent(j));
        //if(err==0.)continue;
        efficiency->SetBinContent(j,eff);
        efficiency->SetBinError(j,err);
        //cout<<"1Deff "<<j<<" "<<eff<<" +/- "<<err<<endl;
      }
    }
    */

    efficiency->Draw( "AP" );
    efficiency->SetMarkerColor( kRed );
    efficiency->SetMarkerStyle( 23 );
    efficiency->SetMarkerSize( 2 );
    if( option == "p" ) { efficiency->GetXaxis()->SetRangeUser( 0., 120. ); }
    else if( option == "eta" ) { efficiency->GetXaxis()->SetLimits( -5., 5. ); }
    else if( option == "phi" ) { efficiency->GetXaxis()->SetLimits( -3.1416, 3.1416 ); }
    else if( option == "pn" ) { efficiency->GetXaxis()->SetLimits( 0., 1. ); }
    else if( option == "m" ) { efficiency->GetXaxis()->SetLimits( 60., 180. ); }

    efficiency->GetYaxis()->SetRangeUser( 0., 1. );

    //efficiency->Delete();

    //theFile->cd();
    //efficiency->Write();
}


void Draw1D( string savedname, string theXtitle, TFile *theFile, int cut1, int cut2 )
{

    TLegend *leg = new TLegend( 0.65, 0.93, 0.89, 0.7 );

    char char_cut1[20];
    char char_cut2[20];

    TH1F *histoArray[9  ];

    for( int selection = 0; selection < 9  ; selection++ ) { //Loop over the different histograms
        //std::string histo = std::to_string(selection);
        char histo[20];
        sprintf( histo, "%d", selection );
        //cout<<savedname+"_"+histo<<endl;
        sprintf( char_cut1, "%d", cut1 );
        sprintf( char_cut2, "%d", cut2 );
        histoArray[selection] = ( TH1F * )( theFile->Get( ( savedname + "_" + histo ).c_str() ) );
        if( selection == 0 ) {
            histoArray[selection]->Draw();
            histoArray[selection]->SetFillColor( kBlue );
            leg->AddEntry( histoArray[selection], "minitree", "f" );
        } else if( selection == 1 && ( cut1 == 1 || cut2 == 1 ) ) {
            histoArray[selection]->Draw( "same" );
            histoArray[selection]->SetFillColor( kRed );
            leg->AddEntry( histoArray[selection], "cut[1]", "f" );
        } else if( selection == 2 && ( cut1 == 2 || cut2 == 2 ) ) {
            histoArray[selection]->Draw( "same" );
            histoArray[selection]->SetFillColor( kYellow );
            leg->AddEntry( histoArray[selection], "cut[2]", "f" );
        } else if( selection == 3 && ( cut1 == 3 || cut2 == 3 ) ) {
            histoArray[selection]->Draw( "same" );
            histoArray[selection]->SetFillColor( kRed );
            leg->AddEntry( histoArray[selection], "cut[3]", "f" );
        } else if( selection == 4 && ( cut1 == 4 || cut2 == 4 ) ) {
            histoArray[selection]->Draw( "same" );
            histoArray[selection]->SetFillColor( kYellow );
            leg->AddEntry( histoArray[selection], "cut[4]", "f" );

        } else if( selection == 5 && ( cut1 == 5 || cut2 == 5 ) ) {
            histoArray[selection]->Draw( "same" );
            histoArray[selection]->SetFillColor( kRed );
            leg->AddEntry( histoArray[selection], "cut[5]" );
        } else if( selection == 6 && ( cut1 == 6 || cut2 == 6 ) ) {
            histoArray[selection]->Draw( "same" );
            histoArray[selection]->SetFillColor( kYellow );
            leg->AddEntry( histoArray[selection], "cut[6]", "f" );
        } else if( selection == 7 && ( cut1 == 7 || cut2 == 7 ) ) {
            histoArray[selection]->Draw( "same" );
            histoArray[selection]->SetFillColor( kRed );
            leg->AddEntry( histoArray[selection], "cut[7]" );
        } else if( selection == 8 && ( cut1 == 8 || cut2 == 8 ) ) {
            histoArray[selection]->Draw( "same" );
            histoArray[selection]->SetFillColor( kYellow );
            leg->AddEntry( histoArray[selection], "cut[8]", "f" );
        }
        histoArray[selection]->SetXTitle( theXtitle.c_str() );
        histoArray[selection]->SetMinimum( 0.9 );
    }

    string folder = "plots/";
    leg->SetFillColor( 0 );
    leg->Draw( "same" );
    Ca0->SaveAs( ( folder + char_cut2 + char_cut1 + savedname + ".png" ).c_str() );
    Ca0->Clear();
}

void binomialEfficiency2D( TH2F *numerator, TH2F *denominator, bool text2D, bool ptaxis )
{
    if( !numerator ) { cout << "numerator not found" << endl; }
    if( !denominator ) { cout << "denominator not found" << endl; }

    TH2F *efficiency = ( TH2F * ) numerator->Clone( "efficiency" );

    efficiency->SetTitle( "Efficiency" );

    if( ptaxis ) {
        efficiency->SetXTitle( "p_{T} Lead (GeV)" );
        efficiency->SetYTitle( "p_{T} Trail (GeV)" );
    } else {
        efficiency->SetXTitle( denominator->GetXaxis()->GetTitle() );
        efficiency->SetYTitle( denominator->GetYaxis()->GetTitle() );
    }

    float eff, err;
    for( int i = 0; i <= numerator->GetXaxis()->GetNbins(); i++ )
        for( int j = 0; j <= numerator->GetYaxis()->GetNbins(); j++ ) {
            if( denominator->GetBinContent( i, j ) != 0 ) {
                eff = numerator->GetBinContent( i, j ) / denominator->GetBinContent( i, j );
                err = sqrt( eff * ( 1 - eff ) / denominator->GetBinContent( i, j ) );
                //if(err==0.)continue;
                efficiency->SetBinContent( i, j, eff );
                efficiency->SetBinError( i, j, err );
                //cout<<i<<" "<<j<<" "<<eff<<"+/-"<<err<<endl;
            }
        }
    if( text2D == true ) {
        efficiency->Draw( "colztextE" );
    } else {
        efficiency->Draw( "colz" );
    }
}

void Draw2D( string savedname, string theXtitle, string theYtitle, TFile *theFile, bool text2D )
{
    TH2F *histoArray2D[9  ];
    for( int selection = 0; selection < 9  ; selection++ ) { //Loop over the different histograms
        //std::string histo = std::to_string(selection); THIS WORKS IN C++ BUT NOT IN CINT
        char histo[20];
        sprintf( histo, "%d", selection );
        //cout<<savedname+"_"+histo<<endl;
        histoArray2D[selection] = ( TH2F * )( theFile->Get( ( savedname + "_" + histo ).c_str() ) );
        if( selection < 9 ) {
            histoArray2D[selection]->SetXTitle( theXtitle.c_str() );
            histoArray2D[selection]->SetYTitle( theYtitle.c_str() );
            if( text2D ) {
                histoArray2D[selection]->DrawNormalized( "colztext" );
            } else {
                histoArray2D[selection]->Draw( "colz" );
            }
            Ca0->SaveAs( ( "plots/" + savedname + "_" + histo + ".png" ).c_str() );
            Ca0->Clear();
        }
    }
}

void make_up()
{
    //  gROOT->Reset();
    //  gStyle->SetOptStat(1111);
    gStyle->SetOptStat( 0 );
    gStyle->SetPalette( 1 );
    gStyle->SetPaintTextFormat( "2.3f" );

    TFile *theFile = new TFile( "output_hlt.root" );

    if( !theFile ) { return 0; }

    system( "mkdir -p plots" );

    //cout<<"creating canvas"<<endl;

    Ca0->cd();

    //turnon curve
    //Interesting bits

    //string interesting_bits[]={"418","6","416","8","5","412","233","7","234","10","187","9","22","198","430","404","188","199","21","190","197","208","201","207","195","196","206","193","424","192","204","194","203","205","209","189","200","191"};

    //string in_bits[]={"418","6"}
    //for(int k=0;k<2;k++){

    TH2F *numerator;
    TH2F *denominator;

    cout << "going for 2D 2/1" << endl;

    numerator  = ( TH2F * )( theFile->Get( "wide_pt1pt2_2" ) );
    denominator = ( TH2F * )( theFile->Get( "wide_pt1pt2_1" ) );
    binomialEfficiency2D( numerator, denominator, true, true );
    Ca0->SaveAs( ( "plots/eff21_wide_pt1pt2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2_2" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2_1" ) );
    binomialEfficiency2D( numerator, denominator, false, true );
    Ca0->SaveAs( ( "plots/eff21_pt1pt2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2Norm_2" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2Norm_1" ) );
    denominator->GetXaxis()->SetTitle( "p_{T} Lead / M_{#gamma #gamma}" );
    denominator->GetYaxis()->SetTitle( "p_{T} Trail / M_{#gamma #gamma}" );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff21_pt1pt2Norm.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2Zoom_2" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2Zoom_1" ) );
    denominator->GetXaxis()->SetTitle( "p_{T} Lead / M_{#gamma #gamma}" );
    denominator->GetYaxis()->SetTitle( "p_{T} Trail / M_{#gamma #gamma}" );
    binomialEfficiency2D( numerator, denominator, true, false );
    Ca0->SaveAs( ( "plots/eff21_pt1pt2Zoom.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "eta1eta2_2" ) );
    denominator = ( TH2F * )( theFile->Get( "eta1eta2_1" ) );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff21_eta1eta2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "phi1phi2_2" ) );
    denominator = ( TH2F * )( theFile->Get( "phi1phi2_1" ) );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff21_phi1phi2.png" ) );
    Ca0->Clear();

    cout << "going for 2D 4/3" << endl;

    numerator  = ( TH2F * )( theFile->Get( "wide_pt1pt2_4" ) );
    denominator = ( TH2F * )( theFile->Get( "wide_pt1pt2_3" ) );
    binomialEfficiency2D( numerator, denominator, true, true );
    Ca0->SaveAs( ( "plots/eff43_wide_pt1pt2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2_4" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2_3" ) );
    binomialEfficiency2D( numerator, denominator, false, true );
    Ca0->SaveAs( ( "plots/eff43_pt1pt2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2Norm_4" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2Norm_3" ) );
    denominator->GetXaxis()->SetTitle( "p_{T} Lead / M_{#gamma #gamma}" );
    denominator->GetYaxis()->SetTitle( "p_{T} Trail / M_{#gamma #gamma}" );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff43_pt1pt2Norm.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2Zoom_4" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2Zoom_3" ) );
    denominator->GetXaxis()->SetTitle( "p_{T} Lead / M_{#gamma #gamma}" );
    denominator->GetYaxis()->SetTitle( "p_{T} Trail / M_{#gamma #gamma}" );
    binomialEfficiency2D( numerator, denominator, true, false );
    Ca0->SaveAs( ( "plots/eff43_pt1pt2Zoom.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "eta1eta2_4" ) );
    denominator = ( TH2F * )( theFile->Get( "eta1eta2_3" ) );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff43_eta1eta2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "phi1phi2_4" ) );
    denominator = ( TH2F * )( theFile->Get( "phi1phi2_3" ) );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff43_phi1phi2.png" ) );
    Ca0->Clear();

    cout << "going for 2D 6/5" << endl;

    numerator  = ( TH2F * )( theFile->Get( "wide_pt1pt2_6" ) );
    denominator = ( TH2F * )( theFile->Get( "wide_pt1pt2_5" ) );
    binomialEfficiency2D( numerator, denominator, true, true );
    Ca0->SaveAs( ( "plots/eff65_wide_pt1pt2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2_6" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2_5" ) );
    binomialEfficiency2D( numerator, denominator, false, true );
    Ca0->SaveAs( ( "plots/eff65_pt1pt2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2Norm_6" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2Norm_5" ) );
    denominator->GetXaxis()->SetTitle( "p_{T} Lead / M_{#gamma #gamma}" );
    denominator->GetYaxis()->SetTitle( "p_{T} Trail / M_{#gamma #gamma}" );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff65_pt1pt2Norm.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2Zoom_6" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2Zoom_5" ) );
    denominator->GetXaxis()->SetTitle( "p_{T} Lead / M_{#gamma #gamma}" );
    denominator->GetYaxis()->SetTitle( "p_{T} Trail / M_{#gamma #gamma}" );
    binomialEfficiency2D( numerator, denominator, true, false );
    Ca0->SaveAs( ( "plots/eff65_pt1pt2Zoom.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "eta1eta2_6" ) );
    denominator = ( TH2F * )( theFile->Get( "eta1eta2_5" ) );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff65_eta1eta2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "phi1phi2_6" ) );
    denominator = ( TH2F * )( theFile->Get( "phi1phi2_5" ) );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff65_phi1phi2.png" ) );
    Ca0->Clear();

    cout << "going for 2D 8/7" << endl;

    numerator  = ( TH2F * )( theFile->Get( "wide_pt1pt2_8" ) );
    denominator = ( TH2F * )( theFile->Get( "wide_pt1pt2_7" ) );
    binomialEfficiency2D( numerator, denominator, true, true );
    Ca0->SaveAs( ( "plots/eff87_wide_pt1pt2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2_8" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2_7" ) );
    binomialEfficiency2D( numerator, denominator, false, true );
    Ca0->SaveAs( ( "plots/eff87_pt1pt2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2Norm_8" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2Norm_7" ) );
    denominator->GetXaxis()->SetTitle( "p_{T} Lead / M_{#gamma #gamma}" );
    denominator->GetYaxis()->SetTitle( "p_{T} Trail / M_{#gamma #gamma}" );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff87_pt1pt2Norm.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2Zoom_8" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2Zoom_7" ) );
    denominator->GetXaxis()->SetTitle( "p_{T} Lead / M_{#gamma #gamma}" );
    denominator->GetYaxis()->SetTitle( "p_{T} Trail / M_{#gamma #gamma}" );
    binomialEfficiency2D( numerator, denominator, true, false );
    Ca0->SaveAs( ( "plots/eff87_pt1pt2Zoom.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "eta1eta2_8" ) );
    denominator = ( TH2F * )( theFile->Get( "eta1eta2_7" ) );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff87_eta1eta2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "phi1phi2_8" ) );
    denominator = ( TH2F * )( theFile->Get( "phi1phi2_7" ) );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff87_phi1phi2.png" ) );
    Ca0->Clear();

    cout << "going for 2D 1/0" << endl;

    numerator  = ( TH2F * )( theFile->Get( "wide_pt1pt2_1" ) );
    denominator = ( TH2F * )( theFile->Get( "wide_pt1pt2_0" ) );
    binomialEfficiency2D( numerator, denominator, true, true );
    Ca0->SaveAs( ( "plots/eff10_wide_pt1pt2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2_1" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2_0" ) );
    binomialEfficiency2D( numerator, denominator, false, true );
    Ca0->SaveAs( ( "plots/eff10_pt1pt2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2Norm_1" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2Norm_0" ) );
    denominator->GetXaxis()->SetTitle( "p_{T} Lead / M_{#gamma #gamma}" );
    denominator->GetYaxis()->SetTitle( "p_{T} Trail / M_{#gamma #gamma}" );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff10_pt1pt2Norm.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "pt1pt2Zoom_1" ) );
    denominator = ( TH2F * )( theFile->Get( "pt1pt2Zoom_0" ) );
    denominator->GetXaxis()->SetTitle( "p_{T} Lead / M_{#gamma #gamma}" );
    denominator->GetYaxis()->SetTitle( "p_{T} Trail / M_{#gamma #gamma}" );
    binomialEfficiency2D( numerator, denominator, true, false );
    Ca0->SaveAs( ( "plots/eff10_pt1pt2Zoom.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "eta1eta2_1" ) );
    denominator = ( TH2F * )( theFile->Get( "eta1eta2_0" ) );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff10_eta1eta2.png" ) );
    Ca0->Clear();

    numerator  = ( TH2F * )( theFile->Get( "phi1phi2_1" ) );
    denominator = ( TH2F * )( theFile->Get( "phi1phi2_0" ) );
    binomialEfficiency2D( numerator, denominator, false, false );
    Ca0->SaveAs( ( "plots/eff10_phi1phi2.png" ) );
    Ca0->Clear();

    //Binomial Efficiency 1D

    TH1F *numerator1D;
    TH1F *denominator1D;

    cout << "going for 1D 2/1" << endl;

    numerator1D = ( TH1F * )( theFile->Get( "higgsEta_2" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsEta_1" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "eta" );
    Ca0->SaveAs( ( "plots/eff21_higgsEta.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsPhi_2" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsPhi_1" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "phi" );
    Ca0->SaveAs( ( "plots/eff21_higgsPhi.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsP_2" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsP_1" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff21_higgsP.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "diphoton_LeadR9_2" ) );
    denominator1D = ( TH1F * )( theFile->Get( "diphoton_LeadR9_1" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff21_diphoton_LeadR9.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "diphoton_SubLeadR9_2" ) );
    denominator1D = ( TH1F * )( theFile->Get( "diphoton_SubLeadR9_1" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff21_diphoton_SubLeadR9.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsPt_2" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsPt_1" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff21_higgsPt.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "massDiphoton_2" ) );
    denominator1D = ( TH1F * )( theFile->Get( "massDiphoton_1" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "m" );
    Ca0->SaveAs( ( "plots/eff21_massDiphoton.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "massHiggs_2" ) );
    denominator1D = ( TH1F * )( theFile->Get( "massHiggs_1" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "m" );
    Ca0->SaveAs( ( "plots/eff21_massHiggs.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptLead_2" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptLead_1" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff21_ptLead.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptTrail_2" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptTrail_1" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff21_ptTrail.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptLeadNorm_2" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptLeadNorm_1" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "pn" );
    Ca0->SaveAs( ( "plots/eff21_ptLeadNorm.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptTrailNorm_2" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptTrailNorm_1" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "pn" );
    Ca0->SaveAs( ( "plots/eff21_ptTrailNorm.png" ) );
    Ca0->Clear();

    cout << "going for 1D 4/3" << endl;

    numerator1D = ( TH1F * )( theFile->Get( "higgsEta_4" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsEta_3" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "eta" );
    Ca0->SaveAs( ( "plots/eff43_higgsEta.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsPhi_4" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsPhi_3" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "phi" );
    Ca0->SaveAs( ( "plots/eff43_higgsPhi.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsP_4" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsP_3" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff43_higgsP.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "diphoton_LeadR9_4" ) );
    denominator1D = ( TH1F * )( theFile->Get( "diphoton_LeadR9_3" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff43_diphoton_LeadR9.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "diphoton_SubLeadR9_4" ) );
    denominator1D = ( TH1F * )( theFile->Get( "diphoton_SubLeadR9_3" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff43_diphoton_SubLeadR9.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsPt_4" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsPt_3" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff43_higgsPt.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "massDiphoton_4" ) );
    denominator1D = ( TH1F * )( theFile->Get( "massDiphoton_3" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "m" );
    Ca0->SaveAs( ( "plots/eff43_massDiphoton.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "massHiggs_4" ) );
    denominator1D = ( TH1F * )( theFile->Get( "massHiggs_3" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "m" );
    Ca0->SaveAs( ( "plots/eff43_massHiggs.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptLead_4" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptLead_3" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff43_ptLead.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptTrail_4" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptTrail_3" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff43_ptTrail.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptLeadNorm_4" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptLeadNorm_3" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "pn" );
    Ca0->SaveAs( ( "plots/eff43_ptLeadNorm.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptTrailNorm_4" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptTrailNorm_3" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "pn" );
    Ca0->SaveAs( ( "plots/eff43_ptTrailNorm.png" ) );


    cout << "going for 1D 6/5" << endl;

    numerator1D = ( TH1F * )( theFile->Get( "higgsEta_6" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsEta_5" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "eta" );
    Ca0->SaveAs( ( "plots/eff65_higgsEta.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsPhi_6" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsPhi_5" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "phi" );
    Ca0->SaveAs( ( "plots/eff65_higgsPhi.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsP_6" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsP_5" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff65_higgsP.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "diphoton_LeadR9_6" ) );
    denominator1D = ( TH1F * )( theFile->Get( "diphoton_LeadR9_5" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff65_diphoton_LeadR9.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "diphoton_SubLeadR9_6" ) );
    denominator1D = ( TH1F * )( theFile->Get( "diphoton_SubLeadR9_5" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff65_diphoton_SubLeadR9.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsPt_6" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsPt_5" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff65_higgsPt.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "massDiphoton_6" ) );
    denominator1D = ( TH1F * )( theFile->Get( "massDiphoton_5" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "m" );
    Ca0->SaveAs( ( "plots/eff65_massDiphoton.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "massHiggs_6" ) );
    denominator1D = ( TH1F * )( theFile->Get( "massHiggs_5" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "m" );
    Ca0->SaveAs( ( "plots/eff65_massHiggs.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptLead_6" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptLead_5" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff65_ptLead.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptTrail_6" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptTrail_5" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff65_ptTrail.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptLeadNorm_6" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptLeadNorm_5" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "pn" );
    Ca0->SaveAs( ( "plots/eff65_ptLeadNorm.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptTrailNorm_6" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptTrailNorm_5" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "pn" );
    Ca0->SaveAs( ( "plots/eff65_ptTrailNorm.png" ) );

    cout << "going for 1D 8/7" << endl;

    numerator1D = ( TH1F * )( theFile->Get( "higgsEta_8" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsEta_7" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "eta" );
    Ca0->SaveAs( ( "plots/eff87_higgsEta.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsPhi_8" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsPhi_7" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "phi" );
    Ca0->SaveAs( ( "plots/eff87_higgsPhi.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsP_8" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsP_7" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff87_higgsP.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "diphoton_LeadR9_8" ) );
    denominator1D = ( TH1F * )( theFile->Get( "diphoton_LeadR9_7" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff87_diphoton_LeadR9.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "diphoton_SubLeadR9_8" ) );
    denominator1D = ( TH1F * )( theFile->Get( "diphoton_SubLeadR9_7" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff87_diphoton_SubLeadR9.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsPt_8" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsPt_7" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff87_higgsPt.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "massDiphoton_8" ) );
    denominator1D = ( TH1F * )( theFile->Get( "massDiphoton_7" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "m" );
    Ca0->SaveAs( ( "plots/eff87_massDiphoton.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "massHiggs_8" ) );
    denominator1D = ( TH1F * )( theFile->Get( "massHiggs_7" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "m" );
    Ca0->SaveAs( ( "plots/eff87_massHiggs.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptLead_8" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptLead_7" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff87_ptLead.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptTrail_8" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptTrail_7" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff87_ptTrail.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptLeadNorm_8" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptLeadNorm_7" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "pn" );
    Ca0->SaveAs( ( "plots/eff87_ptLeadNorm.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptTrailNorm_8" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptTrailNorm_7" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "pn" );
    Ca0->SaveAs( ( "plots/eff87_ptTrailNorm.png" ) );


    cout << "going for 1D 1/0" << endl;

    numerator1D = ( TH1F * )( theFile->Get( "higgsEta_1" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsEta_0" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "eta" );
    Ca0->SaveAs( ( "plots/eff10_higgsEta.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsPhi_1" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsPhi_0" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "phi" );
    Ca0->SaveAs( ( "plots/eff10_higgsPhi.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsP_1" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsP_0" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff10_higgsP.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "diphoton_LeadR9_1" ) );
    denominator1D = ( TH1F * )( theFile->Get( "diphoton_LeadR9_0" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff10_diphoton_LeadR9.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "diphoton_SubLeadR9_1" ) );
    denominator1D = ( TH1F * )( theFile->Get( "diphoton_SubLeadR9_0" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff10_diphoton_SubLeadR9.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "higgsPt_1" ) );
    denominator1D = ( TH1F * )( theFile->Get( "higgsPt_0" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff10_higgsPt.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "massDiphoton_1" ) );
    denominator1D = ( TH1F * )( theFile->Get( "massDiphoton_0" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "m" );
    Ca0->SaveAs( ( "plots/eff10_massDiphoton.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "massHiggs_1" ) );
    denominator1D = ( TH1F * )( theFile->Get( "massHiggs_0" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "m" );
    Ca0->SaveAs( ( "plots/eff10_massHiggs.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptLead_1" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptLead_0" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff10_ptLead.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptTrail_1" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptTrail_0" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "p" );
    Ca0->SaveAs( ( "plots/eff10_ptTrail.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptLeadNorm_1" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptLeadNorm_0" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "pn" );
    Ca0->SaveAs( ( "plots/eff10_ptLeadNorm.png" ) );
    Ca0->Clear();

    numerator1D = ( TH1F * )( theFile->Get( "ptTrailNorm_1" ) );
    denominator1D = ( TH1F * )( theFile->Get( "ptTrailNorm_0" ) );
    binomialEfficiency1D( numerator1D, denominator1D, "pn" );
    Ca0->SaveAs( ( "plots/eff10_ptTrailNorm.png" ) );

    cout << "going for Draw2D" << endl;
    gStyle->SetPaintTextFormat( "1.0f" );
    TH1F *cutflow = ( TH1F * )( theFile->Get( "cutflow" ) );
    cutflow->Draw( "" );
    cutflow->Draw( "htextsame" );
    cutflow->SetFillColor( kYellow );
    cutflow->SetXTitle( "cut[]" );
    Ca0->SaveAs( "plots/cutflow.png" );
    Ca0->Clear();

    gStyle->SetPaintTextFormat( "1.3f" );
    Draw2D( "wide_pt1pt2", "p_{T} Lead (GeV)", "p_{T} Trail (GeV)", theFile, true );
    Draw2D( "phi1phi2", "#phi_{Lead}", "#phi_{Trail}", theFile, false );
    Draw2D( "eta1eta2", "#eta_{Lead}", "#eta_{Trail}", theFile, false );
    Draw2D( "pt1pt2", "p_{T} Lead (GeV)", "p_{T} Trail (GeV)", theFile, false );
    Draw2D( "pt1pt2Norm", "p_{T} Lead / M_{#gamma #gamma}", "p_{T} Trail / M_{#gamma #gamma}", theFile, false );
    Draw2D( "pt1pt2Zoom", "p_{T} Lead / M_{#gamma #gamma}", "p_{T} Trail / M_{#gamma #gamma}", theFile, false );


    cout << "going for Draw1D" << endl;

    int cut1 = 0;
    int cut2 = 1;
    Draw1D( "higgsEta", "#eta_{H}", theFile, cut1, cut2 );
    Draw1D( "higgsPhi", "#phi_{H}", theFile, cut1, cut2 );
    Draw1D( "higgsPt", "p (GeV)", theFile, cut1, cut2 );
    Draw1D( "higgsP", "p (GeV)", theFile, cut1, cut2 );
    Draw1D( "diphoton_LeadR9", "diphoton_LeadR9", theFile, cut1, cut2 );
    Draw1D( "diphoton_SubLeadR9", "diphoton_SubLeadR9", theFile, cut1, cut2 );

    for( cut1 = 1; cut1 <= 7; cut1 = cut1 + 2 ) {
        cut2 = cut1 + 1;
        Draw1D( "higgsEta", "#eta_{H}", theFile, cut1, cut2 );
        Draw1D( "higgsPhi", "#phi_{H}", theFile, cut1, cut2 );
        Draw1D( "higgsPt", "p (GeV)", theFile, cut1, cut2 );
        Draw1D( "higgsP", "p (GeV)", theFile, cut1, cut2 );
        Draw1D( "diphoton_LeadR9", "diphoton_LeadR9", theFile, cut1, cut2 );
        Draw1D( "diphoton_SubLeadR9", "diphoton_SubLeadR9", theFile, cut1, cut2 );
    }

    //Ca0->SetLogy();

    cut1 = 0;
    cut2 = 1;
    Draw1D( "ptLead", "p_{T} Lead (GeV)", theFile, cut1, cut2 );
    Draw1D( "ptTrail", "p_{T} Trail (GeV)", theFile, cut1, cut2 );
    Draw1D( "ptLeadNorm", "p_{T} Lead / M_{#gamma #gamma} ", theFile, cut1, cut2 );
    Draw1D( "ptTrailNorm", "p_{T} Trail / M_{#gamma #gamma}", theFile, cut1, cut2 );
    Draw1D( "massDiphoton", "mass_{#gamma #gamma} (GeV)", theFile, cut1, cut2 );
    Draw1D( "massHiggs", "mass_{H} (GeV)", theFile, cut1, cut2 );


    for( cut1 = 1; cut1 <= 7; cut1 = cut1 + 2 ) {
        cut2 = cut1 + 1;
        Draw1D( "ptLead", "p_{T} Lead (GeV)", theFile, cut1, cut2 );
        Draw1D( "ptTrail", "p_{T} Trail (GeV)", theFile, cut1, cut2 );
        Draw1D( "ptLeadNorm", "p_{T} Lead / M_{#gamma #gamma} ", theFile, cut1, cut2 );
        Draw1D( "ptTrailNorm", "p_{T} Trail / M_{#gamma #gamma}", theFile, cut1, cut2 );
        Draw1D( "massDiphoton", "mass_{#gamma #gamma} (GeV)", theFile, cut1, cut2 );
        Draw1D( "massHiggs", "mass_{H} (GeV)", theFile, cut1, cut2 );
    }

    exit( 0 );
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

