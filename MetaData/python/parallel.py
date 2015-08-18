from Queue import Queue

import commands
import subprocess
import os,sys

from threading import Thread, Semaphore
import threading 
from multiprocessing import cpu_count
from time import sleep

class LsfJob:
    """ a thread to run bsub and wait until it completes """

    #----------------------------------------
    def __init__(self, lsfQueue, jobName="", async=False):
        """ @param cmd is the command to be executed inside the bsub script. Some CMSSW specific wrapper
            code will be added
        """
        self.lsfQueue = lsfQueue
        self.jobName = jobName
        self.async = async
        self.jobid = None
        self.cmd = None
        
    def __str__(self):
        return "LsfJob: %s %s" % ( self.lsfQueue, self.jobName )
        
    #----------------------------------------
    def __call__(self,cmd):
        
        bsubCmdParts = [ "bsub",
                         "-q " + self.lsfQueue,
                         ]
        
        self.cmd = cmd
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
        
        script = ""
        script += "#!/bin/sh\n"
        
        script += "cd " + os.environ['CMSSW_BASE']+"\n"
        script += "eval `scram runtime -sh`"+"\n"
        script += "cd " + os.getcwd()+"\n"
        
        # the user command
        script += cmd+"\n"
        
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
            except:
                output = "%s.log" % self.jobName
                
        return self.exitStatus, output

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

    
class Parallel:
    def __init__(self,ncpu,lsfQueue=None,lsfJobName="job",asyncLsf=False,maxThreads=500):
        self.returned = Queue()
	self.njobs = 0
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
        
        job = LsfJob(self.lsfQueue,jobName,async=True)
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
                cmd = LsfJob(self.lsfQueue,jobName,async=self.asyncLsf)
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
