// Benoit Courbon
// https://raw.githubusercontent.com/bcourbon/flashgg/132cd5163b3564da81b79b91829e9aec47126b78/TagAlgos/src/WorkspaceCombiner.cc

#include "../interface/WorkspaceCombiner.h"

using namespace std;
using namespace RooFit;

// ----------------------------------------------------------------------------------------------------

WorkspaceCombiner::WorkspaceCombiner()
{
}

// ----------------------------------------------------------------------------------------------------

WorkspaceCombiner::~WorkspaceCombiner()
{
}

// ----------------------------------------------------------------------------------------------------

void WorkspaceCombiner::Init( string outputFileName_, vector<string> inputfiles_ )
{

    inputFileNames.clear();
    workspaceNames.clear();
    workspacePaths.clear();
    data.clear();
    trees.clear();
    histos.clear();
    treePaths.clear();
    histoPaths.clear();

    for( unsigned int f = 0; f < inputfiles_.size(); f++ ) { inputFileNames.push_back( inputfiles_[f] ); }

    outputFileName = outputFileName_;

}

// ----------------------------------------------------------------------------------------------------

TDirectoryFile *WorkspaceCombiner::GetFirstFile()
{

    std::cout << "WorkspaceCombiner::GetFirstFile" << std::endl;
    std::cout << inputFileNames[0].c_str() << std::endl;
    TDirectoryFile *file0 = TFile::Open( inputFileNames[0].c_str() );
    return file0;

}

// ----------------------------------------------------------------------------------------------------
void WorkspaceCombiner::GetWorkspaces( TDirectoryFile *tdfile )
{
    std::cout << " WorkspaceCombiner::GetWorkspaces" << std::endl;
    tdfile->Print();
    TList *listofkeys = tdfile->GetListOfKeys();
    std::cout << "Got list of keys!" << std::endl;
    TKey *oldkey = 0;
    for( int k = 0; k < listofkeys->GetSize(); k++ ) {
        std::cout << k << std::endl;
        TKey *key = ( TKey * )listofkeys->At( k );
        std::cout << " Key: " << key->GetName() << " CycleNumber: " << key->GetCycle() << std::endl;
        if( oldkey && !strcmp( oldkey->GetName(), key->GetName() ) ) {
            std::cout << "    Duplicate key to previous, continuing (relying on order being predictable)" << std::endl;
            continue;
        }
        oldkey = key;
        if( strcmp( key->GetClassName(), "RooWorkspace" ) == 0 ) {
            std::cout << " We got a workspace with name " << key->GetName() << std::endl;
            RooWorkspace *work = ( RooWorkspace * )tdfile->Get( key->GetName() );
            //            work->Print();
            std::cout << " made work" << std::endl;
            std::list<RooAbsData *> allData = work->allData();
            std::cout << " made allData" << std::endl;
            vector<RooDataSet *> allDataClone;
            std::cout << " about to iterate over allData " << std::endl;
            for( std::list<RooAbsData *>::iterator it = allData.begin(); it != allData.end(); ++it ) {
                RooDataSet *dataset = dynamic_cast<RooDataSet *>( *it );
                allDataClone.push_back( ( RooDataSet * )dataset->Clone() );
            }
            std::cout << " gonna push back allDataClone " << std::endl;
            data.push_back( allDataClone );
            std::cout << " gonna push back work " << std::endl;
            workspaceNames.push_back( work->GetName() );
            string workpath = "";
            std::cout << " gonna build workpath" << std::endl;
            while( tdfile->InheritsFrom( "TFile" ) == false ) {
                workpath.insert( 0, Form( "/%s", tdfile->GetName() ) );
                tdfile = ( TDirectoryFile * )tdfile->GetMotherDir();
            }
            workpath.erase( 0, 1 );
            std::cout << "workpath is " << workpath << std::endl;
            workspacePaths.push_back( workpath );
            delete work;
        }
        std::cout << " before TDirectoryFile check" << std::endl;
        if( strcmp( key->GetClassName(), "TDirectoryFile" ) == 0 ) {
            std::cout << " We're gonna make a new TDirectoryFile with name " << key->GetName() << " (cycle " << key->GetCycle() << ")" << std::endl;
            TDirectoryFile *newdirectory = ( TDirectoryFile * )tdfile->Get( key->GetName() );
            if( newdirectory ) {
                std::cout << " About to recursively call GetWorkspaces on " << newdirectory << std::endl;
                GetWorkspaces( newdirectory );
            } else {
                std::cout << " That returned null for some reason. Skipping." << std::endl;
            }
        }
    }
}

