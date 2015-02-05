#include <map>

#include "TCanvas.h"
#include "TMath.h"
#include "TLatex.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "TF1.h"
#include "TMatrixD.h"

#include "../interface/ProfileMultiplePdfs.h"

ProfileMultiplePdfs::ProfileMultiplePdfs(){
  listOfPdfs.clear();
  //listOfPdfs = new RooArgList();
}

ProfileMultiplePdfs::~ProfileMultiplePdfs(){
  //delete listOfPdfs;
  for (map<string,pair<RooAbsPdf*,float> >::iterator m=listOfPdfs.begin(); m!=listOfPdfs.end(); m++) delete m->second.first;
}

void ProfileMultiplePdfs::addPdf(RooAbsPdf *pdf, float penaltyTerm){
  //listOfPdfs->add(*pdf);
  listOfPdfs.insert(pair<string,pair<RooAbsPdf*,float> >(pdf->GetName(),make_pair(pdf,penaltyTerm)));
}

void ProfileMultiplePdfs::clearPdfs(){
  listOfPdfs.clear();
}

void ProfileMultiplePdfs::printPdfs(){
  cout << "List of Pdfs: size(" << listOfPdfs.size() << ")" << endl;
  for (map<string,pair<RooAbsPdf*,float> >::iterator m=listOfPdfs.begin(); m!=listOfPdfs.end(); m++) { 
    RooAbsPdf *pdf = m->second.first;
    cout << "\t" << pdf->GetName() << " (penalty=" << m->second.second << ")" << endl;
    cout << "\t -- "; pdf->Print();
  }
}

void ProfileMultiplePdfs::plotNominalFits(RooAbsData *data, RooRealVar *var, int binning, string fname){
  var->setConstant(false);
  var->setVal(0.);
  for (map<string,pair<RooAbsPdf*,float> >::iterator m=listOfPdfs.begin(); m!=listOfPdfs.end(); m++) { 
    RooAbsPdf *pdf = m->second.first;
    pdf->fitTo(*data);
    //RooFitResult *fitRes = pdf->fitTo(*data,Save(true));
    //fitRes->floatParsInit().Print("v");
    //fitRes->floatParsFinal().Print("v");
    TCanvas *canv = new TCanvas();
    RooPlot *plot = var->frame();
    data->plotOn(plot,Binning(binning));
    pdf->plotOn(plot);
    pdf->paramOn(plot,RooFit::Layout(0.34,0.96,0.89),RooFit::Format("NEA",AutoPrecision(1)));
    plot->SetTitle(Form("%s_to_%s",pdf->GetName(),data->GetName()));
    plot->Draw();
    canv->Print(Form("%s_%s_to_%s.pdf",fname.c_str(),pdf->GetName(),data->GetName()));
    canv->Print(Form("%s_%s_to_%s.png",fname.c_str(),pdf->GetName(),data->GetName()));
  }
}

void ProfileMultiplePdfs::addToResultMap(float var, float minNll, RooAbsPdf* pdf){
  if (chosenPdfs.find(var)==chosenPdfs.end()){
    chosenPdfs.insert(pair<float,pair<float,RooAbsPdf*> >(var,make_pair(minNll,pdf)));
  }
  else {
    if (minNll<chosenPdfs[var].first){
      chosenPdfs[var] = pair<float,RooAbsPdf*>(minNll,pdf);
    }
  }
}

RooAbsPdf* ProfileMultiplePdfs::getBestFitPdf(float var){
  if (chosenPdfs.find(var)!=chosenPdfs.end()){
    return chosenPdfs[var].second;
  }
  else {
    float diff=1.e6;
    RooAbsPdf *closest = NULL;
    for (map<float,pair<float,RooAbsPdf*> >::iterator it=chosenPdfs.begin(); it!=chosenPdfs.end(); it++){
      if (TMath::Abs(it->first-var)<diff){
        closest = it->second.second;
        diff=TMath::Abs(it->first-var);
      }
    }
    return closest;
  }
}

double ProfileMultiplePdfs::getGlobalMinNLL(){
  return globalMinNLL;
}

void ProfileMultiplePdfs::saveValues(map<string,double> &vals, RooArgSet *params){
  RooRealVar *parg;
  TIterator *iter = params->createIterator();
  while ((parg=(RooRealVar*)iter->Next())) {
    vals.insert(pair<string,double>(parg->GetName(),parg->getVal()));
  }
}

void ProfileMultiplePdfs::setValues(map<string,double> vals, RooArgSet *params){
  RooRealVar *parg;
  TIterator *iter = params->createIterator();
  while ((parg=(RooRealVar*)iter->Next())) {
    parg->setVal(vals[parg->GetName()]);
  }
}

