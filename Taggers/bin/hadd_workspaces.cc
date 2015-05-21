// Benoit Courbon
// https://github.com/bcourbon/flashgg/blob/132cd5163b3564da81b79b91829e9aec47126b78/TagAlgos/bin/hadd_workspaces.cc

#include "TDirectoryFile.h"
#include <string>
#include <iostream>
#include "../src/WorkspaceCombiner.cc"
#include <vector>


int main( int argc, char *argv[] )
{

    std::string outputfile = argv[1];

    std::vector<string> input;

    for( int f = 2; f < argc; f++ ) { input.push_back( argv[f] ); }

    WorkspaceCombiner merger;

    merger.Init( outputfile, input );

    cout << endl << "Initialization" << endl << endl;

    merger.GetWorkspaces( merger.GetFirstFile() );

    merger.MergeWorkspaces();

    cout << endl << " Merging trees and histos " << endl << endl;

    merger.GetTreesAndHistograms( merger.MergeTreesAndHistograms() );

    cout << endl << " Saving workspaces, histograms and trees " << endl << endl;

    merger.Save();


}
