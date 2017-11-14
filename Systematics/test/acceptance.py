# import ROOT in batch mode
import sys
import math
from subprocess import call, check_output
import re
oldargv = sys.argv[:]
#sys.argv = [ '-b-' ]
import ROOT
#ROOT.gROOT.SetBatch(True)
sys.argv = oldargv

# load FWLite C++ libraries
ROOT.gSystem.Load("libFWCoreFWLite.so");
ROOT.gSystem.Load("libDataFormatsFWLite.so");
ROOT.FWLiteEnabler.enable()

# load FWlite python libraries
from DataFormats.FWLite import Handle, Events

def deltaPhi(phi1,phi2):
        pi=3.14159265359
        dphi=phi1-phi2
        while dphi > pi: dphi -= 2*pi
        while dphi < -pi: dphi += 2*pi
        return dphi

def deltaR(eta1,phi1,eta2,phi2):
        return math.sqrt( (eta1-eta2)**2 + deltaPhi(phi1,phi2)**2)

h = ROOT.TH1D("acc","acc",10,0-.5,10-.5)

#files=['/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-250To400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/120000/BC262FB4-6FCD-E611-B788-90B11C2CB121.root']

#/store/mc/RunIISummer16MiniAODv2/GluGluHToGG_M-125_13TeV_powheg_pythia8//MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1
list0=check_output('/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select find -f /store/mc/RunIISummer16MiniAODv2/GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1',shell=True).split()
files=[ re.sub("/eos/cms","root://eoscms//",f) for f in list0 ]


## counters events
count={"Out-250-400":0.0,"Out-pos":0.0,"Out-neg":0.0,"In-250-400":0.0}
try:
   for f in files:
	# open file (you can use 'edmFileUtil -d /store/whatever.root' to get the physical file name)
	print "->Opening file",f.split()[0]
	events = Events(f.split()[0])
	lhe,lheLabel = Handle("LHEEventProduct"),"externalLHEProducer"
	gen,genLabel = Handle("std::vector<pat::PackedGenParticle>"),"packedGenParticles"

	for iev,event in enumerate(events):
	        event.getByLabel(lheLabel, lhe)
	        event.getByLabel(genLabel, gen)
		if not gen.isValid(): print "gen is not valid"

		hepeup = lhe.product().hepeup()
		w=lhe.product().weights()[0].wgt

		h.Fill(0,w) # all weights

		#V=ROOT.TLorentzVector()
		#first loop photons

		pho1=None
		pho2=None
		photons=[]

		for gp in gen.product():
			if gp.pdgId() !=22 : continue;

			if pho1 == None: 
				pho1 = gp
			elif gp.pt() > pho1.pt():
				pho2=pho1
				pho1=gp
			elif pho2 == None or gp.pt() > pho2.pt() :
				pho2 = gp
			
			if abs(gp.eta())<2.5: ## fully combinatorics
				photons.append(gp)

		if pho1==None or pho2==None:continue

		H=ROOT.TLorentzVector()
		g1 = ROOT.TLorentzVector()
		g2 = ROOT.TLorentzVector()
		g1.SetPtEtaPhiM(pho1.pt(),pho1.eta(),pho1.phi(),0.0)
		g2.SetPtEtaPhiM(pho2.pt(),pho2.eta(),pho2.phi(),0.0)
		H= g1+g2
		passSel=True

		if g1.Pt()/H.M() <= 1./3.: passSel=False
		if g2.Pt()/H.M() <= 1./4.: passSel=False
		if abs(g1.Eta()) > 2.5: passSel=False
		if abs(g2.Eta()) > 2.5: passSel=False

		h.Fill(1,w) # no iso
		iso1=0.0
		iso2=0.0
		isolations=[ 0.0 for p in photons ]
		for gp in gen.product():
			if gp == pho1 : continue
			if gp == pho2 : continue

			if deltaR(pho1.eta(),pho1.phi(),gp.eta(),gp.phi())<0.3:
				iso1+= gp.pt()
			if deltaR(pho2.eta(),pho2.phi(),gp.eta(),gp.phi())<0.3:
				iso2+= gp.pt()
			for ip,pho in enumerate(photons):
				if deltaR(pho.eta(),pho.phi(),gp.eta(),gp.phi())>=0.3:continue
				isolations[ip] += gp.pt()

		if ( abs(H.M() ) - 125.) >1 and passSel:
			print "Outlier!m=",H.M()
			passSel=False

		if iso2<10 and iso1<10 and passSel:	
			h.Fill(2,w) # Iso

		## fully combinatorics
		pho1=None
		pho2=None
		valid=[]
		for ip,pho in enumerate(photons):
		    if isolations[ip]>=10:continue
		    if pho1 == None: 
		    	pho1 = pho
		    elif pho.pt() > pho1.pt():
		    	pho2=pho1
		    	pho1=pho
		    elif pho2 == None or pho.pt() > pho2.pt() :
				pho2 = pho

		if pho1==None or pho2==None:continue ##  check it, because the eta requirement may have killed them

		g1.SetPtEtaPhiM(pho1.pt(),pho1.eta(),pho1.phi(),0.0)
		g2.SetPtEtaPhiM(pho2.pt(),pho2.eta(),pho2.phi(),0.0)
		H= g1+g2
		if g1.Pt()/H.M() <= 1./3.: continue
		if g2.Pt()/H.M() <= 1./4.: continue
		if ( abs(H.M() ) - 125.) >1:
			print "Outlier FC! m=",H.M()
			continue

		h.Fill(3,w)


except KeyboardInterrupt:
	pass

print "DONE"
print "-------------------"
print "SumW=",h.GetBinContent(1)
print "AcceptanceBeforeIso=",h.GetBinContent(2)/h.GetBinContent(1)
print "Acceptance=",h.GetBinContent(3)/h.GetBinContent(1)
print "Acceptance FC=",h.GetBinContent(4)/h.GetBinContent(1)
print "-------------------"
