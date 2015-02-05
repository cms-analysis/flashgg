#!/bin/env python

from optparse import OptionParser, make_option
import sys, os, glob

#----------------------------------------------------------------------

import threading
class JobThread(threading.Thread):
    """ a thread to run bsub and wait until it completes """

    #----------------------------------------

    def __init__(self, cmd, lsfQueue, jobName):
        """ @param cmd is the command to be executed inside the bsub script. Some CMSSW specific wrapper
            code will be added
        """
        threading.Thread.__init__(self)
        self.cmd = cmd
        self.lsfQueue = lsfQueue
        self.jobName = jobName

    #----------------------------------------

    def run(self):
        bsubCmdParts = [ "bsub",
                         "-q " + self.lsfQueue,
                         "-J " + self.jobName,
                         "-K",  # bsub waits until job completes
                         ]

        bsubCmd = " ".join(bsubCmdParts)

        import subprocess
        lsf = subprocess.Popen(bsubCmd, shell=True, # bufsize=bufsize,
                               stdin=subprocess.PIPE,
                               close_fds=True)

        print >> lsf.stdin, "#!/bin/sh"

        print >> lsf.stdin, "cd " + os.environ['CMSSW_BASE']
        print >> lsf.stdin, "eval `scram runtime -sh`"
        print >> lsf.stdin, "cd " + os.getcwd()

        # the 'user' command (combine in this script)
        print >> lsf.stdin, self.cmd
        lsf.stdin.close()

        # wait for the job to complete
        self.exitStatus = lsf.wait()

        if self.exitStatus != 0:
            print "error running job",self.jobName


    #----------------------------------------

#----------------------------------------------------------------------

def runParallelLSF(cmdTemplate, jobParams, lsfQueue):
    # run combine in LSF and wait for the jobs to complete

    import string

    threads = []

    for thisJobParams in jobParams:
        # expand the template argument
        cmd = string.Template(cmdTemplate).safe_substitute(thisJobParams)

        # print "CMD=",cmd

        # create a runner thread and start it
        threads.append(JobThread(cmd, lsfQueue, thisJobParams['jobName']))
        threads[-1].start()

    # wait for threads to complete
    print >> sys.stderr,"waiting for %d jobs to complete..." % len(jobParams)
    for index, (thisJobParams, thread) in enumerate(zip(jobParams,threads)):
        # print >> sys.stderr,"waiting for job " + thisJobParams['jobName'] + " to complete (%d/%d)" % (index+1,len(jobParams))
        thread.join()

    # TODO: should we print a notification if some jobs failed ?
    print >> sys.stderr,"all jobs completed"

#----------------------------------------------------------------------

def extract_par_limits(pars, model_name, mass, cl=0.05):
    
    par_limits = { }
    
    mystruct = "struct Entry{ float quantileExpected; "
    for m in pars:
        mystruct += "float %s;" % m
        par_limits[ m ] = [0.,99999.,-99999.]
    mystruct += "};"

    ROOT.gROOT.ProcessLine(mystruct)
    from ROOT import Entry
    entry = Entry()
    
    tin = ROOT.TFile.Open("higgsCombine%s_single.MultiDimFit.mH%s.root" % ( model_name, mass ) )
    limit = tin.Get("limit")
    limit.SetBranchAddress("quantileExpected", ROOT.AddressOf(entry,"quantileExpected" ) )
    for p in pars:
        limit.SetBranchAddress( p, ROOT.AddressOf(entry,p) )

    for i in range(limit.GetEntries()):
        limit.GetEntry(i)
        for p in pars:
            val = getattr(entry, p)
            qt =  getattr(entry, "quantileExpected")
            if qt == 1.:
                par_limits[p][0] = val
            elif qt >= cl:
                par_limits[p][1] = min( val, par_limits[p][1] )
                par_limits[p][2] = max( val, par_limits[p][2] )
    tin.Close()
    
    return par_limits

def system(cmd, stopOnFailure = True):
    print cmd
    res = os.system(cmd)

    if res != 0 and stopOnFailure:
        print >> sys.stderr,"failed to run command '%s', exiting" % cmd
        sys.exit(res)
    
