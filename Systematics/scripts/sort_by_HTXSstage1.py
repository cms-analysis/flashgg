from ROOT import *

#import gc
#gc.set_debug(gc.DEBUG_LEAK)
#gc.disable()

stage1binname2num = {"UNKNOWN":0, 
                     # Gluon fusion
                     "GG2H_FWDH" : -1, "GG2H_VBFTOPO_JET3VETO" : 1, "GG2H_VBFTOPO_JET3" : 2, "GG2H_0J" : 3, 
                     "GG2H_1J_PTH_0_60" : 4, "GG2H_1J_PTH_60_120" : 5, "GG2H_1J_PTH_120_200" : 6, "GG2H_1J_PTH_GT200" : 7,
                     "GG2H_GE2J_PTH_0_60" : 8, "GG2H_GE2J_PTH_60_120" : 9, "GG2H_GE2J_PTH_120_200" : 10, "GG2H_GE2J_PTH_GT200" : 11, 
                     # "VBF" / QQ2HQQ
                     "QQ2HQQ_FWDH" : -2, "QQ2HQQ_VBFTOPO_JET3VETO" : 12, "QQ2HQQ_VBFTOPO_JET3" : 13, "QQ2HQQ_VH2JET" : 14, "QQ2HQQ_REST" : 15, "QQ2HQQ_PTJET1_GT200" : 16,
                     # qq -> WH
                     "QQ2HLNU_FWDH" : -3, "QQ2HLNU_PTV_0_150" : 17, "QQ2HLNU_PTV_150_250_0J" : 18, "QQ2HLNU_PTV_150_250_GE1J" : 19, "QQ2HLNU_PTV_GT250" : 20,
                     # qq -> ZH
                     "QQ2HLL_FWDH" : -4, "QQ2HLL_PTV_0_150" : 21, "QQ2HLL_PTV_150_250_0J" : 22, "QQ2HLL_PTV_150_250_GE1J" : 23, "QQ2HLL_PTV_GT250" : 24,
                     # gg -> ZH
                     "GG2HLL_FWDH" : -5, "GG2HLL_PTV_0_150" : 25, "GG2HLL_PTV_GT150_0J" : 26, "GG2HLL_PTV_GT150_GE1J" : 27,
                     # ttH
                     "TTH_FWDH" : -6, "TTH" : 28,
                     # bbH
                     "BBH_FWDH" : -7, "BBH" : 29,
                     # tH
                     "TH_FWDH" : -8, "TH" : 30}
stage1binnum2name = {v: k for k, v in stage1binname2num.iteritems()}
relevantstage1bins = [cat for cat in stage1binname2num.values()]

from sys import argv
fn = "output.root"
if len(argv) > 1:
   fn = argv[1]
assert(fn.count(".root"))

_file = TFile(fn)
_ws = _file.Get("tagsDumper/cms_hgg_13TeV")
_data = _ws.allData()

files = {}
wss = {}

for cat in relevantstage1bins:
   wss[cat] = RooWorkspace("cms_hgg_13TeV","cms_hgg_13TeV")
   getattr(wss[cat],'import')(_ws.var("IntLumi"))

for ds in _data:
   initw = ds.sumEntries()
   sumfinw = 0.
   print "STARTING DATASET:",ds.GetName(),"weight:",ds.sumEntries()
   remainingArgList = RooArgSet(ds.get())
   anarg = ds.get().find("stage1bin")
   if not anarg:
      print "CANNOT SPLIT",ds.GetName(),"BECAUSE IT HAS NO stage1bin"
      continue
   remainingArgList.remove(anarg)
   for cat in relevantstage1bins:
      newds = ds.reduce(remainingArgList,"stage1bin==%i" % cat)
      getattr(wss[cat],'import')(newds)
      print "  ENDING DATASET for category %i (%s):"%(cat,stage1binnum2name[cat]),newds.GetName(),"weight:",newds.sumEntries()
      sumfinw += newds.sumEntries()
   if (initw == 0. and abs(sumfinw) > 0.0001) or (initw > 0. and abs((initw - sumfinw)/initw) > 0.001):
      if len(relevantstage1bins)==1:
         # debugging/testing
         print " DISAGREEMENT IN DATASET SUMWEIGHTS BEFORE AND AFTER:",initw,sumfinw
      else:
         raise Exception," DISAGREEMENT IN DATASET SUMWEIGHTS BEFORE AND AFTER: %.4f %.4f"%(initw,sumfinw)
         #pass

for cat in relevantstage1bins:
#   print cat
#   wss[cat].Print()
   newfn = fn.replace(".root","_%s.root" % stage1binnum2name[cat])
   newf = TFile(newfn,"RECREATE")
   newf.mkdir("tagsDumper")
   newf.cd("tagsDumper")
   wss[cat].Write()
   newf.Close()
#   print "end of",cat
   del(wss[cat]) # must happen before python does automatic garbage collection of datasets
#   print "deleted workspace"

#print "end of script"
