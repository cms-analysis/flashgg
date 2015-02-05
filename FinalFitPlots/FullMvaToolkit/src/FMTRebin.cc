#include <ctime>

#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TMath.h"
#include "TTree.h"
#include "TText.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLine.h"

#include "../interface/FMTRebin.h"

using namespace std;

FMTRebin::FMTRebin(){
  cout << "WARNING -- You are using the default constructor of FMTRebin() -- this isn't recommended if you're running the FullMva analysis. It should only be called if you're making use of some of FMTRebins functions" << endl;
  cout << "Passing some dummy defaults for now..." << endl;
  vector<string> systs;
  vector<map<int,vector<double> > > binedges;
  FMTRebin(NULL,NULL,-999.,false,110,150,0.5,100.,180.,160,0.02,0.02,6,3,1,99.8,180.,4,1,2,1,3,systs,false,binedges,false);
}

FMTRebin::FMTRebin(TFile *tF, TFile *oF):
	FMTBase(),
	tFile(tF),
	outFile(oF),
	justRebin_(false),
	catByHand_(false)
{

  signalVector1 = new double[25];
  backgroundVector1 = new double[25];
  cout << "Creating fitter" << endl;
  cout << "Passing outfile" << outFile->GetName() << endl;
  fitter = new FMTFit(tFile,outFile);

}

FMTRebin::FMTRebin(TFile *tF, TFile *oF, double intLumi, bool is2011, int mHMinimum, int mHMaximum, double mHStep, double massMin, double massMax, int nDataBins, double signalRegionWidth, double sidebandWidth, int numberOfSidebands, int numberOfSidebandsForAlgos, int numberOfSidebandGaps, double massSidebandMin, double massSidebandMax, int nIncCategories, bool includeVBF, int nVBFCategories, bool includeLEP, int nLEPCategories, vector<string> systematics, bool rederiveOptimizedBinEdges, vector<map<int,vector<double> > > AllBinEdges, bool verbose):
	
	FMTBase(intLumi, is2011, mHMinimum, mHMaximum, mHStep, massMin, massMax, nDataBins, signalRegionWidth, sidebandWidth, numberOfSidebands, numberOfSidebandsForAlgos, numberOfSidebandGaps, massSidebandMin, massSidebandMax, nIncCategories, includeVBF, nVBFCategories, includeLEP, nLEPCategories, systematics, rederiveOptimizedBinEdges, AllBinEdges, verbose),
	tFile(tF),
	outFile(oF),
	justRebin_(false),
	catByHand_(false)
{
  signalVector1 = new double[25];
	backgroundVector1 = new double[25];
	//tFile = TFile::Open(filename.c_str());
	//outFile = new TFile(outfilename.c_str(),"RECREATE");
  cout << "Creating fitter" << endl;
  cout << "Passing outfile" << outFile->GetName() << endl;
	fitter = new FMTFit(tFile,outFile,intLumi,is2011,mHMinimum, mHMaximum, mHStep, massMin, massMax, nDataBins, signalRegionWidth, sidebandWidth, numberOfSidebands, numberOfSidebandsForAlgos, numberOfSidebandGaps, massSidebandMin, massSidebandMax, nIncCategories, includeVBF, nVBFCategories, includeLEP, nLEPCategories, systematics, rederiveOptimizedBinEdges, AllBinEdges, verbose);

}

FMTRebin::~FMTRebin(){
	//tFile->Close();
	//outFile->Close();
	delete signalVector1;
	delete backgroundVector1;
	delete fitter;
}

