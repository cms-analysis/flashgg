#!/usr/bin/env python

import sys

#----------------------------------------------------------------------
# main
#----------------------------------------------------------------------

from optparse import OptionParser
parser = OptionParser("""
   usage: %prog [options] input_file1 [ input_file2 ... ]

   takes multiple input files (e.g. produced with opttreeDumper.py)
   and creates one merged output ROOT file, renaming
   the output tree
   """)

parser.add_option("-o", "--output",
                  default=None,
                  help="Output ROOT file. WARNING: will be overwritten without confirmation if it already exists.")

parser.add_option("-t", "--output-treename",
                  dest = "outputTreeName",
                  default="opttree",
                  help="Output tree name (default: %default)")

parser.add_option("--input-treename",
                  dest = "inputTreeName",
                  default="tagCandidateDumper/trees/opttree_13TeV_All",
                  help="input tree name (default: %default)")

(options, ARGV) = parser.parse_args()

inputFnames = ARGV[:]

if len(inputFnames) < 1:
    print >> sys.stderr,"no input file specified. Run with -h to show usage."
    sys.exit(1)

if options.output == None:
    print >> sys.stderr,"no output file specified. Run with -h to show usage."
    sys.exit(1)

#----------------------------------------

import ROOT

# create the output file
outFile = ROOT.TFile(options.output, 'recreate')


# loop over all input files
isFirst = True
for fileIndex, inputFname in enumerate(inputFnames):

    print "processing file %s (%d/%d)" % (inputFname, fileIndex + 1, len(inputFnames))

    fin = ROOT.TFile.Open(inputFname, "read")

    if fin == None or not fin.IsOpen():
        print >> sys.stderr,"failed to open ROOT file",inputFname,"exiting"
        sys.exit(1)
    
    ROOT.gROOT.cd()

    tree = fin.Get(options.inputTreeName)

    if tree == None:
        print >> sys.stderr,"could not find input tree %s in file %s, exiting" % (options.inputTreeName, inputFname)
        sys.exit(1)


    # add to output file
    outFile.cd()

    if isFirst:
        # simply clone the first tree
        outTree = tree.CloneTree()
        outTree.SetName(options.outputTreeName)
    else:
        # append to tree
        tree.SetBranchStatus('*', 1)
        outTree.CopyEntries(tree, -1,
                            "FAST"
                            )
                            

    # next iteration
    isFirst = False

# end of loop over input files

outFile.cd()
outTree.Write()
outFile.Close()

