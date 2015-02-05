#!/usr/bin/env python

# This script was synchronized with the Hgg Legacy Systematic settings from the page below on 21st Dec 13
# https://twiki.cern.ch/twiki/bin/view/CMS/HggLegacySystematicUncertainties
# Matt Kenzie

import os,sys,copy

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i","--infilename", help="Input file (binned signal from globe)")
parser.add_option("-o","--outfilename",default="cms_hgg_datacard.txt",help="Name of card to print (default: %default)")
parser.add_option("-p","--procs",default="ggh,vbf,wh,zh,tth",help="String list of procs (default: %default)")
parser.add_option("-c","--ncats",default=9,type="int",help="Number of cats (default: %default)")
parser.add_option("--photonCatScales",default="EBlowR9,EBhighR9,EElowR9,EEhighR9",help="String list of photon scale nuisance names - WILL NOT correlate across years (default: %default)")
parser.add_option("--photonCatScalesCorr",default="MaterialEBCentral,MaterialEBOuterEE,LightColl",help="String list of photon scale nuisance names - WILL correlate across years (default: %default)")
parser.add_option("--photonCatSmears",default="EBlowR9,EBhighR9,EBlowR9Phi,EBhighR9Phi,EElowR9,EEhighR9",help="String list of photon smearing nuisance names - WILL NOT correlate across years (default: %default)")
parser.add_option("--photonCatSmearsCorr",default="",help="String list of photon smearing nuisance names - WILL correlate across years (default: %default)")
parser.add_option("--globalScales",default="NonLinearity:0.001",help="String list of global scale nuisances names with value separated by a \':\' - WILL NOT correlate across years (default: %default)")
parser.add_option("--globalScalesCorr",default="Geant4:0.0005",help="String list of global scale nuisances names with value separated by a \':\' - WILL correlate across years (default: %default)")
parser.add_option("--toSkip",default="",help="proc:cat which are to skipped e.g ggH:11,qqH:12 etc. (default: %default)")
parser.add_option("--isCutBased",default=False,action="store_true")
parser.add_option("--isSpinModel",default=False,action="store_true")
parser.add_option("--isMultiPdf",default=False,action="store_true")
parser.add_option("--isBinnedSignal",default=False,action="store_true")
parser.add_option("--is2011",default=False,action="store_true")
parser.add_option("--scaleFactors",help="Scale factor for spin model pass as e.g. gg_grav:1.351,qq_grav:1.027")
parser.add_option("--quadInterpolate",type="int",default=0,help="Do a quadratic interpolation of globe templates back to 1 sigma from this sigma. 0 means off (default: %default)")
(options,args)=parser.parse_args()

import ROOT as r
if options.quadInterpolate:
	r.gROOT.ProcessLine(".L quadInterpolate.C+g")
	from ROOT import quadInterpolate
r.gROOT.ProcessLine(".L $CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so")
r.gROOT.ProcessLine(".L ../libLoopAll.so")

# convert globe style to combine style process
combProc = {'ggh':'ggH','vbf':'qqH','wzh':'VH','wh':'WH','zh':'ZH','tth':'ttH','bkg_mass':'bkg_mass','gg_grav':'ggH_ALT','qq_grav':'qqbarH_ALT'}
globeProc = {'ggH':'ggh','qqH':'vbf','VH':'wzh','WH':'wh','ZH':'zh','ttH':'tth','bkg_mass':'bkg_mass','ggH_ALT':'gg_grav','qqbarH_ALT':'qq_grav'}
procId = {'ggH':0,'qqH':-1,'VH':-2,'WH':-2,'ZH':-3,'ttH':-4,'ggH_ALT':-5,'qqbarH_ALT':-6,'bkg_mass':1}

# setup
if options.is2011: sqrts=7
else: sqrts=8
if 'wzh'in options.procs.split(','):
	splitVH=False
if 'wh' in options.procs.split(',') and 'zh' in options.procs.split(','):
	splitVH=True
inFile = r.TFile.Open(options.infilename)
outFile = open(options.outfilename,'w')
bkgProcs = ['bkg_mass']
spin2Procs = ['ggH_ALT','qqbarH_ALT']
vbfProcs = ['qqH']
# FOR MVA:
if options.is2011:
	incCats = [0,1,2,3]
	dijetCats = [4,5]
	muonCat = [6,7]
	eleCat = [6,7]
	tightLepCat = [6]
	looseLepCat = [7]
	metCat = [8]
	tthLepCat = [9]
	tthCats = [9]
	vhHadCat = [10]
else:
	incCats = [0,1,2,3,4]
	dijetCats = [5,6,7]
	muonCat = [8,9]
	eleCat = [8,9]
	tightLepCat = [8]
	looseLepCat = [9]
	metCat = [10]
	tthLepCat = [11]
	tthHadCat = [12]
	tthCats = [11,12]
	vhHadCat = [13]
# FOR CIC:
if options.isCutBased:
	incCats = [0,1,2,3,4,5,6,7]
	dijetCats = [8,9]
	muonCat = [10,11]
	eleCat = [10,11]
	tightLepCat = [10]
	looseLepCat = [11]
	metCat = [12]
	if options.is2011:
		tthLepCat = [13]
		tthCats = [13]
		vhHadCat = [14]
	else:
		tthLepCat = [13]
		tthHadCat = [14]
		tthCats = [13,14]
		vhHadCat = [15]
options.procs += ',bkg_mass'
options.procs = [combProc[p] for p in options.procs.split(',')]
options.toSkip = options.toSkip.split(',')

