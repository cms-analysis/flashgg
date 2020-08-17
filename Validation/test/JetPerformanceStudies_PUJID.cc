/** JetPerformanceStudies.cc ---
 *
 * Copyright (C) 2014 yhaddad
 *
 * Author: yhaddad <y.haddad@imperial.ac.uk>
 *
 */

#include <map>
#include <iostream>
#include <TTree.h>
#include <TH1F.h>
#include <TFile.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TCanvas.h>
#include "./roomacro.C"
#include <TMath.h>
#include <TCut.h>
#include <TAxis.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>

TGraph *GetRocCurve( TGraph *gx, TGraph *gy, TString name )
{
    TGraph *roc = new TGraph();
    roc->SetName( name.Data() );
    double *y = gx->GetY();
    double *x = gy->GetY();

    for( int i = 0; i < gx->GetN(); i++ ) {
        roc->SetPoint( i, 1 - x[i], y[i] );
    }
    return roc;
}




void JetPerformanceStudies_PUJID( int catbin = 1 )
{


    std::map<TString, TTree *>     JetTree;
    std::map<TString, TTree *>     GenJetTree;

    std::map<TString, TH1F *>      h_betaStar_PUJ;
    std::map<TString, TH1F *>      h_betaStar_JET;

    std::map<TString, TH1F *>      h_RMS_PUJ;
    std::map<TString, TH1F *>      h_RMS_JET;

    std::map<TString, TCanvas *> Canv_PUJID;
    //std::map<TString,TCanvas*> Canv_BetaStar;

    TFile *file = TFile::Open( "/afs/cern.ch/work/y/yhaddad/jetValidationTrees_VBF_HToGG_PU20bx25.root" );
    JetTree["PF"]      = ( TTree * )file->Get( "flashggJetValidationTreeMaker/jetTree_PF" );
    JetTree["PFCHS0"]  = ( TTree * )file->Get( "flashggJetValidationTreeMakerPFCHS0/jetTree_PFCHS0" );
    JetTree["PFCHSLeg"] = ( TTree * )file->Get( "flashggJetValidationTreeMakerPFCHSLeg/jetTree_PFCHSLeg" );
    //====> cuts

    TCut cat;
    TString catname;
    if( catbin == 1 ) {
        cat = "abs(eta) < 2.5";
        catname = "(|#eta| < 2.5)";
    } else if( catbin == 2 ) {
        cat = "abs(eta) > 2.5 && abs(eta) < 3";
        catname = "(2.5 <|#eta| < 3.0)";
    } else if( catbin == 3 ) {
        cat = "abs(eta) > 3";
        catname = "(|#eta| > 3.0)";
    }

    TCut cut_photon_rej     = "!(photonMatch==1 && GenPhotonPt/bestPt > 0.5 && photondRmin < 0.2)";
    TCut cut_gen_photon_rej = "!(photonMatch==1 && GenPhotonPt/pt > 0.5     && photondRmin < 0.2)";
    TCut cut_pt             = "bestPt>20.0";
    TCut cut_genjetmatch    = "genJetMatch==1";

    TCut cut_PUJ = cut_pt && cut_photon_rej && cat && !cut_genjetmatch;
    TCut cut_JET = cut_pt && cut_photon_rej && cat && cut_genjetmatch;
    // ===> Only for def
    std::cout << ":: PUJ == " << cut_PUJ.GetTitle() << std::endl;
    std::cout << ":: JET == " << cut_JET.GetTitle() << std::endl;

    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        h_betaStar_PUJ[it->first]    = new TH1F( Form( "h_betaStar_PUJ_%s", it->first.Data() ),
                Form( "%s %s;#beta^{*};n_{jets}", it->first.Data(), catname.Data() ),
                50, 0, 1 );

        h_betaStar_JET[it->first]    = new TH1F( Form( "h_betaStar_JET_%s", it->first.Data() ),
                Form( "%s %s;#beta^{*};n_{jets}", it->first.Data(), catname.Data() ),
                50, 0, 1 );


        h_RMS_PUJ[it->first]    = new TH1F( Form( "h_RMS_PUJ_%s", it->first.Data() ),
                                            Form( "%s %s;#LT#Delta R^{2}#GT;n_{jets}", it->first.Data(), catname.Data() ),
                                            50, 0, 0.1 );

        h_RMS_JET[it->first]    = new TH1F( Form( "h_RMS_JET_%s", it->first.Data() ),
                                            Form( "%s %s;#LT#Delta R^{2}#GT;n_{jets}", it->first.Data(), catname.Data() ),
                                            50, 0, 0.1 );




        std::cout << " -----------------------------------------------------------------" << std::endl;
        std::cout << "beta PUJ::" <<  Form( "PUJetID_betaStar>>h_betaStar_PUJ_%s", it->first.Data() ) << std::endl;
        std::cout << "beta JET::" <<  Form( "PUJetID_betaStar>>h_betaStar_JET_%s", it->first.Data() ) << std::endl;
        std::cout << "rms  PUJ::" <<  Form( "PUJetID_rms>>h_RMS_PUJ_%s", it->first.Data() ) << std::endl;
        std::cout << "rms  JET::" <<  Form( "PUJetID_rms>>h_RMS_JET_%s", it->first.Data() ) << std::endl;
        std::cout << " -----------------------------------------------------------------" << std::endl;

        JetTree[it->first]->Project( Form( "h_betaStar_PUJ_%s", it->first.Data() ), "PUJetID_betaStar", cut_PUJ, "" );
        JetTree[it->first]->Project( Form( "h_betaStar_JET_%s", it->first.Data() ), "PUJetID_betaStar", cut_JET, "" );

        JetTree[it->first]->Project( Form( "h_RMS_PUJ_%s", it->first.Data() ), "PUJetID_rms", cut_PUJ, "" );
        JetTree[it->first]->Project( Form( "h_RMS_JET_%s", it->first.Data() ), "PUJetID_rms", cut_JET, "" );

        // canvas
        std::cout << it->first
                  << "\t :JET: " <<  h_RMS_JET[it->first]->GetEntries()
                  << "\t :PUJ: " <<  h_RMS_PUJ[it->first]->GetEntries()
                  << std::endl;

        Canv_PUJID[it->first] = new TCanvas( Form( "c_PUJID_%s", it->first.Data() ),
                                             Form( "c_PIJID_%s", it->first.Data() ), 1000, 500 );
        Canv_PUJID[it->first]->Divide( 2, 1 );
        Canv_PUJID[it->first]->cd( 1 );
        gPad->SetLogy();
        h_RMS_JET[it->first]->SetLineColor( kRed );
        h_RMS_JET[it->first]->SetMarkerColor( kRed );
        h_RMS_JET[it->first]->Draw( "" );
        h_RMS_PUJ[it->first]->Draw( "same" );
        Draw2Legend( h_RMS_JET[it->first], h_RMS_PUJ[it->first], "Jets", "PU Jets" );
        Canv_PUJID[it->first]->cd( 2 );
        gPad->SetLogy();
        h_betaStar_JET[it->first]->SetLineColor( kRed );
        h_betaStar_JET[it->first]->SetMarkerColor( kRed );
        h_betaStar_JET[it->first]->DrawNormalized( "" );
        h_betaStar_PUJ[it->first]->DrawNormalized( "same" );
        Draw2Legend( h_betaStar_JET[it->first], h_betaStar_PUJ[it->first], "Jets", "PU Jets" );

        Canv_PUJID[it->first]->SaveAs( Form( "plots/PUJID_var_%s_%i.png", it->first.Data(), catbin ) );
        Canv_PUJID[it->first]->SaveAs( Form( "plots/PUJID_var_%s_%i.pdf", it->first.Data(), catbin ) );
    }

}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

