from optparse import OptionParser
parser = OptionParser()
parser.add_option("-e","--efficiencyDatFile",default="globeEfficiencies.dat",help="Datfile containing efficiency numbers. \nDefault: %default")
parser.add_option("-v","--vertexFile",default="vtxIdScaleFactorFromZmumu.root",help="Root file containing vertex efficiency scale factor. \nDefault: %default")
parser.add_option("-n","--nominalVertexFile",default="vertex_efficiency.root",help="Root file containing Higgs MC nominal vertex efficiency. \nDefault: %default")
parser.add_option("-o","--outFile",default="../AnalysisScripts/aux/sig_reweighing.root",help="Output file to be read in by globe. \nDefault: %default")
parser.add_option("--nPhoCats",default=6,help="Number of photon categories for globe smearing. \nDefault: %default", type=int)
parser.add_option("--nDiphoCats",default=8,help="Number of diphoton categories for globe smearing. \nDefault: %default", type=int)
parser.add_option("--isCutBased",default=False,action="store_true")
parser.add_option("-d","--draw",default=False,action="store_true")
(options,arg) = parser.parse_args()

import ROOT as r
from ROOT import *
import sys
import os

datfile = open(options.efficiencyDatFile)
outfile = r.TFile(options.outFile,'RECREATE')

atRel=False
relevantLines=[]
catLabels=[]
effTypes=[]

print options.nPhoCats
if options.nPhoCats!=4 and options.nPhoCats!=6:
        sys.exit('nPhoCats has to be 4 or 6 - sorry')
else:
        if options.nPhoCats==4:
                catLabels = ['EBHighR9','EBLowR9','EEHighR9','EELowR9']
        if options.nPhoCats==6:
                catLabels = ['EBHighR9','EBMidR9','EBLowR9','EEHighR9','EEMidR9','EELowR9']

for line in datfile.readlines():
        if line.startswith('#') or line.startswith('\n'): continue
        line = line.strip('\n')
        if line.startswith('effTypes'):
                effTypes = line.split('=')[1].split(',')
                print effTypes
        if line.startswith('analType'): 
                analType = line.split('=')[1]
                if options.isCutBased and analType=='cutbased':
                        atRel=True
                elif not options.isCutBased and analType=='mva':
                        atRel=True
                else:
                        atRel=False
        if atRel:
                relevantLines.append(line)

passedNCats=0
effVals=[]
effErrVals=[]

for line in relevantLines:
        print line
        if line.startswith('cat'): 
                passedNCats+=1
                thisEff={}
                thisEffErr={}
                lineElems = line.split()
                catType = lineElems[0]
                lineElems = lineElems[1:]
                vals=[]
                for i in range(0,len(lineElems),2):
                        effType = effTypes[i/2]
                        thisEff[effType] = float(lineElems[i])
                        thisEffErr[effType] = float(lineElems[i+1])
                effVals.append(thisEff)
                effErrVals.append(thisEffErr)

print 'Found', passedNCats, 'categories'
if passedNCats!=4:
        sys.exit('Only implemented for passing 4 categories - sorry.')
print 'Stored values in dictionary:'
for c in range(passedNCats):
        print '\t', c, 'eff: ', effVals[c]
        print '\t', c, 'err: ', effErrVals[c]
        if len(effVals)!=len(effErrVals):
                sys.exit('Different number of efficiencies in different categories - not cool')

# ---- id efficiency ----
for cat in range(options.nPhoCats):
        catName = catLabels[cat] 
        toteff = 1.
        toteffErrSq = 0.
        passedCat=-1
        for type in effTypes:
                if options.nPhoCats==4:
                        passedCat=cat
                elif options.nPhoCats==6:
                        if type=='presel': # cats are defined as highR9>0.9 so have to transfer
                                if cat%3==2: passedCat = cat/3*2+1 # move 2->1, 5->3
                                else: passedCat = cat/3*2        # move 0,1->0 3,4->2
                        else: # cats are defined as highR9>0.94
                                if cat%3==0: passedCat = cat/3*2 # move 0->0, 3->2
                                else: passedCat = cat/3*2+1 # move 1,2->1, 4,5->3
                else:
                        sys.exit('I dont know what to do if nPhoCats is not 4 or 6 - sorry')

                thisEff = effVals[passedCat][type]
                thisErr = effErrVals[passedCat][type]

                toteff *= thisEff
                toteffErrSq += ((thisErr/thisEff)**2)
        toterr = toteff*(toteffErrSq**0.5)

        print catName, '%5.3f +/- %5.3f'%(toteff,toterr)

        grEff = r.TGraphAsymmErrors()
        grEff.SetPoint(0,0.,toteff)
        grEff.SetPoint(1,10000.,toteff)
        grEff.SetPointError(0,0.,0.,toterr,toterr)
        grEff.SetPointError(1,0.,0.,toterr,toterr)
        grEff.SetName('ratioTP_%s'%catName)
        grEff.SetTitle('ratioTP_%s'%catName)
        grEff.SetMarkerColor(4)
        grEff.SetMarkerStyle(21)
        if options.draw: 
                grEff.Draw("ALP")
                raw_input('Ok?')
        grEff.Write()

