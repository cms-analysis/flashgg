#!/usr/bin/env python

import os
import subprocess

import FWCore.ParameterSet.VarParsing as VarParsing

options = VarParsing.VarParsing("analysis")
options.register('oldfiles',
                 '',
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Old verions input files")
options.register('newfiles',
                 '',
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "New verions input files")
options.register('skipdumper',
                 False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Skip ntuple production")

options.maxEvents = -1
options.parseArguments()

cmd = subprocess.Popen(['git log --format="%H" -n 1'],
                       stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE, shell=True)
git_commit, err = cmd.communicate()
outdir = 'validation_'+git_commit.strip('\n')+'/'
if not os.path.isdir(outdir):
    os.mkdir(outdir)
else:
    print('WARNING: '+outdir+' already exist!')

if not options.skipdumper:    
    #---run dumper on old and new files first
    # OLD
    old_files = ','.join(options.oldfiles)
    cmd = subprocess.Popen(['cmsRun Validation/test/diphoton_technical_validation_cfg.py inputfiles='+old_files+' outputfile='+outdir+'/diphoton_validation_OLD.root'],
                           stdout=subprocess.PIPE,
                           stderr=subprocess.PIPE, shell=True)
    print("RUNNING dumper on old files")
    out, err = cmd.communicate()
    print(err)
    # NEW
    new_files = ','.join(options.newfiles)
    cmd = subprocess.Popen(['cmsRun Validation/test/diphoton_technical_validation_cfg.py inputfiles='+new_files+' outputfile='+outdir+'/diphoton_validation_NEW.root'],
                           stdout=subprocess.PIPE,
                           stderr=subprocess.PIPE, shell=True)
    print("RUNNING dumper on new files")
    out, err = cmd.communicate()
    print(err)

#---load data
import ROOT
ROOT.gROOT.SetBatch(True)
    
from flashgg.Validation.diphoton_technical_validation_cfi import validation_variables

old_ntuples = ROOT.TFile.Open(outdir+'diphoton_validation_OLD.root')
old_tree = old_ntuples.Get("dumper/trees/validation")
new_ntuples = ROOT.TFile.Open(outdir+'diphoton_validation_NEW.root')
new_tree = new_ntuples.Get("dumper/trees/validation")

old_tree.AddFriend(new_tree, "new")

df = ROOT.RDataFrame(old_tree)

histos_diff = {}
histos_new = {}
histos_old = {}
for var in validation_variables:
    if ':=' in var:
        var = var.split(':=')[0].strip()
        histos_new[var] = df.Histo1D('new.'+var)
        histos_old[var] = df.Histo1D(var)        
        histos_diff[var] = df.Define('delta_'+var, 'new.'+var+'-'+var).Histo1D('delta_'+var)

if not os.path.isdir(outdir+'plots/'):
    os.makedirs(outdir+'plots/new_only')
    os.makedirs(outdir+'plots/old_only')

#---Draw difference plots and keep record of comparison in README file    
readme_txt = "Diphoton variable comparison result:"    
for var, histo in histos_diff.iteritems():
    c = ROOT.TCanvas(var)
    histo.SetTitle(";#Delta"+var)
    histo.SetLineWidth(2)
    histo.SetLineColor(ROOT.kBlue+1)
    histo.SetFillColor(ROOT.kCyan+1)
    histo.SetFillStyle(3004)
    histo.Draw()
    if histo.GetMean() != 0:
        print(var)
        readme_txt += var + ": VERSIONS DIFFERw \n"  
    else:
        readme_txt += var + ": OK \n"  
    c.SaveAs(outdir+'plots/'+var+'.png', 'png')
    c.SaveAs(outdir+'plots/'+var+'.pdf', 'pdf')
    c.SaveAs(outdir+'plots/'+var+'.root', 'root')

with open(outdir+'plots/README.txt', 'w') as readme:
        readme.write(readme_txt)

#---Draw variables from NEW ntuples        
for var, histo in histos_new.iteritems():
    c = ROOT.TCanvas(var)
    histo.SetTitle(";"+var)
    histo.SetLineWidth(2)
    histo.SetLineColor(ROOT.kRed+1)
    histo.SetFillColor(ROOT.kOrange+1)
    histo.SetFillStyle(3004)
    histo.Draw()
    c.SaveAs(outdir+'plots/new_only/'+var+'.png', 'png')
    c.SaveAs(outdir+'plots/new_only/'+var+'.pdf', 'pdf')
    c.SaveAs(outdir+'plots/new_only/'+var+'.root', 'root')

#---Draw variables from OLD ntuples            
for var, histo in histos_old.iteritems():
    c = ROOT.TCanvas(var)
    histo.SetTitle(";"+var)
    histo.SetLineWidth(2)
    histo.SetFillStyle(3004)
    histo.SetLineColor(ROOT.kBlack)
    histo.SetFillColor(ROOT.kGray+1)
    histo.Draw()
    c.SaveAs(outdir+'plots/old_only/'+var+'.png', 'png')
    c.SaveAs(outdir+'plots/old_only/'+var+'.pdf', 'pdf')
    c.SaveAs(outdir+'plots/old_only/'+var+'.root', 'root')
    
