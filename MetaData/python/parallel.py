from Queue import Queue

import commands
import subprocess
import os,sys
import getpass
import copy

import htcondor

from threading import Thread, Semaphore
import threading 
from multiprocessing import cpu_count
from time import sleep
from math import floor

# -----------------------------------------------------------------------------------------------------
class BatchRegistry:
    
    domain = None
    host   = None
    proxy  = None
    autoprint  = True
    domains_map = {
        "cern.ch"          : "htcondor",
        "psi.ch"           : "sge",
        "hep.ph.ic.ac.uk"  : "sge",
        "irfu.ext"         : "iclust",
        }
    
    #----------------------------------------
    @staticmethod
    def getRunner(batchSystem):
        if batchSystem == "auto": batchSystem = BatchRegistry.getBatchSystem()        
        if batchSystem == "lsf" : return LsfJob
        elif batchSystem == "htcondor": return HTCondorJob
        elif batchSystem == "sge": return SGEJob
        elif batchSystem == "iclust": return IclustJob
        else:
            raise Exception,"Unrecognized batchSystem: %s" % batchSystem

    #----------------------------------------
    @staticmethod
    def getBatchSystem(domain=None):
        if not domain:
            domain = BatchRegistry.getDomain()
        if not domain in BatchRegistry.domains_map:
            raise Exception,"Could not automatically determine the batchSystem for domain %s. Please specify it manually.\n Known domains are: %s\n" % ( domain, " ".join(BatchRegistry.domains_map.keys()) )
        ret = BatchRegistry.domains_map[domain]
        if BatchRegistry.autoprint:
            print( "\nINFO: We are at '%s', so we will use '%s' as batch system.\n     Please specify the batch system on the command line if you are not happy with this." % (domain,ret) )
            BatchRegistry.autoprint = False
        return ret
    
    #----------------------------------------
    @staticmethod
    def getMonitor(batchSystem):
        if batchSystem == "auto": batchSystem = BatchRegistry.getBatchSystem(BatchRegistry.getDomain())        
        if batchSystem == "lsf" : return LsfMonitor
        elif batchSystem == "htcondor": return HTCondorMonitor
        elif batchSystem == "sge": return SGEMonitor
        elif batchSystem == "iclust": return IclustMonitor
        else:
            raise Exception,"Unrecognized batchSystem: %s" % batchSystem

    #----------------------------------------
    @staticmethod
    def getDomain():
        if not BatchRegistry.domain:
            stat,out = commands.getstatusoutput("hostname -d")
            if stat:
                raise Exception,"Unable to determine domain via `hostname':\n%s" % out
            if out == "":
                    stat, out = commands.getstatusoutput("domainname")
                    if stat:
                        raise Exception,"Unable to determine domain via `domainname':\n%s" % out
            BatchRegistry.domain = out.strip("\n")
        return BatchRegistry.domain

    #----------------------------------------
    @staticmethod
    def getHost():
        if not BatchRegistry.host:
            stat,out = commands.getstatusoutput("hostname -s")
            if stat:
                raise Exception,"Unable to determine domain:\n%s" % out
            BatchRegistry.host = out.strip("\n")
        return BatchRegistry.host

    #----------------------------------------
    @staticmethod
    def getProxy():
        if not BatchRegistry.proxy:
            stat,out = commands.getstatusoutput("voms-proxy-info -e --valid 5:00")
            if stat:
                raise Exception,"voms proxy not found or validity  less than 5 hours:\n%s" % out
            stat,out = commands.getstatusoutput("voms-proxy-info -p")
            out = out.strip().split("\n")[-1] # remove spurious java info at ic.ac.uk
            if stat:
                raise Exception,"Unable to voms proxy:\n%s" % out
            BatchRegistry.proxy = "%s:%s" % ( BatchRegistry.getHost(), out.strip("\n") )
        return BatchRegistry.proxy
            
