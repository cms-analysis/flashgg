import os
import sys

f = open(sys.argv[1])
y = sys.argv[2]

# mass fac at hcp
whFrac2011=[0.65,0.63,0.65,0.63,0.72]
whFrac2012=[0.65,0.63,0.65,0.63,0.64,0.80,0.63,0.83,0.81]
# cic at moriond
#whFrac2011=[0.64,0.63,0.63,0.65]
#whFrac2012=[0.64,0.63,0.63,0.65,0.73,0.68,0.84,0.81,0.5]
whFrac=[]
if y=="2011":
  whFrac=whFrac2011
elif y=="2012":
  whFrac=whFrac2012
else:
  print 'Unrecognised year', sys.argv[2]

atrel=False
rateLine=False
oldProcesses=['ggH','qqH','VH','ttH','bkg_mass']
newProcesses=['ggH','qqH','WH','ZH','ttH','bkg_mass']
details=[]

fileLines = f.readlines()
elsToDuplicate=[]

# figure out which lines need duplicating
for line in fileLines:
  if not line.startswith('process'): continue
  elif '0' in line: continue
  else: 
    els = line.split()
    els = els[1:]
    for i, el in enumerate(els):
      if el=='VH': elsToDuplicate.append(i)
    
for line in fileLines:
  if 'param' in line:
    print line,
    continue
  if line.startswith('shapes VH'):
    print line.replace('VH','WH').replace('wzh','wh'),
    print line.replace('VH','ZH').replace('wzh','zh'),
    continue
  if line.startswith('observation'):
    atrel=True
    print line,
    continue
  if line=='\n':
    print line,
    continue
  if atrel and not line.startswith('process') and not line.startswith('rate'):
    els = line.split()
    print els[0], '   ',
    els = els[1:]
    if els[0]=='lnN': 
      print els[0], '   ',
      els = els[1:]
    for i,el in enumerate(els):
      if i in elsToDuplicate: print el, el,
      else: print el,
    print 
    continue
  if atrel and line.startswith('process'):
    if 'VH' in line:
      print line.replace('VH','WH ZH'),
    else:
      print line.replace('-3','-3 -4'),
    continue
  if atrel and line.startswith('rate'):
    els = line.split()
    print els[0], '   ',
    els = els[1:]
    for i, el in enumerate(els):
      if i in elsToDuplicate:
        print float(el)*whFrac[i//len(oldProcesses)], float(el)*(1.0-whFrac[i//len(oldProcesses)]),
      else:
        print float(el),
    print
    continue
  print line,
     
 

