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

#endif
using namespace std;
// --------- MAIN -------------------
void DiPhotonMVA_Training( TString Nevent = "1000", TString Level = "DiPhoton", bool skipEvtWNoDiPho = true )
{
    // you must define $WORKSPACE first
    TString path;
    path = "{WORKSPACE}/test_dipho_training/";
    path = "test_dipho_training/";
    bool useDiphotonPt = 0;
    bool usePhotonsPt = true;
    int nEvents = std::atoi( Nevent.Data() );

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

    float   weightS[5];
    float   weightB[4];
    weightB[0] = 0;
    weightB[1] = 4746.0 / nEvents; // DyJetsToLL
    weightB[2] = 17180.0 * 0.0379 / nEvents; //gamJets pt>40
    weightB[3] = 145400.0 * 0.001776 / nEvents; //gamJets pt  in 20->40

    weightS[0] = 0;
    weightS[1] = 43.92 * 2.28e-3 / nEvents; //ggH
    weightS[2] = 3.748 * 2.28e-3 / nEvents; //ttH
    weightS[3] = 2.2496 * 2.28e-3 / nEvents; //VBH
    weightS[4] = 0.5608 * 2.28e-3 / nEvents; //WZH

    //Create a new root output file
    string outputFileName;
    outputFileName = "Flashgg_DiPhoton";

    // -- reader
    TFile *outputFile = TFile::Open( ( outputFileName + ".root" ).c_str(), "RECREATE" );
    TMVA::Factory *factory = new TMVA::Factory( outputFileName.c_str(), outputFile,
            "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
    // -- variables
    factory->AddVariable( "leadptom" );
    factory->AddVariable( "subleadptom" );
    factory->AddVariable( "leadmva" );
    factory->AddVariable( "subleadmva" );
    factory->AddVariable( "leadeta" );
    factory->AddVariable( "subleadeta" );
    factory->AddVariable( "sigmarv" );
    factory->AddVariable( "sigmawv" );
    factory->AddVariable( "CosPhi" );
    factory->AddVariable( "vtxprob" );

    // ====== register trees ====================================================
    factory->AddSignalTree( treeS1, weightS[1] );
    factory->AddSignalTree( treeS2, weightS[2] );
    factory->AddSignalTree( treeS3, weightS[3] );
    factory->AddSignalTree( treeS4, weightS[4] );


    factory->AddBackgroundTree( treeB1, weightB[2] );
    factory->AddBackgroundTree( treeB2, weightB[3] );
    factory->AddBackgroundTree( treeB3, weightB[1] );

    // == supress the the negative points on the input variables
    // == this high correlation between variables
    TCut mycuts = ""; // " leadPho_PToM > (60./120.) && sublPho_PToM> (30./120.)";
    TCut mycutb = ""; // " leadPho_PToM> (60./120.) && sublPho_PToM> (30./120.)";
    // tell the factory to use all remaining events in the trees after training for testing:
    factory->PrepareTrainingAndTestTree( mycuts, mycutb,
                                         "SplitMode=Random:NormMode=NumEvents:!V" );
    // Boosted Decision Trees: use BDTG( Gradient Boost )
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
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