# -----------------------------------------------------------------------------------------------------
class WorkNodeJob(object):
    
    nwarnings = 1
    
    #----------------------------------------
    def __init__(self,*args,**kwargs):
        
        
        self.runner          = kwargs.pop("runner",LsfJob)
        self.stage_dest      = kwargs.pop("stage_dest")
        self.stage_cmd       = kwargs.pop("stage_cmd")
        self.stage_patterns  = kwargs.pop("stage_patterns")
        self.tarball         = kwargs.pop("tarball",None)
        self.job_outdir      = kwargs.pop("job_outdir",None)
        self.copy_proxy      = kwargs.pop("copy_proxy",True)

        self.runner = self.runner(*args,**kwargs)
        
    def __getattr__(self,name):
        ## return runner attributes
        if hasattr(self.runner,name):
            return getattr(self.runner,name)
            
        ## raise AttributeError, "No attribute %s in runner instance %s" % (name,str(self.runner))
        return object.__getattr__(name)
           
    def __str__(self):
        return "WorkNodeJob: [%s]" % ( self.runner )

    #----------------------------------------
    def __call__(self,cmd):

        self.cmd = cmd
        runner_kwargs = {}
        script = ""
        script += "#!/bin/bash\n"
        
        # get specific preamble needed by the runner
        #     this commands are expected to bring the process to the job working directory in the worker node
        script += self.runner.preamble()+"\n"
        
        # copy grid proxy over
        if self.copy_proxy and self.runner.copyProxy():
            try:
                proxy = BatchRegistry.getProxy()
                script += "scp -p %s .\n" % proxy
                proxyname = os.path.basename(proxy.split(":")[1])
                script += "export X509_USER_PROXY=$PWD/%s\n" % proxyname
                if WorkNodeJob.nwarnings > 0:
                    WorkNodeJob.nwarnings -= 1
                    print 
                    print
                    print "WARNING: We are counting fact that the jobs will be able to scp the follwing file: %s" % proxy
                    print "         Please make sure that ssh is properly configured."
                    print "         Alternatively, you can copy the proxy to your home folder, set the variable X509_USER_PROXY accordingly and run with the --no-copy-proxy"
                    print 
            except Exception, e:
                if WorkNodeJob.nwarnings > 0:
                    WorkNodeJob.nwarnings -= 1
                    print "WARNING: I could not find a valid grid proxy. This may cause problems if the jobs will read the input through AAA."
                    print e
        else:
            try:
                proxy = BatchRegistry.getProxy()
                proxyname = proxy.split(":")[1]
                script += "export X509_USER_PROXY=%s\n" % proxyname
                if WorkNodeJob.nwarnings > 0:
                    WorkNodeJob.nwarnings -= 1
                    print 
                    print
                    print "WARNING: We are counting on the path for this proxy being visible to the remote jobs:",proxyname
                    print 
            except Exception, e:
                if WorkNodeJob.nwarnings > 0:
                    WorkNodeJob.nwarnings -= 1
                    print "WARNING: I could not find a valid grid proxy. This may cause problems if the jobs will read the input through AAA."
                    print e


                
        # set-up CMSSW
        script += "WD=$PWD\n"
        script += "echo\n"
        script += "echo\n"
        script += "echo\n"
        
        if not self.tarball:
            script += "cd " + os.environ['CMSSW_BASE']+"\n"
        else:
            script += "source $VO_CMS_SW_DIR/cmsset_default.sh\n"
            script += "export SCRAM_ARCH=%s\n" % os.environ['SCRAM_ARCH']
            script += "scram project CMSSW %s\n" % os.environ['CMSSW_VERSION']
            script += "cd %s\n" % os.environ['CMSSW_VERSION']
            script += "tar zxf %s\n" % self.tarball
            script += "cp src/XGBoostCMSSW/XGBoostInterface/toolbox/*xml config/toolbox/$SCRAM_ARCH/tools/selected/\n"
            script += "scram setup rabit\n"
            script += "scram setup xgboost\n"
            script += "scram b\n"
            
        script += "eval $(scram runtime -sh)"+"\n"
        script += "cd $WD\n"
        
        if self.job_outdir:
            script += "mkdir %s\n" % self.job_outdir

        script += 'echo "ls $X509_USER_PROXY"\n'
        script += 'ls $X509_USER_PROXY\n'
        
        ###---dasmaps: allow jobs to use dasgoclient
        script += 'mkdir .dasmaps \n'
        script += 'mv das_maps_dbs_prod.js .dasmaps/ \n\n'

        ###---The user command
        ###
        ###   HTCondor: In order to deal with failed jobs the original jobIds of the failed jobs
        ###   are stored in dummy cmd line option in job_utils.py.
        ###   before submitting a new task the dummy options, if present, is stripped from 
        ###   the cmd line and added to the script properly
        htc = False
        if 'resubMap=' in cmd:            
            htc = True
            resubMap = cmd[cmd.find('resubMap='):cmd.find(' ', cmd.find('resubMap=')) if cmd.find(' ', cmd.find('resubMap='))>0 else len(cmd)]       
            cmd = cmd.replace(resubMap, '')
            resubMap = resubMap.replace('resubMap=', '')
            script += 'declare -a jobIdsMap=('+resubMap.replace(',', ' ')+')\n'
            runner_kwargs['njobs'] = len(resubMap.split(','))

        script += cmd+"\n"
        # HTCondor: return value is 0 if cmd runs successfully, otherwise retval is set to the jobId in order
        # to keep track of failed jobs. To avoid overlaps with the success code the retval is set to jobId+1
        script += 'retval=$?\n'
        if htc:
            script += 'if [[ $retval != 0 ]]; then\n'
            script += '    retval=$(( ${jobIdsMap[${1}]} + 1 )) \n'
            script += 'fi \n'
        
        if self.tarball and self.job_outdir:
            script += 'cd %s\n' % self.job_outdir
            script += 'echo\n'
            script += 'echo\n'
            script += 'echo "Job finished with exit code ${retval}"\n'
            script += 'echo "Files in ouput folder"\n'
            script += 'ls -ltr\n'
        # stage out files
        script += 'if [[ $retval == 0 ]]; then\n'
        script += '    errors=""\n'
        script += '    for file in $(find -name %s); do\n' % " -or -name ".join(self.stage_patterns)
        # script += '        %s $file %s\n' % ( self.stage_cmd, self.stage_dest )
        filesOutPath = '/eos/user/a/atishelm/ntuples/Event_Dumper/HHWWgg_2017'
        script += '        %s $file %s\n' % ( self.stage_cmd, filesOutPath )
        script += '        if [[ $? != 0 ]]; then\n'
        script += '            errors="$errors $file($?)"\n'
        script += '        fi\n'
        script += '    done\n'
        script += '    if [[ -n "$errors" ]]; then\n'
        script += '       echo "Errors while staging files"\n'
        script += '       echo "$errors"\n'
        script += '       exit -2\n'
        script += '    fi\n'
        script += 'fi\n'        

        # get specific epilogue needed by the runner
        # this can be used, for example, to propagate $retval by touching a file
        script += self.runner.epilogue("cp -pv",os.path.abspath(self.job_outdir))+"\n"

        script += 'exit $retval\n'
        script += '\n'
        
        return self.runner.run(script, **runner_kwargs)


