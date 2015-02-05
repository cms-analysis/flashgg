import ROOT as r
import os,sys

r.gROOT.SetStyle("Plain")
r.gROOT.SetBatch(True)

r.gROOT.ProcessLine(".L python/makeToyWS.C+g")
from ROOT import makeToyWS

class CombinedToyMaker:

  def __init__(self,mitFileName):
    
    r.gROOT.SetStyle("Plain")
    r.gROOT.SetBatch(True)

    self.is2011_      = False
    if mitFileName:

	    self.mitFileName_ = mitFileName
	    self.mitFile_     = r.TFile(mitFileName)
	    self.mitWS_       = self.mitFile_.Get("cms_hgg_workspace")
	    self.mitvar_      = self.mitWS_.var("CMS_hgg_mass")
	    self.mitpdfs_     =[]
	    self.mitsighists_ =[]
	    self.mitsigpdfs_  =[]
	    # get background pdf for each mit mass cat
	    for cat in range(5): 
	      self.mitpdfs_.append(self.mitWS_.pdf("pdf_data_pol_model_cat%i"%cat))
	    # get signal histpdf for each mit mass cat
	    for cat in range(5):
	      temp = self.mitWS_.data("roohist_sig_ggh_mass_m124_cat%i"%cat)
	      temp.add(self.mitWS_.data("roohist_sig_vbf_mass_m124_cat%i"%cat))
	      temp.add(self.mitWS_.data("roohist_sig_wzh_mass_m124_cat%i"%cat))
	      temp.add(self.mitWS_.data("roohist_sig_tth_mass_m124_cat%i"%cat))
      	      self.mitsigpdfs_.append(r.RooHistPdf("sigmasspdf_cat%i"%cat,"sigmasspdf_cat%i"%cat,r.RooArgSet(self.mitvar_),temp))

    self.hasFit_      = False
    self.rand_        = r.TRandom3(0)

  def setIs2011(self,isit=True):
    self.is2011_ = isit

  def createPdfs(self,infile,outfile,expSig=0):
    makeToyWS(infile,outfile)
    self.loadPdfs(outfile,expSig)

  def loadPdfs(self,filename,expSig=0):

    if self.is2011_ :bdtcutstring = "bdtoutput>0.05" 
    else: bdtcutstring = "bdtoutput>-0.05" 

    fi = r.TFile(filename)
    self.keyws_           = fi.Get("fits_workspace")
    print 'Loading from.. '
    self.keyws_.Print()
    self.bdtvar_          = self.keyws_.var("bdtoutput")
    self.bdtmgg_          = self.keyws_.var("CMS_hgg_mass")
    self.bdtvbf_          = self.keyws_.var("vbf")
    #self.bdtdatakeys_     = self.keyws_.data("data_forkeyspdf")
    self.bdtdata_         = self.keyws_.data("data_bdt_novbf")
    self.bdtdatavbf_      = self.keyws_.data("data_bdt_vbf")
    if not self.is2011_: self.bdtdatavbftight_      = self.keyws_.data("data_bdt_vbf_tight")
    #self.bdtdatacut_      = self.keyws_.data("data_bdt_cut_all")
    self.icpdf_           = self.keyws_.pdf("data_pow_model")
    self.bdtpdf_          = self.keyws_.pdf("data_pdf")
    self.dataNoVBFEvents_ = self.bdtdata_.sumEntries(bdtcutstring)
    self.dataVBFEvents_   = self.bdtdatavbf_.sumEntries(bdtcutstring)
    if expSig>0:
      self.bdtweight_     = self.keyws_.var("weight")
      self.bdtsigpdf_     = self.keyws_.pdf("sig_pdf")
      self.bdtsigdata_    = self.keyws_.data("sig_bdt_novbf")
      self.bdtsigdatavbf_ = self.keyws_.data("sig_bdt_vbf")
      if not self.is2011_: self.bdtsigdatavbftight_ = self.keyws_.data("sig_bdt_vbf_tight")
      self.sigNoVBFEvents_= self.bdtsigdata_.sumEntries(bdtcutstring)
      self.sigVBFEvents_  = self.bdtsigdatavbf_.sumEntries(bdtcutstring)
      if not self.is2011_:self.sigVBFEventstight_  = self.bdtsigdatavbftight_.sumEntries(bdtcutstring)
    
    print "Non VBF events (data):  ", self.dataNoVBFEvents_
    print "VBF events (data):      ",self.dataVBFEvents_
    if expSig>0:
      print "Non VBF events (sig):   ", self.sigNoVBFEvents_
      print "VBF events (sig):       ",self.sigVBFEvents_
    self.hasFit_      = True

  def savePdfWorkspace(self,filename,expSig):
    fi = r.TFile(filename,"RECREATE")
    fi.cd()
    self.ws_          = r.RooWorkspace("toysworkspace")
    getattr(self.ws_,'import')(self.bdtdata_)
    getattr(self.ws_,'import')(self.bdtdatavbf_)
    getattr(self.ws_,'import')(self.bdtpdf_)
    if expSig>0:
      getattr(self.ws_,'import')(self.bdtsigdata_)
      getattr(self.ws_,'import')(self.bdtsigdatavbf_)
      getattr(self.ws_,'import')(self.bdtsigpdf_)
    self.ws_.Write()
    print "Saved PDF and DataSets to workspace in ", fi.GetName()
    fi.Close()

  def saveToyWorkspace(self,filename):
    fi = r.TFile(filename,"RECREATE")
    fi.cd()
    self.toyws_       = r.RooWorkspace("gentoyworkspace")
    getattr(self.toyws_,'import')(self.genbdtdata_)
    for i, genmdat in enumerate(self.genmassdata_):
      getattr(self.toyws_,'import')(genmdat)
      getattr(self.toyws_,'import')(self.mitpdfs_[i])

    getattr(self.toyws_,'import')(self.allgenmassdata_)
    self.toyws_.Write()
    fi.Close()

  def getEventsPerCat(self,gendata):
    events=[]
    events.append(gendata.sumEntries("bdtoutput>=0.89"))
    events.append(gendata.sumEntries("bdtoutput>=0.74 && bdtoutput<0.89"))
    events.append(gendata.sumEntries("bdtoutput>=0.545 && bdtoutput<0.74"))
    events.append(gendata.sumEntries("bdtoutput>=0.05 && bdtoutput<0.545"))
    return events

  def genData(self,outwsname,expSig):
    if not self.hasFit_: sys.exit('Diphoton output not yet fitted. Bailing out.')

    # set ranges
    self.bdtvar_.setRange("cat0",0.89,1.0)
    self.bdtvar_.setRange("cat1",0.74,0.89)
    self.bdtvar_.setRange("cat2",0.545,0.74)
    self.bdtvar_.setRange("cat3",0.05,0.545)
    
    print 'Data and pdfs obtained:'
    for i, events in enumerate(self.getEventsPerCat(self.bdtdata_)):
      print '\tCat %d  data has    %5d events'%(i,events)
    print '\tCat 4  data has    %5d events'%self.bdtdatavbf_.sumEntries("bdtoutput>=0.05")
    print '\tNo vbf data has    %5d events'%self.bdtdata_.sumEntries("bdtoutput>=0.05")
    print '\tComb   data has    %5d events'%(self.bdtdata_.sumEntries("bdtoutput>=0.05")+self.bdtdatavbf_.sumEntries("bdtoutput>=0.05"))

    # first gen around bdt keys pdf
    self.toyNoVBFEvents_ = self.rand_.Poisson(self.dataNoVBFEvents_)
    self.bdtvar_.setRange(0.05,1.)
    self.genbdtdata_  = self.bdtpdf_.generate(r.RooArgSet(self.bdtvar_),self.toyNoVBFEvents_)
    self.genbdtdata_.SetName("gen_diphoton_output")

    self.genbdtdatacats_=[]
    self.genbdtdatacats_.append(r.RooDataSet("gen_bdtoutput_cat0","gen_bdtoutput_cat0",r.RooArgSet(self.bdtvar_),r.RooFit.Import(self.genbdtdata_),r.RooFit.Cut("bdtoutput>=0.89")));
    self.genbdtdatacats_.append(r.RooDataSet("gen_bdtoutput_cat1","gen_bdtoutput_cat1",r.RooArgSet(self.bdtvar_),r.RooFit.Import(self.genbdtdata_),r.RooFit.Cut("bdtoutput>=0.74 && bdtoutput<0.89")));
    self.genbdtdatacats_.append(r.RooDataSet("gen_bdtoutput_cat2","gen_bdtoutput_cat2",r.RooArgSet(self.bdtvar_),r.RooFit.Import(self.genbdtdata_),r.RooFit.Cut("bdtoutput>=0.545 && bdtoutput<0.74")));
    self.genbdtdatacats_.append(r.RooDataSet("gen_bdtoutput_cat3","gen_bdtoutput_cat3",r.RooArgSet(self.bdtvar_),r.RooFit.Import(self.genbdtdata_),r.RooFit.Cut("bdtoutput>=0.05 && bdtoutput<0.545")));

    eventsPerCat = self.getEventsPerCat(self.genbdtdata_)
    if len(eventsPerCat)!=len(self.mitpdfs_)-1: sys.exit('Different numbers of categories. Bailing out.')

    self.genmassdata_ = []
    # given nevents in bdtoutput generate around each non VBF cat
    for i,dset in enumerate(self.genbdtdatacats_):
      self.genmassdata_.append(self.mitpdfs_[i].generate(r.RooArgSet(self.bdtmgg_),dset.numEntries()))
      self.genmassdata_[i].SetName("gen_mass_data_cat%d"%i)
      if i==0: self.allgenmassdatanovbf_=r.RooDataSet("gen_combmassdata_novbf","gen_combmassdata_novbf",self.genmassdata_[i],r.RooArgSet(self.bdtmgg_))
      else: self.allgenmassdatanovbf_.append(self.genmassdata_[i])

    # throw toy in VBF cat
    self.toyVBFEvents_ = self.rand_.Poisson(int(self.dataVBFEvents_))
    self.genmassdata_.append(self.mitpdfs_[4].generate(r.RooArgSet(self.bdtmgg_),int(self.toyVBFEvents_)))
    self.genmassdata_[4].SetName("gen_mass_data_cat4")

    # combine data in VBF cat for IC analysis
    self.allgenmassdata_ = r.RooDataSet("gen_combmassdata_all","gen_combmassdata_all",self.allgenmassdatanovbf_,r.RooArgSet(self.bdtmgg_))
    self.allgenmassdata_.append(self.genmassdata_[4])

    print 'Background toy thrown:'
    for i, massdat in enumerate(self.genmassdata_):
      print '\tCat %d  toy has     %5d events'%(i,massdat.sumEntries())
    print '\tNo vbf toy has     %5d events'%self.allgenmassdatanovbf_.sumEntries()
    print '\tComb   toy has     %5d events'%self.allgenmassdata_.numEntries()

    # then throw signal around bdt correlated with total mass and split into mass categories
    self.gensigbdtdata_ = []
    if expSig>0:
      print 'Sig MC and pdfs obtained:'
      for i, events in enumerate(self.getEventsPerCat(self.bdtsigdata_)):
        print '\tCat %d  sig mc has  %5d events'%(i,events)
      print '\tCat 4  sig mc has  %5d events'%self.bdtsigdatavbf_.sumEntries("bdtoutput>=0.05")
      print '\tNo vbf sig mc has  %5d events'%self.bdtsigdata_.sumEntries("bdtoutput>=0.05")
      print '\tComb   sig mc has  %5d events'%(self.bdtsigdata_.sumEntries("bdtoutput>=0.05")+self.bdtsigdatavbf_.sumEntries("bdtoutput>=0.05"))

      self.toyNoVBFEventsSig_ = self.rand_.Poisson(expSig*self.sigNoVBFEvents_)
      self.gensigdatanovbf_ = self.bdtsigpdf_.generate(r.RooArgSet(self.bdtvar_,self.bdtmgg_),self.toyNoVBFEventsSig_)
      self.gensigdatanovbf_.SetName("gen_sig_output_novbf")
     
      self.gensigbdtdata_=[]
      self.gensigbdtdata_.append(r.RooDataSet("gen_sig_data_cat0","gen_sig_data_cat0",r.RooArgSet(self.bdtvar_,self.bdtmgg_),r.RooFit.Import(self.gensigdatanovbf_),r.RooFit.Cut("bdtoutput>=0.89")));
      self.gensigbdtdata_.append(r.RooDataSet("gen_sig_data_cat1","gen_sig_data_cat1",r.RooArgSet(self.bdtvar_,self.bdtmgg_),r.RooFit.Import(self.gensigdatanovbf_),r.RooFit.Cut("bdtoutput>=0.74 && bdtoutput<0.89")));
      self.gensigbdtdata_.append(r.RooDataSet("gen_sig_data_cat2","gen_sig_data_cat2",r.RooArgSet(self.bdtvar_,self.bdtmgg_),r.RooFit.Import(self.gensigdatanovbf_),r.RooFit.Cut("bdtoutput>=0.545 && bdtoutput<0.74")));
      self.gensigbdtdata_.append(r.RooDataSet("gen_sig_data_cat3","gen_sig_data_cat3",r.RooArgSet(self.bdtvar_,self.bdtmgg_),r.RooFit.Import(self.gensigdatanovbf_),r.RooFit.Cut("bdtoutput>=0.05 && bdtoutput<0.545")));

      # throw signal toy in VBF cat
      self.toyVBFEventsSig_ = self.rand_.Poisson(expSig*self.sigVBFEvents_)
      self.gensigmassdatavbf_ = self.mitsigpdfs_[4].generate(r.RooArgSet(self.bdtmgg_),self.toyVBFEventsSig_)

      print 'Signal (%d x SM) toy thrown:'%expSig
      #for i, events in enumerate(self.getEventsPerCat(self.gensigdatanovbf_)):
      for i, massdat in enumerate(self.gensigbdtdata_):
        print '\tCat %d  sig toy has %5d events'%(i,self.gensigbdtdata_[i].sumEntries())
      print '\tCat 4  sig toy has %5d events'%(self.gensigmassdatavbf_.sumEntries())
      print '\tNo vbf sig toy has %5d events'%self.gensigdatanovbf_.sumEntries()
      print '\tComb   sig toy has %5d events'%(self.gensigdatanovbf_.sumEntries()+self.gensigmassdatavbf_.sumEntries())

		# combine signal and background
    self.genbkgmassdata_=[]
    for i, dset in enumerate(self.genmassdata_):
     	self.genbkgmassdata_.append(dset.Clone("gen_bkg_mass_cat%d"%i))
     	if expSig>0:                                      
     	  if i<4: dset.append(self.gensigbdtdata_[i])
     	  else: dset.append(self.gensigmassdatavbf_)
     	dset.Print()                                      
    
    self.genbkgbdtdata_=[]
    for i, dset in enumerate(self.genbdtdatacats_):
     	self.genbkgbdtdata_.append(dset.Clone("gen_bkg_bdt_cat%d"%i))
     	if expSig>0: dset.append(self.gensigbdtdata_[i])                                      
     	dset.Print()                                      
      
    # now have following datasets:
    # BKG: genbkgmassdata_[5] (mass)
    #      genbkgbdtdata_[4]  (BDT)
    # SIG: gensigbdtdata_[4]  (mass,BDT)
    #      gensigmassdatavbf_ (mass)
    # S+B: genmassdata_[5]    (mass)
    #      genbdtdatacats_[4] (BDT)

    # name gen datasets and make gen datahists to match workspace and save workspace
    self.outwsFile_ = r.TFile(outwsname,"RECREATE")
    self.outws_ = r.RooWorkspace("cms_hgg_workspace")
    self.genmassdatahist_=[]
    self.bdtmgg_.setBins(160)
    for i,dataset in enumerate(self.genmassdata_):
      dataset.SetName("data_mass_cat%i"%i)
      self.genmassdatahist_.append(r.RooDataHist("roohist_data_mass_cat%i"%i,"roohist_data_mass_cat%i"%i,r.RooArgSet(self.bdtmgg_),dataset))
      getattr(self.outws_,'import')(dataset)
      getattr(self.outws_,'import')(self.genmassdatahist_[i])
    self.outwsFile_.cd()
    self.outws_.Write()
    print 'Mass fac toy workspace written to file ', self.outwsFile_.GetName()
    self.outwsFile_.Close()

  def getToyVBFevents(self):
    print 'Toy VBF events', self.toyVBFEvents_
    return int(self.toyVBFEvents_)

  def returnWindowData(self,mH,size):
    returnList = []

    mHL = mH*(1.-size)
    mHH = mH*(1.+size)

    if self.is2011_ : bdtcut = 0.05
    else	    : bdtcut = -0.05

    for i in range(self.bdtdata_.numEntries()):
      val_m = (self.bdtdata_.get(i)).getRealValue("CMS_hgg_mass")
      val_b = (self.bdtdata_.get(i)).getRealValue("bdtoutput")
      if val_m > mHL and val_m < mHH and val_b >=bdtcut: returnList.append((val_b,((val_m-mH)/mH)))

    for i in range(self.bdtdatavbf_.numEntries()):
      val_m = (self.bdtdatavbf_.get(i)).getRealValue("CMS_hgg_mass")
      val_b = 1.01
      if val_m > mHL and val_m < mHH and val_b >=bdtcut: returnList.append((val_b,((val_m-mH)/mH)))

    if self.is2011_:return returnList

    for i in range(self.bdtdatavbftight_.numEntries()):
      val_m = (self.bdtdatavbf_.get(i)).getRealValue("CMS_hgg_mass")
      val_b = 1.05
      if val_m > mHL and val_m < mHH and val_b >=bdtcut: returnList.append((val_b,((val_m-mH)/mH)))

    return returnList

  def returnWindowToyData(self,mH,size):
    returnList = []

    mHL = mH*(1.-size)
    mHH = mH*(1.+size)

    if self.is2011_ : bdtcut = 0.05
    else	    : bdtcut = -0.05

    for cat, dset in enumerate(self.genmassdata_):
      for ev in range(dset.numEntries()):
        val_m = (self.genmassdata_[cat].get(ev)).getRealValue("CMS_hgg_mass");
        if cat<4: val_b = (self.genbdtdatacats_[cat].get(ev)).getRealValue("bdtoutput");
        else: val_b = 1.01
        if val_m > mHL and val_m < mHH and val_b >=bdtcut: returnList.append((val_b,((val_m-mH)/mH)))

    return returnList
  
  def returnSignalAsimovData(self,mH,size):
    returnList = []

    mHL = mH*(1.-size)
    mHH = mH*(1.+size)

    print self.bdtsigdata_.numEntries()
    print self.bdtsigdata_.sumEntries()
    print self.bdtsigdatavbf_.numEntries()
    print self.bdtsigdatavbf_.sumEntries()

    self.bdtsigdata_.Print()
    self.bdtsigdatavbf_.Print()

    if self.is2011_ : bdtcut = 0.05
    else	    : bdtcut = -0.05

    test = r.TH1F("t","t",100,-1,1)

    for i in range(self.bdtsigdata_.numEntries()):
      val_m = (self.bdtsigdata_.get(i)).getRealValue("CMS_hgg_mass")
      val_b = (self.bdtsigdata_.get(i)).getRealValue("bdtoutput")
      val_w = self.bdtsigdata_.weight()
      if val_m > mHL and val_m < mHH and val_b>=0.05:
        returnList.append((val_b,((val_m-mH)/mH),val_w))
        test.Fill(val_b,val_w)

    for i in range(self.bdtsigdatavbf_.numEntries()):
      val_m = (self.bdtsigdatavbf_.get(i)).getRealValue("CMS_hgg_mass")
      val_b = 1.01
      val_w = self.bdtsigdatavbf_.weight()
      if val_m > mHL and val_m < mHH and val_b>=0.05: returnList.append((val_b,((val_m-mH)/mH),val_w))

    c = r.TCanvas()
    test.Draw()
    c.SaveAs("test.pdf")

    if self.is2011_:return returnList

    for i in range(self.bdtsigdatavbftight_.numEntries()):
      val_m = (self.bdtsigdatavbftight_.get(i)).getRealValue("CMS_hgg_mass")
      val_b = 1.05
      val_w = self.bdtsigdatavbftight_.weight()
      if val_m > mHL and val_m < mHH and val_b >=bdtcut: returnList.append((val_b,((val_m-mH)/mH),val_w))

    
    return returnList    

  def plotData(self,path,nbinsMass,nbinsBDT):
    
    if not os.path.isdir(path+"/PlotsForToys"):
      os.makedirs(path+"/PlotsForToys")
    if not os.path.isdir(path+"/PlotsForToys/data"):
      os.makedirs(path+"/PlotsForToys/data")
    can = r.TCanvas()

    self.bdtvar_.setRange("allcats",0.05,1.);
    
    frame1 = self.bdtvar_.frame()
    frame1.SetTitle("BDT output in data")
    self.bdtdata_.plotOn(frame1,r.RooFit.Binning(nbinsBDT))
    self.bdtpdf_.plotOn(frame1,r.RooFit.NormRange("allcats"))
    frame1.Draw()
    can.SaveAs(path+"/PlotsForToys/data/data_bdt.pdf")
   
    self.bdtvar_.setRange(0.05,1.)
    frame2 = self.bdtvar_.frame()
    frame2.SetTitle("BDT output in data")
    self.bdtdata_.plotOn(frame2,r.RooFit.Binning(nbinsBDT))
    self.bdtpdf_.plotOn(frame2,r.RooFit.NormRange("allcats"))
    self.bdtdatavbf_.plotOn(frame2,r.RooFit.Binning(nbinsBDT),r.RooFit.MarkerColor(4),r.RooFit.LineColor(4))
    frame2.Draw()
    can.SaveAs(path+"/PlotsForToys/data/data_bdt_zoom.pdf")
    
    frame3 = self.bdtmgg_.frame()
    self.bdtdata_.plotOn(frame3,r.RooFit.Binning(nbinsMass))
    self.icpdf_.plotOn(frame3)
    frame3.SetTitle("Mass in data for bdtoutput>=0.05")
    frame3.Draw()
    can.SaveAs(path+"/PlotsForToys/data/data_mass.pdf")

  def plotToy(self,path,nbinsMass,nbinsBDT,expSig=0):
    
    if not os.path.isdir(path+"/PlotsForToys/toy/bkg"):
      os.makedirs(path+"/PlotsForToys/toy/bkg")
    if not os.path.isdir(path+"/PlotsForToys/toy/sig"):
      os.makedirs(path+"/PlotsForToys/toy/sig")
    if not os.path.isdir(path+"/PlotsForToys/toy/sandb"):
      os.makedirs(path+"/PlotsForToys/toy/sandb")
    
    # have following datasets:
    # BKG: genbkgmassdata_[5] (mass)
    #      genbkgbdtdata_[4]  (BDT)
    # SIG: gensigbdtdata_[4]  (mass,BDT)
    #      gensigmassdatavbf_ (mass)
    # S+B: genmassdata_[5]    (mass)
    #      genbdtdatacats_[4] (BDT)

    self.bdtvar_.setRange(0.05,1.)
    can = r.TCanvas()
    # bkg only toy first
    for i,dset in enumerate(self.genbkgbdtdata_):
      frame = self.bdtvar_.frame()
      frame.SetTitle("BDT output toy cat %d - background"%i)
      dset.plotOn(frame,r.RooFit.Binning(nbinsBDT))
      self.bdtpdf_.plotOn(frame,r.RooFit.NormRange("cat%d"%i))
      frame.Draw()
      can.SaveAs("%s/PlotsForToys/toy/bkg/toy_bkg_bdt_cat%d.pdf"%(path,i))
      if i==0: self.genbkgbdtcomb_ = r.RooDataSet("gen_bkg_bdt_comb","gen_bkg_bdt_comb",dset,r.RooArgSet(self.bdtvar_))
      else: self.genbkgbdtcomb_.append(dset)

    frame1 = self.bdtvar_.frame()
    frame1.SetTitle("BDT output toy no VBF - background")
    self.genbkgbdtcomb_.plotOn(frame1,r.RooFit.Binning(nbinsBDT))
    self.bdtpdf_.plotOn(frame1,r.RooFit.Range(0.05,1.),r.RooFit.NormRange("allcats"))
    frame1.Draw()
    can.SaveAs(path+"/PlotsForToys/toy/bkg/toy_bkg_bdt_novbf.pdf")

    for i,dset in enumerate(self.genbkgmassdata_):
      frame = self.bdtmgg_.frame()
      frame.SetTitle("Mass toy cat %d - background"%i)
      dset.plotOn(frame,r.RooFit.Binning(nbinsMass))
      self.mitpdfs_[i].plotOn(frame)
      frame.Draw()
      can.SaveAs("%s/PlotsForToys/toy/bkg/toy_bkg_mass_cat%d.pdf"%(path,i))
      if i==0:
        self.genbkgmasscombnovbf_ = r.RooDataSet("gen_bkg_mass_comb_novbf","gen_bkg_mass_comb_novbf",dset,r.RooArgSet(self.bdtmgg_))
        self.genbkgmasscomb_ = r.RooDataSet("gen_bkg_mass_comb","gen_bkg_mass_comb",dset,r.RooArgSet(self.bdtmgg_))
      elif i==4:
        self.genbkgmasscomb_.append(dset)
      else:
        self.genbkgmasscombnovbf_.append(dset)
        self.genbkgmasscomb_.append(dset)

    frame2 = self.bdtmgg_.frame()
    frame2.SetTitle("Mass toy no VBF - background")
    self.genbkgmasscombnovbf_.plotOn(frame2,r.RooFit.Binning(nbinsMass))
    self.icpdf_.plotOn(frame2,r.RooFit.LineColor(r.kMagenta))
    frame2.Draw()
    can.SaveAs(path+"/PlotsForToys/toy/bkg/toy_bkg_mass_novbf.pdf")

    frame3 = self.bdtmgg_.frame()
    frame3.SetTitle("Mass toy all combined - background")
    self.genbkgmasscomb_.plotOn(frame3,r.RooFit.Binning(nbinsMass))
    self.icpdf_.plotOn(frame3,r.RooFit.LineColor(r.kMagenta))
    frame3.Draw()
    can.SaveAs(path+"/PlotsForToys/toy/bkg/toy_bkg_mass_all.pdf")

    # have following datasets:
    # BKG: genbkgmassdata_[5] (mass)
    #      genbkgbdtdata_[4]  (BDT)
    # SIG: gensigbdtdata_[4]  (mass,BDT)
    #      gensigmassdatavbf_ (mass)
    # S+B: genmassdata_[5]    (mass)
    #      genbdtdatacats_[4] (BDT)

    if expSig>0:
      # sig toy second
      for i,dset in enumerate(self.gensigbdtdata_):
        # bdt
        frame = self.bdtvar_.frame()
        frame.SetTitle("BDT output toy cat %d - signal (%d x SM)"%(i,expSig))
        dset.plotOn(frame,r.RooFit.Binning(nbinsBDT))
        self.bdtsigpdf_.plotOn(frame,r.RooFit.NormRange("cat%d"%i))
        frame.Draw()
        can.SaveAs("%s/PlotsForToys/toy/sig/toy_sig_bdt_cat%d.pdf"%(path,i))
        # mass
        frame = self.bdtmgg_.frame()
        frame.SetTitle("Mass toy cat %d - signal (%d x SM) "%(i,expSig))
        dset.plotOn(frame,r.RooFit.Binning(nbinsMass))
        self.mitsigpdfs_[i].plotOn(frame,r.RooFit.LineColor(r.kMagenta))
        frame.Draw()
        can.SaveAs("%s/PlotsForToys/toy/sig/toy_sig_mass_cat%d.pdf"%(path,i))
        if i==0: self.gensigcombnovbf_ = r.RooDataSet("gen_sig_mass_comb_novbf","gen_sig_mass_comb_novbf",dset,r.RooArgSet(self.bdtvar_,self.bdtmgg_))
        else: self.gensigcombnovbf_.append(dset)
      
      # plot sig mass in vbf
      frame4 = self.bdtmgg_.frame()
      frame4.SetTitle("Mass toy cat 4 - signal (%d x SM)"%expSig)
      self.gensigmassdatavbf_.plotOn(frame4,r.RooFit.Binning(nbinsMass))
      self.mitsigpdfs_[4].plotOn(frame4)
      frame4.Draw()
      can.SaveAs("%s/PlotsForToys/toy/sig/toy_sig_mass_cat4.pdf"%path)

      # plot bdt comb
      frame5 = self.bdtvar_.frame()
      frame5.SetTitle("BDT output toy no VBF - signal (%d x SM)"%expSig)
      self.gensigcombnovbf_.plotOn(frame5,r.RooFit.Binning(nbinsBDT))
      self.bdtsigpdf_.plotOn(frame5)
      frame5.Draw()
      can.SaveAs(path+"/PlotsForToys/toy/sig/toy_sig_bdt_novbf.pdf")

      # plot mass comb no vbf
      frame6 = self.bdtmgg_.frame()
      frame6.SetTitle("Mass toy no VBF - signal (%d x SM)"%expSig)
      self.gensigcombnovbf_.plotOn(frame6,r.RooFit.Binning(nbinsMass))
      self.bdtsigpdf_.plotOn(frame6)
      frame6.Draw()
      can.SaveAs(path+"/PlotsForToys/toy/sig/toy_sig_mass_novbf.pdf")

      # plot mass comb all
      self.gensigmasscomb_ = r.RooDataSet("gen_sig_mass_comb","gen_sig_mass_comb",self.gensigcombnovbf_,r.RooArgSet(self.bdtmgg_))
      self.gensigmasscomb_.append(self.gensigmassdatavbf_)
      frame7 = self.bdtmgg_.frame()
      frame7.SetTitle("Mass toy all combined - signal (%d x SM)"%expSig)
      self.gensigmasscomb_.plotOn(frame7,r.RooFit.Binning(nbinsMass))
      self.bdtsigpdf_.plotOn(frame7)
      frame7.Draw()
      can.SaveAs(path+"/PlotsForToys/toy/sig/toy_sig_mass_all.pdf")

    # have following datasets:
    # BKG: genbkgmassdata_[5] (mass)
    #      genbkgbdtdata_[4]  (BDT)
    # SIG: gensigbdtdata_[4]  (mass,BDT)
    #      gensigmassdatavbf_ (mass)
    # S+B: genmassdata_[5]    (mass)
    #      genbdtdatacats_[4] (BDT)

    # comb toy last
    for i,dset in enumerate(self.genbdtdatacats_):
      # bdt
      frame = self.bdtvar_.frame()
      frame.SetTitle("BDT output toy cat %d - s+b"%i)
      dset.plotOn(frame,r.RooFit.Binning(nbinsBDT))
      self.bdtpdf_.plotOn(frame,r.RooFit.Normalization(self.genbkgbdtdata_[i].sumEntries(),r.RooAbsReal.NumEvent),r.RooFit.NormRange("cat%d"%i))
      frame.Draw()
      can.SaveAs("%s/PlotsForToys/toy/sandb/toy_sandb_bdt_cat%d.pdf"%(path,i))
      if i==0: self.genallbdtcomb_ = r.RooDataSet("gen_sandb_bdt_comb","gen_sandb_bdt_comb",dset,r.RooArgSet(self.bdtvar_))
      else: self.genallbdtcomb_.append(dset)

    frame8 = self.bdtvar_.frame()
    frame8.SetTitle("BDT output toy no VBF - s+b")
    self.genallbdtcomb_.plotOn(frame8,r.RooFit.Binning(nbinsBDT))
    self.bdtpdf_.plotOn(frame8,r.RooFit.Normalization(self.genbkgbdtdata_[0].sumEntries()+self.genbkgbdtdata_[1].sumEntries()+self.genbkgbdtdata_[2].sumEntries()+self.genbkgbdtdata_[3].sumEntries(),r.RooAbsReal.NumEvent))
    frame8.Draw()
    can.SaveAs(path+"/PlotsForToys/toy/sandb/toy_sandb_bdt_novbf.pdf")

    for i,dset in enumerate(self.genmassdata_):
      # mass
      frame = self.bdtmgg_.frame()
      frame.SetTitle("Mass toy cat %d - s+b"%i)
      dset.plotOn(frame,r.RooFit.Binning(nbinsMass))
      self.mitpdfs_[i].plotOn(frame,r.RooFit.Normalization(self.genbkgmassdata_[i].sumEntries(),r.RooAbsReal.NumEvent))
      frame.Draw()
      can.SaveAs("%s/PlotsForToys/toy/sandb/toy_sandb_mass_cat%d.pdf"%(path,i))
      if i==0:
        self.genallmasscombnovbf_ = r.RooDataSet("gen_sandb_mass_comb_novbf","gen_sandb_mass_comb_novbf",dset,r.RooArgSet(self.bdtmgg_))
        self.genallmasscomb_ = r.RooDataSet("gen_sandb_mass_comb","gen_sandb_mass_comb",dset,r.RooArgSet(self.bdtmgg_))
      elif i==4:
        self.genallmasscomb_.append(dset)
      else:
        self.genallmasscombnovbf_.append(dset)
        self.genallmasscomb_.append(dset)

    frame9 = self.bdtmgg_.frame()
    frame9.SetTitle("Mass toy no VBF - s+b")
    self.genallmasscombnovbf_.plotOn(frame9,r.RooFit.Binning(nbinsMass))
    self.icpdf_.plotOn(frame9,r.RooFit.Normalization(self.genbkgmasscombnovbf_.sumEntries(),r.RooAbsReal.NumEvent),r.RooFit.LineColor(r.kMagenta))
    frame9.Draw()
    can.SaveAs(path+"/PlotsForToys/toy/sandb/toy_sandb_mass_novbf.pdf")

    frame10 = self.bdtmgg_.frame()
    frame10.SetTitle("Mass toy all combined - s+b")
    self.genallmasscomb_.plotOn(frame10,r.RooFit.Binning(nbinsMass))
    self.icpdf_.plotOn(frame10,r.RooFit.Normalization(self.genbkgmasscomb_.sumEntries(),r.RooAbsReal.NumEvent),r.RooFit.LineColor(r.kMagenta))
    frame10.Draw()
    can.SaveAs(path+"/PlotsForToys/toy/sandb/toy_sandb_mass_all.pdf")



      
