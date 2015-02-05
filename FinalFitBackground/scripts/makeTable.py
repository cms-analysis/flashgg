#!/usr/bin/env python

import os
import sys
import math

categories={}
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i","--infile")
parser.add_option("--c2011",default=5,type="int")
parser.add_option("--c2012",default=9,type="int")
parser.add_option("-m","--mh",type="float")
parser.add_option("-s","--mu",action="store_true",default=False)
(options,args)=parser.parse_args()

categories={'2011':range(options.c2011), '2012':range(options.c2012)}
#categories={'2012':[3]}
#categories={'2011':range(5)}
ftypes=['mass']
stypes=['Fab','Paul','Chi2','AIC']
nameDict={'Fab':'Hgg Nominal','Paul':'Envelope (no pen)','Chi2':'Envelope (1/dof)','AIC':'Envelope (2/dof)'}
truthDict={'pol':'Bernstein','exp':'Exponential','pow':'PowerLaw','lau':'Laurent','key':'Kernal Est','reg':'Regression','hyb':'Hybrid'}

print categories

import ROOT as r

# use output file from ./scripts/drawCoverage.py
inf = r.TFile(options.infile)

fileKeyNames = []
for key in inf.GetListOfKeys():
  fileKeyNames.append(key.GetName())

def findTruths(year,cat):
  truths = [] 
  for key in fileKeyNames:
    if '%s_cat%d_mass'%(year,cat) in key and 'Fab' in key:
      if 'kpdf' in key: 
        truth='kpdf'
      elif 'smooth' in key:
        truth='smooth'
      elif 'hybrid' in key:
        truth='hybrid'
      else:
        truth = key.split('mass_')[1].split('_Fab')[0]
      if truth not in truths: truths.append(truth)
  return truths

def findMatches(year,cat,ftype,stype):
  
  result = []
  for key in fileKeyNames:
    if '%s_cat%d_%s_'%(year,cat,ftype) in key and stype in key:
      result.append(key)
  return result

def getBiggestDeviation(graph,mean):
  x = r.Double()
  y = r.Double()
  largest_dev=0.
  yval=0.
  yerr=0.
  for p in range(graph.GetN()):
    graph.GetPoint(p,x,y)
    if r.TMath.Abs(y-mean)>largest_dev:
      largest_dev = r.TMath.Abs(y-mean)
      yval=float(y)
      yerr = float(graph.GetErrorY(p))
  return [yval,yerr]

def findGraph(file,year,cat,ftype,truth,stype,ext):
  
  matches=[]
  list_of_keys = [key.GetName() for key in file.GetListOfKeys()]
  stdName = '%s_cat%d_%s_%s_%s_%s'%(year,cat,ftype,truth,stype,ext)
  if stdName in list_of_keys:
    pullGraph = inf.Get(stdName)
    return pullGraph
  else:
    for name in list_of_keys:
      if (year in name) and ('cat%d'%cat in name) and (ftype in name) and (truth in name) and (stype in name) and (ext in name):
        matches.append(name)
    if len(matches)==1:
      pullGraph = inf.Get(matches[0])
      return pullGraph
    else:
      print 'matches:', matches
      sys.exit('Not good enough. Too many matches')

outf = open('CoverageTable.txt','w')

for year, catlist in categories.items():
  outf.write('\\begin{table}\n')
  outf.write('\t\\begin{center}\n')
  outf.write('\t\t\\begin{tabular}{| c | c | c | c | c | c |}\n')
  outf.write('\t\t\t\\hline\n')
  outf.write('\t\t\t\\multicolumn{6}{|l|}{\\textbf{Bias study %s}} \\\\ \n'%(year))
  outf.write('\t\t\t\\hline\n')
  outf.write('\t\t\t\\hline\n')
  outf.write('\t\t\t\\multirow{2}{*}{\\textbf{Truth}} & \\multirow{2}{*}{\\textbf{d.o.f}} & \\multicolumn{4}{|c|}{\\textbf{Maximum Pull}} \\\\ \n')
  outf.write('\t\t\t\\cline{3-6}\n')
  outf.write('\t\t\t& & Hgg Nominal & Env. (no pen) & Env. (1/d.o.f) & Env. (2/d.o.f) \\\\ \n')
  for cat in catlist:
    outf.write('\t\t\t\\hline\n')
    outf.write('\t\t\t\\multicolumn{6}{|c|}{\\textbf{Category %d}} \\\\ \n'%(cat))
    outf.write('\t\t\t\\hline\n')
    for ftype in ftypes:
      #outf.write('Function of %s\n'%ftype)
      for truth in findTruths(year,cat):
        #print truth
        if truth=='kpdf':
          func = 'key'
          order = 1
        elif truth=='smooth':
          func = 'reg'
          order = 1
        elif truth=='hybrid':
          func = 'hyb'
          order = 1
        else:
          func = truthDict[truth[:3]]
          order = int(truth[3:])+1

        outf.write('\t\t\t%s & %d '%(func,order))
        for stype in stypes:
          #pullGraph = inf.Get('%s_cat%d_%s_%s_%s_pull'%(year,cat,ftype,truth,stype))
          #cov1sigGraph = inf.Get('%s_cat%d_%s_%s_%s_cov1.0_coverage'%(year,cat,ftype,truth,stype))
          pullGraph = findGraph(inf,year,cat,ftype,truth,stype,'pull')
          cov1sigGraph = findGraph(inf,year,cat,ftype,truth,stype,'cov1.0_coverage')
          
          if options.mh:
            pullDev = [r.TMath.Abs(pullGraph.Eval(options.mh)),0.01]
            covDev = r.TMath.Abs(cov1sigGraph.Eval(options.mh)-0.683)
          else:
            pullDev = getBiggestDeviation(pullGraph,0.)
            covDev = getBiggestDeviation(cov1sigGraph,0.683)

          if (r.TMath.Abs(pullDev[0]))>0.14:
            outf.write(' & \\textbf{%4.2f}$\\pm$%4.2f '%(pullDev[0],pullDev[1]))
          else:
            outf.write(' & %4.2f$\\pm$%4.2f '%(pullDev[0],pullDev[1]))
          #print year, cat, ftype, stype, truth, getBiggestDeviation(pullGraph,0.), getBiggestDeviation(cov1sigGraph,0.683)
        outf.write(' \\\\ \n')
  
  outf.write('\t\t\t\\hline\n')
  outf.write('\t\t\\end{tabular}\n')
  outf.write('\t\t\\caption{Table showing summary of bias study for the %s categories}\n'%year)
  outf.write('\t\t\\label{table:pullCov%s}\n'%year)
  outf.write('\t\\end{center}\n')
  outf.write('\\end{table}\n')