# -----------------------------------------------------------------------------------------------------
class WorkNodeJobFactory(object):
    
    # ------------------------------------------------------------------------------------------------
    def __init__(self,stage_dest,
                 stage_cmd="cp -pv",stage_patterns=["'*.root'","'*.xml'"],job_outdir=None,runner=None,batchSystem="auto",copy_proxy=True):
        
        if not runner:
            self.runner = BatchRegistry.getRunner(batchSystem)
            
        self.stage_dest = stage_dest
        self.stage_cmd  = stage_cmd
        self.stage_patterns  = stage_patterns
        self.tarball = None
        self.job_outdir      = job_outdir
        self.copy_proxy      = copy_proxy

    # ------------------------------------------------------------------------------------------------
    def stageDest(self,dest):
        self.stage_dest = dest

    # ------------------------------------------------------------------------------------------------
    def setTarball(self,tarball):
        self.tarball = tarball
        
    # ------------------------------------------------------------------------------------------------
    def mkTarball(self,tarball=None,
                  tarball_entries=["python","lib","bin","external","flashgg/MetaData/python/PU_MixFiles_2017_miniaodv2_310"],tarball_patterns=[("src/*","data")],
                  tarball_transform=None):
        
        self.tarball = tarball
        content=tarball_entries

        for folder,pattern in tarball_patterns:
            stat,out = commands.getstatusoutput("cd $CMSSW_BASE; find %s -name %s" % ( folder, pattern ) )
            ## print out
            if stat != 0:
                print "error (%d) finding files to create job tarball" % stat
                print "folder : %s"
                print "pattern: %s"
                print out
                sys.exit(stat)
            content.extend( [f for f in out.split("\n") if f != ""] )                
        args = []
        if tarball_transform:
            args.extend( ["--transform",tarball_transform] )
        args.extend(["-h","--show-transformed","-zvcf",tarball])
        args.extend(content)
        print 
        print "Preparing tarball with the following content:"
        print "\n".join(content)
        print
        stat,out =  commands.getstatusoutput("cd $CMSSW_BASE; tar %s" % " ".join(args) )

        if stat != 0:
            print "error (%d) creating job tarball"
            print "CMSSW_BASE: %s" % os.environ["CMSSW_BASE"]
            print "args: %s" % " ".join(args)
            print out
    

    #----------------------------------------
    def getStageCmd(self):
        
        stage_cmd = self.stage_cmd
        if stage_cmd != "guess":
            return stage_cmd

        if self.stage_dest.startswith("/store"):
            stage_cmd = "cmsStage"
        elif self.stage_dest.startswith("root://"):
            stage_cmd = "xrdcp"
        elif self.stage_dest.startswith("rsync") or "@" in self.stage_dest or "::" in self.stage_dest:
            stage_cmd = "rsync -av"
        else:
            stage_cmd = "cp -pv"

        return stage_cmd
            
    #----------------------------------------
    def __call__(self,*args,**kwargs):
        
        stage_cmd = self.getStageCmd()
            
        kwargs["runner"]         = self.runner
        kwargs["stage_dest"]     = self.stage_dest
        kwargs["stage_cmd"]      = stage_cmd
        kwargs["stage_patterns"] = self.stage_patterns
        kwargs["tarball"]        = self.tarball
        kwargs["job_outdir"]     = self.job_outdir
        kwargs["copy_proxy"]     = self.copy_proxy
        
        return WorkNodeJob(*args,**kwargs)