std::vector<double> FMTRebin::significanceOptimizedBinning(TH1F *hs,TH1F *hb,int nTargetBins, int mass){

  // Performs Optimized Binning based on a Signal and Background  distributions
  // First runs the optimizedBinning on background and rebins S and B clones, note, always performs 
  // revise_target=false,direction=-1 and use_n_entries=true
  // nTargetBins is used for the flat binning, decision to merge is based on improvement to expected significance
  // Full scan is done for largest significance (wardning, could be very slow for tight constraints)
  gStyle->SetOptStat(0); 
  if (verbose_) cout << "Significane Optimized Binning......" << endl;
  int ninitBins = hb->GetNbinsX();
  if (hs->Integral()==0 ||  hb->Integral()==0 || ninitBins < 2) {
    std::vector<double> binEdges;
    binEdges.push_back(hb->GetBinLowEdge(1));
    binEdges.push_back(hb->GetBinLowEdge(ninitBins+1));
    if (verbose_){
      cout << "HS: " << hs->Integral() << endl;
      cout << "HB: " << hb->Integral() << endl;
      cout << "HBins: " << hb->GetNbinsX() << endl;
    }
    return binEdges;
  }

  std::vector<double> binEdges = optimizedReverseBinning(hb,nTargetBins,false,true);

  // Just TESTING HERE so remove this line soon!
  //	nTargetBins = 150; // this gives us about 144 with the latest thing :)
  //	std::vector<double> binEdges = optimizedReverseBinning(hb,nTargetBins,false,false);
  //---------------------------------------------------------------------------------

  int j =0;
  double *arrBins = new double[binEdges.size()];
  for (std::vector<double>::iterator it=binEdges.begin();it!=binEdges.end();it++){
    //cout << *it << endl;
    arrBins[j]=*it;
    j++;	
  }
  // Create new rebinned histograms (only temporary)
  //TCanvas *canv = new TCanvas();
  TH1F *hbnew =(TH1F*) hb->Rebin(binEdges.size()-1,"hbnew",arrBins);
  TH1F *hsnew =(TH1F*) hs->Rebin(binEdges.size()-1,"hsnew",arrBins);
  //hbnew->SetLineColor(kBlue);
  //hsnew->SetLineColor(kBlue);
  //TCanvas *cB = new TCanvas();
  //TCanvas *cS = new TCanvas();
  //cB->cd();
  //hbnew->Draw();
  //cS->cd();
  //hsnew->Draw();

  // Better smoothing which doesn't use the first and last binsi, performs a fit to the histogram	
  if (hsnew->Integral()!=0 && hbnew->Integral()!=0 && binEdges.size()-1 > 10){
    histogramSmoothingFit(hsnew);
    histogramSmoothingFit(hbnew);
    //hsnew->Smooth(1000);
    //hbnew->Smooth(1000);
  }
  hbnew->SetLineColor(kBlue);
  hsnew->SetLineColor(kRed);
  
  TCanvas *canv = new TCanvas();
  hbnew->Draw("HIST");
  TH1F *temp = (TH1F*)hsnew->Clone(Form("%s_dummy",hsnew->GetName()));
  temp->Scale(10.);
  temp->Draw("HISTsame");
  //cB->cd();
  //hbnew->Draw("same");
  //cS->cd();
  //hsnew->Draw("same");

  // --------------------------- TEST --------------------------- //
  //hsnew->Rebin(2);
  //hbnew->Rebin(2);
  //hsnew->Rebin(2);
  //hbnew->Rebin(2);
  // --------------------------- TEST --------------------------- //

  // Do we really need the background histogram ?  we will be assuming that the first step is nentries per bin

  // Smooth signal new binned histograms, the size of smoothing should be ~1% of the total bins	
  //int nSmooth = (int) 0.01*hsnew->GetNbinsX();
  //hsnew->Smooth(nSmooth);

  delete [] arrBins;

  if (hbnew->Integral()==0 || hsnew->Integral()==0) return binEdges;
  if (hbnew->GetNbinsX() <= 10) return binEdges;

  int nNewBins = hbnew->GetNbinsX();

  // Here is the New Algorithm
  int 	*counters, *chosen_counters,*frozen_counters;
  double 	highestMaxSignificance=0;
  int 	chosenN=1;
  int 	*finalCounters=NULL ;

  g_step = (int)TMath::Exp(TMath::Log(nNewBins/2)/2);
  if (g_step < 1) g_step=1;

  int Retry=0;

  for (int N=1;N<8;N++){				// Refuse to go beyond 8 Bins, will take forever
    sweepmode=0;	// First perform Broad Scan with optimized step size (g_step)
    bool skipBroad = false;
    if ( nNewBins < (N-1+2+Retry) ) {std::cout << "Forced to perform Fine scan since all the Retries failed to find a nice minimum :("<<std::endl; skipBroad=true;}
    //if (Retry>20) {std::cout << "Forced to perform Fine scan since all the Retries failed to find a nice minimum :("<<std::endl; skipBroad=true;}

    double maximumSignificance=0;
    counters = new int[N];
    chosen_counters = new int[N];
    frozen_counters = new int[N];
    for (int c=0;c<N;c++) counters[c]=c+2+Retry;	// init to starting values
    for (int c=0;c<N;c++) frozen_counters[c]=c+2+Retry; // init to starting values
    for (int c=0;c<N;c++) chosen_counters[c]=c+2+Retry; // init to starting values

    double diff;
    clock_t start;

    std::cout << "Performing Fully optimized Scan"	<<std::endl;
    start=clock();
    if (!skipBroad) maxSigScan(&maximumSignificance,frozen_counters,chosen_counters,hsnew,hbnew,N,counters,N-1);

    sweepmode=1;	// Now do Fine scan after having found rough maximum
    for (int c=0;c<N;c++) counters[c]=chosen_counters[c]; // init to rough guess
    for (int c=0;c<N;c++) frozen_counters[c]=chosen_counters[c];  // init to rough guess

    // For full scanning, need to move lowest boundary to lowest point now
    int resetpoint = (2>frozen_counters[0]-g_step) ? 2 : frozen_counters[0]-g_step;
    counters[0]=resetpoint;

    maxSigScan(&maximumSignificance,frozen_counters,chosen_counters,hsnew,hbnew,N,counters,N-1);

    diff = ( std::clock() - start ) / (double)CLOCKS_PER_SEC;
    std::cout << Form("Finished, time taken = %3.5f",diff)<<std::endl;
    std::cout << "N Bins, Max Significance -> " << N+1 << " "<<maximumSignificance << std::endl;
    std::cout << "Boundaries at: [ ";
    for (int cc=0;cc<N; cc++) std::cout << hsnew->GetBinLowEdge(chosen_counters[cc]) << " , ";
    std::cout << " ]" << std::endl;

    if (maximumSignificance < highestMaxSignificance){
         
         std::cout << "Looks like the Broad Scan found a local maxmimum and got stuck there (can happen if the initial number of bins is small), Try again " <<std::endl;
         cout << "Chosen bins:   "; 
         for (int c=0;c<N;c++) cout << chosen_counters[c] << "    ";
         cout << endl;
         N--; Retry++;
         continue;
 
    }
    else Retry =0;


    if ((maximumSignificance-highestMaxSignificance)/highestMaxSignificance > 0.001){
      highestMaxSignificance = maximumSignificance ;
      finalCounters= new int[N];
      chosenN = N;
      for (int cc=0;cc<N;cc++) finalCounters[cc]=chosen_counters[cc];
    } else {
      
      break;
    }

  }

  //cB->Print(Form("plots/pdf/rebin_B_%d.pdf",mass));
  //cB->Print(Form("plots/png/rebin_B_%d.png",mass));
  //cS->Print(Form("plots/pdf/rebin_S_%d.pdf",mass));
  //cS->Print(Form("plots/png/rebin_S_%d.png",mass));

  std::vector<double> newbinEdges;
  newbinEdges.push_back(hsnew->GetBinLowEdge(1));
  for (int newguy=0;newguy<chosenN;newguy++){
    std::cout << "newEdge = " << hsnew->GetBinLowEdge(finalCounters[newguy]) << " at bin " << finalCounters[newguy] <<std::endl;
    newbinEdges.push_back(hsnew->GetBinLowEdge(finalCounters[newguy]));
  }
  newbinEdges.push_back(hsnew->GetBinLowEdge(nNewBins+1));

  for (unsigned int c=0; c<newbinEdges.size(); c++){
    TLine l;
    l.SetLineColor(kGreen+3);
    l.SetLineWidth(2);
    l.SetLineStyle(kDashed);
    l.DrawLine(newbinEdges[c],0.1,newbinEdges[c],hbnew->GetMaximum());
  }

  canv->Print(Form("plots/pdf/rebin_%d.pdf",mass));
  canv->Print(Form("plots/png/rebin_%d.png",mass));
  delete canv;
  delete [] finalCounters;
  delete [] counters;
  delete [] chosen_counters;
  //delete cB;
  //delete cS;

  return newbinEdges;

}

