#!/usr/bin/env python

from optparse import OptionParser, make_option
import sys
import json 
from pprint import pprint 
from math import fabs
import operator
import random
import ROOT
from copy import copy

isMVA=False


def mkBranch(tree, nt, name, type):
    tree.Branch(name,ROOT.AddressOf(nt,name),"%s/%s" % (name,type) )

def dumpTree(file,lst,isMVA,filter=None):

    file.cd()
    nt = ROOT.Entry()
    tree = ROOT.TTree("hgg_mini_tree","hgg_mini_tree")
    mkBranch(tree,nt,"run","I")
    mkBranch(tree,nt,"lumis","I")
    mkBranch(tree,nt,"event","I")
    mkBranch(tree,nt,"category","I")
    if isMVA: mkBranch(tree,nt,"diphotonMVA","F")
    mkBranch(tree,nt,"CMS_hgg_mass","F")

    for idx,vars in lst.iteritems():
        if len(vars.keys()) == 0:
            continue
        if filter and (not idx in filter):
            continue
        nt.run, nt.lumis, nt.event = idx
        if isMVA:
            nt.diphotonMVA = vars["diphoBDT"]
            nt.category = vars["evcat"]
            nt.CMS_hgg_mass = vars["mgg"]
        else:
            nt.category = vars["cat"]
            nt.CMS_hgg_mass = vars["mass"]
        tree.Fill()
    tree.Write()

def getlist(input,minrun=0,maxrun=999999999,ebonly=False):
    lst = {}

    global isMVA
    

    for line in input.split("\n"):
        vars = {}
        for i in line.replace("="," ").replace("\t"," ").replace("::","").split(" "):
            try:
                if i != "":
                    j = i.split(":")
                    if j[0] == "run" or j[0] == "lumi" or j[0] == "event":
                        globals()[j[0]] = int(j[1])
                    else:
                        try:
                            vars[j[0]] = float(j[1])
                        except Exception, e:
                            ### print e
                            pass
            except Exception, e:
                print e

        try:
            if "diphoBDT" in vars:
                isMVA = True
            else:
                if "cat" in vars:
                    vars["evcat"] = vars["cat"]
                    vars["sceta_1"] = vars["scEta1"]
                    vars["sceta_2"] = vars["scEta2"]
                    isMVA = False
        except Exception, e:
            print line, e
    

        try:
            if run<=minrun or run>maxrun:
                continue
            
            if "diphoBDT" in vars and vars["diphoBDT"] < -0.05:
                  continue

            if ebonly and ( abs(vars["sceta_1"]) > 1.5 or abs(vars["sceta_2"]) > 1.5 ):
                continue

            if ebonly and (not "diphoBDT"  in vars) and ( vars["evcat"] == 2 or vars["evcat"] == 3 ):
                print (run, lumi, event), vars
            lst[  (run, lumi, event) ] = vars
        except Exception, e:
            pass
        


    return lst

