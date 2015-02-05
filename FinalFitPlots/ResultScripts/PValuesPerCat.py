#!/usr/bin/env python
import os
import time
import threading
import shutil
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-d", "--datacard", dest="datacard", help="Input Datacard")
parser.add_option("-e", "--Expected", action="store_true", dest="Expected", default=False, help="Do Expected PValues or Limits")
parser.add_option("-o", "--Observed", action="store_true", dest="Observed", default=False, help="Do Observed PValues or Limits")
parser.add_option("-l", "--Limits", action="store_true", dest="Limits", default=False, help="Do Asymptotic CLs Limits")
parser.add_option("-p", "--PValues", action="store_true", dest="PValues", default=False, help="Do PValues")
parser.add_option("-b", "--BestFit", action="store_true", dest="BestFit", default=False, help="Do BestFit Signal Strength")
parser.add_option("-L", "--MaxLikelihood", action="store_true", dest="MaxLikelihood", default=False, help="Do MaxlikelihoodFit per mass")
parser.add_option("-m", "--Masses", dest="Masses", default="", help="Only calculate limits or pvalues at listed masses. --Masses=120,124,125")
parser.add_option("-c", "--Categories", action="store_true", dest="Categories", default=False, help="Do PValues for all categories")
parser.add_option("-u", "--CustumCat", dest="CustumCat", default="", help="Make datacards and do PValues for custum category. --CustumCat=\"cat0 cat1 cat2 cat3\" ")
parser.add_option("-s", "--OutputDirectory", dest="OutputDirectory", default="", help="Output Directory")
parser.add_option("-t", "--toysFile", dest="toysFile", default="", help="Asimov Toy Set File")


parser.add_option("--min", type="float", dest="MinMass", default=110, help="Minimum Mass")
parser.add_option("--max", type="float", dest="MaxMass", default=150, help="Maximum Mass")
parser.add_option("--step", type="float", dest="MassStep", default=0.5, help="Mass step")
parser.add_option("--debug", action="store_true", dest="debug", default=False, help="Enable Debug Mode")
parser.add_option("--dryrun", action="store_true", dest="dryrun", default=False, help="Don't calculate pvalues, but it will still make datacards.")
parser.add_option("--expectSignal", dest="expectSignal", default=1, help="Expected Signal Strength")
parser.add_option("--nosubdirectory", action="store_true", dest="nosubdirectory", default=False, help="Don't place output in sub-directory. Handy with -s if only running one type of task.")
parser.add_option("--overwrite", action="store_true", dest="overwrite", default=False, help="Overwrite output directory")
parser.add_option("--RegularMasses", action="store_true", dest="RegularMasses", default=False, help="Only run pvalues at 110-150 in 5 GeV steps.")
parser.add_option("--SkipDatacard", action="store_true", dest="SkipDatacard", default=False, help="Don't Calculate limits or pvalues for the given datacard. Use this if you only want limits or pvalues for a custum category, but don't have a datacard yet.")
parser.add_option("--threads", type="int", dest="threads", default=0, help="Maximum Number of Threads")
(options, args) = parser.parse_args()

def waitthreads(program,maxthreads):
    time.sleep(0.1)
    threads=int(os.popen("ps | grep "+program+" | wc -l").readline())
    while threads>maxthreads:
        time.sleep(0.25)
        threads=int(os.popen("ps | grep "+program+" | wc -l").readline())
    return

if options.datacard=="":
    print "Must provide datacard!"
    exit(1)

if options.threads==0:
    options.threads=int(os.popen("cat /proc/cpuinfo | grep processor | awk '{print $3}' | tail -1 | xargs -i echo '{}+1' | bc").readlines()[0].strip('\n'))
if options.debug: print "Max Threads:",options.threads

Masses=[x * 0.1 for x in range(options.MinMass*10,options.MaxMass*10+1,options.MassStep*10)]
if options.RegularMasses: Masses=range(110,151,5)
if options.Masses!="":
    Masses=[]
    for Mass in options.Masses.split(","): Masses.append(float(Mass))

if options.debug and options.RegularMasses: print "Running with restricted masses:",Masses
else: print "Running with full mass range:",Masses

if options.toysFile!="": options.toysFile=os.path.abspath(options.toysFile)