if options.photonCatScales=='': options.photonCatScales = []
else: options.photonCatScales = options.photonCatScales.split(',')
if options.photonCatScalesCorr=='': options.photonCatScalesCorr = []
else: options.photonCatScalesCorr = options.photonCatScalesCorr.split(',')
if options.photonCatSmears=='': options.photonCatSmears = []
else: options.photonCatSmears = options.photonCatSmears.split(',')
if options.photonCatSmearsCorr=='': options.photonCatSmearsCorr = []
else: options.photonCatSmearsCorr = options.photonCatSmearsCorr.split(',')
if options.globalScales=='': options.globalScales = []
else: options.globalScales = options.globalScales.split(',')
if options.globalScalesCorr=='': options.globalScalesCorr = []
else: options.globalScalesCorr = options.globalScalesCorr.split(',')

inWS = inFile.Get('cms_hgg_workspace')
intL = inWS.var('IntLumi').getVal()

# info = [file,workspace,name]
file_ext = 'mva'
if options.isCutBased: file_ext = 'cic'
if options.isSpinModel: file_ext = 'spin'

if options.isMultiPdf:
	dataFile = 'CMS-HGG_%s_%dTeV_multipdf.root'%(file_ext,sqrts)
	bkgFile = 'CMS-HGG_%s_%dTeV_multipdf.root'%(file_ext,sqrts)
	dataWS = 'multipdf'
	bkgWS = 'multipdf'
else:
	dataFile = 'CMS-HGG_%s_%dTeV_multipdf.root'%(file_ext,sqrts)
	bkgFile = 'CMS-HGG_%s_%dTeV_data.root'%(file_ext,sqrts)
	dataWS = 'multipdf'
	bkgWS = 'cms_hgg_workspace'
if options.isBinnedSignal:
	sigFile = 'CMS-HGG_%s_%dTeV_sig_interpolated.root'%(file_ext,sqrts)
	sigWS = 'cms_hgg_workspace'
else:
	sigFile = 'CMS-HGG_%s_%dTeV_sigfit.root'%(file_ext,sqrts)
	sigWS = 'wsig_%dTeV'%(sqrts)

fileDetails = {}
fileDetails['data_obs'] = [dataFile,dataWS,'roohist_data_mass_$CHANNEL']
if options.isMultiPdf:
	fileDetails['bkg_mass']	= [bkgFile,bkgWS,'CMS_hgg_$CHANNEL_%dTeV_bkgshape'%sqrts]
else:
	fileDetails['bkg_mass']	= [bkgFile,bkgWS,'pdf_data_pol_model_%dTeV_$CHANNEL'%sqrts]

if options.isBinnedSignal:
	fileDetails['ggH'] 			= [sigFile,sigWS,'roohist_sig_ggh_mass_m$MASS_$CHANNEL']
	fileDetails['qqH'] 			= [sigFile,sigWS,'roohist_sig_vbf_mass_m$MASS_$CHANNEL']
	if splitVH:
		fileDetails['WH'] 			=	[sigFile,sigWS,'roohist_sig_wh_mass_m$MASS_$CHANNEL']
		fileDetails['ZH'] 			=	[sigFile,sigWS,'roohist_sig_zh_mass_m$MASS_$CHANNEL']
	else:
		fileDetails['VH'] 			=	[sigFile,sigWS,'roohist_sig_vh_mass_m$MASS_$CHANNEL']
	fileDetails['ttH'] 			= [sigFile,sigWS,'roohist_sig_tth_mass_m$MASS_$CHANNEL']
	if options.isSpinModel:
		fileDetails['ggH_ALT'] 			= [sigFile,sigWS,'roohist_sig_gg_grav_mass_m$MASS_$CHANNEL']
		fileDetails['qqbarH_ALT'] 			= [sigFile,sigWS,'roohist_sig_qq_grav_mass_m$MASS_$CHANNEL']
else:
	fileDetails['ggH'] 			= [sigFile,sigWS,'hggpdfsmrel_%dTeV_ggh_$CHANNEL'%sqrts]
	fileDetails['qqH'] 			= [sigFile,sigWS,'hggpdfsmrel_%dTeV_vbf_$CHANNEL'%sqrts]
	if splitVH:
		fileDetails['WH'] 			=	[sigFile,sigWS,'hggpdfsmrel_%dTeV_wh_$CHANNEL'%sqrts]
		fileDetails['ZH'] 			=	[sigFile,sigWS,'hggpdfsmrel_%dTeV_zh_$CHANNEL'%sqrts]
	else:
		fileDetails['VH'] 			=	[sigFile,sigWS,'hggpdfsmrel_%dTeV_wzh_$CHANNEL'%sqrts]
	fileDetails['ttH'] 			= [sigFile,sigWS,'hggpdfsmrel_%dTeV_tth_$CHANNEL'%sqrts]
	if options.isSpinModel:
		fileDetails['ggH_ALT'] 			= [sigFile,sigWS,'hggpdfsmrel_%dTeV_gg_grav_$CHANNEL'%sqrts]
		fileDetails['qqbarH_ALT'] 			= [sigFile,sigWS,'hggpdfsmrel_%dTeV_qq_grav_$CHANNEL'%sqrts]

# theory systematics arr=[up,down]
# these come in specific types (as must be correlated with combination)
# 	- see https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsWG/HiggsCombinationConventions
# QCDscale_ggH, QCDscale_qqH, QCDscale_VH, QCDscale_ttH
# pdf_gg, pdf_qqbar
theorySyst = {}
theorySyst['QCDscale_ggH'] = {}
theorySyst['QCDscale_qqH'] = {}
theorySyst['QCDscale_VH'] = {}
theorySyst['QCDscale_ttH'] = {}
theorySyst['pdf_gg'] = {}
theorySyst['pdf_qqbar'] = {}
# 7 TeV
if options.is2011:
	# scale
	theorySyst['QCDscale_ggH']['ggH'] = [0.071,-0.078] 
	theorySyst['QCDscale_qqH']['qqH'] = [0.003,-0.003]
	if splitVH:
		theorySyst['QCDscale_VH']['WH'] = [0.009,-0.009]
		theorySyst['QCDscale_VH']['ZH'] = [0.029,-0.029]
	else:
		theorySyst['QCDscale_VH']['VH'] = [0.030,-0.030]
	theorySyst['QCDscale_ttH']['ttH'] = [0.032,-0.093]
	# pdf
	theorySyst['pdf_gg']['ggH'] = [0.076,-0.071] 
	theorySyst['pdf_qqbar']['qqH'] = [0.025,-0.021]
	if splitVH:
		theorySyst['pdf_qqbar']['WH'] = [0.026,-0.026]
		theorySyst['pdf_qqbar']['ZH'] = [0.027,-0.027]
	else:
		theorySyst['pdf_qqbar']['VH'] = [0.037,-0.037]
	theorySyst['pdf_gg']['ttH'] = [0.084,-0.084]