// ----------------------------------------------------------------------------------------------------

void WorkspaceCombiner::MergeWorkspaces()
{

    for( unsigned int f = 1; f < inputFileNames.size(); f++ ) {

        std::cout << " MergeWorkspaces f=" << f << std::endl;

        TFile *file = TFile::Open( inputFileNames[f].c_str() );

        cout << endl << "Combining Current File " << f << " / " << inputFileNames.size() << " - " << inputFileNames[f] << endl << endl;

        file->cd();

        cout << endl << "before workspaceNames loop" << endl << endl;
        for( unsigned int w = 0; w < workspaceNames.size(); w++ ) {
            std::cout << "   " << workspaceNames[w] << " " << workspacePaths[w]  << std::endl;
            RooWorkspace *work;
            if( workspacePaths[w] == "" ) { work = ( RooWorkspace * ) file->Get( workspaceNames[w].c_str() ); } //look into root folder
            else {										  //look into directory
                TDirectory *dir = ( TDirectory * ) file->Get( workspacePaths[w].c_str() );
                work = ( RooWorkspace * ) dir->Get( workspaceNames[w].c_str() );
            }
            //			work->Print();
            std::list<RooAbsData *> allData = work->allData();
            for( std::list<RooAbsData *>::iterator it = allData.begin(); it != allData.end(); ++it ) {
                unsigned int d = 0;
                RooDataSet *dataset = dynamic_cast<RooDataSet *>( *it );
                std::cout << "   Dataset name: " << dataset->GetName() << std::endl;
                for( d = 0 ; d < data[w].size() ; d++ ) {
                    if( data[w][d]->GetName() == dataset->GetName() ) {
                        data[w][d]->append( *dataset );
                        break;
                    }
                }
                if( d == data[w].size() ) { // no match
                    data[w].push_back( ( RooDataSet * )dataset->Clone() );
                }


                //  				data[w][d]->append(*dataset);
                //  				data[w][d]->Print();
                //				d++;
            }
        }
        cout << endl << "after workspaceNames loop" << endl << endl;


        file->Close();

        cout << endl << "Finished Combining File - " << inputFileNames[f] << endl << endl;

    }
}

// ----------------------------------------------------------------------------------------------------
TDirectoryFile *WorkspaceCombiner::MergeTreesAndHistograms()
{

    std::string inputchain = inputFileNames[0];
    for( unsigned int i = 1; i < inputFileNames.size(); i++ ) {
        inputchain.append( " " );
        inputchain.append( inputFileNames[i] );
    }

    outputAux = "outputTreesAndHistos.root";

    gSystem->Exec( Form( "hadd -f %s %s", outputAux.c_str(), inputchain.c_str() ) );

    TDirectoryFile *outputAuxFile = TFile::Open( outputAux.c_str() );

    return outputAuxFile;

}

