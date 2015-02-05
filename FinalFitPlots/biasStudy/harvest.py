#!/usr/bin/env python

import sys,glob,os

from optparse import OptionParser, make_option

from Queue import Queue

from threading import Thread, Semaphore
from multiprocessing import cpu_count

import commands

src=sys.argv[1]
dst=sys.argv[2]

def system(cmd):
    stat, out = commands.getstatusoutput(cmd)
    return cmd, stat, out

class Parallel:
    def __init__(self,ncpu):
        self.semaphore = Semaphore()
        self.queue = Queue(ncpu)
        
    def __call__(self,cmd):
        self.queue.put(cmd)
        ## print cmd
        ret = system( cmd )
        self.queue.get()
        self.queue.task_done()
        return ret


class Wrap:
    def __init__(self, func, args, queue):
        self.queue = queue
        self.func = func
        self.args = args
        
    def __call__(self):
        self.queue.put( self.func( *self.args ) )
    
jobs = {}
all = glob.glob("%s/higgsCombine_cat[0-9]*_truth*.Max[Ll]ikelihoodFit.mH*.[0-9]*.root" % (src) )
for f in all:
    job="higgsCombine_%s.MaxLikelihoodFit.root" % ( os.path.basename(f).split("_",1)[-1].split(".",1)[0] )
    
    if not job in jobs:
        jobs[job] = ""
    jobs[job] += " %s" % f


ncpu = cpu_count()
parallel = Parallel(ncpu)
retvals = Queue()
ntasks = 0

for job,files in jobs.iteritems():
    print "scheduling ", job
    cmd = "hadd -f %s/%s %s" % ( dst, job, files )
    wrap = Wrap( parallel, (cmd,), retvals )
    thread = Thread(None,wrap)
    thread.start()
    ntasks += 1


for i in range(ntasks):
    cmd,stat,out = retvals.get()
    retvals.task_done()
    fname = os.path.basename(cmd.split(" ")[2])
    if stat == 0:
        print fname, "done"
    else:
        print fnanme, "failed"
        print cmd
        print out
        