# 8 TeV
else:
	# scale
	theorySyst['QCDscale_ggH']['ggH'] = [0.072,-0.078]
	theorySyst['QCDscale_qqH']['qqH'] = [0.002,-0.002]
	if splitVH:
		theorySyst['QCDscale_VH']['WH'] = [0.010,-0.010]
		theorySyst['QCDscale_VH']['ZH'] = [0.031,-0.031]
	else:
		theorySyst['QCDscale_VH']['VH'] = [0.031,-0.031] 
	theorySyst['QCDscale_ttH']['ttH'] = [0.038,-0.093]
	# pdf
	theorySyst['pdf_gg']['ggH'] = [0.075,-0.069] 
	theorySyst['pdf_qqbar']['qqH'] = [0.026,-0.028]
	if splitVH:
		theorySyst['pdf_qqbar']['WH'] = [0.023,-0.023]
		theorySyst['pdf_qqbar']['ZH'] = [0.025,-0.025]
	else:
		theorySyst['pdf_qqbar']['VH'] = [0.034,-0.034]
	theorySyst['pdf_gg']['ttH'] = [0.081,-0.081]

# BR uncertainty
brSyst = [0.050,-0.049]

# lumi syst
if options.is2011:
	lumiSyst = 0.022
else:
	lumiSyst = 0.026

# vtx eff
if options.is2011:
	vtxSyst = 0.015
else:
	vtxSyst = 0.015

# r9 syst (cut based only)
if options.isCutBased:
	if options.is2011:
		r9barrelSyst = 0.080 
		r9mixedSyst = 0.115
	else:
		r9barrelSyst = 0.040 
		r9mixedSyst = 0.065

trigEff = 0.01

# from globe
globeSystDump = open('globeSystDump.dat','w')
globeSysts={}
globeSysts['idEff'] = 'n_id_eff'
##globeSysts['triggerEff'] = 'n_trig_eff'
if not options.isCutBased:
  globeSysts['phoIdMva'] = 'n_id_mva'
  globeSysts['regSig'] = 'n_sigmae'
if options.isBinnedSignal:
	globeSysts['E_scale'] = 'n_e_scale'
	globeSysts['E_res'] = 'n_e_res'
if options.isSpinModel:
	globeSysts['ptSpin'] = 'n_pt'

# QCD scale and PDF variations on PT-Y (replaced k-Factor PT variation) 
if not options.isBinnedSignal:
	globeSysts['pdfWeight_QCDscale'] = 'n_sc'
	for pdfi in range(1,27):
		globeSysts['pdfWeight_pdfset%d'%pdfi] = 'n_pdf_%d'%pdfi

# vbf uncertainties - vbfSysts['name'] = [ggEffect,qqEffect] - append migration effects
# naming is important to correlate with combination

vbfSysts={}
vbfSysts['QCDscale_gg2in'] = [] # on the hgg twiki this is referred to JetVeto_QCDscale
vbfSysts['UEPS'] = []
vbfSysts['CMS_scale_j'] = [] # on the hgg twiki this is referred to as JEC
# pu jet eff = [ggEffect,qqEffect,WHeffect,ZHeffect,ttHeffect] - append for each vbf cat and for each VH hadronic cat
puJetIdEff = []
if options.isCutBased:
	# 7TeV
	if options.is2011:
		vbfSysts['QCDscale_gg2in'].append([0.25,0.]) # All VBF cats -> inclusive
		vbfSysts['QCDscale_gg2in'].append([0.05,0.]) # VBF tight -> VBF loose
		vbfSysts['UEPS'].append([0.01,0.01])
		vbfSysts['UEPS'].append([0.02,0.01])
		vbfSysts['CMS_scale_j'].append([0.11,0.03])
		vbfSysts['CMS_scale_j'].append([0.02,0.02])
		# no PU ID at 7TeV
	# 8TeV
	else:
		vbfSysts['QCDscale_gg2in'].append([0.25,0.]) # All VBF cats -> inclusive
		vbfSysts['QCDscale_gg2in'].append([0.05,0.]) # VBF tight -> VBF loose
		vbfSysts['UEPS'].append([0.01,0.01])
		vbfSysts['UEPS'].append([0.02,0.01])
		vbfSysts['CMS_scale_j'].append([0.11,0.03])
		vbfSysts['CMS_scale_j'].append([0.02,0.02])
		puJetIdEff.append([0.030,0.029,0.026,0.026,0.011])
		puJetIdEff.append([0.056,0.055,0.046,0.046,0.023])
		puJetIdEff.append([0.010,0.010,0.009,0.009,0.009])
