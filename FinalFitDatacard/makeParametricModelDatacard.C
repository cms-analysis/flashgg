#include <iostream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCut.h"
#include "TH1D.h"
#include "TCanvas.h"

using namespace std;


void makeParametricModelDatacard(string infilename, string wsfilename="0", string outfilename="cms_hgg_datacard.txt", bool massfact=true, int mass=125, bool splitVH=false){

  TFile *inFile = TFile::Open(infilename.c_str());
  TFile *outFile = TFile::Open("DatacardTree.root","RECREATE");
  
  vector<TTree*> trees;
  trees.push_back((TTree*)inFile->Get(Form("datacard_trees/ggh_m%d_8TeV",mass)));
  trees.push_back((TTree*)inFile->Get(Form("datacard_trees/vbf_m%d_8TeV",mass)));
  trees.push_back((TTree*)inFile->Get(Form("datacard_trees/wzh_m%d_8TeV",mass)));
  trees.push_back((TTree*)inFile->Get(Form("datacard_trees/tth_m%d_8TeV",mass)));

  TList *listOfTrees = new TList();
  for (vector<TTree*>::iterator it=trees.begin(); it!=trees.end(); it++) listOfTrees->Add(*it);

  TTree *tree = TTree::MergeTrees(listOfTrees);
  tree->SetName(Form("AllSignal_m%d",mass)); 
  cout << "Merged trees.... " << endl;
  tree->Print();
  
  //float xsecs[50];
  //xsecs[0] = 19.83  *2.25e-03;
  //xsecs[1] = 1.573*2.25e-03;
  //xsecs[2] = (0.7154+0.4044)*2.25e-03;
  //xsecs[3] = 0.1334*2.25e-03;

  const double lumi = 19620.;  

  const double lumiuncert = 1.044;
  const double triguncert = 1.01;
  const double vtxuncert = 0.002;

  const int ncats=9;
  const int nInclusiveCats=4;
  const int nVBFCats=2;
  const int nExclusiveCats=5;
  const int muTag=nInclusiveCats+nVBFCats;
  const int eleTag=nInclusiveCats+nVBFCats+1;
  const int metTag=nInclusiveCats+nVBFCats+2;

  //string pathToDatBkgWS="/afs/cern.ch/work/m/mkenzie/private/h2g/latest_workspaces/CMS-HGG_massfacmva_hcpPreapproval.root";
  //string pathToSigWS="/afs/cern.ch/work/m/mkenzie/private/h2g/latest_workspaces/SigMod_massfacmva_hcpPreapproval.root";
  string pathToDatBkgWS="CMS-HGG.root";
  string pathToSigWS="CMS-HGG_sigfit.root";

  // define various cuts

  // category cuts
  vector<TCut> catcuts;
  vector<string> catnames;
  for (int cat=0; cat<ncats; cat++) {
    //catcuts.push_back(TCut(Form("category==%d",cat)));
    catnames.push_back(Form("cat%d",cat));
  }
 
  TCut vbfloose("(category==5)");
  TCut vbftight("(category==4)");
  TCut allvbfcut("category==4 || category==5");
  //TCut vbfloose("(bdtmod>=(-0.05) && vbfmva>=0.93 && vbfmva<0.985)");
  //TCut vbftight("(bdtmod>=(-0.05) && vbfmva>=0.985)");
  //TCut allvbfcut("(bdtmod>=(-0.05) && vbfmva>=0.93)");
  TCut muon("(category==6)");
  TCut ele("(category==7)");
  TCut met("(category==8)");

//   catcuts.push_back(TCut("(bdtmod>=0.91)"*!allvbfcut));
//   catcuts.push_back(TCut("(bdtmod>=0.79 && bdtmod<0.91)"*!allvbfcut));
//   catcuts.push_back(TCut("(bdtmod>=0.49 && bdtmod<0.79)"*!allvbfcut));
//   catcuts.push_back(TCut("(bdtmod>=(-0.05) && bdtmod<0.49)"*!allvbfcut));
  catcuts.push_back(TCut("(category==0)"));
  catcuts.push_back(TCut("(category==1)"));
  catcuts.push_back(TCut("(category==2)"));
  catcuts.push_back(TCut("(category==3)"));
  catcuts.push_back(vbftight);
  catcuts.push_back(vbfloose);
  catcuts.push_back(muon);
  catcuts.push_back(ele);
  catcuts.push_back(met);

  // inclusive category boundary cuts
  vector<TCut> inccats;
  if (massfact){
    inccats.push_back(TCut("(bdtmod>=0.91)"));
    inccats.push_back(TCut("(bdtmod>=0.79 && bdtmod<0.91)"));
    inccats.push_back(TCut("(bdtmod>=0.49 && bdtmod<0.79)"));
    inccats.push_back(TCut("(bdtmod>=(-0.05) && bdtmod<0.49)"));
  }
  else {
    inccats.push_back(TCut("(inc_cat==0)"));
    inccats.push_back(TCut("(inc_cat==1)"));
    inccats.push_back(TCut("(inc_cat==2)"));
    inccats.push_back(TCut("(inc_cat==3)"));
  }

  // start by setting bdtmod as alias for bdtout
  // this can change later when bdtmod becomes bdtout_id_shift etc.
  tree->SetAlias("bdtmod","bdtout");

  // process cuts
  vector<TCut> proccuts;
  proccuts.push_back(TCut("(process_id==0)"));
  proccuts.push_back(TCut("(process_id==1)"));
  if (splitVH) {
    proccuts.push_back(TCut("(process_id==2)"));
    proccuts.push_back(TCut("(process_id==3)"));
  }
  else {
    proccuts.push_back(TCut("(process_id==5)"));
  }
  proccuts.push_back(TCut("(process_id==4)"));

  // process names
  vector<string> procnames;
  procnames.push_back("ggH");
  procnames.push_back("qqH");
  if (splitVH) {
    procnames.push_back("WH");
    procnames.push_back("ZH");
  }
  else {
    procnames.push_back("VH");
  }
  procnames.push_back("ttH");
  procnames.push_back("bkg_mass");
  //FILE *file = fopen(outfilename.c_str(),"w");
  FILE *file = fopen("temp_datacard.txt","w");

  // process names in file
  vector<string> procfilenames;
  procfilenames.push_back("ggh");
  procfilenames.push_back("vbf");
  if (splitVH) {
    procfilenames.push_back("wh");
    procfilenames.push_back("zh");
  }
  else {
    procfilenames.push_back("wzh");
  }
  procfilenames.push_back("tth");
  procfilenames.push_back("bkg_mass");
  
  printf("intro\n");
  
  //introductory stuff
  fprintf(file,"CMS-HGG DataCard for Unbinned Limit Setting\n");
  fprintf(file,"Run with: combine\n");
  fprintf(file,"---------------------------------------------\n");
  fprintf(file, "imax %i\n", ncats);
  fprintf(file, "jmax *\n");
  fprintf(file, "kmax *\n");
  fprintf(file,"---------------------------------------------\n");    
  
  fprintf(file,"\n\n\n\n");

  printf("done intro\n");
  
  //data
  fprintf(file, "shapes data_obs * %s cms_hgg_workspace:roohist_data_mass_$CHANNEL\n", pathToDatBkgWS.c_str());
  
  //bkg pdfs
  fprintf(file, "shapes bkg_mass * %s cms_hgg_workspace:pdf_data_pol_model_8TeV_$CHANNEL\n", pathToDatBkgWS.c_str());
  
  //signal pdfs
  fprintf(file, "shapes ggH * %s wsig_8TeV:hggpdfsmrel_ggh_$CHANNEL\n", pathToSigWS.c_str());
  fprintf(file, "shapes qqH * %s wsig_8TeV:hggpdfsmrel_vbf_$CHANNEL\n", pathToSigWS.c_str());
  if (splitVH) {
    fprintf(file, "shapes WH  * %s wsig_8TeV:hggpdfsmrel_wh_$CHANNEL\n", pathToSigWS.c_str());
    fprintf(file, "shapes ZH  * %s wsig_8TeV:hggpdfsmrel_zh_$CHANNEL\n", pathToSigWS.c_str());
  }
  else {
    fprintf(file, "shapes VH  * %s wsig_8TeV:hggpdfsmrel_wzh_$CHANNEL\n", pathToSigWS.c_str());
  }
  fprintf(file, "shapes ttH * %s wsig_8TeV:hggpdfsmrel_tth_$CHANNEL", pathToSigWS.c_str());

  fprintf(file,"\n\n\n\n");

  //bin and observation lines
  fprintf(file, "bin         ");
  for (int icat=0; icat<ncats; ++icat) {
    fprintf(file, "%s ",catnames.at(icat).c_str());
  }
  fprintf(file,"\n");
  
  fprintf(file, "observation ");
  for (int icat=0; icat<ncats; ++icat) {
    fprintf(file, "-1 ");
  }    
  fprintf(file,"\n");
  
  //rates
  fprintf(file,"bin     ");
  for (int icat=0; icat<ncats; ++icat) {
    for (unsigned int iproc=0; iproc<procnames.size(); ++iproc) {
      fprintf(file,"%s ", catnames.at(icat).c_str());
    }
  }
  fprintf(file,"\n");

  fprintf(file,"process ");
  for (int icat=0; icat<ncats; ++icat) {
    for (unsigned int iproc=0; iproc<procnames.size(); ++iproc) {
      fprintf(file,"%s ", procnames.at(iproc).c_str());
    }
  }
  fprintf(file,"\n");    

  fprintf(file,"process ");
  for (int icat=0; icat<ncats; ++icat) {
    for (unsigned int iproc=0; iproc<procnames.size(); ++iproc) {
      int idx;
      if (iproc==(procnames.size()-1)) idx = 1;
      else idx = -iproc;
      fprintf(file, "%i ", idx);
    }
  }
  fprintf(file,"\n");    
  
  fprintf(file,"rate ");
  for (int icat=0; icat<ncats; ++icat) {
    for (unsigned int iproc=0; iproc<procnames.size(); ++iproc) {
      float rate;
      if (iproc==(procnames.size()-1)) rate = 1.0;
      else rate = lumi;
      fprintf(file, "%3.1f ", rate);
    }
  }
  fprintf(file,"\n");      

  fprintf(file,"\n\n\n\n");

  printf("done rates\n");

  //theory systematics (overall normalization)
  fprintf(file, "QCDscale_ggH         lnN  ");
  for (int icat=0; icat<ncats; ++icat) {
    if (splitVH) fprintf(file, "0.918/1.076 - - - - - ");
    else fprintf(file, "0.918/1.076 - - - - ");
  }
  fprintf(file,"\n");      
  
  fprintf(file, "pdf_gg               lnN  ");
  for (int icat=0; icat<ncats; ++icat) {
    if (splitVH) fprintf(file, "0.930/1.076 - - - 0.920/1.080 - ");
    else fprintf(file, "0.930/1.076 - - 0.920/1.080 - ");
  }
  fprintf(file,"\n");      
  
  fprintf(file, "QCDscale_qqH         lnN  ");
  for (int icat=0; icat<ncats; ++icat) {
    if (splitVH) fprintf(file, "- 0.992/1.003 - - - - ");
    else fprintf(file, "- 0.992/1.003 - - - ");
  }
  fprintf(file,"\n");      
  
  fprintf(file, "pdf_qqbar            lnN  ");
  for (int icat=0; icat<ncats; ++icat) {
    if (splitVH) fprintf(file, "- 0.972/1.026 0.958/1.042 0.958/1.042 - - ");
    else fprintf(file, "- 0.972/1.026 0.958/1.042 - - ");
  }
  fprintf(file,"\n");      

  fprintf(file, "QCDscale_VH          lnN  ");
  for (int icat=0; icat<ncats; ++icat) {
    if (splitVH) fprintf(file, "- - 0.982/1.021 0.982/1.021 - - ");
    else fprintf(file, "- - 0.982/1.021 - - ");
  }
  fprintf(file,"\n");      
  
  fprintf(file, "QCDscale_ttH         lnN  ");
  for (int icat=0; icat<ncats; ++icat) {
    if (splitVH) fprintf(file, "- - - - 0.906/1.041 - ");
    else fprintf(file, "- - - 0.906/1.041 - ");
  }
  fprintf(file,"\n");      
  
  fprintf(file,"\n\n\n\n");

  //lumi uncertainty
  fprintf(file,"lumi_8TeV                  lnN  ");
  for (int icat=0; icat<ncats; ++icat) {
    for (unsigned int iproc=0; iproc<procnames.size(); ++iproc) {
      if (iproc!=(procnames.size()-1)) fprintf(file, "%3f ", lumiuncert);
      else fprintf(file, "- ");      
    }
  }
  fprintf(file,"\n");     
  
  //trigger efficiency uncertainty
  fprintf(file,"CMS_hgg_eff_trig lnN  ");
  for (int icat=0; icat<ncats; ++icat) {
    for (unsigned int iproc=0; iproc<procnames.size(); ++iproc) {
      //float rate;
      if (iproc!=(procnames.size()-1)) fprintf(file, "%3f ", triguncert);
      else fprintf(file, "- ");      
    }
  }
  fprintf(file,"\n");   
 
  // photon efficiency uncertainties
  vector<double> deffsinglesr;
  deffsinglesr.push_back(1.010);
  deffsinglesr.push_back(1.026);

  vector<TCut> singlecutsr;
  TCut singlecut0 = "( (lead_isEB || sublead_isEB) && !(lead_isEB && sublead_isEB) )";
  TCut singlecut1 = "( (!lead_isEB || !sublead_isEB) && !(!lead_isEB && !sublead_isEB) )";
  singlecutsr.push_back(singlecut0);
  singlecutsr.push_back(singlecut1);

  vector<TCut> doublecutsr;
  TCut doublecut0 = "(lead_isEB && sublead_isEB)";
  TCut doublecut1 = "(!lead_isEB && !sublead_isEB)";
  doublecutsr.push_back(doublecut0);
  doublecutsr.push_back(doublecut1);
  
  printf("starting 2eff loop\n");

  TH1D *hcount = new TH1D("hcount","",1,0.5,1.5);
    
  for (unsigned int ieff=0; ieff<deffsinglesr.size(); ++ieff) {
    std::vector<double> catuncerts;
    double uncert = deffsinglesr.at(ieff);
    TCut singlecut = singlecutsr.at(ieff);
    TCut doublecut = doublecutsr.at(ieff);      
    if (ieff==0) fprintf(file, "CMS_id_eff_eb      lnN  ");
    else if (ieff==1) fprintf(file, "CMS_id_eff_ee      lnN  ");
    else cout << "wtf?" << endl;

    for (int icat=0; icat<ncats; ++icat) {
      for (unsigned int iproc=0; iproc<(procnames.size()-1); ++iproc) {
        
        TCut catproccut = catcuts.at(icat)*proccuts.at(iproc);
        TCut catprocsingle = catproccut*singlecut;
        TCut catprocdouble = catproccut*doublecut;
        
        printf("j = %i, icat = %i, k = %i\n",ieff,icat,iproc);
       
        // tot events in this cat for this process
        tree->Draw("1>>hcount",catproccut,"goff");
        double ntot = hcount->GetSumOfWeights();
        hcount->Reset();
        printf("ntot = %5f\n",ntot);
        
        // events passing single photon cut
        tree->Draw("1>>hcount",catprocsingle,"goff");
        double singlefrac = hcount->GetSumOfWeights()/ntot;
        hcount->Reset();          
        printf("singlefrac = %5f\n",singlefrac);
        
        // events passing double photon cut
        tree->Draw("1>>hcount",catprocdouble,"goff");
        double doublefrac = hcount->GetSumOfWeights()/ntot;
        hcount->Reset();          
        printf("doublefrac = %5f\n",doublefrac);
	
        double catuncert = 1.0 + singlefrac*(uncert-1.0) + doublefrac*(uncert*uncert-1.0);
        fprintf(file, "%3f ",catuncert);
      }
      fprintf(file, "- ");
    }
    fprintf(file, "\n");
  }
  fprintf(file,"\n\n\n\n");
  printf("ending 2eff loop\n");

  //vbf uncertainties
  std::vector<std::string> vbfuncertnames;
  std::vector<double> vbfgguncerts;
  std::vector<double> vbfuncerts;
  
  vbfuncertnames.push_back("JEC");
  if ( massfact ){
    vbfuncerts.push_back(1.035);
    vbfgguncerts.push_back(1.11);
  }
  else{
    vbfuncerts.push_back(1.025);
    vbfgguncerts.push_back(1.10);
  }

  vbfuncertnames.push_back("UEPS");
  if ( massfact ){
    vbfuncerts.push_back(1.08);
    vbfgguncerts.push_back(1.26);
  }
  else{
    vbfuncerts.push_back(1.072);
    vbfgguncerts.push_back(1.28);
  }

  vbfuncertnames.push_back("CMS_eff_j");
  vbfuncerts.push_back(1.02);
  vbfgguncerts.push_back(1.02);
  

  printf("vbf uncerts\n");
  for (unsigned int j=0; j<vbfuncerts.size(); ++j) {
    double vbfuncert = vbfuncerts.at(j);
    double gguncert = vbfgguncerts.at(j);      
    fprintf(file, "%s      lnN  ",vbfuncertnames.at(j).c_str());
    // inclusive cats
    for (int icat=0; icat<(ncats-nExclusiveCats); ++icat) {
      for (unsigned int k=0; k<(procnames.size()-1); ++k) {
        double uncert;
        if (k==1) uncert = vbfuncert;
        else uncert = gguncert;
        printf("first count\n");
        TCut catproccut = catcuts.at(icat)*proccuts.at(k);
        TCut incproccut = inccats.at(icat)*proccuts.at(k);
        
        tree->Draw("1>>hcount",catproccut,"goff");
        double ntot = hcount->GetSumOfWeights();
        hcount->Reset();
        
        printf("second count icat = %i, k = %i\n",icat,k);
        //double ntagged = counttree(tree,allvbfcut*inccats.at(icat)*proccuts[k]);
        
        tree->Draw("1>>hcount",allvbfcut*incproccut,"goff");
        double ntagged = hcount->GetSumOfWeights();
        hcount->Reset();
        
        //double ntagged = counttree(tree,allvbfcut*incproccut);
        double catuncert = (ntot - (uncert-1.0)*ntagged)/ntot;
        fprintf(file, "%3f ",catuncert);
      }
      //printf("uncert = %5f,  ntot = %5f, singlefrac = %5f, doublefrac = %5f\n",uncert,ntot,singlefrac,doublefrac);
      //printf("%3f %3f %3f %3f - ",catuncert,catuncert,catuncert,catuncert);
      fprintf(file, "- ");
    }
    // vbf categories
    if (splitVH) {
      fprintf(file, "%3f %3f %3f %3f %3f - ",gguncert,vbfuncert,gguncert,gguncert,gguncert);
      fprintf(file, "%3f %3f %3f %3f %3f - ",gguncert,vbfuncert,gguncert,gguncert,gguncert);
    }
    else {
      fprintf(file, "%3f %3f %3f %3f - ",gguncert,vbfuncert,gguncert,gguncert);
      fprintf(file, "%3f %3f %3f %3f - ",gguncert,vbfuncert,gguncert,gguncert);
    }
    //printf("%3f %3f - ",vbfuncert,gguncert);
    // lepton and met categories
    if (splitVH) {
      fprintf(file, "- - - - - - ");
      fprintf(file, "- - - - - - ");
      fprintf(file, "- - - - - - ");
    }
    else {
      fprintf(file, "- - - - - ");
      fprintf(file, "- - - - - ");
      fprintf(file, "- - - - - ");
    }

    fprintf(file, "\n");
    
  }

  std::vector<std::string> vbfmiguncertnames;
  std::vector<double> vbfmiguncerts;
  std::vector<double> vbfmiggguncerts;
  
  // N.B. need to pass the uncertainties on the LOOSE dijet category here!

  vbfmiguncertnames.push_back("CMS_hgg_JECmigration");
  if ( massfact ){
    vbfmiguncerts.push_back(1.005);
    vbfmiggguncerts.push_back(1.025);
  }
  else{
     vbfmiguncerts.push_back(1.024);
     vbfmiggguncerts.push_back(1.005);
  }

  vbfmiguncertnames.push_back("CMS_hgg_UEPSmigration");
  if ( massfact ){
    vbfmiguncerts.push_back(1.01);
    vbfmiggguncerts.push_back(1.045);
  }
  else{
    vbfmiguncerts.push_back(1.034);
    vbfmiggguncerts.push_back(1.063);
  }

  printf("vbf migration uncerts\n");
  for (unsigned int j=0; j<vbfmiguncerts.size(); ++j) {
    fprintf(file, "%s      lnN  ",vbfmiguncertnames.at(j).c_str());          
    
    // inclusive cats
    for (int icat=0; icat<(ncats-nExclusiveCats); ++icat) {
      if (splitVH) fprintf(file, "- - - - - - ");
      else fprintf(file, "- - - - - ");
    }

    double vbfuncert = vbfmiguncerts.at(j);
    double gguncert = vbfmiggguncerts.at(j);      
 
    // vbf cats
    for (unsigned int k=0; k<proccuts.size(); ++k) {
      double uncert;
      if (k==1) uncert = vbfuncert;
      else uncert = gguncert;
  
      TCut looseproccut = catcuts.at(ncats-nExclusiveCats+1)*proccuts.at(k);
      TCut tightproccut = catcuts.at(ncats-nExclusiveCats)*proccuts.at(k);

      tree->Draw("1>>hcount",looseproccut,"goff");
      double nloose = hcount->GetSumOfWeights();
      hcount->Reset();

      tree->Draw("1>>hcount",tightproccut,"goff");
      double ntight = hcount->GetSumOfWeights();
      hcount->Reset();
        
      
      
//         double nloose = counttree(tree,looseproccut);
//         double ntight = counttree(tree,tightproccut);
      
      double nmigr = (uncert-1.0)*nloose;
      double antiuncert = (ntight-nmigr)/ntight;

      fprintf(file,"%3f ",antiuncert);
    }
    fprintf(file, "- ");
    for (unsigned int k=0; k<(procnames.size()-1); ++k) {
      double uncert;
      if (k==1) uncert = vbfuncert;
      else uncert = gguncert;
      
      fprintf(file,"%3f ",uncert);
    }
    fprintf(file, "- ");
    
    // lepton and met categories
    for (int cat=nInclusiveCats+nVBFCats; cat<ncats; cat++){
      if (splitVH) fprintf(file, "- - - - - - ");
      else fprintf(file, "- - - - - ");
    }

    fprintf(file,"\n\n");

  }
  
  fprintf(file, "CMS_hgg_eff_e         lnN  ");
  for (int icat=0; icat<ncats; ++icat) {
    if (splitVH) {
      if (icat==eleTag) fprintf(file, "- - 1.010 1.010 1.010 - ");
      else fprintf(file, "- - - - - - ");
    }
    else {
      if (icat==eleTag) fprintf(file, "- - 1.010 1.010 - ");
      else fprintf(file, "- - - - - ");
    }
  }
  fprintf(file,"\n");      

  fprintf(file, "CMS_hgg_eff_m         lnN  ");
  for (int icat=0; icat<ncats; ++icat) {
    if (splitVH) {
      if (icat==muTag) fprintf(file, "- - 1.010 1.010 1.010 - ");
      else fprintf(file, "- - - - - - ");
    }
    else {
      if (icat==muTag) fprintf(file, "- - 1.010 1.010 - ");
      else fprintf(file, "- - - - - ");
    }
  }
  fprintf(file,"\n");      
  
  fprintf(file, "CMS_hgg_eff_MET        lnN  ");
  for (int icat=0; icat<ncats; ++icat) {
    if (splitVH){
      if (icat==metTag) fprintf(file, "1.15 1.15 1.04 1.04 1.04 - ");
      else fprintf(file, "- - - - - - ");
    }
    else {
      if (icat==metTag) fprintf(file, "1.15 1.15 1.04 1.04 - ");
      else fprintf(file, "- - - - - ");
    }
  }
  fprintf(file,"\n");      

  //return;
  //vertex selection fraction uncertainty
  fprintf(file, "CMS_hgg_nuissancedeltafracright param 1.0 %3f\n", vtxuncert);
  fprintf(file,"\n"); 
  //energy scale/smearing single photon categories
  //tracked event by event in globe

  float scale_err_;
  float smear_err_;
  float weight_;
  int category_;
  tree->SetBranchAddress("scale_err",&scale_err_);
  tree->SetBranchAddress("smear_err",&smear_err_);
  tree->SetBranchAddress("weight",&weight_);
  tree->SetBranchAddress("category",&category_);

  vector<double> dscales;
  vector<double> dsmears;
  vector<double> dweights;

  for (int icat=0; icat<ncats; ++icat) {
    dscales.push_back(0.);
    dsmears.push_back(0.);
    dweights.push_back(0.);
  }
  for (int e=0; e<tree->GetEntries(); e++){
    tree->GetEntry(e);
    //// dscales[category_] += weight_*scale_err_*scale_err_;
    //// dsmears[category_] += weight_*smear_err_*smear_err_;
    dscales[category_] += weight_*scale_err_;
    dsmears[category_] += weight_*smear_err_;
    dweights[category_] += weight_;
  }
  
  // divide by sum of weights and write to card
  for (int icat=0; icat<ncats; ++icat){
    // dsmears[icat]=sqrt(dsmears[icat]/dweights[icat]);
    dsmears[icat]/=dweights[icat];
    if (icat<5) fprintf(file, "CMS_hgg_nuissancedeltasmear%s param 0.0 %5f\n" , catnames.at(icat).c_str(), dsmears.at(icat));
  }
  fprintf(file,"\n\n\n");

  for (int icat=0; icat<ncats; ++icat){
    /// dscales[icat]=sqrt(dscales[icat]/dweights[icat]);
    dscales[icat]/=dweights[icat];
    if (icat<5) fprintf(file, "CMS_hgg_nuissancedeltam%s param 0.0 %5f\n" , catnames.at(icat).c_str(), dscales.at(icat));
  }
  fprintf(file,"\n");
  
  // global scale
  fprintf(file,"CMS_hgg_globalscale param 0.0 0.004717\n");
  fprintf(file,"\n\n\n");
  
  // shape uncertainties
  printf("shape uncertainties\n");
  
  //TH1D *hcount = new TH1D("hcount","",1,0.5,1.5);
  hcount->Reset();
 
  if (massfact){
  if (wsfilename=="0" || wsfilename=="") {
    fprintf(file,"CMS_hgg_n_sigmae lnN ");
    for (unsigned int icat=0; icat<catcuts.size(); ++icat) {
      for (unsigned int iproc=0; iproc<(procnames.size()-1); ++iproc) {
        tree->SetAlias("bdtmod","bdtout");
        hcount->Reset();
        tree->Draw("1>>hcount",catcuts.at(icat)*proccuts.at(iproc),"goff");
        double nom = hcount->GetSumOfWeights();
        tree->SetAlias("bdtmod","bdtout_sigE_up");
        hcount->Reset();
        tree->Draw("1>>hcount",catcuts.at(icat)*proccuts.at(iproc),"goff");
        double sigeplus = hcount->GetSumOfWeights();
        tree->SetAlias("bdtmod","bdtout_sigE_down");
        hcount->Reset();
        tree->Draw("1>>hcount",catcuts.at(icat)*proccuts.at(iproc),"goff");
        double sigeminus = hcount->GetSumOfWeights();
        tree->SetAlias("bdtmod","bdtout");
	fprintf(file,"%.3f/%.3f ",sigeplus/nom, sigeminus/nom);
      }
      fprintf(file,"- ");
    }
    fprintf(file,"\n");

    fprintf(file,"CMS_hgg_n_id lnN ");
    for (unsigned int icat=0; icat<catcuts.size(); ++icat) {
      for (unsigned int iproc=0; iproc<(procnames.size()-1); ++iproc) {
        hcount->Reset();
        tree->SetAlias("bdtmod","bdtout");
        tree->Draw("1>>hcount",catcuts.at(icat)*proccuts.at(iproc),"goff");
        double nom = hcount->GetSumOfWeights();
        tree->SetAlias("bdtmod","bdtout_id_up");
        hcount->Reset();
        tree->Draw("1>>hcount",catcuts.at(icat)*proccuts.at(iproc),"goff");
        double idminus = hcount->GetSumOfWeights();
        tree->SetAlias("bdtmod","bdtout_id_down");
        hcount->Reset();
        tree->Draw("1>>hcount",catcuts.at(icat)*proccuts.at(iproc),"goff");
        double idplus = hcount->GetSumOfWeights();
        tree->SetAlias("bdtmod","bdtout");
        fprintf(file,"%.3f/%.3f ",idminus/nom, idplus/nom);
      }
      fprintf(file,"- ");
    }
    fprintf(file,"\n"); 
  }
  else {
  if (wsfilename=="0" || wsfilename=="") {
    TFile *wsFile = TFile::Open(wsfilename.c_str());
    fprintf(file,"CMS_hgg_n_id lnN ");
    for (unsigned int icat=0; icat<catcuts.size(); ++icat) {
      for (unsigned int iproc=0; iproc<(procfilenames.size()-1); ++iproc) {
        TH1F *nominal = (TH1F*)wsFile->Get(Form("th1f_sig_%s_mass_m125_cat%d",procfilenames[iproc].c_str(),icat));
        TH1F *up = (TH1F*)wsFile->Get(Form("th1f_sig_%s_mass_m125_cat%d_phoIdMvaUp01_sigma",procfilenames[iproc].c_str(),icat));
        TH1F *down = (TH1F*)wsFile->Get(Form("th1f_sig_%s_mass_m125_cat%d_phoIdMvaDown01_sigma",procfilenames[iproc].c_str(),icat));
        if (nominal->Integral() !=0 ) {
          fprintf(file,"%.3f/%.3f ",down->Integral()/nominal->Integral(), up->Integral()/nominal->Integral());
        }
        else {
          fprintf(file,"1.0/1.0 ");
        }
      }
      fprintf(file,"- ");
    }
    fprintf(file,"\n"); 

    fprintf(file,"CMS_hgg_n_sigmae lnN ");
    for (unsigned int icat=0; icat<catcuts.size(); ++icat) {
      for (unsigned int iproc=0; iproc<(procfilenames.size()-1); ++iproc) {
        TH1F *nominal = (TH1F*)wsFile->Get(Form("th1f_sig_%s_mass_m125_cat%d",procfilenames[iproc].c_str(),icat));
        TH1F *up = (TH1F*)wsFile->Get(Form("th1f_sig_%s_mass_m125_cat%d_regSigUp01_sigma",procfilenames[iproc].c_str(),icat));
        TH1F *down = (TH1F*)wsFile->Get(Form("th1f_sig_%s_mass_m125_cat%d_regSigDown01_sigma",procfilenames[iproc].c_str(),icat));
        if (nominal->Integral() !=0 ) {
          fprintf(file,"%.3f/%.3f ",down->Integral()/nominal->Integral(), up->Integral()/nominal->Integral());
        }
        else {
          fprintf(file,"1.0/1.0 ");
        }
      }
      fprintf(file,"- ");
    }
    fprintf(file,"\n"); 
    wsFile->Close();
  }
  }
  }
  inFile->Close();
  outFile->Close();

  fclose(file);
  if (splitVH) system(Form("python removeLeptonGGHVBFCard.py temp_datacard.txt --splitVH > %s",outfilename.c_str()));
  else system(Form("python removeLeptonGGHVBFCard.py temp_datacard.txt > %s",outfilename.c_str()));

}