"""
    frame1 = self.bdtvar_.frame()
    self.genbdtdata_.plotOn(frame1,r.RooFit.Binning(nbinsBDT))
    self.bdtpdf_.plotOn(frame1,r.RooFit.Range(0.05,1.))
    frame1.SetTitle("BDT output toy no VBF")
    frame1.Draw()
    can.SaveAs(path+"/PlotsForToys/toy/toy_bdt_novbf.pdf")

    for i, dset in enumerate(self.genbdtdatacats_):
      frame2 = self.bdtvar_.frame()
      dset.plotOn(frame2,r.RooFit.Binning(nbinsBDT))
      self.bdtpdf_.plotOn(frame2,r.RooFit.NormRange("cat%d"%i))
      frame2.SetTitle("BDT output toy cat %d"%i)
      frame2.Draw()
      can.SaveAs("%s/PlotsForToys/toy/toy_bdt_cat%d.pdf"%(path,i))

    # mass
    for i, dataset in enumerate(self.genmassdata_):
      frame3 = self.bdtmgg_.frame()
      frame3.SetTitle("Mass toy for category %i"%i)
      dataset.plotOn(frame3,r.RooFit.Binning(nbinsMass))
      self.mitpdfs_[i].plotOn(frame3)
      frame3.Draw()
      can.SaveAs(path+"/PlotsForToys/toy/toy_mass_cat%i.pdf"%i)

    frame4 = self.bdtmgg_.frame()
    frame4.SetTitle("Mass toy for all non VBF categories combined")
    self.allgenmassdatanovbf_.plotOn(frame4,r.RooFit.Binning(nbinsMass))
    self.icpdf_.plotOn(frame4,r.RooFit.Normalization(self.toyNoVBFEvents_,r.RooAbsReal.NumEvent),r.RooFit.LineColor(r.kMagenta))
    frame4.Draw()
    can.SaveAs(path+"/PlotsForToys/toy/toy_mass_comb_noVBF.pdf")

    frame5 = self.bdtmgg_.frame()
    frame5.SetTitle("Mass toy for all categories combined")
    self.allgenmassdata_.plotOn(frame5,r.RooFit.Binning(nbinsMass))
    self.icpdf_.plotOn(frame5,r.RooFit.Normalization(self.toyNoVBFEvents_+self.toyVBFEvents_,r.RooAbsReal.NumEvent),r.RooFit.LineColor(r.kMagenta))
    frame5.Draw()
    can.SaveAs(path+"/PlotsForToys/toy/toy_mass_comb.pdf")

    frame6 = self.bdtmgg_.frame()
    frame6.SetTitle("Mass toy for all non VBF categories in signal")
    self.gensigdatanovbf_.plotOn(frame6,r.RooFit.Binning(nbinsMass))
    self.bdtsigpdf_.plotOn(frame6)
    frame6.Draw()
    can.SaveAs(path+"/PlotsForToys/toy/toy_sig_mass_novbf.pdf")

    frame7 = self.bdtvar_.frame()
    frame7.SetTitle("BDT output toy for all non VBF cateogories in signal")
    self.gensigdatanovbf_.plotOn(frame7,r.RooFit.Binning(nbinsBDT))
    self.bdtsigpdf_.plotOn(frame7)
    frame7.Draw()
    can.SaveAs(path+"/PlotsForToys/toy/toy_sig_bdt_novbf.pdf")

    for i, dset in enumerate(self.gensigbdtdata_):
      dB = self.bdtvar_.frame()
      dB.SetTitle("BDT output toy cat %d signal"%i)
      dset.plotOn(dB)
      self.bdtsigpdf_.plotOn(dB,r.RooFit.NormRange("cat%d"%i))
      dB.Draw()
      can.SaveAs("%s/PlotsForToys/toy/toy_sig_bdt_cat%d.pdf"%(path,i))
      dM = self.bdtmgg_.frame()
      dM.SetTitle("Mass toy cat %d signal"%i)
      dset.plotOn(dM)
      self.mitsigpdfs_[i].plotOn(dM,r.RooFit.LineColor(r.RooFit.kMagenta))
      dM.Draw()
      can.SaveAs("%s/PlotsForToys/toy/toy_sig_mass_cat%d.pdf"%(path,i))

    frame8 = self.bdtmgg_.frame()
    frame8.SetTitle("Mass toy cat 4 signal")
    self.gensigmassdatavbf_.plotOn(frame8)
    self.mitsigpdfs_[4].plotOn(frame8)
    frame8.Draw()
    can.SaveAs("%s/PlotsForToys/toy/toy_sig_mass_cat4.pdf"%path)

    # mass

    #self.icr1_       = r.RooRealVar("r1","r1",-8.,-10.,0.)
    #self.icr2_       = r.RooRealVar("r2","r2",-0.05,-10.,0.)
    #self.icf1_       = r.RooRealVar("f1","f1",0.01,0.,1.)
    #self.icpdfmass_  = r.RooGenericPdf("data_pow_model","data_pow_model","(1-@3)*TMath::Power(@0,@1)+@3*TMath::Power(@0,@2)",r.RooArgList(self.mitvar_,self.icr1_,self.icr2_,self.icf1_));
"""