// no penalty
pair<double,map<string,TGraph*> > ProfileMultiplePdfs::profileLikelihood(RooAbsData *data, RooRealVar *obs_var, RooRealVar *var, float low, float high, float stepsize){
 
  if (chosenPdfs.size()>0)chosenPdfs.clear();
  var->setConstant(false); 
  // For initial fit, we use the values passed
  var->setRange(low,high);
  var->setVal(0.); // something sensible
  map<string,TGraph*> minNlls;
  globalMinNLL=1.e6;
  // first find global minNll for reference point
  map<string,double> mapOfValues;
  bool fitOk = true;
  for (map<string,pair<RooAbsPdf*,float> >::iterator m=listOfPdfs.begin(); m!=listOfPdfs.end(); m++) { 
    RooAbsPdf *pdf = m->second.first;
    RooFitResult *nom = pdf->fitTo(*data,PrintLevel(-1),PrintEvalErrors(-1),Warnings(false),Save(true));
    RooArgSet *params = pdf->getParameters(*obs_var);
    saveValues(mapOfValues,params);
    fitOk = (var->getVal()>low && var->getVal() < high && var->getError() >0.0001 && bestFitErr < 99.) ;// nom->status();
    std::cout << "Converged Fit? " << fitOk<< ", "<< pdf->GetName() << " Absolute NLL (uncorrected ) " << 2*nom->minNll() << std::endl;
    if (2*nom->minNll()+params->getSize()<globalMinNLL){
      fitOk = (var->getVal()>low && var->getVal() < high && var->getError() >0.0001 && bestFitErr < 99.) ;// nom->status();
      if (fitOk){
        bestFitVal = var->getVal();
        bestFitErr = var->getError();
        bestFitPdf = pdf;
      	globalMinNLL = 2*nom->minNll();
      }
    }
  }
  cout << "Best fit at:" << endl;
  cout << "\tnll = " << globalMinNLL << endl;
  cout << "\tmu  = " << bestFitVal << endl;
  cout << "\terr  = "<< bestFitErr << endl;
  cout << "\tpdf = " << bestFitPdf->GetName() << endl;
 
  stepsize=stepsize*bestFitErr;
  if (!fitOk){
	low = -1*high;
	high = high;
	stepsize = (high-low)/3;
  } else {
   low  = bestFitVal-2.5*bestFitErr;
   high = bestFitVal+2.5*bestFitErr;
  }
  std::vector<float> scanValues;
  for (float v=low; v<(high+stepsize); v+=stepsize){
    scanValues.push_back(v);	
    if (fitOk && v<bestFitVal && v+stepsize>(bestFitVal+stepsize/10.)){ // dont add it if its very close since 
	scanValues.push_back((float)bestFitVal);
    }
  }
  std::cout << "N Points to scan == " << scanValues.size() <<std::endl;
  var->setRange(low-0.5*fabs(low),high+0.5*fabs(high));
  // now perform the scan
  cout << "Scanning...." << endl;
  for (map<string,pair<RooAbsPdf*,float> >::iterator m=listOfPdfs.begin(); m!=listOfPdfs.end(); m++) { 
    RooAbsPdf *pdf = m->second.first;
    cout << "\t pdf " << distance(listOfPdfs.begin(),m) << "/" << listOfPdfs.size() << " (" << pdf->GetName() << ")" << endl;
    TGraph *thisNLL = new TGraph();
    int p=0;
    for (std::vector<float>::iterator vit = scanValues.begin();vit!=scanValues.end();vit++){
      float v = *vit;
      cout << Form("\t%5.1f%%\r",100.*(v-low)/(high-low)) << flush;
      RooArgSet *params = (RooArgSet*)pdf->getParameters(*var);
      setValues(mapOfValues,params);
      var->setConstant(false);
      var->setVal(v);
      var->setConstant(true);
      if (fitOk){
         RooFitResult *scan = pdf->fitTo(*data,PrintLevel(-1),PrintEvalErrors(-1),Warnings(false),Save(true));
	 if (scan->edm() < 0.01) {
           thisNLL->SetPoint(p,v,TMath::Min(25.,(2*scan->minNll()-globalMinNLL)));
           addToResultMap(v,TMath::Min(25.,(2*scan->minNll()-globalMinNLL)),pdf);
	 } else{
           thisNLL->SetPoint(p,v,25.);
           addToResultMap(v,25.,pdf);
 	 }	
         delete scan;
      } else {
         thisNLL->SetPoint(p,v,25.);
         addToResultMap(v,25.,pdf);
      }
      p++;
    }
    cout << endl;
    thisNLL->SetName(Form("minnll_%s_%s",pdf->GetName(),data->GetName()));
    minNlls.insert(pair<string,TGraph*>(pdf->GetName(),thisNLL));
  }
  var->setConstant(false);
  return pair<double,map<string,TGraph*> >(globalMinNLL,minNlls);
}