std::vector<double> FMTRebin::optimizedReverseBinning(TH1F *hb,int nTargetBins,bool revise_target, bool use_n_target){
  // Return a set of bins which are "smoother" 
  if (revise_target) {
    if (use_n_target){
      std::cerr << "WARNING -- FMTRebin::OptimizedBinning -- Can't use number of Entries as target in revised binning algo " << std::endl; 
      use_n_target = false;  // geometric algo always use revised number of bins, not number of entries

    }
  }

  int nBins = hb->GetNbinsX();
  std::vector<double> binEdges;

  double targetNumbers;
  if (use_n_target) targetNumbers = nTargetBins; 
  else targetNumbers = hb->Integral()/nTargetBins;

  if (hb->Integral() < 2*targetNumbers){
    std::cout << "FMTRebin::OptimizedBinning -- Not enough entries in histogram for target numbers calculated - " 
      << targetNumbers 
      << ", Returning current bin boundaries "  << std::endl;
    //for (int j=2;j<=nBins+1;j++) binEdges.push_back(hb->GetBinLowEdge(j));
    binEdges.push_back(hb->GetBinLowEdge(1));
    binEdges.push_back(hb->GetBinLowEdge(nBins+1));
    return binEdges;
  }
  binEdges.push_back(hb->GetBinLowEdge(nBins+1));

  std::cout << "FMTRebin::optimizedBinning -- Performing Reverse Optimize Binning" <<std::endl;
  double sumBin = 0;
  int i=nBins;
  while (i>=1){
    if (revise_target) targetNumbers = hb->Integral(1,i)/nTargetBins;
    sumBin=hb->GetBinContent(i);
    double highEdge=hb->GetBinLowEdge(i);

    bool carryOn = sumBin <= targetNumbers;
    while ( carryOn){
      if (i>1){
        sumBin+=hb->GetBinContent(i-1);
        highEdge = hb->GetBinLowEdge(i-1);
        carryOn =(sumBin <targetNumbers && i>=1);
        i--;
      } else {
        highEdge = hb->GetBinLowEdge(i);
        carryOn=false;
      }
    }
    binEdges.push_back(highEdge);
    i--;
  }
  if (sumBin < 10) binEdges.erase(binEdges.end()-2);
  reverse(binEdges.begin(),binEdges.end());
  return binEdges;

}

double FMTRebin::calculateSigMulti(double *s1, double *b1, int nchannel){

  //	int nchannel=s1.size();
  double sterm=0;
  double logterms=0;
  for (int i=0;i<nchannel;i++){
    logterms+=(s1[i]+b1[i])*TMath::Log((s1[i]+b1[i])/b1[i]);
    sterm+=s1[i];
  }
  double sig =  1.4142*TMath::Sqrt(logterms - sterm);
  return sig;
}

void FMTRebin::histogramSmoothing(TH1F* h, int n){
  // Nothing too special, a function which will smooth a histogram but ignore the first and last
  // bins, useful for the "flat-binning" approach! 
  if (h->GetNbinsX()>3){
    int nbin = h->GetNbinsX();
    TH1F *h2 = new TH1F(Form("hn%s",h->GetName()),Form("hn%s",h->GetName()),nbin-2,0,1);
    for (int i=1;i<=nbin-2;i++){
      h2->SetBinContent(i,h->GetBinContent(i+1));
    }
    h2->Smooth(n);
    for (int i=2;i<=nbin-1;i++){
      h->SetBinContent(i,h2->GetBinContent(i-1));
    }

  }

  return;
}

void FMTRebin::histogramSmoothingFit(TH1F* h){
  // Nothing too special, a function which will smooth a histogram but ignore the first and last
  // bins, useful for the "flat-binning" approach! 
  //TCanvas *c = new TCanvas();
  //TH1F *old = (TH1F*)h->Clone();
  //old->SetLineColor(kRed);
  //old->SetLineWidth(2);
  float originalIntegral=h->Integral();
  if (h->GetNbinsX()>3){
    int nbin = h->GetNbinsX();
    TH1F *h2 = new TH1F(Form("hn%s",h->GetName()),Form("hn%s",h->GetName()),nbin-2,0,1);
    for (int i=1;i<=nbin-2;i++){
      h2->SetBinContent(i,h->GetBinContent(i+1));
    }
    h->Fit("pol9","F","",h->GetBinLowEdge(2),h->GetBinLowEdge(h->GetNbinsX()));
    //h2->Smooth(n);
    for (int i=2;i<=nbin-1;i++){
      if (h->GetFunction("pol9")->Eval(h->GetBinCenter(i-1)) < 0){
        h->SetBinContent(i,0.5*(h->GetBinContent(i-1)+h->GetBinContent(i+1)));

      } else {
        h->SetBinContent(i,h->GetFunction("pol9")->Eval(h->GetBinCenter(i)));
      }
    }

  }
  h->Scale(originalIntegral/h->Integral());
  //old->Draw("hist");
  //h->SetLineColor(kBlack);
  //h->Draw("hist same");
  //c->SaveAs(Form("test_%s.pdf",h->GetName()));
  return;
}

