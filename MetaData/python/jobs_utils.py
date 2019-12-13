from optpars_utils import *

from pprint import pprint

import os,json,fcntl,stat
from parallel  import *
from threading import Semaphore

import sys
from subprocess import call, Popen, PIPE
from copy import copy
import shlex

# -------------------------------------------------------------------------------                                                                                           
def shell_args(cmd):
    return [ shell_expand(a) for a in shlex.split(cmd) ]

# -------------------------------------------------------------------------------
def shell_expand(string):
    if string:
        return os.path.expanduser( os.path.expandvars(string) )
    return None

# -------------------------------------------------------------------------------
def dumpCfg(options,skip=[]):
    cfg = {}
    for key,val in options.__dict__.iteritems():
        if not key.startswith("__opt__") and key != "dumpCfg" and not key in skip:
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

                make_option("--dsfilter", action = "callback",
                            callback = lambda option, opt, value, parser:
                               setattr(parser.values, option.dest, value.split(',')),
                            metavar = "'/ds1*,/ds2*'",
                            type="string", dest="selectedDatasetPatterns",
                            default=None, help="comma separated list of dataset name patterns to run on (default: run on all)"),

                make_option("--load",  # special option to load whole configuaration from JSON
                            action="callback",callback=Load(),dest="__opts__",
                            type="string",
                            help="load JSON file with configuration",metavar="CONFIG.json"
                            ),
                make_option("-n","--njobs",dest="njobs",type="int",default=0,
                            help="number of jobs to run"),
                make_option("-q","--queue",dest="queue",type="string",default=None,
                            help="LSF queue to use. default: %default"),
                make_option("--sync-lsf",dest="asyncLsf",action="store_false",default=True,
                            help="Run LSF jobs in sync mode (with -K). This will spawn one thread per job. Use only if you know what you are doing."
                            " default: False"),
                make_option("--use-tarball",dest="use_tarball",action="store_true",default=True,
                            help="Make a sandbox tarball for the task default: %default"),
                make_option("--no-use-tarball",dest="useTarball",action="store_false",default=True,
                            help="Do not make a sandbox tarball for the task."),
                make_option("--stage-to",dest="stageTo",action="store",default=None,type="string",
                            help="Stage output to folder. default: %default"),                
                make_option("--stage-cmd",dest="stageCmd",action="store",default="guess",type="string",
                            help="Stage out command. (use 'guess' to have the script guessing the command from the output folder) default : %default"),                
                make_option("--summary",dest="summary",action="store_true",default=False,
                            help="Print jobs summary and exit"),
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
                make_option("-m","--max-resubmissions",dest="maxResub", type="int",default=2),
                make_option("-N","--ncpu",dest="ncpu", type="int",default=cpu_count()),
                make_option("--nCondorCpu",dest="ncondorcpu", type="int",default=1,help="Number of cpu cores per job to request from condor"),
                make_option("--make-light-tarball",dest="lighttarball", action="store_true",default=False,help="Include datafolders only as symbolic link in tarball"),
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
                make_option("-R","--resubmit-missing",dest="resubMissing",default=False, action="store_true",
                            help="resubmit unfinished jobs upon continue."
                            ),
                make_option("--resubmit-ids",dest="resubmit_ids", default="", type="string",
                            help="Specify list of job ids to be resubmitted"
                            ),
                make_option("-b","--batch-system",dest="batchSystem",type="string",
                            default="auto",help="Batch system name. Currently supported: sge lsf, default: %default"
                            ),
                make_option("--no-copy-proxy",dest="copy_proxy",action="store_false",
                            default=True,help="Do not try to copy the grid proxy to the worker nodes."
                            ),
                ]
                              )
        
        # parse the command line
        (self.options, self.args) = parser.parse_args()
        self.maxResub = self.options.maxResub

        if self.options.cmdLine:
            self.args = self.args+shell_args(str(self.options.cmdLine))
        
        if self.options.jobExe:
            self.options.jobExe = shell_expand(self.options.jobExe)
            if not self.args[0] == self.options.jobExe:
                self.args = [self.options.jobExe]+self.args
            
        self.uniqueNames = {}

        if self.options.batchSystem == 'auto':
            self.options.batchSystem = BatchRegistry.getBatchSystem()
        # self.checkCrossSections()

    # -------------------------------------------------------------------------------------------------------------------
    def __call__(self):
        """
        __call__
        Run all jobs.
        """
        if self.options.summary:
            self.options.dry_run = True
            self.options.cont = True
  
        self.jobFactory = WorkNodeJobFactory(self.options.stageTo,self.options.stageCmd,job_outdir=self.options.outputDir,
                                            batchSystem=self.options.batchSystem,copy_proxy=self.options.copy_proxy)
        self.parallel = Parallel(self.options.ncpu,lsfQueue=self.options.queue,lsfJobName="%s/runJobs" % self.options.outputDir,
                                 asyncLsf=self.options.asyncLsf,jobDriver=self.jobFactory,batchSystem=self.options.batchSystem,ncondorcpu=self.options.ncondorcpu)
        
        self.jobs = None
        self.failedJobIds = {}
        self.nFailedJobs = 0

        if self.options.cont:
            if self.options.asyncLsf:
                self.loadLsfMon()
        else:
            self.firstRun()
            
        self.monitor()
        self.parallel.stop()

    # -------------------------------------------------------------------------------------------------------------------
    def loadLsfMon(self):
        
        with open("%s/task_config.json" % (self.options.outputDir), "r" ) as cfin:
            task_config = json.loads(cfin.read())
        jobs = task_config["jobs"]
        
        if self.options.useTarball:
            if not "tarball" in task_config:
                print 
                print "You asked to run the jobs using a sandbox tarball, but the tarball name was not found in the task configuration"
                print "    If you specified the --use-tarball now but not in the original submission, please remove it."
                print "    Otherwise the task configuration may have been corrupted."
                print 
                sys.exit(-1)
            self.jobFactory.setTarball(task_config["tarball"])
            if not self.options.stageTo:
                self.jobFactory.stageDest( os.path.abspath(self.options.outputDir) )

        self.parallel.setJobId(task_config.get("last_job_id",1))
        resub = False
        for job in jobs:
            self.failedJobIds[len(self.failedJobIds)] = []
            cmd, args, outfile, nsub, ret, batchId = job
            if type(batchId) == tuple or type(batchId) == list:
                jobName,batchId = batchId
            else:
                jobName=None
            if ret != 0 or (self.options.batchSystem == 'htcondor' and len(job[5][1])>0):
                if nsub <= self.options.maxResub:
                    resub = True
                    if self.options.resubMissing:
                        if self.options.batchSystem == 'htcondor' and self.options.resubmit_ids != "":
                            for i,arg in enumerate(args):
                                if 'resubMap' in arg:
                                    args[i] = 'resubMap='+self.options.resubmit_ids
                            print('Forced resubmission: jobs %s will be resubmitted' % self.options.resubmit_ids)
                        out = self.parallel.run(cmd,args,jobName=jobName)
                        if self.options.queue and self.options.asyncLsf:
                            job[5] = out[-1][1][1]
                        self.storeTaskConfig(task_config)
                    else:
                        self.parallel.addJob(cmd,args,batchId,jobName)
                else:
                    print("Not all jobs ended successfully but the --max-resubmissions option prevent further processing")
                    print("If you wish to resubmit the failed jobs please specify --max-resubmissions M with M greater than %d" % self.options.maxResub)
                        
        if not resub:
            print("All jobs successfully processed")

    # -------------------------------------------------------------------------------------------------------------------

    def isSelectedDataset(self, dsname):
        if self.options.selectedDatasetPatterns == None:
            # no filter specified on the command line
            return True

        import fnmatch

        matches = any(fnmatch.fnmatch(dsname, pattern) for pattern in self.options.selectedDatasetPatterns)
        return matches

    # -------------------------------------------------------------------------------------------------------------------
    def firstRun(self):

        (options,args) = (self.options, self.args)
        parallel = self.parallel
        
        task_config = {}

        outputPfx = options.output.replace(".root","")
        
        if not options.outputDir:
            sys.exit("\nPlease specify an output folder using the -d option\n")

        if options.dumpCfg:
            print ( dumpCfg(options) )
            sys.exit(0)
            
        if not os.path.exists(options.outputDir):
            os.mkdir(options.outputDir)
        outputPfx = "%s/%s" % ( options.outputDir, outputPfx )
        

        args.append("processIdMap=%s/config.json" % os.path.abspath(options.outputDir))

        pset = args[0] if not options.jobExe else args[1]
        with open(pset,"r") as pin:
            with open("%s/%s" % ( options.outputDir, os.path.basename(pset) ), "w+" ) as pout:
                pout.write(pin.read())
                pout.close()
                if not options.jobExe: os.chmod( "%s/%s" % ( options.outputDir, os.path.basename(pset)), 0755  )
            pin.close()
        pset = "%s/%s" % ( options.outputDir, os.path.basename(pset) )
        pset = os.path.abspath(pset)
        
        if options.useTarball:
            apset = os.path.abspath(pset)
            self.jobFactory.mkTarball("%s/sandbox.tgz" % os.path.abspath(options.outputDir),
                                      tarball_entries=[apset,"python","lib","bin","src/flashgg/MetaData/python"],tarball_patterns=[("src/*","data"), ("external/*","data"), ("src/*","toolbox"), ("src/*","python")],
                                      tarball_transform="'s,%s,pset.py,'" % (apset.lstrip("/")), light=self.options.lighttarball
                                      )
            if not options.queue:
                print "\nWARNING: You specified the --use-tarball option but no batch queue. The tarball was created but the jobs won't actually use it."
                print "           To avoid this printout run with --no-use-tarball or specify a batch queue using the --queue option.\n"
                options.useTarball = False
                
            task_config["tarball"] = self.jobFactory.tarball
            
        if not options.stageTo:
            self.jobFactory.stageDest( os.path.abspath(options.outputDir) )
            options.stageTo = os.path.abspath(options.outputDir)
            print "\nWill stage output to %s using the command '%s'\n" % ( self.jobFactory.stage_dest, self.jobFactory.getStageCmd() )

        if options.jobExe:
            args[1] = pset
        else:
            args[0] = pset

        with open("%s/config.json" % (options.outputDir), "w+" ) as fout:
            fout.write( dumpCfg(options,skip=["dry_run","summary"]) )
        
        # store cmdLine
        options.cmdLine = str(" ".join(args))

        outfiles = []
        doutfiles = {}
        poutfiles = {}
        
        jobs = []

        for name,datasets in options.processes.iteritems():
            poutfiles[name] = ( "%s_%s.root" % ( outputPfx,name), [] )
        
            for dset in datasets:

                #----------

                # check if this datasets was selected
                
                if not self.isSelectedDataset(dset):
                    # skip this dataset
                    print "skipping",dset
                    continue

                #----------
                self.failedJobIds[len(self.failedJobIds)] = []

                job = args[0]
                if self.options.jobExe:
                    pyjob = ""
                else:
                    pyjob = job
                if type(dset) == list:
                    dset,dopts = dset
                else:
                    dopts = {}
                jobargs = copy(args[1:])
                dsetName = dset.lstrip("/").replace("/","_")
                dsetName = self.getUniqueName(dsetName)
                outfile = "%s_%s.root" % ( outputPfx, dsetName )
                if len(outfile) > 255:
                    outfile = outfile.replace("RunIISpring16DR80X-2_3_0-25ns_","")
                doutfiles[dsetName] = ( str(outfile),[] )
                jobargs.extend( ["dataset=%s" % dset, "outputFile=%s" % outfile ] )
                # add (and replace) per-dataset job arguments
                dargs = dopts.get("args",[])
                if type(dargs) != list:
                    print "\nERROR : dataset-specific arguments should be list not %s" % (type(dargs))
                    print "          dataset %s" % dset
                    sys.exit(-1)
                if len(dargs) > 0:
                    replace = {}
                    for arg in dargs:
                        aname,val = arg.split("=")
                        if aname == "outputFile":
                            if val.count("/") != outfile.count("/"):
                                print "SCZ The specified outputFile does not yield same the subdir as expected:",val
                                val = "/".join(outfile.split("/")[:-1]+[val.split("/")[-1]])
                                print "SCZ So we adjust the outputFile name to: ",val
                        replace[aname] = "=".join((aname,val))
                    newargs = []
                    anames = []
                    for arg in jobargs:
                        if not "=" in arg: 
                            newargs.append(arg)
                            continue
                        aname,val = arg.split("=")
                        if aname in replace: newargs.append( replace.pop(aname) )
                        else: newargs.append(arg)
                    jobargs = newargs
                    for aname,arg in replace.iteritems(): jobargs.append(arg)
                print "running: %s %s" % ( job, " ".join(jobargs) )
                njobs = dopts.get("njobs",options.njobs) if options.njobs != 0 else 0
                if  njobs != 0:
                    print  "splitting in (up to) %d jobs\n checking how many are needed... " % njobs, 
                    dnjobs = 0
                    dargs = jobargs+shell_args("nJobs=%d" % (njobs)) 
                    ret,out = parallel.run("python %s" % pyjob,dargs+shell_args("dryRun=1 getMaxJobs=1 dumpPython=%s.py" % os.path.join(options.outputDir,dsetName) ),interactive=True)[2]
                    maxJobs = self.getMaxJobs(out)
                    print maxJobs
                    if maxJobs < 0:
                        print "Error getting number of jobs to be submitted"
                        print out
                    hadd = self.getHadd(out,outfile)
                    print " now submitting jobs",
                    #---HTCondor cluster submission
                    if self.options.batchSystem == 'htcondor' and self.options.queue:
                        iargs = jobargs+shell_args("nJobs=%d jobId=${jobIdsMap[${1}]} resubMap=%s" % (maxJobs, ','.join([str(x) for x in range(maxJobs)])))
                        dnjobs = maxJobs
                        batchId = -1
                        if not options.dry_run:
                            ret,out = parallel.run(job,iargs)[-1]
                            if self.options.queue and self.options.asyncLsf:
                                batchId = out[1]
                        output = []
                        for ijob in range(maxJobs):
                            output.append(hadd.replace(".root","_%d.root" % ijob))
                            outfiles.append( output )
                            doutfiles[dsetName][1].append( outfiles[-1] )
                            poutfiles[name][1].append( outfiles[-1] )
                        jobs.append( (job,iargs,output,0,-1,batchId) )
                    #---Other batch system single jobs submission
                    else:
                        for ijob in range(maxJobs):
                            ## FIXME allow specific job selection                                                        
                            iargs = jobargs+shell_args("nJobs=%d jobId=%d" % (maxJobs, ijob))
                            dnjobs += 1 
                            batchId = -1
                            if not options.dry_run:
                                ret,out = parallel.run(job,iargs)[-1]
                                if self.options.queue and self.options.asyncLsf:
                                    batchId = out[1]
                                print ".",
                            output = hadd.replace(".root","_%d.root" % ijob)
                            outfiles.append( output )
                            doutfiles[dsetName][1].append( outfiles[-1] )
                            poutfiles[name][1].append( outfiles[-1] )
                            jobs.append( (job,iargs,output,0,-1,batchId) )
                    print "\n %d jobs submitted" % dnjobs                
                else:
                    ret,out = parallel.run("python %s" % pyjob,jobargs+shell_args("dryRun=1 dumpPython=%s.py" % os.path.join(options.outputDir,dsetName)),interactive=True)[2]
                    if ret != 0:
                        print ret,out
                        continue
                    output = self.getHadd(out,outfile)

                    batchId = -1
                    if not options.dry_run:
                        ret,out = parallel.run(job,jobargs)[-1]
                        if self.options.queue and self.options.asyncLsf:
                            batchId = out[1]
                            
                    outfiles.append( output )
                    jobs.append( (job,jobargs,output,0,-1,batchId) )
                    poutfiles[name][1].append( outfiles[-1] )
                print

        task_config["jobs"] =  jobs
        task_config["datasets_output"] =  doutfiles
        task_config["process_output"] =  poutfiles
        task_config["output"] =  outfiles
        task_config["outputPfx"] =  outputPfx
        
        self.storeTaskConfig(task_config)

    # -------------------------------------------------------------------------------------------------------------------
    def storeTaskConfig(self,task_config):
        with open("%s/task_config.json" % (self.options.outputDir), "w+" ) as cfout:
            task_config["last_job_id"] = self.parallel.currJobId()
            cfout.write( json.dumps(task_config,indent=4) )
            cfout.close()
            
    # -------------------------------------------------------------------------------------------------------------------
    def getUniqueName(self,basename):
        if basename in self.uniqueNames:
            self.uniqueNames[basename] += 1
        else:
            self.uniqueNames[basename] = 0
            return basename
        return "%s%d" % (basename,self.uniqueNames[basename])

    # -------------------------------------------------------------------------------------------------------------------
    def monitor(self):

        (options,args) = (self.options, self.args)
        parallel = self.parallel
        
        with open("%s/task_config.json" % (options.outputDir), "r" ) as cfin:
            task_config = json.loads(cfin.read())
        
        doutfiles = task_config["datasets_output"]
        poutfiles = task_config["process_output"]
        outfiles  = task_config["output"]
        outputPfx = task_config["outputPfx"]

        self.task_config = task_config
        
        if options.summary:
            self.printSummary()
            return

        if not options.dry_run:
            returns = self.wait(parallel, self)
            while self.nFailedJobs > 0:
                for i, failed in self.failedJobIds.items():
                    if len(failed) > 0:
                        ijob = task_config['jobs'][i]
                        inam, iargs = ijob[0:2]
                        for ir, arg in enumerate(iargs):
                            if 'resubMap' in arg:
                                iargs[ir] = 'resubMap='+','.join([str(id) for id in failed])
                        out = self.parallel.run(inam, iargs, jobName=ijob[5][0])
                        if self.options.queue and self.options.asyncLsf:
                            ijob[5] = out[-1][1][1]
                        self.storeTaskConfig(self.task_config)                    
                
                self.nFailedJobs = 0
                self.wait(parallel, self)
                    
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

        self.storeTaskConfig(task_config)
        
        self.parallel.stop()

    # -------------------------------------------------------------------------------------------------------------------
    def wait(self,parallel,handler=None):
        return parallel.wait(handler)

    # -------------------------------------------------------------------------------------------------------------------
    def handleJobOutput(self,job,jobargs,ret):
        print "------------"
        print "Job finished: (exit code %d) '%s' '%s'" % ( ret[0], job, " ".join(jobargs) )
        print "Job output: "
        print

        lines = ret[1].split("\n")
        if self.options.queue and self.options.asyncLsf:
            lines = lines[-10:]
        for line in lines:
            print line
        print
        jobargs = shell_args(" ".join(jobargs))
        job = jobargs[0]
        jobargs = jobargs[1:]
        for i,ijob in enumerate(self.task_config["jobs"]):
            inam,iargs = ijob[0:2]
            if inam == job and iargs == jobargs:
                ijob[4] = ret[0]
                if ret[0] != 0:
                    print ""
                    print "Job failed. Number of resubmissions: %d / %d. " % (ijob[3], self.maxResub),
                    if ijob[3] < self.maxResub:
                        if self.options.batchSystem == 'htcondor' and self.options.queue:                    
                            print "Collecting failed job for future resubmission."
                            ijob[3] += 1
                            if ijob[3] == self.maxResub and "lastAttempt=1" not in iargs:
                                iargs.append("lastAttempt=1")   
                            #---HTCondor exit code is 0 for success jobId+1 in case of failure
                            self.failedJobIds[i].append(ret[0]-1)
                            self.nFailedJobs += 1
                            self.storeTaskConfig(self.task_config)
                            #---return 0 since the job is not currently running, although it will be resubmitted later
                            return 0
                        else:
                            print "Resubmitting."
                            ijob[3] += 1
                            if ijob[3] == self.maxResub:
                                iargs.append("lastAttempt=1")                        
                            jobName = ijob[5][0] if self.options.queue else None                        
                            out = self.parallel.run(inam,iargs,jobName=jobName)
                            if self.options.queue and self.options.asyncLsf:
                                ijob[5] = out[-1][1][1]
                                self.storeTaskConfig(self.task_config)
                            print "------------"
                            return 1
                    else:
                        print "Giving up."
                #---HTCondor remove jobid from the jobid list sotred in the task_config json 
                #   (do not rely on return value stored in ijob[4] for HTC)                
                elif self.options.batchSystem == 'htcondor' and self.options.queue and ret[2] in ijob[5][1]:
                    ijob[5][1].remove(ret[2])
        
        self.storeTaskConfig(self.task_config)
        print "------------"
        return 0
    
    # -------------------------------------------------------------------------------------------------------------------
    def getHadd(self,stg,fallback):
        for line in stg.split("\n"):
            if line.startswith("hadd:"):
                return line.replace("hadd:","")
        return fallback

    # -------------------------------------------------------------------------------------------------------------------
    def getMaxJobs(self,stg):
        for line in stg.split("\n"):
            if line.startswith("maxJobs:"):
                return int(line.replace("maxJobs:",""))
        return -1
    
    # -------------------------------------------------------------------------------------------------------------------
    def printSummary(self):
        
        jobs = self.task_config["jobs"]
        procs = self.task_config["datasets_output"]
        
        status = {}
        for job in jobs:
            cmd, args, outfiles, nsub, ret, batchId = job
            if isinstance(outfiles, list):    
                for i,outfile in enumerate(outfiles):
                    status[outfile] = (nsub,ret,batchId[i])
            else:
                status[outfiles] = (nsub,ret,batchId)
            
        for proc,out in procs.iteritems():
            outfile,outfiles = out
            unpacked_outfiles = []
            for outfile in outfiles:
                if isinstance(outfile, list):
                    for of in outfile:
                        unpacked_outfiles.append(of)
                else:
                    unpacked_outfiles.append(outfile)
            finished = []
            missing  = {}
            for jfile in unpacked_outfiles:                
                nsub,ret,batchId = status[jfile]
                if ret != 0:
                    if not nsub in missing:
                        missing[nsub] = []
                    missing[nsub].append( (jfile,batchId) )
                else:
                    finished.append(jfile)
            print "----------"
            print "process:           %s " % proc
            print "njobs:             %d " % len(outfiles)
            print "finished:          %d " % len(finished)
            for nsub,lst in missing.iteritems():
                print "submitted %d times: %d"  % (nsub+1, len(lst))
                if self.options.verbose:
                    for jfile,batchId in lst:
                        print "%s: %s" % (jfile,batchId[0])
            print 
                
    # -------------------------------------------------------------------------------------------------------------------

    def checkCrossSections(self):
        # checks if all cross sections (apart from data)
        # were specified

        import re
        from flashgg.MetaData.samples_utils import SamplesManager
        from flashgg.MetaData.JobConfig import JobConfig

        # ignore any escaping etc.
        kwargs = {}
        for part in re.split('\s+', self.options.cmdLine):
            key, value = part.split('=',1)
            kwargs[key] = value

        if not kwargs.has_key('campaign'):
            print >> sys.stderr,"WARNING: campaign not set"

        # automatically get metadata locations right for both high-mass and SM Higgs
        try:
            from diphotons.MetaData.JobConfig import customize
        except ImportError:
            from flashgg.MetaData.JobConfig import customize
        if not kwargs.has_key("metaDataSrc"):
            kwargs["metaDataSrc"] = customize.metaDataSrc
        if not kwargs.has_key("crossSections"):
            kwargs["crossSections"] = customize.crossSections

        # TODO: the following line is duplicated from
        #       JobConfig(..) but we can't make JobConfig.options.parseArguments()
        #       to parse anything other than sys.argv without modifying sys.argv temporarily...
        campaign = kwargs.get('campaign', "")

        jobConfig = JobConfig(**kwargs)

        sm = SamplesManager("$CMSSW_BASE/src/%s/MetaData/data/%s/datasets*.json" % (jobConfig.metaDataSrc, campaign),
                            jobConfig.crossSections,
                            )

        hasProblem = False

        # loop over all types of processes (data, signal, background)
        for dsList in self.options.processes.values():
            for idsName in dsList:
                #if args were provided for this dataset, then it is a list...
                if isinstance(idsName, list):
                    dsName=idsName[0]
                #... or just a string
                else:
                    dsName=idsName

                # check if this datasets was selected
                if not self.isSelectedDataset(dsName):
                    # skip this dataset
                    continue

                #----------
                try:
                    name, xsec, totEvents, files, maxEvents = sm.getDatasetMetaData(jobConfig.maxEvents, dsName, jobId=-1, nJobs= jobConfig.nJobs)

                    if xsec == None:
                        print >> sys.stderr,"cross section for",dsName,"not found in",jobConfig.crossSections
                        hasProblem = True

                except Exception, ex:
                    if type(ex) == Exception and (
                        ex.message.startswith('No dataset matched the request:') or
                        ex.message.startswith('More then one dataset matched the request:')):
                        print >> sys.stderr, ex.message
                    else:
                        raise

            # end of loop over all processes
        # end of loop over group of process types

        if hasProblem:
            print >> sys.stderr,"problems found, exiting"
            sys.exit(1)

    # -------------------------------------------------------------------------------------------------------------------        