pair<double,map<string,TGraph*> > ProfileMultiplePdfs::computeEnvelope(pair<double,map<string,TGraph*> > minNllCurves, string name, float penalty){

  // note penalty is always scaled by the number of params in the background part of the fit
  // correct overall min for nparams
  double envelopeMin=minNllCurves.first+penalty*(bestFitPdf->getVariables()->getSize()-3);
  map<string,TGraph*> minNlls=minNllCurves.second;
  map<string,TGraph*> correctedMinNlls(minNlls);
  int npoints=-1;// avoid uninitialised variable error in cmssw;
  // checks graphs are compatible;
  for (map<string,TGraph*>::iterator it=minNlls.begin(); it!=minNlls.end(); it++){
    map<string,TGraph*>::iterator fIt=minNlls.begin();
    npoints=fIt->second->GetN();
    if (fIt->second->GetN()!=it->second->GetN()) {
      cerr << "ERROR -- npoints in graphs do not match: " << fIt->first << " and " << it->second << endl;
      exit(1);
    }
  }
  // now find envelope and apply correction
  float globalMin=1.e8;
  std::string bestFitPdf;
  for (int p=0; p<npoints; p++){
    float minPoint=1.e8;
    double x,y;
    for (map<string,TGraph*>::iterator mIt=minNlls.begin(); mIt!=minNlls.end(); mIt++){
      // find nBkgParams in pdf
      if (listOfPdfs.find(mIt->first)==listOfPdfs.end()){
        cerr << "ERROR - pdf " << mIt->first << " not found in list of pdfs" << endl;
        exit(1);
      }
      // subtract 4 = (mass,mu,bkgYield)
      int nBkgParams = listOfPdfs[mIt->first].first->getVariables()->getSize()-3;
      mIt->second->GetPoint(p,x,y);
      double valWithPenalty = y+(nBkgParams*penalty);
      if (valWithPenalty<minPoint){
        minPoint=valWithPenalty;
        //envelopeNLL->SetPoint(p,x,valWithPenalty);
        if (minPoint<globalMin) {
		globalMin=minPoint;
		bestFitPdf=mIt->first;
	}
      }
      correctedMinNlls[mIt->first]->SetPoint(p,x,valWithPenalty);
      correctedMinNlls[mIt->first]->SetTitle((mIt->first).c_str());
      //mIt->second->SetPoint(p,x,valWithPenalty);
    }
  }
  
  // loop again correcting back to zero and resetting best fit
  TGraph *envelopeNLL = new TGraph();
  for (int p=0; p<npoints; p++){
    float minPoint=1.e8;
    double x,y;
    for (map<string,TGraph*>::iterator mIt=correctedMinNlls.begin(); mIt!=correctedMinNlls.end(); mIt++){
      mIt->second->GetPoint(p,x,y);
      if (y<minPoint){
        minPoint=y;
        envelopeNLL->SetPoint(p,x,y-globalMin);
      }
      mIt->second->SetPoint(p,x,y-globalMin);
    }
  }

  // USE MIN POINT
  /*
  for (float v=low; v<high; v+=stepsize){
    float minPoint=1.e8;
    for (map<string,TGraph*>::iterator mIt=minNlls.begin(); mIt!=minNlls.end(); mIt++){
      // find nBkgParams in pdf
      if (listOfPdfs.find(mIt->first)==listOfPdfs.end()){
        cerr << "ERROR - pdf " << mIt->first << " not found in list of pdfs" << endl;
        exit(1);
      }
      // subtract 4 = (mass,mu,bkgYield)
      int nBkgParams = listOfPdfs[mIt->first].first->getVariables()->getSize()-3;
      
      double valWithPenalty = mIt->second->Eval(v)+(nBkgParams*penalty);
      if (valWithPenalty<minPoint){
        minPoint=valWithPenalty;
        envelopeNLL->SetPoint(p,v,valWithPenalty);
      }
    }
    p++;
  }
  */
  envelopeNLL->SetName(name.c_str());
  envelopeNLL->SetTitle(bestFitPdf.c_str());
	
  correctedMinNlls.insert(pair<string,TGraph*>("envelope",envelopeNLL));
  std::cout << " Returning NLL Profile " << std::endl;
  return pair<double,map<string,TGraph*> >(envelopeMin,correctedMinNlls);

}

