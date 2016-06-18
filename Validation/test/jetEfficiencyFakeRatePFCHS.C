{
    int color = 0;

    std::string bxList[2] = {"PU40bx50", "PU20bx25"};
    std::string jetColList[8] = {"PF", "PFCHS0f0dz02", "PFCHS0f0dz05", "PFCHS0f0dz1", "PFCHS0f0dz2", "PFCHS0fpv1", "PFCHS0fpv2", "PFCHS0fpv3"};
    std::string etaList[5] = {"Inner", "Ears", "Outer"};
    std::string ptList[5] = {"low", "mid", "high"};


    TTree **jetTree50;
    TTree **jetTree25;
    jetTree50 = new TTree *[8];
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


    TFile *_file1 = TFile::Open( "jetValidationCollection_PU20bx25.root" ); //Open File
    //TFile *_file1 = TFile::Open("/afs/cern.ch/work/s/sethzenz/public/72x_jetValidationCollection_PU20bx25.root");//Open File
    //TFile *_file1 = TFile::Open("/afs/cern.ch/work/s/sethzenz/public/jetValidationCollection_72xMiniAOD_GluGluToHgg_PU20bx25.root");//Open File
    TTree *jetTree25[0] = ( TTree * )flashggJetValidationTreeMaker->Get( "jetTree_PF" );
    TTree *jetTree25[1] = ( TTree * )flashggJetValidationTreeMakerPFCHS0f0dz02->Get( "jetTree_PFCHS0f0dz02" );
    TTree *jetTree25[2] = ( TTree * )flashggJetValidationTreeMakerPFCHS0f0dz05->Get( "jetTree_PFCHS0f0dz05" );
    TTree *jetTree25[3] = ( TTree * )flashggJetValidationTreeMakerPFCHS0f0dz1->Get( "jetTree_PFCHS0f0dz1" );
    TTree *jetTree25[4] = ( TTree * )flashggJetValidationTreeMakerPFCHS0f0dz2->Get( "jetTree_PFCHS0f0dz2" );
    TTree *jetTree25[5] = ( TTree * )flashggJetValidationTreeMakerPFCHS0fpv1->Get( "jetTree_PFCHS0fpv1" );
    TTree *jetTree25[6] = ( TTree * )flashggJetValidationTreeMakerPFCHS0fpv2->Get( "jetTree_PFCHS0fpv2" );
    TTree *jetTree25[7] = ( TTree * )flashggJetValidationTreeMakerPFCHS0fpv3->Get( "jetTree_PFCHS0fpv3" );

    TTree *genJetTree25[0] = ( TTree * )flashggJetValidationTreeMaker->Get( "genJetTree_PF" );
    TTree *genJetTree25[1] = ( TTree * )flashggJetValidationTreeMakerPFCHS0f0dz02->Get( "genJetTree_PFCHS0f0dz02" );
    TTree *genJetTree25[2] = ( TTree * )flashggJetValidationTreeMakerPFCHS0f0dz05->Get( "genJetTree_PFCHS0f0dz05" );
    TTree *genJetTree25[3] = ( TTree * )flashggJetValidationTreeMakerPFCHS0f0dz1->Get( "genJetTree_PFCHS0f0dz1" );
    TTree *genJetTree25[4] = ( TTree * )flashggJetValidationTreeMakerPFCHS0f0dz2->Get( "genJetTree_PFCHS0f0dz2" );
    TTree *genJetTree25[5] = ( TTree * )flashggJetValidationTreeMakerPFCHS0fpv1->Get( "genJetTree_PFCHS0fpv1" );
    TTree *genJetTree25[6] = ( TTree * )flashggJetValidationTreeMakerPFCHS0fpv2->Get( "genJetTree_PFCHS0fpv2" );
    TTree *genJetTree25[7] = ( TTree * )flashggJetValidationTreeMakerPFCHS0fpv3->Get( "genJetTree_PFCHS0fpv3" );

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
    c->Print( "effPlotsPFCHS.pdf[" );
    TH1F *dummy0 = new TH1F( "d0", "do", 1, 1, 2 );
    TH1F *dummy1 = new TH1F( "d1", "do", 1, 1, 2 );
    TH1F *dummy2 = new TH1F( "d2", "do", 1, 1, 2 );
    TH1F *dummy3 = new TH1F( "d3", "do", 1, 1, 2 );
    TH1F *dummy4 = new TH1F( "d4", "do", 1, 1, 2 );
    TH1F *dummy5 = new TH1F( "d5", "do", 1, 1, 2 );
    TH1F *dummy6 = new TH1F( "d6", "do", 1, 1, 2 );
    TH1F *dummy7 = new TH1F( "d7", "do", 1, 1, 2 );
    dummy0->SetLineColor( kRed );
    dummy1->SetLineColor( kBlue );
    dummy2->SetLineColor( kGreen );
    dummy3->SetLineColor( kBlack );
    dummy4->SetLineColor( 7 );
    dummy5->SetLineColor( 12 );
    dummy6->SetLineColor( 41 );
    dummy7->SetLineColor( kViolet );



    TFile *t = new TFile( "graphs5.root", "recreate" );

    TLegend *legu = new TLegend( 0.1, 0.8, 0.48, 0.9 );
    legu->AddEntry( dummy0 , "pf", "LEP" );
    legu->AddEntry( dummy1 , "f0dz02", "LEP" );
    //		legu->AddEntry(dummy2 ,"f0dz05","LEP");
    //		legu->AddEntry(dummy3 ,"f0dz1","LEP");
    //		legu->AddEntry(dummy4 ,"f0dz2","LEP");
    legu->AddEntry( dummy5 , "fpv1", "LEP" );
    legu->AddEntry( dummy6 , "fpv2", "LEP" );
    //		legu->AddEntry(dummy7 ,"fpv3","LEP");
    TLegend *legd = new TLegend( 0.1, 0.1, 0.48, 0.2 );
    legd->AddEntry( dummy0 , "pf", "LEP" );
    legd->AddEntry( dummy1 , "f0dz02", "LEP" );
    //		legd->AddEntry(dummy2 ,"f0dz05","LEP");
    //		legd->AddEntry(dummy3 ,"f0dz1","LEP");
    //		legd->AddEntry(dummy4 ,"f0dz2","LEP");
    legd->AddEntry( dummy5 , "fpv1", "LEP" );
    legd->AddEntry( dummy6 , "fpv2", "LEP" );
    //		legd->AddEntry(dummy7 ,"fpv3","LEP");

    for( unsigned int treeLoop = 0; treeLoop < 8 ; treeLoop++ )
    {
        if( treeLoop == 2 || treeLoop == 3 || treeLoop == 4 || treeLoop > 6 ) { continue; }
        for( unsigned int cutLoop = 0 ; cutLoop < 3; cutLoop++ ) {


            std::cout << cutLoop + 3 * treeLoop << std::endl;
            //std::string cutStringText = cut0 + andand + cutStringsEtaText[cutLoop];
            //	TCut cutString = new TCut(cutStringText);

            //ETA plots (fake rates)

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
                if( treeLoop == 5 ) {
                    graph25->SetLineColor( 12 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 6 ) {
                    graph25->SetLineColor( 41 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 7 ) {
                    graph25->SetLineColor( kViolet );
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
                if( treeLoop == 5 ) {
                    graph25->SetLineColor( 12 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 6 ) {
                    graph25->SetLineColor( 41 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 7 ) {
                    graph25->SetLineColor( kViolet );
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
                if( treeLoop == 5 ) {
                    graph25->SetLineColor( 12 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 6 ) {
                    graph25->SetLineColor( 41 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 7 ) {
                    graph25->SetLineColor( kViolet );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }


            //	//std::cout << "DEBUG 8 "<< std::endl;

            if( color ) { c->Print( "effPlotsPFCHS.pdf" ); }
            c->Clear();

            ////	std::cout << "DEBUG 9 "<< std::endl;

            // PT plots (fake rates)
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
            //graph50->Draw();
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
                if( treeLoop == 5 ) {
                    graph25->SetLineColor( 12 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 6 ) {
                    graph25->SetLineColor( 41 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 7 ) {
                    graph25->SetLineColor( kViolet );
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
                if( treeLoop == 5 ) {
                    graph25->SetLineColor( 12 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 6 ) {
                    graph25->SetLineColor( 41 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 7 ) {
                    graph25->SetLineColor( kViolet );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }
            graph25->Draw( "same P" );


            // Eta Plots (eff)
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
                if( treeLoop == 5 ) {
                    graph25->SetLineColor( 12 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 6 ) {
                    graph25->SetLineColor( 41 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 7 ) {
                    graph25->SetLineColor( kViolet );
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
                if( treeLoop == 5 ) {
                    graph25->SetLineColor( 12 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 6 ) {
                    graph25->SetLineColor( 41 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 7 ) {
                    graph25->SetLineColor( kViolet );
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
                if( treeLoop == 5 ) {
                    graph25->SetLineColor( 12 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 6 ) {
                    graph25->SetLineColor( 41 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 7 ) {
                    graph25->SetLineColor( kViolet );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }

            // PT Plots (eff)
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
                if( treeLoop == 5 ) {
                    graph25->SetLineColor( 12 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 6 ) {
                    graph25->SetLineColor( 41 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 7 ) {
                    graph25->SetLineColor( kViolet );
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
                if( treeLoop == 5 ) {
                    graph25->SetLineColor( 12 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 6 ) {
                    graph25->SetLineColor( 41 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 7 ) {
                    graph25->SetLineColor( kViolet );
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
                if( treeLoop == 5 ) {
                    graph25->SetLineColor( 12 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 6 ) {
                    graph25->SetLineColor( 41 );
                    graph25->Draw( "same P" );
                }
                if( treeLoop == 7 ) {
                    graph25->SetLineColor( kViolet );
                    graph25->Draw( "same P" );
                }
                c->cd();
            }

        }

    }
    if( !color )
    {
//	c0->Print("effPlotsPFCHS.pdf");
//	c1->Print("effPlotsPFCHS.pdf");
//	c2->Print("effPlotsPFCHS.pdf");
        c3->cd();
        legu->Draw();
        c3->Print( "effPlotsPFCHS.pdf" );
        c4->cd();
        legu->Draw();
        c4->Print( "effPlotsPFCHS.pdf" );
        c5->cd();
        legu->Draw();
        c5->Print( "effPlotsPFCHS.pdf" );
//	c6->Print("effPlotsPFCHS.pdf");
//	c7->Print("effPlotsPFCHS.pdf");
//	c8->Print("effPlotsPFCHS.pdf");
        c9->cd();
        legu->Draw();
        c9->Print( "effPlotsPFCHS.pdf" );
        c10->cd();
        legu->Draw();
        c10->Print( "effPlotsPFCHS.pdf" );
        c11->cd();
        legu->Draw();
        c11->Print( "effPlotsPFCHS.pdf" );
//	c12->Print("effPlotsPFCHS.pdf");
//	c13->Print("effPlotsPFCHS.pdf");
//	c14->Print("effPlotsPFCHS.pdf");
        c15->cd();
        legd->Draw();
        c15->Print( "effPlotsPFCHS.pdf" );
        c16->cd();
        legd->Draw();
        c16->Print( "effPlotsPFCHS.pdf" );
        c17->cd();
        legd->Draw();
        c17->Print( "effPlotsPFCHS.pdf" );
//	c18->Print("effPlotsPFCHS.pdf");
//	c19->Print("effPlotsPFCHS.pdf");
//	c20->Print("effPlotsPFCHS.pdf");
        c21->cd();
        legd->Draw();
        c21->Print( "effPlotsPFCHS.pdf" );
        c22->cd();
        legd->Draw();
        c22->Print( "effPlotsPFCHS.pdf" );
        c23->cd();
        legd->Draw();
        c23->Print( "effPlotsPFCHS.pdf" );
    }


    c->Print( "effPlotsPFCHS.pdf]" );
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