for line in relevantLines:
        # ---- trigger efficiency ----
        if line.startswith('effL1HLT'): 
                effL1HLT = float(line.split('=')[1].split(',')[0])
                effL1HLTErr = float(line.split('=')[1].split(',')[1])
                for cat in range(options.nDiphoCats):
                        grL1HLT = r.TGraphAsymmErrors()
                        grL1HLT.SetPoint(0,0,effL1HLT)
                        grL1HLT.SetPointError(0,0.0,0.0,effL1HLTErr,effL1HLTErr)
                        grL1HLT.SetPoint(1,10000,effL1HLT)
                        grL1HLT.SetPointError(1,0.0,0.0,effL1HLTErr,effL1HLTErr)
                        grL1HLT.SetName('effL1HLT_cat%d'%cat)
                        grL1HLT.SetTitle('effL1HLT_cat%d'%cat)
                        grL1HLT.SetMarkerColor(4)
                        grL1HLT.SetMarkerStyle(21)
                        if options.draw: 
                                grL1HLT.Draw("ALP")
                                raw_input('Ok?')
                        grL1HLT.Write()
        # ---- r9 efficiency ----
        if line.startswith('effHighR9'):
                vals = line.split('=')[1].split(',')
                if len(vals)!=6:
                        sys.exit('For R9 eff need: highR9EB eff, highR9EB scale factor, highR9EB scale factor error, highR9EE eff, highR9EE scale factor, highR9EE scale factor error')
			#		for cat in ['EBHighR9','EBLowR9','EEHighR9','EELowR9']:
			#                        eff=-1.
			#                        err=-1.
			#                        if 'EB' in cat:
			#                                eff = float(vals[0])
			#                                err = float(vals[1])
			#                        elif 'EE' in cat:
			#                                eff = float(vals[2])
			#                                err = float(vals[3])
			#                        else:
			#                                sys.exit('I dont know what to do here')
			#                        
			#                        if 'LowR9' in cat:
			#                                err = 1.-( 1. - eff*(1.+err)/(1.-eff))
			#                                eff = 1.-eff
		for cat in ['EBHighR9','EBLowR9','EEHighR9','EELowR9']:
			if 'EB' in cat:
				eff   = float(vals[0])
                                sf    = float(vals[1])
                                sferr = float(vals[2])
			elif 'EE' in cat:
				eff   = float(vals[3])
                                sf    = float(vals[4])
                                sferr = float(vals[5])
                        else:
                                sys.exit('I dont know what to do here')
                        
                        if 'LowR9' in cat:
				sfl = (1. - sf*eff)/(1.-eff) 
                                sflerr = sferr*eff/(1.-eff)
				sf = sfl
				sferr = sflerr
				
                        grR9 = r.TGraphAsymmErrors()
                        grR9.SetPoint(0,0.,sf)
                        grR9.SetPoint(1,10000.,sf)
                        grR9.SetPointError(0,0.,0.,sferr,sferr)
                        grR9.SetPointError(1,0.,0.,sferr,sferr)
                        grR9.SetName('ratioR9_%s'%cat)
                        grR9.SetTitle('ratioR9_%s'%cat)
                        grR9.SetMarkerColor(4)
                        grR9.SetMarkerStyle(21)
                        if options.draw:
                                grR9.Draw("ALP")
                                raw_input('Ok?')
                        grR9.Write();

# ---- vertex efficiency ----
nominalVtxEffFile     = r.TFile(options.nominalVertexFile)
scaleFactorVtxEffFile = r.TFile(options.vertexFile)

eff_ratio = scaleFactorVtxEffFile.Get("scaleFactor")
mc_eff = nominalVtxEffFile.Get("efficiency")

pass_rewei = mc_eff.Clone("pass_rewei")
pass_rewei.SetTitle("Wrong vertex reweighing; p_{T}(#gamma #gamma); Weight")
fail_rewei = mc_eff.Clone("fail_rewei")
fail_rewei.SetTitle("Wrong vertex reweighing; p_{T}(#gamma #gamma); Weight")


lfwe = 0.
for i in range(mc_eff.GetN()):
	if mc_eff.GetX()[i] != eff_ratio.GetX()[i]:
		print "Efficiency and ratio have different binning %d %f %f" % ( i, mc_eff.GetX()[i] , eff_ratio.GetX()[i] )
		sys.exit(1)
		
	pw  = eff_ratio.GetY()[i]
	pwe = eff_ratio.GetErrorY(i)

	x  = mc_eff.GetX()[i]
	xe = mc_eff.GetErrorX(i)
		
	eff  = mc_eff.GetY()[i]
	### effe = mc_eff.GetErrorY(i)      

	if( x < 40 ):
		effe = 0.005
	else:
		effe = 0.001
		## if eff < 0.95 and eff*pw <0.95:
	if eff < 0.98:
		fw  = (1. - eff*pw)  / (1. - eff)
		fwe = sqrt( (1.-fw)*(1.-fw)*pwe*pwe + fw*fw/((1-eff)*(1.-eff))*effe*effe )
		lfwe = fwe
	else:
		fw  = 1.
		try:
			fwe = sqrt( (1.-fw)*(1.-fw)*pwe*pwe + fw*fw/((1-eff)*(1.-eff))*effe*effe )
		except:
			fwe = sqrt( (1.-fw)*(1.-fw)*pwe*pwe + fw*fw/(0.02*0.02)*effe*effe )
			## fwe = lfwe                                                               

	pass_rewei.SetPoint(i,x,pw)
	pass_rewei.SetPointError(i,xe,xe,pwe,pwe)
	fail_rewei.SetPoint(i,x,fw)
	fail_rewei.SetPointError(i,xe,xe,fwe,fwe)
	print "x = %1.1f pass_w = %1.2f +- %1.2f fail_w = %1.2f +- %1.2f " % ( x, pw, pwe, fw, fwe )

for cat in range(options.nDiphoCats):
	outfile.cd()
	pass_rewei.Clone("ratioVertex_cat%d_pass" %cat).Write()
	fail_rewei.Clone("ratioVertex_cat%d_fail" %cat).Write()
	if options.draw:
		pass_rewei.Draw("ALP")
		fail_rewei.Draw("PLSAME")
		raw_input('Ok?')
		
outfile.Close()
