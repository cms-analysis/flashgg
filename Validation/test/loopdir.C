#include "TH1.h"
#include "TKey.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TLine.h"
#include "TFile.h"
#include "TPaveStats.h"
#include "TROOT.h"
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <algorithm>

void loopdir( TFile *f1 )
{

    TIter next( f1->GetListOfKeys() );
    TKey *k; //histos inside the file.
    TCanvas *c = new TCanvas( "c", "canvas", 800, 800 );
    while( ( k = ( TKey * )next() ) ) { //loop over histos
        TClass *cl = gROOT->GetClass( k->GetClassName() );
        std::cout << k->GetName() << std::endl;
        if( !cl->InheritsFrom( "TH1" ) ) { continue; } //make sure it is a histogram!

        TH1 *h1 = ( TH1 * )k->ReadObj(); //get handle on histo, cast as TH1
        h1->SetBit( TH1::kNoTitle ); //don't print title

        std::ostringstream title;
        title << h1->GetName();

        //	std::cout << h1->GetName() << std::endl;
        //	std::cout << (title.str().find("un")) << std::endl;
        //	std::cout << (title.str().size()) << std::endl;
        //	std::cout << ((title.str().find("un"))>title.str().size()) << std::endl;

        if( ( title.str().find( "hOverE" ) ) < title.str().size() ) {
            c->SetLogx();
            std::cout << "[SETTING LOG X|" << std::endl;
        } else {
            c->SetLogx( 0 );
        }

        if( ( title.str().find( "un" ) ) < title.str().size() ) {continue;} //if it contains "un", it is the 53x unmatched electrons. We use this later, when plotting vs the regular 53x electrons. so skip them now and access later.

        //std::cout << (title.str().find("5")) << std::endl;

        // PLOTS 53X vs 70X
        if( ( title.str().find( "5" ) ) < title.str().size() ) { // make sure the histo title contains 5. These are the regular 53x histos.

            c->cd();
            TPad *pad1 = new TPad( "pad1", "pad1", 0, 0.3, 1, 1.0 ); //create pad for distributions
            pad1->SetBottomMargin( 0.05 );
            pad1->SetGridx();         // Vertical grid
            pad1->Draw();             // Draw the upper pad: pad1
            pad1->cd();               // pad1 becomes the current pad
            h1->SetStats( 0 );        // No statistics on upper plot

            //	used to zoom to a more sensible range
            float xlo = 0;
            float xhi = 0;

            //find sensble lower axis range
            for( int n = 0 ; n < h1->GetNbinsX(); n++ ) {
                //std::cout << h1->GetBinContent(n) << std::endl;
                //if(n %1 ==0) std::cout << "test " << n << "	" << h1->GetBinCenter(n) << " " <<  h1->GetBinContent(n) << std::endl;
                if( h1->GetBinContent( n ) < h1->GetEntries() / 10000 ) {continue ;} //basically picks out first bin with more than 100 events.
                xlo = h1->GetBinCenter( n - 1 );
                break;
            }

            // find sensible upper axis range
            for( int n = h1->GetNbinsX();  n > -1 * h1->GetNbinsX()  ;  n-- ) {
                //std::cout << h1->GetBinContent(n) << std::endl;
                if( h1->GetBinContent( n ) < h1->GetEntries() / 10000 ) { continue; } //basically picks out first bin with more than 100 events.
                xhi = h1->GetBinCenter( n + 1 );
                break;
            }

            //	std::cout << xlo << "	" << xhi << std::endl;;
            //	if ((title.str().find("FBrem"))< title.str().size()){
            //	h1->GetXaxis()->SetRangeUser(0.01,xhi);
            //	}
            //	if ((title.str().find("hOverE"))< title.str().size()){
            //	h1->GetXaxis()->SetRangeUser(0.01,xhi);
            //	std::cout << "hOverE" << std::endl;
            //	}else{
            h1->GetXaxis()->SetRangeUser( xlo, xhi );
            //	}

            if( ( title.str().find( "graph" ) ) < title.str().size() ) {
                h1->Draw( "h" );
            } else {
                h1->DrawNormalized( "h" );
            }

            std::string title2 = title.str().replace( title.str().find( "5" ), 1, "7" );
            std::cout << title2 << std::endl;
            TKey *k2 = ( f1->GetKey( title2.c_str() ) ); //grab equivalent 70x histo
            TH1 *h2 = ( TH1 * )k2->ReadObj(); //cast as Th1
            if( ( title.str().find( "graph" ) ) < title.str().size() ) {
                h2->Draw( "h same" ); // plot on same graph
            } else {
                h2->DrawNormalized( "h same" ); // plot on same graph
            }

            // Plot the ratio plot below
            c->cd();          // Go back to the main canvas before defining pad2
            TPad *pad2 = new TPad( "pad2", "pad2", 0, 0.05, 1, 0.3 );
            pad2->SetTopMargin( 0 );
            pad2->SetBottomMargin( 0.2 );
            pad2->SetGridx(); // vertical grid
            pad2->Draw();
            pad2->cd();       // pad2 becomes the current pad

            TH1F *h3 = ( TH1F * )h1->Clone( "h3" ); //make raio plot
            h3->SetLineColor( kBlack );
            h3->SetMarkerSize( 0.1 );
            h3->Sumw2();
            h3->SetStats( 0 );    // No statistics on lower plot
            Double_t factor = ( h2->GetEntries() ) / ( h1->GetEntries() );
            h3->Divide( h1, h2, factor, 1. );
            //h3->Divide(h2);
            double max = h3->GetMaximum();
            double min = h3->GetMinimum();
            h3->SetMarkerStyle( 21 );
            h3->SetMinimum( min ); // Define Y ..
            h3->SetMaximum( max ); // .. range
            h3->Draw( "ep" );     // Draw the ratio plot


            h1->GetYaxis()->SetTitleSize( 20 );
            h1->GetYaxis()->SetTitleFont( 43 );
            h1->GetYaxis()->SetTitleOffset( 1.55 );


            h3->GetYaxis()->SetTitle( "ratio 53x/70x " );
            h3->GetYaxis()->SetNdivisions( 505 );
            h3->GetYaxis()->SetTitleSize( 20 );
            h3->GetYaxis()->SetTitleFont( 43 );
            h3->GetYaxis()->SetTitleOffset( 1.55 );
            h3->GetYaxis()->SetLabelFont( 43 ); // Absolute font size in pixel (precision 3)
            h3->GetYaxis()->SetLabelSize( 15 );

            // X axis ratio plot settings
            h3->GetXaxis()->SetTitle( title.str().replace( title.str().find( "5" ), 1, "" ).c_str() );
            h3->GetXaxis()->SetTitleSize( 20 );
            h3->GetXaxis()->SetTitleFont( 43 );
            h3->GetXaxis()->SetTitleOffset( 4. );
            h3->GetXaxis()->SetLabelFont( 43 ); // Absolute font size in pixel (precision 3)
            h3->GetXaxis()->SetLabelSize( 15 );

            double w = h3->GetBinWidth( 0 );
            int N = h3->GetNbinsX();
            TLine *l = new TLine( 0, 0, 0, 0 );
            l->SetLineColor( kRed );
            //l->DrawLine(h3->GetBinCenter(0)-0.49*w,1,h3->GetBinCenter(N)+0.5*w,1);
            l->DrawLine( xlo, 1, xhi, 1 );


            c->cd();
            pad1->cd();

            if( ( title.str().find( "Pt" ) ) < title.str().size() ) {
                TLegend *leg = new TLegend( 0.7, 0.7, 0.9, 0.9 );
                leg->AddEntry( h1, "PFCHS (legacy vtx)", "l" );
                leg->AddEntry( h2, "PUPPI (legacy vtx)", "l" );
                leg->AddEntry( h3, "ratio", "lep" );
                leg->Draw();
            } else {
                TLegend *leg = new TLegend( 0.1, 0.7, 0.3, 0.9 );
                leg->AddEntry( h1, "PFCHS (legacy vtx)", "l" );
                leg->AddEntry( h2, "PUPPI (legacy vtx)", "l" );
                leg->AddEntry( h3, "ratio", "lep" );
                leg->Draw();
            }

            //	std::cout <<  << "	" <<  << std:: endl;
            // save pdf
            std::ostringstream saveas;
            saveas << "Plots/" << h1->GetName() << ".pdf" ;
            c->Print( saveas.str().c_str() );
            std::cout << "print" << std::endl;
        }

        /*	// 53X matched vs 53X unmatched //basically same as above
        	if ((title.str().find("5"))< title.str().size()){
        		c->cd();
        		TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
        		pad1->SetBottomMargin(0.05);
        		pad1->SetGridx();         // Vertical grid
        		pad1->Draw();             // Draw the upper pad: pad1
        		pad1->cd();               // pad1 becomes the current pad
        		h1->SetStats(0);          // No statistics on upper plot

        	//	float w1 = h1->GetBinWidth(0);
        		float xlo =0;
        		float xhi =0;
        		for (int n = 0 ; n < h1->GetNbinsX(); n++)
        		{
        			//std::cout << h1->GetBinContent(n) << std::endl;
        			//if(n %1 ==0) std::cout << "test " << n << "	" << h1->GetBinCenter(n) << " " <<  h1->GetBinContent(n) << std::endl;
        			if (h1->GetBinContent(n) < h1->GetEntries()/10000 ) {continue ;}
        			xlo = h1->GetBinCenter(n-1);
        			break;
        		}

        		for (int n = h1->GetNbinsX();  n> -1*h1->GetNbinsX()  ;  n--)
        		{
        			//std::cout << h1->GetBinContent(n) << std::endl;
        			if (h1->GetBinContent(n) < h1->GetEntries()/10000 ) continue;
        			xhi = h1->GetBinCenter(n+1);
        			break;
        		}

        //	std::cout << xlo << "	" << xhi << std::endl;;
        		if ((title.str().find("FBrem"))< title.str().size()){
        		h1->GetXaxis()->SetRangeUser(0.001,xhi);
        		}
        		if ((title.str().find("hOverE"))< title.str().size()){
        		h1->GetXaxis()->SetRangeUser(0.001,xhi);
        		}

        		h1->GetXaxis()->SetRangeUser(xlo,xhi);
        		h1->DrawNormalized("h");

        		std::string title2 = title.str().replace(title.str().find("5"), 1, "5_un");
        		TKey *k2 =(f1->GetKey(title2.c_str()));
        		TH1 *h2 =(TH1*)k2->ReadObj();
        		h2->DrawNormalized("h same");

        	//	h1->GetYaxis()->SetLabelSize(0.);
        		//		TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
        		//		axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
        		//		axis->SetLabelSize(15);
        		//		axis->Draw();


        		c->cd();          // Go back to the main canvas before defining pad2
        		TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
        		pad2->SetTopMargin(0);
        		pad2->SetBottomMargin(0.2);
        		pad2->SetGridx(); // vertical grid
        		pad2->Draw();
        		pad2->cd();       // pad2 becomes the current pad

        		TH1F *h3 = (TH1F*)h1->Clone("h3");
        		h3->SetLineColor(kBlack);
        		h3->SetMarkerSize(0.1);
        		h3->Sumw2();
        		h3->SetStats(0);      // No statistics on lower plot
        		Double_t factor = (h2->GetEntries())/(h1->GetEntries());
        		h3->Divide(h1,h2, factor, 1.);

        		h3->SetMarkerStyle(21);
        		double max = h3->GetMaximum();
        		double min = h3->GetMinimum();
        		h3->GetYaxis()->SetRangeUser(min,max);  // Define Y ..
        		h3->Draw("ep");       // Draw the ratio plot

        		h1->GetYaxis()->SetTitleSize(20);
        		h1->GetYaxis()->SetTitleFont(43);
        		h1->GetYaxis()->SetTitleOffset(1.55);


        		h3->GetYaxis()->SetTitle("ratio h1/h2 ");
        		h3->GetYaxis()->SetNdivisions(505);
        		h3->GetYaxis()->SetTitleSize(20);
        		h3->GetYaxis()->SetTitleFont(43);
        		h3->GetYaxis()->SetTitleOffset(1.55);
        		h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
        		h3->GetYaxis()->SetLabelSize(15);

        		// X axis ratio plot settings
        		h3->GetXaxis()->SetTitle(title.str().replace(title.str().find("5"), 1, "").c_str());
        		h3->GetXaxis()->SetTitleSize(20);
        		h3->GetXaxis()->SetTitleFont(43);
        		h3->GetXaxis()->SetTitleOffset(4.);
        		h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
        		h3->GetXaxis()->SetLabelSize(15);

        		double w = h3->GetBinWidth(0);
        		int N = h3->GetNbinsX();
        		TLine *l = new TLine(0,0,0,0);
        		l->SetLineColor(kRed);
        		//l->DrawLine(h3->GetBinCenter(0)-0.49*w,1,h3->GetBinCenter(N)+0.5*w,1);
        		l->DrawLine(xlo,1,xhi,1);

        		c->cd();
        		pad1->cd();

        		if ((title.str().find("Pt"))< title.str().size()){
        		TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
        		leg->AddEntry(h1,"53X","l");
        		leg->AddEntry(h2,"53X unmatched","l");
        		leg->AddEntry(h3,"ratio","lep");
        		leg->Draw();
        		}else {
        		TLegend *leg = new TLegend(0.1,0.7,0.3,0.9);
        		leg->AddEntry(h1,"53X","l");
        		leg->AddEntry(h2,"53X unmatched","l");
        		leg->AddEntry(h3,"ratio","lep");
        		leg->Draw();
        		}

        		//	std::cout <<  << "	" <<  << std:: endl;

        		std::ostringstream saveas;
        		saveas << "Plots/" << h1->GetName()<<"_un.pdf" ;
        		c->Print(saveas.str().c_str());
        		std::cout <<"print" << std::endl;
        	}*/
        //PLOT DELTA of 53X and 70X
        if( ( title.str().find( "diff" ) ) < title.str().size() ) {
            c->cd();
            h1->SetStats( 1 );        // No statistics on upper plot

            //std::string title2 = title.str().replace(title.str().find("5"), 1, "_diff");
            //TKey *k2 =(f1->GetKey(title2.c_str()));
            //	TH1 *h2 =(TH1*)k2->ReadObj();
            //	float w1 = h1->GetBinWidth(0);
            float xlo = 0;
            float xhi = 0;
            for( int n = 0 ; n < h1->GetNbinsX(); n++ ) {
                //std::cout << h1->GetBinContent(n) << std::endl;
                //if(n %1 ==0) std::cout << "test " << n << "	" << h1->GetBinCenter(n) << " " <<  h1->GetBinContent(n) << std::endl;
                if( h1->GetBinContent( n ) < h1->GetEntries() / 100 ) {continue ;}
                xlo = h1->GetBinCenter( n - 1 );
                break;
            }

            for( int n = h1->GetNbinsX();  n > -1 * h1->GetNbinsX()  ;  n-- ) {
                //std::cout << h1->GetBinContent(n) << std::endl;
                if( h1->GetBinContent( n ) < h1->GetEntries() / 100 ) { continue; }
                xhi = h1->GetBinCenter( n + 1 );
                break;
            }

            xlo = std::max( std::fabs( xlo ), std::fabs( xhi ) );
            xhi = xlo;
            xlo = -xlo;
            //std::cout << xlo << "	" << xhi << std::endl;;
            h1->GetXaxis()->SetRangeUser( xlo, xhi );
            //	h1->DrawNormalized("h");

            h1->GetXaxis()->SetTitle( title.str().replace( title.str().find( "_diff" ), 5, "" ).insert( 0, "#Delta" ).c_str() );

            h1->GetYaxis()->SetTitleSize( 20 );
            h1->GetYaxis()->SetTitleFont( 43 );
            h1->GetYaxis()->SetTitleOffset( 1.55 );
            h1->Draw( "h" );
            gPad->Update();
            TPaveStats *st = ( TPaveStats * )h1->FindObject( "stats" );
            st->SetX1NDC( 0.1 );
            st->SetX2NDC( 0.3 );
            st->SetY1NDC( 0.7 );
            st->SetY2NDC( 0.9 );

            //	std::cout <<  << "	" <<  << std:: endl;

            std::ostringstream saveas;
            saveas << "Plots/" << h1->GetName() << ".pdf" ;
            c->Print( saveas.str().c_str() );
            std::cout << "print" << std::endl;
            c->Clear();
        }
    }
    //  c.Print("hsimple.ps]");
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

