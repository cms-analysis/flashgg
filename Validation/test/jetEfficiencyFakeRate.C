{
    //TFile *_file0 = TFile::Open("jetValidationCollection_PU40bx50.root");//Open File
    TFile *_file0 = TFile::Open( "/afs/cern.ch/work/s/sethzenz/public/72x_jetValidationCollection_PU40bx50.root" ); //Open File



    std::string bxList[2] = {"PU40bx50", "PU20bx25"};
    std::string jetColList[5] = {"PF", "PFCHS0", "PFCHSLeg", "PUPPI0", "PUPPILeg"};

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

    //TFile *_file1 = TFile::Open("jetValidationCollection_PU20bx25.root");//Open File
    TFile *_file1 = TFile::Open( "/afs/cern.ch/work/s/sethzenz/public/72x_jetValidationCollection_PU20bx25.root" ); //Open File
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
            //	TGraphAsymmErrors *graphList50_EtaFake[cutLoop+3*treeLoop] = new  TGraphAsymmErrors(num,den);
            graph50 ->GetXaxis()->SetTitle( "#eta" );
            graph50 ->SetLineColor( kRed );

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
            graph25->SetLineColor( kBlue );

            graph50 ->Draw();
            graph25 ->Draw( "same" );
            //graphList25_EtaFake[cutLoop+3*treeLoop] = (TGraphAsymmErrors*)graph25->Clone();
            //graphList50_EtaFake[cutLoop+3*treeLoop] = (TGraphAsymmErrors*)graph50->Clone();
            dummy50 =	( graph50 );
            dummy25 =	( graph25 );
            TLegend *leg = new TLegend( 0.1, 0.8, 0.48, 0.9 );
            leg->AddEntry( dummy50 , "50bx, 40pu", "LEP" );
            leg->AddEntry( dummy25 , "25bx, 20pu", "LEP" );
            leg->Draw();

            //	//std::cout << "DEBUG 8 "<< std::endl;

            c->Print( "effPlots.pdf" );
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
            graph50->SetLineColor( kRed );

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
            graph25->SetLineColor( kBlue );
            c->Clear();
            graph50->Draw();
            graph25->Draw( "same" );

            TLegend *leg = new TLegend( 0.1, 0.8, 0.48, 0.9 );
            leg->AddEntry( graph50, "50bx, 40pu", "LEP" );
            leg->AddEntry( graph25, "25bx, 20pu", "LEP" );
            leg->Draw();
            c->Print( "effPlots.pdf" );

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
            graph50->SetLineColor( kRed );

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
            graph25->SetLineColor( kBlue );
            c->Clear();
            graph50->Draw();
            graph25->Draw( "same" );

            TLegend *leg = new TLegend( 0.1, 0.8, 0.48, 0.9 );
            leg->AddEntry( graph50, "50bx, 40pu", "LEP" );
            leg->AddEntry( graph25, "25bx, 20pu", "LEP" );
            leg->Draw();
            c->Print( "effPlots.pdf" );


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
            graph50->SetLineColor( kRed );

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
            graph25->SetLineColor( kBlue );
            c->Clear();
            graph50->Draw();
            graph25->Draw( "same" );

            TLegend *leg = new TLegend( 0.1, 0.8, 0.48, 0.9 );
            leg->AddEntry( graph50, "50bx, 40pu", "LEP" );
            leg->AddEntry( graph25, "25bx, 20pu", "LEP" );
            leg->Draw();
            c->Print( "effPlots.pdf" );

        }

    }

    for( int i = 0; i < 15; i++ )
    {


//	c->Clear();
//graphList25_EtaFake[i]->Draw("same");

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