TH1F* FMTRebin::rebinBinnedDataset(std::string new_name, TH1F *hb,std::vector<double> binEdges, int cat){

  double *arrBins = new double[binEdges.size()];
  int j=0;
  std::cout << "Rebin Binned Data (new edges) " << cat << " "; 
  for (std::vector<double>::iterator it=binEdges.begin();it!=binEdges.end();it++){
    arrBins[j]=*it;
    std::cout << *it << ", ";
    j++;
  }
  std::cout << std::endl;
  //const char *h_name = (const char *) hb->GetName;
  //const char *title  = (const char *) hb->GetTitle;
  // this hard code should be removed
  if ((isVBFCat(cat) && int(binEdges.size())!=getnVBFCategories()+1) || (isLEPCat(cat) && int(binEdges.size())!=getnLEPCategories()+1)){
    cout << "INC? " << isIncCat(cat) << endl;
    cout << "VBF? " << isVBFCat(cat) << endl;
    cout << "LEP? " << isLEPCat(cat) << endl;
    cout << "nVBFCat " << getnVBFCategories() << endl;
    cout << "nBinEdg " << binEdges.size() << endl;
    cerr << "ERROR -- FMTRebin::rebinBinnedDataset - I\'m confused about what to do. The bin edges specified for cat " << cat << " do no match the binning. Bailing out." << endl;
    exit(1);
  }

  TH1F *hbnew;

  if (isVBFCat(cat)){
    // do VBF
    int vbfIt = cat-getnIncCategories();
    double lowEdge=arrBins[vbfIt];
    double highEdge=arrBins[vbfIt+1];
    std::cout << "VBF Category " << cat << " in bin egdes " << lowEdge << ", "<< highEdge <<std::endl;
    double center=lowEdge+((highEdge-lowEdge)/2.);
    hbnew = new TH1F("hV","hV",1,lowEdge,highEdge);
    for (int i=0; i<hb->GetEntries(); i++) hbnew->Fill(center);
    hbnew->Sumw2();
    if (hb->GetEntries()!=0) hbnew->Scale(hb->Integral()/hbnew->Integral());
    std::cout << " ..... Nevents -- " << hb->Integral() <<std::endl;
  }
  else if (isLEPCat(cat)){
    // do LEP
    int lepIt = cat-getnVBFCategories()-getnIncCategories();
    double lowEdge=arrBins[lepIt];
    double highEdge=arrBins[lepIt+1];
    std::cout << "LEP Category " << cat << " in bin egdes " << lowEdge << ", "<< highEdge <<std::endl;
    double center=lowEdge+((highEdge-lowEdge)/2.);
    hbnew = new TH1F("hL","hL",1,lowEdge,highEdge);
    for (int i=0; i<hb->GetEntries(); i++) hbnew->Fill(center);
    hbnew->Sumw2();
    if (hb->GetEntries()!=0) hbnew->Scale(hb->Integral()/hbnew->Integral());
    std::cout << " ..... Nevents -- " << hb->Integral() <<std::endl;
  }
  else if (isIncCat(cat)){
    /*
    if (getcatByHand()){
      double incBinWidth=2./float(getnIncCategories());
      hbnew = new TH1F("hI","hI",1,-1.+(cat*incBinWidth),-1.+((cat+1)*incBinWidth));
      for (int i=0; i<hb->GetEntries(); i++) hbnew->Fill(-1.+((cat+0.5)*incBinWidth));
      hbnew->Sumw2();
      if (hb->GetEntries()!=0) hbnew->Scale(hb->Integral()/hbnew->Integral());
    }
    else {*/
      hbnew =(TH1F*) hb->Rebin(binEdges.size()-1,hb->GetName(),arrBins);
    //}
  }
  else {
    cerr << "ERROR -- FMTRebin::rebinBinnedDataset - cat " << cat << " is not recognised. Bailing out" << endl;
    exit(1);
  }
  hbnew->SetName(Form("%s",new_name.c_str()));
  //cout << "title for new re-binned histogream - " << hb->GetTitle()<<endl; 
  hbnew->SetTitle(hb->GetTitle());
  
  /*
  for (int b=1; b<=hbnew->GetNbinsX(); b++){
    cout << "b" << b << ":   " << hbnew->GetBinContent(b) << "  " << hbnew->GetBinLowEdge(b) << endl;
    cout << "b" << b << ":   " << hb->Integral(hb->FindBin(hbnew->GetBinLowEdge(b)),hb->FindBin(hbnew->GetBinLowEdge(b+1))) << endl;
  }
  cout << hb->Integral(1,157) << endl;
  cout << hb->Integral(157,165) << endl;
  cout << hb->Integral(165,169) << endl;
  cout << hb->Integral(169,176) << endl;
  cout << hb->Integral(176,183) << endl;
  */
  // Just a quick test, mask the last "channel"
  //hbnew->SetBinContent(hbnew->GetNbinsX(),0);
  //cout << "DONT DO THIS IN MAIN PROGRAM ----- LINE 1563 rebin setting last bin to 0" <<endl;

  delete [] arrBins;
  return hbnew;	
}

