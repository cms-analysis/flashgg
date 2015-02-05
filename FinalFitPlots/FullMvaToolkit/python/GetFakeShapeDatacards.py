import os, sys, numpy
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-i","--inpath",dest="inpath")
parser.add_option("-o","--outpath",dest="outpath")
parser.add_option("-D","--shapeDatacards",action="store_true",dest="shapDat",default=False)
parser.add_option("-C","--runCombine",action="store_true",dest="runComb",default=False)
parser.add_option("-N","--normsToText",action="store_true",dest="normText",default=False)
parser.add_option("-m","--plotByMH",action="store_true",dest="plotByMH",default=False)
parser.add_option("-b","--plotByBin",action="store_true",dest="plotByBin",default=False)
parser.add_option("","--mhLow",dest="mhLow",type="float",default=110.)
parser.add_option("","--mhHigh",dest="mhHigh",type="float",default=150.)
parser.add_option("","--mhStep",dest="mhStep",type="float",default=0.5)
(options,args)=parser.parse_args()

outpath=options.outpath
files=[]
mL=options.mhLow
mH=options.mhHigh+options.mhStep
mS=options.mhStep
for m in numpy.arange(mL,mH,mS):
	files.append(options.inpath+"/mva-datacard_grad_%3.1f.txt"%m)

if not os.path.isdir(outpath):
  os.makedirs(outpath)

print files

if options.shapDat:
	for m in numpy.arange(mL,mH,mS):
		text = open("%s/mva-datacard_grad_%3.1f.txt"%(options.inpath,m))
		outtext = open("%s/mva-datacard_grad_%3.1f.txt"%(outpath,m),'w')
		for i, line in enumerate(text.readlines()):
			if i==6:
				outtext.write('shapes * * FAKE \n')
				outtext.write(line)
			else:
				outtext.write(line)
		text.close()
		outtext.close()

if options.runComb:
	for m in numpy.arange(mL,mH,mS):
		print '\n----- Runnning ML for S/B on mass %3.1f -----'%m
		os.system("combine "+outpath+"/mva-datacard_grad_%3.1f.txt -M MaxLikelihoodFit -D data_grad -m %3.1f --saveNorm"%(m,m))
		os.system("mv mlfit.root "+outpath+"/mlfit_%3.1f.root"%m)
	os.system("mv higgsCombine* "+outpath)

if options.normText:
	for m in numpy.arange(mL,mH,mS):
		os.system("python python/mlfitNormsToText.py %s/mlfit_%3.1f.root > %s/%3.1fout.txt"%(outpath,m,outpath,m))

if options.plotByMH:
	for m in numpy.arange(mL,mH,mS):
		os.system("python plotbymH.py %s FakeShapeResults.root %3.1f"%(outpath,m))

if options.plotByBin:
	for binM in [110,115,120,125,130,135,140,150]:
		os.system("python plotbybin.py %s FakeShapeResults.root %d"%(outpath,binM)) 
      
