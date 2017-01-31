#!/usr/bin/env python
# 
# --------------------------------------------
# Standard python import
from optparse import OptionParser, make_option
import fnmatch, glob, os, sys, json, itertools, array
#sys.argv.append( '-b' )

## ------------------------------------------------------------------------------------------------------------------------------------------------------


#from templates_maker import buildRooDataSet
from ROOT import TH2D, TH1D, TFile, TProfile, TCanvas, TGraphAsymmErrors
from ROOT import RooWorkspace
from ROOT import RooAbsData
from ROOT import RooDataSet
from ROOT import *

gSystem.AddIncludePath("/mnt/t3nfs01/data01/shome/vtavolar/Flashgg/80forsigmaMdecorr/clean80/CMSSW_8_0_8_patch1/src/flashgg/Systematics/interface")
gSystem.Load("/mnt/t3nfs01/data01/shome/vtavolar/Flashgg/80forsigmaMdecorr/clean80/CMSSW_8_0_8_patch1/src/flashgg/Systematics/interface/DataSetFiller_h.so")
gSystem.Load("/mnt/t3nfs01/data01/shome/vtavolar/Flashgg/80forsigmaMdecorr/clean80/CMSSW_8_0_8_patch1/src/flashgg/Systematics/src/DataSetFiller_cc.so")
gROOT.ProcessLine('#include "flashgg/Systematics/interface/DataSetFiller.h"')

#gSystem.Load('transfBDT_C.so')

from ROOT import gROOT
gROOT.ForceStyle()
gStyle.SetOptTitle(0)
gStyle.SetOptStat(0)

class Load:
    def __call__(self,option, opt_str, value, parser, *args, **kwargs):
        if option.dest == "__opts__":
            dest = parser.values
        else:
            dest = getattr(parser.values,option.dest)
        origin = getattr(parser.values,"%s_src" % option.dest,None)
        if origin:
            origin += ",%s" % value
        else:
            origin = value
            setattr(parser.values,"%s_src" % option.dest,origin)

        if type(dest) == dict:
            setter = dict.__setitem__
            getter = dict.get
        else:
            setter = setattr
            getter = getattr
        
        for cfg in value.split(","):
            cf = open(cfg)
            settings = json.loads(cf.read())
            for k,v in settings.iteritems():
                print k,v
                attr  = getter(dest,k,None)
                if attr and type(attr) == list:           
                    attr.extend(v)
                setter(dest,k,v)
            cf.close()



#"variables": [
#        ("genPt","recoPt",[0.0,100.0,200.0,300.0,400.0,500.0,600.0,700.0,10000.0],[0.0,10000.0]), 
#        ("genNjets2p5","recoNjets2p5",[-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,100.0],[]) ],




def getSystLabels():
    phosystlabels=[]
    jetsystlabels=[]
    systlabels=[]
    for direction in ["Up","Down"]:
        phosystlabels.append("MvaShift%s01sigma" % direction)
#            phosystlabels.append("MvaLinearSyst%s01sigma" % direction)
        phosystlabels.append("SigmaEOverEShift%s01sigma" % direction)
        phosystlabels.append("MaterialCentral%s01sigma" % direction)
        phosystlabels.append("MaterialForward%s01sigma" % direction)
        phosystlabels.append("FNUFEB%s01sigma" % direction)
        phosystlabels.append("FNUFEE%s01sigma" % direction)
        jetsystlabels.append("JEC%s01sigma" % direction)
        jetsystlabels.append("JER%s01sigma" % direction)
        jetsystlabels.append("RMSShift%s01sigma" % direction)
        for r9 in ["HighR9","LowR9"]:
            for region in ["EB","EE"]:
                phosystlabels.append("ShowerShape%s%s%s01sigma"%(r9,region,direction))
#                    phosystlabels.append("MCSmear%s%s%s01sigma" % (r9,region,direction))
                phosystlabels.append("MCScale%s%s%s01sigma" % (r9,region,direction))
                for var in ["Rho","Phi"]:
                    phosystlabels.append("MCSmear%s%s%s%s01sigma" % (r9,region,var,direction))
    systlabels += phosystlabels
    systlabels += jetsystlabels
    return systlabels

#variables=[("leadGenPt", "subleadGenPt", [0.,100.,200.,300.,400.,500.,600.,700.,10000.])]
variables=[]
#[["genPt", "recoPt", [0.,100.,200.,300.,400.,500.,600.,700.,10000.],[0.,10000.]], ["genNjets2p5", "recoNjets2p5", [-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,100.],[]] ]
procs=[]
#procs=["InsideAcceptance_130_13TeV"]
cats=[]
#cats=["SigmaMpTTag_0", "SigmaMpTTag_1", "SigmaMpTTag_2"]

import numpy as np

