import os, sys, fnmatch

def getDescrip(f):
  desc='DESCRIPTION TO BE DETERMINED'
  if 'func' in f:
    if 'dm' in f:
      desc='Evolution of mean as function of mH'
    elif 'sigma' in f:
      desc='Evolution of width as function of mH'
    elif 'f' in f and 'frac' not in f:
      desc='Evolution of gaussian fraction as function of mH'
    elif 'frac' in f:
      desc='Evolution of right/wrong fraction as function of mH'
  elif 'mass' in f:
    desc='Mass distribution with fit'
  elif 'xsec' in f:
    if 'ggh' in f:
      desc='Cross section for process ggh'
    elif 'vbf' in f:
      desc='Cross section for process vbf'
    elif 'wzh' in f:
      desc='Cross section for process wzh'
    elif 'tth' in f:
      desc='Cross section for process tth'
  elif 'branch' in f:
    desc='Branching ratios'
  return desc

out = open('plots/default.html','w')

fils=[]
for root, dirs, files in os.walk('plots'):
  for f in fnmatch.filter(files,'*.png'):
    fils.append(f)

fils.sort()

# do contents first
out.write('<html>\n')
out.write('<body>\n')
out.write('<h1>Signal fitting report</h1><br/>\n')
out.write('<h2>Table of contents</h2><br/>\n')
out.write('<ul>\n')
for f in fils:
  name=os.path.basename(f).split('.png')[0]
  out.write('<li><a href=\"#'+name+'\">'+name+'</a></li>\n')
out.write('</ul>\n')

for f in fils:
 name=os.path.basename(f).split('.png')[0]
 out.write('<a name='+name+'>\n')
 out.write('<h2>'+name+'</h2><br>\n')
 out.write('<a href='+f+'><img width=\"500\" src=\"'+f+'\"></a>\n')
 out.write('<br/>'+getDescrip(f)+'<br/>\n')
