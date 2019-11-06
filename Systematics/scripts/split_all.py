from os import listdir,popen
from sys import argv
stage1 = False
inclusive = False
if len(argv) > 1:
    if "tage1" in str(argv[1]): stage1 = True
    elif "ncl" in str(argv[1]): inclusive = True

for fn in listdir("."):
    if fn.count("pythia8.root") or fn.count("Up.root") or fn.count("Down.root") or fn.count("UpPS.root") or fn.count("DownPS.root") or fn.count('amcatnlo.root') or fn.count('TuneCUETP8M1.root') or fn.count('CP5.root'):
        cmd = "python ../../scripts/sort_by_HTXS.py %s" % fn
        #cmd = "python $CMSSW_BASE/src/flashgg/Systematics/scripts/sort_by_HTXS.py %s" % fn
        #if stage1: cmd = "python ../../scripts/sort_by_HTXSstage1.py %s" % fn
        if stage1: cmd = "python $CMSSW_BASE/src/flashgg/Systematics/scripts/sort_by_HTXSstage1.py %s" % fn
        elif inclusive: cmd = "python $CMSSW_BASE/src/flashgg/Systematics/scripts/sort_by_HTXSinclusive.py %s" % fn
        print cmd
        result = popen(cmd).read()
        print result