savefmts=['.png','.root','.pdf','.jpg']
# Main routine
def main(o,args):
    variables = options.variables
    procs = options.procs
    cats = options.cats
    print "****variables***"
    print variables
    #from configfile import variables, procs, cats
    if 0 and options.files.startswith("/store"):
        filepath = "root://eoscms/"+str(options.files)
    else:
        filepath = options.files
    print 'filepath is '+str(filepath)
    infile = TFile.Open(filepath)
    infile.Print()
    wspace =  RooWorkspace("cms_hgg_13TeV")
###    tree = infile.Get("tagsDumper/cms_hgg_13TeV")
    wspace.Print()

    variables_0=[]    
    variables_1=[]    
    for ivarset in range(len(variables)):
        isData=False
        varset = variables[ivarset]
        print 'we look at the variable ...'
        print variables[ivarset]
        if not variables[ivarset][-1]:
#            print variables[ivarset]
                    #variables[ivarset][-1]=variables[ivarset][-2]
            variables[ivarset] = (variables[ivarset][0], variables[ivarset][1], variables[ivarset][2], variables[ivarset][2])
        if not variables[ivarset][1]:
            variables[ivarset] = (variables[ivarset][0], variables[ivarset][0], variables[ivarset][2], [1,1])
            isData=True
        variables_0.append(variables[ivarset][0])
        variables_1.append(variables[ivarset][1])

#    for proc in procs:
#        for cat in cats:
        proc = procs[0]
        cat = cats[0]
        tree = infile.Get( "tagsDumper/trees/"+str(proc)+"_"+str(cat) )
        varsInTree = []
        branchNames = tree.GetListOfBranches().Clone()
        for i in range( branchNames.GetEntries() ):
            varsInTree.append(branchNames.At(i).GetName())
        print "varsInTree"
        print varsInTree
        #do not dump into ws vars if they are needed only for splitting, or if they are "other" vars not used at the moment (not even for splitting)
        varsToDump = [var for var in varsInTree if (var not in options.othervars and var not in variables_0 and var not in variables_1) ]
        print "varsToDump"
        print varsToDump
        #eliminate duplicates from list
        varsToDump = list(set(varsToDump))
        roorealvars=[]
        rooarglist=RooArgList()
        limitedvars=[]
        rrv0=RooRealVar(varsToDump[0],varsToDump[0],-10.,10.)
        rrv1=RooRealVar(varsToDump[1],varsToDump[1],-10.,10.)
        ras = RooArgSet(rrv0,rrv1)
#        print varsToDump[20]
#        print varsToDump[23]
#        del varsToDump[23]
        
        print "varsToDump before for"
        print varsToDump
        for var in varsToDump:
            print "print them one by one"
            print var
#            val = tree.GetBranch(var).GetEntry(0)
#            print type(val)
##            if type(val) is int:
##                print "skipping var ",var
##                continue
            if var in limitedvars:
                rrv = RooRealVar(str(var),limitedvars[var]["min"],limitedvars[var]["max"])
                roorealvars.append(rrv)
                rooarglist.add(rrv)
            else:
                rrv = RooRealVar(str(var), str(var), -float("inf"), float("inf"))
#                rrv = wspace.factory(str(var)+"[0.]")
                rrv.Print()
#                rrv.SetName(str(var))
#                rrv.SetTitle(str(var))
                roorealvars.append(rrv)
                rooarglist.add(rrv, True)
        datasetsReduced={}
        print roorealvars
        rooarglist.Print()
        for proc in procs:
            print "proc ",proc
            for cat in cats:
                print "cat ",cat
                tree = infile.Get( "tagsDumper/trees/"+str(proc)+"_"+str(cat) )
                
                for bound1 in range(len(variables[ivarset][-2])-1):
                    print "bound1 ", variables[ivarset][-2][bound1]
                    for bound2 in range(len(variables[ivarset][-1])-1):
                        print "bound2 ", variables[ivarset][-2][bound2]
#                name = str(proc)+"_"+str(cat), str(proc)+"_"+str(cat)
                        lowb1 = str(variables[ivarset][-2][bound1]).replace("-","m").replace(".","p")
                        upb1 = str(variables[ivarset][-2][bound1+1]).replace("-","m").replace(".","p")
                        lowb2 = str(variables[ivarset][-1][bound2]).replace("-","m").replace(".","p")
                        upb2 = str(variables[ivarset][-1][bound2+1]).replace("-","m").replace(".","p")
###                        dname = str(proc.split('_',1)[0])+"_"+variables[ivarset][0]+"_"+str(variables[ivarset][-2][bound1])+"to"+str(variables[ivarset][-2][bound1+1])+"_"+str(proc.split('_',1)[1])+"_"+str(cat)+"_"+variables[ivarset][1]+"_"+str(variables[ivarset][-1][bound2])+"to"+str(variables[ivarset][-1][bound2+1])
                        dname = str(proc.split('_',1)[0])+"_"+variables[ivarset][0]+"_"+str(lowb1)+"to"+str(upb1)+"_"+str(proc.split('_',1)[1])+"_"+str(cat)+"_"+variables[ivarset][1]+"_"+str(lowb2)+"to"+str(upb2)
                        if isData:
                            lowb1 = str(varset[-2][bound1]).replace("-","m").replace(".","p")
                            upb1 = str(varset[-2][bound1+1]).replace("-","m").replace(".","p")
                            dname = str(proc)+"_"+str(cat)+"_"+varset[0]+"_"+str(lowb1)+"to"+str(upb1)
                        filler = DataSetFiller(str(dname), str(dname), rooarglist, "weight", False)
                        wei = TCut("weight")