void FMTRebin::maxSigScan(double *maximumSignificance,int *frozen_counters,int *chosen_counters,TH1F *hs, TH1F *hb, int N,int *counters, int movingCounterIndex){


  if (movingCounterIndex >=N ) std::cout << "Should never have got here!!!" <<std::endl;
  if (movingCounterIndex < 0) return;
  if (counters[movingCounterIndex] < 2) std::cout << "WHAT IS GOING ON?? " <<  movingCounterIndex << " " << counters[movingCounterIndex]<<std::endl;
  int nBins = hs->GetNbinsX();
  double significance_now;
  if (not sweepmode){
    // N is number of boundaries
    int m=1;
    for (int k=movingCounterIndex+1;k<N;k++) {
      counters[k]=counters[movingCounterIndex]+m;
      m++;
    }
  } else {
    // N is number of boundaries
    int m=1;
    for (int k=movingCounterIndex+1;k<N;k++) {
      int newpoint = (frozen_counters[k] - g_step > 1) ? frozen_counters[k] - g_step:2;
      counters[k]= (newpoint > counters[movingCounterIndex]+m )? newpoint: counters[movingCounterIndex]+m ;
      m++;
    }
  }

  if (counters[movingCounterIndex] < 2) std::cout << "WHAT IS GOING ON?? " <<  movingCounterIndex << " " << counters[movingCounterIndex]<<std::endl;

  if ( movingCounterIndex==N-1) {	
    if (not sweepmode){
      for (;counters[N-1]<=nBins;counters[N-1]+=g_step){
        for (int j=0;j<=N-1;j++){
          if (j==0){
            signalVector1[j] = (hs->Integral(1,counters[j]-1));
            backgroundVector1[j] = (hb->Integral(1,counters[j]-1));

          } else {
            signalVector1[j] = (hs->Integral(counters[j-1],counters[j]-1));
            backgroundVector1[j] = (hb->Integral(counters[j-1],counters[j]-1));

          }
        }
        signalVector1[N]=(hs->Integral(counters[N-1],nBins));
        backgroundVector1[N]=(hb->Integral(counters[N-1],nBins));
        significance_now = calculateSigMulti(signalVector1,backgroundVector1,N+1);

        if (significance_now>*maximumSignificance){
          *maximumSignificance=significance_now;
          for (int j=0;j<N;j++){
            chosen_counters[j]=counters[j];	
            if (chosen_counters[j] < 0) std::cout << "Freak OUT !!! - " << j << "  " << chosen_counters[j] <<std::endl;
          }
        }
      }
      maxSigScan(maximumSignificance,frozen_counters,chosen_counters,hs,hb,N,counters,movingCounterIndex-1);
    } else { // fine scanning
      int currmax = (nBins < frozen_counters[N-1] + g_step )? nBins : frozen_counters[N-1] + g_step;
      for (;counters[N-1]<=currmax;counters[N-1]++){
        for (int j=0;j<=N-1;j++){
          if (j==0){
            signalVector1[j] = (hs->Integral(1,counters[j]-1));
            backgroundVector1[j] = (hb->Integral(1,counters[j]-1));

          } else {
            signalVector1[j] = (hs->Integral(counters[j-1],counters[j]-1));
            backgroundVector1[j] = (hb->Integral(counters[j-1],counters[j]-1));

          }
        }	
        signalVector1[N]=(hs->Integral(counters[N-1],nBins));
        backgroundVector1[N]=(hb->Integral(counters[N-1],nBins));
        significance_now = calculateSigMulti(signalVector1,backgroundVector1,N+1);
        if (significance_now>*maximumSignificance){
          *maximumSignificance=significance_now;
          for (int j=0;j<N;j++){
            chosen_counters[j]=counters[j];
          }
        }
      }
      maxSigScan(maximumSignificance,frozen_counters,chosen_counters,hs,hb,N,counters,movingCounterIndex-1);
    }
  }


  else if (counters[movingCounterIndex]+1 <= nBins-(N-movingCounterIndex)){

    if (not sweepmode){
      if (counters[movingCounterIndex]+g_step <= nBins-(N-movingCounterIndex)){
        counters[movingCounterIndex]+=g_step;
      } else {
        counters[movingCounterIndex]=nBins-(N-movingCounterIndex);
      }
      int m=1;
      for (int k=movingCounterIndex+1;k<N;k++) {
        counters[k]=counters[movingCounterIndex]+m;
        m++;
      }
      maxSigScan(maximumSignificance,frozen_counters,chosen_counters,hs,hb,N,counters,movingCounterIndex+1);
    } else {
      // N is number of boundaries
      if (counters[movingCounterIndex]+1 <= frozen_counters[movingCounterIndex]+g_step){
        counters[movingCounterIndex]++;
        int m=1;
        for (int k=movingCounterIndex+1;k<N;k++) {
          int newpoint = (frozen_counters[k] - g_step > 1) ? frozen_counters[k] - g_step:2;
          if (newpoint < 0) {std::cout << "Whaaaaaa? "<< newpoint <<std::endl;}
          counters[k]= (newpoint > counters[movingCounterIndex]+m )? newpoint: counters[movingCounterIndex]+m ;
          m++;
        }
        maxSigScan(maximumSignificance,frozen_counters,chosen_counters,hs,hb,N,counters,movingCounterIndex+1);
      } else {
        if (movingCounterIndex>0){
          maxSigScan(maximumSignificance,frozen_counters,chosen_counters,hs,hb,N,counters,movingCounterIndex-1);
        } else {
          return;
        }
      }
    }

  }

  else { // got to the end,

    if (movingCounterIndex>0){
      maxSigScan(maximumSignificance,frozen_counters,chosen_counters,hs,hb,N,counters,movingCounterIndex-1);
    } else {
      return;
    }
  }
}

TH1F* FMTRebin::sumMultiBinnedDatasets(string new_name, vector<TH1F*> hists, double normalisation, bool scale){
  
  int nHists=hists.size();
  TH1F *histOne = (TH1F*)hists[0]->Clone();
  histOne->SetName(new_name.c_str());
  if (!scale && normalisation<0) normalisation=1.0;
  if (!scale) histOne->Scale(1.0/histOne->Integral());

  for (int i=1; i<nHists; i++){
    if (scale) histOne->Add(hists[i]);
    else histOne->Add(hists[i],1.0/hists[i]->Integral());
  }

  if (normalisation>0) histOne->Scale(normalisation/histOne->Integral());

  //tFile->cd();
  //write(tFile,histOne);
	outFile->cd();
  gDirectory->cd(Form("%s:/",outFile->GetName()));
  histOne->Write();
	return histOne;
}

void FMTRebin::makeOutputHistogram(string new_name, string old_name, int binningMass){

  vector<double> theEdges;
	vector<TH1F*> old, rebinned;
	for (int cat=0; cat<getNcats(); cat++){
    if (isVBFCat(cat) && !getincludeVBF()) continue;
    if (isLEPCat(cat) && !getincludeLEP()) continue;
    if (isIncCat(cat)) theEdges = getBinEdges(binningMass);
    else if (isVBFCat(cat)) theEdges = getVBFBinEdges(binningMass);
    else if (isLEPCat(cat)) theEdges = getLEPBinEdges(binningMass);
    else {
      cerr << "Category " << cat << " not recognised " << endl;
      exit(1);
    }
		old.push_back((TH1F*)tFile->Get(Form("%s_cat%d",old_name.c_str(),cat)));
		rebinned.push_back(rebinBinnedDataset(Form("%s_cat%d",new_name.c_str(),cat),old[cat],theEdges,cat));
    if (verbose_) checkHisto(old[cat]);
    if (verbose_) checkHisto(rebinned[cat]);
		//write(tFile,rebinned[cat]);
		outFile->cd();
    gDirectory->cd(Form("%s:/",outFile->GetName()));
		rebinned[cat]->Write();
	}
  if (!getincludeLEP() && !getincludeVBF()) mergeHistograms(new_name,rebinned[0]);
  if (getincludeVBF()) for (int vCat=0; vCat<getnVBFCategories(); vCat++) mergeHistograms(new_name,rebinned[0],rebinned[vCat+1]);
  if (getincludeLEP()) for (int lCat=0; lCat<getnLEPCategories(); lCat++) mergeHistograms(new_name,rebinned[0],rebinned[lCat+getnVBFCategories()+1]);
  //tFile->cd();
  if (verbose_) checkHisto(rebinned[0]);
  if (verbose_) cout << "----------------------------------------------------------------------------------------------" << endl;
	outFile->cd();
  gDirectory->cd(Form("%s:/",outFile->GetName()));
  rebinned[0]->Write();
	//write(tFile,rebinned[0]);
}