else:
	# 7TeV
	if options.is2011:
		vbfSysts['QCDscale_gg2in'].append([0.30,0.]) # All VBF cats -> inclusive
		vbfSysts['QCDscale_gg2in'].append([0.14,0.]) # VBF tight -> VBF loose
		vbfSysts['UEPS'].append([0.03,0.01])
		vbfSysts['UEPS'].append([0.01,0.02])
		vbfSysts['CMS_scale_j'].append([0.10,0.04])
		vbfSysts['CMS_scale_j'].append([0.06,0.01])
		# no PU ID at 7TeV
	# 8TeV
	else:
		vbfSysts['QCDscale_gg2in'].append([0.30,0.]) # All VBF cats -> inclusive
		vbfSysts['QCDscale_gg2in'].append([0.14,0.]) # VBF cat5+cat6 -> VBF loose (cat7)
		vbfSysts['QCDscale_gg2in'].append([0.05,0.]) # VBF cat5 -> VBF cat 6
		vbfSysts['UEPS'].append([0.03,0.01])
		vbfSysts['UEPS'].append([0.01,0.02])
		vbfSysts['UEPS'].append([0.01,0.02])
		vbfSysts['CMS_scale_j'].append([0.10,0.04])
		vbfSysts['CMS_scale_j'].append([0.06,0.01])
		vbfSysts['CMS_scale_j'].append([0.05,0.01])
		puJetIdEff.append([0.029,0.029,0.023,0.023,0.009])
		puJetIdEff.append([0.031,0.035,0.024,0.024,0.010])
		puJetIdEff.append([0.040,0.040,0.023,0.023,0.009])
		puJetIdEff.append([0.010,0.010,0.009,0.009,0.009])
# check ok
for systName, systVal in vbfSysts.items():
	if not (len(systVal)==len(dijetCats)): sys.exit('Number of VBF categories not consistent with VBF syst values given')
if not options.is2011:
	if not (len(puJetIdEff)==len(dijetCats)+len(vhHadCat)): sys.exit('Number of VBF categories not consistent with VBF syst values given')

# lepton + MET systs
# [VH tight, VH loose, ttH leptonic]
eleSyst = {}
eleSyst['ggH'] = [0.,0.,0.] 
eleSyst['qqH'] = [0.,0.,0.]
eleSyst['WH'] = [0.0028,0.0024,0.] 
eleSyst['ZH'] = [0.0044,0.0025,0.]
eleSyst['ttH'] = [0.0026,0.,0.0022]
muonSyst = {}
muonSyst['ggH'] = [0.0,0.0,0.0]
muonSyst['qqH'] = [0.0,0.0,0.0]
muonSyst['WH'] = [0.0027,0.0034,0.]
muonSyst['ZH'] = [0.0054,0.0037,0.]
muonSyst['ttH'] = [0.0026,0.,0.0022]
metSyst = {}
metSyst['ggH'] = [0.,0.,0.04] 
metSyst['qqH'] = [0.,0.,0.04]
metSyst['WH'] = [0.012,0.019,0.026] 
metSyst['ZH'] = [0.009,0.015,0.021]
metSyst['ttH'] = [0.011,0.012,0.040]

# syst for tth tags - [ttHlep,tthHad]
btagSyst={}
btagSyst['ggH'] = [0.,0.02]
btagSyst['qqH'] = [0.,0.]
btagSyst['WH'] = [0.,0.]
btagSyst['ZH'] = [0.,0.]
btagSyst['ttH'] = [0.01,0.01]
# spec for ggh in tth cats - [MC_low_stat,gluon_splitting,parton_shower]
ggHforttHSysts = {}
ggHforttHSysts['CMS_hgg_tth_mc_low_stat'] = 0.25
ggHforttHSysts['CMS_hgg_tth_gluon_splitting'] = 0.13
ggHforttHSysts['CMS_hgg_tth_parton_shower'] = 0.30

# rate adjustments
looseLepRateScale = 0.9909
tightLepRateScale = 0.9886
tthLepRateScale = 0.980
tthHadRateScale = 0.995

# for spin
scaleFactors = {}
if options.scaleFactors and options.isSpinModel:
	for fac in options.scaleFactors.split(','):
		scaleFactors[fac.split(':')[0]] = float(fac.split(':')[1])

def interp1Sigma(th1f_nom,th1f_down,th1f_up):
	nomE = th1f_nom.Integral()
	if nomE==0:
		return [1.000,1.000]
	downE = th1f_down.Integral()/nomE
	upE = th1f_up.Integral()/nomE
	if options.quadInterpolate!=0:
		downE = quadInterpolate(-1.,-1.*options.quadInterpolate,0.,1.*options.quadInterpolate,th1f_down.Integral(),th1f_nom.Integral(),th1f_up.Integral())
		upE = quadInterpolate(1.,-1.*options.quadInterpolate,0.,1.*options.quadInterpolate,th1f_down.Integral(),th1f_nom.Integral(),th1f_up.Integral())
		if upE != upE: upE=1.000
		if downE != downE: downE=1.000
	return [downE,upE]

def printPreamble():
	print 'Preamble...'
	if options.isCutBased:
		outFile.write('CMS-HGG datacard for parametric model - cut based analysis %dTeV \n'%sqrts)
	else:
		outFile.write('CMS-HGG datacard for parametric model - mass factorized analysis %dTeV \n'%sqrts)
	outFile.write('Auto-generated by h2gglobe/Macros/makeParametricModelDatacard.py\n')
	outFile.write('Run with: combine\n')
	outFile.write('---------------------------------------------\n')
	outFile.write('imax *\n')
	outFile.write('jmax *\n')
	outFile.write('kmax *\n')
	outFile.write('---------------------------------------------\n')
	outFile.write('\n')

def printFileOptions():
	print 'File opts...'
	for typ, info in fileDetails.items():
		for c in range(options.ncats):
			file = info[0]
			wsname = info[1]
			pdfname = info[2].replace('$CHANNEL','cat%d'%c)
			if typ not in options.procs and typ!='data_obs': continue
			if options.isBinnedSignal:
				outFile.write('shapes %-10s %-15s %-30s %-30s %-30s_$SYSTEMATIC01_sigma\n'%(typ,'cat%d_%dTeV'%(c,sqrts),file,wsname+':'+pdfname,wsname+':'+pdfname))
			else:
				outFile.write('shapes %-10s %-15s %-30s %-30s\n'%(typ,'cat%d_%dTeV'%(c,sqrts),file,wsname+':'+pdfname))
	outFile.write('\n')