#                        selCondition = (val1 > variables[ivarset][-2][bound1]) and (val1 < variables[ivarset][-2][bound1+1]) and (val2 > variables[ivarset][-1][bound2]) and (val2 < variables[ivarset][-1][bound2+1])
                        if not isData:
                            print  str(variables[ivarset][0]) + " > " + str(variables[ivarset][-2][bound1]) + " && " + str(variables[ivarset][0]) + " < " + str(variables[ivarset][-2][bound1+1]) + " && " + str(variables[ivarset][1]) + " > " + str(variables[ivarset][-1][bound2]) + " && " + str(variables[ivarset][1]) + " < " + str(variables[ivarset][-1][bound2+1]) 
                            wei *= TCut( str(variables[ivarset][0]) + " > " + str(variables[ivarset][-2][bound1]) + " && " + str(variables[ivarset][0]) + " < " + str(variables[ivarset][-2][bound1+1]) + " && " + str(variables[ivarset][1]) + " > " + str(variables[ivarset][-1][bound2]) + " && " + str(variables[ivarset][1]) + " < " + str(variables[ivarset][-1][bound2+1])  )
                        else:
                            print  str(variables[ivarset][0]) + " > " + str(variables[ivarset][-2][bound1]) + " && " + str(variables[ivarset][0]) + " < " + str(variables[ivarset][-2][bound1+1]) 
                            wei *= TCut( str(variables[ivarset][0]) + " > " + str(variables[ivarset][-2][bound1]) + " && " + str(variables[ivarset][0]) + " < " + str(variables[ivarset][-2][bound1+1]) )
                        wei.Print()
                        filler.fillFromTree(tree,wei.GetTitle())
                        ds = filler.get()
#                ds = RooDataSet(str(proc)+"_"+str(cat), str(proc)+"_"+str(cat))
                        ds.Print("V")
                        datasetsReduced[dname]=ds


        ###now let's read shifted trees (systematics)
        systlabels = getSystLabels()
        rrv_cms_mass = RooRealVar("CMS_hgg_mass", "CMS_hgg_mass", 100, 180)
        rrv_cms_mass.setBins(160)
        rrv_weight = RooRealVar("weight", "weight", -float("inf"), float("inf"))
        rooarglist=RooArgList(rrv_cms_mass,rrv_weight)
        rrv_pdfWeights={}
        for iw in range(60):
            rrv_pdfw = RooRealVar("pdfWeight_"+str(iw), "weight*pdfWeights["+str(iw)+"]/pdfWeights[0]", -float("inf"), float("inf"))
            rrv_pdfWeights[rrv_pdfw.GetName()] = (rrv_pdfw)
        rooarglist_pdfw=RooArgList(rrv_cms_mass,rrv_weight)
        for irrv in rrv_pdfWeights.values():
            rooarglist_pdfw.add(irrv)
        for proc in procs:
            print "proc ",proc
            for cat in cats:
                print "cat ",cat
                for bound1 in range(len(variables[ivarset][-2])-1):
                    print "bound1 ", variables[ivarset][-2][bound1]
                    for bound2 in range(len(variables[ivarset][-1])-1):
                        print "bound2 ", variables[ivarset][-2][bound2]
    #                name = str(proc)+"_"+str(cat), str(proc)+"_"+str(cat)
                        lowb1 = str(variables[ivarset][-2][bound1]).replace("-","m").replace(".","p")
                        upb1 = str(variables[ivarset][-2][bound1+1]).replace("-","m").replace(".","p")
                        lowb2 = str(variables[ivarset][-1][bound2]).replace("-","m").replace(".","p")
                        upb2 = str(variables[ivarset][-1][bound2+1]).replace("-","m").replace(".","p")
   ###                        dname = str(proc.split('_',1)[0])+"_"+variables[ivarset][0]+"_"+str(variables[ivarset][-2][bound1])+"to"+str(variables[ivarset][-2][bound1+1])+"_"+str(proc.split('_',1)[1])+"_"+str(cat)+"_"+variables[ivarset][1]+"_"+str(variables[ivarset][-1][bound2])+"to"+str(variables[ivarset][-1][bound2+1])+"_pdfWeights"
                        dname = str(proc.split('_',1)[0])+"_"+variables[ivarset][0]+"_"+str(lowb1)+"to"+str(upb1)+"_"+str(proc.split('_',1)[1])+"_"+str(cat)+"_"+variables[ivarset][1]+"_"+str(lowb2)+"to"+str(upb2)+"_pdfWeights"
                        if isData:
                            lowb1 = str(varset[-2][bound1]).replace("-","m").replace(".","p")
                            upb1 = str(varset[-2][bound1+1]).replace("-","m").replace(".","p")
                            dname = str(proc)+"_"+str(cat)+"_"+varset[0]+"_"+str(lowb1)+"to"+str(upb1)+"_pdfWeights"
                        filler = DataSetFiller(str(dname), str(dname), rooarglist_pdfw, "weight", False)
                            ###wei = TCut("weight")
                        wei = TCut("1")
                            #                        selCondition = (val1 > variables[ivarset][-2][bound1]) and (val1 < variables[ivarset][-2][bound1+1]) and (val2 > variables[ivarset][-1][bound2]) and (val2 < variables[ivarset][-1][bound2+1])
                        if not isData:
                            print  str(variables[ivarset][0]) + " > " + str(variables[ivarset][-2][bound1]) + " && " + str(variables[ivarset][0]) + " < " + str(variables[ivarset][-2][bound1+1]) + " && " + str(variables[ivarset][1]) + " > " + str(variables[ivarset][-1][bound2]) + " && " + str(variables[ivarset][1]) + " < " + str(variables[ivarset][-1][bound2+1]) 
                            wei *= TCut( str(variables[ivarset][0]) + " > " + str(variables[ivarset][-2][bound1]) + " && " + str(variables[ivarset][0]) + " < " + str(variables[ivarset][-2][bound1+1]) + " && " + str(variables[ivarset][1]) + " > " + str(variables[ivarset][-1][bound2]) + " && " + str(variables[ivarset][1]) + " < " + str(variables[ivarset][-1][bound2+1])  )
                        else:
                            print  str(variables[ivarset][0]) + " > " + str(variables[ivarset][-2][bound1]) + " && " + str(variables[ivarset][0]) + " < " + str(variables[ivarset][-2][bound1+1]) 
                            wei *= TCut( str(variables[ivarset][0]) + " > " + str(variables[ivarset][-2][bound1]) + " && " + str(variables[ivarset][0]) + " < " + str(variables[ivarset][-2][bound1+1]) )
                        wei.Print()
                        filler.fillFromTree(tree,wei.GetTitle())
                        ds = filler.get()
