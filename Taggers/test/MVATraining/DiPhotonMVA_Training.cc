///==== include ====
#include "TFile.h"
#include "TChain.h"
#include "TMinuit.h"
#include <sstream>
#include <iostream>
#include "TMVA/Factory.h"
#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

//#include "TMVAGui.C"

#endif
using namespace std;
// --------- MAIN -------------------
void DiPhotonMVA_Training( TString Nevent = "1000", TString Level = "DiPhoton", bool skipEvtWNoDiPho = true )
{
    // you must define $WORKSPACE first
    TString path;
//    path = "WORKSPACE}/test_dipho_training/";
      path = "test_dipho_training/";
//    bool useDiphotonPt = 0;
//    bool usePhotonsPt = true;


    TFile *inputS1 = TFile::Open( path + "output_GluGluToHToGG_M-125_13TeV-powheg-pythia6_numEvent" + Nevent + "_histos.root" );
    TFile *inputS2 = TFile::Open( path + "output_TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola_numEvent" + Nevent + "_histos.root" );
    TFile *inputS3 = TFile::Open( path + "output_VBF_HToGG_M-125_13TeV-powheg-pythia6_numEvent" + Nevent + "_histos.root" );
    TFile *inputS4 = TFile::Open( path + "output_WH_ZH_HToGG_M-125_13TeV_pythia6_numEvent" + Nevent + "_histos.root" );

    TFile *inputB1 = TFile::Open( path + "output_GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6_numEvent" + Nevent + "_histos.root" );
    TFile *inputB2 = TFile::Open( path + "output_GJet_Pt20to40_doubleEMEnriched_TuneZ2star_13TeV-pythia6_numEvent" + Nevent + "_histos.root" );
    TFile *inputB3 = TFile::Open( path + "output_DYJetsToLL_M-50_13TeV-madgraph-pythia8_numEvent" + Nevent + "_histos.root" );

    TTree *treeS1 = ( TTree * )inputS1->Get( Level + "MVADumperNew/trees/ggh_m125_13TeV_All" );
    TTree *treeS2 = ( TTree * )inputS2->Get( Level + "MVADumperNew/trees/tth_m125_13TeV_All" );
    TTree *treeS3 = ( TTree * )inputS3->Get( Level + "MVADumperNew/trees/vbf_m125_13TeV_All" );
    TTree *treeS4 = ( TTree * )inputS4->Get( Level + "MVADumperNew/trees/wzh_m125_13TeV_All" );

    TTree *treeB1 = ( TTree * )inputB1->Get( Level + "MVADumperNew/trees/gamJet_13TeV_All" );
    TTree *treeB2 = ( TTree * )inputB2->Get( Level + "MVADumperNew/trees/gamJet_13TeV_All" ); 
    TTree *treeB3 = ( TTree * )inputB3->Get( Level + "MVADumperNew/trees/dyJets_13TeV_All" );

    // Declaration of leaf types

    //float ph1.pt/mass;
    //float ph2.pt/mass;
    //float ph1.idmva
    //float ph2.idmva; 
    //float ph1.eta;
    //float ph2.eta;
    //float masserrsmeared/mass;
    //float masserrsmearedwrongvtx/mass;
    //float TMath::CosPhi(ph1.phi-ph2.phi);
    //float vtxprob;

    //    treeS1->SetBranchAddress("leadptom" ,&ph1.pt/mass);
    //    treeS1->SetBranchAddress("subleadptom" ,&ph2.pt/mass);
    //    treeS1->SetBranchAddress("leadmva" ,&ph1.idmva);
    //    treeS1->SetBranchAddress("subleadmva" ,&ph2.idmva);
    //    treeS1->SetBranchAddress("leadeta" ,&ph1.eta);
    //    treeS1->SetBranchAddress("subleadeta" ,&ph2.eta);
    //    treeS1->SetBranchAddress("sigmarv" ,&masserrsmeared/mass);
    //    treeS1->SetBranchAddress("sigmawv" ,&masserrsmearedwrongvtx/mass);
    //    treeS1->SetBranchAddress("CosPhi" ,&TMath::Cos(ph1.phi-ph2.phi));
    //    treeS1->SetBranchAddress("vtxprob" ,&vtxprob);

    // Create a new root output file.
    string outputFileName;
        outputFileName = "Flashgg_DiPhoton";

    // -- reader
    TFile *outputFile = TFile::Open( ( outputFileName + ".root" ).c_str(), "RECREATE" );
    TMVA::Factory *factory = new TMVA::Factory( outputFileName.c_str(), outputFile,
            "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
    // -- variables
        factory->AddVariable( "leadptom");
        factory->AddVariable( "subleadptom");
        factory->AddVariable( "leadmva");
        factory->AddVariable( "subleadmva");
        factory->AddVariable( "leadeta");
        factory->AddVariable( "subleadeta");
        factory->AddVariable( "sigmarv");
        factory->AddVariable( "sigmawv");
        factory->AddVariable( "CosPhi");
        factory->AddVariable( "vtxprob");



//        factory->AddVariable( "sigmarv" );
//        factory->AddVariable( "sigmawv" );
//        factory->AddVariable( "vtxprob" );
//        factory->AddVariable( "leadptom" );
//        factory->AddVariable( "subleadptom" );
//        factory->AddVariable( "leadeta" );
//        factory->AddVariable( "subleadeta" );
//        factory->AddVariable( "CosPhi" );
//        factory->AddVariable( "leadmva" );
//        factory->AddVariable( "subleadmva" );



    //event weights per tree (see below for setting event-wise weights)
    Double_t signalWeight = 1.0;
    Double_t backgroundWeight = 1.0;


    // ====== register trees ====================================================
    factory->AddSignalTree( treeS1, signalWeight );
    factory->AddSignalTree( treeS2, signalWeight );
    factory->AddSignalTree( treeS3, signalWeight );
    factory->AddSignalTree( treeS4, signalWeight );


    factory->AddBackgroundTree( treeB1, backgroundWeight );
    factory->AddBackgroundTree( treeB2, backgroundWeight );
    factory->AddBackgroundTree( treeB3, backgroundWeight );


    // == supress the the negative points on the input variables
    // == this high correlation between variables
    TCut mycuts = ""; // " leadPho_PToM > (60./120.) && sublPho_PToM> (30./120.)";
    TCut mycutb = ""; // " leadPho_PToM> (60./120.) && sublPho_PToM> (30./120.)";
    //if( skipEvtWNoVBF ) {
       // mycuts = "dipho_PToM>=0"; // Skip the event with -999
        //mycutb = "dipho_PToM>=0"; //
    //}

    // tell the factory to use all remaining events in the trees after training for testing:
    factory->PrepareTrainingAndTestTree( mycuts, mycutb,
                                         "SplitMode=Random:NormMode=NumEvents:!V" );
    // Boosted Decision Trees: use BDTG ( Gradient Boost )
    factory->BookMethod( TMVA::Types::kBDT, "BDTG",
                         "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:NNodesMax=5:MaxDepth=3" );
    factory->BookMethod( TMVA::Types::kBDT, "BDT",
                         "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
    //"!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:NNodesMax=15:MaxDepth=5" );
    // book Cuts
    //factory->BookMethod( TMVA::Types::kCuts, "CutsGA",
    // "H:!V:FitMethod=GA:CutRangeMin[0]=20:CutRangeMax[0]=500:CutRangeMin[1]=20:CutRangeMax[1]=500:VarProp=FSmart:VarProp[4]=FMin:EffSel:Steps=30:Cycles=3:PopSize=500:SC_steps=10:SC_rate=5:SC_factor=0.95" );
    // ---- Now you can tell the factory to train, test, and evaluate the MVAs
    // Train MVAs using the set of training events
    factory->TrainAllMethods();
    // ---- Evaluate all MVAs using the set of test events
    factory->TestAllMethods();
    // ----- Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();
    // --------------------------------------------------------------
    // Save the output
    outputFile->Close();
    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;
    delete factory;

//	if (!gROOT->IsBatch()) TMVAGui( (outputFileName+".root").c_str() );
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

