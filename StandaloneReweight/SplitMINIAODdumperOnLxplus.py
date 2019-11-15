#!/usr/bin/env python
import os, re
import commands
import math, time
import sys
import argparse
import subprocess

parser = argparse.ArgumentParser(description='This script splits tasks in multiple jobs and sends them on LXBATCH')

parser.add_argument("-l", "--label",          required=True,     type=str,  help="job label")
parser.add_argument("-e", "--exe",            required=True,     type=str,  help="executable")
parser.add_argument("-p", "--proxy",          required=True,     type=str,  help="proxy filename to access grid ")
parser.add_argument("-o", "--outputFolder",   required=True,     type=str,  help="folder where to store output files")
parser.add_argument("-c", "--configFile",     required=True,     type=str,  help="template config file to be run")
parser.add_argument("-i", "--inputfilelist",  required=True,     type=str,  help="input file list")
parser.add_argument("-q", "--queue",          default="workday",     type=str,  help="lxbatch queue to use")
parser.add_argument("-s", "--submit",                                       help="submit jobs", action='store_true')
parser.add_argument("-v", "--verbose",                                      help="increase output verbosity", action='store_true')


args = parser.parse_args()

job_labels = ["node_SM","node_2","node_3","node_4","node_5","node_6","node_7","node_8","node_9","node_10","node_11","node_12","GluGluToHHTo2B2G_TuneCP5"]

print 
print 'START'
print 

currDir = os.getcwd()

print

try:
   subprocess.check_output(['mkdir','jobs'])
except subprocess.CalledProcessError as e:
   print e.output
try:
   subprocess.check_output(['mkdir','jobs/'+args.label])
except subprocess.CalledProcessError as e:
   print e.output
try:
   subprocess.check_output(['mkdir',args.outputFolder])
except subprocess.CalledProcessError as e:
   print e.output
try:
   subprocess.check_output(['mkdir',args.outputFolder+"/"+args.label+"/"])
except subprocess.CalledProcessError as e:
   print e.output

##### loop for creating and sending jobs --> one file per job #####
i=0
for job_label in job_labels:
   print "doing job for "+job_label
   filenamelist = ""
   with open(args.inputfilelist) as filelist:
      for filename in filelist:   
         #print "checking "+filename
         if job_label in filename:
            modifiedfilename = filename.replace("\n","")
            #print modifiedfilename+" matches"
            if(filenamelist != ""): 
               filenamelist += ' \ \n'
            filenamelist += modifiedfilename

   ##### creates directory and file list for job #######
   jobDir = currDir+'/jobs/'+args.label+'/job_'+str(i)
   os.system('mkdir '+jobDir)
   os.chdir(jobDir)
   
   ##### copy executable to the jobDir ######
   os.system('cp '+args.exe+' '+jobDir+"/executable.exe")

   ##### copy proxyfile to the jobDir ######
   os.system("cp "+args.proxy+' '+jobDir+"/")
   
   ##### creates ntupler config file #######
   with open(args.configFile) as fi:
      contents = fi.read()
      replaced_contents = contents.replace("FILENAMES",filenamelist)
      replaced_contents = replaced_contents.replace("OUTPUTNAME",args.outputFolder+"/"+job_label+".root")
      #replaced_contents = replaced_contents.replace('OUTPUT_PATH', str(args.outputFolder+args.label+"/"))
      with open(jobDir+"/config.cfg", "w") as fo:
         fo.write(replaced_contents)
   
   ##### creates jobs #######
   with open('job_'+str(i)+'.sh', 'w') as fout:
      fout.write("#!/bin/sh\n")
      fout.write("echo\n")
      fout.write("echo 'START---------------'\n")
      fout.write("workdir=$PWD\n")
      fout.write("echo 'current dir: ' ${workdir}\n")
      fout.write("export XRD_NETWORKSTACK=IPv4\n")
      fout.write("cd /afs/cern.ch/user/f/fmonti/work/flashggNew/CMSSW_10_5_0/src/flashgg/\n")
      fout.write("eval `scram runtime -sh`\n")
      fout.write("cd $workdir\n")
      fout.write("export X509_USER_PROXY="+args.proxy+"\n")
      fout.write("mkdir "+str(args.outputFolder+"/"+args.label)+"\n")
      fout.write(jobDir+"/executable.exe "+jobDir+"/config.cfg\n")
      fout.write("echo 'STOP---------------'\n")
      fout.write("echo\n")
      
   os.system("chmod 755 job_"+str(i)+".sh")
   i=i+1

#make the submit file
condorsubFilename = currDir+"/jobs/submit_jobs.sub"
condorsub = open( condorsubFilename,"w")
condorsub.write("executable            = $(scriptname)\n")
condorsub.write("output                = $(scriptname).$(ClusterId).out\n")
condorsub.write("error                 = $(scriptname).$(ClusterId).err\n")
condorsub.write("log                   = "+currDir+"/jobs/"+"log.$(ClusterId).log\n")
condorsub.write('+JobFlavour           = "workday"\n')
#if options.tier0:
#   condorsub.write('+AccountingGroup      = "group_u_CMS.CAF.ALCA"\n')
condorsub.write("queue scriptname matching "+currDir+"/jobs/"+args.label+'/job_*/job_*.sh')
condorsub.close()
###### sends jobs ######
if args.submit:
   os.system("condor_submit "+ currDir+"/jobs/submit_jobs.sub")

print
print "your jobs:"
os.system("condor_q")
print
print 'END'
print
