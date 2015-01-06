from Queue import Queue

import commands
import subprocess
import os

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
    def __init__(self, func, args, queue):
        self.queue = queue
        self.func = func
        self.args = args
        
    def __call__(self,interactive=False):
        ret = self.func( *self.args )
        if interactive:
            return ret
        else:
            self.queue.put( ret  )

    
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
        wrap = Wrap( self, (cmd,args), self.returned )
        if interactive:
            return wrap(interactive=True)
	self.njobs += 1
        thread = Thread(None,wrap)
        thread.start()
    
    def getJobId(self):
        self.sem.acquire()
        ret = self.jobId
        self.jobId += 1
        self.sem.release()
        return ret
        
    def __call__(self,cmd,args):

        if type(cmd) == str or type(cmd) == unicode:
            ## print cmd
            cmd = "%s %s" % (cmd, " ".join(args) )
            args = (cmd,)
            if self.lsfQueue:
                cmd = LsfJob(self.lsfQueue,"%s%d" % (self.lsfJobName, self.getJobId()))
            else:
                cmd = commands.getstatusoutput
                
        self.running.put((cmd,args))
        ret = cmd( *args ) 
        self.running.get()
        self.running.task_done()
        return cmd,args,ret

    def wait(self):
        returns = []
        for i in range(self.njobs):
            print "Finished jobs: %d. Total jobs: %d" % (i, self.njobs)
            job, jobargs, ret = self.returned.get()
            if type(job) == str:
                print "finished: %s %s" % ( job, " ".join(jobargs) )
                for line in ret[1].split("\n"):
                    print line
            returns.append(ret)
        return returns
