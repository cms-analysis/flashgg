/*  rootlogon.C
 *  Autor: Haddad Yacine
 *  Mail : haddad.yacine@gmx.fr
 */

#include "TStyle.h"
#include <iomanip.h>
#include <time.h>

void setildStyle()
{
    TStyle *ildStyle = new  TStyle( "ildStyle", "Yacine Style" );

    //gInterpreter->ProcessLine(".! ps | grep root");

    // General
    ildStyle->SetFillColor( 10 );
    ildStyle->SetTitleFillColor( 10 );
    //ildStyle->SetTextFont(18); //@
    ildStyle->SetLineWidth( 2 ); //@

    ildStyle->SetPaperSize( 20, 24 );

    // For Canvas
    //ildStyle->SetCanvasPreferGL(true); // as a test
    ildStyle->SetCanvasBorderMode( 0 );
    ildStyle->SetCanvasColor( 0 );   // chaned fron kWhite to -1
    ildStyle->SetCanvasDefH( 600 );   //Height of canvas
    ildStyle->SetCanvasDefW( 600 );   //Width of canvas
    ildStyle->SetCanvasDefX( 900 );     //POsition on screen
    ildStyle->SetCanvasDefY( 20 );

    // For Pad
    ildStyle->SetPadBorderMode( 0 );
    ildStyle->SetPadColor( 0 );      // chaned fron kWhite to -1
    ildStyle->SetPadGridX( false );
    ildStyle->SetPadGridY( false );
    ildStyle->SetGridColor( kGray );
    ildStyle->SetGridStyle( 3 );
    ildStyle->SetGridWidth( 1 );
    ildStyle->SetPadTickX( 1 );
    ildStyle->SetPadTickY( 1 );

    // for frame
    ildStyle->SetFrameBorderMode( 0 );
    ildStyle->SetFrameBorderSize( 1 );
    ildStyle->SetFrameFillColor( 0 ); // chaned fron kWhite to -1
    ildStyle->SetFrameFillStyle( 0 );
    ildStyle->SetFrameLineColor( 1 );
    ildStyle->SetFrameLineStyle( 1 );
    ildStyle->SetFrameLineWidth( 1 ); // default : 2

    // For Hito
    ildStyle->SetHistFillColor( 0 );  // changed from kWhite to -1
    ildStyle->SetHistLineColor( kBlue + 3 );
    ildStyle->SetHistLineStyle( 0 ); //@
    ildStyle->SetHistLineWidth( 2 ); //@
    ildStyle->SetEndErrorSize( 0 );
    ildStyle->SetErrorX( 0. );
    ildStyle->SetMarkerColor( kBlue + 3 );
    ildStyle->SetMarkerSize( 0.7 ); //@
    ildStyle->SetMarkerStyle( 20 ); //@

    // for function
    ildStyle->SetFuncColor( kOrange - 3 );
    ildStyle->SetFuncStyle( 1 );
    ildStyle->SetFuncWidth( 2 );
    ildStyle->SetOptFit( 01100 );
    ildStyle->SetFitFormat( "3.4f" ); //gStyle->SetFitFormat("3.1g");


    // for the statistic box
    ildStyle->SetOptStat( 0 );
    ildStyle->SetStatBorderSize( 0 );
    ildStyle->SetStatFont( 42 );
    ildStyle->SetStatFontSize( 0.07 );
    ildStyle->SetStatColor( 0 );
    ildStyle->SetStatStyle( 0 );
    ildStyle->SetStatW( 0.25 );
    ildStyle->SetStatH( 0.125 );
    ildStyle->SetStatX( 0.90 );
    ildStyle->SetStatY( 0.90 );
    ildStyle->SetStatBorderSize( 0 );

    //ildStyle->SetStatX(1.0-ildStyle->GetPadRightMargin()-0.02);
    //ildStyle->SetStatY(1.0-ildStyle->GetPadTopMargin()-0.02);

    // Margins
    ildStyle->SetPadBottomMargin( 0.15 );
    ildStyle->SetPadTopMargin( 0.13 );
    ildStyle->SetPadRightMargin( 0.13 );
    ildStyle->SetPadLeftMargin( 0.15 );

    // Global Title
    ildStyle->SetTitleFont( 42 ); //@
    ildStyle->SetTitleSize( 0.07 );//@
    ildStyle->SetTitleColor( 1 );
    ildStyle->SetTitleTextColor( 1 );
    ildStyle->SetTitleFillColor( 0 );              // Changed -- JEK
    ildStyle->SetTitleFontSize( 0.05 ); //@
    ildStyle->SetTitleBorderSize( 0 );
    ildStyle->SetTitleAlign( 33 );
    ildStyle->SetTitleX( 0.8 );
    ildStyle->SetTitleY( 0.95 );
    // Axis Titles
    ildStyle->SetTitleColor( 1,   "xyz" );
    ildStyle->SetTitleFont( 42,  "xyz" );
    ildStyle->SetTitleSize( 0.07, "xyz" );
    ildStyle->SetTitleOffset( 1.1, "yz" );
    ildStyle->SetTitleOffset( 1.0, "x" );
    //ildStyle->SetTitleXOffset(1.08);
    //ildStyle->SetTitleYOffset(1.2);

    // Axis Labels
    ildStyle->SetLabelColor( kBlack, "xyz" );
    ildStyle->SetLabelFont( 42,    "xyz" );
    ildStyle->SetLabelOffset( 0.015, "xyz" );
    ildStyle->SetLabelSize( 0.05,  "xyz" );

    // Legend
    ildStyle->SetLegendBorderSize( 0 );

    // Axis
    ildStyle->SetAxisColor( 1,    "XYZ" );
    ildStyle->SetTickLength( 0.03, "XYZ" );
    ildStyle->SetNdivisions( 508,  "XYZ" );
    ildStyle->SetStripDecimals( kTRUE );

    // Change for log plots
    ildStyle->SetOptLogx( 0 );
    ildStyle->SetOptLogy( 0 );
    ildStyle->SetOptLogz( 0 );

    // For a Pretty Plalette
    const Int_t NRGBs = 5;
    const Int_t NCont = 200;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable( NRGBs, stops, red, green, blue, NCont );
    ildStyle->SetNumberContours( NCont );

    //====> done
    ildStyle->cd();
    gROOT->ForceStyle();
    gStyle->ls();

}


void rootlogon()
{
    setildStyle();
}

// on the grid
void onGrid( bool gridOn )
{
    ildStyle->SetPadGridX( gridOn );
    ildStyle->SetPadGridY( gridOn );
}

// on the stat option
void onStatOpt( bool onStat )
{
    if( onStat ) { ildStyle->SetOptStat( "emr" ); }
    else       { ildStyle->SetOptStat( 0 ); }
}

void onFitOpt( bool onStat )
{
    if( onStat ) { ildStyle->SetOptStat( "emr" ); }
    else       { ildStyle->SetOptStat( 0 ); }
}

void onTranspacy( bool trans )
{
    ildStyle->SetCanvasColor( -1 );   // chaned fron kWhite to -1
    ildStyle->SetPadColor( -1 );      // chaned fron kWhite to -1
    ildStyle->SetFrameFillColor( -1 );
    ildStyle->SetTitleFillColor( -1 );
    ildStyle->SetHistFillColor( -1 );

}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

