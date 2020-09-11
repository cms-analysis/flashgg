import ROOT as r
r.gROOT.SetBatch(True)

#import gc
#gc.set_debug(gc.DEBUG_LEAK)
#gc.disable()

stage1p2binname2num = { 'UNKNOWN' : 0,
                        'GG2H_FWDH' : -1, 
                        'GG2H_PTH_200_300' : 1, 
                        'GG2H_PTH_300_450' : 2, 
                        'GG2H_PTH_450_650' : 3, 
                        'GG2H_PTH_GT650' : 4, 
                        'GG2H_0J_PTH_0_10' : 5, 
                        'GG2H_0J_PTH_GT10' : 6, 
                        'GG2H_1J_PTH_0_60' : 7, 
                        'GG2H_1J_PTH_60_120' : 8, 
                        'GG2H_1J_PTH_120_200' : 9, 
                        'GG2H_GE2J_MJJ_0_350_PTH_0_60' : 10, 
                        'GG2H_GE2J_MJJ_0_350_PTH_60_120' : 11, 
                        'GG2H_GE2J_MJJ_0_350_PTH_120_200' : 12, 
                        'GG2H_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25' : 13, 
                        'GG2H_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_GT25' : 14, 
                        'GG2H_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25' : 15, 
                        'GG2H_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_GT25' : 16, 
                        'QQ2HQQ_FWDH' : -2, 
                        'QQ2HQQ_0J' : 17, 
                        'QQ2HQQ_1J' : 18, 
                        'QQ2HQQ_GE2J_MJJ_0_60' : 19, 
                        'QQ2HQQ_GE2J_MJJ_60_120' : 20, 
                        'QQ2HQQ_GE2J_MJJ_120_350' : 21, 
                        'QQ2HQQ_GE2J_MJJ_GT350_PTH_GT200' : 22, 
                        'QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25' : 23, 
                        'QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_GT25' : 24, 
                        'QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25' : 25, 
                        'QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_GT25' : 26, 
                        'QQ2HLNU_FWDH' : -3, 
                        'QQ2HLNU_PTV_0_75' : 27, 
                        'QQ2HLNU_PTV_75_150' : 28, 
                        'QQ2HLNU_PTV_150_250_0J' : 29, 
                        'QQ2HLNU_PTV_150_250_GE1J' : 30, 
                        'QQ2HLNU_PTV_GT250' : 31, 
                        'QQ2HLL_FWDH' : -4, 
                        'QQ2HLL_PTV_0_75' : 32, 
                        'QQ2HLL_PTV_75_150' : 33, 
                        'QQ2HLL_PTV_150_250_0J' : 34, 
                        'QQ2HLL_PTV_150_250_GE1J' : 35, 
                        'QQ2HLL_PTV_GT250' : 36, 
                        'GG2HLL_FWDH' : -5, 
                        'GG2HLL_PTV_0_75' : 37, 
                        'GG2HLL_PTV_75_150' : 38, 
                        'GG2HLL_PTV_150_250_0J' : 39, 
                        'GG2HLL_PTV_150_250_GE1J' : 40, 
                        'GG2HLL_PTV_GT250' : 41, 
                        'TTH_FWDH' : -6, 
                        'TTH_PTH_0_60' : 42, 
                        'TTH_PTH_60_120' : 43, 
                        'TTH_PTH_120_200' : 44, 
                        'TTH_PTH_200_300' : 45, 
                        'TTH_PTH_GT300' : 46, 
                        'BBH_FWDH' : -7, 
                        'BBH' : 47, 
                        'TH_FWDH' : -8, 
                        'TH' : 48
}

stage1p2binnum2name = {v: k for k, v in stage1p2binname2num.iteritems()}
relevantstage1p2bins = [cat for cat in stage1p2binname2num.values()]

from sys import argv
fn = "output.root"
if len(argv) > 1:
   fn = argv[1]
assert(fn.count(".root"))

_file = r.TFile(fn)
_ws = _file.Get("tagsDumper/cms_hgg_13TeV")
_data = _ws.allData()

files = {}
wss = {}

for cat in relevantstage1p2bins:
   wss[cat] = r.RooWorkspace("cms_hgg_13TeV","cms_hgg_13TeV")
   getattr(wss[cat],'import')(_ws.var("IntLumi"))

for ds in _data:
   initw = ds.sumEntries()
   sumfinw = 0.
   print "STARTING DATASET:",ds.GetName(),"weight:",ds.sumEntries()
   remainingArgList = r.RooArgSet(ds.get())
   anarg = ds.get().find("stage1p2bin")
   if not anarg:
      print "CANNOT SPLIT",ds.GetName(),"BECAUSE IT HAS NO stage1p2bin"
      continue
   remainingArgList.remove(anarg)
   for cat in relevantstage1p2bins:
      newds = ds.reduce(remainingArgList,"stage1p2bin==%i" % cat)
      getattr(wss[cat],'import')(newds)
      print "  ENDING DATASET for category %i (%s):"%(cat,stage1p2binnum2name[cat]),newds.GetName(),"weight:",newds.sumEntries()
      sumfinw += newds.sumEntries()
   if (initw == 0. and abs(sumfinw) > 0.0001) or (initw > 0. and abs((initw - sumfinw)/initw) > 0.001):
      if len(relevantstage1p2bins)==1:
         # debugging/testing
         print " DISAGREEMENT IN DATASET SUMWEIGHTS BEFORE AND AFTER:",initw,sumfinw
      else:
         raise Exception," DISAGREEMENT IN DATASET SUMWEIGHTS BEFORE AND AFTER: %.4f %.4f"%(initw,sumfinw)
         #pass

for cat in relevantstage1p2bins:
#   print cat
#   wss[cat].Print()
   newfn = fn.replace(".root","_%s.root" % stage1p2binnum2name[cat])
   newf = r.TFile(newfn,"RECREATE")
   newf.mkdir("tagsDumper")
   newf.cd("tagsDumper")
   wss[cat].Write()
   newf.Close()
#   print "end of",cat
   del(wss[cat]) # must happen before python does automatic garbage collection of datasets
#   print "deleted workspace"

#print "end of script"