def printObsProcBinLines():
	print 'Rates...'
	outFile.write('%-15s '%'bin')
	for c in range(options.ncats):
		outFile.write('cat%d_%dTeV '%(c,sqrts))
	outFile.write('\n')
	
	outFile.write('%-15s '%'observation')
	for c in range(options.ncats):
		outFile.write('-1 ')
	outFile.write('\n')
	
	outFile.write('%-15s '%'bin')
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			outFile.write('cat%d_%dTeV '%(c,sqrts))
	outFile.write('\n')
	
	outFile.write('%-15s '%'process')
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			outFile.write('%s '%p)
	outFile.write('\n')

	outFile.write('%-15s '%'process')
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			outFile.write('%d '%procId[p])
	outFile.write('\n')

	outFile.write('%-15s '%'rate')
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs:
				outFile.write('1.0 ')
			else:
				if options.isBinnedSignal:
					outFile.write('-1 ')
				else:
					scale=1.
					if c in looseLepCat: scale *= looseLepRateScale
					if c in tightLepCat: scale *= tightLepRateScale
					if c in tthCats:
						if c in tthLepCat: scale *= tthLepRateScale
						else: scale *= tthHadRateScale
					if options.isSpinModel: 
						scale=1.
						if globeProc[p] in scaleFactors.keys():
							scale = scaleFactors[globeProc[p]]
					outFile.write('%7.1f '%(intL*scale))
	outFile.write('\n')
	outFile.write('\n')

def printNuisParam(name,typ,sqrtS=None):
	val="1.0"
	if ":" in name:
		name,val = name.split(":")
	if sqrtS:
		typ="%dTeV%s" % (sqrtS, typ)
	outFile.write('%-40s param 0.0 %s\n'%('CMS_hgg_nuisance_%s_%s'%(name,typ),val))

def printNuisParams():
	if not options.isBinnedSignal:
		print 'Nuisances...'
		## outFile.write('%-40s param 0.0 %1.4g\n'%('CMS_hgg_nuisance_%dTeVdeltafracright'%sqrts,vtxSyst))
		outFile.write('%-40s param 0.0 %1.4g\n'%('CMS_hgg_nuisance_deltafracright',vtxSyst))
		if options.isCutBased:
			outFile.write('%-40s param 0.0 %1.4g\n'%('CMS_hgg_nuisance_%dTeVdeltar9barrel'%sqrts,r9barrelSyst))
			outFile.write('%-40s param 0.0 %1.4g\n'%('CMS_hgg_nuisance_%dTeVdeltar9mixed'%sqrts,r9mixedSyst))
		for phoSyst in options.photonCatScales:
			printNuisParam(phoSyst,"scale",sqrts)
			## outFile.write('%-40s param 0.0 1.0\n'%('CMS_hgg_nuisance_%s_%dTeVscale'%(phoSyst,sqrts)))
		for phoSyst in options.photonCatScalesCorr:
			printNuisParam(phoSyst,"scale")
			## outFile.write('%-40s param 0.0 1.0\n'%('CMS_hgg_nuisance_%s_scale'%(phoSyst)))
		for phoSyst in options.globalScales:
			printNuisParam(phoSyst,"scale",sqrts)			
			## outFile.write('%-40s param 0.0 %1.4g\n'%('CMS_hgg_nuisance_%s_%dTeVscale'%(phoSyst.split(':')[0],sqrts),float(phoSyst.split(':')[1])))
		for phoSyst in options.globalScalesCorr:
			printNuisParam(phoSyst,"scale")
			## outFile.write('%-40s param 0.0 %1.4g\n'%('CMS_hgg_nuisance_%s_scale'%(phoSyst.split(':')[0]),float(phoSyst.split(':')[1])))
		for phoSyst in options.photonCatSmears:
			printNuisParam(phoSyst,"smear",sqrts)
			## outFile.write('%-40s param 0.0 1.0\n'%('CMS_hgg_nuisance_%s_%dTeVsmear'%(phoSyst,sqrts)))
		for phoSyst in options.photonCatSmearsCorr:
			printNuisParam(phoSyst,"smear")
			## outFile.write('%-40s param 0.0 1.0\n'%('CMS_hgg_nuisance_%s_smear'%(phoSyst)))
		outFile.write('\n')

def printTheorySysts():
	# as these are antisymmetric lnN systematics - implement as [1/(1.+err_down)] for the lower and [1.+err_up] for the upper
	print 'Theory...'
	for systName, systDetails in theorySyst.items():
		outFile.write('%-35s   lnN   '%systName)
		for c in range(options.ncats):
			for p in options.procs:
				if '%s:%d'%(p,c) in options.toSkip: continue
				if p in systDetails.keys():
					outFile.write('%5.3f/%5.3f '%(1./(1.-systDetails[p][1]),1.+systDetails[p][0]))
				else:
					outFile.write('- ')
		outFile.write('\n')
	outFile.write('\n')

	# br
	outFile.write('%-35s   lnN   '%('CMS_hgg_BR'))
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs or p in spin2Procs:
				outFile.write('- ')
			else:
				outFile.write('%5.3f/%5.3f '%(1./(1.-brSyst[1]),1.+brSyst[0]))
	outFile.write('\n')
	outFile.write('\n')

def printLumiSyst():
	print 'Lumi...'
	outFile.write('%-35s   lnN   '%('lumi_%dTeV'%sqrts))
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs:
				outFile.write('- ')
			else:
				outFile.write('%5.3f '%(1.+lumiSyst))
	outFile.write('\n')
	outFile.write('\n')

def printTrigSyst():
	print 'Trig...'
	outFile.write('%-35s   lnN   '%'CMS_hgg_n_trig_eff')
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs:
				outFile.write('- ')
			else:
				outFile.write('%5.3f '%(1.+trigEff))
	outFile.write('\n')
	outFile.write('\n')

