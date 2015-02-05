#!/bin/env python

import ROOT
import array
from optparse import OptionParser
from pprint import pprint
import os.path as p
import os, sys
from subprocess import check_call as call

ROOT.gROOT.SetBatch()
ROOT.gROOT.ProcessLine(".L Macros/pileup/pileup.C+g")
from ROOT import fillHists

runBinning = [0, 197495, 203767, 210000]
h1 = ROOT.TH1D("pileup","pileup",100,0,100)
h2 = ROOT.TH2D("pu_2D", "pu_2D", 100, 0, 100, 3, array.array('d',runBinning))

parser = OptionParser(usage="usage: %prog [options] EOS_source_directory\nrun with --help to get list of options")
parser.add_option("--putBack",    action="store_true", default=False, help="Put back merged file in source directory [default: %default].")
parser.add_option("--putHadoop",  action="store_true", default=False, help="Put back merged file in source directory [default: %default].")

(options, args) = parser.parse_args()

if len(args) != 1:
    parser.print_usage()
    raise RuntimeError("Need exactly one directory to work with.")

options.inDir = args[0]
options.inDirName = p.basename(options.inDir)
options.inDirBase = p.dirname(options.inDir)
options.eosLs = "/afs/cern.ch/project/eos/installation/pro/bin/eos root://eoscms ls /eos/cms"

if not options.inDirName:
    raise RuntimeError("Empty target directory name (which defines the sample name). Check path.")

try:
    ld_path = os.getenv("LD_LIBRARY_PATH")
except:
    ld_path = ""
if not "/afs/cern.ch/project/eos/installation/pro/lib64/" in ld_path:
    os.putenv("LD_LIBRARY_PATH", "%s:%s" % ( ld_path, "/afs/cern.ch/project/eos/installation/pro/lib64/" ) )
    
if options.putHadoop:
    call( """ls %(inDir)s | awk '/root$/ { print \"%(inDir)s/\"$1  }' | sed 's/\?.*$//' > %(inDirName)s.files.txt"""
      % vars(options), shell=True)
else: 
    call( """%(eosLs)s%(inDir)s | awk '/root$/ { print \"root://eoscms//eos/cms%(inDir)s/\"$1  }' | sed 's/\?.*$//' > %(inDirName)s.files.txt"""
      % vars(options), shell=True)
call( """rm -f %(inDirName)s.pileup.root %(inDirName)s.pileup.root.log""" % vars(options), shell=True)

file = open("%(inDirName)s.files.txt"%vars(options))
lines = file.readlines()
file.close()

black_list = [
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_0.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_2.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_23.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_24.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_25.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_27.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_28.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_29.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_3.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_30.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_33.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_47.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_48.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_50.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_51.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_52.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_54.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_55.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_56.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_57.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_58.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_59.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_60.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_61.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_62.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_64.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_65.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_66.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_67.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_68.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_69.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_70.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_71.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_72.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_73.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_75.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_76.root",
			"root://eoscms//eos/cms/store/group/phys_higgs/cmshgg/reduced/reduction_RDMC_June2013_v2/mc/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1/DiPhotonJetsBox_M60_8TeV-sherpa_Summer12_DR53X-PU_RD1_START53_V7N-v1_78.root"
]

for i,l in enumerate(lines):
    if l.split("\n")[0] in black_list: continue
    rootFile = ROOT.TFile.Open(l.split("\n")[0])
    tree = rootFile.Get("event")
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("pu_n", 1)
    tree.SetBranchStatus("run", 1)
    
    entries = tree.GetEntries()
    for z in xrange(entries):
        print '\t %d/%d  -- %4.1f%% \r'%(i,len(lines),100.*float(z)/entries),
        sys.stdout.flush()
        tree.GetEntry(z)
        h2.Fill(tree.pu_n, tree.run)
        h1.Fill(tree.pu_n)

print 'Done'
out = ROOT.TFile("%(inDirName)s.pileup.root"%vars(options), "recreate")
h2.Write()
h1.Write()
out.Close()

if options.putHadoop:
    cmd = """cp %(inDirName)s.pileup.root %(inDirBase)s/%(inDirName)s.pileup.root"""
    print "Copying back root"
    call(cmd % vars(options), shell=True)
    
if options.putBack:
    cmd = """cmsStage -f %(inDirName)s.pileup.root %(inDirBase)s/%(inDirName)s.pileup.root"""
    print "Copying back root"
    call(cmd % vars(options), shell=True)

