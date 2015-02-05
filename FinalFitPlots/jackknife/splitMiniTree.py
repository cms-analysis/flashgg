#!/bin/env python

from optparse import OptionParser, make_option
import sys

def main(options,args):

    import ROOT
    import numpy
    import json
    import os
    
    mydir = os.path.dirname(sys.argv[0])
    ROOT.gSystem.SetIncludePath("-I$ROOTSYS/include -I$ROOFITSYS/include -I%s" % (mydir))
    ROOT.gROOT.LoadMacro("%s/SplitMiniTree.C+g" % mydir )

    partitions = json.load(open(options.partitions))

    if options.globeParametric:
        pdfName = "pdf_data_pol_model_8TeV_cat%d"
        wsName  = "wbkg_8TeV"
        dsName  = "roohist_data_mass_cat%d" 
    elif options.globeBinned:
        pdfName = "pdf_data_pol_model_8TeV_cat%d"
        wsName  = "cms_hgg_workspace"
        dsName  = "roohist_data_mass_cat%d" 
    else:
        pdfName = "CMS_hgg_mvacat%d_8TeV_bkgshape"
        wsName  = "wbkg"
        dsName  = "databinned_mvacat%d_8TeV" 

    output = options.input
    if options.outdir != "":
        output = "%s/%s" % ( options.outdir, os.path.basename(output) )
    splitMiniTree = ROOT.SplitMiniTree(options.input,options.wspace,wsName,pdfName,dsName,output)

    if options.ncat > 0:
        splitMiniTree.setNcat(options.ncat)
    if options.nbins > 0:
        splitMiniTree.setBinning(options.nbins,options.massMin,options.massMax)
    
    for ipart,partition in enumerate(partitions):
        for run,lumi,event in partition:
            splitMiniTree.addToPartition(ipart,run,lumi,event)
    
    splitMiniTree.splitCopy()

    print "npart: %d" % len(partitions)

if __name__ == "__main__":
    parser = OptionParser(option_list=[
        make_option("-i", "--input",
                    action="store", type="string", dest="input",
                    default="",
                    help="default: %default", metavar=""
                    ),
        make_option("-w", "--wspace",
                    action="store", type="string", dest="wspace",
                    default="",
                    help="default: %default", metavar=""
                    ),
        make_option("-d", "--outdir",
                    action="store", type="string", dest="outdir",
                    default="",
                    help="default: %default", metavar=""
                    ),
        make_option("-g", "--globeBinned",
                    action="store_true", dest="globeBinned",
                    default=False,
                    help="default: %default", metavar=""
                    ),
        make_option("-G", "--globeParametric",
                    action="store_true", dest="globeParametric",
                    default=False,
                    help="default: %default", metavar=""
                    ),
        make_option("-m", "--MIT",
                    action="store_false", dest="globe",
                    default=False,
                    help="", metavar=""
                    ),
        make_option("-p", "--partitions",
                    action="store", type="string", dest="partitions",
                    default="",
                    help="default : %default", metavar=""
                    ),
        make_option("-c", "--ncat",
                    action="store", type="int", dest="ncat",
                    default=-1,
                    help="default : %default", metavar=""
                    ),
        make_option("-b", "--nbins",
                    action="store", type="int", dest="nbins",
                    default=-1,
                    help="default : %default", metavar=""
                    ),
        make_option("-L", "--massMin",
                    action="store", type="float", dest="massMin",
                    default=100,
                    help="default : %default", metavar=""
                    ),
        make_option("-H", "--massMax",
                    action="store", type="float", dest="massMax",
                    default=180,
                    help="default : %default", metavar=""
                    ),
        
        ])
    
    (options, args) = parser.parse_args()

    sys.argv.append("-b")
    main( options, args ) 
    