# -----------------------------------------------------------------------------------------------------
class HTCondorJob(object):
    """ a thread to run condor_submit and wait until it completes """

    #----------------------------------------
    def __init__(self, htcondorQueue, jobName="", async=True, replacesJob=None, copy_proxy=False):
        """ 
        @param cmd is the command to be executed inside the bsub script. Some CMSSW specific wrapper
        code will be added
        """
        
        # super(LsfJob,self).__init()
        
        self.htcondorQueue = htcondorQueue
        self.jobName = jobName
        self.cfgName = jobName+".sub"
        self.execName = jobName+".sh"        
        self.jobid = None
        self.cmd = None
        self.replacesJob = replacesJob
        self.copy_proxy = copy_proxy

        if async != True:
            print "HTCondorJob: synchronous job processing is not supported by for HTCondor jobs ... running async jobs instead"

        self.async = True
        
    def __str__(self):
        return "HTCondorJob: [%s] [%s]" % ( self.htcondorQueue, self.cfgName)
        
    def setJobId(self,jobid):
        self.jobid = jobid
        
    #----------------------------------------
    def preamble(self):
        # fix to ensure AAA redirecting works properly on lxplus/lxbatch
        # see https://hypernews.cern.ch/HyperNews/CMS/get/wanaccess/448/1/1.html
        return "export XRD_NETWORKSTACK=IPv4"

    #----------------------------------------
    def epilogue(self,cmd,dest):
        return ""

    def copyProxy(self):
        return True

    #----------------------------------------
    def run(self, script, njobs=1):
                
        logdir = os.path.dirname(self.jobName)
        if not os.path.exists(logdir):
            os.mkdir(logdir)
                
        with open(self.execName, "w+") as fout:
            fout.write(script)
            fout.close()        

        with open(self.cfgName, "w+") as fout:
            fout.write('+JobFlavour   = "'+self.htcondorQueue+'"\n\n')
            fout.write('+OnExitHold   = ExitStatus != 0 \n\n')
            fout.write('periodic_release =  (NumJobStarts < 4) && ((CurrentTime - EnteredCurrentStatus) > 60) \n\n')
            fout.write('input         = %s/.dasmaps/das_maps_dbs_prod.js \n' % os.environ['HOME'])
            fout.write('executable    = '+self.execName+'\n')
            fout.write('arguments     = $(ProcId)\n')
            #fout.write('arguments   = $(ProcId)\n')
            if self.copy_proxy:
                fout.write('input       = '+BatchRegistry.getProxy().split(":")[1]+'\n')
            fout.write('output        = '+self.jobName+'_$(ClusterId).$(ProcId).out\n')
            fout.write('error         = '+self.jobName+'_$(ClusterId).$(ProcId).err\n')
            fout.write('log           = '+self.jobName+'_$(ClusterId).$(ProcId)_htc.log\n\n')
            fout.write('max_retries   = 1\n')
            fout.write('queue '+str(njobs)+' \n')
            fout.close()        

        import subprocess
        htc = subprocess.Popen("condor_submit "+self.cfgName,
                               shell=True, # bufsize=bufsize,
                               stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE,
                               close_fds=True)
            
        out,err = htc.communicate()

        self.exitStatus = htc.returncode
        
        if self.exitStatus != 0:
            print "error running job", self.jobName, self.exitStatus
            print out
            print err
        else:
            self.jobid = []
            for line in out.split("\n"):
                if "cluster" in line:
                    clusterid = line.split()[-1]
                    break
            for ijob in range(int(njobs)):
                self.jobid.append(clusterid+str(ijob))

        if self.async:
            return self.exitStatus, (out,(self.jobName,self.jobid))

        return self.handleOutput()

    #----------------------------------------
    def __call__(self,cmd):
        
        script = ""
        script += "#!/bin/bash\n"
        
        script += "cd " + os.environ['CMSSW_BASE']+"\n"
        script += "eval `scram runtime -sh`"+"\n"
        script += "cd " + os.getcwd()+"\n"
        
        if os.environ.get('X509_USER_PROXY',None):
            script += "export X509_USER_PROXY=%s\n" % os.environ['X509_USER_PROXY']

        ###---In order to deal with failed jobs the original jobIds of the failed jobs
        ###   are stored in dummy cmd line option in job_utils.py.
        ###   before submitting a new task the dummy options, if present, is stripped from 
        ###   the cmd line and added to the script properly
        # if 'resubMap=' in cmd:     
        #     resubMap = cmd[cmd.find('resubMap='):cmd.find(' ', cmd.find('resubMap=')) if cmd.find(' ', cmd.find('resubMap='))>0 else len(cmd)]       
        #     cmd = cmd.replace(resubMap, '')
        #     resubMap = resubMap.replace('resubMap=', '')
        #     script += 'declare -a jobIdsMap=('+resubMap.replace(',', ' ')+')\n'
        # else:
        #     if 'nJobs=' in cmd:
        #         njobs = cmd[cmd.find('nJobs=')+len('nJobs='):cmd.find(' ', cmd.find('nJobs='))]
        #         resubMap = ' '.join([str(x) for x in range(int(njobs))])
        #         script += 'declare -a jobIdsMap=('+resubMap.replace(',', ' ')+')\n'
        #     else:
        #         script += 'declare -a jobIdsMap=(0)\n'

        self.cmd = cmd
        
        #---the user command
        script += cmd+"\n"
        
        return self.run(script)
        
    
    #----------------------------------------
    def handleOutput(self, jobid):
        self.exitStatus = -1
        if self.async and jobid:
            evt_list = []       
            jel = htcondor.JobEventLog(str(self.jobName+"_"+jobid+"_htc.log"))
            for event in jel.events(stop_after=0):
                evt_jobid_str = str(event.cluster)+'.'+str(event.proc)
                if evt_jobid_str == jobid:
                    evt_list.append(event)
            jel.close()

            #---Normal exit
            if evt_list[-1].type is htcondor.JobEventType.JOB_TERMINATED:                    
                self.exitStatus = evt_list[-1]["ReturnValue"]
            #---Job killed by user
            elif evt_list[-1].type is htcondor.JobEventType.JOB_ABORTED:
                with open(self.execName, "r") as fin:
                    lines = fin.readlines()
                    jobIdsMap = []
                    for line in lines:
                        if 'jobIdsMap=' in line:
                            jobIdsMap = [int(x) for x in line[line.find('(')+1:line.find(')')].split()]
                self.exitStatus = jobIdsMap[evt_list[-1].proc] if len(jobIdsMap)>0 else -1

            ###---get job exit code from either condor_history or the logfile
            # status, log = commands.getstatusoutput("condor_history %s -long -attributes ExitCode" % self.jobid )
            # if status == 0 and len(log) > 0:
            #     msg = log.split("\n")[0].replace("=", "").split()[-1]
            #     if msg == "0":
            #         self.exitStatus = 0                    
            #     else:
            #         self.exitStatus = -1
            # else:
            #     with open(self.jobName+".out") as log:
            #         lines = log.readlines()
            #         for line in lines:
            #             if "Job finished with exit code" in line:
            #                 exit_code = line.split()[-1]
            #                 if exit_code == "0":
            #                     self.exitStatus = 0                    
            #                 else:
            #                     self.exitStatus = -1                                
                
        ###---collect both the stdout and stderr
        output = ""
        if self.jobName:
            output += "STDOUT:\n"
            try:
                with open("%s_%s.out" % (self.jobName, jobid)) as lf:
                    output = lf.read()
                    lf.close()
                    output += "%s_%s.out\n" % (self.jobName, jobid)
            except:
                output = "%s_%s.out\n" % (self.jobName, jobid)
            output += "STDERR:\n"
            try:
                with open("%s_%s.err" % (self.jobName, jobid)) as lf:
                    output = lf.read()
                    lf.close()
                    output += "%s_%s.err\n" % (self.jobName, jobid)
            except:
                output+= "%s_%s.err\n" % (self.jobName, jobid)
        
        return self.exitStatus, output, jobid

# -----------------------------------------------------------------------------------------------------
class HTCondorMonitor(object):
    def __init__(self,jobsqueue,retqueue):
        self.jobsqueue = jobsqueue
        self.retqueue = retqueue
        self.stop = False

    def __call__(self):
        self.jobsmap = {}
        self.jobids = {}
        self.clonesMap = {}

        while not self.stop:
            if not self.jobsqueue.empty():
                job = self.jobsqueue.get()
                if job.jobid == None:
                    print "INTERNAL ERROR: job id not set %s" % job
                    self.retqueue.put( (job, [job.cmd], job.handleOutput(job.jobid)) )
                else:
                    for jobid in job.jobid:
                        self.jobsmap[jobid] = job
                    self.jobids[job.jobName] = job.jobid
                    
                    if job.replacesJob:
                        if not job.replacesJob in clonesMap:
                            self.clonesMap[job.replacesJob] = [jobids[job.replacesJob]]                        
                        self.clonesMap[job.replacesJob].append(job.jobid)
            
            self.monitor()
            sleep(0.1)


    def cancel(self,jobid):
        commands.getstatusoutput("condor_rm %d" % jobid)
        
    def monitor(self):
        if len(self.jobsmap.keys())==0:
            return
            
        current_jobs = copy.deepcopy(self.jobsmap)        
        evt_list = {}
        for jobid, job in current_jobs.iteritems():
            evt_list[jobid] = []
            jel = htcondor.JobEventLog(str(job.jobName+"_"+jobid+"_htc.log"))
            for event in jel.events(stop_after=0):
                evt_jobid_str = str(event.cluster)+'.'+str(event.proc)
                if evt_jobid_str == jobid:
                    evt_list[jobid].append(event)                    
            jel.close()

        for jobid, events in evt_list.iteritems():
            if len(events)>0 and (events[-1].type is htcondor.JobEventType.JOB_TERMINATED or events[-1].type is htcondor.JobEventType.JOB_ABORTED):
                self.jobFinished(jobid, events[-1])

    def jobFinished(self,jobid,status):
        if not jobid in self.jobsmap:
            print "%s not found: %s" % ( jobid, " ".join(self.jobsmap.keys()) )
            return
        htcJob = self.jobsmap[jobid]
        try:
            self.retqueue.put( (htcJob, [htcJob.cmd], htcJob.handleOutput(jobid)) )
        except Exception, e:
            print "Error getting output of %s " % htcJob
            print jobid, status
            print e
                    
        ancestor = htcJob.replacesJob if htcJob.replacesJob else htcJob.jobName
        if ancestor in self.clonesMap:
            for clone in self.clonesMap[ancestor]:
                if clone != jobid:
                    self.cancel(clone)
                self.jobsmap.pop(clone)
                        
        self.jobsmap.pop(jobid)
    
