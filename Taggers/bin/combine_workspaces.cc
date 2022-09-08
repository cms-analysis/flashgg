#include "TDirectoryFile.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../src/WorkspaceLinearCombiner.cc"
#include <vector>
#include <map>

int main( int argc, char *argv[] )
{
    char inputFileName[500];
    char outputFileName[500];
    if ( argc < 3 ){
        std::cout << "missing argument: pass the txt file with the list of files and coefficient with the format: f1.root coeff \n f2.root coeff \n ... and the name of the output file (combination.root)" << std::endl; 
        std::cout << "combine_workspaces files_and_coeffs_list.txt outputname.root" << std::endl;
        return 1;
    }
    strcpy(inputFileName,argv[1]);
    strcpy(outputFileName,argv[2]);

    std::ifstream inputFile(inputFileName);
    std::string   line;

    std::map<std::string, float> inputMap;

    std::cout << "Reading the input file list " << inputFileName << std::endl;
    while( std::getline(inputFile, line) ){
        std::stringstream   linestream(line);
        std::string         filename;
        float               coeff;
        linestream >> filename >> coeff;
        inputMap[filename] = coeff;
    }
    inputFile.close();
    
    std::cout << "Will merge the workspaces in the linear combination with name " << outputFileName << std::endl;
        
    WorkspaceLinearCombiner combiner;

    combiner.Init( string(outputFileName), inputMap );

    cout << endl << "Initialization" << endl << endl;

    combiner.GetWorkspaces( combiner.GetFirstFile() );

    cout << endl << " Got workspaces " << endl << endl;

    combiner.MergeWorkspaces();

    combiner.Save();
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