def main(options, args):

    combine_args = ""
    txt2ws_args  = ""
    if options.statOnly:
        combine_args += "--fastScan"
        ## txt2ws_args  += "--stat"
    

    model_args  = { "ggHqqH" : "-P HiggsAnalysis.CombinedLimit.PhysicsModel:floatingXSHiggs --PO modes=ggH,qqH",
                    "cVcF"   : "-P HiggsAnalysis.CombinedLimit.HiggsCouplingsLOSM:cVcF",
                    "rVrF"   : "-P HiggsAnalysis.CombinedLimit.PhysicsModel:rVrFXSHiggs",
                    "rV"     : "-P HiggsAnalysis.CombinedLimit.PhysicsModel:rVrFXSHiggs",
                    "rVprf"  : "-P HiggsAnalysis.CombinedLimit.PhysicsModel:rVrFXSHiggs",
                    "mumH"   : "-P HiggsAnalysis.CombinedLimit.PhysicsModel:floatingHiggsMass",
                    "mH"     : "-P HiggsAnalysis.CombinedLimit.PhysicsModel:rVrFXSHiggs --PO higgsMassRange=120,130",
                    }

    model_combine_args  = { "ggHqqH" : "",
                            "cVcF"   : "",
                            "rVrF"   : "",
                            "rV"     : "--floatOtherPOIs=0 -P RV",
                            "rVprf"  : "--floatOtherPOIs=1 -P RV",
                            "mH"     : "--floatOtherPOIs=1 -P MH",
                            "mumH"   : "-P r -P MH",
                            }
   
    model_pars  = { "ggHqqH" : ["r_ggH","r_qqH"],
                    "cVcF"   : ["CV","CF"],
                    "rVrF"   : ["RV","RF"],
                    "rV"     : ["RV"],
                    "rVprf"  : ["RV"],
                    "mH"     : ["MH"],
                    "mumH"     : ["r","MH"],
                    }

    pars_ranges = { "r_ggH"  : (0.,999999.,False),
                    "r_qqH"  : (0.,999999.,False),
                    "CV"     : (0.,2.,False),
                    "CF"     : (0.,3.,True),
                    "RV"     : (-4.,4.,False),
                    "RF"     : (-4.,4.,False),
                    "MH"     : (120.,130.,False),
                    "r"      : (-20,20.,False),
                    }

    parallel = "%s/parallel" % ( os.path.abspath(os.path.dirname(sys.argv[0])) )
    os.chdir( options.workdir )
    
    combine = "combine --preFitValue=0. --saveNLL %s %s " % ( combine_args, model_combine_args[options.model] )
    if options.expected > 0.:
        combine += " -t -1 --expectSignal=%f" % options.expected

    ## generate the model
    mass = "%1.5g" % options.mH
    model_name = "%s%1.5g" % ( options.model, options.mH )
    if options.label != "":
        model_name += "_%s" % options.label
    if options.statOnly:
        model_name += "_stat"

    #--------------------
    # check the existence of the merged output file before running
    # anything (if we run more than just the initial determination
    # of the maximum). Unless we force overwriting it.
    #--------------------

    haddOutputFile = "higgsCombine%s_grid.MultiDimFit.mH%s.root" % (model_name,mass)

    if not options.singleOnly:
        if os.path.exists(haddOutputFile) and not options.overwrite:
            print >> sys.stderr,"merged output file",os.path.abspath(haddOutputFile),"already exists, exiting (use --overwrite to force overwriting)"
            sys.exit(1)


    #--------------------

    model = "%s.root" % model_name
    if not os.path.isfile(model) or options.forceRedoWorkspace:

        if not os.path.exists(options.datacard):
            print >> sys.stderr,"datacard '%s' does not exist, exiting" % options.datacard
            sys.exit(1)

        system("text2workspace.py %s %s -o %s -m %1.5g %s" % ( txt2ws_args, options.datacard, model, options.mH, model_args[options.model] ) )

    ## best fit
    system("%s -M MultiDimFit %s --algo=singles -v2 -n %s_single -m %s | tee combine_%s_single.log"  % (
        combine, model, model_name, mass, model_name ) )

    par_limits = extract_par_limits(model_pars[options.model],model_name, mass)
        

    ## clone model limiting the parameters range
    ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
    fmod = ROOT.TFile.Open(model)
    ws = fmod.Get("w")
    
    for name, vals in par_limits.iteritems():
        best, down, up = vals
        if options.scanrange <= 0.:
            pmin, pmax = pars_ranges[name][0:2]
        else:
            pmin = max( best - options.scanrange*( best - down ), pars_ranges[name][0] )
            pmax = min( best + options.scanrange*( up   - best ), pars_ranges[name][1] )
        if pars_ranges[name][2]:
            pmax = max( abs(pmin), abs(pmax) )
            pmin = -pmax
        print name, pmin, pmax
        ws.var(name).setMin(pmin)
        ws.var(name).setMax(pmax)

    ws.writeToFile("%s_grid_test.root" % model_name)
    fmod.Close()

    if options.singleOnly:
        return

    ## run the NLL scan
    jobs=""
    jobParams = []
    step = options.npoints / 16 
    for ip in range(options.npoints/step+1):
        firstPoint = ip*step
        lastPoint = (ip+1)*step-1

        jobs="%s %d %d " % ( jobs, firstPoint, lastPoint )
        jobParams.append(dict(firstPoint = firstPoint,
                            lastPoint  = lastPoint,
                            jobName = "%d..%d" % (firstPoint, lastPoint), 
                            ))
    print jobs

    if options.submit:
        # submit to LSF
        runParallelLSF("%s -M MultiDimFit %s_grid_test.root -m %s -v0 -n %s_grid${firstPoint} --algo=grid --points=%d --firstPoint=${firstPoint} --lastPoint=${lastPoint} | tee combine_%s_grid${firstPoint}.log" % (combine, model_name, mass, model_name, options.npoints, model_name), jobParams, options.lsfQueue)
    else:
        # run interactively
        system( "%s -N2 --eta '%s -M MultiDimFit %s_grid_test.root -m %s -v0 -n %s_grid{1} --algo=grid --points=%d --firstPoint={1} --lastPoint={2} | tee combine_%s_grid{1}.log' ::: %s " % ( parallel, combine, model_name, mass, model_name, options.npoints, model_name, jobs ) )

    hadd = "hadd"
    if options.overwrite:
        hadd += " -f"
    hadd += " " + haddOutputFile

    for f in glob.glob("higgsCombine%s_grid[0-9]*.MultiDimFit.mH%s.root" % (model_name,mass) ):
        hadd += " %s" % f
    system(hadd)
    