"""
  def createSigPdf(self,tree):
    if not self.hasFit_:
      sys.exit('There is no background PDF made or loaded yet. Can\'t create signal. Bailing out')

    # also need to get event weight for signal
    self.weight_ = r.RooRealVar("weight","weight",0,1)
    self.bdtsigdata_    = r.RooDataSet("sigdata_bdt","sigdata_bdt",r.RooArgSet(self.bdtvar_,self.var_,self.bdtvbf_,self.weight_),r.RooFit.Import(tree),r.RooFit.Cut("vbf==0"),"weight")
    self.bdtsigdatavbf_ = r.RooDataSet("sigdata_bdtVBF","sigdata_bdtVBF",r.RooArgSet(self.bdtvar_,self.var_,self.bdtvbf_,self.weight_),r.RooFit.Import(tree),r.RooFit.Cut("vbf==1"),"weight")
    self.bdtsigdatavbftight_ = r.RooDataSet("sigdata_bdtVBF_tight","sigdata_bdtVBF",r.RooArgSet(self.bdtvar_,self.var_,self.bdtvbf_,self.weight_),r.RooFit.Import(tree),r.RooFit.Cut("vbf==1"),"weight")

    self.sigNoVBFEvents_= self.bdtsigdata_.sumEntries("bdtoutput>=0.05")
    self.sigVBFEvents_  = self.bdtsigdatavbf_.sumEntries("bdtoutput>=0.05")
    self.var_.setBins(320)
    self.bdtvar_.setBins(200)
    self.bdtsighist_    = r.RooDataHist("sigbdthist","sigbdthist",r.RooArgSet(self.bdtvar_,self.var_),self.bdtsigdata_)
    self.bdtsigpdf_     = r.RooHistPdf("sigpdf","sigpdf",r.RooArgSet(self.bdtvar_,self.var_),self.bdtsighist_)

    print 'not vbf sig above -0.8 ', self.bdtsigdata_.sumEntries("bdtoutput>-0.8")
    print 'not vbf sig above 0.05 ', self.bdtsigdata_.sumEntries("bdtoutput>=0.05")
    print 'vbf sig above -0.8 ', self.bdtsigdatavbf_.sumEntries("bdtoutput>-0.8")
    print 'vbf sig above 0.05 ', self.bdtsigdatavbf_.sumEntries("bdtoutput>=0.05")
  
  def createKeysPdf(self,tree):
    self.bdtvar_      = r.RooRealVar("bdtoutput","bdtoutput",-1,1)
    self.bdtmgg_      = r.RooRealVar("mgg","mgg",100,180)
    self.bdtvbf_      = r.RooRealVar("vbf","vbf",0,1)
    self.dataforan_   = r.RooDataSet("foran","foran",r.RooArgSet(self.bdtvar_,self.bdtmgg_),r.RooFit.Import(tree),r.RooFit.Cut("bdtoutput>=0.05"))
    print 'all data above 0.05 ', self.dataforan_.sumEntries()
    
    self.bdtdata_     = r.RooDataSet("roodata_bdt","roodata_bdt",r.RooArgSet(self.bdtvar_,self.bdtmgg_,self.bdtvbf_),r.RooFit.Import(tree),r.RooFit.Cut("bdtoutput>-0.8 && vbf==0"))
    print 'not vbf data above -0.8  ', self.bdtdata_.sumEntries()
    print 'not vbf data above 0.05 ', self.bdtdata_.sumEntries("bdtoutput>=0.05")

    self.bdtdatavbf_  = r.RooDataSet("roodata_bdtVBF","roodata_bdtVBF",r.RooArgSet(self.bdtvar_,self.bdtmgg_,self.bdtvbf_),r.RooFit.Import(tree),r.RooFit.Cut("bdtoutput>-0.8 && vbf==1"))
    print 'vbf data above 0.8  ', self.bdtdatavbf_.sumEntries()
    print 'vbf data above 0.05 ', self.bdtdatavbf_.sumEntries("bdtoutput>=0.05")
    
    self.bdtvar_.setRange(-0.8,1.)
    self.bdtpdf_ = r.RooKeysPdf("bdtpdf","bdtpdf",self.bdtvar_,self.bdtdata_)

    self.dataNoVBFEvents_=self.bdtdata_.sumEntries("bdtoutput>=0.05")
    self.dataVBFEvents_=self.bdtdatavbf_.sumEntries("bdtoutput>=0.05")
    print self.dataNoVBFEvents_
    print self.dataVBFEvents_
    self.hasFit_      = True

"""