#                        binned_ds = ds.binnedClone(str(dname))
#                        binned_ds.SetTitle(str(dname))
                        ds.SetName(str(dname))
                        ds.SetTitle(str(dname))
                        totpdfweights={}
                        ##initialise keys and values of pdfweights dict (so we don't have to check it at each iteration)

                        iset=ds.get()
                        iset.Print()
                        print iset
                        iteriset = iset.createIterator()
                        iwvar = iteriset.Next()
#                        for iwvar in iset:
                        while iwvar:
                            totpdfweights[iwvar.GetName()] = 0.
                            iwvar = iteriset.Next()

                        #now loop through the entries and sum    
                        for ientry in range(ds.numEntries()):
                            iset=ds.get(ientry)
                            iteriset = iset.createIterator()
                            iwvar = iteriset.Next()
                            while iwvar:
#                            for iwvar in iset:
                                totpdfweights[iwvar.GetName()] = totpdfweights[iwvar.GetName()] + iset.getRealValue(iwvar.GetName())
                                iwvar = iteriset.Next()
                        for ikey in totpdfweights.keys():
                            if ikey=='CMS_hgg_mass':
                                print "we found ",ikey
                                print "so we continue"
                                continue
                            if datasetsReduced[dname.replace("_pdfWeights","")].sumEntries() != 0:
                                totpdfweights[ikey] = totpdfweights[ikey]/datasetsReduced[dname.replace("_pdfWeights","")].sumEntries()
                            else:
                                totpdfweights[ikey]=0.
                            rrv_pdfWeights[ikey].setVal(totpdfweights[ikey])
#                            ds.var(str(ikey)).setVal(totpdfweights[ikey])
                        rooarglist_pdfw_final=RooArgSet()
                        for irrv in rrv_pdfWeights.values():
                            rooarglist_pdfw_final.add(irrv)
                        rrv_sumentries = RooRealVar("sumW","sumW", 0)
                        rrv_sumentries.setVal(datasetsReduced[dname.replace("_pdfWeights","")].sumEntries())
                        ds_final = RooDataSet(str(dname), str(dname), rooarglist_pdfw_final, rrv_sumentries.GetName())
                        ds_final.add( rooarglist_pdfw_final, rrv_sumentries.getVal() )
    #                ds = RooDataSet(str(proc)+"_"+str(cat), str(proc)+"_"+str(cat))
                        ds.Print("V")
                        ds_final.Print("V")
