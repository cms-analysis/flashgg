from Queue import Queue

import commands
import subprocess
import os,sys

from threading import Thread, Semaphore
import threading 
from multiprocessing import cpu_count
from time import sleep

class LsfJob(object):
    """ a thread to run bsub and wait until it completes """
    
    #----------------------------------------
    def __init__(self, lsfQueue, jobName="", async=False):
        """ @param cmd is the command to be executed inside the bsub script. Some CMSSW specific wrapper
            code will be added
        """
        
        # super(LsfJob,self).__init()
        
        self.lsfQueue = lsfQueue
        self.jobName = jobName
        self.async = async
        self.jobid = None
        self.cmd = None
        
    def __str__(self):
        return "LsfJob: %s %s" % ( self.lsfQueue, self.jobName )
        
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
            script += "export $X509_USER_PROXY=%s\n" % os.environ['X509_USER_PROXY']
                    
        # the user command
        script += cmd+"\n"
        
        return self.run(script)
        
    
    #----------------------------------------
    def handleOutput(self):
        ## print "handleOutput"
        if self.async:
            
            result = commands.getstatusoutput("bjobs %d" % self.jobid)
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
class TarballLsfJob(LsfJob):
    
    #----------------------------------------
    def __init__(self,*args,**kwargs):
        
        
        self.stage_dest      = kwargs.pop("stage_dest")
        self.stage_cmd       = kwargs.pop("stage_cmd")
        self.stage_patterns  = kwargs.pop("stage_patterns")
        self.tarball         = kwargs.pop("tarball",None)
        self.job_outdir      = kwargs.pop("job_outdir",None)

        super(TarballLsfJob,self).__init__(*args,**kwargs)
        
    #----------------------------------------
    def __call__(self,cmd):
        
        self.cmd = cmd
        script = ""
        script += "#!/bin/bash\n"
        
        script += "WD=$PWD\n"
        script += "echo\n"
        script += "echo\n"
        script += "echo working directory is $WD\n"
        script += "echo\n"
        if not self.tarball:
            script += "cd " + os.environ['CMSSW_BASE']+"\n"
        else:
            script += "[[ -n $CMS_PATH ]] && source $VO_CMS_SW_DIR/cmsset_default.sh\n"
            script += "export SCRAM_ARCH=%s\n" % os.environ['SCRAM_ARCH']
            script += "scram project CMSSW %s\n" % os.environ['CMSSW_VERSION']
            script += "cd %s\n" % os.environ['CMSSW_VERSION']
            script += "tar zxf %s\n" % self.tarball            
            script += "scram b\n"
            
        script += "eval $(scram runtime -sh)"+"\n"
        script += "cd $WD\n"
        
        if os.environ.get('X509_USER_PROXY',None):
            script += "export $X509_USER_PROXY=%s\n" % os.environ['X509_USER_PROXY']
            
        if self.tarball and self.job_outdir:
            script += "mkdir %s\n" % self.job_outdir
            
        # the user command
        script += cmd+"\n"

        if self.tarball and self.job_outdir:
            script += 'cd %s\n' % self.job_outdir
            script += 'echo\n'
            script += 'echo\n'
            script += 'echo "Job finished with exit code $?"\n'
            script += 'echo "Files in ouput folder"\n'
            script += 'ls -ltr\n'
        # stage out files
        script += 'retval=$?\n'
        script += 'if [[ $retval == 0 ]]; then\n'
        script += '    errors=""\n'
        script += '    for file in $(find -name %s); do\n' % " -or -name ".join(self.stage_patterns)
        script += '        %s $file %s\n' % ( self.stage_cmd, self.stage_dest )
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
        script += 'exit $retval\n'
        script += '\n'
        
        return self.run(script)
        

