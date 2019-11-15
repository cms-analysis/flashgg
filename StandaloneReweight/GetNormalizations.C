
vector<string> filenames = 
{
  "/afs/cern.ch/user/f/fmonti/work/flashggNew/outputtest/GluGluToHHTo2B2G_TuneCP5.root",
  //"/afs/cern.ch/user/f/fmonti/work/flashggNew/CMSSW_10_5_0/src/flashgg/dumptest2/node_SM.root",
  "/afs/cern.ch/user/f/fmonti/work/flashggNew/CMSSW_10_5_0/src/flashgg/dumptest2/node_5.root",
  "/afs/cern.ch/user/f/fmonti/work/flashggNew/outputtest/node_2.root",  
  "/afs/cern.ch/user/f/fmonti/work/flashggNew/outputtest/node_3.root",  
  "/afs/cern.ch/user/f/fmonti/work/flashggNew/outputtest/node_4.root", 
  //"/afs/cern.ch/user/f/fmonti/work/flashggNew/outputtest/node_5.root", 
  "/afs/cern.ch/user/f/fmonti/work/flashggNew/outputtest/node_6.root",  
  "/afs/cern.ch/user/f/fmonti/work/flashggNew/outputtest/node_7.root",
  "/afs/cern.ch/user/f/fmonti/work/flashggNew/outputtest/node_8.root",
  "/afs/cern.ch/user/f/fmonti/work/flashggNew/outputtest/node_9.root",
  "/afs/cern.ch/user/f/fmonti/work/flashggNew/outputtest/node_10.root",
  "/afs/cern.ch/user/f/fmonti/work/flashggNew/outputtest/node_11.root",
  "/afs/cern.ch/user/f/fmonti/work/flashggNew/outputtest/node_12.root"
};


void GetNormalizations()
{
  //Get the name of the trees in the old file
  TChain* inchain = new TChain();
  for(auto filename : filenames)
  {
    cout<<"Reading file "<<filename<<endl;
    inchain->Add((filename+"/Events").c_str());
  }

  float Generator_weight;
  //float* doubleHreweight = new float[Nkl*Nkt];
  float benchmarkreweight[15];
  //float* klarray = new float[Nkl*Nkt];
  //float* ktarray = new float[Nkl*Nkt];
  int NevMC;

  inchain->SetBranchStatus("*",0);
  inchain->SetBranchStatus("Generator_weight",1);
  //inchain->SetBranchStatus("doubleHreweight",1);
  inchain->SetBranchStatus("benchmarkreweight",1);
  //inchain->SetBranchStatus("klarray",1);
  //inchain->SetBranchStatus("ktarray",1);
  inchain->SetBranchStatus("NevMC",1);

  inchain->SetBranchAddress("Generator_weight",&Generator_weight);
  //inchain->SetBranchAddress("doubleHreweight",doubleHreweight);
  inchain->SetBranchAddress("benchmarkreweight",benchmarkreweight);
  //inchain->SetBranchAddress("klarray",klarray);
  //inchain->SetBranchAddress("ktarray",ktarray);
  inchain->SetBranchAddress("NevMC",&NevMC);

  //Loop over entries
  float weightSUM[15]; //sum(benchmarkreweight/NevMC)
  float weightSUM2[15]; //sum(benchmarkreweight*Generator_weight/NevMC)
  for(int ibench=0; ibench<15; ++ibench)
  {
    weightSUM[ibench]=0;
    weightSUM2[ibench]=0;
  }
  long Nentries = inchain->GetEntries();
  cout<<Nentries<<" entries"<<endl;
  for(long ientry=0;ientry<Nentries;++ientry)
  {
    inchain->GetEntry(ientry);
    if(ientry%10000==0)
      cout<<"reading entry "<<ientry<<"\r"<<std::flush;

    for(int ibench=0; ibench<15; ++ibench)
    {
      weightSUM[ibench] += benchmarkreweight[ibench]/NevMC;
      weightSUM2[ibench] += benchmarkreweight[ibench]*Generator_weight/NevMC;
    }
  }

  for(int ibench=0; ibench<15; ++ibench)
  {
    cout<<"weightSUM["<<ibench+1<<"] = "<<weightSUM[ibench]<<endl;
    cout<<"weightSUM2["<<ibench+1<<"] = "<<weightSUM2[ibench]<<endl;
  }
  delete inchain;

}
