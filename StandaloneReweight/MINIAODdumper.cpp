
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "DoubleHReweighter.h"
#include "CfgManager.h"
#include "CfgManagerT.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "TROOT.h"

using namespace std;

string toBinary(int n)
{
  std::string r;
  while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
  return r;
}

int main(int argc, char** argv)
{
  CfgManager conf(argv[1]);
  int   Nkl          = conf.GetOpt<int>    ("Input.Nkl");
  float klmin        = conf.GetOpt<float>  ("Input.klmin");
  float klmax        = conf.GetOpt<float>  ("Input.klmax");
  int   Nkt          = conf.GetOpt<int>    ("Input.Nkt");
  float ktmin        = conf.GetOpt<float>  ("Input.ktmin");
  float ktmax        = conf.GetOpt<float>  ("Input.ktmax");
  vector<string> oldfilename = conf.GetOpt<vector<string> > ("Input.filename");
  string newfilename = conf.GetOpt<string>("Output.filename");
  
  //Get the name of the trees in the old file
  TChain* inchain = new TChain();
  for(auto filename : oldfilename)
  {
    cout<<"Reading file "<<filename<<endl;
    inchain->Add((filename+"/Events").c_str());
  }

  float Generator_weight;
  //float genWeight,LHEWeight;
  //unsigned nLHEPdfWeight;
  //float LHEPdfWeight;
  //unsigned nLHEScaleWeight;
  //float LHEScaleWeight;
  unsigned nGenPart;
  float GenPart_eta[200], GenPart_mass[200], GenPart_phi[200], GenPart_pt[200];
  int GenPart_genPartIdxMother[200], GenPart_pdgId[200], GenPart_status[200], GenPart_statusFlags[200];

  inchain->SetBranchStatus("*",0);
  inchain->SetBranchStatus("Generator_weight",1);
  inchain->SetBranchStatus("nGenPart",1);
  inchain->SetBranchStatus("GenPart_eta",1);
  inchain->SetBranchStatus("GenPart_mass",1);
  inchain->SetBranchStatus("GenPart_phi",1);
  inchain->SetBranchStatus("GenPart_pt",1);
  inchain->SetBranchStatus("GenPart_genPartIdxMother",1);
  inchain->SetBranchStatus("GenPart_pdgId",1);
  inchain->SetBranchStatus("GenPart_status",1);
  inchain->SetBranchStatus("GenPart_statusFlags",1);

  inchain->SetBranchAddress("Generator_weight",&Generator_weight);
  inchain->SetBranchAddress("nGenPart",&nGenPart);
  inchain->SetBranchAddress("GenPart_eta",GenPart_eta);
  inchain->SetBranchAddress("GenPart_mass",GenPart_mass);
  inchain->SetBranchAddress("GenPart_phi",GenPart_phi);
  inchain->SetBranchAddress("GenPart_pt",GenPart_pt);
  inchain->SetBranchAddress("GenPart_genPartIdxMother",GenPart_genPartIdxMother);
  inchain->SetBranchAddress("GenPart_pdgId",GenPart_pdgId);
  inchain->SetBranchAddress("GenPart_status",GenPart_status);
  inchain->SetBranchAddress("GenPart_statusFlags",GenPart_statusFlags);

  //open the files
  inchain->LoadTree(0);
  TFile *newfile = new TFile(newfilename.c_str(),"RECREATE");
  TTree *newtree = new TTree("Events","Events");//inchain->CloneTree(0);
  float* doubleHreweight = new float[Nkl*Nkt];
  float benchmarkreweight[15];
  //float benchmarkXS[15];
  float* klarray = new float[Nkl*Nkt];
  float* ktarray = new float[Nkl*Nkt];
  int NevMC;
  TLorentzVector h1;
  TLorentzVector h2;
  float mHH;
  float costhetaHH;
  newtree->Branch("Generator_weight",&Generator_weight,"Generator_weight/F");
  newtree->Branch("doubleHreweight",doubleHreweight,Form("doubleHreweight[%i]/F",Nkl*Nkt));
  newtree->Branch("benchmarkreweight",benchmarkreweight,"benchmarkreweight[15]/F");
  //newtree->Branch("benchmarkXS",benchmarkXS,"benchmarkXS[15]/F");
  newtree->Branch("klarray",klarray,Form("klarray[%i]/F",Nkl*Nkt));
  newtree->Branch("ktarray",ktarray,Form("ktarray[%i]/F",Nkl*Nkt));
  newtree->Branch("NevMC",&NevMC,"NevMC/I");
  newtree->Branch("mHH",&mHH,"mHH/F");
  newtree->Branch("costhetaHH",&costhetaHH,"costhetaHH/F");
  newtree->Branch("genHiggs1",&h1);
  newtree->Branch("genHiggs2",&h2);
  //Load the reweighter
  DoubleHReweighter rew(conf);

  //Loop over entries
  long Nentries = inchain->GetEntries();
  cout<<Nentries<<" entries"<<endl;
  NevMC=Nentries; 
  for(long ientry=0;ientry<Nentries;++ientry)
  {
    inchain->GetEntry(ientry);
    if(ientry%10000==0)
      cout<<"reading entry "<<ientry<<"\r"<<std::flush;

    //find the gen Higgs
    //cout<<Generator_weight<<endl;
    //getchar();
    vector<TLorentzVector> higgsvec;
    for(unsigned ipart=0; ipart<nGenPart; ++ipart)
    {
      /*
      cout<<"----"<<endl
	  <<"pdgId "<<GenPart_pdgId[ipart]<<endl
	  <<"status "<<GenPart_status[ipart]<<endl
	  <<"statusFlags "<<GenPart_statusFlags[ipart]<<endl
	  <<"statusFlags_binary "<< toBinary(GenPart_statusFlags[ipart])<<endl
	  <<"mother id "<<GenPart_genPartIdxMother[ipart]<<endl;
         
      getchar();
      */
      if(GenPart_pdgId[ipart]==25 && GenPart_genPartIdxMother[ipart]==0)
      {
	//cout<<"found higgs"<<endl;
	TLorentzVector v;
	v.SetPtEtaPhiM(GenPart_pt[ipart], GenPart_eta[ipart], GenPart_phi[ipart], GenPart_mass[ipart]);
	higgsvec.push_back(v);
      }
    }
    if(higgsvec.size()!=2)
    {
      cout<<"WARNING: found "<<higgsvec.size()<<" hard higgs --> skip event"<<endl;
      continue;
    }
    h1 = higgsvec.at(0);
    h2 = higgsvec.at(1);
    mHH = (h1 + h2).M();
    costhetaHH = rew.getCosThetaStar_CS(h1, h2);
    for(int ikl=0; ikl<Nkl; ++ikl)
    {
      float kl = klmin + (ikl+0.5)*(klmax-klmin)/Nkl;
      for(int ikt=0; ikt<Nkt; ++ikt)
      {
	float kt = ktmin + (ikt+0.5)*(ktmax-ktmin)/Nkt;
	klarray[ikl+Nkl*ikt] = kl;
	ktarray[ikl+Nkl*ikt] = kt;
	doubleHreweight[ikl+Nkl*ikt] = rew.getWeight(kl, kt, mHH, costhetaHH);
      }
    }
    
    for(int ibench=0; ibench<15; ++ibench)
    {
      benchmarkreweight[ibench] = rew.getWeight(ibench, mHH, costhetaHH);
      //benchmarkXS[ibench] = 1.; ?????? 
    }
    newtree->Fill();    
  }
  newfile->cd();
  newtree->AutoSave();
  delete newfile;
  delete inchain;
  return 0;
}
