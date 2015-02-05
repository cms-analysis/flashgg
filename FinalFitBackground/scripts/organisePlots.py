#!/usr/bin/env python

import os
import fnmatch

cats=9
muVal=[-1.5,-1.0,-0.5,0.0,0.5,1.0,1.5]

folder='coverage2012'
web_folder='~/www/BackgroundProfiling/'

os.system('mkdir -p %s/plots'%folder)
os.system('mkdir -p %s/%s'%(web_folder,folder))

for cat in range(cats):
  os.system('mkdir -p %s/plots/cat%d'%(folder,cat))
  for root,dirs,files in os.walk('%s/cat%d/coverage'%(folder,cat)):
    for file in files:
      #new_file = file.replace('.pdf','_cat%d.pdf'%cat)
      #new_file = file.replace('.png','_cat%d.png'%cat)
      new_file = file
      os.system('cp %s/%s %s/plots/cat%d/%s'%(root,file,folder,cat,new_file))

  montage_bias=[]
  montage_pull=[]
  for mu in muVal:
    for root,dirs,files in os.walk('%s/cat%d_mu%3.1f/plots/bias'%(folder,cat,mu)):
        for file in files:
          if (file.startswith('bias') or file.startswith('pull')) and 'prof' not in file:
            if '.pdf' in file: new_file = file.replace('.pdf','_mu%3.1f.pdf'%(mu))
            if '.png' in file: new_file = file.replace('.png','_mu%3.1f.png'%(mu))
            os.system('cp %s/%s %s/plots/cat%d/%s'%(root,file,folder,cat,new_file))
            if file.startswith('bias') and '.png' in file: montage_bias.append(root+'/'+file)
            if file.startswith('pull') and '.png' in file: montage_pull.append(root+'/'+file)
  #bias_line = 'pdfjoin -o %s/plots/bias_cat%d.pdf '%(folder,cat)
  #pull_line = 'pdfjoin -o %s/plots/pull_cat%d.pdf '%(folder,cat)
  bias_line = 'convert -delay 50 '
  pull_line = 'convert -delay 50 '
  for b in montage_bias:
    bias_line += b+' '
  for p in montage_pull:
    pull_line += p+' '
  bias_line += '%s/plots/cat%d/bias.gif'%(folder,cat)
  pull_line += '%s/plots/cat%d/pull.gif'%(folder,cat)

  os.system(bias_line)
  os.system(pull_line)

os.system('cp -r %s/plots/cat* %s/%s/'%(folder,web_folder,folder))
os.system('make_html.py %s -P 3 -c -l -d --title=\"Background Profiling - Bias and Coverage\"'%(web_folder))
