from os import listdir,popen
from sys import argv

from optparse import OptionParser
parser = OptionParser()
parser.add_option('--doStage1', default=False, action='store_true', help='Split by stage 1.0 bins')
parser.add_option('--doStage1p1', default=False, action='store_true', help='Split by stage 1.1 bins')
parser.add_option('--doStage1p2', default=False, action='store_true', help='Split by stage 1.2 bins')
parser.add_option('--doInclusive', default=True, action='store_true', help='Do it the old style by stage 0 process')
parser.add_option('--doIntermediate', default=False, action='store_true', help='Split intermediate files (for when hadding does not work in one go')
(opts,args) = parser.parse_args()

for fn in listdir("."):
    if fn.count("pythia8.root") or fn.count("Up.root") or fn.count("Down.root") or fn.count("UpPS.root") or fn.count("DownPS.root") or fn.count('amcatnlo.root') or fn.count('TuneCUETP8M1.root') or fn.count('CP5.root'):
        cmd = "python ../../scripts/sort_by_HTXS.py %s" % fn
        #cmd = "python $CMSSW_BASE/src/flashgg/Systematics/scripts/sort_by_HTXS.py %s" % fn
        #if stage1: cmd = "python ../../scripts/sort_by_HTXSstage1.py %s" % fn
        if opts.doStage1p2: cmd = "python $CMSSW_BASE/src/flashgg/Systematics/scripts/sort_by_HTXSstage1p2.py %s" % fn
        elif opts.doStage1p1: cmd = "python $CMSSW_BASE/src/flashgg/Systematics/scripts/sort_by_HTXSstage1p1.py %s" % fn
        elif opts.doStage1: cmd = "python $CMSSW_BASE/src/flashgg/Systematics/scripts/sort_by_HTXSstage1.py %s" % fn
        elif opts.doInclusive: cmd = "python $CMSSW_BASE/src/flashgg/Systematics/scripts/sort_by_HTXSinclusive.py %s" % fn
        print cmd
        result = popen(cmd).read()
        print result
    elif opts.doIntermediate and fn.count("intermediate") and fn.count(".root"):
        if opts.doStage1p2: cmd = "python $CMSSW_BASE/src/flashgg/Systematics/scripts/sort_by_HTXSstage1p2.py %s" % fn
        elif opts.doStage1p1: cmd = "python $CMSSW_BASE/src/flashgg/Systematics/scripts/sort_by_HTXSstage1p1.py %s" % fn
        elif opts.doStage1: cmd = "python $CMSSW_BASE/src/flashgg/Systematics/scripts/sort_by_HTXSstage1.py %s" % fn
        elif opts.doInclusive: cmd = "python $CMSSW_BASE/src/flashgg/Systematics/scripts/sort_by_HTXSinclusive.py %s" % fn
        print cmd
        result = popen(cmd).read()
        print result