# -----------------------------------------------------------------------------------------------------
class TarballLsfJobFactory(object):
    
    # ------------------------------------------------------------------------------------------------
    def __init__(self,stage_dest,
                 stage_cmd="cp -pv",stage_patterns=["'*.root'","'*.xml'"],job_outdir=None):
        
        self.stage_dest = stage_dest
        self.stage_cmd  = stage_cmd
        self.stage_patterns  = stage_patterns
        self.tarball = None
        self.job_outdir      = job_outdir

    # ------------------------------------------------------------------------------------------------
    def stageDest(self,dest):
        self.stage_dest = dest

    # ------------------------------------------------------------------------------------------------
    def setTarball(self,tarball):
        self.tarball = tarball
        
    # ------------------------------------------------------------------------------------------------
    def mkTarball(self,tarball=None,
                  tarball_entries=["python","lib","bin"],tarball_patterns={"src/*":"data"},
                  tarball_transform=None):
        
        self.tarball = tarball
        content=tarball_entries
        for folder,pattern in tarball_patterns.iteritems():
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
        ## print out
        ## print "CMSSW_BASE: %s" % os.environ["CMSSW_BASE"]
        ## print "args: %s" % " ".join(args)
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
            
        kwargs["stage_dest"]     = self.stage_dest
        kwargs["stage_cmd"]      = stage_cmd
        kwargs["stage_patterns"] = self.stage_patterns
        kwargs["tarball"]        = self.tarball
        kwargs["job_outdir"]     = self.job_outdir
        
        return TarballLsfJob(*args,**kwargs)

        

# -----------------------------------------------------------------------------------------------------
class LsfMonitor:
    def __init__(self,jobsqueue,retqueue):
        self.jobsqueue = jobsqueue
        self.retqueue = retqueue
        self.stop = False
    def __call__(self):
        jobsmap = {}
        
        while not self.stop:
            if not self.jobsqueue.empty():
                job = self.jobsqueue.get()
                jobsmap[str(job.jobid)] = job
                        
            status = commands.getstatusoutput("bjobs %s" % " ".join(jobsmap.keys()))
            for line in status[1].split("\n")[1:]:
                toks = [ l for l in line.split(" ") if l != "" ]
                jobid = toks[0]
                status = toks[2]
                if status in ["DONE","EXIT","ZOMBI","UNKWN"]:
                    lsfJob = jobsmap[jobid]
                    self.retqueue.put( (lsfJob, [lsfJob.cmd], lsfJob.handleOutput()) )
                    jobsmap.pop(jobid)
            sleep(0.1)

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
    def __init__(self,ncpu,lsfQueue=None,lsfJobName="job",asyncLsf=False,maxThreads=500,jobDriver=LsfJob):
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
        if self.lsfQueue:
            self.running = Queue()
        else:
            self.running = Queue(ncpu)
            
        if self.lsfQueue and self.asyncLsf:
            self.lsfMon = LsfMonitor(self.lsfJobs,self.returned)
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
        
        ret = (None,(None,None))
        if not ( self.lsfQueue and  self.asyncLsf ):
            thread = Thread(None,wrap)
            thread.start()
        else:
            ret = wrap(interactive=True)
            
            
        self.sem.acquire()
	self.njobs += 1
        self.sem.release()
        
        return ret

    def addJob(self,cmd,args,batchId,jobName=None):
        if not self.asyncLsf:
            return
        
        job = self.JobDriver(self.lsfQueue,jobName,async=True)
        job.jobid = batchId
        job.cmd = " ".join([cmd]+args)
        self.lsfJobs.put(job)

        self.sem.acquire()
	self.njobs += 1
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
        
    def __call__(self,cmd,args,interactive,jobName=None):

        if type(cmd) == str or type(cmd) == unicode:
            ## print cmd
            cmd = "%s %s" % (cmd, " ".join(args) )
            args = (cmd,)
            if self.lsfQueue and not interactive:
                if not jobName:
                    jobName = "%s%d" % (self.lsfJobName,self.getJobId())
                cmd = self.JobDriver(self.lsfQueue,jobName,async=self.asyncLsf)
            else:
                cmd = commands.getstatusoutput

        ret = cmd( *args )
        if self.lsfQueue and not interactive and self.asyncLsf:
            self.lsfJobs.put(cmd)
        return cmd,args,ret
    
    def stop(self):
        if self.lsfQueue and self.asyncLsf:
            self.lsfMon.stop = True
        
    def wait(self,handler=None,printOutput=True):
        returns = []
        self.sem.acquire()
        njobs = int(self.njobs)
        self.sem.release()
        nleft = njobs
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

        self.sem.acquire()
        self.njobs -= njobs
        self.sem.release()
        return returns