if __name__ == "__main__":
    parser = OptionParser(option_list=[
        make_option("-w", "--workdir",
                    action="store", type="string", dest="workdir",
                    default="",
                    help="default : [%default]", metavar="WORKING_DIR"
                    ),
        make_option("-f", "--forceRedoWorkspace",
                    action="store_true",  dest="forceRedoWorkspace",
                    default=False,
                    help="default : [%default]",
                    ),
        make_option("-d", "--datacard",
                    action="store", type="string", dest="datacard",
                    default="datacard_fine.txt",
                    help="default : [%default]", metavar="DATACARD"
                    ),
        make_option("-r", "--scanrange",
                    action="store", type="float", dest="scanrange",
                    default=4.,
                    help="default : [%default]", metavar="SCANRANGE"
                    ),
        make_option("-m", "--mass",
                    action="store", type="float", dest="mH",
                    default=125.,
                    help="default : [%default]", metavar=""
                    ),
        make_option("-n", "--npoints",
                    action="store", type="int", dest="npoints",
                    default=10000,
                    help="default : [%default]", metavar="NPOINTS"
                    ),
        make_option("-M", "--model",
                    action="store", type="string", dest="model",
                    default="ggHqqH",
                    help="default : [%default]", metavar=""
                    ),
        make_option("--label",
                    action="store", type="string", dest="label",
                    default="",
                    help="default : [%default]", metavar=""
                    ),
        make_option("-s", "--singleOnly",
                    action="store_true", dest="singleOnly",
                    default=False,
                    help="default : [%default]", metavar=""
                    ),
        make_option("-S", "--statOnly",
                    action="store_true", dest="statOnly",
                    default=False,
                    help="default : [%default]", metavar=""
                    ),
        make_option("-e", "--expected",
                    action="store", type="float", dest="expected",
                    default=0.,
                    help="default : [%default]", metavar=""
                    ),

        make_option("--submit",
                    action="store_true",
                    default = False,
                    help="submit to LSF instead of running interactively", 
                    ),

        make_option("--lsf-queue",
                    type="str", dest = "lsfQueue",
                    default = "8nh",
                    help="LSF queue to use when running with --submit (default : [%default])", 
                    ),

        make_option("--overwrite",
                    action="store_true",
                    default = False,
                    help="force overwriting the final combined output ROOT file", 
                    ),



        ])
    



    (options, args) = parser.parse_args()
    if options.workdir == "":
        options.workdir = args.pop(1)

    sys.argv.append("-b")
    import ROOT

    main( options, args )