void FMTRebin::mergeHistograms(std::string nameHist, TH1F *hist1){


   // This one is simply just a renaming tool
   //Get Bin Low edges of histogram 1
   int nbins1 = hist1->GetNbinsX();
   int nbins2 = 0;
   int nbinsTot = nbins1+nbins2;

   //double *arrBins1 = new double[nbins1];
   //double *arrBins2 = new double[nbins2+1];
   double *arrBinsTot = new double[nbinsTot+1];

   for (int i=1;i<=nbins1+1;i++){
     arrBinsTot[i-1]=hist1->GetBinLowEdge(i);
   }

   //const char *histoname = hist1->GetName();
   const char *histotitle = hist1->GetTitle();

   TH1F *newHist = new TH1F(Form("NUMPTYNAME%s",hist1->GetName()),histotitle,nbinsTot,arrBinsTot);
   newHist->SetName(nameHist.c_str());
   for (int i=1;i<=nbins1;i++){
	newHist->SetBinContent(i,hist1->GetBinContent(i));
	newHist->SetBinError(i,hist1->GetBinError(i));
   } 

   if (verbose_) std::cout << "FMTRebin::MergeHistograms -- Replacing th1f - " 
	     << newHist->GetName()
	     << std::endl;
   // Now the dangerous part!
   *hist1 = *newHist;

}

void FMTRebin::mergeHistograms(std::string nameHist, TH1F* hist1, TH1F* hist2){
   
   //Get Bin Low edges of histogram 1
   int nbins1 = hist1->GetNbinsX();
   int nbins2 = hist2->GetNbinsX();
   int nbinsTot = nbins1+nbins2;

   //double *arrBins1 = new double[nbins1];
   //double *arrBins2 = new double[nbins2+1];
   double *arrBinsTot = new double[nbinsTot+1];

   for (int i=1;i<=nbins1;i++){
     arrBinsTot[i-1]=hist1->GetBinLowEdge(i);
   }
   for (int i=1;i<=nbins2+1;i++){	// Include upper edge in 2nd hist
     arrBinsTot[i+nbins1-1]=hist2->GetBinLowEdge(i);
   }

   //const char *histoname = hist1->GetName();
   const char *histotitle = hist1->GetTitle();

   TH1F *newHist = new TH1F(Form("NUMPTYNAME%s",hist1->GetName()),histotitle,nbinsTot,arrBinsTot);
   newHist->SetName(nameHist.c_str());
   for (int i=1;i<=nbins1;i++){
	newHist->SetBinContent(i,hist1->GetBinContent(i));
	newHist->SetBinError(i,hist1->GetBinError(i));
   } 
   for (int i=1;i<=nbins2;i++){
	newHist->SetBinContent(i+nbins1,hist2->GetBinContent(i));
	newHist->SetBinError(i+nbins1,hist2->GetBinError(i));
   } 

   if (verbose_) std::cout << "FMTRebin::MergeHistograms -- Replacing th1f - " 
	     << newHist->GetName()
	     << std::endl;
   // Now the dangerous part!
   *hist1 = *newHist;
}