#                        datasetsReduced[dname]=binned_ds
#                        datasetsReduced[dname]=ds
                        datasetsReduced[dname]=ds_final
                
                        
            
        for label in systlabels:
            for proc in procs:
                print "proc ",proc
                for cat in cats:
                    print "cat ",cat
                    tree = infile.Get( "tagsDumper/trees/"+str(proc)+"_"+str(cat)+"_"+str(label) )
                    
                    for bound1 in range(len(variables[ivarset][-2])-1):
                        print "bound1 ", variables[ivarset][-2][bound1]
                        for bound2 in range(len(variables[ivarset][-1])-1):
                            print "bound2 ", variables[ivarset][-2][bound2]
    #                name = str(proc)+"_"+str(cat), str(proc)+"_"+str(cat)
                            lowb1 = str(variables[ivarset][-2][bound1]).replace("-","m").replace(".","p")
                            upb1 = str(variables[ivarset][-2][bound1+1]).replace("-","m").replace(".","p")
                            lowb2 = str(variables[ivarset][-1][bound2]).replace("-","m").replace(".","p")
                            upb2 = str(variables[ivarset][-1][bound2+1]).replace("-","m").replace(".","p")
###                            dname = str(proc.split('_',1)[0])+"_"+variables[ivarset][0]+"_"+str(variables[ivarset][-2][bound1])+"to"+str(variables[ivarset][-2][bound1+1])+"_"+str(proc.split('_',1)[1])+"_"+str(cat)+"_"+variables[ivarset][1]+"_"+str(variables[ivarset][-1][bound2])+"to"+str(variables[ivarset][-1][bound2+1])+"_"+str(label)
                            dname = str(proc.split('_',1)[0])+"_"+variables[ivarset][0]+"_"+str(lowb1)+"to"+str(upb1)+"_"+str(proc.split('_',1)[1])+"_"+str(cat)+"_"+variables[ivarset][1]+"_"+str(lowb2)+"to"+str(upb2)+"_"+str(label)
                            if isData:
                                lowb1 = str(varset[-2][bound1]).replace("-","m").replace(".","p")
                                upb1 = str(varset[-2][bound1+1]).replace("-","m").replace(".","p")
                                dname = str(proc)+"_"+str(cat)+"_"+varset[0]+"_"+str(lowb1)+"to"+str(upb1)+"_"+str(label)
                            filler = DataSetFiller(str(dname), str(dname), rooarglist, "weight", False)
                            wei = TCut("weight")
    #                        selCondition = (val1 > variables[ivarset][-2][bound1]) and (val1 < variables[ivarset][-2][bound1+1]) and (val2 > variables[ivarset][-1][bound2]) and (val2 < variables[ivarset][-1][bound2+1])
                            if not isData:
                                print  str(variables[ivarset][0]) + " > " + str(variables[ivarset][-2][bound1]) + " && " + str(variables[ivarset][0]) + " < " + str(variables[ivarset][-2][bound1+1]) + " && " + str(variables[ivarset][1]) + " > " + str(variables[ivarset][-1][bound2]) + " && " + str(variables[ivarset][1]) + " < " + str(variables[ivarset][-1][bound2+1]) 
                                wei *= TCut( str(variables[ivarset][0]) + " > " + str(variables[ivarset][-2][bound1]) + " && " + str(variables[ivarset][0]) + " < " + str(variables[ivarset][-2][bound1+1]) + " && " + str(variables[ivarset][1]) + " > " + str(variables[ivarset][-1][bound2]) + " && " + str(variables[ivarset][1]) + " < " + str(variables[ivarset][-1][bound2+1])  )
                            else:
                                print  str(variables[ivarset][0]) + " > " + str(variables[ivarset][-2][bound1]) + " && " + str(variables[ivarset][0]) + " < " + str(variables[ivarset][-2][bound1+1]) 
                                wei *= TCut( str(variables[ivarset][0]) + " > " + str(variables[ivarset][-2][bound1]) + " && " + str(variables[ivarset][0]) + " < " + str(variables[ivarset][-2][bound1+1]) )
                            wei.Print()
                            filler.fillFromTree(tree,wei.GetTitle())
                            ds = filler.get()
                            binned_ds = ds.binnedClone(str(dname))
                            binned_ds.SetTitle(str(dname))
    #                ds = RooDataSet(str(proc)+"_"+str(cat), str(proc)+"_"+str(cat))
                            ds.Print("V")
                            datasetsReduced[dname]=binned_ds





        new_wspace = RooWorkspace("cms_hgg_13TeV")
