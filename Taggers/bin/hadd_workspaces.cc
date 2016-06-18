// Benoit Courbon
// https://github.com/bcourbon/flashgg/blob/132cd5163b3564da81b79b91829e9aec47126b78/TagAlgos/bin/hadd_workspaces.cc

#include "TDirectoryFile.h"
#include <string>
#include <iostream>
#include "../src/WorkspaceCombiner.cc"
#include <vector>


int main( int argc, char *argv[] )
{
    bool doTreesAndHistograms = false;

    std::vector<string> input;
    std::string outputfile = argv[1];

    for( int f = 2; f < argc; f++ ) { input.push_back( argv[f] ); }

    WorkspaceCombiner merger;

    merger.Init( outputfile, input );

    cout << endl << "Initialization" << endl << endl;

    merger.GetWorkspaces( merger.GetFirstFile() );

    cout << endl << " Got workspaces " << endl << endl;

    merger.MergeWorkspaces();

    if( doTreesAndHistograms ) {
        cout << endl << " Merging trees and histos " << endl << endl;
        merger.GetTreesAndHistograms( merger.MergeTreesAndHistograms() );
        cout << endl << " Saving workspaces, histograms and trees " << endl << endl;
    } else {
        cout << endl << " Skipping trees and histos, saving workspaces only" << endl << endl;
    }

    merger.Save( doTreesAndHistograms );
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