def main(options,args):

    global isMVA

    ROOT.gROOT.SetStyle("Plain")
    ROOT.gStyle.SetPalette(1)
    ROOT.gStyle.SetOptStat(111111)
    ROOT.gStyle.SetOptFit(1)
    
    ROOT.gROOT.ProcessLine( \
       "struct Entry{          \
           int run;            \
           int lumis;          \
           int event;          \
           int category;       \
           float CMS_hgg_mass; \
           float diphotonMVA;  \
          };"
       )
    
    fn1 = args.pop(0)
    file1 = open(fn1)

    fn2 = ""
    file2 = None
    if len(args)>0:
        fn2 = args.pop(0)
        file2 = open(fn2)
    
    print "reading list1"
    list1 = getlist( file1.read(), options.minrun, options.maxrun, options.ebOnly )
    set1isMVA = copy(isMVA)


    isMVA = False
    if file2:
        print "reading list2"
        list2 = getlist( file2.read(), options.minrun, options.maxrun, options.ebOnly )
    else:
        list2 = {}
    set2isMVA = copy(isMVA)

    print "getting event lists"
    events1 = list1.keys()
    events2 = list2.keys()
    
    print "computing union, intersection and differences"
    common = set(events1).intersection(  set(events2) )
    all    = list(set(events1).union(  set(events2) ))
    
    only1 = list(set(events1) -  set(events2))
    only2 = list(set(events2) -  set(events1))
    
    only1.sort()
    only2.sort()

    filter = None
    if options.commonOnly:
        filter = common
    
    if options.makeTrees:
        print "isMVA ",set1isMVA
        print "making mini tree"
        out1 = fn1.replace("txt","root")
        if options.output1 != "":
            out1 = options.output1
        fout1 = ROOT.TFile.Open(out1,"recreate")
        dumpTree(fout1,list1,set1isMVA,filter)
        fout1.Close()

    if options.makeTrees:
        print "isMVA ",set2isMVA
        print "making mini tree"
        out2 = fn2.replace("txt","root")
        if options.output2 != "":
            out2 = options.output2
        fout2 = ROOT.TFile.Open(out2,"recreate")
        dumpTree(fout2,list2,set2isMVA,filter)
        fout2.Close()

    if options.doMigrationMatrix:
        migration = ROOT.TH2F("migration","migration",10,-1,9,10,-1,9)
        for event in common:
            try:
                cat1 =  list1[event]["evcat"]
                cat2 =  list2[event]["evcat"]
                weight = 1.
                try:
                    weight = list1[event]["weight"]
                except:
                    pass
                migration.Fill(cat1,cat2,weight)
            except Exception, e:
                print e, list1[event], list2[event]
            
        for event in only1:
            try:
                cat1 =  list1[event]["evcat"]
                cat2 =  -1
                weight = 1.
                try:
                    weight = list1[event]["weight"]
                except:
                    pass
                migration.Fill(cat1,cat2,weight)
            except Exception, e:
                print e, list1[event]


        for event in only2:
            try:
                cat1 =  -1
                cat2 =  list2[event]["evcat"]
                weight = 1.
                try:
                    weight = list2[event]["weight"]
                except:
                    pass
                migration.Fill(cat1,cat2,weight)
            except Exception, e:
                print e, list2[event]

        if options.ebOnly:
            fmig = ROOT.TFile.Open("%s/migration_ebonly.root" % (options.jsondir), "recreate" )
        else:
            fmig = ROOT.TFile.Open("%s/migration.root" % (options.jsondir), "recreate" )
            
        fmig.cd()
        migration.Write()
        fmig.Close()
        
    if options.randomSeed:
        random.seed(options.randomSeed)
        open("%s/seed.txt" % (options.jsondir),"w+").write(str(options.randomSeed))

    if options.commonOnly:
        all = list(common)

    for id in options.d:
        ## all.sort()
        random.shuffle(all)

        d = int(id)
        n = len(all)
        g = n / d
        print "computing partitions: n=%d d=%d g=%d" % (n,d,g)
        if n % d != 0:
            g += 1
        parts = [ [] for k in range(g) ]
        for j in range(n):
            parts[ j % g ].append( all[j] )

        open("%s/partitions_%d.json" % (options.jsondir,d),"w+").write(json.dumps(parts))



if __name__ == "__main__":
    parser = OptionParser(option_list=[
        make_option("-d", "--delete",
                    action="append", dest="d",
                    default=[],
                    help="default: %default", metavar=""
                    ),
        make_option("-t", "--makeTrees",
                    action="store_true", dest="makeTrees",
                    default=False,
                    help="default: %default", metavar=""
                    ),
        make_option("--doMigrationMatrix",
                    action="store_true", dest="doMigrationMatrix",
                    default=False,
                    help="default: %default", metavar=""
                    ),
        make_option("--ebOnly",
                    action="store_true", dest="ebOnly",
                    default=False,
                    help="default: %default", metavar=""
                    ),
        make_option("--commonOnly",
                    action="store_true", dest="commonOnly",
                    default=False,
                    help="default: %default", metavar=""
                    ),
        make_option("-o", "--jsondir",
                    action="store", type="string", dest="jsondir",
                    default="./",
                    help="default : %default", metavar=""
                    ),
        make_option("-m", "--minrun",
                    action="store", type="int", dest="minrun",
                    default=0,
                    help="default : %default", metavar=""
                    ),
        make_option("-M", "--maxrun",
                    action="store", type="int", dest="maxrun",
                    default=999999999,
                    help="default : %default", metavar=""
                    ),
        make_option("--output1",
                    action="store", type="string", dest="output1",
                    default="",
                    help="default : %default", metavar=""
                    ),
        make_option("--output2",
                    action="store", type="string", dest="output2",
                    default="",
                    help="default : %default", metavar=""
                    ),
        make_option("--randomSeed",
                    action="store", type="int", dest="randomSeed",
                    default=None,
                    help="default : %default", metavar=""
                    ),
        
        ])
    
    (options, args) = parser.parse_args()

    print options, args

    sys.argv.append("-b")
    main( options, args ) 