#Make Datacards
DatacardList=[]
if not options.SkipDatacard:
    DatacardList.append(os.path.abspath(options.datacard))

basedir=os.getcwd()

if options.Categories:
    categories=os.popen("grep bin "+options.datacard+" | grep cat | grep -v combine | head -n 1 | sed 's|bin[ \\t][ \\t]*||' | sed 's|[ \\t][ \\t]*| |g' | sed 's|[ \\t][ \\t]*$||'").readlines()[0].strip("\n")
    if options.debug: print "Categories:",categories
    categorylist=categories.split(" ")
    if options.debug: print "Category List:",categorylist
    for cat in categorylist:
        veto=""
        for catveto in categorylist:
            if cat==catveto: continue
            veto+="|ch1_"+catveto
        veto=veto[1:]
        if options.debug: print "Veto string for",cat,":",veto
        outputdatacardname=basedir+"/"+os.path.basename(options.datacard).replace(".txt","_"+cat+".txt")
        if options.debug: print "combineCards.py --xc=\""+veto+"\" "+os.path.abspath(options.datacard)+" >& "+outputdatacardname
        os.system("combineCards.py --xc=\""+veto+"\" "+os.path.abspath(options.datacard)+" >& "+outputdatacardname)
        DatacardList.append(outputdatacardname)


if options.CustumCat!="":
    categorylist=os.popen("grep bin "+options.datacard+" | grep cat | grep -v combine | head -n 1 | sed 's|bin[ \\t][ \\t]*||' | sed 's|[ \\t][ \\t]*| |g' | sed 's|[ \\t][ \\t]*$||'").readlines()[0].strip("\n").split(" ")
    if options.debug: print "Category List:",categorylist
    keeplist=options.CustumCat.split(" ")
    veto=""
    for cat in categorylist:
        vetocat=True
        for keepcat in keeplist:
            if keepcat==cat: vetocat=False
        if vetocat: veto+="|ch1_"+cat
    veto=veto[1:]
    cat=options.CustumCat.replace(" ","")
    if options.debug: print "Veto string for",cat,":",veto
    outputdatacardname=basedir+"/"+os.path.basename(options.datacard).replace(".txt","_"+cat+".txt")
    if options.debug: print "combineCards.py --xc=\""+veto+"\" "+os.path.abspath(options.datacard)+" &> "+outputdatacardname
    os.system("combineCards.py --xc=\""+veto+"\" "+os.path.abspath(options.datacard)+" &> "+outputdatacardname)
    DatacardList.append(outputdatacardname)

