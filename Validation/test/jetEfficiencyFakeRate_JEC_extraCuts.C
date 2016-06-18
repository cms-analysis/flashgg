{
    //TFile *_file0 = TFile::Open("jetValidationCollection_ggH_JEC_small.root");//Open File
    TFile *_file0 = TFile::Open( "/afs/cern.ch/work/y/yhaddad/jetValidationTrees_VBF_HToGG_PU20bx25.root" ); //Open File
    //TFile *_file0 = TFile::Open("/afs/cern.ch/work/s/sethzenz/public/72x_jetValidationCollection_PU40bx50.root");//Open File
    //TFile *_file0 = TFile::Open("/afs/cern.ch/work/s/sethzenz/public/jetValidationCollection_70x_GluGluToHgg_PU20bx25.root");//Open File

    int color = 0;

    std::string bxList[2] = {"PU40bx50", "PU20bx25"};
    std::string jetColList[5] = {"PF", "PFCHS0", "PFCHSLeg", "PUPPI0", "PUPPILeg"};
    std::string etaList[5] = {"Inner", "Ears", "Outer"};
    std::string ptList[4] = {"lowest", "low", "mid", "high"};
    //TCut extraCut="legacyEqZeroth==1 && nDiphotons>1";
    TCut extraCut = "!(photonMatch==1 && GenPhotonPt/bestPt > 0.5 && photondRmin < 0.2)";
    TCut numCut = "passesPUJetID>0";
    std::cout << extraCut.GetName() << std::endl;


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

    TFile *_file1 = TFile::Open( "/afs/cern.ch/work/y/yhaddad/jetValidationTrees_VBF_HToGG_PU20bx25.root" ); //Open File
    //TFile *_file1 = TFile::Open("/afs/cern.ch/work/s/sethzenz/public/72x_jetValidationCollection_PU20bx25.root");//Open File
    //TFile *_file1 = TFile::Open("/afs/cern.ch/work/s/sethzenz/public/jetValidationCollection_72xMiniAOD_GluGluToHgg_PU20bx25.root");//Open File
    TTree *jetTree25[0] = ( TTree * )flashggJetValidationTreeMaker->Get( "jetTree_PF" );
    std::cout << "entriess " <<  jetTree25[0]->GetEntries() << std::endl;
    TTree *jetTree25[1] = ( TTree * )flashggJetValidationTreeMakerPFCHS0->Get( "jetTree_PFCHS0" );
    TTree *jetTree25[2] = ( TTree * )flashggJetValidationTreeMakerPFCHSLeg->Get( "jetTree_PFCHSLeg" );
    TTree *jetTree25[3] = ( TTree * )flashggJetValidationTreeMakerPUPPI0->Get( "jetTree_PUPPI0" );
    TTree *jetTree25[4] = ( TTree * )flashggJetValidationTreeMakerPUPPILeg->Get( "jetTree_PUPPILeg" );

    TTree *genJetTree25[0] = ( TTree * )flashggJetValidationTreeMaker->Get( "genJetTree_PF" );
    TTree *genJetTree25[1] = ( TTree * )flashggJetValidationTreeMakerPFCHS0->Get( "genJetTree_PFCHS0" );
    TTree *genJetTree25[2] = ( TTree * )flashggJetValidationTreeMakerPFCHSLeg->Get( "genJetTree_PFCHSLeg" );
    TTree *genJetTree25[3] = ( TTree * )flashggJetValidationTreeMakerPUPPI0->Get( "genJetTree_PUPPI0" );
    TTree *genJetTree25[4] = ( TTree * )flashggJetValidationTreeMakerPUPPILeg->Get( "genJetTree_PUPPILeg" );

    TCanvas *c = new TCanvas( "c", "c", 500, 500 );
    TCanvas *ca  = new TCanvas( "ca", "ca ", 500, 500 );
    TCanvas *cb  = new TCanvas( "cb", "cb ", 500, 500 );
    TCanvas *cc  = new TCanvas( "cc", "cc ", 500, 500 );
    TCanvas *cd  = new TCanvas( "cd", "cd ", 500, 500 );
    TCanvas *c0  = new TCanvas( "c0", "c0 ", 500, 500 );
    TCanvas *c1  = new TCanvas( "c1", "c1 ", 500, 500 );
    TCanvas *c2  = new TCanvas( "c2", "c2 ", 500, 500 );
    TCanvas *c3  = new TCanvas( "c3", "c3 ", 500, 500 );
    TCanvas *c4  = new TCanvas( "c4", "c4 ", 500, 500 );
    TCanvas *c5  = new TCanvas( "c5", "c5 ", 500, 500 );
    TCanvas *c6  = new TCanvas( "c6", "c6 ", 500, 500 );
    TCanvas *c7  = new TCanvas( "c7", "c7 ", 500, 500 );
    TCanvas *c8  = new TCanvas( "c8", "c8 ", 500, 500 );
    TCanvas *c9  = new TCanvas( "c9", "c9 ", 500, 500 );
    TCanvas *c10 = new TCanvas( "c10", "c10", 500, 500 );
    TCanvas *c11 = new TCanvas( "c11", "c11", 500, 500 );
    TCanvas *c12 = new TCanvas( "c12", "c12", 500, 500 );
    TCanvas *c13 = new TCanvas( "c13", "c13", 500, 500 );
    TCanvas *c14 = new TCanvas( "c14", "c14", 500, 500 );
    TCanvas *c15 = new TCanvas( "c15", "c15", 500, 500 );
    TCanvas *c16 = new TCanvas( "c16", "c16", 500, 500 );
    TCanvas *c17 = new TCanvas( "c17", "c17", 500, 500 );
    TCanvas *c18 = new TCanvas( "c18", "c18", 500, 500 );
    TCanvas *c19 = new TCanvas( "c19", "c19", 500, 500 );
    TCanvas *c20 = new TCanvas( "c20", "c20", 500, 500 );
    TCanvas *c21 = new TCanvas( "c21", "c21", 500, 500 );
    TCanvas *c22 = new TCanvas( "c22", "c22", 500, 500 );
    TCanvas *c23 = new TCanvas( "c23", "c23", 500, 500 );
    TCut cutStringsEta[3] = {"abs(eta)<2.5", "abs(eta)>3", " !(abs(eta)<2.5) && !(abs(eta)>3)"};
    TCut cutStringsBestPt[4] = {"bestPt > 20 && bestPt < 30", "bestPt > 30 && bestPt < 50", "bestPt > 50 && bestPt < 80", " bestPt >80"};
    TCut cutStringsPt[4] = {"pt > 20 && pt < 30", "pt > 30 && pt < 50", "pt > 50 && pt < 80", " pt >80"};
    std::string cutStringsPtText[4] = {"pt > 20 && pt < 30", "pt > 30 && pt < 50", "pt > 50 && pt < 80", " pt >80"};
    std::string cutStringsEtaText[3] = {"fabs(eta)<2.5", "fabs(eta)>3", " 2.5<fabs(eta)<3 "};
    TCut bestPtCut = "bestPt>20";
    TCut ptCut = "pt>20";
    TCut genMatch = "genJetMatch";
    TCut recoMatch = "recoJetMatch";
    //std::string andand =" && ";
    c->Print( "effPlotsJEC.pdf[" );
    TGraphAsymmErrors *dummy50;
    TGraphAsymmErrors *dummy25;
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




    TLegend *legu = new TLegend( 0.1, 0.8, 0.48, 0.9 );
    legu->AddEntry( dummy0 , "pf", "LEP" );
    legu->AddEntry( dummy1 , "pfchs0", "LEP" );
    legu->AddEntry( dummy2 , "pfchsLeg", "LEP" );
    //legu->AddEntry(dummy3 ,"f0dz1","LEP");
    //	legu->AddEntry(dummy4 ,"f0dz2","LEP");
    //	legu->AddEntry(dummy5 ,"fpv1","LEP");
    //	legu->AddEntry(dummy6 ,"fpv2","LEP");
    //	legu->AddEntry(dummy7 ,"fpv3","LEP");
    TLegend *legd = new TLegend( 0.1, 0.1, 0.48, 0.2 );
    legd->AddEntry( dummy0 , "pf", "LEP" );
    legd->AddEntry( dummy1 , "pfchs0", "LEP" );
    legd->AddEntry( dummy2 , "pfchsLeg", "LEP" );
    //	legd->AddEntry(dummy3 ,"f0dz1","LEP");
    //	legd->AddEntry(dummy4 ,"f0dz2","LEP");
    //	legd->AddEntry(dummy5 ,"fpv1","LEP");
    //	legd->AddEntry(dummy6 ,"fpv2","LEP");
    //	legd->AddEntry(dummy7 ,"fpv3","LEP");

    TFile *t = new TFile( "graphs5.root", "recreate" );

    for( unsigned int treeLoop = 0; treeLoop < 3 ; treeLoop++ )
    {
        for( unsigned int cutLoop = 0 ; cutLoop < 4; cutLoop++ ) {


            std::cout << cutLoop + 3 * treeLoop << std::endl;
            //std::string cutStringText = cut0 + andand + cutStringsEtaText[cutLoop];
            //	TCut cutString = new TCut(cutStringText);

            //ETA plots (fake rates)
            jetTree50[treeLoop]->Draw( "eta>>htemp(50,-5,5)", numCut && extraCut && bestPtCut && cutStringsBestPt[cutLoop] && !genMatch );
            TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *num = ( TH1F * )htemp->Clone( "num" );
            num->SetTitle( ( "fake rate " + cutStringsPtText[cutLoop] + " " + jetColList[treeLoop] ).c_str() );
            jetTree50[treeLoop]->Draw( "eta>>htemp(50,-5,5)", extraCut &&  bestPtCut && cutStringsBestPt[cutLoop] );
            htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *den = ( TH1F * )htemp->Clone( "den" );
            TGraphAsymmErrors *graph50 = new TGraphAsymmErrors( num, den );
            graph50->GetYaxis()->SetRangeUser( 0., 1. );
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

            if( cutLoop == 3 ) {
                ca->cd();
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

            jetTree25[treeLoop]->Draw( "eta>>htemp(50,-5,5)", numCut && extraCut && bestPtCut && cutStringsBestPt[cutLoop] && !genMatch );
            TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *num = ( TH1F * )htemp->Clone( "num" );
            num->SetTitle( ( "fake rate " + cutStringsPtText[cutLoop] + " " + jetColList[treeLoop] ).c_str() );
            jetTree25[treeLoop]->Draw( "eta>>htemp(50,-5,5)", extraCut && bestPtCut && cutStringsBestPt[cutLoop] );
            htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *den = ( TH1F * )htemp->Clone( "den" );
            TGraphAsymmErrors *graph25 = new TGraphAsymmErrors( num, den );
            graph25->GetYaxis()->SetRangeUser( 0., 1. );

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
            if( cutLoop == 3 ) {
                cb->cd();
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

            if( color ) { c->Print( "effPlotsJEC.pdf" ); }
            c->Clear();

            ////	std::cout << "DEBUG 9 "<< std::endl;
            if( cutLoop != 3 ) {
                // PT plots (fake rates)
                jetTree50[treeLoop]->Draw( "bestPt>>htemp(50,0,250)", numCut && extraCut && bestPtCut && cutStringsEta[cutLoop] && !genMatch );
                TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
                TH1F *num = ( TH1F * )htemp->Clone( "num" );
                num->SetTitle( ( "fake rate " + cutStringsEtaText[cutLoop] + " " + jetColList[treeLoop] ).c_str() );
                jetTree50[treeLoop]->Draw( "bestPt>>htemp(50,0,250)", extraCut && bestPtCut && cutStringsEta[cutLoop] );
                htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
                TH1F *den = ( TH1F * )htemp->Clone( "den" );
                TGraphAsymmErrors *graph50 = new TGraphAsymmErrors( num, den );
                graph50->GetYaxis()->SetRangeUser( 0., 1. );

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
                jetTree25[treeLoop]->Draw( "bestPt>>htemp(50,0,250)", numCut && extraCut && ptCut && cutStringsEta[cutLoop] && !genMatch );
                TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
                TH1F *num = ( TH1F * )htemp->Clone( "num" );
                num->SetTitle( ( "fake rate " + cutStringsEtaText[cutLoop] ).c_str() );
                jetTree25[treeLoop]->Draw( "bestPt>>htemp(50,0,250)", extraCut && ptCut && cutStringsEta[cutLoop] );
                htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
                TH1F *den = ( TH1F * )htemp->Clone( "den" );
                TGraphAsymmErrors *graph25 = new TGraphAsymmErrors( num, den );

                graph25->GetYaxis()->SetRangeUser( 0., 1. );
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
                if( color ) { c->Print( "effPlotsJEC.pdf" ); }
            }

            // Eta Plots (eff)
            genJetTree50[treeLoop]->Draw( "eta>>htemp(50,-5,5)", numCut && extraCut && ptCut && cutStringsPt[cutLoop] && recoMatch );
            TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *num = ( TH1F * )htemp->Clone( "num" );
            num->SetTitle( ( "efficiency " + cutStringsPtText[cutLoop] + " " + jetColList[treeLoop] ).c_str() );
            genJetTree50[treeLoop]->Draw( "eta>>htemp(50,-5,5)", extraCut && ptCut && cutStringsPt[cutLoop] );
            htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *den = ( TH1F * )htemp->Clone( "den" );
            TGraphAsymmErrors *graph50 = new TGraphAsymmErrors( num, den );
            graph50->GetYaxis()->SetRangeUser( 0., 1. );

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
            if( cutLoop == 3 ) {
                cc->cd();
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

            genJetTree25[treeLoop]->Draw( "eta>>htemp(50,-5,5)", numCut && extraCut && ptCut && cutStringsPt[cutLoop] && recoMatch );
            TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *num = ( TH1F * )htemp->Clone( "num" );
            num->SetTitle( ( "efficiency " + cutStringsPtText[cutLoop] ).c_str() );
            genJetTree25[treeLoop]->Draw( "eta>>htemp(50,-5,5)", extraCut && ptCut && cutStringsPt[cutLoop] );
            htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
            TH1F *den = ( TH1F * )htemp->Clone( "den" );
            TGraphAsymmErrors *graph25 = new TGraphAsymmErrors( num, den );

            graph25->GetYaxis()->SetRangeUser( 0., 1. );
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
            if( cutLoop == 3 ) {
                cd->cd();
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
            if( color ) { c->Print( "effPlotsJEC.pdf" ); }


            if( cutLoop != 3 ) {
                // PT Plots (eff)
                genJetTree50[treeLoop]->Draw( "pt>>htemp(50,0,250)", numCut && extraCut && ptCut && cutStringsEta[cutLoop] && recoMatch );
                TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
                TH1F *num = ( TH1F * )htemp->Clone( "num" );
                num->SetTitle( ( "efficiency " + cutStringsEtaText[cutLoop] + " " + jetColList[treeLoop] ).c_str() );
                genJetTree50[treeLoop]->Draw( "pt>>htemp(50,0,250)", extraCut && ptCut && cutStringsEta[cutLoop] );
                htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
                TH1F *den = ( TH1F * )htemp->Clone( "den" );
                TGraphAsymmErrors *graph50 = new TGraphAsymmErrors( num, den );
                graph50->GetYaxis()->SetRangeUser( 0., 1. );

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

                genJetTree25[treeLoop]->Draw( "pt>>htemp(50,0,250)", numCut && extraCut && ptCut && cutStringsEta[cutLoop] && recoMatch );
                TH1F *htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
                TH1F *num = ( TH1F * )htemp->Clone( "num" );
                num->SetTitle( ( "efficiency " + cutStringsEtaText[cutLoop] ).c_str() );
                genJetTree25[treeLoop]->Draw( "pt>>htemp(50,0,250)", extraCut && ptCut && cutStringsEta[cutLoop] );
                htemp = ( TH1F * )gPad->GetPrimitive( "htemp" );
                TH1F *den = ( TH1F * )htemp->Clone( "den" );
                TGraphAsymmErrors *graph25 = new TGraphAsymmErrors( num, den );

                graph25->GetYaxis()->SetRangeUser( 0., 1. );
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
            if( color ) { c->Print( "effPlotsJEC.pdf" ) ; }

        }

    }
    if( !color )
    {
//	c0->Print("effPlotsJEC.pdf");
//	c1->Print("effPlotsJEC.pdf");
//	c2->Print("effPlotsJEC.pdf");
        c3->cd();
        legu->Draw();
        c3->Print( "effPlotsJEC.pdf" );
        c4->cd();
        legu->Draw();
        c4->Print( "effPlotsJEC.pdf" );
        c5->cd();
        legu->Draw();
        c5->Print( "effPlotsJEC.pdf" );
        cb->cd();
        legu->Draw();
        cb->Print( "effPlotsJEC.pdf" );
//	c6->Print("effPlotsJEC.pdf");
//	c7->Print("effPlotsJEC.pdf");
//	c8->Print("effPlotsJEC.pdf");
        c9->cd();
        legu->Draw();
        c9->Print( "effPlotsJEC.pdf" );
        c10->cd();
        legu->Draw();
        c10->Print( "effPlotsJEC.pdf" );
        c11->cd();
        legu->Draw();
        c11->Print( "effPlotsJEC.pdf" );
//	c12->Print("effPlotsJEC.pdf");
//	c13->Print("effPlotsJEC.pdf");
//	c14->Print("effPlotsJEC.pdf");
        c15->cd();
        legd->Draw();
        c15->Print( "effPlotsJEC.pdf" );
        c16->cd();
        legd->Draw();
        c16->Print( "effPlotsJEC.pdf" );
        c17->cd();
        legd->Draw();
        c17->Print( "effPlotsJEC.pdf" );
        cd->cd();
        legd->Draw();
        cd->Print( "effPlotsJEC.pdf" );
//	c18->Print("effPlotsJEC.pdf");
//	c19->Print("effPlotsJEC.pdf");
//	c20->Print("effPlotsJEC.pdf");
        c21->cd();
        legd->Draw();
        c21->Print( "effPlotsJEC.pdf" );
        c22->cd();
        legd->Draw();
        c22->Print( "effPlotsJEC.pdf" );
        c23->cd();
        legd->Draw();
        c23->Print( "effPlotsJEC.pdf" );
    }


    for( int i = 0; i < 15; i++ )
    {


//	c->Clear();
//graphList25_EtaFake[i]->Draw("same P");

//	c->Print("effPlots.pdf]");
    }

    c->Print( "effPlotsJEC.pdf]" );

}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

