from ROOT import *

#import gc
#gc.set_debug(gc.DEBUG_LEAK)
#gc.disable()

stage0catname2num = {"UNKNOWN":0, "GG2H_FWDH":10, "GG2H":11, "VBF_FWDH":20, "VBF":21, "VH2HQQ_FWDH":22, "VH2HQQ":23,
                     "QQ2HLNU_FWDH":30, "QQ2HLNU":31, "QQ2HLL_FWDH":40, "QQ2HLL":41, "GG2HLL_FWDH":50, "GG2HLL":51,
                     "TTH_FWDH":60, "TTH":61, "BBH_FWDH":70, "BBH":71, "TH_FWDH":80, "TH":81 }
stage0catnum2name = {v: k for k, v in stage0catname2num.iteritems()}
relevantstage0cats = [cat for cat in stage0catname2num.values() if cat > 0 and (cat <= 49 or cat == 60 or cat == 61)]
#relevantstage0cats = [40] # for debugging
#print relevantstage0cats

from sys import argv
fn = "output.root"
if len(argv) > 1:
   fn = argv[1]
assert(fn.count(".root"))

_file = TFile(fn)
_ws = _file.Get("tagsDumper/cms_hgg_13TeV")
_data = _ws.allData()
#print _data

files = {}
wss = {}

for cat in relevantstage0cats:
   wss[cat] = RooWorkspace("cms_hgg_13TeV","cms_hgg_13TeV")
   getattr(wss[cat],'import')(_ws.var("IntLumi"))

for ds in _data:
   initw = ds.sumEntries()
   sumfinw = 0.
   print "STARTING DATASET:",ds.GetName(),"weight:",ds.sumEntries()
   remainingArgList = RooArgSet(ds.get())
   anarg = ds.get().find("stage0cat")
   if not anarg:
      print "CANNOT SPLIT",ds.GetName(),"BECAUSE IT HAS NO stage0cat"
      continue
   remainingArgList.remove(anarg)
   for cat in relevantstage0cats:
      newds = ds.reduce(remainingArgList,"stage0cat==%i" % cat)
      getattr(wss[cat],'import')(newds)
      print "  ENDING DATASET for category %i (%s):"%(cat,stage0catnum2name[cat]),newds.GetName(),"weight:",newds.sumEntries()
      sumfinw += newds.sumEntries()
   if (initw == 0. and abs(sumfinw) > 0.0001) or (initw > 0. and abs((initw - sumfinw)/initw) > 0.001):
      if len(relevantstage0cats)==1:
         # debugging/testing
         print " DISAGREEMENT IN DATASET SUMWEIGHTS BEFORE AND AFTER:",initw,sumfinw
      else:
         if fn.count("VBF"):
            print "DISAGREEMENT but moving on because VBF is wonky"
         else:
            raise Exception," DISAGREEMENT IN DATASET SUMWEIGHTS BEFORE AND AFTER: %.4f %.4f"%(initw,sumfinw)

for cat in relevantstage0cats:
#   print cat
#   wss[cat].Print()
   newfn = fn.replace(".root","_%s.root" % stage0catnum2name[cat])
   newf = TFile(newfn,"RECREATE")
   newf.mkdir("tagsDumper")
   newf.cd("tagsDumper")
   wss[cat].Write()
   newf.Close()
#   print "end of",cat
   del(wss[cat]) # must happen before python does automatic garbage collection of datasets
#   print "deleted workspace"

#print "end of script"
