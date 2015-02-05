#!/usr/bin/env python

import sys, os, re

# sys.path.append(os.path.abspath(os.path.dirname(sys.argv[0])) + "/../../utils")

# import utils
import operator
from pprint import pprint

allProcs = ('ggh', 'vbf', 'wh', 'zh', 'tth')

#----------------------------------------------------------------------

def findWorkspaces(rootFile):
    """ only searches for top level objects in the file """

    retval = []

    for key in rootFile.GetListOfKeys():

        obj = rootFile.Get(key.GetName())

        if isinstance(obj, ROOT.RooWorkspace):
            retval.append(obj)

    return retval


#----------------------------------------------------------------------

def findSingleWorkspace(rootFile):

    workspaces = findWorkspaces(rootFile)

    if len(workspaces) != 1:
        print >> sys.stderr,"not exactly one but",len(workspaces),"workspaces found in file",rootFile.GetName()
        sys.exit(1)

    return workspaces[0]

#----------------------------------------------------------------------
# main
#----------------------------------------------------------------------

from optparse import OptionParser

parser = OptionParser(   """
   usage: %prog [options] hgg_sig_workspace.root

   Tool to print the number of Gaussian components for each category,
   right/wrong vertex and signal production process for a H -> gamma gamma
   signal model file.  

""")

parser.add_option("--ws",
                  dest="wsname",
                  default = None,
                  help="name of the signal workspace",
                  )

parser.add_option("--sqrts",
                  dest="sqrts",
                  default = None,
                  help="force sqrts (for workspaces with multiple sqrts definitions)",
                  )


parser.add_option("--latex",
                  dest="latex",
                  default = False,
                  action = "store_true",
                  help="print output as a LaTeX table",
                  )

parser.add_option("--width",
                  dest="width",
                  default = False,
                  action = "store_true",
                  help="print number of Voigtians for the Higgs decay width signal model",
                  )

(options, ARGV) = parser.parse_args()

# libraries to load before opening the workspace file
options.lib = [ "$CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so" ]

#----------------------------------------

if len(ARGV) != 1:
    print >> sys.stderr,"wrong number of positional arguments. Run with -h to get more information"
    sys.exit(1)

fname = ARGV[0]

#----------------------------------------
# checks before continuing
#----------------------------------------

if not os.path.exists(fname):
    print >> sys.stderr,"file %s does not exist" % fname
    sys.exit(1)

for lib in options.lib:
    if not os.path.exists(os.path.expandvars(lib)):
        print >> sys.stderr,"library " + lib + " does not exist, exiting"
        sys.exit(1)

#----------------------------------------

# utils.addRooStatTypes()
import ROOT ; gcs = []

# load the libraries needed to process the workspace
for lib in options.lib:
    ROOT.gSystem.Load(lib)


fin = ROOT.TFile(fname)

assert fin.IsOpen()

# look at the workspace
global workspace

# TODO: use WorkspaceUtils
if options.wsname == None:
    workspace = findSingleWorkspace(fin)
else:
    workspace = fin.Get(options.wsname)
    if workspace == None:
        print >> sys.stderr,"workspace '%s' not found in file '%s'" % (options.wsname, fname)
        sys.exit(1)

import itertools

# for the moment, assume that there is no right/wrong vertex split

#--------------------
# print number of Gaussians used per process
#--------------------

allPdfNames = set()
tmp = workspace.allPdfs() ; gcs.append(tmp)
it = tmp.fwdIterator()

while True:
    pdf = it.next()

    if not pdf:
        break

    allPdfNames.add(pdf.GetName())


#--------------------
# check for sqrts

if options.sqrts == None:

    sqrtsFound = set()

    for pdfName in allPdfNames:
        mo = re.search("(\d+)TeV", pdfName)
        if mo:
            sqrtsFound.add(mo.group(1))


    if len(sqrtsFound) > 1:
        print "multiple sqrts found:"," ".join(sqrtsFound)
        sys.exit(1)

    if len(sqrtsFound) == 1:
        sqrts = str(list(sqrtsFound)[0]) + "TeV"
    else:
        sqrts = ""
else:
    sqrts = str(options.sqrts) + "TeV"
    

#--------------------

# first index is the category name
# second index is right/wrong vertex
# third index is the signal process
numGaussians = {}

sqrtsFound = None

for catnum in itertools.count():
    catname = "cat" + str(catnum)

    anythingFoundInThisCat = False

    for rightVertex, vertexPrefix, vertexName in (
        (True, 'rv', 'right'),
        (False,'wv', 'wrong'),
        ):

        for proc in allProcs:

            thisNumGaussians = None

            for gaussnum in itertools.count():

                if options.width:
                    name = "voig_g%(gaussnum)d_%(proc)s_%(catname)s_%(vertexPrefix)s_%(sqrts)s_NW" % locals()
                else:
                    # e.g. gaus_g0_ggh_cat0_rv_7TeV
                    name = "gaus_g%(gaussnum)d_%(proc)s_%(catname)s_%(vertexPrefix)s_%(sqrts)s" % locals()

                if not name in allPdfNames:
                    break

                # a Gaussian was found
                thisNumGaussians = gaussnum + 1

            # end of loop over all Gaussians

            if thisNumGaussians != None:
                numGaussians.setdefault(catname,{}).setdefault(vertexName,{})[proc] = thisNumGaussians

                anythingFoundInThisCat = True

        # end of loop over processes

    if not anythingFoundInThisCat:
        break
# end of loop over categories

maxCatNum = catnum - 1

print "--------------------"

if options.width:
    print "number of Voigtians"
else:
    print "number of Gaussians:"
print "--------------------"

if not options.latex:
    # pprint(numGaussians)
    for catnum in range(maxCatNum + 1):
        catname = "cat" + str(catnum)

        print catname + ":"

        for vertex in ("right", "wrong"):
            print "  %s vertex:" % vertex,

            for proc in allProcs:
                print "%-3s: %d" % (proc, numGaussians[catname][vertex][proc]),
            print

else:
    # print a LaTeX formatted table

    numProcs = len(allProcs)

    columnFormat = "|c||" + "|".join([ "c" ] * numProcs) + "||" + "|".join([ "c" ] * numProcs) + "|"

    print "\\begin{table}[!htbp]"
    print "  \\centering"
    print "  \\begin{tabular}{" + columnFormat + "} \\hline"
    print "    category & \\multicolumn{%d}{c|}{production mechanism} \\\\" % (2 * numProcs)
    print "             & \\multicolumn{%d}{c||}{right vertex} & \\multicolumn{%d}{c|}{wrong vertex} \\\\" % (numProcs, numProcs)
    print "             & " + " & ".join(allProcs + allProcs) + " \\\\ \\hline"
    print "  %"

    for catnum in range(maxCatNum + 1):
        catname = "cat" + str(catnum)

        items = []

        items.append(catnum)

        for vertex in ("right", "wrong"):
            for proc in allProcs:
                items.append(numGaussians[catname][vertex][proc])

        print " & ".join(str(x) for x in items),"\\\\",

        if catnum == maxCatNum:
            print "\\hline",
        print

    # end of loop over categories

    print "  \\end{tabular}"
    print "\\end{table}"


    