###        getattr(new_wspace, 'import')(wspace.var("CMS_hgg_mass"))
###        getattr(new_wspace, 'import')(wspace.var("IntLumi"))
###        if not isData:
###            getattr(new_wspace, 'import')(wspace.var("dZ"))
###        getattr(new_wspace, 'import')(wspace.var("weight"))
#    for ds in alldata :
#        new_wspace.removeSet(str(ds.GetName()))
        outfilename = options.outfile.split('.')[0]+'_'+str(variables[ivarset][0])+'_3_testPdfWeights.root'
        outfile = TFile(outfilename, 'RECREATE')
        stepsize=int(len(datasetsReduced.keys())/10)
        iteration = 0
        while(len(datasetsReduced.keys())>0):
            print 'iteration '+str(iteration)
            iteration=iteration+1
            try: outfile
            except NameError: outfile = TFile(outfilename, 'UPDATE')

            try: new_wspace
            except NameError: 
                outfile.ls()
                new_wspace = outfile.Get("cms_hgg_13TeV")
                print "cms_hgg_13TeV;1"
                gDirectory.Delete("cms_hgg_13TeV;1")
                outfile.ls()
#        if not 'outfile' in globals():
#            print 'outfile is not defined'
#            outfile = TFile(options.outfile, 'UPDATE')
#        if not 'new_wspace' in globals():
#            new_wspace = outfile.Get("cms_hgg_13TeV")
#            print 'newspace is not defined'
            written=[]
            if stepsize > len(datasetsReduced.keys()):
                stepsize = len(datasetsReduced.keys())
            for ikey in range(stepsize):
            #    for ikey in range(len(datasetsReduced.keys())):
            #        dataset.Print("V")
            #        if dataset.numEntries() > 100:
                getattr(new_wspace, 'import')(datasetsReduced[datasetsReduced.keys()[ikey]])
                datasetsReduced[datasetsReduced.keys()[ikey]].Write()
                written.append(datasetsReduced.keys()[ikey])
            print 'Number of data in ws '
            print len(new_wspace.allData())
        #new_wspace.Print()
            new_wspace.Write()
            outfile.Write()
            outfile.Close()
            for wkey in written:
                del datasetsReduced[wkey]
            del new_wspace
            del outfile