void FMTRebin::makeSignalOutputHistogram(string new_name, string old_name, int binningMass){

  // Do central signal MC
  vector<double> theEdges;
	vector<TH1F*> old, rebinned;
	for (int cat=0; cat<getNcats(); cat++){
    if (isVBFCat(cat) && !getincludeVBF()) continue;
    if (isLEPCat(cat) && !getincludeLEP()) continue;
    if (isIncCat(cat)) theEdges = getBinEdges(binningMass);
    else if (isVBFCat(cat)) theEdges = getVBFBinEdges(binningMass);
    else if (isLEPCat(cat)) theEdges = getLEPBinEdges(binningMass);
    else {
      cerr << "Category " << cat << " not recognised " << endl;
      exit(1);
    }
		old.push_back((TH1F*)tFile->Get(Form("%s_cat%d",old_name.c_str(),cat)));
		rebinned.push_back(rebinBinnedDataset(Form("%s_cat%d",new_name.c_str(),cat),old[cat],theEdges,cat));
    if (verbose_) checkHisto(old[cat]);
    if (verbose_) checkHisto(rebinned[cat]);
		outFile->cd();
    gDirectory->cd(Form("%s:/",outFile->GetName()));
		rebinned[cat]->Write();
		//write(tFile,rebinned[cat]);
	}
  if (!getincludeLEP() && !getincludeVBF()) {
	mergeHistograms(new_name,rebinned[0]);
  }
  if (getincludeVBF()) for (int vCat=0; vCat<getnVBFCategories(); vCat++) mergeHistograms(new_name,rebinned[0],rebinned[vCat+1]);
  if (getincludeLEP()) for (int lCat=0; lCat<getnLEPCategories(); lCat++) mergeHistograms(new_name,rebinned[0],rebinned[lCat+getnVBFCategories()+1]);
  if (verbose_) checkHisto(rebinned[0]); 
	outFile->cd();
  gDirectory->cd(Form("%s:/",outFile->GetName()));
	rebinned[0]->Write();
	//write(tFile,rebinned[0]);

  old.clear(), rebinned.clear();
	// Do systematic templates
	vector<string> theSystematics = getsystematics();
	for (vector<string>::iterator syst=theSystematics.begin(); syst!=theSystematics.end(); syst++){
		vector<TH1F*>  up, up_rebinned, down, down_rebinned;
		for (int cat=0; cat<getNcats(); cat++){
      if (isVBFCat(cat) && !getincludeVBF()) continue;
      if (isLEPCat(cat) && !getincludeLEP()) continue;
      if (isIncCat(cat)) theEdges = getBinEdges(binningMass);
      else if (isVBFCat(cat)) theEdges = getVBFBinEdges(binningMass);
      else if (isLEPCat(cat)) theEdges = getLEPBinEdges(binningMass);
      else {
        cerr << "Category " << cat << " not recognised " << endl;
        exit(1);
      }
			up.push_back((TH1F*)tFile->Get(Form("%s_cat%d_%sUp01_sigma",old_name.c_str(),cat,syst->c_str())));
			down.push_back((TH1F*)tFile->Get(Form("%s_cat%d_%sDown01_sigma",old_name.c_str(),cat,syst->c_str())));
			up_rebinned.push_back(rebinBinnedDataset(Form("%s_cat%d_%sUp01_sigma",new_name.c_str(),cat,syst->c_str()),up[cat],theEdges,cat));
			down_rebinned.push_back(rebinBinnedDataset(Form("%s_cat%d_%sDown01_sigma",new_name.c_str(),cat,syst->c_str()),down[cat],theEdges,cat));
      if (verbose_) checkHisto(up[cat]);
      if (verbose_) checkHisto(up_rebinned[cat]);
      if (verbose_) checkHisto(down[cat]);
      if (verbose_) checkHisto(down_rebinned[cat]);
			outFile->cd();
      gDirectory->cd(Form("%s:/",outFile->GetName()));
			up_rebinned[cat]->Write();
			down_rebinned[cat]->Write();
			//write(tFile,up_rebinned[cat]);
			//write(tFile,down_rebinned[cat]);
		}
    if (!getincludeLEP() && !getincludeVBF()){
        mergeHistograms(Form("%s_%sUp01_sigma",new_name.c_str(),syst->c_str()),up_rebinned[0]);
        mergeHistograms(Form("%s_%sDown01_sigma",new_name.c_str(),syst->c_str()),down_rebinned[0]);
    }
    if (getincludeVBF()) {
      for (int vCat=0; vCat<getnVBFCategories(); vCat++) {
        mergeHistograms(Form("%s_%sUp01_sigma",new_name.c_str(),syst->c_str()),up_rebinned[0],up_rebinned[vCat+1]);
        mergeHistograms(Form("%s_%sDown01_sigma",new_name.c_str(),syst->c_str()),down_rebinned[0],down_rebinned[vCat+1]);
      }
    }
    if (getincludeLEP()) {
      for (int lCat=0; lCat<getnLEPCategories(); lCat++) {
        mergeHistograms(Form("%s_%sUp01_sigma",new_name.c_str(),syst->c_str()),up_rebinned[0],up_rebinned[lCat+getnVBFCategories()+1]);
        mergeHistograms(Form("%s_%sDown01_sigma",new_name.c_str(),syst->c_str()),down_rebinned[0],down_rebinned[lCat+getnVBFCategories()+1]);
      }
    }
    if (verbose_) checkHisto(up_rebinned[0]);
    if (verbose_) checkHisto(down_rebinned[0]);
		outFile->cd();
    gDirectory->cd(Form("%s:/",outFile->GetName()));
		up_rebinned[0]->Write();
		down_rebinned[0]->Write();
		//write(tFile,up_rebinned[0]);
		//write(tFile,down_rebinned[0]);
    
    up.clear(); down.clear(); up_rebinned.clear(); down_rebinned.clear();
	}
 
}


void FMTRebin::rebinBackground(double mass, int binningMass){

  makeOutputHistogram(Form("th1f_bkg_mc_grad_%3.1f",mass),Form("th1f_bkg_BDT_grad_%3.1f",mass),binningMass);
  makeOutputHistogram(Form("th1f_data_grad_%3.1f",mass),Form("th1f_data_BDT_grad_%3.1f",mass),binningMass);

  // rebin sidebands for bkg mc and bkg model (from data)
  for (int sideband_i=1; sideband_i<=getnumberOfSidebands(); sideband_i++){
    makeOutputHistogram(Form("th1f_bkg_mc_%dhigh_grad_%3.1f",sideband_i,mass),Form("th1f_bkg_%dhigh_BDT_grad_%3.1f",sideband_i,mass),binningMass);
    makeOutputHistogram(Form("th1f_bkg_mc_%dlow_grad_%3.1f",sideband_i,mass),Form("th1f_bkg_%dlow_BDT_grad_%3.1f",sideband_i,mass),binningMass);
    makeOutputHistogram(Form("th1f_bkg_%dhigh_grad_%3.1f",sideband_i,mass),Form("th1f_data_%dhigh_BDT_grad_%3.1f",sideband_i,mass),binningMass);
    makeOutputHistogram(Form("th1f_bkg_%dlow_grad_%3.1f",sideband_i,mass),Form("th1f_data_%dlow_BDT_grad_%3.1f",sideband_i,mass),binningMass);
  }
}

void FMTRebin::rebinSignal(int thisMass, int binningMass){

    makeSignalOutputHistogram(Form("th1f_sig_grad_ggh_%d.0_%d.0",binningMass,thisMass),Form("th1f_sig_BDT_grad_ggh_%d.0",thisMass),binningMass);
    makeSignalOutputHistogram(Form("th1f_sig_grad_vbf_%d.0_%d.0",binningMass,thisMass),Form("th1f_sig_BDT_grad_vbf_%d.0",thisMass),binningMass);
    makeSignalOutputHistogram(Form("th1f_sig_grad_wzh_%d.0_%d.0",binningMass,thisMass),Form("th1f_sig_BDT_grad_wzh_%d.0",thisMass),binningMass);
    makeSignalOutputHistogram(Form("th1f_sig_grad_tth_%d.0_%d.0",binningMass,thisMass),Form("th1f_sig_BDT_grad_tth_%d.0",thisMass),binningMass);

}