# -----------------------------------------------------------------------------------------------------
class LsfJob(object):
    """ a thread to run bsub and wait until it completes """

    #----------------------------------------
    def __init__(self, lsfQueue, jobName="", async=False, replacesJob=None):
        """ @param cmd is the command to be executed inside the bsub script. Some CMSSW specific wrapper
            code will be added
        """
        
        # super(LsfJob,self).__init()
        
        self.lsfQueue = lsfQueue
        self.jobName = jobName
        self.async = async
        self.jobid = None
        self.cmd = None
        self.replacesJob = replacesJob
        
    def __str__(self):
        return "LsfJob: [%s] [%s] [%s]" % ( self.lsfQueue, self.jobName, self.jobid )
        
    def setJobId(self,jobid):
        self.jobid = jobid
    #----------------------------------------
    def preamble(self):
        # fix to ensure AAA redirecting works properly on lxplus/lxbatch
        # see https://hypernews.cern.ch/HyperNews/CMS/get/wanaccess/448/1/1.html
        return "export XRD_NETWORKSTACK=IPv4"

    #----------------------------------------
    def epilogue(self,cmd,dest):
        return ""

    def copyProxy(self):
        return True

    #----------------------------------------
    def run(self,script):
        
        bsubCmdParts = [ "bsub",
                         "-q " + self.lsfQueue,
                         ]
        
        if not self.async:
            bsubCmdParts.append("-K")  # bsub waits until job completes
            
        if( self.jobName ):
            logdir = os.path.dirname(self.jobName)
            if not os.path.exists(logdir):
                os.mkdir(logdir)
            bsubCmdParts.append("-J " + self.jobName)
            bsubCmdParts.append("-o %s.log" % self.jobName)
        
        bsubCmd = " ".join(bsubCmdParts)
        
        import subprocess
        lsf = subprocess.Popen(bsubCmd, shell=True, # bufsize=bufsize,
                               stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE,
                               close_fds=True)
        
        with open("%s.sh" % self.jobName,"w+") as fout:
            fout.write(script)
            fout.close()        
        
        out,err = lsf.communicate(script)
        
        # wait for the job to complete
        ## self.exitStatus = lsf.wait()
        self.exitStatus = lsf.returncode
        
        if self.exitStatus != 0:
            print "error running job",self.jobName, self.exitStatus
            print out
            print err
        else:
            self.jobid = None
            for line in out.split("\n"):
                if line.startswith("Job <"):
                    self.jobid = int(line.split("<",1)[1].split(">",1)[0])
                    break
            
        if self.async:
            return self.exitStatus, (out,(self.jobName,self.jobid))

        return self.handleOutput()

    #----------------------------------------
    def __call__(self,cmd):
        
        self.cmd = cmd
        script = ""
        script += "#!/bin/bash\n"
        
        script += "cd " + os.environ['CMSSW_BASE']+"\n"
        script += "eval `scram runtime -sh`"+"\n"
        script += "cd " + os.getcwd()+"\n"
        
        if os.environ.get('X509_USER_PROXY',None):
            script += "export X509_USER_PROXY=%s\n" % os.environ['X509_USER_PROXY']
                    
        # the user command
        script += cmd+"\n"
        
        return self.run(script)
        
    
    #----------------------------------------
    def handleOutput(self):
        ## print "handleOutput"
        if self.async:
            self.exitStatus = -1
            result = commands.getstatusoutput("bjobs %s" % str(self.jobid))

            for line in result[1].split("\n"):
                if line.startswith(str(self.jobid)):
                    exitStatus = [ l for l in line.split(" ") if l != "" ][2]
                    if exitStatus == "DONE":
                        self.exitStatus = 0
                    else:
                        self.exitStatus = -1
                    break
            
        output = ""
        if self.jobName:
            try:
                with open("%s.log" % self.jobName) as lf:
                    output = lf.read()
                    lf.close()
                    output += "%s.log\n" % self.jobName
            except:
                output = "%s.log" % self.jobName
        
        return self.exitStatus, output        
    
# -----------------------------------------------------------------------------------------------------
class LsfMonitor(object):
    def __init__(self,jobsqueue,retqueue):
        self.jobsqueue = jobsqueue
        self.retqueue = retqueue
        self.stop = False

    def __call__(self):
        self.jobsmap = {}
        self.jobids = {}
        self.clonesMap = {}

        while not self.stop:
            if not self.jobsqueue.empty():
                job = self.jobsqueue.get()
                if job.jobid == None:
                    print "INTERNAL ERROR: job id not set %s" % job
                    self.retqueue.put( (job, [job.cmd], job.handleOutput()) )
                else:
                    self.jobsmap[str(job.jobid)] = job
                    self.jobids[job.jobName] = job.jobid
                    
                    if job.replacesJob:
                        if not job.replacesJob in clonesMap:
                            self.clonesMap[job.replacesJob] = [jobids[job.replacesJob]]                        
                        self.clonesMap[job.replacesJob].append(job.jobid)
            
            self.monitor()
            sleep(0.1)


    def cancel(self,jobid):
        commands.getstatusoutput("bkill -s 9 %d" % jobid)
        
    def monitor(self):
        if len(self.jobsmap.keys())==0:
            return
        status = commands.getstatusoutput("bjobs %s" % " ".join(self.jobsmap.keys()))
        for line in status[1].split("\n")[1:]:
            toks = [ l for l in line.split(" ") if l != "" ]
            jobid = toks[0]
            status = toks[2]
            if status in ["DONE","EXIT","ZOMBI","UNKWN"]:
                self.jobFinished(jobid,status)
                
    def jobFinished(self,jobid,status):
        if not jobid in self.jobsmap:
            print "%s not found: %s" % ( jobid, " ".join(self.jobsmap.keys()) )
            return
        lsfJob = self.jobsmap[jobid]
        try:
            self.retqueue.put( (lsfJob, [lsfJob.cmd], lsfJob.handleOutput()) )
        except Exception, e:
            print "Error getting output of %s " % lsfJob
            print jobid, status
            print e
                    
        ancestor = lsfJob.replacesJob if lsfJob.replacesJob else lsfJob.jobName
        if ancestor in self.clonesMap:
            for clone in self.clonesMap[ancestor]:
                if clone != jobid:
                    self.cancel(clone)
                self.jobsmap.pop(clone)
                        
        self.jobsmap.pop(jobid)

