#!/bin/env python

from optparse import OptionParser
from pprint import pprint
import os.path

def which(program):
    import os
    def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

    fpath, fname = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file
    
    return None

parser = OptionParser(usage="usage: %prog [options] some_data.json\nrun with --help to get list of options")
parser.add_option("--skipLumi",   action="store_true", default=False, help="Skip lumi calculation [default: %default].")
parser.add_option("--skipPileup", action="store_true", default=False, help="Skip PU calculation [default: %default].")
parser.add_option("--minBiasXsec", default=68300,  type="int", help="Minimum bias cross section to use in calculating the PU [default: %default].")
parser.add_option("--maxPileupBin",  default=100,  type="int", help="Largest PU value [default: %default].")
#parser.add_option("--numPileupBins", default=100,  type="int", help="Number of PU bins [default: %default].")
parser.add_option("--pileupJson", default='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/PileUp/pileup_JSON_DCSONLY_190389-194533.txt', help="Pileup JSON file [default: %default].", metavar='pileup_JSON.txt')
parser.add_option("--dryRun", action="store_true", default=False, help="Do not execute the commands [default: %default].")

(options, args) = parser.parse_args()

if options.skipPileup and options.skipLumi:
    raise RuntimeError("Skip both PU and Lumi calculation? Nothing to do.")


if len(args) != 1:
    parser.print_usage()
    raise RuntimeError("Need exactly one JSON file to work with.")

options.inputJson = args[0]
options.inputJsonName = os.path.basename(options.inputJson)

jobs = list()

#pu
if not options.skipPileup:
    calcModes = { 'true':
                  {'pileupHistName':'pileup_true', 'numPileupBins': options.maxPileupBin * 10},
                  'observed':
                  {'pileupHistName':'pileup', 'numPileupBins': options.maxPileupBin},
                  }
    
    cmd = "pileupCalc.py\
    --pileupHistName %(pileupHistName)s --calcMode %(calcMode)s\
    --minBiasXsec %(minBiasXsec)s\
    -i %(inputJson)s\
    --inputLumiJSON %(pileupJson)s\
    --maxPileupBin %(maxPileupBin)d   --numPileupBins %(numPileupBins)d\
    %(inputJsonName)s.%(minBiasXsec)s.%(calcMode)s.pileup.root &> %(inputJsonName)s.%(minBiasXsec)s.%(calcMode)s.pileup.root.log"
    
    for options.calcMode in calcModes.keys():
        print "Queuing %(calcMode)s PU calculation." % vars(options)
        jobs.append(cmd % dict( vars(options).items() + calcModes[options.calcMode].items() ) )


#lumi
if not options.skipLumi:
    calculators = [ 'lumiCalc2', 'pixelLumiCalc' ]
    cmd = "%(calculator)s.py -i %(inputJson)s --nowarning overview &> %(inputJsonName)s.%(calculator)s.lumi"

    for options.calculator in calculators:
        program = "%(calculator)s.py" % vars(options)
        if not which(program):
            raise RuntimeError("%s not found. Check https://twiki.cern.ch/twiki/bin/view/CMS/LumiCalc for how to install lumi stuff." % program)
        print "Queuing %(calculator)s lumi calculation." % vars(options)
        jobs.append(cmd % vars(options))

if options.dryRun:
    print "Dry run. Here are the jobs I would run:"
    pprint( jobs )
    raise SystemExit(0)


from subprocess import call

#function to be used in parallel
def runCmd(cmd):
    if not cmd: return 0
    return call(cmd, shell=True)

from multiprocessing import Pool
pool = Pool()

# run all jobs in parallel
print "Running queued commmands in parallel. (This can take a while...)"
ret = pool.map(runCmd, jobs)
if reduce(lambda x, y: x+y, ret):
        raise RuntimeError, "Non-zero return code in parallel loop. Check logs."

#merge the pileup files
cmd = "rm %(inputJsonName)s.%(minBiasXsec)s.pileup.root; \
hadd %(inputJsonName)s.%(minBiasXsec)s.pileup.root \
%(inputJsonName)s.%(minBiasXsec)s.true.pileup.root \
%(inputJsonName)s.%(minBiasXsec)s.observed.pileup.root \
&> %(inputJsonName)s.%(minBiasXsec)s.pileup.root.log"

call(cmd % vars(options), shell=True)
        