#    isData=False
#    dname=""
#    print variables
#    for varset in variables:
#        print 'we look at the variable ...'
#        print varset
#        if not varset[-1]:
#                    #varset[-1]=varset[-2]
#            varset = (varset[0], varset[1], varset[2], varset[2])
#        if not varset[1]:
#            varset = (varset[0], varset[0], varset[2], [1,1])
#            isData=True
#        datasetsReduced={}
#        c=TCanvas("c","",1)
#        binning1=varset[-2]
#        binning1[-1]=binning1[-2]*1.2
#        print binning1
#        binning2=varset[-1]
#        binning2[-1]=binning2[-2]*1.2
#        print binning2
#        th2sigtot=TH2D("signalEv_"+str(varset[0])+"_vs_"+str(varset[1])+"_all", "signalEv_"+str(varset[0])+"_vs_"+str(varset[1])+"_all", len(binning1)-1, np.asarray(binning1), len(binning2)-1, np.asarray(binning2))
#        th2sigtot.GetXaxis().SetTitle(str(varset[0]))
#        th2sigtot.GetYaxis().SetTitle(str(varset[1]))
#        for proc in procs:
#            for cat in cats:
#                print "reading ds "+str(proc)+"_"+str(cat)+" from ws"
#                dataset = wspace.data(str(proc)+"_"+str(cat))
#                dataset.Print()
#                th2sig = TH2D("signalEv_"+str(varset[0])+"_vs_"+str(varset[1])+"_"+str(proc)+"_"+str(cat), "signalEv_"+str(varset[0])+"_vs_"+str(varset[1])+"_"+str(proc)+"_"+str(cat), len(binning1)-1, np.asarray(binning1), len(binning2)-1, np.asarray(binning2))
#                th2sig.GetXaxis().SetTitle(str(varset[0]))
#                th2sig.GetYaxis().SetTitle(str(varset[1]))
#                for bound1 in range(len(varset[-2])-1):
#                    for bound2 in range(len(varset[-1])-1):
#                        if isData:
#                            dname = str(proc)+"_"+str(cat)+"_"+varset[0]+"_"+str(varset[-2][bound1])+"to"+str(varset[-2][bound1+1])
#                        else:
##                            dname = str(proc)+"_"+str(cat)+"_"+varset[0]+"_"+str(varset[-2][bound1])+"to"+str(varset[-2][bound1+1])+"_Vs_"+varset[1]+"_"+str(varset[-1][bound2])+"to"+str(varset[-1][bound2+1])
#                            dname = str(proc.split('_',1)[0])+"_"+varset[0]+"_"+str(varset[-2][bound1])+"to"+str(varset[-2][bound1+1])+"_"+str(proc.split('_',1)[1])+"_"+str(cat)+"_"+varset[1]+"_"+str(varset[-1][bound2])+"to"+str(varset[-1][bound2+1])
#                        datRed = dataset.Clone(dname)
#                        datRed.reset()
#                        datasetsReduced[ dname ] = datRed
#                val=0.
#                maxEntries = dataset.numEntries()
#                if options.maxEntries != -1:
#                    maxEntries = options.maxEntries
#                for i in range(maxEntries):
#                    if i%10000==0: 
#                        print i
#                    iset = dataset.get(i)
#                    val1 = iset.getRealValue(varset[0])
#                    val2 = iset.getRealValue(varset[1])
##                    print val1,val2
#
#                    for bound1, bound2 in ((b1,b2) for b1 in range(len(varset[-2])-1) for b2 in range(len(varset[-1])-1) ):
#                        if isData:
#                            selCondition = (val1 > varset[-2][bound1]) and (val1 < varset[-2][bound1+1]) 
#                            dname = str(proc)+"_"+str(cat)+"_"+varset[0]+"_"+str(varset[-2][bound1])+"to"+str(varset[-2][bound1+1])
#                        else:
#                            selCondition = (val1 > varset[-2][bound1]) and (val1 < varset[-2][bound1+1]) and (val2 > varset[-1][bound2]) and (val2 < varset[-1][bound2+1])
##                            dname = str(proc)+"_"+str(cat)+"_"+varset[0]+"_"+str(varset[-2][bound1])+"to"+str(varset[-2][bound1+1])+"_Vs_"+varset[1]+"_"+str(varset[-1][bound2])+"to"+str(varset[-1][bound2+1])
#                            dname = str(proc.split('_',1)[0])+"_"+varset[0]+"_"+str(varset[-2][bound1])+"to"+str(varset[-2][bound1+1])+"_"+str(proc.split('_',1)[1])+"_"+str(cat)+"_"+varset[1]+"_"+str(varset[-1][bound2])+"to"+str(varset[-1][bound2+1])
#                        if selCondition:
##                            print "filling dataset"
###                            print dataset.weight()
#                            datasetsReduced[dname].add(iset, dataset.weight())
#                            break
#                for bound1 in range(len(varset[-2])-1):
#                    for bound2 in range(len(varset[-1])-1):
#                        if isData:
#                            dname = str(proc)+"_"+str(cat)+"_"+varset[0]+"_"+str(varset[-2][bound1])+"to"+str(varset[-2][bound1+1])
#                        else:
#                            dname = str(proc.split('_',1)[0])+"_"+varset[0]+"_"+str(varset[-2][bound1])+"to"+str(varset[-2][bound1+1])+"_"+str(proc.split('_',1)[1])+"_"+str(cat)+"_"+varset[1]+"_"+str(varset[-1][bound2])+"to"+str(varset[-1][bound2+1])
#                            
#                            print "binning1 ",float(binning1[bound1])+(float(binning1[bound1+1])-float(binning1[bound1]))/2.
#                            print binning1[bound1+1]
#                            print binning1[bound1]
#                            print "cat1 ",(float(varset[-2][bound1+1])-float(varset[-2][bound1]))/2.
#                            print "binning2 ",float(binning2[bound2])+(float(binning2[bound2+1])-float(binning2[bound2]))/2.
#                            print binning2[bound2+1]
#                            print binning2[bound2]
#                            print "cat2 ",(float(varset[-1][bound2+1])-float(varset[-1][bound2]))/2.
#                            print "sumentries ",datasetsReduced[dname].sumEntries()
#                            th2sig.Fill( float(binning1[bound1])+(float(binning1[bound1+1])-float(binning1[bound1]))/2., float(binning2[bound2])+(float(binning2[bound2+1])-float(binning2[bound2]))/2., datasetsReduced[dname].sumEntries())
#                    
#                th2sig.Print("V")
#                c.cd()
#                if options.logz:
#                    c.SetLogz(1)
#
#                th2sigtot.Add(th2sig)
#                th2sig.Draw("colz")
#                for fmt in savefmts:
#                    savename = th2sig.GetName()
#                    if options.logz:
#                        savename = str(savename)+"_logz"
#                    c.SaveAs(str(savename)+str(fmt))
#        c.cd()
#        if options.logz:
#            c.SetLogz(1)
#        th2sigtot.Draw("colz")
#        savename = th2sigtot.GetName()
#        if options.logz:
#            savename = str(savename)+"_logz"
#        for fmt in savefmts:
#            c.SaveAs(str(savename)+str(fmt))
##            print "####  Compare reductions  ####"
#        
#        new_wspace = RooWorkspace("cms_hgg_13TeV")
#        getattr(new_wspace, 'import')(wspace.var("CMS_hgg_mass"))
#        getattr(new_wspace, 'import')(wspace.var("IntLumi"))
#        if not isData:
#            getattr(new_wspace, 'import')(wspace.var("dZ"))
#        getattr(new_wspace, 'import')(wspace.var("weight"))
##    alldata = new_wspace.allData()
##    for ds in alldata :
##        new_wspace.removeSet(str(ds.GetName()))
#        outfilename = options.outfile.split('.')[0]+'_'+str(varset[0])+'_3th2.root'
#        outfile = TFile(outfilename, 'RECREATE')
#        stepsize=int(len(datasetsReduced.keys())/10)
#        iteration = 0
#        while(len(datasetsReduced.keys())>0):
#            print 'iteration '+str(iteration)
#            iteration=iteration+1
#            try: outfile
#            except NameError: outfile = TFile(outfilename, 'UPDATE')
#
#            try: new_wspace
#            except NameError: 
#                outfile.ls()
#                new_wspace = outfile.Get("cms_hgg_13TeV")
#                print "cms_hgg_13TeV;1"
#                gDirectory.Delete("cms_hgg_13TeV;1")
#                outfile.ls()
##        if not 'outfile' in globals():
##            print 'outfile is not defined'
##            outfile = TFile(options.outfile, 'UPDATE')
##        if not 'new_wspace' in globals():
##            new_wspace = outfile.Get("cms_hgg_13TeV")
##            print 'newspace is not defined'
#            written=[]
#            if stepsize > len(datasetsReduced.keys()):
#                stepsize = len(datasetsReduced.keys())
#            for ikey in range(stepsize):
#            #    for ikey in range(len(datasetsReduced.keys())):
#            #        dataset.Print("V")
#            #        if dataset.numEntries() > 100:
#                getattr(new_wspace, 'import')(datasetsReduced[datasetsReduced.keys()[ikey]])
#                datasetsReduced[datasetsReduced.keys()[ikey]].Write()
#                written.append(datasetsReduced.keys()[ikey])
#            print 'Number of data in ws '
#            print len(new_wspace.allData())
#        #new_wspace.Print()
#            new_wspace.Write()
#            outfile.Write()
#            outfile.Close()
#            for wkey in written:
#                del datasetsReduced[wkey]
#            del new_wspace
#            del outfile
#
#
##    for dataset in datasetsReduced.values():
###        dataset.Print("V")
###        if dataset.numEntries() > 100:
##        getattr(new_wspace, 'import')(dataset)
##        dataset.Write()
###            datasetReduce.Print("V")
###            datasetReduce2.Print("V")
##    histos={}
##    histos= getPlots(infile)
##    #draw and save plots
##    outfile.cd()
##    for histo in histos:
##        histo.Write()
##        c = TCanvas('c','',1)
##        histo.Draw('ehist')
##        c.SetLogy(variables[str(histo.GetName()).split('_')[1]]['logy'])
##        c.SetLogx(variables[str(histo.GetName()).split('_')[1]]['logx'])
##        for fmt in savefmts:
##            c.SaveAs(str(options.outdir)+'/'+str(histo.GetName())+str(fmt))
##    new_wspace.Write()
#
##    outfile.Write()
##    outfile.Close()

    

