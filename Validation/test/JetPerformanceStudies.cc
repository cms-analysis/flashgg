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




void JetPerformanceStudies( int catbin = 1, bool withPUJID = true )
{



    std::map<TString, TCanvas *> Canv;

    std::map<TString, TTree *>     JetTree;
    std::map<TString, TTree *>     GenJetTree;

    std::map<TString, TH1F *>                h_pt_all;
    std::map<TString, TH1F *>                h_pt_sel;
    std::map<TString, TH1F *>                h_effpt_all;
    std::map<TString, TH1F *>                h_effpt_sel;
    std::map<TString, TH2F *>                h2_response_pt;
    std::map<TString, TGraphErrors *>        gr_resolution_pt;
    std::map<TString, TGraphErrors *>        gr_response_pt;
    std::map<TString, TGraphAsymmErrors *>   fake_rate_vs_pt;
    std::map<TString, TGraphAsymmErrors *>   fake_rate_vs_eat;
    std::map<TString, TGraphAsymmErrors *>   eff_vs_pt;
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
        cat = "abs(eta) < 2.5";
        if( withPUJID ) { catname = "with PUJID (|#eta| < 2.5)"; }
        else          { catname = "w/o  PUJID (|#eta| < 2.5)"; }
    } else if( catbin == 2 ) {
        cat = "abs(eta) > 2.5 && abs(eta) < 3";
        if( withPUJID ) { catname = "with PUJID (2.5 <|#eta| < 3.0)"; }
        else          { catname = "w/o  PUJID (2.5 <|#eta| < 3.0)"; }
    } else if( catbin == 3 ) {
        cat = "abs(eta) > 3";
        if( withPUJID ) { catname = "with PUJID (|#eta| > 3.0)"; }
        else          { catname = "w/o  PUJID (|#eta| > 3.0)"; }
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

    // ===> Only for def


    TFile *fplots = new TFile( Form( "plots/perfomances_cat_%i_PUJID_%i.root", catbin, withPUJID ), "UPDATE" );
    fplots->cd();
    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        h_pt_all[it->first] = new TH1F( Form( "h_pt_all_%s", it->first.Data() ),
                                        Form( "%s;p_{t};#varepsilon_{fake}", it->first.Data() ),
                                        50, 0, 200 );
        h_pt_sel[it->first] = new TH1F( Form( "h_pt_sel_%s", it->first.Data() ),
                                        Form( "%s;p_{t};#varepsilon_{fake}", it->first.Data() ),
                                        50, 0, 200 );

        h_effpt_all[it->first] = new TH1F( Form( "h_effpt_all_%s", it->first.Data() ),
                                           Form( "%s;p_{t};#varepsilon_{fake}", it->first.Data() ),
                                           50, 0, 200 );
        h_effpt_sel[it->first] = new TH1F( Form( "h_effpt_sel_%s", it->first.Data() ),
                                           Form( "%s;p_{t};#varepsilon_{fake}", it->first.Data() ),
                                           50, 0, 200 );


        h2_response_pt[it->first] = new TH2F( Form( "h2_response_pt_%s", it->first.Data() ),
                                              Form( "%s;p^{gen}_{t};p^{reco}_{t}/p^{gen}_{t}", it->first.Data() ),
                                              45, 20, 200, 200, 0, 2 );

        JetTree[it->first]->Draw( Form( "bestPt>>h_pt_all_%s", it->first.Data() ), cut_all, "" );
        JetTree[it->first]->Draw( Form( "bestPt>>h_pt_sel_%s", it->first.Data() ), cut_all && !cut_genjetmatch, "" );

        if( withPUJID ) {
            JetTree[it->first]->Draw( Form( "bestPt/genJetPt:genJetPt>>h2_response_pt_%s", it->first.Data() ),
                                      cut_photon_rej && cat &&  cut_PUJID && cut_genjetmatch, "" );
        } else {
            JetTree[it->first]->Draw( Form( "bestPt/genJetPt:genJetPt>>h2_response_pt_%s", it->first.Data() ),
                                      cut_photon_rej && cat && cut_genjetmatch, "" );
        }
        std::cout << Form( "g_resolution_pt_%s :: ", it->first.Data() )
                  << h2_response_pt[it->first]->GetEntries() << std::endl;

        jet_perf perff( h2_response_pt[it->first], Form( "g_perf_pt_%s", it->first.Data() ) );
        gr_resolution_pt[it->first] = perff.GetResolution();
        gr_response_pt[it->first] = perff.GetResponse();

        fake_rate_vs_pt[it->first] = new TGraphAsymmErrors( h_pt_sel[it->first], h_pt_all[it->first] );
        fake_rate_vs_pt[it->first] -> SetName( Form( "g_fake_%s", it->first.Data() ) );
        fake_rate_vs_pt[it->first] -> SetTitle( Form( "%s;p_{t};#varepsilon_{fake}", it->first.Data() ) );

        h_pt_all[it->first]->Clear();
        h_pt_sel[it->first]->Clear();

        GenJetTree[it->first]->Draw( Form( "pt>>h_effpt_all_%s", it->first.Data() ), cut_gen_all , "" );
        if( withPUJID ) {
            GenJetTree[it->first]->Draw( Form( "pt>>h_effpt_sel_%s", it->first.Data() ), cut_recojetmatch && cut_gen_all && cut_PUJID, "" );
        } else {
            GenJetTree[it->first]->Draw( Form( "pt>>h_effpt_sel_%s", it->first.Data() ), cut_recojetmatch && cut_gen_all, "" );
        }

        std::cout << "======= cut ====== "  << std::endl;
        std::cout << "\t all::" << cut_gen_all.GetTitle() << std::endl;

        TCut tmpcut = cut_recojetmatch && cut_gen_all && cut_PUJID;
        std::cout << "\t sel::" <<  tmpcut.GetTitle() << std::endl;
        std::cout << "================== "  << std::endl;

        eff_vs_pt[it->first] = new TGraphAsymmErrors( h_effpt_sel[it->first], h_effpt_all[it->first] );
        eff_vs_pt[it->first] -> SetName( Form( "g_eff_%s", it->first.Data() ) );
        eff_vs_pt[it->first] -> SetTitle( Form( "%s;p_{t};#varepsilon", it->first.Data() ) );


        h2_response_pt  [it->first]->Write( "", TObject::kOverwrite );
        //h_pt_all        [it->first]->Write();;
        //h_pt_sel        [it->first]->Write();;
        //h_effpt_all     [it->first]->Write();;
        //h_effpt_sel     [it->first]->Write();;
        //h2_response_pt  [it->first]->Write();;
        //gr_resolution_pt[it->first]->Write();;
        //gr_response_pt  [it->first]->Write();;
        //fake_rate_vs_pt [it->first]->Write();;
        //fake_rate_vs_eat[it->first]->Write();;
        //eff_vs_pt       [it->first]->Write();;
        //eff_vs_eat      [it->first]->Write();;
        //roc_curve       [it->first]->Write();;
        //roc_curve[it->first] = GetRocCurve(eff_vs_pt[it->first],
        //fake_rate_vs_pt[it->first],
        //it->first);

    }

    fplots->Close();


    std::cout << "====> ploting" << std::endl;
    std::cout << "      fake rate" << std::endl;
    TCanvas *c_fake_pt  = new TCanvas( "c_fake_pt", "", 500, 500 );
    c_fake_pt->cd();
    int count = 1;
    TMultiGraph *gmul_fake_pt = new TMultiGraph();
    gmul_fake_pt->SetTitle( Form( "%s;p_{t}[GeV];fakes", catname.Data() ) );
    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        fake_rate_vs_pt[it->first]->SetLineColor( count );//int(55 + count*10));
        fake_rate_vs_pt[it->first]->SetMarkerColor( count ); //int(55 + count*10));
        gmul_fake_pt->Add( fake_rate_vs_pt[it->first] );
        count++;
    }
    gmul_fake_pt->SetMinimum( 0 );
    gmul_fake_pt->SetMaximum( 1 );

    gmul_fake_pt->Draw( "AP" );
    c_fake_pt->Modified();
    c_fake_pt->Update();
    gmul_fake_pt->GetXaxis()->SetRangeUser( 0, 200 );

    Draw3Legend( fake_rate_vs_pt["PF"],
                 fake_rate_vs_pt["PFCHS0"],
                 fake_rate_vs_pt["PFCHSLeg"],
                 "PF",
                 "PFCHS0",
                 "PFCHSLeg" );

    c_fake_pt->SaveAs( Form( "plots/fakerate_cat_%i_PUJID_%i.pdf", catbin, withPUJID ) );
    c_fake_pt->SaveAs( Form( "plots/fakerate_cat_%i_PUJID_%i.png", catbin, withPUJID ) );

    std::cout << "      efficiency" << std::endl;

    TCanvas *c_eff_pt  = new TCanvas( "c_eff_pt", "", 500, 500 );
    c_eff_pt->cd();
    count = 1;
    TMultiGraph *gmul_eff_pt = new TMultiGraph();
    gmul_eff_pt->SetTitle( Form( "%s;p_{t}[GeV];#varepsilon", catname.Data() ) );
    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        eff_vs_pt[it->first]->SetLineColor( count );//int(55 + count*10));
        eff_vs_pt[it->first]->SetMarkerColor( count ); //int(55 + count*10));
        gmul_eff_pt->Add( eff_vs_pt[it->first] );
        count++;
    }

    gmul_eff_pt->SetMinimum( 0 );
    gmul_eff_pt->SetMaximum( 1 );
    gmul_eff_pt->Draw( "AP" );
    c_eff_pt->Modified();
    c_eff_pt->Update();
    gmul_eff_pt->GetXaxis()->SetRangeUser( 0, 200 );

    Draw3Legend( eff_vs_pt["PF"],
                 eff_vs_pt["PFCHS0"],
                 eff_vs_pt["PFCHSLeg"],
                 "PF",
                 "PFCHS0",
                 "PFCHSLeg", "3" );
    c_eff_pt->SaveAs( Form( "plots/eff_cat_%i_PUJID_%i.pdf", catbin, withPUJID ) );
    c_eff_pt->SaveAs( Form( "plots/eff_cat_%i_PUJID_%i.png", catbin, withPUJID ) );
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
    gmul_res->SetTitle( Form( "%s;p^{gen}_{t}[GeV];#sigma(R_{jet})/<R_{jet}>", catname.Data() ) );
    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        gr_resolution_pt[it->first]->SetLineColor( count );//int(55 + count*10));
        gr_resolution_pt[it->first]->SetMarkerColor( count ); //int(55 + count*10));
        gmul_res->Add( gr_resolution_pt[it->first] );
        count++;
    }

    gmul_res->SetMinimum( 0 );
    gmul_res->SetMaximum( 0.5 );

    //gmul_res->GetXaxis()->SetRangeUser(0,200);

    gmul_res->Draw( "AP" );
    c_res->Modified();
    c_res->Update();
    gmul_res->GetXaxis()->SetRangeUser( 0, 200 );

    Draw3Legend( gr_resolution_pt["PF"],
                 gr_resolution_pt["PFCHS0"],
                 gr_resolution_pt["PFCHSLeg"],
                 "PF",
                 "PFCHS0",
                 "PFCHSLeg" );


    c_res->SaveAs( Form( "plots/resolution_cat_%i_PUJID_%i.pdf", catbin, withPUJID ) );
    c_res->SaveAs( Form( "plots/resolution_cat_%i_PUJID_%i.png", catbin, withPUJID ) );



    TCanvas *c_resp  = new TCanvas( "c_resp", "", 500, 500 );
    c_resp->cd();
    count = 1;
    TMultiGraph *gmul_resp = new TMultiGraph();
    gmul_resp->SetTitle( Form( "%s;p^{gen}_{t}[GeV];<R_{jet}>", catname.Data() ) );
    for( std::map<TString, TTree *>::iterator it = JetTree.begin(); it != JetTree.end(); ++it ) {
        std::cout << count << " :: " << gr_response_pt[it->first] << std::endl;

        gr_response_pt[it->first]->SetLineColor( count );//int(55 + count*10));
        gr_response_pt[it->first]->SetMarkerColor( count ); //int(55 + count*10));
        gmul_resp->Add( gr_response_pt[it->first] );
        count++;
    }

    gmul_resp->SetMinimum( 0 );
    gmul_resp->SetMaximum( 2 );

    //gmul_res->GetXaxis()->SetRangeUser(0,200);

    gmul_resp->Draw( "AP" );
    c_resp->Modified();
    c_resp->Update();
    gmul_resp->GetXaxis()->SetRangeUser( 0, 200 );

    Draw3Legend( gr_response_pt["PF"],
                 gr_response_pt["PFCHS0"],
                 gr_response_pt["PFCHSLeg"],
                 "PF",
                 "PFCHS0",
                 "PFCHSLeg" );

    c_resp->SaveAs( Form( "plots/response_cat_%i_PUJID_%i.pdf", catbin, withPUJID ) );
    c_resp->SaveAs( Form( "plots/response_cat_%i_PUJID_%i.png", catbin, withPUJID ) );


}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