// can include penalty here and will give envelope
map<string,TGraph*> ProfileMultiplePdfs::profileLikelihoodEnvelope(RooAbsData *data, RooRealVar *var, float low, float high, float stepsize){
  
  if (chosenPdfs.size()>0)chosenPdfs.clear();
  var->setConstant(false);
  // For initial fit, we use the values passed
  var->setRange(low,high);
  var->setVal(0.);
  map<string,TGraph*> minNlls;
  TGraph *globalNLL = new TGraph();
  globalMinNLL=1.e6;
  // first find global minNll for reference point
  bool fitOk = true;
  for (map<string,pair<RooAbsPdf*,float> >::iterator m=listOfPdfs.begin(); m!=listOfPdfs.end(); m++) { 
    RooAbsPdf *pdf = m->second.first;
    float penalty = m->second.second;
    RooFitResult *nom = pdf->fitTo(*data,PrintLevel(-1),PrintEvalErrors(-1),Warnings(false),Save(true));
    if (2*nom->minNll()+penalty<globalMinNLL){
      fitOk = (var->getVal()>low && var->getVal() < high && var->getError() >0.0001 && bestFitErr < 99.) ;// nom->status();
      if (fitOk){
        bestFitVal = var->getVal();
        bestFitErr = var->getError();
        bestFitPdf = pdf;
      	globalMinNLL = 2*nom->minNll()+penalty;
      }
    }
  }
  cout << "Best fit at:" << endl;
  cout << "\tnll = " << globalMinNLL << endl;
  cout << "\tmu  = " << bestFitVal << endl;
  cout << "\terr  = "<< bestFitErr << endl;
  cout << "\tpdf = " << bestFitPdf->GetName() << endl;

  stepsize=stepsize*bestFitErr;

  if (!fitOk){
	low = -1*high;
	high = high;
	stepsize = (high-low)/3;
  } else {
   low  = bestFitVal-2.5*bestFitErr;
   high = bestFitVal+2.5*bestFitErr;
  }
  std::vector<float> scanValues;
  std::cout << "N Points to scan == " << (high-low) /stepsize <<std::endl;

  for (float v=low; v<(high+stepsize); v+=stepsize){
    scanValues.push_back(v);
    if (fitOk && v<bestFitVal && v+stepsize>bestFitVal) scanValues.push_back((float)bestFitVal);
  }
  var->setRange(low-0.2*fabs(low),high+0.2*fabs(high));
  // now perform the scan
  for (map<string,pair<RooAbsPdf*,float> >::iterator m=listOfPdfs.begin(); m!=listOfPdfs.end(); m++) { 
    RooAbsPdf *pdf = m->second.first;
    float penalty = m->second.second;
    TGraph *thisNLL = new TGraph();
    int p=0;
    for (std::vector<float>::iterator vit = scanValues.begin();vit!=scanValues.end();vit++){
      float v = *vit;
      var->setConstant(false);
      var->setVal(v);
      var->setConstant(true);
      if (fitOk) {
	  RooFitResult *scan = pdf->fitTo(*data,PrintLevel(-1),PrintEvalErrors(-1),Warnings(false),Save(true));
	  if (scan->edm() < 0.01) {
      //RooFitResult *scan = pdf->fitTo(*data,Save(true));
      	    thisNLL->SetPoint(p,v,(2*scan->minNll()-globalMinNLL+penalty));
      	    addToResultMap(v,2*scan->minNll()-globalMinNLL+penalty,pdf);
	  } else {
      	    thisNLL->SetPoint(p,v,25);
      	    addToResultMap(v,25,pdf);
	  }
          delete scan;
      } else {
      	  thisNLL->SetPoint(p,v,(25));
      	  addToResultMap(v,25,pdf);
      }
      //thisNLL->SetPoint(p,v,(scan->minNll()+penalty));
      //addToResultMap(v,scan->minNll()+penalty,pdf);
      p++;
    }
    thisNLL->SetName(Form("minnll_%s_%s",pdf->GetName(),data->GetName()));
    minNlls.insert(pair<string,TGraph*>(thisNLL->GetName(),thisNLL));
  }
  int p=0;
  for (std::vector<float>::iterator vit = scanValues.begin();vit!=scanValues.end();vit++){
    float v = *vit;
    float minPoint=1.e8;
    for (map<string,TGraph*>::iterator mIt=minNlls.begin(); mIt!=minNlls.end(); mIt++){
      if (mIt->second->Eval(v)<minPoint){
        minPoint=mIt->second->Eval(v);
        globalNLL->SetPoint(p,v,minPoint);
      }
    }
    p++;
  }
  // release constraint
  var->setConstant(false);
  globalNLL->SetName("envelope");
  minNlls.insert(pair<string,TGraph*>(globalNLL->GetName(),globalNLL));
  int n =globalNLL->GetN();
  double *x = globalNLL->GetX();
  double *y = globalNLL->GetY();
  int locmin = TMath::LocMin(n,y);
  cout << "Min of graph at x=" << x[locmin] << " y=" << y[locmin] << endl;  
  return minNlls;
}

void ProfileMultiplePdfs::plot(map<string,TGraph*> minNlls, string fname){
  
  if (minNlls.find("envelope")==minNlls.end()){
    cerr << "ERROR - envelope graph not found in map minNlls" << endl;
    exit(1);
  }
  else {
    TCanvas *canv = new TCanvas();
    TGraph *envelope = minNlls["envelope"];
    envelope->SetLineWidth(2);
    envelope->SetLineColor(kRed);
    envelope->Draw("ALP");

    TLatex *lat = new TLatex();
    lat->SetTextSize(0.015);
    lat->SetTextAngle(75.);

    for (map<string,TGraph*>::iterator it=minNlls.begin(); it!=minNlls.end(); it++){
      if (it->first!="envelope"){
        it->second->SetLineColor(kBlue+1);
        it->second->SetLineStyle(kDashed);
        it->second->Draw("LPsame");

	TGraph *gr = getMinPoints(it->second);
	double xx,yy;
	if (gr) {
		 gr->GetPoint(1,xx,yy);
	         lat->DrawLatex(xx,yy,(it->first).c_str());
	}
      }
    }
    canv->Print(Form("%s.pdf",fname.c_str()));
    canv->Print(Form("%s.png",fname.c_str()));
    delete canv;
    delete lat;
  }

}

