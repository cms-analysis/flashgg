{
    TFile *_file0 = TFile::Open( "jetValidationCollection_PU40bx50.root" ); //Open File
//	TFile *_file0 = TFile::Open("/afs/cern.ch/work/s/sethzenz/public/72x_jetValidationCollection_PU40bx50.root");//Open File
    //TFile *_file0 = TFile::Open("/afs/cern.ch/work/s/sethzenz/public/jetValidationCollection_70x_GluGluToHgg_PU20bx25.root");//Open File

    int color = 0;

    std::string bxList[2] = {"PU40bx50", "PU20bx25"};
    std::string jetColList[5] = {"PF", "PFCHS0", "PFCHSLeg", "PUPPI0", "PUPPILeg"};
    std::string etaList[5] = {"Inner", "Ears", "Outer"};
    std::string ptList[5] = {"low", "mid", "high"};


    TTree **jetTree50;
    TTree **jetTree25;
    jetTree50 = new TTree *[5];
    jetTree25 = new TTree *[5];

    TTree **genJetTree50;
    TTree **genJetTree25;
    genJetTree50 = new TTree *[5];
    genJetTree25 = new TTree *[5];

    TGraphAsymmErrors **graphs25_EtaEff;
    TGraphAsymmErrors **graphs50_EtaEff;
    graphList25_EtaEff = new TGraphAsymmErrors *[15];
    graphList50_EtaEff = new TGraphAsymmErrors *[15];

    TGraphAsymmErrors **graphs25_EtaFake;
    TGraphAsymmErrors **graphs50_EtaFake;
    graphList25_EtaFake = new TGraphAsymmErrors *[15];
    graphList50_EtaFake = new TGraphAsymmErrors *[15];

    TGraphAsymmErrors **graphs25_PtEff;
    TGraphAsymmErrors **graphs50_PtEff;
    graphList25_PtEff = new TGraphAsymmErrors *[15];
    graphList50_PtEff = new TGraphAsymmErrors *[15];

    TGraphAsymmErrors **graphs25_PtFake;
    TGraphAsymmErrors **graphs50_PtFake;
    graphList25_PtFake = new TGraphAsymmErrors *[15];
    graphList50_PtFake = new TGraphAsymmErrors *[15];

    TTree *jetTree50[0] = ( TTree * )flashggJetValidationTreeMaker->Get( "jetTree_PF" );
    TTree *jetTree50[1] = ( TTree * )flashggJetValidationTreeMakerPFCHS0->Get( "jetTree_PFCHS0" );
    TTree *jetTree50[2] = ( TTree * )flashggJetValidationTreeMakerPFCHSLeg->Get( "jetTree_PFCHSLeg" );
    TTree *jetTree50[3] = ( TTree * )flashggJetValidationTreeMakerPUPPI0->Get( "jetTree_PUPPI0" );
    TTree *jetTree50[4] = ( TTree * )flashggJetValidationTreeMakerPUPPILeg->Get( "jetTree_PUPPILeg" );

    TTree *genJetTree50[0] = ( TTree * )flashggJetValidationTreeMaker->Get( "genJetTree_PF" );
    TTree *genJetTree50[1] = ( TTree * )flashggJetValidationTreeMakerPFCHS0->Get( "genJetTree_PFCHS0" );
    TTree *genJetTree50[2] = ( TTree * )flashggJetValidationTreeMakerPFCHSLeg->Get( "genJetTree_PFCHSLeg" );
    TTree *genJetTree50[3] = ( TTree * )flashggJetValidationTreeMakerPUPPI0->Get( "genJetTree_PUPPI0" );
    TTree *genJetTree50[4] = ( TTree * )flashggJetValidationTreeMakerPUPPILeg->Get( "genJetTree_PUPPILeg" );

    TFile *_file1 = TFile::Open( "jetValidationCollection_PU40bx50_new.root" ); //Open File
    //TFile *_file1 = TFile::Open("/afs/cern.ch/work/s/sethzenz/public/72x_jetValidationCollection_PU20bx25.root");//Open File
    //TFile *_file1 = TFile::Open("/afs/cern.ch/work/s/sethzenz/public/jetValidationCollection_72xMiniAOD_GluGluToHgg_PU20bx25.root");//Open File
    TTree *jetTree25[0] = ( TTree * )flashggJetValidationTreeMaker->Get( "jetTree_PF" );
    TTree *jetTree25[1] = ( TTree * )flashggJetValidationTreeMakerPFCHS0->Get( "jetTree_PFCHS0" );
    TTree *jetTree25[2] = ( TTree * )flashggJetValidationTreeMakerPFCHSLeg->Get( "jetTree_PFCHSLeg" );
    TTree *jetTree25[3] = ( TTree * )flashggJetValidationTreeMakerPUPPI0->Get( "jetTree_PUPPI0" );
    TTree *jetTree25[4] = ( TTree * )flashggJetValidationTreeMakerPUPPILeg->Get( "jetTree_PUPPILeg" );

    TTree *genJetTree25[0] = ( TTree * )flashggJetValidationTreeMaker->Get( "genJetTree_PF" );
    TTree *genJetTree25[1] = ( TTree * )flashggJetValidationTreeMakerPFCHS0->Get( "genJetTree_PFCHS0" );
    TTree *genJetTree25[2] = ( TTree * )flashggJetValidationTreeMakerPFCHSLeg->Get( "genJetTree_PFCHSLeg" );
    TTree *genJetTree25[3] = ( TTree * )flashggJetValidationTreeMakerPUPPI0->Get( "genJetTree_PUPPI0" );
    TTree *genJetTree25[4] = ( TTree * )flashggJetValidationTreeMakerPUPPILeg->Get( "genJetTree_PUPPILeg" );

    TCanvas *c = new TCanvas( "c", "c", 800, 800 );
    TCanvas *c0  = new TCanvas( "c0", "c0 ", 800, 800 );
    TCanvas *c1  = new TCanvas( "c1", "c1 ", 800, 800 );
    TCanvas *c2  = new TCanvas( "c2", "c2 ", 800, 800 );
    TCanvas *c3  = new TCanvas( "c3", "c3 ", 800, 800 );
    TCanvas *c4  = new TCanvas( "c4", "c4 ", 800, 800 );
    TCanvas *c5  = new TCanvas( "c5", "c5 ", 800, 800 );
    TCanvas *c6  = new TCanvas( "c6", "c6 ", 800, 800 );
    TCanvas *c7  = new TCanvas( "c7", "c7 ", 800, 800 );
    TCanvas *c8  = new TCanvas( "c8", "c8 ", 800, 800 );
    TCanvas *c9  = new TCanvas( "c9", "c9 ", 800, 800 );
    TCanvas *c10 = new TCanvas( "c10", "c10", 800, 800 );
    TCanvas *c11 = new TCanvas( "c11", "c11", 800, 800 );
    TCanvas *c12 = new TCanvas( "c12", "c12", 800, 800 );
    TCanvas *c13 = new TCanvas( "c13", "c13", 800, 800 );
    TCanvas *c14 = new TCanvas( "c14", "c14", 800, 800 );
    TCanvas *c15 = new TCanvas( "c15", "c15", 800, 800 );
    TCanvas *c16 = new TCanvas( "c16", "c16", 800, 800 );
    TCanvas *c17 = new TCanvas( "c17", "c17", 800, 800 );
    TCanvas *c18 = new TCanvas( "c18", "c18", 800, 800 );
    TCanvas *c19 = new TCanvas( "c19", "c19", 800, 800 );
    TCanvas *c20 = new TCanvas( "c20", "c20", 800, 800 );
    TCanvas *c21 = new TCanvas( "c21", "c21", 800, 800 );
    TCanvas *c22 = new TCanvas( "c22", "c22", 800, 800 );
    TCanvas *c23 = new TCanvas( "c23", "c23", 800, 800 );
    TCut cutStringsEta[3] = {"abs(eta)<2.5", "abs(eta)>3", " !(abs(eta)<2.5) && !(abs(eta)>3)"};
    TCut cutStringsBestPt[3] = {"bestPt > 20 && bestPt < 50", "bestPt > 50 && bestPt < 80", " bestPt >80"};
    TCut cutStringsPt[3] = {"pt > 20 && pt < 50", "pt > 50 && pt < 80", " pt >80"};
    std::string cutStringsPtText[3] = {"pt > 20 && pt < 50", "pt > 50 && pt < 80", " pt >80"};
    std::string cutStringsEtaText[3] = {"fabs(eta)<2.5", "fabs(eta)>3", " 2.5<fabs(eta)<3 "};
    TCut bestPtCut = "bestPt>20";
    TCut ptCut = "pt>20";
    TCut genMatch = "genJetMatch";
    TCut recoMatch = "recoJetMatch";
    //std::string andand =" && ";
    c->Print( "effPlots.pdf[" );
    TGraphAsymmErrors *dummy50;
    TGraphAsymmErrors *dummy25;

    TFile *t = new TFile( "graphs5.root", "recreate" );

    for( unsigned int treeLoop = 0; treeLoop < 5 ; treeLoop++ )
    {
        for( unsigned int cutLoop = 0 ; cutLoop < 3; cutLoop++ ) {


            std::cout << cutLoop + 3 * treeLoop << std::endl;
            //std::string cutStringText = cut0 + andand + cutStringsEtaText[cutLoop];
            //	TCut cutString = new TCut(cutStringText);

            //ETA plots (fake rates)
            jetTree50[treeLoop]->Draw( "eta>>htemp(50,-5,5)", bestPtCut && cutStringsBestPt[cutLoop] && !genMatch );
            TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *num = ( TH1F * )htemp->Clone( "num" );
            num->SetTitle( ( "fake rate " + cutStringsPtText[cutLoop] + " " + jetColList[treeLoop] ).c_str() );
            jetTree50[treeLoop]->Draw( "eta>>htemp(50,-5,5)", bestPtCut && cutStringsBestPt[cutLoop] );
            htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *den = ( TH1F * )htemp->Clone( "den" );
            TGraphAsymmErrors *graph50 = new TGraphAsymmErrors( num, den );
            //	TGraphAsymmErrors *graphList50_EtaFake[cutLoop+3*treeLoop] = graph50->Clone();
            graph50 ->GetXaxis()->SetTitle( "#eta" );
            std::string name = ( jetColList[treeLoop] + ptList[cutLoop] + bxList[0] );
            graph50 ->SetName( name.c_str() );
            graph50 ->Write();
            if( cutLoop == 0 ) {
                c0->cd();
                if( treeLoop == 0 ) {
                    graph50->SetLineColor( kRed );
                    graph50->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph50->SetLineColor( kBlue );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph50->SetLineColor( kGreen );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph50->SetLineColor( kBlack );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph50->SetLineColor( 7 );
                    graph50->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 1 ) {
                c1->cd();
                if( treeLoop == 0 ) {
                    graph50->SetLineColor( kRed );
                    graph50->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph50->SetLineColor( kBlue );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph50->SetLineColor( kGreen );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph50->SetLineColor( kBlack );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph50->SetLineColor( 7 );
                    graph50->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 2 ) {
                c2->cd();
                if( treeLoop == 0 ) {
                    graph50->SetLineColor( kRed );
                    graph50->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph50->SetLineColor( kBlue );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph50->SetLineColor( kGreen );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph50->SetLineColor( kBlack );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph50->SetLineColor( 7 );
                    graph50->Draw( "same P" );
                }
                c->cd();
            }


            jetTree25[treeLoop]->Draw( "eta>>htemp(50,-5,5)", bestPtCut && cutStringsBestPt[cutLoop] && !genMatch );
            TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *num = ( TH1F * )htemp->Clone( "num" );
            num->SetTitle( ( "fake rate " + cutStringsPtText[cutLoop] + " " + jetColList[treeLoop] ).c_str() );
            jetTree25[treeLoop]->Draw( "eta>>htemp(50,-5,5)", bestPtCut && cutStringsBestPt[cutLoop] );
            htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *den = ( TH1F * )htemp->Clone( "den" );
            TGraphAsymmErrors *graph25 = new TGraphAsymmErrors( num, den );
//	TGraphAsymmErrors *graphList25_EtaFake[cutLoop+3*treeLoop] =  graph25;
            graph25->GetXaxis()->SetTitle( "#eta" );

            std::string name = ( jetColList[treeLoop] + ptList[cutLoop] + bxList[1] );
            graph25 ->SetName( name.c_str() );
            graph25 ->Write();
            if( cutLoop == 0 ) {
                c3->cd();
                if( treeLoop == 0 ) {
                    graph25->SetLineColor( kRed );
                    graph25->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph25->SetLineColor( kBlue );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph25->SetLineColor( kGreen );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph25->SetLineColor( kBlack );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph25->SetLineColor( 7 );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 1 ) {
                c4->cd();
                if( treeLoop == 0 ) {
                    graph25->SetLineColor( kRed );
                    graph25->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph25->SetLineColor( kBlue );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph25->SetLineColor( kGreen );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph25->SetLineColor( kBlack );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph25->SetLineColor( 7 );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 2 ) {
                c5->cd();
                if( treeLoop == 0 ) {
                    graph25->SetLineColor( kRed );
                    graph25->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph25->SetLineColor( kBlue );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph25->SetLineColor( kGreen );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph25->SetLineColor( kBlack );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph25->SetLineColor( 7 );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }

            if( color ) { graph50 ->SetLineColor( kRed ); }
            if( color )	{ graph25->SetLineColor( kBlue ); }

            graph50 ->Draw();
            graph25 ->Draw( "same AP" );
            //graphList25_EtaFake[cutLoop+3*treeLoop] = (TGraphAsymmErrors*)graph25->Clone();
            //graphList50_EtaFake[cutLoop+3*treeLoop] = (TGraphAsymmErrors*)graph50->Clone();
            dummy50 =	( graph50 );
            dummy25 =	( graph25 );
            TLegend *leg = new TLegend( 0.1, 0.8, 0.48, 0.9 );
            leg->AddEntry( dummy50 , "50bx, 40pu", "LEP" );
            leg->AddEntry( dummy25 , "25bx, 20pu", "LEP" );
            leg->Draw();

            //	//std::cout << "DEBUG 8 "<< std::endl;

            if( color ) { c->Print( "effPlots.pdf" ); }
            c->Clear();

            ////	std::cout << "DEBUG 9 "<< std::endl;

            // PT plots (fake rates)
            jetTree50[treeLoop]->Draw( "bestPt>>htemp(50,0,250)", bestPtCut && cutStringsEta[cutLoop] && !genMatch );
            TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *num = ( TH1F * )htemp->Clone( "num" );
            num->SetTitle( ( "fake rate " + cutStringsEtaText[cutLoop] + " " + jetColList[treeLoop] ).c_str() );
            jetTree50[treeLoop]->Draw( "bestPt>>htemp(50,0,250)", bestPtCut && cutStringsEta[cutLoop] );
            htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *den = ( TH1F * )htemp->Clone( "den" );
            TGraphAsymmErrors *graph50 = new TGraphAsymmErrors( num, den );
            //	TGraphAsymmErrors *graphList50_PtFake[cutLoop+3*treeLoop] = new TGraphAsymmErrors(num,den);
            graph50->GetXaxis()->SetTitle( "P_{T}" );
            graph50 ->GetXaxis()->SetTitle( "#eta" );
            std::string name = ( jetColList[treeLoop] + ptList[cutLoop] + bxList[0] );
            graph50 ->SetName( name.c_str() );
            graph50 ->Write();
            if( cutLoop == 0 ) {
                c6->cd();
                if( treeLoop == 0 ) {
                    graph50->SetLineColor( kRed );
                    graph50->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph50->SetLineColor( kBlue );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph50->SetLineColor( kGreen );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph50->SetLineColor( kBlack );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph50->SetLineColor( 7 );
                    graph50->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 1 ) {
                c7->cd();
                if( treeLoop == 0 ) {
                    graph50->SetLineColor( kRed );
                    graph50->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph50->SetLineColor( kBlue );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph50->SetLineColor( kGreen );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph50->SetLineColor( kBlack );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph50->SetLineColor( 7 );
                    graph50->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 2 ) {
                c8->cd();
                if( treeLoop == 0 ) {
                    graph50->SetLineColor( kRed );
                    graph50->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph50->SetLineColor( kBlue );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph50->SetLineColor( kGreen );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph50->SetLineColor( kBlack );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph50->SetLineColor( 7 );
                    graph50->Draw( "same P" );
                }
                c->cd();
            }
            jetTree25[treeLoop]->Draw( "bestPt>>htemp(50,0,250)", ptCut && cutStringsEta[cutLoop] && !genMatch );
            TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *num = ( TH1F * )htemp->Clone( "num" );
            num->SetTitle( ( "fake rate " + cutStringsEtaText[cutLoop] ).c_str() );
            jetTree25[treeLoop]->Draw( "bestPt>>htemp(50,0,250)", ptCut && cutStringsEta[cutLoop] );
            htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *den = ( TH1F * )htemp->Clone( "den" );
            TGraphAsymmErrors *graph25 = new TGraphAsymmErrors( num, den );
            //	TGraphAsymmErrors *graphList25_PtFake[cutLoop+3*treeLoop] = new TGraphAsymmErrors(num,den);
            graph25->GetXaxis()->SetTitle( "P_{T}" );
            c->Clear();
            graph50->Draw();
            std::string name = ( jetColList[treeLoop] + ptList[cutLoop] + bxList[1] );
            graph25 ->SetName( name.c_str() );
            graph25 ->Write();
            if( cutLoop == 0 ) {
                c9->cd();
                if( treeLoop == 0 ) {
                    graph25->SetLineColor( kRed );
                    graph25->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph25->SetLineColor( kBlue );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph25->SetLineColor( kGreen );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph25->SetLineColor( kBlack );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph25->SetLineColor( 7 );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 1 ) {
                c10->cd();
                if( treeLoop == 0 ) {
                    graph25->SetLineColor( kRed );
                    graph25->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph25->SetLineColor( kBlue );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph25->SetLineColor( kGreen );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph25->SetLineColor( kBlack );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph25->SetLineColor( 7 );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 2 ) {
                c11->cd();
                if( treeLoop == 0 ) {
                    graph25->SetLineColor( kRed );
                    graph25->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph25->SetLineColor( kBlue );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph25->SetLineColor( kGreen );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph25->SetLineColor( kBlack );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph25->SetLineColor( 7 );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }
            if( color ) { graph50->SetLineColor( kRed ); }
            if( color ) { graph25->SetLineColor( kBlue ); }
            graph25->Draw( "same P" );

            TLegend *leg = new TLegend( 0.1, 0.8, 0.48, 0.9 );
            leg->AddEntry( graph50, "50bx, 40pu", "LEP" );
            leg->AddEntry( graph25, "25bx, 20pu", "LEP" );
            leg->Draw();
            if( color ) { c->Print( "effPlots.pdf" ); }

            // Eta Plots (eff)
            genJetTree50[treeLoop]->Draw( "eta>>htemp(50,-5,5)", ptCut && cutStringsPt[cutLoop] && recoMatch );
            TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *num = ( TH1F * )htemp->Clone( "num" );
            num->SetTitle( ( "efficiency " + cutStringsPtText[cutLoop] + " " + jetColList[treeLoop] ).c_str() );
            genJetTree50[treeLoop]->Draw( "eta>>htemp(50,-5,5)", ptCut && cutStringsPt[cutLoop] );
            htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *den = ( TH1F * )htemp->Clone( "den" );
            TGraphAsymmErrors *graph50 = new TGraphAsymmErrors( num, den );
            //	TGraphAsymmErrors *graphList50_EtaEff[cutLoop+3*treeLoop] = new TGraphAsymmErrors(num,den);
            graph50->GetXaxis()->SetTitle( "#eta" );
            std::string name = ( jetColList[treeLoop] + ptList[cutLoop] + bxList[0] );
            graph50 ->SetName( name.c_str() );
            graph50 ->Write();
            if( cutLoop == 0 ) {
                c12->cd();
                if( treeLoop == 0 ) {
                    graph50->SetLineColor( kRed );
                    graph50->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph50->SetLineColor( kBlue );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph50->SetLineColor( kGreen );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph50->SetLineColor( kBlack );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph50->SetLineColor( 7 );
                    graph50->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 1 ) {
                c13->cd();
                if( treeLoop == 0 ) {
                    graph50->SetLineColor( kRed );
                    graph50->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph50->SetLineColor( kBlue );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph50->SetLineColor( kGreen );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph50->SetLineColor( kBlack );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph50->SetLineColor( 7 );
                    graph50->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 2 ) {
                c14->cd();
                if( treeLoop == 0 ) {
                    graph50->SetLineColor( kRed );
                    graph50->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph50->SetLineColor( kBlue );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph50->SetLineColor( kGreen );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph50->SetLineColor( kBlack );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph50->SetLineColor( 7 );
                    graph50->Draw( "same P" );
                }
                c->cd();
            }
            //	graph50->SetLineColor(kRed);

            genJetTree25[treeLoop]->Draw( "eta>>htemp(50,-5,5)", ptCut && cutStringsPt[cutLoop] && recoMatch );
            TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *num = ( TH1F * )htemp->Clone( "num" );
            num->SetTitle( ( "efficiency " + cutStringsPtText[cutLoop] ).c_str() );
            genJetTree25[treeLoop]->Draw( "eta>>htemp(50,-5,5)", ptCut && cutStringsPt[cutLoop] );
            htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *den = ( TH1F * )htemp->Clone( "den" );
            TGraphAsymmErrors *graph25 = new TGraphAsymmErrors( num, den );
            //	TGraphAsymmErrors *graphList25_EtaEff[cutLoop+3*treeLoop] = new TGraphAsymmErrors(num,den);
            graph25->GetXaxis()->SetTitle( "#eta" );
            std::string name = ( jetColList[treeLoop] + ptList[cutLoop] + bxList[1] );
            graph25 ->SetName( name.c_str() );
            graph25 ->Write();
            if( cutLoop == 0 ) {
                c15->cd();
                if( treeLoop == 0 ) {
                    graph25->SetLineColor( kRed );
                    graph25->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph25->SetLineColor( kBlue );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph25->SetLineColor( kGreen );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph25->SetLineColor( kBlack );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph25->SetLineColor( 7 );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 1 ) {
                c16->cd();
                if( treeLoop == 0 ) {
                    graph25->SetLineColor( kRed );
                    graph25->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph25->SetLineColor( kBlue );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph25->SetLineColor( kGreen );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph25->SetLineColor( kBlack );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph25->SetLineColor( 7 );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 2 ) {
                c17->cd();
                if( treeLoop == 0 ) {
                    graph25->SetLineColor( kRed );
                    graph25->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph25->SetLineColor( kBlue );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph25->SetLineColor( kGreen );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph25->SetLineColor( kBlack );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph25->SetLineColor( 7 );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }
            //graph25->SetLineColor(kBlue);
            c->Clear();
            if( color ) { graph50->SetLineColor( kRed ); }
            if( color ) { graph25->SetLineColor( kBlue ); }
            graph50->Draw();
            graph25->Draw( "same P" );

            TLegend *leg = new TLegend( 0.1, 0.8, 0.48, 0.9 );
            leg->AddEntry( graph50, "50bx, 40pu", "LEP" );
            leg->AddEntry( graph25, "25bx, 20pu", "LEP" );
            leg->Draw();
            if( color ) { c->Print( "effPlots.pdf" ); }


            // PT Plots (eff)
            genJetTree50[treeLoop]->Draw( "pt>>htemp(50,0,250)", ptCut && cutStringsEta[cutLoop] && recoMatch );
            TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *num = ( TH1F * )htemp->Clone( "num" );
            num->SetTitle( ( "efficiency " + cutStringsEtaText[cutLoop] + " " + jetColList[treeLoop] ).c_str() );
            genJetTree50[treeLoop]->Draw( "pt>>htemp(50,0,250)", ptCut && cutStringsEta[cutLoop] );
            htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *den = ( TH1F * )htemp->Clone( "den" );
            TGraphAsymmErrors *graph50 = new TGraphAsymmErrors( num, den );
            //	TGraphAsymmErrors *graphList50_PtEff[cutLoop+3*treeLoop] = new TGraphAsymmErrors(num,den);
            graph50->GetXaxis()->SetTitle( "P_{T}" );
            std::string name = ( jetColList[treeLoop] + ptList[cutLoop] + bxList[0] );
            graph50 ->SetName( name.c_str() );
            graph50 ->Write();
            if( cutLoop == 0 ) {
                c18->cd();
                if( treeLoop == 0 ) {
                    graph50->SetLineColor( kRed );
                    graph50->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph50->SetLineColor( kBlue );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph50->SetLineColor( kGreen );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph50->SetLineColor( kBlack );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph50->SetLineColor( 7 );
                    graph50->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 1 ) {
                c19->cd();
                if( treeLoop == 0 ) {
                    graph50->SetLineColor( kRed );
                    graph50->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph50->SetLineColor( kBlue );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph50->SetLineColor( kGreen );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph50->SetLineColor( kBlack );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph50->SetLineColor( 7 );
                    graph50->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 2 ) {
                c20->cd();
                if( treeLoop == 0 ) {
                    graph50->SetLineColor( kRed );
                    graph50->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph50->SetLineColor( kBlue );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph50->SetLineColor( kGreen );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph50->SetLineColor( kBlack );
                    graph50->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph50->SetLineColor( 7 );
                    graph50->Draw( "same P" );
                }
                c->cd();
            }
            //	graph50->SetLineColor(kRed);

            genJetTree25[treeLoop]->Draw( "pt>>htemp(50,0,250)", ptCut && cutStringsEta[cutLoop] && recoMatch );
            TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *num = ( TH1F * )htemp->Clone( "num" );
            num->SetTitle( ( "efficiency " + cutStringsEtaText[cutLoop] ).c_str() );
            genJetTree25[treeLoop]->Draw( "pt>>htemp(50,0,250)", ptCut && cutStringsEta[cutLoop] );
            htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *den = ( TH1F * )htemp->Clone( "den" );
            TGraphAsymmErrors *graph25 = new TGraphAsymmErrors( num, den );
            //	TGraphAsymmErrors *graphList25_PtEff[cutLoop+3*treeLoop] = new TGraphAsymmErrors(num,den);
            graph25->GetXaxis()->SetTitle( "P_{T}" );
            std::string name = ( jetColList[treeLoop] + ptList[cutLoop] + bxList[1] );
            graph25 ->SetName( name.c_str() );
            graph25 ->Write();
            if( cutLoop == 0 ) {
                c21->cd();
                if( treeLoop == 0 ) {
                    graph25->SetLineColor( kRed );
                    graph25->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph25->SetLineColor( kBlue );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph25->SetLineColor( kGreen );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph25->SetLineColor( kBlack );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph25->SetLineColor( 7 );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 1 ) {
                c22->cd();
                if( treeLoop == 0 ) {
                    graph25->SetLineColor( kRed );
                    graph25->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph25->SetLineColor( kBlue );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph25->SetLineColor( kGreen );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph25->SetLineColor( kBlack );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph25->SetLineColor( 7 );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }
            if( cutLoop == 2 ) {
                c23->cd();
                if( treeLoop == 0 ) {
                    graph25->SetLineColor( kRed );
                    graph25->Draw( "AP" );
                }
                if( treeLoop == 1 ) {
                    graph25->SetLineColor( kBlue );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 2 ) {
                    graph25->SetLineColor( kGreen );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 3 ) {
                    graph25->SetLineColor( kBlack );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 4 ) {
                    graph25->SetLineColor( 7 );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }
            //	graph25->SetLineColor(kBlue);
            if( color ) { graph50->SetLineColor( kRed ); }
            if( color ) { graph25->SetLineColor( kBlue ); }
            c->Clear();
            graph50->Draw();
            graph25->Draw( "same P" );

            TLegend *leg = new TLegend( 0.1, 0.8, 0.48, 0.9 );
            leg->AddEntry( graph50, "50bx, 40pu", "LEP" );
            leg->AddEntry( graph25, "25bx, 20pu", "LEP" );
            leg->Draw();
            if( color ) { c->Print( "effPlots.pdf" ) ; }

        }

    }
    if( !color )
    {
//	c0->Print("effPlots.pdf");
//	c1->Print("effPlots.pdf");
//	c2->Print("effPlots.pdf");
        c3->Print( "effPlots.pdf" );
        c4->Print( "effPlots.pdf" );
        c5->Print( "effPlots.pdf" );
//	c6->Print("effPlots.pdf");
//	c7->Print("effPlots.pdf");
//	c8->Print("effPlots.pdf");
        c9->Print( "effPlots.pdf" );
        c10->Print( "effPlots.pdf" );
        c11->Print( "effPlots.pdf" );
//	c12->Print("effPlots.pdf");
//	c13->Print("effPlots.pdf");
//	c14->Print("effPlots.pdf");
        c15->Print( "effPlots.pdf" );
        c16->Print( "effPlots.pdf" );
        c17->Print( "effPlots.pdf" );
//	c18->Print("effPlots.pdf");
//	c19->Print("effPlots.pdf");
//	c20->Print("effPlots.pdf");
        c21->Print( "effPlots.pdf" );
        c22->Print( "effPlots.pdf" );
        c23->Print( "effPlots.pdf" );
    }

    for( int i = 0; i < 15; i++ )
    {


//	c->Clear();
//graphList25_EtaFake[i]->Draw("same P");

//	c->Print("effPlots.pdf]");
    }

    c->Print( "effPlots.pdf]" );
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

