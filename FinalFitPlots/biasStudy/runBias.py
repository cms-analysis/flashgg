#!/bin/env python

import time
from optparse import OptionParser, make_option
import sys, os, glob

objs = []

def system(cmd):
    print cmd
    os.system(cmd)

##
def main(options,args):

    
    ws = "cms_hgg_workspace"
    fname = "CMS-HGG_gof.root"
    
    tin = open("%s/%s" % (options.mydir,options.datacard_template),"r")
    template = tin.read()
    tin.close()

    truth = "datacard_truth_cat%d_%s.txt" % ( options.cat, options.truth )
    test = "datacard_test_cat%d_%s.txt" % ( options.cat, options.test )
    
    card = open(test, "w+" )
    card.write( template % { "cat" : "cat%d" % options.cat,
                             "bkg_shape" : "%s    %s:%s_cat%d_extpdf" % ( fname, ws, options.test, options.cat )
                             }
                )
    card.close()

    combine = "combine -s %d -t %d " % ( options.seed, options.ntoys )

    if options.ntoys == -1:
        options.truth += "_asimov"

    system( "%s -M GenerateOnly --saveToys -m %1.4g -n _cat%d_truth_%s %s >&/dev/null" % ( combine, options.masses[0], options.cat, options.truth, truth ) )

    toysFile = "higgsCombine_cat%d_truth_%s.GenerateOnly.mH%1.4g.%d.root" % ( options.cat, options.truth,  options.masses[0], options.seed )

    if options.ntoys == -1:
        combine += " --plots "
    for mass in options.masses:
        system( "%s -M MaxLikelihoodFit --rMin -10 --rMax 10 --toysFile %s -m %1.4g -n _cat%d_truth_%s_test_%s %s  >&/dev/null"
                % ( combine, toysFile, mass, options.cat, options.truth, options.test, test ) )
        if options.ntoys == -1:
            system("mkdir -p asimov_plots/cat%d_truth_%s_test_%s_m%1.4g" % ( options.cat, options.truth, options.test, mass))
            system("mv cat%d_{prefit,fit_s,fit_b}.png asimov_plots/cat%d_truth_%s_test_%s_m%1.4g"
                      % ( options.cat, options.cat, options.truth, options.test, mass))
    
    mergedFile = "higgsCombine_cat%d_truth_%s_test_%s.MaxLikelihoodFit.mHAll.%d.root" % ( options.cat, options.truth, options.test, options.seed )
    system( "hadd %s higgsCombine*MaxLikelihoodFit*.root" % mergedFile )

    
if __name__ == "__main__":
    parser = OptionParser(option_list=[
        make_option("--truth",
                    action="store", type="string", dest="truth",
                    default="",
                    help="default : [%default]", metavar=""
                    ),
        make_option("--test",
                    action="store", type="string", dest="test",
                    default="",
                    help="default : [%default]", metavar=""
                    ),
        make_option("-c","--cat",
                    action="store", type="int", dest="cat",
                    default=None,
                    help="default : [%default]", metavar=""
                    ),
        make_option("-t","--ntoys",
                    action="store", type="int", dest="ntoys",
                    default=100000,
                    help="default : [%default]", metavar=""
                    ),
        make_option("-s","--seed",
                    action="store", type="int", dest="seed",
                    default=-1,
                    help="default : [%default]", metavar=""
                    ),
        make_option("-m","--masses",
                    action="append", type="float", dest="masses",
                    default=[],
                    help="default : [%default]", metavar=""
                    ),
        make_option("-w","--workdir",
                    action="store", type="string", dest="workdir",
                    default="./",
                    help="default : [%default]", metavar=""
                    ),
        make_option("-d","--datacard-template",
                    action="store", type="string", dest="datacard_template",
                    default="datacard_bias_study_template.txt",
                    help="default : [%default]", metavar=""
                    ),
        ])
    
    (options, args) = parser.parse_args()

    if len(options.masses) == 0:
        options.masses.append(125)

    if options.seed == -1:
        options.seed = int(round(time.time() * 1000)) % 100000

    options.mydir = os.getcwd()
    os.chdir(options.workdir)
    
    sys.argv=[sys.argv[0],"-b"]
    import ROOT

    ROOT.RooMsgService.instance().setGlobalKillBelow(ROOT.RooFit.FATAL)
    ROOT.RooMsgService.instance().setSilentMode(True)

    main( options, args )