void ProfileMultiplePdfs::print(map<string,TGraph*> minNlls, float low, float high, float stepsize){
  
  for (map<string,TGraph*>::iterator it=minNlls.begin(); it!=minNlls.end(); it++){
    cout << Form("%8s  ",it->first.c_str());
  }
  cout << endl;
  for (float v=low; v<(high+stepsize); v+=stepsize){
    cout << Form("%2.2f  ",v);
    for (map<string,TGraph*>::iterator it=minNlls.begin(); it!=minNlls.end(); it++){
      cout << Form("%2.4f  ",it->second->Eval(v));
    }
    cout << endl;
    cout << "Best fit = " << this->getBestFitPdf(v)->GetName() << endl;
  }
  
}

pair<double,double> ProfileMultiplePdfs::getGraphMin(TGraph *graph){
  int n =graph->GetN();
  double *x = graph->GetX();
  double *y = graph->GetY();
  int locmin = TMath::LocMin(n,y);
  return pair<double,double>(x[locmin],y[locmin]);
}

pair<double,double> ProfileMultiplePdfs::interpError(TGraph *nll, float sigma){
  
  assert(nll->GetN()>0);
  float deltaNLLcrossing = sigma*sigma;
  TGraph *grRotUpp = new TGraph();
  TGraph *grRotLow = new TGraph();
  int pLow=0,pHigh=0;
  pair<double,double> graphMin = getGraphMin(nll);
  for (int p=0; p<nll->GetN(); p++){
    double x,y;
    nll->GetPoint(p,x,y);
    if (x<=graphMin.first) {
      grRotLow->SetPoint(pLow,y,x);
      pLow++;
    }
    if (x>=graphMin.first) {
      grRotUpp->SetPoint(pHigh,y,x);
      pHigh++;
    }
  }
  return pair<double,double>(grRotLow->Eval(deltaNLLcrossing),grRotUpp->Eval(deltaNLLcrossing));
}

double ProfileMultiplePdfs::interpErrorSymmetric(TGraph *nll, float sigma){

  pair<double,double> error = interpError(nll,sigma);
  double muVal = quadInterpMinimum(nll);
  return ((error.second-muVal)+(muVal-error.first))/2.;
}

double ProfileMultiplePdfs::quadInterpMinimum(TGraph *nll, float stepsize){

  assert(nll->GetN()>0);
  int lowest_p=-1;// avoid uninitialised variable error in cmssw;
  double min_y=1.e8;
  double x,y;
  for (int p=0; p<nll->GetN(); p++){
    nll->GetPoint(p,x,y);
    if (y<min_y){
      min_y=y;
      lowest_p=p;
    }
  }
  
  // problematic if lowest point is at boundary
  assert(lowest_p!=0);
  assert(lowest_p<nll->GetN()-1);

  TGraph *temp = new TGraph();
  nll->GetPoint(lowest_p-1,x,y);
  float xlow=x;
  temp->SetPoint(0,x,y);
  nll->GetPoint(lowest_p,x,y);
  temp->SetPoint(1,x,y);
  nll->GetPoint(lowest_p+1,x,y);
  float xhigh=x;
  temp->SetPoint(2,x,y);

  TF1 func("f1","[0]*x*x+[1]*x+[2]",xlow,xhigh);
  temp->Fit(&func,"QN");

  double min=1.e8;
  double bestFit=-999.;// avoid uninitialised variable error in cmssw;
  for (float scan=xlow; scan<(xhigh+stepsize); scan+=stepsize){
    double val = func.Eval(scan);
    if (val<min){
      min=val;
      bestFit = scan;
    }
  }
  delete temp;
  return bestFit;
}

pair<double,double> ProfileMultiplePdfs::quadInterpMin(TGraph *g, float stepsize){
  
  double xlow, xhigh, y;
  g->GetPoint(0,xlow,y);
  g->GetPoint(g->GetN()-1,xhigh,y);
  TF1 func("f1","[0]*x*x+[1]*x+[2]",xlow,xhigh);

  g->Fit(&func,"QN");

  double min=1.e8;
  double bestFit=-999.;// avoid uninitialised variable error in cmssw;
  for (double v=xlow; v<(xhigh+stepsize); v+=stepsize){
    if (func.Eval(v)<min){
      min=func.Eval(v);
      bestFit=v;
    }
  }
  return pair<double,double>(bestFit,min);
}