# -----------------------------------------------------------------------------------------------------
class SGEJob(LsfJob):
    """ a thread to run qsub and wait until it completes """
    def __init__(self,*args,**kwargs):
        self.rebootMitigation = (BatchRegistry.getDomain() in ["hep.ph.ic.ac.uk"])
        
        super(SGEJob, self).__init__(*args, **kwargs)

    def __str__(self):
        return "SGEJoB: [%s] [%s] [%s]" % ( self.lsfQueue, self.jobName, self.jobid )

    def preamble(self):
        ret = ""
        mydomain = BatchRegistry.getDomain()
        # domain-specific configuration
        if mydomain == "psi.ch":
            ret += "source $VO_CMS_SW_DIR/cmsset_default.sh\n"
            ret += "export WORKDIR=$TMPDIR/sgejob-$JOB_ID\n"
            ret += "mkdir -p $WORKDIR\n"
            ret += "cd $WORKDIR\n"
            ret += 'echo "Current directory is $WORKDIR"\n'
            ret += "source $VO_CMS_SW_DIR/cmsset_default.sh"
        if mydomain == "hep.ph.ic.ac.uk":
            ret += "mkdir -p $TMP/sgejob-$JOB_ID\n"
            ret += "cd $TMP/sgejob-$JOB_ID\n"
        return ret

    def epilogue(self,cmd,dest):
        ret = ""
        ret += 'if [[ $retval == 0 ]]; then\n'
        fn = "%s.sh.done" % (self.jobName.split("/")[-1])
        ret += '    touch %s\n' % fn
        ret += '    %s %s %s\n' % ( cmd, fn, dest )
        ret += 'else\n'
        fn = "%s.sh.fail" % (self.jobName.split("/")[-1])
        ret += '    touch %s\n' % fn
        ret += '    %s %s %s\n' % ( cmd, fn, dest )
        ret += 'fi\n'
        return ret

    def copyProxy(self):
        mydomain = BatchRegistry.getDomain()
        # domain-specific configuration
        if mydomain == "hep.ph.ic.ac.uk":
            return False
        return True
        
    def run(self,script):

        mydomain = BatchRegistry.getDomain()

        if mydomain == "hep.ph.ic.ac.uk":
            qsubCmdParts = [ "qsub", "-q hep.q" ]
            if self.lsfQueue == "hepshort.q":
                qsubCmdParts.append("-l h_rt=3:0:0")
            elif self.lsfQueue == "hepmedium.q":
                qsubCmdParts.append("-l h_rt=6:0:0")
            else:
                # assume long queue is intended
                qsubCmdParts.append("-l h_rt=48:0:0")
        else:
            qsubCmdParts = [ "qsub",
                             "-q " + self.lsfQueue,
                             ]

        if not self.async:
            qsubCmdParts.append("-sync y")  # qsub waits until job completes

        if( self.jobName ):
            logdir = os.path.abspath(os.path.dirname(self.jobName))
            logfile = "%s/%s.log" % (logdir, os.path.basename(self.jobName))
            if not os.path.exists(logdir):
                os.mkdir(logdir)
            qsubCmdParts.append("-N " + self.jobName.strip("/").replace("/","_"))
            qsubCmdParts.append("-o %s -e %s" % (logfile,logfile))

        qsubCmd = " ".join(qsubCmdParts)

        import subprocess
        sge = subprocess.Popen(qsubCmd, shell=True, # bufsize=bufsize,                                                                                                                                                               
                               stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE,
                               close_fds=True)
        
        with open("%s.sh" % self.jobName,"w+") as fout:
            fout.write(script)
            fout.close()
        out,err = sge.communicate(script)
                                                         
        self.exitStatus = sge.returncode

        if self.exitStatus != 0:
            print "error running job",self.jobName, self.exitStatus
            print out
            print err
        else:
            self.jobid = None
            for line in out.split("\n"):
                if line.startswith("Your job"):
                    self.jobid = int(line.split(" ")[2])
                    break
            
        if self.async:
            return self.exitStatus, (out,(self.jobName,self.jobid))

        return self.handleOutput()

    def handleOutput(self):

        if self.async:
            result = commands.getstatusoutput("qstat")
            checkcount = 0
            if (self.rebootMitigation):
                while len(result[1].split("\n")) < 3:
                    print "SGE REBOOT MITIGATION: qstat result seems too short"
                    print "---------------------------------------------------"
                    print result[1]
                    print "---------------------------------------------------"
                    print "... so we wait before trying again"
                    sleep(5.)
                    result = commands.getstatusoutput("qstat")
                    checkcount += 1
                    if checkcount > 12:
                        break
                    
            self.exitStatus = 0 # assume it is done unless listed
            for line in result[1].split("\n"):
                if line.startswith(str(self.jobid)):
                    self.exitStatus = -1
                    break
            if self.exitStatus == 0:
                logdir = os.path.abspath(os.path.dirname(self.jobName))
                ## if os.path.isfile('%s/%s.sh.fail' % (logdir,self.jobName.split("/")[-1])) and not os.path.isfile('%s/%s.sh.done' % (logdir,self.jobName.split("/")[-1])):
                if not os.path.isfile('%s/%s.sh.done' % (logdir,self.jobName.split("/")[-1])):
                    self.exitStatus = 1

        output = ""
        if self.jobName:
            try:
                with open("%s.log" % self.jobName) as lf:
                    output = lf.read()
                    lf.close()
            except:
                output = "%s.log" % self.jobName

        return self.exitStatus, output

