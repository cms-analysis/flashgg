#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "PhysicsTools/Utilities/macros/setTDRStyle.C"

#include "flashgg/Validation/interface/drawUtils.h"

using namespace std;

void GetAllUntaggedCat( vector<string> *categories )
{
    categories->push_back( "dipho_mva_cat0" );
    categories->push_back( "dipho_mva_cat1" );
    categories->push_back( "dipho_mva_cat2" );
    categories->push_back( "dipho_mva_cat3" );
    categories->push_back( "dipho_mva_cat4" );
    categories->push_back( "dipho_mva_cat5" );
    categories->push_back( "dipho_mva_cat6" );
    categories->push_back( "dipho_mva_cat7" );
    categories->push_back( "dipho_mva_cat8" );
    categories->push_back( "dipho_mva_cat9" );
    categories->push_back( "dipho_mva_cat10" );
    categories->push_back( "dipho_mva_cat11" );
    categories->push_back( "dipho_mva_cat12" );
    categories->push_back( "dipho_mva_cat13" );
    categories->push_back( "dipho_mva_cat14" );

    return;
}

void GetAllVBFCat( vector<string> *categories )
{
    categories->push_back( "vbfcat0" );
    categories->push_back( "vbfcat1" );
    categories->push_back( "vbfcat2" );

    return;
}

//----------Variables and categories loop function----------------------------------------
void ProcessSample( TTree *flashggTree, TTree *globeTree,
                    vector<string> *variables, vector<string> *categories,
                    TString *drawOption, string outputDir, string sampleName, bool doSplit )
{
    //---create outdir if not exists
    std::string mkdir_command = "mkdir -p " + outputDir + sampleName;
    system( mkdir_command.c_str() );

    TFile *outFile = TFile::Open( ( outputDir + sampleName + "/comparison_plots.root" ).c_str(), "recreate" );
    //---variables loop---
    for( unsigned int iVar = 0; iVar < variables->size(); iVar++ ) {
        istringstream splitter( variables->at( iVar ) );
        string variable;
        int nbins = 100;
        float min = 0, max = 100;
        vector<string> tokens{istream_iterator<string>{splitter},
                              istream_iterator<string>{}};

        variable = tokens.at( 0 );
        if( tokens.size() > 1 )
        { nbins = stoi( tokens.at( 1 ) ); }
        if( tokens.size() > 2 )
        { min = stof( tokens.at( 2 ) ); }
        if( tokens.size() > 3 )
        { max = stof( tokens.at( 3 ) ); }

        //---categories loop---
        for( unsigned int iCat = 0; iCat < categories->size(); iCat++ ) {
            //---draw vbf var only for vbf categories
            if( variable.find( "dijet" ) != string::npos &&
                    categories->at( iCat ).find( "vbf" ) == string::npos &&
                    categories->at( iCat ) != "all_cat" )
            { continue; }

            outFile->cd();
            string h_name = categories->at( iCat ) + "_" + variable;
            TH1F *h_new = new TH1F( TString( h_name + "_FLASHgg" ), h_name.c_str(), nbins, min, max );
            TH1F *h_old =  new TH1F( TString( h_name + "_GLOBE" ), h_name.c_str(), nbins, min, max );

            TString cat_cut( categories->at( iCat ) );
            //---do not split in categories if cat_string is void
            if( categories->at( iCat ) != "all_cat" )
            { cat_cut.Insert( cat_cut.Last( 't' ) + 1, "==" ); }
            else
            { cat_cut = "1"; }
            //---Draw histos from trees
            flashggTree->Draw( TString( variable + ">>" + h_name + "_FLASHgg" ), cat_cut, "goff" );
            globeTree->Draw( TString( variable + ">>" + h_name + "_GLOBE" ),
                             TString( "full_weight*(" + cat_cut + ")" ), "goff" );

            //---Draw histos to output files---
            //---skip empty categories
            if( h_new->GetEntries() == 0 && h_old->GetEntries() == 0 )
            { continue; }
            //---output root file
            if( drawOption->Contains( "root" ) ) {
                h_new->Write();
                h_old->Write();
            }
            //---draw png/pdf plots
            string plotsDir = outputDir + sampleName + "/";
            if( doSplit )
            { plotsDir += categories->at( iCat ) + "/"; }
            if( drawOption->Contains( "png" ) || drawOption->Contains( "pdf" ) )
                compareHistos( h_new, h_old, string( "FLASHgg" ), string( "GLOBE" ),
                               h_name, plotsDir, drawOption );
        }
    }
    outFile->Close();
}