double ProfileMultiplePdfs::quadInterpCrossing(TGraph *g, double crossing, float stepsize){
  
  double xlow, xhigh, y;
  g->GetPoint(0,xlow,y);
  g->GetPoint(g->GetN()-1,xhigh,y);
  TF1 func("f1","[0]*x*x+[1]*x+[2]",xlow,xhigh);

  g->Fit(&func,"QN");

  double diff=1.e8;
  double fitPoint=-999.;// avoid uninitialised variable error in cmssw;
  for (double v=xlow; v<(xhigh+stepsize); v+=stepsize){
    if (TMath::Abs(func.Eval(v)-crossing)<diff){
      diff=TMath::Abs(func.Eval(v)-crossing);
      fitPoint=v;
    }
  }
  return fitPoint;
}

TGraph* ProfileMultiplePdfs::getMinPoints(TGraph *graph){


  int lowest_p=-1;// avoid uninitialised variable error in cmssw 
  double min_y=1.e8;
  double x,y;
  for (int p=0; p<graph->GetN(); p++){
    graph->GetPoint(p,x,y);
    if (y<min_y){
      min_y=y;
      lowest_p=p;
    }
  }
  if (lowest_p==0 || lowest_p==graph->GetN()-1) {
    return NULL;
  }
  else {
    TGraph *ret = new TGraph();
    graph->GetPoint(lowest_p-1,x,y);
    ret->SetPoint(0,x,y);
    double yleft = y;
    graph->GetPoint(lowest_p,x,y);
    ret->SetPoint(1,x,y);
    double ymin = y;
    graph->GetPoint(lowest_p+1,x,y);
    ret->SetPoint(2,x,y);
    double yright = y;
    if ( fabs(yright-ymin) >= 1. || fabs(yleft-ymin) >=1 ) return NULL;
    return ret;
  }
}

TGraph* ProfileMultiplePdfs::getCrossingPointHigh(TGraph *graph, float crossing){
 
  int crossing_p=-1;// avoid uninitialised variable error in cmssw 
  double x,y;
  for (int p=graph->GetN()-1; p>=0; p--){
    graph->GetPoint(p,x,y);
    if (y<crossing){
      crossing_p=p;
      break;
    }
  }
  if (crossing_p==0 || crossing_p==graph->GetN()-1) {
    return NULL;
  }
  else {
    TGraph *ret = new TGraph();
    graph->GetPoint(crossing_p-1,x,y);
    ret->SetPoint(0,x,y);
    graph->GetPoint(crossing_p,x,y);
    ret->SetPoint(1,x,y);
    graph->GetPoint(crossing_p+1,x,y);
    ret->SetPoint(2,x,y);
    return ret;
  }
}

TGraph* ProfileMultiplePdfs::getCrossingPointLow(TGraph *graph, float crossing){
 
  int crossing_p=-1;// avoid uninitialised variable error in cmssw;
  double x,y;
  for (int p=0; p<graph->GetN(); p++){
    graph->GetPoint(p,x,y);
    if (y<crossing){
      crossing_p=p;
      break;
    }
  }
  if (crossing_p==0 || crossing_p==graph->GetN()-1) {
    return NULL;
  }
  else {
    TGraph *ret = new TGraph();
    graph->GetPoint(crossing_p-1,x,y);
    ret->SetPoint(0,x,y);
    graph->GetPoint(crossing_p,x,y);
    ret->SetPoint(1,x,y);
    graph->GetPoint(crossing_p+1,x,y);
    ret->SetPoint(2,x,y);
    return ret;
  }
}

pair<double,pair<double,double> > ProfileMultiplePdfs::getMinAndErrorLinear(TGraph *graph, float sigma, bool safemode){
  pair<double,pair<double,double> > failedResult(999.,make_pair(-9999.,+9999.));
  if (graph->GetN()==0) {
    if (safemode) return failedResult;
    else {
			cout << "This graph has no points" << endl;
			exit(1);
		}
  }
  float nll_crossing=sigma*sigma;

	// min
	double miny=1.e8;
	double x,y,minx;
	int minP=0;
	for (int p=0; p<graph->GetN(); p++){
		graph->GetPoint(p,x,y);
		if (y<miny){
			miny=y;
			minx=x;
			minP=p;
		}
	}
	if (minP==0 || minP==graph->GetN()) minx=failedResult.first;
	//failedResult.first = minx;

  TGraph *lowPoints = getCrossingPointLow(graph,nll_crossing);
  TGraph *highPoints = getCrossingPointHigh(graph,nll_crossing);
  
  if (!lowPoints || !highPoints) {
    //if (safemode) return failedResult;
    if (!safemode){ //else {	
			cout << "There aren't enough points" << endl;
			exit(1);
		}
  }
	double eLow=0;
	double eHigh=0;
	double x0,x1;
	double y0,y1;
	double x2;
	double y2;
	if (lowPoints){
	  // low err
	  // Swap the order for EVALS	
	  for (int p=0; p<lowPoints->GetN(); p++){
		lowPoints->GetPoint(p,x,y);
		lowPoints->SetPoint(p,y,x);
	  }
	  lowPoints->GetPoint(0,x0,y0);
	  lowPoints->GetPoint(1,x1,y1);

	  if (fabs(x0-x1) > 1){
		double nsigma = TMath::Sqrt(x1);
		eLow = minx-(minx-y1)/nsigma;
	  } else {
		eLow = lowPoints->Eval(nll_crossing);
	  }
	} else {
		eLow = ((failedResult.second).first);
	}

	if (highPoints){
	  // high err
	  for (int p=0; p<highPoints->GetN(); p++){
		highPoints->GetPoint(p,x,y);
		highPoints->SetPoint(p,y,x);
	  }
	  highPoints->GetPoint(2,x2,y2);
	  highPoints->GetPoint(1,x1,y1);
	  //std::cout << "Points x ... " << x0 <<", " << x1 <<", "<< x2 <<std::endl;
	  //std::cout << " y  " << y0 << ", " << ", " << y1 << ", " << y2 <<std::endl;
	  if (fabs(x2-x1) > 1){
	  	double nsigma = TMath::Sqrt(x1);
		eHigh = minx+(y1-minx)/nsigma;
	  } else {
		eHigh = highPoints->Eval(nll_crossing);
	  }
	} else {
		eHigh = ((failedResult.second).second);
	
	}
	  //std::cout << "Results min,low,high = " << minx << ", "<<eLow << ", " << eHigh << std::endl;
	return make_pair(minx,make_pair(eLow,eHigh));
}