## ------------------------------------------------------------------------------------------------------------------------------------------------------    
if __name__ == "__main__":
    parser = OptionParser(option_list=[
            make_option("-i", "--indir",
                        action="store", type="string", dest="indir",
                        default="./",
                        help="input directory", metavar="DIR"
                        ),
            make_option("-f", "--files",
                        action="store", type="string", dest="files",
                        default="allSig125IA.root",
                        help="pattern of files to be read", metavar="PATTERN"
                        ), 
            make_option("-t", "--treeName",
                        action="store", type="string", dest="treename",
                        default="TestTree",
                        help="TTree name", metavar="TREENAME"
                        ),
            make_option("-o", "--outfile",
                        action="store", type="string", dest="outfile",
                        default="reduced.root",
                        help="outputfile", metavar="FILE"
                        ),
            make_option("-l", "--label",
                        action="store", type="string", dest="label",
                        default="",
                        help="label", metavar="LABEL"
                        ),
            make_option("-w", "--weightsdir",
                        action="store", type="string", dest="weightsdir",
                        default="weights",
                        help="tmva weights dir", metavar="DIR"
                        ),
#            make_option("-V", "--variables",
#                        action="store", dest="variables", type="string",
#                        default="",
#                        help="list of variables"
#                        ),
            make_option("-T", "--tmvaSettings",
                        action="store", dest="tmvaSettings", type="string",
                        default="dipho.json",
                        help="settings for the TMVA training"
                        ),
            make_option("-v", "--verbose",
                        action="store_true", dest="verbose",
                        default=False,
                        ),
            make_option("-O", "--optimize",
                        action="store_true", dest="optimize",
                        default=False,
                        ),
            make_option("-D", "--outputdir",
                        action="store_true", dest="outdir",
                        default="plots",
                        ),
            make_option("-L", "--logz",
                        action="store_true", dest="logz",
                        default=False,
                        ),
            make_option("-N", "--maxEntries",
                        action="store", type="int", dest="maxEntries",
                        default=-1,
                        ),
            make_option("--load",  # special option to load whole configuaration from JSON
                        action="callback",callback=Load(),dest="__opts__",
                        type="string",
                        help="load JSON file with configuration",metavar="reduce_cfg.json"
                        ),

            ])

    (options, args) = parser.parse_args()

    sys.argv.append("-b")
    main(options, args)