for datacard in DatacardList:

    print "Running on datacard:",os.path.basename(datacard)
    datacardoutputdir=os.path.basename(datacard)[:os.path.basename(datacard).rfind(".txt")]
    if options.OutputDirectory!="": datacardoutputdir=options.OutputDirectory+"/"+datacardoutputdir
    if options.toysFile=="": datacardoutputdirexpected=datacardoutputdir+"_Expected_"+str(options.expectSignal)+"SM"
    if options.toysFile!="": datacardoutputdirexpected=datacardoutputdir+"_Expected_"+str(options.expectSignal)+"SM_Asimov"
    if options.Limits: limitoutputdir=datacardoutputdir+"_Limits"
    if options.BestFit: bestfitdir=datacardoutputdir+"_BestFit"
    if options.MaxLikelihood: maxlikelihoodfitdir=datacardoutputdir+"_MaxLikelihood"
    if options.nosubdirectory:
        datacardoutputdir=os.path.abspath(options.OutputDirectory)
        datacardoutputdirexpected=os.path.abspath(options.OutputDirectory)
        limitoutputdir=os.path.abspath(options.OutputDirectory)
        bestfitdir=os.path.abspath(options.OutputDirectory)

    if not options.dryrun:
        if options.OutputDirectory!="":
            if os.path.exists(options.OutputDirectory) and options.overwrite: shutil.rmtree(options.OutputDirectory)
            if not os.path.exists(options.OutputDirectory): os.makedirs(options.OutputDirectory)
        if not options.nosubdirectory:
            if options.Observed and options.PValues:
                if os.path.exists(datacardoutputdir) and options.overwrite: shutil.rmtree(datacardoutputdir)
                if not os.path.exists(datacardoutputdir): os.makedirs(datacardoutputdir)
                datacardoutputdir=os.path.abspath(datacardoutputdir)
            if options.Expected and options.PValues:
                if os.path.exists(datacardoutputdirexpected) and options.overwrite: shutil.rmtree(datacardoutputdirexpected)
                if not os.path.exists(datacardoutputdirexpected): os.makedirs(datacardoutputdirexpected)
                datacardoutputdirexpected=os.path.abspath(datacardoutputdirexpected)
            if options.Limits:
                if os.path.exists(limitoutputdir) and options.overwrite: shutil.rmtree(limitoutputdir)
                if not os.path.exists(limitoutputdir): os.makedirs(limitoutputdir)
                limitoutputdir=os.path.abspath(limitoutputdir)
            if options.BestFit:
                if os.path.exists(bestfitdir) and options.overwrite: shutil.rmtree(bestfitdir)
                if not os.path.exists(bestfitdir): os.makedirs(bestfitdir)
                bestfitdir=os.path.abspath(bestfitdir)
            if options.MaxLikelihood:
                if os.path.exists(maxlikelihoodfitdir) and options.overwrite: shutil.rmtree(maxlikelihoodfitdir)
                if not os.path.exists(maxlikelihoodfitdir): os.makedirs(maxlikelihoodfitdir)
                maxlikelihoodfitdir=os.path.abspath(maxlikelihoodfitdir)

    threadlist=[]
    threads=0
    for mass in Masses:
        massstring = str(mass)
        time.sleep(0.1)
        threads=int(os.popen("ps | grep combine | wc -l").readline())
        if options.debug: print threads,"threads running. Maximum Threads:",options.threads
        waitthreads("combine",options.threads)
        if options.Observed and options.PValues:
            os.chdir(datacardoutputdir)
            if options.debug: print "combine "+datacard+" -m "+massstring+" -M ProfileLikelihood -s -1 -n PValue --signif --pvalue >& higgsCombinePValue.ProfileLikelihood.mH"+massstring+".log &"
            if not options.dryrun: os.system("combine "+datacard+" -m "+massstring+" -M ProfileLikelihood -s -1 -n PValue --signif --pvalue >& higgsCombinePValue.ProfileLikelihood.mH"+massstring+".log &")
            waitthreads("combine",options.threads)
        if options.Expected and options.PValues and options.toysFile=="":
            os.chdir(datacardoutputdirexpected)
            if options.debug: print "combine "+datacard+" -m "+massstring+" -M ProfileLikelihood -t -1 -s -1 -n PValueExpected --signif --pvalue --expectSignal="+str(options.expectSignal)+" >& higgsCombinePValueExpected.ProfileLikelihood.mH"+massstring+".log &"
            if not options.dryrun: os.system("combine "+datacard+" -m "+massstring+" -M ProfileLikelihood -t -1 -s -1 -n PValueExpected --signif --pvalue --expectSignal="+str(options.expectSignal)+" >& higgsCombinePValueExpected.ProfileLikelihood.mH"+massstring+".log &")
            waitthreads("combine",options.threads)
        if options.Expected and options.PValues and options.toysFile!="":
            os.chdir(datacardoutputdirexpected)
            if options.debug: print "combine "+datacard+" -m "+massstring+" -M ProfileLikelihood -t -1 -s -1 -n PValueExpected --signif --pvalue --toysFile="+options.toysFile+" >& higgsCombinePValueExpected.ProfileLikelihood.mH"+massstring+".log &"
            if not options.dryrun: os.system("combine "+datacard+" -m "+massstring+" -M ProfileLikelihood -t -1 -s -1 -n PValueExpected --signif --pvalue --toysFile="+options.toysFile+" >& higgsCombinePValueExpected.ProfileLikelihood.mH"+massstring+".log &")
            waitthreads("combine",options.threads)
        if options.Limits:
            os.chdir(limitoutputdir)
            if options.debug: print "combine "+datacard+" -m "+massstring+" -M Asymptotic --minimizerStrategy=1 --minosAlgo=stepping -s -1 --picky --run=expected >& higgsCombineLimit.Asymptotic.mH"+massstring+".log &"
            if options.Expected and not options.Observed and not options.dryrun: os.system("combine "+datacard+" -m "+massstring+" -M Asymptotic --minimizerStrategy=1 --minosAlgo=stepping -s -1 --picky --run=expected >& higgsCombineLimit.Asymptotic.mH"+massstring+".log &")
            if not options.Expected and options.Observed and not options.dryrun: os.system("combine "+datacard+" -m "+massstring+" -M Asymptotic --minimizerStrategy=1 --minosAlgo=stepping -s -1 --picky --run=observed >& higgsCombineLimit.Asymptotic.mH"+massstring+".log &")
            if options.Observed and options.Expected and not options.dryrun: os.system("combine "+datacard+" -m "+massstring+" -M Asymptotic --minimizerStrategy=3 --minosAlgo=stepping  --minimizerAlgo=Minuit  -s -1  >& higgsCombineLimit.Asymptotic.mH"+massstring+".log &")
            waitthreads("combine",options.threads)
        if options.BestFit:
            os.chdir(bestfitdir)
            if options.debug: print "combine "+datacard+" -m "+massstring+" -M ChannelCompatibilityCheck --rMin=-20 --verbose=1 --saveFitResult -s -1 -n SignalStrength >& higgsCombineTest.ChannelCompatibilityCheck.mH"+massstring+".log &"
            if not options.dryrun: os.system("combine "+datacard+" -m "+massstring+" -M ChannelCompatibilityCheck --rMin=-20 --verbose=1 --saveFitResult -s -1 -n SignalStrength >& higgsCombineTest.ChannelCompatibilityCheck.mH"+massstring+".log &")
        if options.MaxLikelihood:
            os.chdir(maxlikelihoodfitdir)
            if options.debug: print "combine "+datacard+" -m "+massstring+"  -M MaxLikelihoodFit  --rMin=-20 > higgsCombineTest.MaxLikelihoodFit"+massstring+".log"
            if not options.dryrun: os.system("combine "+datacard+" -m "+massstring+"  -M MaxLikelihoodFit --minimizerAlgo=Minuit  --rMin=-20 > higgsCombineTest.MaxLikelihoodFit"+massstring+".log &")

    os.chdir(basedir)
    threads=int(os.popen("ps | grep combine | wc -l").readline())
    waitthreads("combine",0)
    if options.debug: print threads,"threads running",options.threads
    if options.PValues and options.OutputDirectory=="": options.OutputDirectory="."
    if options.Observed and options.PValues and not options.dryrun: os.system("hadd -f "+options.OutputDirectory+"/higgsCombinePValue."+os.path.basename(datacardoutputdir)+".ProfileLikelihood.root "+datacardoutputdir+"/higgsCombinePValue.ProfileLikelihood.mH[0-9][0-9]*.[0-9-][0-9]*.root >& "+datacardoutputdir+"/higgsCombinePValue.ProfileLikelihood.log &")
    if options.Expected and options.PValues and not options.dryrun: os.system("hadd -f "+options.OutputDirectory+"/higgsCombinePValue."+os.path.basename(datacardoutputdirexpected)+".ProfileLikelihood.root "+datacardoutputdirexpected+"/higgsCombinePValueExpected.ProfileLikelihood.mH[0-9][0-9]*.[0-9-][0-9]*.root >& "+datacardoutputdirexpected+"/higgsCombinePValueExpected.ProfileLikelihood.log &")
    if options.Limits and not options.dryrun:
        os.chdir(limitoutputdir)
        filelist = os.popen("/bin/ls higgsCombineTest.Asymptotic.*.root").readlines()
        for file in filelist:
            file = file.strip("\n")
            newfile = file.rsplit(".",2)[0]+"."+file.rsplit(".",2)[2]
            print "old, new,",file,newfile
            os.rename(file,newfile)
        os.chdir(basedir)
    if options.BestFit and not options.dryrun:
        os.chdir(bestfitdir)
        filelist = os.popen("/bin/ls higgsCombineSignalStrength.ChannelCompatibilityCheck.*.root").readlines()
        for file in filelist:
            file = file.strip("\n")
            newfile = file.rsplit(".",2)[0]+"."+file.rsplit(".",2)[2]
            os.rename(file,newfile)
        os.chdir(basedir)

print "Done!"