def getGlobeLine(proc,cat,name):
	th1f_nom = inFile.Get('th1f_sig_%s_mass_m125_cat%d'%(proc,cat))
	th1f_up  = inFile.Get('th1f_sig_%s_mass_m125_cat%d_%sUp01_sigma'%(proc,cat,name))
	th1f_dn  = inFile.Get('th1f_sig_%s_mass_m125_cat%d_%sDown01_sigma'%(proc,cat,name))
	systVals = interp1Sigma(th1f_nom,th1f_dn,th1f_up)
	globeSystDump.write('%s nom: %5.3f up: %5.3f down: %5.3f vals: [%5.3f,%5.3f] \n'%(name,th1f_nom.Integral(),th1f_up.Integral(),th1f_dn.Integral(),systVals[0],systVals[1]))
	if options.isBinnedSignal: 
		line = '0.333 '
	else:
		if systVals[0]==1 and systVals[1]==1:
			line = '- '
		else:
			line = '%5.3f/%5.3f '%(systVals[0],systVals[1])
	return line

def printGlobeSysts():
	print 'Efficiencies...'
	for globeSyst, paramSyst in globeSysts.items():
		if 'pdfWeight' and 'QCDscale' in globeSyst: # special case
			if options.isBinnedSignal: 
				outFile.write('%-25s   shape   '%(globeSyst))
				for c in range(options.ncats):
					for p in options.procs:
						if '%s:%d'%(p,c) in options.toSkip: continue
						if p in bkgProcs:
							outFile.write('- ')
						else:
							outFile.write('0.333 ')
			else: 
				outFile.write('%-35s   lnN   '%('CMS_hgg_%s_ggH'%paramSyst))
				for c in range(options.ncats):
					for p in options.procs:
						if '%s:%d'%(p,c) in options.toSkip: continue
						if p=='ggH': outFile.write(getGlobeLine(globeProc[p],c,globeSyst))
						else: outFile.write('- ')
				outFile.write('\n')
				outFile.write('%-35s   lnN   '%('CMS_hgg_%s_qqH'%paramSyst))
				for c in range(options.ncats):
					for p in options.procs:
						if '%s:%d'%(p,c) in options.toSkip: continue
						if p=='qqH': outFile.write(getGlobeLine(globeProc[p],c,globeSyst))
						else: outFile.write('- ')
		else:		
			if options.isBinnedSignal:
				outFile.write('%-25s   shape   '%(globeSyst))
			else:
				outFile.write('%-35s   lnN   '%('CMS_hgg_%s'%paramSyst))
			for c in range(options.ncats):
				for p in options.procs:
					if '%s:%d'%(p,c) in options.toSkip: continue
					if p in bkgProcs or ('pdfWeight' in globeSyst and (p!='ggH' and p!='qqH')):
						outFile.write('- ')
					else:
						outFile.write(getGlobeLine(globeProc[p],c,globeSyst))
		outFile.write('\n')
	outFile.write('\n')

def printVbfSysts():
	# these always confuse me !
	# the plan is to loop vbf systematics

	# we first figure out what migrations are needed
	# e.g. for 5 inc cats and 3 vbf cats we need:
	# cat5 -> cat6, cat5+cat6 -> cat7, cat5+cat6+cat7 -> incCats
	# make arrays for these but then actually reverse them so they match the ordering 
	# of the numbers written at the top and in the twiki
	# the other important thing is to adjust the name of the VBFtot -> incCats mig to 
	# correlate with combination for the QCDscale and UEPS

	vbfMigrateFromCats=[]
	vbfMigrateToCats=[]
	vbfMigrateFromEvCount={}
	vbfMigrateToEvCount={}

	# work out which cats we are migrating to and from
	temp = []
	for c in dijetCats:
		temp.append(c)
		vbfMigrateFromCats.append(copy.copy(temp))
		if c==len(incCats)+len(dijetCats)-1: # i.e. last vbf cat
			vbfMigrateToCats.append(incCats)
		else:
			vbfMigrateToCats.append([c+1])
	# reverse
	vbfMigrateToCats.reverse()
	vbfMigrateFromCats.reverse()
	
	# now get relevant event counts
	for p in options.procs:
		if p in bkgProcs: continue
		vbfMigrateToEvCount[p] = []
		for cats in vbfMigrateToCats:
			sum=0
			for c in cats:
				th1f =  inFile.Get('th1f_sig_%s_mass_m125_cat%d'%(globeProc[p],c))
				sum += th1f.Integral()
			vbfMigrateToEvCount[p].append(sum)
	for p in options.procs:
		if p in bkgProcs: continue
		vbfMigrateFromEvCount[p] = []
		for cats in vbfMigrateFromCats:
			sum=0
			for c in cats:
				th1f =  inFile.Get('th1f_sig_%s_mass_m125_cat%d'%(globeProc[p],c))
				sum += th1f.Integral()
			vbfMigrateFromEvCount[p].append(sum)
	
	#print 'From:   ',vbfMigrateFromCats
	#print '\t Evs: ', vbfMigrateFromEvCount
	#print 'To: ', vbfMigrateToCats
	#print '\t Evs: ', vbfMigrateToEvCount

	# now print relevant numbers
	for vbfSystName, vbfSystValArray in vbfSysts.items():
		for migIt, vbfSystVal in enumerate(vbfSystValArray):
			name = vbfSystName
			if migIt>0:
				name += '_migration%d'%migIt
			outFile.write('%-35s   lnN   '%name)
			for c in range(options.ncats):
				for p in options.procs:
					if '%s:%d'%(p,c) in options.toSkip: continue
					if p=='ggH': thisUncert = vbfSystVal[0]
					elif p=='qqH': thisUncert = vbfSystVal[1]
					else:
						outFile.write('- ')
						continue
					if thisUncert==0:
						outFile.write('- ')
					else:
						if c in vbfMigrateToCats[migIt]:
							outFile.write('%1.4g '%((vbfMigrateToEvCount[p][migIt]-thisUncert*vbfMigrateFromEvCount[p][migIt])/vbfMigrateToEvCount[p][migIt]))
						elif c in vbfMigrateFromCats[migIt]:
							outFile.write('%1.4g '%(1.+thisUncert))
						else:
							outFile.write('- ')
			outFile.write('\n')
		outFile.write('\n')
	
	# pu id eff  -- NOTE to correlate with combination change to CMS_eff_j
	# only in 2012
	if not options.is2011:
		outFile.write('%-35s   lnN   '%('CMS_eff_j'))
		vbfCatCounter=0
		for c in range(options.ncats):
			for i,p in enumerate(options.procs):
				if '%s:%d'%(p,c) in options.toSkip: continue
				if p in bkgProcs:
					outFile.write('- ')
					continue
				if c in dijetCats or c in vhHadCat:
					outFile.write('%6.4f/%6.4f '%(1.-puJetIdEff[vbfCatCounter][i],1.+puJetIdEff[vbfCatCounter][i]))
					if i==len(options.procs)-2: 
						vbfCatCounter += 1
				else:
					outFile.write('- ')
					continue
		outFile.write('\n')

