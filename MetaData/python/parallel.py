from Queue import Queue

import commands
import subprocess
import os,sys

from threading import Thread, Semaphore
from multiprocessing import cpu_count


class LsfJob:
    """ a thread to run bsub and wait until it completes """

    #----------------------------------------
    def __init__(self, lsfQueue, jobName=""):
        """ @param cmd is the command to be executed inside the bsub script. Some CMSSW specific wrapper
            code will be added
        """
        self.lsfQueue = lsfQueue
        self.jobName = jobName
    
    def __str__(self):
        return "LsfJob: %s %s" % ( self.lsfQueue, self.jobName )
        
    #----------------------------------------
    def __call__(self,cmd):
        bsubCmdParts = [ "bsub",
                         "-q " + self.lsfQueue,
                         "-K",  # bsub waits until job completes
                         ]

        if( self.jobName ):
            bsubCmdParts.append("-J " + self.jobName)
            bsubCmdParts.append("-o %s.log" % self.jobName)

        bsubCmd = " ".join(bsubCmdParts)

        import subprocess
        lsf = subprocess.Popen(bsubCmd, shell=True, # bufsize=bufsize,
                               stdin=subprocess.PIPE,
                               close_fds=True)
        
        print >> lsf.stdin, "#!/bin/sh"

        print >> lsf.stdin, "cd " + os.environ['CMSSW_BASE']
        print >> lsf.stdin, "eval `scram runtime -sh`"
        print >> lsf.stdin, "cd " + os.getcwd()

        # the user command
        print >> lsf.stdin, cmd
        lsf.stdin.close()
        
        # wait for the job to complete
        self.exitStatus = lsf.wait()

        if self.exitStatus != 0:
            print "error running job",self.jobName
        
        return self.exitStatus, ""
    
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
    def __init__(self,ncpu,lsfQueue=None,lsfJobName="job"):
        self.returned = Queue()
	self.njobs = 0
        self.lsfQueue = lsfQueue
        self.lsfJobName = lsfJobName
        self.jobId = 0
        self.sem = Semaphore()
        if self.lsfQueue:
            self.running = Queue()
        else:
            self.running = Queue(ncpu)
        
            
    def run(self,cmd,args,interactive=False):
        wrap = Wrap( self, (cmd,args,interactive), self.returned, self.running )
        if interactive:
            return wrap(interactive=True)
        self.sem.acquire()
	self.njobs += 1
        self.sem.release()
        ## print self.njobs
        thread = Thread(None,wrap)
        thread.start()
    
    def getJobId(self):
        self.sem.acquire()
        ret = self.jobId
        self.jobId += 1
        self.sem.release()
        return ret
        
    def __call__(self,cmd,args,interactive):

        if type(cmd) == str or type(cmd) == unicode:
            ## print cmd
            cmd = "%s %s" % (cmd, " ".join(args) )
            args = (cmd,)
            if self.lsfQueue and not interactive:
                cmd = LsfJob(self.lsfQueue,"%s%d" % (self.lsfJobName, self.getJobId()))
            else:
                cmd = commands.getstatusoutput
        ## print "put"
        ## self.running.put((cmd,args))
        ## print args
        ret = cmd( *args ) 
        ## print "get"
        ## self.running.get()
        ## print "done"
        ## self.running.task_done()
        ## print "ok"
        return cmd,args,ret

    def wait(self):
        returns = []
        self.sem.acquire()
        njobs = self.njobs
        self.sem.release()
        for i in range(njobs):
            print "Finished jobs: %d. Total jobs: %d" % (i, self.njobs)
            job, jobargs, ret = self.returned.get()
            if type(job) == str:
                print "finished: %s %s" % ( job, " ".join(jobargs) )
                for line in ret[1].split("\n"):
                    print line
            returns.append(ret)
        
        self.sem.acquire()
        self.njobs -= njobs
        self.sem.release()
        return returns
