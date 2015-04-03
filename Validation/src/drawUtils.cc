#include "flashgg/Validation/interface/drawUtils.h"

void compareHistos( TH1F *histo_new, TH1F *histo_old, std::string label_new,
                    std::string label_old, std::string variable,
                    std::string outputDir, TString *drawOption )
{
    //---create outdir if not exists
    std::string mkdir_command = "mkdir -p " + outputDir;
    system( mkdir_command.c_str() );
    gStyle->SetOptStat( "emr" );

    double xNorm = histo_new->Integral() / histo_old->Integral();
    histo_old -> Scale( xNorm );

    histo_new -> GetXaxis() -> SetLabelSize( 0 );
    histo_new -> GetYaxis() -> SetLabelSize( 0.04 );
    histo_new -> GetXaxis() -> SetTitleSize( 0.05 );
    histo_new -> GetYaxis() -> SetTitleSize( 0.05 );
    histo_new -> GetYaxis() -> SetTitleOffset( 1.25 );
    histo_new -> GetXaxis() -> SetTitle( "" );
    histo_old -> GetXaxis() -> SetTitle( "" );
    std::string xTitle = variable;
    char yTitle[50];
    sprintf( yTitle, "events" );
    histo_new -> GetYaxis() -> SetTitle( yTitle );
    histo_old -> GetYaxis() -> SetTitle( yTitle );
    histo_old -> SetLineColor( kRed );
    histo_new -> SetMarkerColor( kBlack );
    histo_new -> SetMarkerSize( 0.5 );
    histo_old -> SetFillColor( kRed );
    histo_old -> SetFillStyle( 3003 );
    histo_old -> SetLineWidth( 2 );
    histo_new -> SetLineWidth( 1 );

    float maximum = -1.;
    if( histo_new -> GetMaximum() > maximum )
    { maximum = histo_new -> GetMaximum(); }
    if( histo_old -> GetMaximum() > maximum )
    { maximum = histo_old -> GetMaximum(); }
    histo_new -> SetMaximum( 1.2 * maximum );
    histo_new -> SetMinimum( 0. );
    TCanvas *c1 = new TCanvas( "c1", "c1" );
    c1->cd();
    TPaveStats *st_new = new TPaveStats();
    TPaveStats *st_old = new TPaveStats();
    TPaveStats *st_ratio = new TPaveStats();
    TPad *cUp = new TPad( "pad_0", "pad_0", 0.00, 0.36, 1.00, 1.00 );
    TPad *cDown = new TPad( "pad_1", "pad_1", 0.00, 0.00, 1.00, 0.36 );
    cUp->SetBottomMargin( 0.01 );
    cDown->SetTopMargin( 0.01 );
    cUp->Draw();
    cDown->Draw();
    cUp->cd();
    histo_new -> Draw( "e" );
    gPad -> Update();
    st_new = ( TPaveStats * )( histo_new->GetListOfFunctions()->FindObject( "stats" ) );
    st_new->SetX1NDC( 0.82 ); //new x start position
    st_new->SetX2NDC( 0.99 ); //new x end position
    st_new->SetY1NDC( 0.70 ); //new y start position
    st_new->SetY2NDC( 0.82 ); //new y end position
    st_new->SetTextColor( kBlack );
    st_new->Draw( "sames" );
    histo_old -> Draw( "sames" );
    gPad -> Update();
    st_old = ( TPaveStats * )( histo_old->GetListOfFunctions()->FindObject( "stats" ) );
    st_old->SetX1NDC( 0.82 ); //new x start position
    st_old->SetX2NDC( 0.99 ); //new x end position
    st_old->SetY1NDC( 0.56 ); //new y start position
    st_old->SetY2NDC( 0.68 ); //new y end position
    st_old->SetTextColor( kRed );
    st_old->Draw( "sames" );
    TLegend *legend = new TLegend( 0.82, 0.82, 0.99, 0.94 );
    legend -> SetFillColor( kWhite );
    legend -> SetFillStyle( 1000 );
    legend -> SetLineWidth( 0 );
    legend -> SetLineColor( kWhite );
    legend -> SetTextFont( 42 );
    legend -> SetTextSize( 0.04 );
    legend -> AddEntry( histo_new, label_new.c_str(), "P" );
    legend -> AddEntry( histo_old, label_old.c_str(), "F" );
    legend -> Draw( "same" );
    TLatex *latex = new TLatex( 0.026, 0.97, xTitle.c_str() );
    latex -> SetNDC();
    latex -> SetTextFont( kBlack );
    latex -> SetTextSize( 0.04 );
    latex -> Draw( "same" );

    //---ratio plot
    cDown->cd();
    TH1F *histo_ratio = ( TH1F * )histo_new->Clone( "histo_ratio" );
    histo_ratio->Sumw2();
    histo_ratio->Divide( histo_old );
    std::string histo_ratio_label = label_new + "/" + label_old;
    histo_ratio -> GetXaxis() -> SetTitle( xTitle.c_str() );
    histo_ratio -> GetYaxis() -> SetTitle( histo_ratio_label.c_str() );
    histo_ratio -> SetMaximum( 2 );
    histo_ratio -> SetMinimum( 0 );
    histo_ratio -> SetMarkerColor( kBlack );
    histo_ratio -> SetMarkerSize( 0.5 );
    histo_ratio -> GetXaxis() -> SetLabelSize( 0.07 );
    histo_ratio -> GetYaxis() -> SetLabelSize( 0.07 );
    histo_ratio -> GetXaxis() -> SetTitleSize( 0.07 );
    histo_ratio -> GetYaxis() -> SetTitleSize( 0.07 );
    histo_ratio -> GetYaxis() -> SetTitleOffset( 0.7 );
    histo_ratio -> Draw( "e" );
    TF1 *f_const = new TF1( "f_1", "[0]", histo_ratio->GetBinCenter( 1 ) - histo_ratio->GetBinWidth( 1 ) / 2,
                            histo_ratio->GetBinCenter( histo_ratio->GetNbinsX() ) +
                            histo_ratio->GetBinWidth( histo_ratio->GetNbinsX() ) / 2 );
    f_const -> FixParameter( 0, 1 );
    f_const -> SetLineColor( kRed );
    f_const -> SetLineWidth( 2 );
    f_const -> Draw( "same" );
    gPad -> Update();
    st_ratio = ( TPaveStats * )( histo_ratio->GetListOfFunctions()->FindObject( "stats" ) );
    st_ratio->SetX1NDC( 0. ); //new x start position
    st_ratio->SetX2NDC( 0. ); //new x end position
    st_ratio->SetY1NDC( 0. ); //new y start position
    st_ratio->SetY2NDC( 0. ); //new y end position
    st_ratio->SetTextColor( kBlack );
    st_ratio->Draw( "sames" );


    cUp->cd();
    // double KSprob = histo_new -> KolmogorovTest(histo_old,"");
    // char KSbuffer[50];
    // sprintf(KSbuffer,"prob_{K.S.} = %1.2e",KSprob);
    // latex = new TLatex(0.4, 0.97, KSbuffer);
    // latex -> SetNDC();
    // latex -> SetTextFont(42);
    // latex -> SetTextSize(0.04);
    // latex -> Draw("same");
    double myChi2 = myChiSquareTest( histo_ratio );
    char myChi2buffer[50];
    sprintf( myChi2buffer, "#chi^{2} = %1.2e", myChi2 );
    latex = new TLatex( 0.62, 0.97, myChi2buffer );
    latex -> SetNDC();
    latex -> SetTextFont( 42 );
    latex -> SetTextSize( 0.04 );
    latex -> Draw( "same" );

    if( drawOption->Contains( "png" ) )
    { c1 -> Print( ( outputDir + xTitle + ".png" ).c_str(), "png" ); }
    if( drawOption->Contains( "pdf" ) )
    { c1 -> Print( ( outputDir + xTitle + ".pdf" ).c_str(), "pdf" ); }
    delete c1;
    delete histo_ratio;
}

double myChiSquareTest( TH1F *h1 )
{
    double val = 0.;
    int n_points = 0;
    for( int bin = 1; bin <= h1->GetNbinsX(); ++bin ) {
        if( h1->GetBinContent( bin ) <= 0. ) { continue; }
        val += ( fabs( h1->GetBinContent( bin ) - 1 ) ) / h1->GetBinError( bin );
        n_points++;
    }
    if( n_points++ != 0 ) { return val / n_points++; }
    else { return 0; }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

