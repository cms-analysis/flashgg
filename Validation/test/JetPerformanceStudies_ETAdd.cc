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




void JetPerformanceStudies_ETA( int catbin = 1, bool withPUJID = true )
{
    std::map<TString, TCanvas *> Canv;

    std::map<TString, TTree *>     JetTree;
    std::map<TString, TTree *>     GenJetTree;

    std::map<TString, TH1F *>                h_eta_all;
    std::map<TString, TH1F *>                h_eta_sel;
    std::map<TString, TH1F *>                h_effeta_all;
    std::map<TString, TH1F *>                h_effeta_sel;
    std::map<TString, TH2F *>                h2_response_eta;

    std::map<TString, TGraphErrors *>        gr_resolution_eta;
    std::map<TString, TGraphErrors *>        gr_response_eta;
    std::map<TString, TGraphAsymmErrors *>   fake_rate_vs_eta;
    std::map<TString, TGraphAsymmErrors *>   fake_rate_vs_eat;
    std::map<TString, TGraphAsymmErrors *>   eff_vs_eta;
    std::map<TString, TGraphAsymmErrors *>   eff_vs_eat;
    std::map<TString, TGraph *>              roc_curve;

    TFile *file        = TFile::Open( "/afs/cern.ch/work/y/yhaddad/jetValidationTrees_VBF_HToGG_PU20bx25.root" );
    JetTree["PF"]      = ( TTree * )file->Get( "flashggJetValidationTreeMaker/jetTree_PF" );
    JetTree["PFCHS0"]  = ( TTree * )file->Get( "flashggJetValidationTreeMakerPFCHS0/jetTree_PFCHS0" );
    JetTree["PFCHSLeg"] = ( TTree * )file->Get( "flashggJetValidationTreeMakerPFCHSLeg/jetTree_PFCHSLeg" );
    //JetTree["PUPPI0"]  = (TTree*)file->Get("flashggJetValidationTreeMakerPUPPI0/jetTree_PUPPI0");
    //JetTree["PUPPILeg"]= (TTree*)file->Get("flashggJetValidationTreeMakerPUPPILeg/jetTree_PUPPILeg");

    GenJetTree["PF"]      = ( TTree * )file->Get( "flashggJetValidationTreeMaker/genJetTree_PF" );
    GenJetTree["PFCHS0"]  = ( TTree * )file->Get( "flashggJetValidationTreeMakerPFCHS0/genJetTree_PFCHS0" );
    GenJetTree["PFCHSLeg"] = ( TTree * )file->Get( "flashggJetValidationTreeMakerPFCHSLeg/genJetTree_PFCHSLeg" );
    //GenJetTree["PUPPI0"]  = (TTree*)file->Get("flashggJetValidationTreeMakerPUPPI0/jetTree_PUPPI0");
    //GenJetTree["PUPPILeg"]= (TTree*)file->Get("flashggJetValidationTreeMakerPUPPILeg/jetTree_PUPPILeg");
    //====> cuts
    TCut cat;
    TString catname;
    if( catbin == 1 ) {
        cat = "pt > 20 && pt < 30";
        if( withPUJID ) { catname = "with PUJID (20 < pt < 30)"; }
        else          { catname = "w/o  PUJID (20 < pt < 30)"; }
    } else if( catbin == 2 ) {
        cat = "pt > 30 && pt < 50";
        if( withPUJID ) { catname = "with PUJID (30 < pt < 50)"; }
        else          { catname = "w/o  PUJID (30 < pt < 50)"; }
    } else if( catbin == 3 ) {
        cat = "pt > 50 && pt < 80";
        if( withPUJID ) { catname = "with PUJID (50 < pt < 80)"; }
        else          { catname = "w/o  PUJID (50 < pt < 80)"; }
    } else if( catbin == 4 ) {
        cat = "pt > 80";
        if( withPUJID ) { catname = "with PUJID (pt > 80)"; }
        else          { catname = "w/o  PUJID (pt > 80)"; }
    }
    TCut cut_photon_rej     = "!(photonMatch==1 && GenPhotonPt/bestPt > 0.5 && photondRmin < 0.2)";
    TCut cut_gen_photon_rej = "!(photonMatch==1 && GenPhotonPt/pt > 0.5 && photondRmin < 0.2)";
    TCut cut_PUJID          = "passesPUJetID>0";
    TCut cut_pt             = "bestPt>20.";
    TCut cut_gen_pt         = "pt>20.";
    TCut cut_genjetmatch    = "genJetMatch>0";
    TCut cut_recojetmatch   = "recoJetMatch>0";

    TCut cut_all = cut_pt && cut_photon_rej && cat;
    if( withPUJID ) { cut_all = cut_all && cut_PUJID; }

    TCut cut_gen_all = cut_gen_pt && cut_gen_photon_rej && cat;
    if( withPUJID ) { cut_gen_all = cut_gen_all && cut_PUJID; }

    // ===> Only for def


    TFile *fplots = new TFile( Form( "plots/perfomances_cat_%i_PUJID_%i.root", catbin, withPUJID ), "UPDATE" );
    fplots->cd();
    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        h_eta_all[it->first] = new TH1F( Form( "h_eta_all_%s", it->first.Data() ),
                                         Form( "%s;#eta;#varepsilon_{fake}", it->first.Data() ),
                                         50, -5, 5 );
        h_eta_sel[it->first] = new TH1F( Form( "h_eta_sel_%s", it->first.Data() ),
                                         Form( "%s;#eta;#varepsilon_{fake}", it->first.Data() ),
                                         50, -5, 5 );

        h_effeta_all[it->first] = new TH1F( Form( "h_effeta_all_%s", it->first.Data() ),
                                            Form( "%s;#eta;#varepsilon_{fake}", it->first.Data() ),
                                            50, -5, 5 );
        h_effeta_sel[it->first] = new TH1F( Form( "h_effeta_sel_%s", it->first.Data() ),
                                            Form( "%s;#eta;#varepsilon_{fake}", it->first.Data() ),
                                            50, -5, 5 );


        h2_response_eta[it->first] = new TH2F( Form( "h2_response_eta_%s", it->first.Data() ),
                                               Form( "%s;#eta;p^{reco}_{t}/p^{gen}_{t}", it->first.Data() ),
                                               25, -5, 5, 200, 0, 2 );

        JetTree[it->first]->Draw( Form( "eta>>h_eta_all_%s", it->first.Data() ), cut_all, "" );
        JetTree[it->first]->Draw( Form( "eta>>h_eta_sel_%s", it->first.Data() ), cut_all && !cut_genjetmatch, "" );

        if( withPUJID ) {
            JetTree[it->first]->Draw( Form( "bestPt/genJetPt:eta>>h2_response_eta_%s", it->first.Data() ),
                                      cut_photon_rej && cat &&  cut_PUJID && cut_genjetmatch, "" );
        } else {
            JetTree[it->first]->Draw( Form( "bestPt/genJetPt:eta>>h2_response_eta_%s", it->first.Data() ),
                                      cut_photon_rej && cat && cut_genjetmatch, "" );
        }
        std::cout << Form( "g_resolution_eta_%s :: ", it->first.Data() )
                  << h2_response_eta[it->first]->GetEntries() << std::endl;

        jet_perf perff( h2_response_eta[it->first], Form( "g_perf_eta_%s", it->first.Data() ) );
        gr_resolution_eta[it->first] = perff.GetResolution();
        gr_response_eta[it->first] = perff.GetResponse();

        fake_rate_vs_eta[it->first] = new TGraphAsymmErrors( h_eta_sel[it->first], h_eta_all[it->first] );
        fake_rate_vs_eta[it->first] -> SetName( Form( "g_fake_%s", it->first.Data() ) );
        fake_rate_vs_eta[it->first] -> SetTitle( Form( "%s;eta;#varepsilon_{fake}", it->first.Data() ) );

        h_eta_all[it->first]->Clear();
        h_eta_sel[it->first]->Clear();

        GenJetTree[it->first]->Draw( Form( "eta>>h_effeta_all_%s", it->first.Data() ), cut_gen_all , "" );
        GenJetTree[it->first]->Draw( Form( "eta>>h_effeta_sel_%s", it->first.Data() ), cut_recojetmatch && cut_gen_all, "" );

        eff_vs_eta[it->first] = new TGraphAsymmErrors( h_effeta_sel[it->first], h_effeta_all[it->first] );
        eff_vs_eta[it->first] -> SetName( Form( "g_eff_%s", it->first.Data() ) );
        eff_vs_eta[it->first] -> SetTitle( Form( "%s;eta;#varepsilon", it->first.Data() ) );


        h2_response_eta  [it->first]->Write( "", TObject::kOverwrite );
        //h_eta_all        [it->first]->Write();;
        //h_eta_sel        [it->first]->Write();;
        //h_effeta_all     [it->first]->Write();;
        //h_effeta_sel     [it->first]->Write();;
        //h2_response_eta  [it->first]->Write();;
        //gr_resolution_eta[it->first]->Write();;
        //gr_response_eta  [it->first]->Write();;
        //fake_rate_vs_eta [it->first]->Write();;
        //fake_rate_vs_eat[it->first]->Write();;
        //eff_vs_eta       [it->first]->Write();;
        //eff_vs_eat      [it->first]->Write();;
        //roc_curve       [it->first]->Write();;
        //roc_curve[it->first] = GetRocCurve(eff_vs_eta[it->first],
        //fake_rate_vs_eta[it->first],
        //it->first);

    }

    fplots->Close();


    std::cout << "====> ploting" << std::endl;
    std::cout << "      fake rate" << std::endl;
    TCanvas *c_fake_eta  = new TCanvas( "c_fake_eta", "", 500, 500 );
    c_fake_eta->cd();
    int count = 1;
    TMultiGraph *gmul_fake_eta = new TMultiGraph();
    gmul_fake_eta->SetTitle( Form( "%s;eta;fakes", catname.Data() ) );
    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        fake_rate_vs_eta[it->first]->SetLineColor( count );//int(55 + count*10));
        fake_rate_vs_eta[it->first]->SetMarkerColor( count ); //int(55 + count*10));
        gmul_fake_eta->Add( fake_rate_vs_eta[it->first] );
        count++;
    }
    gmul_fake_eta->SetMinimum( 0 );
    gmul_fake_eta->SetMaximum( 1 );

    gmul_fake_eta->Draw( "AP" );
    c_fake_eta->Modified();
    c_fake_eta->Update();
    gmul_fake_eta->GetXaxis()->SetRangeUser( -5, 5 );

    Draw3Legend( fake_rate_vs_eta["PF"],
                 fake_rate_vs_eta["PFCHS0"],
                 fake_rate_vs_eta["PFCHSLeg"],
                 "PF",
                 "PFCHS0",
                 "PFCHSLeg" );

    c_fake_eta->SaveAs( Form( "plots/fakerate_cat_%i_PUJID_%i.pdf", catbin, withPUJID ) );
    c_fake_eta->SaveAs( Form( "plots/fakerate_cat_%i_PUJID_%i.png", catbin, withPUJID ) );
    std::cout << "      efficiency" << std::endl;
    TCanvas *c_eff_eta  = new TCanvas( "c_eff_eta", "", 500, 500 );
    c_eff_eta->cd();
    count = 1;
    TMultiGraph *gmul_eff_eta = new TMultiGraph();
    gmul_eff_eta->SetTitle( Form( "%s;#eta;#varepsilon", catname.Data() ) );
    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        eff_vs_eta[it->first]->SetLineColor( count );//int(55 + count*10));
        eff_vs_eta[it->first]->SetMarkerColor( count ); //int(55 + count*10));
        gmul_eff_eta->Add( eff_vs_eta[it->first] );
        count++;
    }

    gmul_eff_eta->SetMinimum( 0 );
    gmul_eff_eta->SetMaximum( 1 );
    gmul_eff_eta->Draw( "AP" );
    c_eff_eta->Modified();
    c_eff_eta->Update();
    gmul_eff_eta->GetXaxis()->SetRangeUser( -5, 5 ); //->SetRangeUser(0,200);

    Draw3Legend( eff_vs_eta["PF"],
                 eff_vs_eta["PFCHS0"],
                 eff_vs_eta["PFCHSLeg"],
                 "PF",
                 "PFCHS0",
                 "PFCHSLeg", "2" );
    c_eff_eta->SaveAs( Form( "plots/eta_eff_cat_%i_PUJID_%i.pdf", catbin, withPUJID ) );
    c_eff_eta->SaveAs( Form( "plots/eta_eff_cat_%i_PUJID_%i.png", catbin, withPUJID ) );
    //TCanvas *c_roc  = new TCanvas("c_roc","",500,500);
    //c_roc->cd();
    //count = 1;
    //TMultiGraph *gmul_roc = new TMultiGraph();
    //for (std::map<TString,TTree*>::iterator it=JetTree.begin(); it!=JetTree.end(); ++it){
    //  roc_curve[it->first]->SetLineColor  (count);//int(55 + count*10));
    //  roc_curve[it->first]->SetMarkerColor(count);//int(55 + count*10));
    //  gmul_roc->Add(roc_curve[it->first]);
    //  count++;
    //}
    //gmul_roc->Draw("AP");
    //Draw3Legend(roc_curve["PF"],
    //	      roc_curve["PFCHS0"],
    //	      roc_curve["PFCHSLeg"],
    //	      "PF",
    //	      "PFCHS0",
    //	      "PFCHSLeg");
    //


    TCanvas *c_res  = new TCanvas( "c_res", "", 500, 500 );
    c_res->cd();
    count = 1;
    TMultiGraph *gmul_res = new TMultiGraph();
    gmul_res->SetTitle( Form( "%s;#eta;#sigma(R_{jet})/<R_{jet}>", catname.Data() ) );
    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        gr_resolution_eta[it->first]->SetLineColor( count );//int(55 + count*10));
        gr_resolution_eta[it->first]->SetMarkerColor( count ); //int(55 + count*10));
        gmul_res->Add( gr_resolution_eta[it->first] );
        count++;
    }

    gmul_res->SetMinimum( 0 );
    gmul_res->SetMaximum( 1.0 );

    //gmul_res->GetXaxis()->SetRangeUser(0,200);

    gmul_res->Draw( "AP" );
    c_res->Modified();
    c_res->Update();
    gmul_res->GetXaxis()->SetRangeUser( -5, 5 ); //SetRangeUser(0,200);
    gmul_res->GetYaxis()->SetRangeUser( 0, 1 ); //SetRangeUser(0,200);

    Draw3Legend( gr_resolution_eta["PF"],
                 gr_resolution_eta["PFCHS0"],
                 gr_resolution_eta["PFCHSLeg"],
                 "PF",
                 "PFCHS0",
                 "PFCHSLeg" );


    c_res->SaveAs( Form( "plots/eta_resolution_cat_%i_PUJID_%i.pdf", catbin, withPUJID ) );
    c_res->SaveAs( Form( "plots/eta_resolution_cat_%i_PUJID_%i.png", catbin, withPUJID ) );



    TCanvas *c_resp  = new TCanvas( "c_resp", "", 500, 500 );
    c_resp->cd();
    count = 1;
    TMultiGraph *gmul_resp = new TMultiGraph();
    gmul_resp->SetTitle( Form( "%s;p^{gen}_{t}[GeV];<R_{jet}>", catname.Data() ) );
    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        std::cout << count << " :: " << gr_response_eta[it->first] << std::endl;

        gr_response_eta[it->first]->SetLineColor( count );//int(55 + count*10));
        gr_response_eta[it->first]->SetMarkerColor( count ); //int(55 + count*10));
        gmul_resp->Add( gr_response_eta[it->first] );
        count++;
    }

    gmul_resp->SetMinimum( 0 );
    gmul_resp->SetMaximum( 2 );

    //gmul_res->GetXaxis()->SetRangeUser(0,200);

    gmul_resp->Draw( "AP" );
    c_resp->Modified();
    c_resp->Update();
    gmul_resp->GetXaxis()->SetRangeUser( -5, 5 ); //->SetRangeUser(0,200);

    Draw3Legend( gr_response_eta["PF"],
                 gr_response_eta["PFCHS0"],
                 gr_response_eta["PFCHSLeg"],
                 "PF",
                 "PFCHS0",
                 "PFCHSLeg" );

    c_resp->SaveAs( Form( "plots/eta_response_cat_%i_PUJID_%i.pdf", catbin, withPUJID ) );
    c_resp->SaveAs( Form( "plots/eta_response_cat_%i_PUJID_%i.png", catbin, withPUJID ) );


}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