TH1F* FMTRebin::getCombBackground(int mass_hyp, int cat, double bkgInSigThisMass){
  
  vector<TH1F*> vecBkgForBinning;
  pair<int,int> sidebandNumbs = getNsidebandsUandD(double(mass_hyp));
  int nLower=sidebandNumbs.first;
  int nHigher=sidebandNumbs.second;
  //int nHigher=3;

  // loop lower
  for (int sideband_i=getnumberOfSidebandGaps()+1; sideband_i<=getnumberOfSidebandGaps()+nLower; sideband_i++){
    vecBkgForBinning.push_back((TH1F*)tFile->Get(Form("th1f_bkg_%dlow_BDT_grad_%3d.0_cat%d",sideband_i,mass_hyp,cat)));
  }
  // loop higher
  for (int sideband_i=getnumberOfSidebandGaps()+1; sideband_i<=getnumberOfSidebandGaps()+nHigher; sideband_i++){
    vecBkgForBinning.push_back((TH1F*)tFile->Get(Form("th1f_bkg_%dhigh_BDT_grad_%3d.0_cat%d",sideband_i,mass_hyp,cat)));
  }
  // also put bkg MC in signal region into binning algo
  vecBkgForBinning.push_back((TH1F*)tFile->Get(Form("th1f_bkg_BDT_grad_%3d.0_cat%d",mass_hyp,cat)));

  // sum contributions from sidebands and normalise
  if (cat!=0) bkgInSigThisMass=-1;
  TH1F *bkgForBinning = sumMultiBinnedDatasets(Form("th1f_bkg_BDT_grad_all_%3d.0_cat%d",mass_hyp,cat),vecBkgForBinning,bkgInSigThisMass,true);

  //tFile->cd();
	outFile->cd();
  gDirectory->cd(Form("%s:/",outFile->GetName()));
  bkgForBinning->Write();
	//write(tFile,bkgForBinning);
  return bkgForBinning;
}

TH1F* FMTRebin::getCombSignal(int mass_hyp,int cat){

  vector<TH1F*> vecSigForBinning;

  vecSigForBinning.push_back((TH1F*)tFile->Get(Form("th1f_sig_BDT_grad_ggh_%3d.0_cat%d",mass_hyp,cat)));
  vecSigForBinning.push_back((TH1F*)tFile->Get(Form("th1f_sig_BDT_grad_vbf_%3d.0_cat%d",mass_hyp,cat)));
  vecSigForBinning.push_back((TH1F*)tFile->Get(Form("th1f_sig_BDT_grad_wzh_%3d.0_cat%d",mass_hyp,cat)));
  vecSigForBinning.push_back((TH1F*)tFile->Get(Form("th1f_sig_BDT_grad_tth_%3d.0_cat%d",mass_hyp,cat)));

  TH1F *sigForBinning = sumMultiBinnedDatasets(Form("th1f_sig_BDT_grad_all_%3d.0_cat%d",mass_hyp,cat),vecSigForBinning,-1,true);

  //tFile->cd();
	outFile->cd();
  gDirectory->cd(Form("%s:/",outFile->GetName()));
  sigForBinning->Write();
	//write(tFile,sigForBinning);
  return sigForBinning;
}

void FMTRebin::executeRebinning(int mass){
	
  checkMCMass(mass);
  if (verbose_) printRunOptions();
	
  cout << "File attempt" << endl;
	cout << tFile->GetName() << endl;
  cout << "Write to " << endl;
  cout << outFile->GetName() << endl;
	//RooWorkspace *tempWS = (RooWorkspace*)tFile->Get("cms_hgg_workspace");
	vector<double> BinEdges, VBFBinEdges,LEPBinEdges;
	if (!getrederiveOptimizedBinEdges()){
		BinEdges = getBinEdges(mass);
		VBFBinEdges = getVBFBinEdges(mass);
		LEPBinEdges = getLEPBinEdges(mass);
    // for now hard code lep tag which doesn't exist!
		//LEPBinEdges.push_back(2.);
		//LEPBinEdges.push_back(2.04);
		//setLEPBinEdges(mass,LEPBinEdges);
	}
	else {
		cout << "Running fit" << endl;
		double bkgInSigThisMass=(fitter->FitPow(double(mass))).first;
		TH1F *bkgForBinning[getNcats()];
		TH1F *sigForBinning[getNcats()];
		for (int cat=0; cat<getNcats(); cat++){
			bkgForBinning[cat] = getCombBackground(mass,cat,bkgInSigThisMass);
			sigForBinning[cat] = getCombSignal(mass,cat);
		}
		// get inclusive bin edges
    BinEdges = significanceOptimizedBinning(sigForBinning[0],bkgForBinning[0],20,mass);
    // set vbf edges from 1. in 0.04 steps
    VBFBinEdges.push_back(1.);
    for (int vCat=0; vCat<getnVBFCategories(); vCat++) VBFBinEdges.push_back(1.+(vCat+1)*0.04);
    // set lep edges from 2. in 0.04 steps
    LEPBinEdges.push_back(1.+(getnVBFCategories())*0.04);
    for (int lCat=0; lCat<getnLEPCategories(); lCat++) LEPBinEdges.push_back(1.+(getnVBFCategories()+lCat+1)*0.04);
    
    // set the bin edges here
		setBinEdges(mass,BinEdges);
		setVBFBinEdges(mass,VBFBinEdges);
		setLEPBinEdges(mass,LEPBinEdges);
		
	}
	cout << "Extrated bin edges - mass " << mass << endl;
  cout << (getBinEdges(mass)).size() << " " << (getVBFBinEdges(mass)).size() << " " << (getLEPBinEdges(mass)).size() << endl;
	// now rebin background at +- mHStep and do the fit
 	
	if (!justRebin_){
		vector<double> theMasses = getMHMasses(mass);
		for (vector<double>::iterator mH = theMasses.begin(); mH != theMasses.end(); mH++){
			fitter->FitPow(*mH);
			rebinBackground(*mH,mass);
		}

		// now rebin signal at +- 5GeV for interpolation
		vector<int> theMCMasses = getUandDMCMasses(mass);
		for (vector<int>::iterator mH = theMCMasses.begin(); mH != theMCMasses.end(); mH++){
			rebinSignal(*mH,mass);
		}
	}

	cout << Form("------ REBINNING MASS %3d.0 COMPLETE ------",mass) << endl;
}

bool FMTRebin::getcatByHand(){
  return catByHand_;
}
void FMTRebin::setcatByHand(bool catBH){
  catByHand_=catBH;
}
bool FMTRebin::getjustRebin(){
	return justRebin_;
}
void FMTRebin::setjustRebin(bool reb){
	justRebin_=reb;
}
