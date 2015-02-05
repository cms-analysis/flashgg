import ROOT
import sys
ROOT.gSystem.Load("libRooFit.so")
ROOT.gSystem.Load("../libLoopAll.so");
ROOT.gROOT.ProcessLine(".L combineWorkspaces.C+")
from ROOT import workspaceMerger

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-o","--outputFile",dest="outputFile",default="CMS-HGG.root")
(options,args)=parser.parse_args()

merger = workspaceMerger(options.outputFile);
for file_i in args: 
	merger.addFile(file_i)
 
merger.combineWorkspaces()
merger.cleanUp()
print "Finished Combining Files, output saved to %s"%options.outputFile