pair<double,pair<double,double> > ProfileMultiplePdfs::getMinAndErrorNoScale(TGraph *graph, float sigma, float stepsize, bool safemode){
	
	double min=1.e5;
	for (int p=0; p<graph->GetN(); p++){
		double x,y;
		graph->GetPoint(p,x,y);
		if (y<min) min=y;
	}
	
	TGraph *g = new TGraph();
	for (int p=0; p<graph->GetN(); p++){
		double x,y;
		graph->GetPoint(p,x,y);
		y=y-min;
		g->SetPoint(p,x,y);
	}
	pair<double,pair<double,double> > res = getMinAndError(g,sigma,stepsize,safemode);
	delete g;
	return res;
}

pair<double,pair<double,double> > ProfileMultiplePdfs::getMinAndError(TGraph *graph, float sigma, float stepsize, bool safemode){
  
  pair<double,pair<double,double> > failedResult(999.,make_pair(-9999.,+9999.));
  if (graph->GetN()==0) {
    if (safemode) return failedResult;
    else {
			cout << "This graph has no points" << endl;
			exit(1);
		}
  }
  float nll_crossing=sigma*sigma;

  TGraph *minPoints = getMinPoints(graph);
  TGraph *lowPoints = getCrossingPointLow(graph,nll_crossing);
  TGraph *highPoints = getCrossingPointHigh(graph,nll_crossing);
  
  if (minPoints) { 
	double x,y;
	minPoints->GetPoint(1,x,y);
	failedResult.first =x;
  
  }

  if (!minPoints || !lowPoints || !highPoints) {
    if (safemode) return failedResult;
    else {	
			cout << "There aren't enough points" << endl;
			exit(1);
		}
  }

  pair<double,double> minP = quadInterpMin(minPoints,stepsize);
  double low = quadInterpCrossing(lowPoints,nll_crossing+minP.second,stepsize);
  double high = quadInterpCrossing(highPoints,nll_crossing+minP.second,stepsize);

  return pair<double,pair<double,double> >(minP.first,make_pair(low,high));

}

pair<double,pair<double,double> > ProfileMultiplePdfs::getMinAndErrorAsymm(TGraph *graph, float sigma, float stepsize, bool safemode){
  
  pair<double,pair<double,double> > info = getMinAndError(graph,sigma,stepsize,safemode);
  return pair<double,pair<double,double> >(info.first,make_pair(info.first-info.second.first,info.second.second-info.first));

}

vector<double> ProfileMultiplePdfs::getMinAndErrorAsymmVec(TGraph *graph, float sigma, float stepsize, bool safemode){
  
  pair<double,pair<double,double> > info = getMinAndError(graph,sigma,stepsize,safemode);
  vector<double> res;
  res.push_back(info.first);
  res.push_back(info.first-info.second.first);
  res.push_back(info.second.second-info.first);
  return res;
}
vector<double> ProfileMultiplePdfs::getMinAndErrorLinearAsymmVec(TGraph *graph, float sigma, float stepsize, bool safemode){
  
  pair<double,pair<double,double> > info = getMinAndErrorLinear(graph,sigma,safemode);
  vector<double> res;
  res.push_back(info.first);
  res.push_back(info.first-info.second.first);
  res.push_back(info.second.second-info.first);
  return res;
}

pair<double,double> ProfileMultiplePdfs::getMinAndErrorSymm(TGraph *graph, float sigma, float stepsize, bool safemode){
  
  pair<double,pair<double,double> > info = getMinAndError(graph,sigma,stepsize,safemode);
  double symmerr = ((info.first-info.second.first)+(info.second.second-info.first))/2.;
  return pair<double,double>(info.first,symmerr);

}