# -----------------------------------------------------------------------------------------------------
class IclustJob(LsfJob):
    """ a thread to run qsub and wait until it completes """

    def __str__(self):
        return "IclustJob: [%s] [%s] [%s]" % ( self.lsfQueue, self.jobName, self.jobid )

    def preamble(self):
        ret = ""
        mydomain = BatchRegistry.getDomain()
        # domain-specific configuration
        if mydomain == "irfu.ext":
            # from the submission directory... FIXME
            ret += 'tmpdir="`mktemp -d sgejob.XXXXXXXXXX`"\n'
            ret += "cd $tmpdir\n"
        return ret

    def epilogue(self,cmd,dest):
        ret = ""
        ret += 'if [[ $retval == 0 ]]; then\n'
        fn = "%s.sh.done" % (self.jobName.split("/")[-1])
        ret += '    touch %s\n' % fn
        ret += 'else\n'
        fn = "%s.sh.fail" % (self.jobName.split("/")[-1])
        ret += '    touch %s\n' % fn
        ret += 'fi\n'
        return ret

    def copyProxy(self):
        mydomain = BatchRegistry.getDomain()
        # domain-specific configuration
        if mydomain == "irfu.ext":
            return False
        return True
        
    def run(self,script):

        qsubCmdParts = [ "clubatch" ]

        #if not self.async:
        #    qsubCmdParts.append("-sync y")  # qsub waits until job completes

        if( self.jobName ):
            logdir = os.path.abspath(os.path.dirname(self.jobName))
            logfile = "%s/%s.log" % (logdir, os.path.basename(self.jobName))
            if not os.path.exists(logdir):
                os.mkdir(logdir)
            #qsubCmdParts.append("-N " + self.jobName.strip("/").replace("/","_"))
            #qsubCmdParts.append("-o %s -e %s" % (logfile,logfile))

        qsubCmd = " ".join(qsubCmdParts)

        scriptname = "%s.sh" % self.jobName
        with open(scriptname,"w+") as fout:
            fout.write(script)
            fout.close()
        stat,out = commands.getstatusoutput("chmod 755 %s" % scriptname)
        stat,out = commands.getstatusoutput("clubatch %s" % scriptname)
                                                         
        self.exitStatus = stat

        if self.exitStatus != 0:
            print "error running job",self.jobName, self.exitStatus
            print out
        else:
            self.jobid = out.rstrip("\n")

        #if self.async:
        #    return self.exitStatus, (out,(self.jobName,self.jobid))

        return self.handleOutput()

    def handleOutput(self):

        if self.async:
            result = commands.getstatusoutput("qstat")
            checkcount = 0
            if (self.rebootMitigation):
                while len(result[1].split("\n")) < 3:
                    print "SGE REBOOT MITIGATION: qstat result seems too short"
                    print "---------------------------------------------------"
                    print result[1]
                    print "---------------------------------------------------"
                    print "... so we wait before trying again"
                    sleep(5.)
                    result = commands.getstatusoutput("qstat")
                    checkcount += 1
                    if checkcount > 12:
                        break
            self.exitStatus = 0 # assume it is done unless listed
            for line in result[1].split("\n"):
                if line.startswith(str(self.jobid)):
                    self.exitStatus = -1
                    break
            if self.exitStatus == 0:
                logdir = os.path.abspath(os.path.dirname(self.jobName))
                if os.path.isfile('%s/%s.sh.fail' % (logdir,self.jobName.split("/")[-1])) and not os.path.isfile('%s/%s.sh.done' % (logdir,self.jobName.split("/")[-1])):
                    self.exitStatus = 1

        output = ""
        if self.jobName:
            try:
                with open("%s.log" % self.jobName) as lf:
                    output = lf.read()
                    lf.close()
            except:
                output = "%s.log" % self.jobName

        return self.exitStatus, output

# -----------------------------------------------------------------------------------------------------
class SGEMonitor(LsfMonitor):
    ### def __call__(self):
    ###     jobsmap = {}
    ### 
    ###     while not self.stop:
    ###         if not self.jobsqueue.empty():
    ###             job = self.jobsqueue.get()
    ###             jobsmap[str(job.jobid)] = job
    ### 
    ###         status = commands.getstatusoutput("qstat")
    ###         jobids = []
    ###         statuses = []
    ###         for line in status[1].split("\n")[2:]:
    ###             toks = line.split()
    ###             jobids.append(toks[0])
    ###             statuses.append(toks[2])
    ###         for jobid in jobsmap.keys():
    ###             if not jobids.count(jobid):
    ###                 # i.e. job is no longer on the list, and hence done
    ###                 lsfJob = jobsmap[jobid]
    ###                 self.retqueue.put( (lsfJob, [lsfJob.cmd], lsfJob.handleOutput()) )
    ###                 jobsmap.pop(jobid)
    ###         sleep(5.)

    def __init__(self,*args,**kwargs):
        self.rebootMitigation = (BatchRegistry.getDomain() in ["hep.ph.ic.ac.uk"])
        
        super(SGEMonitor, self).__init__(*args, **kwargs)

    def monitor(self):
        status = commands.getstatusoutput("qstat")
        checkcount = 0
        if (self.rebootMitigation):
            while len(status[1].split("\n")) < 3:
                print "SGE REBOOT MITIGATION: qstat result seems too short"
                print "---------------------------------------------------"
                print status[1]
                print "---------------------------------------------------"
                print "... so we wait before trying again"
                sleep(5.)
                status = commands.getstatusoutput("qstat")
                checkcount += 1
                if checkcount > 12:
                    break
        jobids = []
        statuses = []
        for line in status[1].split("\n")[2:]:
            toks = line.split()
            jobids.append(toks[0])
            statuses.append(toks[2])
            #                print "DEBUG jobid jobids",jobid,jobids[0]
            #                print type(jobid),type(jobids[0])
            #                print
            #                print jobs

        for jobid in self.jobsmap.keys():
            if not jobids.count(jobid):
                # i.e. job is no longer on the list, and hence done
                self.jobFinished(jobid,None)
                    