def printLepSysts():
	print 'Lep...'
	# electron efficiency -- NOTE to correlate with combination change to CMS_eff_e
	outFile.write('%-35s   lnN   '%('CMS_eff_e'))
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: 
				outFile.write('- ')
				continue
			if p in bkgProcs or p=='ggH' or p=='qqH': 
				outFile.write('- ')
				continue
			else:
				if c in tightLepCat: thisUncert = eleSyst[p][0]
				elif c in looseLepCat: thisUncert = eleSyst[p][1]
				elif c in tthLepCat: thisUncert = eleSyst[p][2]
				else: thisUncert = 0.
				if thisUncert==0:
					outFile.write('- ')
				else:
					outFile.write('%6.4f/%6.4f '%(1.-thisUncert,1+thisUncert))
	outFile.write('\n')
	
	# muon efficiency -- NOTE to correlate with combination change to CMS_eff_m
	outFile.write('%-35s   lnN   '%('CMS_eff_m'))
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: 
				outFile.write('- ')
				continue
			if p in bkgProcs or p=='ggH' or p=='qqH': 
				outFile.write('- ')
				continue
			else:
				if c in tightLepCat: thisUncert = muonSyst[p][0]
				elif c in looseLepCat: thisUncert = muonSyst[p][1]
				elif c in tthLepCat: thisUncert = muonSyst[p][2]
				else: thisUncert = 0.
				if thisUncert==0:
					outFile.write('- ')
				else:
					outFile.write('%6.4f/%6.4f '%(1.-thisUncert,1+thisUncert))
	outFile.write('\n')

	# met efficiency -- NOTE to correlate with combination change to CMS_scale_met
	outFile.write('%-35s   lnN   '%('CMS_scale_met'))
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: 
				outFile.write('- ')
				continue
			if p in bkgProcs or p=='ggH' or p=='qqH': 
				outFile.write('- ')
				continue
			else:
				if c in tightLepCat: thisUncert = metSyst[p][0]
				elif c in looseLepCat: thisUncert = metSyst[p][1]
				elif c in tthLepCat: thisUncert = metSyst[p][2]
				else: thisUncert = 0.
				if thisUncert==0:
					outFile.write('- ')
				else:
					outFile.write('%6.4f/%6.4f '%(1.-thisUncert,1+thisUncert))
	outFile.write('\n')

def printTTHSysts():
	print 'tth...'
	# b tag efficiency
	outFile.write('%-35s   lnN   '%('CMS_eff_b'))
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: 
				outFile.write('- ')
				continue
			if p in bkgProcs: 
				outFile.write('- ')
				continue
			if c in tthCats:
				if options.is2011:
					thisUncert = (btagSyst[p][0]**2+btagSyst[p][1]**2)**0.5
				else:
					if c in tthLepCat: 
						thisUncert = btagSyst[p][0]
					if c in tthHadCat:
						thisUncert = btagSyst[p][1]
				if thisUncert==0:
					outFile.write('- ')
				else:
					outFile.write('%6.4f/%6.4f '%(1.-thisUncert,1.+thisUncert))
			else:
				outFile.write('- ')
	outFile.write('\n')

	# ggh uncerts on tth
	for systName, systVal in ggHforttHSysts.items():
		outFile.write('%-35s   lnN   '%systName)
		for c in range(options.ncats):
			for p in options.procs:
				if '%s:%d'%(p,c) in options.toSkip: 
					outFile.write('- ')
					continue
				if p=='ggH' and c in tthCats:
					outFile.write('%6.4f/%6.4f '%(1.-systVal,1.+systVal))
				else:
					outFile.write('- ')
					continue
		outFile.write('\n')

def printMultiPdf():
	if options.isMultiPdf:
		for c in range(options.ncats):
			outFile.write('pdfindex_%d_%dTeV  discrete\n'%(c,sqrts))

# __main__ here
printPreamble()
printFileOptions()
printObsProcBinLines()
printNuisParams()
printTheorySysts()
printLumiSyst()
printTrigSyst()
printGlobeSysts()
if not options.isSpinModel:
	printVbfSysts()
	printLepSysts()
	printTTHSysts()
printMultiPdf()


