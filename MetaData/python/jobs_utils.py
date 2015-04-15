from optpars_utils import *

from pprint import pprint

import os,json,fcntl
from parallel  import Parallel
from threading import Semaphore

import sys
from subprocess import call, Popen, PIPE
from copy import copy
import shlex

# Load command line option utilities
from parallel import *

# -------------------------------------------------------------------------------                                                                                           
def shell_args(cmd):
    return [ shell_expand(a) for a in shlex.split(cmd) ]

# -------------------------------------------------------------------------------
def shell_expand(string):
    if string:
        return os.path.expanduser( os.path.expandvars(string) )
    return None

# -------------------------------------------------------------------------------
def dumpCfg(options):
    cfg = {}
    for key,val in options.__dict__.iteritems():
        if not key.startswith("__opt__") and key != "dumpCfg":
            cfg[key] = val
    return json.dumps( cfg,indent=4)


# -------------------------------------------------------------------------------
class JobsManager(object):
    
    def __init__(self,
                 defaults={}
                 ):       
        """
        Constructur:
        @defaults: default options
        """

        # Command line options
        parser = OptionParser(option_list=[
                make_option("--processes", action="callback", callback=Load(), type="string", dest="processes",
                            default={}, help="List of datasets to be analyzed"),
                make_option("--load",  # special option to load whole configuaration from JSON
                            action="callback",callback=Load(),dest="__opts__",
                            type="string",
                            help="load JSON file with configuration",metavar="CONFIG.json"
                            ),
                make_option("-n","--njobs",dest="njobs",type="int",default=0,
                            help="number of jobs to run"),
                make_option("-q","--queue",dest="queue",type="string",default=None,
                            help="LSF queue to use. default: %default"),
                make_option("-o","--output",dest="output",type="string",
                            default="output.root", help="output file name. default: %default"),
                make_option("-d","--outputDir",dest="outputDir",type="string",
                            default=None, help="output folder. default: %default"),
                make_option("-x","--jobEx",dest="jobExe",type="string",
                            default=None, help="job executable. default: %default"),
                make_option("-c","--cmdLine",dest="cmdLine",type="string",
                            default=None, help="job command line. The script arguments will be prepended. default: %default"),
                make_option("--dumpCfg",
                            action="store_true",
                            default=False,
                            help="dump configuaration and exit. default: %default"),
                make_option("-v","--verbose",
                            action="store_true", dest="verbose",
                            default=False,
                            help="default: %default"),
                make_option("-m","--max-resubmissions",dest="maxResub", type="int",default=3),
                make_option("-N","--ncpu",dest="ncpu", type="int",default=cpu_count()),
                make_option("-H","--hadd",dest="hadd",default=False, action="store_true",
                            help="hadd output files when all jobs are finished."
                            ),
                make_option("-D","--hadd-dateset",dest="hadd_dataset",default=False, action="store_true",
                            help="hadd output per dataset when all jobs are finished."
                            ),
                make_option("-P","--hadd-process",dest="hadd_process",default=False, action="store_true",
                            help="hadd output per process when all jobs are finished."
                            ),
                make_option("--dry-run",dest="dry_run",default=False, action="store_true",
                            help="do not actually run the jobs."
                            ),
                make_option("-C","--cont",dest="cont",default=False, action="store_true",
                            help="continue interrupted task."
                            ),
                ]
                              )
        
        # parse the command line
        (self.options, self.args) = parser.parse_args()
        self.maxResub = self.options.maxResub

        if self.options.cmdLine:
            self.args = self.args+shell_args(str(self.options.cmdLine))
        
        if self.options.jobExe:
            self.args = [shell_expand(self.options.jobExe)]+self.args
            
    def __call__(self):
        """
        __call__
        Run all jobs.
        """
        self.parallel = Parallel(self.options.ncpu,lsfQueue=self.options.queue,lsfJobName="%s/runJobs" % self.options.outputDir,asyncLsf=False)
        
        self.jobs = None
        if self.options.cont:
            pass
        else:
            self.firstRun()
            
        self.monitor()
        
    def firstRun(self):

        (options,args) = (self.options, self.args)
        parallel = self.parallel
        
        outputPfx = options.output.replace(".root","")
        
        if not options.outputDir:
            sys.exit("Please specify an output folder")

        if options.dumpCfg:
            print ( dumpCfg(options) )
            sys.exit(0)

        if not os.path.exists(options.outputDir):
            os.mkdir(options.outputDir)
        outputPfx = "%s/%s" % ( options.outputDir, outputPfx )
        
        args.append("processIdMap=%s/config.json" % options.outputDir)
        ## options.cmdLine += " %s" % (" ".join(args))
        options.cmdLine = str(" ".join(args))
        with open("%s/config.json" % (options.outputDir), "w+" ) as fout:
            fout.write( dumpCfg(options) )
            
        
        
        outfiles = []
        doutfiles = {}
        poutfiles = {}
        
        jobs = []

        for name,datasets in options.processes.iteritems():
            poutfiles[name] = ( "%s_%s.root" % ( outputPfx,name), [] )
        
            for dset in datasets:
                job = args[0]
                if self.options.jobExe:
                    pyjob = ""
                else:
                    pyjob = job
                jobargs = copy(args[1:])
                dsetName = dset.lstrip("/").replace("/","_")
                outfile = "%s_%s.root" % ( outputPfx, dsetName )
                doutfiles[dset] = ( str(outfile),[] )
                jobargs.extend( ["dataset=%s" % dset, "outputFile=%s" % outfile ] )
                print "running: %s %s" % ( job, " ".join(jobargs) )
                if options.njobs != 0:
                    print  "splitting in (up to) %d jobs\n checking how many are needed... " % options.njobs
                    dnjobs = 0
                    dargs = jobargs+shell_args("nJobs=%d" % (options.njobs)) 
                    ret,out = parallel.run("python %s" % pyjob,dargs+shell_args("dryRun=1 getMaxJobs=1 dumpPython=%s.py" % os.path.join(options.outputDir,dsetName) ),interactive=True)[2]
                    maxJobs = self.getMaxJobs(out)
                    if maxJobs < 0:
                        print "Error getting numer of jobs to be submitted"
                        print out
                    hadd = self.getHadd(out,outfile)
                    ## for ijob in range(options.njobs):
                    for ijob in range(maxJobs):
                        ## FIXME allow specific job selection
                        ## iargs = dargs+shell_args("jobId=%d" % (ijob))
                        iargs = jobargs+shell_args("nJobs=%d jobId=%d" % (maxJobs, ijob))
                        ## # run python <command-line> dryRun=1 to check if the job needs to be run
                        ## ret,out = parallel.run("python %s" % pyjob,iargs+shell_args("dryRun=1"),interactive=True)[2]
                        ## if ret != 0:
                        ##     continue
                        dnjobs += 1 
                        if not options.dry_run:
                            ## FIXME: 
                            ##   - handle output
                            ##   - store log files
                            parallel.run(job,iargs)
                        ## outfiles.append( outfile.replace(".root","_%d.root" % ijob) )
                        ## output = self.getHadd(out,outfile.replace(".root","_%d.root" % ijob))
                        output = hadd.replace(".root","_%d.root" % ijob)
                        outfiles.append( output )
                        doutfiles[dset][1].append( outfiles[-1] )
                        poutfiles[name][1].append( outfiles[-1] )
                        jobs.append( (job,iargs,output,0,-1) )
                    print " %d jobs actually submitted" % dnjobs                
                else:
                    ret,out = parallel.run("python %s" % pyjob,jobargs+shell_args("dryRun=1 dumpPython=%s.py" % os.path.join(options.outputDir,dsetName)),interactive=True)[2]
                    if ret != 0:
                        print ret,out
                        continue
                    if not options.dry_run:
                        parallel.run(job,jobargs)
                    ## outfiles.append( outfile )
                    output = self.getHadd(out,outfile)
                    outfiles.append( output )
                    jobs.append( (job,jobargs,output,0,-1) )
                    poutfiles[name][1].append( outfiles[-1] )
                print

        task_config = {
            "jobs" : jobs,
            "datasets_output" : doutfiles,
            "process_output"  : poutfiles,
            "output"          : outfiles,
            "outputPfx"       : outputPfx
            }
        with open("%s/task_config.json" % (options.outputDir), "w+" ) as cfout:
            cfout.write( json.dumps(task_config,indent=4) )
            cfout.close()
            
    def monitor(self):

        (options,args) = (self.options, self.args)
        parallel = self.parallel
        
        with open("%s/task_config.json" % (options.outputDir), "r" ) as cfin:
            task_config = json.loads(cfin.read())
        
        doutfiles = task_config["datasets_output"]
        poutfiles = task_config["process_output"]
        outfiles  = task_config["output"]
        outputPfx = task_config["outputPfx"]
        

        if not options.dry_run:
            ## FIXME: job resubmission
            self.jobs = task_config["jobs"]
            returns = self.wait(parallel,self)
            task_config["jobs"] = self.jobs
            
        if options.hadd:
            print "All jobs finished. Merging output."
            p = Parallel(options.ncpu)
            hadd = "hadd -f "
            if options.hadd_process:
                for proc,out in poutfiles.iteritems():
                    outfile,outfiles = out
                    p.run("%s %s" % (hadd, outfile), outfiles )
            if options.hadd_dataset:
                if options.hadd_process:
                    hadd += " -T"
                for dset,out in doutfiles.iteritems():
                    outfile,outfiles = out
                    p.run("%s %s" % (hadd,outfile), outfiles) 
            if not (options.hadd_process or options.hadd_dataset):
                p.run("%s %s.root" % (hadd,outputPfx), outfiles)
            
            self.wait(p)

        with open("%s/task_config.json" % (options.outputDir), "w+" ) as cfout:
            cfout.write( json.dumps(task_config,indent=4) )
            cfout.close()
        
        self.parallel.stop()

    def wait(self,parallel,handler=None):
        return parallel.wait(handler)
    
        ### for i in range(parallel.njobs):
        ###     print "Finished jobs: %d. Total jobs: %d" % (i, parallel.njobs)
        ###     job, jobargs, ret = parallel.returned.get()
        ###     print "finished: %s %s" % ( job, " ".join(jobargs) )
        ###     for line in ret[1].split("\n"):
        ###         print line

    def handleJobOutput(self,job,jobargs,ret):
        print "------------"
        print "Job finished: (exit code %d) '%s' '%s'" % ( ret[0], job, " ".join(jobargs) )
        print "Job output: "
        print 
        for line in ret[1].split("\n"):
            print line
        print
        jobargs = shell_args(" ".join(jobargs))
        job = jobargs[0]
        jobargs = jobargs[1:]
        for ijob in self.jobs:
            inam,iargs = ijob[0:2]
            ### print inam, job, inam == job
            ### for i,a in enumerate(iargs):
            ###     b = jobargs[i]
            ###     print a, b,  a == b
            if inam == job and iargs == jobargs:
                ijob[4] = ret[0]
                if ret[0] != 0:
                    print ""
                    print "Job failed. Number of resubmissions: %d / %d. " % (ijob[3], self.maxResub),
                    if ijob[3] < self.maxResub:
                        print "Resubmitting."
                        self.parallel.run(inam,iargs)
                        ijob[3] += 1
                        print "------------"
                        return 1
                    else:
                        print "Giving up."
        print "------------"
        return 0
    
    def getHadd(self,stg,fallback):
        for line in stg.split("\n"):
            if line.startswith("hadd:"):
                return line.replace("hadd:","")
        return fallback

    def getMaxJobs(self,stg):
        for line in stg.split("\n"):
            if line.startswith("maxJobs:"):
                return int(line.replace("maxJobs:",""))
        return -1