# -----------------------------------------------------------------------------------------------------
class IclustMonitor(LsfMonitor):

    def monitor(self):
        status = commands.getstatusoutput("qstat")
        jobids = []
        statuses = []
        for line in status[1].split("\n")[2:]:
            toks = line.split()
            jobids.append(toks[0])
            statuses.append(toks[4])
            #                print "DEBUG jobid jobids",jobid,jobids[0]
            #                print type(jobid),type(jobids[0])
            #                print
            #                print jobs
        for jobid in self.jobsmap.keys():
            if not jobids.count(jobid):
                # i.e. job is no longer on the list, and hence done
                self.jobFinished(jobid,None)
                    
# -----------------------------------------------------------------------------------------------------
class Wrap:
    def __init__(self, func, args, retqueue, runqueue):
        self.retqueue = retqueue
        self.runqueue = runqueue
        self.func = func
        self.args = args
        
    def __call__(self,interactive=False):
        if not interactive:
            self.runqueue.put(1)
        ret = self.func( *self.args )
        if interactive:
            return ret
        else:
            self.runqueue.get()
            self.runqueue.task_done()
            self.retqueue.put( ret  )

    
# -----------------------------------------------------------------------------------------------------
class Parallel:
    def __init__(self,ncpu,lsfQueue=None,lsfJobName="job",asyncLsf=False,maxThreads=500,jobDriver=None,batchSystem="auto"):
        self.returned = Queue()
	self.njobs = 0
        self.JobDriver=jobDriver
        self.lsfJobs = Queue()
        self.lsfQueue = lsfQueue
        self.lsfJobName = lsfJobName
        self.jobId = 0
        self.sem = Semaphore()
        self.maxThreads = maxThreads
        self.asyncLsf = asyncLsf
        self.batchSystem = batchSystem

        if self.lsfQueue:
            self.running = Queue()
        else:
            self.running = Queue(ncpu)
        
        if not self.JobDriver:
            self.JobDriver = BatchRegistry.getRunner(self.batchSystem)
            
        if self.lsfQueue and self.asyncLsf:
            self.lsfMon = BatchRegistry.getMonitor(self.batchSystem)(self.lsfJobs,self.returned)
            thread = Thread(None,self.lsfMon)
            thread.start()
        
            
    def run(self,cmd,args,interactive=False,jobName=None):
        myargs = [cmd,args,interactive]
        if jobName:
            myargs.append(jobName)
        wrap = Wrap( self, myargs, self.returned, self.running )
        if interactive:
            return wrap(interactive=True)
        
        while threading.activeCount() > self.maxThreads:
            sleep(0.05)
        
        ret = (None,(None,(None,(None,None))))
        if not ( self.lsfQueue and  self.asyncLsf ):
            thread = Thread(None,wrap)
            thread.start()
        else:
            ret = wrap(interactive=True)
            
        self.sem.acquire()
	self.njobs += len(ret[-1][-1][-1][-1]) if isinstance(ret[-1][-1][-1][-1], list) else 1
        self.sem.release()
        
        return ret

    def addJob(self,cmd,args,batchId,jobName=None):
        if not self.asyncLsf:
            return
        
        job = self.JobDriver(self.lsfQueue,jobName,async=True)

        job.setJobId(batchId)
        job.cmd = " ".join([cmd]+args)
        self.lsfJobs.put(job)

        self.sem.acquire()
	self.njobs += len(batchId) if isinstance(batchId, list) else 1
        self.sem.release()
        
    def currJobId(self):
        self.sem.acquire()
        ret = self.jobId
        self.sem.release()
        return ret

    def setJobId(self,jobId):
        self.sem.acquire()
        self.jobId = jobId
        self.sem.release()
        
    def getJobId(self):
        self.sem.acquire()
        ret = self.jobId
        self.jobId += 1
        self.sem.release()
        return ret
        
    def __call__(self,cmd,args,interactive,jobName=None,replacesJob=None):

        if type(cmd) == str or type(cmd) == unicode:
            ## print cmd
            cmd = "%s %s" % (cmd, " ".join(args) )
            args = (cmd,)
            if self.lsfQueue and not interactive:
                if not jobName:
                    jobName = "%s%d" % (self.lsfJobName,self.getJobId())
                cmd = self.JobDriver(self.lsfQueue,jobName,async=self.asyncLsf,replacesJob=replacesJob)
            else:
                cmd = commands.getstatusoutput

        ret = cmd( *args )
        if self.lsfQueue and not interactive and self.asyncLsf:
            self.lsfJobs.put(cmd)
        return cmd,args,ret
    
    def stop(self):
        if self.lsfQueue and self.asyncLsf:
            self.lsfMon.stop = True
        
    def wait(self,handler=None,printOutput=True,struggleThr=0.):
        returns = []
        self.sem.acquire()
        njobs = int(self.njobs)
        self.sem.release()
        nleft = njobs
        nstruggle = int(floor(nleft*struggleThr))
        while nleft>0:
            print ""
            print "--- Running jobs: %d. Total jobs: %d (total submissions: %s)" % (nleft, njobs, self.njobs)
            print ""
            job, jobargs, ret = self.returned.get()
            if printOutput:
                try:
                    print "Job finished: '%s' '%s'" % ( job, " ".join([str(a) for a in jobargs]) )
                    if not handler:
                        for line in ret[1].split("\n"):
                            print line
                except:
                    pass
            if handler:
                nleft += handler.handleJobOutput(job, jobargs, ret)
            else:
                returns.append( (job,jobargs,ret) )
            nleft -= 1 
            if nleft != 0 and nleft == nstruggle and handler:
                handler.analyzeStrugglers(self)
                nstruggle = int(floor(nstruggle*struggleThr))
            
                
        self.sem.acquire()
        self.njobs -= njobs
        self.sem.release()
        return returns
