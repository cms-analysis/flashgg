#!/usr/bin/env python

import os
import sys
import fnmatch

dir=sys.argv[1]

running=[]
failed=[]
done=[]
queued=[]

for root,dirs,files in os.walk(dir):
  for filename in fnmatch.filter(files,'*.run'):
    running.append(filename)
  for filename in fnmatch.filter(files,'*.fail'):
    failed.append(filename)
  for filename in fnmatch.filter(files,'*.done'):
    done.append(filename)
  for filename in fnmatch.filter(files,'*.sh'):
    if os.path.exists(dir+'/'+filename+'.run') or os.path.exists(dir+'/'+filename+'.fail') or os.path.exists(dir+'/'+filename+'.done'): continue 
    else:
      queued.append(filename)

print '\n'
print 'Queued (or unknown)..', len(queued)
print '\t', queued
print '\n'
print 'Running..', len(running)
print '\t', running
print '\n'
print 'Failed...', len(failed)
print '\t', failed
print '\n'
print 'Done...', len(done)
print '\t', done