double ProfileMultiplePdfs::quadInterpMinimumOld(TGraph *nll, float stepsize){
 
  assert(nll->GetN()>0);
  int lowest_p=-1;// avoid uninitialised variable error in cmssw;
  double min_y=1.e8;
  double x,y;
  for (int p=0; p<nll->GetN(); p++){
    nll->GetPoint(p,x,y);
    if (y<min_y){
      min_y=y;
      lowest_p=p;
    }
  }

  double X1,X2,X3,Y1,Y2,Y3;
  nll->GetPoint(lowest_p,X2,Y2);
  nll->GetPoint(lowest_p-1,X1,Y1);
  nll->GetPoint(lowest_p+1,X3,Y3);
  TF1 func("f1","[0]*x*x+[1]*x+[2]",-5,5);
  
  double entries[9];
  entries[0]=X1*X1; entries[1]=X1; entries[2]=1;
  entries[3]=X2*X2; entries[4]=X2; entries[5]=1;
  entries[6]=X3*X3; entries[7]=X3; entries[8]=1;

  //create the Matrix;
  TMatrixD M(3,3);
  M.SetMatrixArray(entries);
  M.Invert();

  double a = M(0,0)*Y1+M(0,1)*Y2+M(0,2)*Y3;
  double b = M(1,0)*Y1+M(1,1)*Y2+M(1,2)*Y3;
  double c = M(2,0)*Y1+M(2,1)*Y2+M(2,2)*Y3;

  func.SetParameter(0,a);
  func.SetParameter(1,b);
  func.SetParameter(2,c);

  double min=1.e8;
  double bestFit=-999.;// avoid uninitialised variable error in cmssw;
  for (float scan=X2-0.1; scan<X2+0.1; scan+=stepsize){
    double val = func.Eval(scan);
    if (val<min){
      min=val;
      bestFit = scan;
    }
  }
  return bestFit;

}

double ProfileMultiplePdfs::getPull(TGraph *nllIn, float trueVal, float stepsize, bool safemode){

  if (!nllIn) return 999.;
  if (nllIn->GetN()<3) return 999.;
  // Step 1, Run through the curve and clean it up
  double xs,ys;
  double xs1,ys1;
  TGraph *nll = new TGraph();
  int ccount = 0;
  for (int p=0; p<nllIn->GetN()-1; p++){	
    nllIn->GetPoint(p,xs,ys);
    nllIn->GetPoint(p+1,xs1,ys1);
    if ( fabs(ys1-ys) >=2 ) continue ; // This is quite a leap!
    if (ys1<25){
      nll->SetPoint(ccount,xs1,ys1);
      ccount++;
    }
  }
  
  TGraph *minPoints = getMinPoints(nll);

  if (!minPoints) {
    if (safemode) return 999.;
    else exit(1);
  }

  //pair<double,double> minP = quadInterpMin(minPoints,stepsize);
  // Use linear for minimum
   
  double x,y;
  minPoints->GetPoint(1,x,y);
  double truthNll = nll->Eval(trueVal);
  double bestFitNll = y;
  double bestFit    = x;
  
  if (truthNll<bestFitNll) {
    if (safemode) return 999.;
    else exit(1);
  }

  if ( fabs(bestFit-trueVal)< 0.001) return 0;

  // Calculate where the end points are 
  double xleft,yleft,xright,yright;
  nll->GetPoint(0,xleft,yleft);
  nll->GetPoint(nll->GetN()-1,xright,yright);
  if (trueVal < xleft || trueVal > xright){
	// try another definition	
	// First find the furthest we can get to  
	//pair<double,pair<double,double> > altPull = getMinAndErrorLinear(nll,1.,safemode);	
	if (bestFit <= trueVal){
	    // use yright 
	    double yrightsig = TMath::Sqrt(yright-bestFitNll);
	    double err = (1./yrightsig)*(xright-bestFit);
	    return (bestFit-trueVal)/err;
	} else{
	    double yleftsig = TMath::Sqrt(yleft-bestFitNll);
	    double err = (1./yleftsig)*(bestFit-xleft);
	    return (bestFit-trueVal)/err;
	}
  }

  double factor=1.;
  if (bestFit<trueVal) factor=-1.;
  //if (fabs(x-trueVal)<=2*stepsize )return 0;
  minPoints->GetPoint(0,x,y);
  double nll0 = y;
  minPoints->GetPoint(2,x,y);
  double nll2 = y;
  double nearestDist=0;
  if (bestFit<trueVal) nearestDist = fabs(nll0-bestFitNll);
  else  nearestDist = fabs(nll2-bestFitNll);

  if ( (truthNll-bestFitNll) < nearestDist ) truthNll = minPoints->Eval(trueVal);//factor = 0;
  return TMath::Sqrt(fabs(truthNll-bestFitNll))*factor;

}
