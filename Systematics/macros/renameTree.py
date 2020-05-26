import ROOT
from ROOT import TFile, TTree, TIter

filenames=["allGluGlu_120.root","allGluGlu_125.root","allGluGlu_130.root","allGluGlu_OA_120.root","allGluGlu_OA_125.root","allGluGlu_OA_130.root"]
prefix="ggh"

for filename in filenames:
    file= TFile(filename, "UPDATE")
    file.cd('tagsDumper/trees')
    file.ls()
    dir = file.GetDirectory('tagsDumper/trees')
    #nextkey = TIter( file.GetListOfKeys() )
    #key=1
    #oldkey=0
    #key = nextkey()
    #print key
    #while (key ):
    listofkeys = dir.GetListOfKeys()
    print listofkeys
    for key in listofkeys:
        obj = key.ReadObj()
        print obj
        if ( obj.IsA().InheritsFrom( "TTree" ) ):
            tree= obj
            print prefix+tree.GetName()
            tree.SetName(prefix+tree.GetName())
    file.Write()
#tree