//**********MAIN**************************************************************************
int main( int argc, char *argv[] )
{
    setTDRStyle();
    gStyle->SetPadTickX( 1 );
    gStyle->SetPadTickY( 1 );
    gStyle->SetOptTitle( 0 );
    gStyle->SetOptFit( 1 );

    gSystem->Load( "libFWCoreFWLite" );
    AutoLibraryLoader::enable();

    map<string, pair<TH1F *, TH1F *> > histos_container;

    if( argc < 2 ) {
        cout << "Usage : " << argv[0] << " [parameters.py]" << endl;
        return 0;
    }
    if( !edm::readPSetsFrom( argv[1] )->existsAs<edm::ParameterSet>( "process" ) ) {
        cout << " ERROR: ParametersSet 'process' is missing in your configuration file"
             << endl;
        return 0;
    }
    //-----get the python configuration-----
    const edm::ParameterSet &process = edm::readPSetsFrom( argv[1] )->getParameter<edm::ParameterSet>( "process" );
    const edm::ParameterSet &filesOpt = process.getParameter<edm::ParameterSet>( "ioFilesOpt" );
    //---out file option---
    string outputDir = filesOpt.getParameter<string>( "outDir" );
    if( outputDir == "" )
    { outputDir = "$CMSSW_BASE/src/flashgg/GlobeComparison/plots/"; }
    //---draw option---
    TString drawOption( filesOpt.getParameter<string>( "drawOpt" ) );
    //---categories option---
    vector<string> categories;
    const edm::ParameterSet &cat_param = process.getParameter<edm::ParameterSet>( "categories" );
    bool doSplit = cat_param.getParameter<bool>( "doSplit" );
    //---untagged
    vector<string> untagged_cat;
    if( doSplit && cat_param.exists( "untagged" ) )
    { untagged_cat = cat_param.getParameter<vector<string> >( "untagged" ); }
    if( untagged_cat.size() > 0 && untagged_cat.at( 0 ) == "all" )
    { GetAllUntaggedCat( &categories ); }
    else {
        for( unsigned int iStr = 0; iStr < untagged_cat.size(); iStr++ )
        { categories.push_back( untagged_cat.at( iStr ) ); }
    }
    //---vbf
    vector<string> vbf_cat;
    if( doSplit && cat_param.exists( "vbf" ) )
    { vbf_cat = cat_param.getParameter<vector<string> >( "vbf" ); }
    if( vbf_cat.size() > 0 && ( vbf_cat.at( 0 ) == "all" || vbf_cat.at( 0 ) != "" ) )
    { GetAllVBFCat( &categories ); }
    else {
        for( unsigned int iStr = 0; iStr < vbf_cat.size(); iStr++ )
        { categories.push_back( vbf_cat.at( iStr ) ); }
    }
    //---do not split opotion
    if( categories.size() == 0 )
    { categories.push_back( "all_cat" ); }
    //---get the validation variables---
    const edm::ParameterSet &var_param = process.getParameter<edm::ParameterSet>( "variables" );
    vector<string> variables( var_param.getParameter<vector<string> >( "variables" ) );
    const edm::ParameterSet &samplesOpt = process.getParameter<edm::ParameterSet>( "samplesOpt" );
    bool processGGH = samplesOpt.getParameter<bool>( "GGH" );
    bool processVBF = samplesOpt.getParameter<bool>( "VBF" );
    bool processWZH = samplesOpt.getParameter<bool>( "WZH" );
    bool processTTH = samplesOpt.getParameter<bool>( "TTH" );
    //---process selected samples---
    TFile *globeFile = TFile::Open( filesOpt.getParameter<string>( "globeFile" ).c_str() );
    if( processGGH ) {
        TFile *flashggFileGGH = TFile::Open( filesOpt.getParameter<string>( "flashggFileGGH" ).c_str() );
        TTree *flashggTreeGGH = ( TTree * )flashggFileGGH->Get( filesOpt.getParameter<string>( "flashggTreeName" ).c_str() );
        TTree *globeTreeGGH = ( TTree * )globeFile->Get( "ggh_m125_8TeV" );
        ProcessSample( flashggTreeGGH, globeTreeGGH, &variables, &categories, &drawOption,
                       outputDir, string( "ggh_m125" ), doSplit );
    }
    if( processVBF ) {
        TFile *flashggFileVBF = TFile::Open( filesOpt.getParameter<string>( "flashggFileVBF" ).c_str() );
        TTree *flashggTreeVBF = ( TTree * )flashggFileVBF->Get( filesOpt.getParameter<string>( "flashggTreeName" ).c_str() );
        TTree *globeTreeVBF = ( TTree * )globeFile->Get( "vbf_m125_8TeV" );
        ProcessSample( flashggTreeVBF, globeTreeVBF, &variables, &categories, &drawOption,
                       outputDir, string( "vbf_m125" ), doSplit );
    }
    if( processWZH ) {
        TFile *flashggFileWZH = TFile::Open( filesOpt.getParameter<string>( "flashggFileWZH" ).c_str() );
        TTree *flashggTreeWZH = ( TTree * )flashggFileWZH->Get( filesOpt.getParameter<string>( "flashggTreeName" ).c_str() );
        TTree *globeTreeWZH = ( TTree * )globeFile->Get( "wzh_m125_8TeV" );
        ProcessSample( flashggTreeWZH, globeTreeWZH, &variables, &categories, &drawOption,
                       outputDir, string( "wzh_m125" ), doSplit );
    }
    if( processTTH ) {
        TFile *flashggFileTTH = TFile::Open( filesOpt.getParameter<string>( "flashggFileTTH" ).c_str() );
        TTree *flashggTreeTTH = ( TTree * )flashggFileTTH->Get( filesOpt.getParameter<string>( "flashggTreeName" ).c_str() );
        TTree *globeTreeTTH = ( TTree * )globeFile->Get( "tth_m125_8TeV" );
        ProcessSample( flashggTreeTTH, globeTreeTTH, &variables, &categories, &drawOption,
                       outputDir, string( "tth_m125" ), doSplit );
    }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

