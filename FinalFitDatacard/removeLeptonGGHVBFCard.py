import os
import sys

f = open(sys.argv[1])
splitVH=False
if len(sys.argv)>2:
  splitVH=True

atrel=False
rateLine=False
details=[]

for line in f.readlines():
  if line.startswith('observation'):
    atrel=True
    print line,
    continue
  if line=='\n':
    print line,
    continue
  if not atrel:
    print line,
    continue
  if 'nuissance' in line or 'globalscale' in line:
    print line,
    continue
  if atrel:
    els = line.split()
    print els[0], '   ',
    els = els[1:]
    if els[0]=='lnN': 
      print els[0], '   ',
      els = els[1:]
   
   # hard code: for 9 cats + 4 procs need to remove ggh/vbf cat6/cat7 elements 30,31,35,36
    for i,el in enumerate(els):
      if (not splitVH and (i==30 or i==31 or i==35 or i==36)) or (splitVH and (i==36 or i==37 or i==42 or i==43)):
        continue
      else:
        print el, 
    print '\n'
      