# DEFUNCT OLD FUNCTIONS........
"""
def printLepSystsOld():

	print 'Lep...'
	# electron efficiency
	outFile.write('%-35s   lnN   '%('CMS_hgg_eff_e'))
	eleEvCount={}
	incEvCount={}
	for p in options.procs:
		eleEvCount[p] = 0.
		incEvCount[p] = 0.
		for c in range(options.ncats):
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs: continue
			th1f = inFile.Get('th1f_sig_%s_mass_m125_cat%d'%(globeProc[p],c))
			if c in incCats: incEvCount[p] += th1f.Integral()
			elif c in tthLepCat or c in eleCat: eleEvCount[p] += th1f.Integral()
			else: continue
	#write lines
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs:
				outFile.write('- ')
				continue
			else:
				thisUncert = eleSyst[p]
			if c in incCats:
				if thisUncert != 0:
					outFile.write('%6.4f '%((incEvCount[p]-thisUncert*eleEvCount[p])/incEvCount[p]))
				else:
					outFile.write('- ')						
			elif c in tthLepCat or c in eleCat:
				if thisUncert != 0:
					outFile.write('%6.4f '%(1.+thisUncert))
				else:
					outFile.write('- ')
			else:
				outFile.write('- ')
	outFile.write('\n')

	# muon efficiency
	outFile.write('%-35s   lnN   '%('CMS_hgg_eff_m'))
	muonEvCount={}
	incEvCount={}
	for p in options.procs:
		muonEvCount[p] = 0.
		incEvCount[p] = 0.
		for c in range(options.ncats):
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs: continue
			th1f = inFile.Get('th1f_sig_%s_mass_m125_cat%d'%(globeProc[p],c))
			if c in incCats: incEvCount[p] += th1f.Integral()
			elif c in tthLepCat or c in muonCat: muonEvCount[p] += th1f.Integral()
			else: continue
	#write lines
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs:
				outFile.write('- ')
				continue
			else:
				thisUncert = muonSyst[p]
			if c in incCats:
				if thisUncert != 0:
					outFile.write('%6.4f '%((incEvCount[p]-thisUncert*muonEvCount[p])/incEvCount[p]))
				else:
					outFile.write('- ')						
			elif c in tthLepCat or c in muonCat:
				if thisUncert != 0:
					outFile.write('%6.4f '%(1.+thisUncert))
				else:
					outFile.write('- ')
			else:
				outFile.write('- ')
	outFile.write('\n')

def printMetSystsOld():
	
	#met efficiency
	print 'Met ...'
	outFile.write('%-35s   lnN   '%('CMS_hgg_scale_met'))
	metEvCount={}
	incEvCount={}
	for p in options.procs:
		metEvCount[p] = 0.
		incEvCount[p] = 0.
		for c in range(options.ncats):
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs: continue
			th1f = inFile.Get('th1f_sig_%s_mass_m125_cat%d'%(globeProc[p],c))
			if c in incCats: incEvCount[p] += th1f.Integral()
			elif c in metCat: metEvCount[p] += th1f.Integral()
			else:continue
	#write lines
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs:
				outFile.write('- ')
				continue
			else:
				thisUncert = metSyst[p]
			if c in incCats:
				if thisUncert != 0:
					outFile.write('%6.4f '%((incEvCount[p]-thisUncert*metEvCount[p])/incEvCount[p]))
				else:
					outFile.write('- ')						
			elif c in metCat:
				if thisUncert != 0:
					outFile.write('%6.4f '%(1.+thisUncert))
				else:
					outFile.write('- ')
			else:
				outFile.write('- ')
	outFile.write('\n')
        
	#migration from the two vhlep cat due to met
	outFile.write('%-35s   lnN   '%('CMS_hgg_met_migration'))
	tightLepEvCount={}
	looseLepEvCount={}
	for p in options.procs:
		tightLepEvCount[p]=0.
		looseLepEvCount[p]=0.
		
		for c in range(options.ncats):
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs: continue
			if c in tightLepCat or c in looseLepCat:
				th1f = inFile.Get('th1f_sig_%s_mass_m125_cat%d'%(globeProc[p],c))
				if c in tightLepCat:
					tightLepEvCount[p] += th1f.Integral()
				else:
					looseLepEvCount[p] += th1f.Integral()
	# write lines
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs:
				outFile.write('- ')
				continue
			else:
				thisUncert = metSyst[p]
			if c in tightLepCat or c in looseLepCat:
				if c in tightLepCat:
					outFile.write('%6.4f '%(1.+thisUncert))
				elif c in eleCat:
					if looseLepEvCount[p]==0:
						outFile.write('1.000 ')
					else:
						outFile.write('%6.4f '%((looseLepEvCount[p]-thisUncert*tightLepEvCount[p])/looseLepEvCount[p]))
			else:
				outFile.write('- ')
	outFile.write('\n')

def printTTHSystsOld():
	print 'TTH...'
	for tthSystName, tthSystVals in tthSysts.items():
		outFile.write('%-35s   lnN   '%tthSystName)
		tthEvCount={}
		incEvCount={}
		for p in options.procs:
			tthEvCount[p] = 0.
			incEvCount[p] = 0.
			for c in range(options.ncats):
				if '%s:%d'%(p,c) in options.toSkip: continue
				if p in bkgProcs: continue
				th1f = inFile.Get('th1f_sig_%s_mass_m125_cat%d'%(globeProc[p],c))
				if c in incCats: incEvCount[p] += th1f.Integral()
				elif c in tthCats: tthEvCount[p] += th1f.Integral()
				else:continue
		#write lines
		for c in range(options.ncats):
			for p in options.procs:
				if '%s:%d'%(p,c) in options.toSkip: continue
				if p in bkgProcs:
					outFile.write('- ')
					continue
				elif p=='ttH': 
					thisUncert = tthSystVals[1]
				elif p=='qqH':
					thisUncert = 0
				else:
					thisUncert = tthSystVals[0]
				if c in incCats:
					if thisUncert != 0:
						outFile.write('%6.4f '%((incEvCount[p]-thisUncert*tthEvCount[p])/incEvCount[p]))
					else:
						outFile.write('- ')						
				elif c in tthCats:
					if thisUncert != 0:
						outFile.write('%6.4f '%(1.+thisUncert))
					else:
						outFile.write('- ')
				else:
					outFile.write('- ')
		outFile.write('\n')
"""