// ----------------------------------------------------------------------------------------------------
void WorkspaceCombiner::GetTreesAndHistograms( TDirectoryFile *file )
{

    TList *listofkeys = file->GetListOfKeys();
    for( int k = 0; k < listofkeys->GetSize(); k++ ) {
        TKey *key = ( TKey * )listofkeys->At( k );
        if( strcmp( key->GetClassName(), "TTree" ) == 0 ) {
            TTree *tree = new TTree;
            tree = ( TTree * )file->Get( key->GetName() );
            trees.push_back( tree );
            string treepath = "";
            TDirectoryFile *fileclone = file;
            while( fileclone->InheritsFrom( "TFile" ) == false ) {
                treepath.insert( 0, Form( "/%s", fileclone->GetName() ) );
                fileclone = ( TDirectoryFile * )fileclone->GetMotherDir();
            }
            treepath.erase( 0, 1 );
            treePaths.push_back( treepath );
        }
        if( strcmp( key->GetClassName(), "TH1F" ) == 0 ) {
            TH1F *histo1 = ( TH1F * )file->Get( key->GetName() );
            histos.push_back( histo1 );
            string histopath = "";
            TDirectoryFile *fileclone = file;
            while( fileclone->InheritsFrom( "TFile" ) == false ) {
                histopath.insert( 0, Form( "/%s", fileclone->GetName() ) );
                fileclone = ( TDirectoryFile * )fileclone->GetMotherDir();
            }
            histopath.erase( 0, 1 );
            histoPaths.push_back( histopath );
        }
        if( strcmp( key->GetClassName(), "TH2F" ) == 0 ) {
            TH2F *histo2 = ( TH2F * )file->Get( key->GetName() );
            histos.push_back( histo2 );
            string histopath = "";
            TDirectoryFile *fileclone = file;
            while( fileclone->InheritsFrom( "TFile" ) == false ) {
                histopath.insert( 0, Form( "/%s", fileclone->GetName() ) );
                fileclone = ( TDirectoryFile * )fileclone->GetMotherDir();
            }
            histopath.erase( 0, 1 );
            histoPaths.push_back( histopath );
        }
        if( strcmp( key->GetClassName(), "TDirectoryFile" ) == 0 ) {
            TDirectoryFile *newdirectory = ( TDirectoryFile * )file->Get( key->GetName() );
            GetTreesAndHistograms( newdirectory );
        }
    }

}
// ----------------------------------------------------------------------------------------------------

void WorkspaceCombiner::Save( bool doTreesAndHistograms )
{

    TFile *outfile = TFile::Open( outputFileName.c_str(), "RECREATE", "ROOT file with merge workspace" );


    for( unsigned int w = 0; w < workspaceNames.size(); w++ ) {
        RooWorkspace *outputws = new RooWorkspace();
        outputws->SetName( workspaceNames[w].c_str() );
        for( unsigned int d = 0; d < data[w].size(); d++ ) {
            RooDataSet *already_there = ( RooDataSet * ) outputws->data( data[w][d]->GetName() );
            if( already_there ) {
                std::cout << " doing an append of " << data[w][d]->GetName() << " in WorkspaceCombiner::Save" << std::endl;
                already_there->append( *data[w][d] );
            } else {
                std::cout << " doing an explicit import of " << data[w][d]->GetName() << " in WorkspaceCombiner::Save" << std::endl;
                outputws->import( *data[w][d] );
            }
            //data[w][d]->Print();
        }
        if( outfile->GetDirectory( workspacePaths[w].c_str() ) == false ) { outfile->mkdir( workspacePaths[w].c_str() ); }
        outfile->cd( workspacePaths[w].c_str() );
        std::cout << " ABOUT TO WRITE OUTPUT WORKSPACE" << std::endl;
        outputws->Write();
        std::cout << " ABOUT TO DELETE OUTPUT WORKSPACE" << std::endl;
        //outputws->Print();
        delete outputws;
        std::cout << " DONE WITH WORKSPACE " << std::endl;

    }


    if( doTreesAndHistograms ) {
        for( unsigned int t = 0; t < trees.size(); t++ ) {
            if( outfile->GetDirectory( treePaths[t].c_str() ) == false ) { outfile->mkdir( treePaths[t].c_str() ); }
            outfile->cd( treePaths[t].c_str() );
            trees[t]->CloneTree()->Write();
            //trees[t]->CloneTree()->Print();
        }

        for( unsigned int h = 0; h < histos.size(); h++ ) {
            if( outfile->GetDirectory( histoPaths[h].c_str() ) == false ) { outfile->mkdir( histoPaths[h].c_str() ); }
            outfile->cd( histoPaths[h].c_str() );
            histos[h]->Write();
            //histos[h]->Print();
        }
    } else {
        std::cout << " SKIPPING TREES AND HISTOGRAMS IN SAVE" << std::endl;
    }

    std::cout << " ABOUT TO CLOSE OUTPUT FILE " << std::endl;

    outfile->Close();
    if( doTreesAndHistograms ) {
        gSystem->Exec( Form( "rm %s", outputAux.c_str() ) );
    }
    std::cout << " END OF WorkspaceCombiner::Save" << std::endl;
}

// ----------------------------------------------------------------------------------------------------


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
