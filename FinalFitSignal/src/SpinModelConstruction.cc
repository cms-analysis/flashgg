#include <fstream>

#include "RooPlot.h"
#include "RooConstVar.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooAbsReal.h"

#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/predicate.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string/replace.hpp"
#include "boost/algorithm/string/erase.hpp"

#include "../interface/SpinModelConstruction.h"

using namespace std;
using namespace RooFit;
using namespace boost;

SpinModelConstruction::SpinModelConstruction(RooRealVar *massVar, RooRealVar *MHvar, RooRealVar *intL, int mh, string proc, int cat, int sqrts, string systematicsFileName, double scaleGravGG, double scaleGravQQ, int verbosity, bool quadraticSigmaSum):
	mass(massVar),
	MH(MHvar),
	intLumi(intL),
	mh_(mh),
	proc_(proc),
	cat_(cat),
	sqrts_(sqrts),
	scaleGravGG_(scaleGravGG),
	scaleGravQQ_(scaleGravQQ),
	verbosity_(verbosity),
	quadraticSigmaSum_(quadraticSigmaSum),
	systematicsSet_(false)
{

	norm = new Normalization_8TeV();
	norm->Init(sqrts_);
	TGraph *brGraph = norm->GetBrGraph();
	brSpline = graphToSpline(Form("fbr_%dTeV",sqrts_),brGraph);
  
  string procs[8] = {"ggh","vbf","wzh","wh","zh","tth","gg_grav","qq_grav"};
  for (int i=0; i<8; i++){
    TGraph *xsGraph = norm->GetSigmaGraph(procs[i].c_str());
    RooSpline1D *xsSpline = graphToSpline(Form("fxs_%s_%dTeV",procs[i].c_str(),sqrts_),xsGraph);
    xsSplines.insert(pair<string,RooSpline1D*>(procs[i],xsSpline));
  }

	loadSignalSystematics(systematicsFileName);
}

SpinModelConstruction::~SpinModelConstruction(){}

void SpinModelConstruction::setRVdatasets(map<int,RooDataSet*> data){
  rvDatasets = data;
}

void SpinModelConstruction::setWVdatasets(map<int,RooDataSet*> data){
  wvDatasets = data;
}

void SpinModelConstruction::setSTDdatasets(map<int,RooDataSet*> data){
  stdDatasets = data;
}

void SpinModelConstruction::makeSTDdatasets(){
	RooDataSet *data = (RooDataSet*)rvDatasets[mh_]->Clone(Form("sig_%s_mass_m%d_cat%d",proc_.c_str(),mh_,cat_));
	data->append(*wvDatasets[mh_]);
	stdDatasets.insert(pair<int,RooDataSet*>(mh_,data));
}

void SpinModelConstruction::setRVfitParams(map<int,map<string,RooRealVar*> > fP){
	fitParamsRV = fP;
}

void SpinModelConstruction::setWVfitParams(map<int,map<string,RooRealVar*> > fP){
	fitParamsWV = fP;
}

RooSpline1D* SpinModelConstruction::graphToSpline(string name, TGraph *graph){
  
  vector<double> xValues, yValues;
  for (double mh=mh_-5.; mh<mh_+5.25; mh+=0.5){
    xValues.push_back(mh);
    yValues.push_back(graph->Eval(mh));
  }
  RooSpline1D *res = new RooSpline1D(name.c_str(),name.c_str(),*MH,xValues.size(),&(xValues[0]),&(yValues[0]));
  return res;
}

RooSpline1D* SpinModelConstruction::graphToSpline(string name, TGraph *graph, RooAbsReal *var){
  
  vector<double> xValues, yValues;
  for (double mh=mh_-5.; mh<mh_+5.25; mh+=0.5){
    xValues.push_back(mh);
    yValues.push_back(graph->Eval(mh));
  }
  RooSpline1D *res = new RooSpline1D(name.c_str(),name.c_str(),*var,xValues.size(),&(xValues[0]),&(yValues[0]));
  return res;
}

void SpinModelConstruction::stripSpace(string &line){
	stringstream lineSt(line);
	line="";
	string word;
	while (lineSt >> word) {
		line.append(word).append(" ");
	}
	line = line.substr(0,line.size()-1);
}

void SpinModelConstruction::printVec(vector<string> vec){

	cout << "[";
	for (unsigned i=0; i<vec.size()-1;i++){
		cout << vec[i] << ",";
	}
	cout << vec[vec.size()-1] << "]" << endl;
}

void SpinModelConstruction::addToSystematicsList(vector<string> systs){
	for (vector<string>::iterator it=systs.begin(); it!=systs.end(); it++){
		if (find(systematicsList.begin(),systematicsList.end(),*it)!=systematicsList.end()) {
			cout << "ERROR - duplicate systematic names! " << *it << " already found in systematics list." << endl;
			exit(1);
		}
		else {
			systematicsList.push_back(*it);
		}
	}
}

bool SpinModelConstruction::isGlobalSyst(string name){
	for (vector<string>::iterator it=globalScales.begin(); it!=globalScales.end(); it++){
		if (*it==name) return true;
	}
	for (vector<string>::iterator it=globalScalesCorr.begin(); it!=globalScalesCorr.end(); it++){
		if (*it==name) return true;
	}
	return false;
}

bool SpinModelConstruction::isPerCatSyst(string name){
	for (vector<string>::iterator it=photonCatScales.begin(); it!=photonCatScales.end(); it++){
		if (*it==name) return true;
	}
	for (vector<string>::iterator it=photonCatScalesCorr.begin(); it!=photonCatScalesCorr.end(); it++){
		if (*it==name) return true;
	}
	for (vector<string>::iterator it=photonCatSmears.begin(); it!=photonCatSmears.end(); it++){
		if (*it==name) return true;
	}
	for (vector<string>::iterator it=photonCatSmearsCorr.begin(); it!=photonCatSmearsCorr.end(); it++){
		if (*it==name) return true;
	}
	return false;
}

void SpinModelConstruction::printSystMap(map<string,map<int,map<string,double> > > &theMap){

	for (map<string,map<int,map<string,double> > >::iterator p = theMap.begin(); p != theMap.end(); p++) {
		for (map<int,map<string,double> >::iterator c = p->second.begin(); c != p->second.end(); c++){
			cout << "Proc = " << p->first << "  DiphotonCat: " << c->first << endl;
			for (map<string,double>::iterator m = c->second.begin(); m != c->second.end(); m++){
				cout << "\t " << m->first << " : " << m->second << endl;
			}
		}
	}
}

void SpinModelConstruction::addToSystMap(map<string,map<int,map<string,double> > > &theMap, string proc, int diphotonCat, string phoSystName, double var){
	// does proc map exist?
	if (theMap.find(proc)!=theMap.end()) {
		// does the cat map exist?
		if (theMap[proc].find(diphotonCat)!=theMap[proc].end()){
			theMap[proc][diphotonCat].insert(make_pair(phoSystName,var));
		}
		else{
			map<string,double> temp;
			temp.insert(make_pair(phoSystName,var));
			theMap[proc].insert(make_pair(diphotonCat,temp));
		}
	}
	else {
		map<string,double> temp;
		map<int,map<string,double> > cTemp;
		temp.insert(make_pair(phoSystName,var));
		cTemp.insert(make_pair(diphotonCat,temp));
		theMap.insert(make_pair(proc,cTemp));
	}
}

float SpinModelConstruction::getRequiredAddtionalGlobalScaleFactor(string name){
	float retVal=-999;
	// check non correlated
	if (globalScalesOpts.find(name)==globalScalesOpts.end()) return retVal;
	else {
		for (vector<pair<int,float> >::iterator it=globalScalesOpts[name].begin(); it!=globalScalesOpts[name].end(); it++){
			if (cat_==it->first) return it->second;
		}
	}
	// check correlated
	if (globalScalesCorrOpts.find(name)==globalScalesCorrOpts.end()) return retVal;
	else {
		for (vector<pair<int,float> >::iterator it=globalScalesCorrOpts[name].begin(); it!=globalScalesCorrOpts[name].end(); it++){
			if (cat_==it->first) return it->second;
		}
	}
	return retVal;
}

void SpinModelConstruction::setHighR9cats(string catString){
	vector<string> cats;
	split(cats,catString,boost::is_any_of(","));
	for (unsigned int i=0; i<cats.size(); i++){
		highR9cats.push_back(boost::lexical_cast<int>(cats[i]));
	}
}

void SpinModelConstruction::setLowR9cats(string catString){
	vector<string> cats;
	split(cats,catString,boost::is_any_of(","));
	for (unsigned int i=0; i<cats.size(); i++){
		lowR9cats.push_back(boost::lexical_cast<int>(cats[i]));
	}
}

bool SpinModelConstruction::isHighR9cat(){
	for (vector<int>::iterator it=highR9cats.begin(); it!=highR9cats.end(); it++){
		if (*it==cat_) return true;
	}
	return false;
}

bool SpinModelConstruction::isLowR9cat(){
	for (vector<int>::iterator it=lowR9cats.begin(); it!=lowR9cats.end(); it++){
		if (*it==cat_) return true;
	}
	return false;
}
void SpinModelConstruction::loadSignalSystematics(string filename){
	
	int diphotonCat=-1;
	string proc;
	ifstream datfile;
	datfile.open(filename.c_str());
	if (datfile.fail()) exit(1);
	while (datfile.good()){
		
		string line;
		getline(datfile,line);
		
		// The input file needs correct ordering
		if (line=="\n" || line.substr(0,1)=="#" || line==" " || line.empty()) continue;
		
		// First read the various categories and names
		if (starts_with(line,"photonCatScales=")){
			line = line.substr(line.find("=")+1,string::npos);
			if (line.empty()) continue;
			split(photonCatScales,line,boost::is_any_of(","));
			if (verbosity_){
				cout << "PhotonCatScales: ";
				if (verbosity_) printVec(photonCatScales);
			}
			addToSystematicsList(photonCatScales);
		}
		else if (starts_with(line,"photonCatScalesCorr=")){
			line = line.substr(line.find("=")+1,string::npos);
			if (line.empty()) continue;
			split(photonCatScalesCorr,line,boost::is_any_of(","));
			if (verbosity_){
				cout << "PhotonCatScalesCorr: ";
				if (verbosity_) printVec(photonCatScalesCorr);
			}
			addToSystematicsList(photonCatScalesCorr);
		}
		else if (starts_with(line,"photonCatSmears=")){
			line = line.substr(line.find("=")+1,string::npos);
			if (line.empty()) continue;
			split(photonCatSmears,line,boost::is_any_of(","));
			if (verbosity_){
				cout << "PhotonCatSmears: ";
				if (verbosity_) printVec(photonCatSmears);
			}
			addToSystematicsList(photonCatSmears);
		}
		else if (starts_with(line,"photonCatSmearsCorr=")){
			line = line.substr(line.find("=")+1,string::npos);
			if (line.empty()) continue;
			split(photonCatSmearsCorr,line,boost::is_any_of(","));
			if (verbosity_){
				cout << "PhotonCatSmearsCorr: ";
				if (verbosity_) printVec(photonCatSmearsCorr);
			}
			addToSystematicsList(photonCatSmearsCorr);
		}
		else if (starts_with(line,"globalScales=")){
			line = line.substr(line.find("=")+1,string::npos);
			if (line.empty()) continue;
			// split first by comma and then by colon for specific options
			vector<string> temp;
			split(temp,line,boost::is_any_of(","));
			if (verbosity_) cout << "GlobalScales: ";
			for (vector<string>::iterator strIt=temp.begin(); strIt!=temp.end(); strIt++){
				vector<string> opts;
				vector<pair<int,float> > optDetails;
				split(opts,*strIt,boost::is_any_of(":"));
				globalScales.push_back(opts[0]);
				assert((opts.size()-1)%2==0);
				if (verbosity_) cout << "[" << opts[0] << ":";
				for (unsigned int i=1; i<opts.size(); i+=2) {
					optDetails.push_back(make_pair(boost::lexical_cast<int>(opts[i]),boost::lexical_cast<float>(opts[i+1])));
					if (verbosity_) cout << "(" << opts[i] << "," << opts[i+1] << ")";
				}
				globalScalesOpts.insert(make_pair(opts[0],optDetails));
			}
			if (verbosity_) cout << "]" << endl;
			addToSystematicsList(globalScales);
		}
		else if (starts_with(line,"globalScalesCorr=")){
			line = line.substr(line.find("=")+1,string::npos);
			if (line.empty()) continue;
			// split first by comma and then by colon for specific options
			vector<string> temp;
			split(temp,line,boost::is_any_of(","));
			if (verbosity_) cout << "GlobalScalesCorr: ";
			for (vector<string>::iterator strIt=temp.begin(); strIt!=temp.end(); strIt++){
				vector<string> opts;
				vector<pair<int,float> > optDetails;
				split(opts,*strIt,boost::is_any_of(":"));
				globalScalesCorr.push_back(opts[0]);
				assert((opts.size()-1)%2==0);
				if (verbosity_) cout << "[" << opts[0] << ": ";
				for (unsigned int i=1; i<opts.size(); i+=2) {
					optDetails.push_back(make_pair(boost::lexical_cast<int>(opts[i]),boost::lexical_cast<float>(opts[i+1])));
					if (verbosity_) cout << "(" << opts[i] << "," << opts[i+1] << ")";
				}
				globalScalesCorrOpts.insert(make_pair(opts[0],optDetails));
			}
			addToSystematicsList(globalScalesCorr);
			if (verbosity_) cout << "]" << endl;
		}
		// Then read diphoton cat
		else if (starts_with(line,"diphotonCat")){
			diphotonCat = boost::lexical_cast<int>(line.substr(line.find("=")+1,string::npos));
		}
		// And the process
		else if (starts_with(line,"proc")){
			proc = line.substr(line.find('=')+1,string::npos);
			if (verbosity_) cout << "Process:  " << proc << "  DiphoCat: " << diphotonCat << endl;
		}
		// Then read values
		else {
			stripSpace(line);
			vector<string> els;
			split(els,line,boost::is_any_of(" "));
			if (verbosity_) {cout << "\t"; printVec(els);}
			if (els.size()!=4) {
				cout << "I cant read this datfile " << endl;
				exit(1);
			}
			string phoSystName = els[0];
			double meanCh = lexical_cast<double>(els[1]);
			if( meanCh != 0. ) { 
				addToSystMap(meanSysts,proc,diphotonCat,phoSystName,meanCh);
				RooConstVar *meanChVar = new RooConstVar(Form("const_%s_cat%d_mean_%s",proc.c_str(),diphotonCat,phoSystName.c_str()),Form("const_%s_cat%d_mean_%s",proc.c_str(),diphotonCat,phoSystName.c_str()),meanCh);
				photonSystematicConsts.insert(make_pair(meanChVar->GetName(),meanChVar));
			}
			double sigmaCh = lexical_cast<double>(els[2]);
			if( sigmaCh != 0. ) {
				addToSystMap(sigmaSysts,proc,diphotonCat,phoSystName,sigmaCh);
				RooConstVar *sigmaChVar = new RooConstVar(Form("const_%s_cat%d_sigma_%s",proc.c_str(),diphotonCat,phoSystName.c_str()),Form("const_%s_cat%d_sigma_%s",proc.c_str(),diphotonCat,phoSystName.c_str()),sigmaCh);
				photonSystematicConsts.insert(make_pair(sigmaChVar->GetName(),sigmaChVar));
			}
			double rateCh = lexical_cast<double>(els[3]);
			if( rateCh != 0. ) {
				addToSystMap(rateSysts,proc,diphotonCat,phoSystName,rateCh);
				RooConstVar *rateChVar = new RooConstVar(Form("const_%s_cat%d_rate_%s",proc.c_str(),diphotonCat,phoSystName.c_str()),Form("const_%s_cat%d_rate_%s",proc.c_str(),diphotonCat,phoSystName.c_str()),rateCh);
				photonSystematicConsts.insert(make_pair(rateChVar->GetName(),rateChVar));
			}
		}
	}
	datfile.close();

	// now make the actual systematics
	for (vector<string>::iterator it=systematicsList.begin(); it!=systematicsList.end(); it++){
		RooRealVar *var = new RooRealVar(Form("CMS_hgg_nuisance_%s",it->c_str()),Form("CMS_hgg_nuisance_%s",it->c_str()),0.,-5.,5.);
		var->setConstant(true);
		photonSystematics.insert(make_pair(var->GetName(),var));
	}
}

RooAbsReal* SpinModelConstruction::getMeanWithPhotonSyst(RooAbsReal *dm, string name){

	string formula=Form("(@0+(@1*@0/%d))*(1.",mh_);
	RooArgList *dependents = new RooArgList();
	dependents->add(*MH); // MH sits at @0
	dependents->add(*dm); // dm sits at @1

	// check for global scales first
	for (unsigned int i=0; i<systematicsList.size(); i++){
		string syst = systematicsList[i];
		int formPlace = dependents->getSize();
		if (isGlobalSyst(syst)) {
			RooRealVar *nuisScale = photonSystematics[Form("CMS_hgg_nuisance_%s",syst.c_str())];
			formula += Form("+@%d",formPlace);
			// should check special extras 
			float additionalFactor = getRequiredAddtionalGlobalScaleFactor(syst);
			if (additionalFactor>-999) formula += Form("*%3.1f",additionalFactor);
			dependents->add(*nuisScale);
		}
	}
	// then do per photon scales
	for (unsigned int i=0; i<systematicsList.size(); i++){
		string syst = systematicsList[i];
		int formPlace = dependents->getSize();
		bool hasEffect = false;
		if (isPerCatSyst(syst)) {
			if (photonSystematicConsts.find(Form("const_%s_cat%d_mean_%s",proc_.c_str(),cat_,syst.c_str())) != photonSystematicConsts.end() ) {
				RooConstVar *constVar = photonSystematicConsts[Form("const_%s_cat%d_mean_%s",proc_.c_str(),cat_,syst.c_str())];
				RooRealVar *nuisVar = photonSystematics[Form("CMS_hgg_nuisance_%s",syst.c_str())];
				if (constVar->getVal()>=0.0001) {
					hasEffect = true;
					formula += Form("+@%d*@%d",formPlace,formPlace+1);
					dependents->add(*constVar);
					dependents->add(*nuisVar);
				}
			}
			if (verbosity_ && !hasEffect) {
				cout << "WARNING -- systematic " << syst << " is found to have no effect on the scale for category " << cat_ << " and process " << proc_ << " so it is being skipped." << endl;
			}
		}
	}
	formula+=")";
	RooFormulaVar *formVar = new RooFormulaVar(name.c_str(),name.c_str(),formula.c_str(),*dependents);
	return formVar;
}

RooAbsReal* SpinModelConstruction::getSigmaWithPhotonSyst(RooAbsReal *sig_fit, string name){

	string formula="@0*(1.";
	RooArgList *dependents = new RooArgList();
	dependents->add(*sig_fit); // sig_fit sits at @0
	if (quadraticSigmaSum_) formula += "+TMath::Sqrt(0.";
	
	for (unsigned int i=0; i<systematicsList.size(); i++){
		string syst = systematicsList[i];
		int formPlace = dependents->getSize();
		bool hasEffect = false;
		if (isPerCatSyst(syst)) {
			if (photonSystematicConsts.find(Form("const_%s_cat%d_sigma_%s",proc_.c_str(),cat_,syst.c_str())) != photonSystematicConsts.end() ) {
				RooConstVar *constVar = photonSystematicConsts[Form("const_%s_cat%d_sigma_%s",proc_.c_str(),cat_,syst.c_str())];
				RooRealVar *nuisVar = photonSystematics[Form("CMS_hgg_nuisance_%s",syst.c_str())];
				if (constVar->getVal()>=0.0001) {
					hasEffect = true;
					formula += Form("+@%d*@%d",formPlace,formPlace+1);
					if (quadraticSigmaSum_) formula += Form("*@%d*@%d",formPlace,formPlace+1);
					dependents->add(*constVar);
					dependents->add(*nuisVar);
				}
			}
			if (verbosity_ && !hasEffect) {
				cout << "WARNING -- systematic " << syst << " is found to have no effect on the resolution for category " << cat_ << " and process " << proc_ << " so it is being skipped." << endl;
			}
		}
	}
	if (quadraticSigmaSum_) formula+=")";
	formula+=")";
	formula+=Form("*(@%d/%d)",dependents->getSize(),mh_);
	dependents->add(*MH);
	formula = Form("TMath::Max(%s,1.e-6)",formula.c_str()); // consider smooth cutoff ? 
	RooFormulaVar *formVar = new RooFormulaVar(name.c_str(),name.c_str(),formula.c_str(),*dependents);
	return formVar;
}

RooAbsReal* SpinModelConstruction::getRateWithPhotonSyst(string name){
	
	string formula="(1.";
	RooArgList *dependents = new RooArgList();

	for (unsigned int i=0; i<systematicsList.size(); i++){
		string syst = systematicsList[i];
		int formPlace = dependents->getSize();
		bool hasEffect = false;
		if (isPerCatSyst(syst)) {
			if (photonSystematicConsts.find(Form("const_%s_cat%d_rate_%s",proc_.c_str(),cat_,syst.c_str())) != photonSystematicConsts.end() ) {
				RooConstVar *constVar = photonSystematicConsts[Form("const_%s_cat%d_rate_%s",proc_.c_str(),cat_,syst.c_str())];
				RooRealVar *nuisVar = photonSystematics[Form("CMS_hgg_nuisance_%s",syst.c_str())];
				if (constVar->getVal()>=0.001) {
					hasEffect = true;
					formula += Form("+@%d*@%d",formPlace,formPlace+1);
					dependents->add(*constVar);
					dependents->add(*nuisVar);
				}
			}
			if (verbosity_ && !hasEffect) {
				cout << "WARNING -- systematic " << syst << " is found to have no effect on the rate for category " << cat_ << " and process " << proc_ << " so it is being skipped." << endl;
			}
		}
	}

	formula+=")";
	if (isHighR9cat()) {
		formula += Form("*(1.+@%d)",dependents->getSize());
		dependents->add(*r9barrelNuisance);
	}
	if (isLowR9cat()) {
		formula += Form("*(1.+@%d)",dependents->getSize());
		dependents->add(*r9mixedNuisance);
	}
	RooFormulaVar *formVar = new RooFormulaVar(name.c_str(),name.c_str(),formula.c_str(),*dependents);
	return formVar;
}

void SpinModelConstruction::setupSystematics(){
  
	vertexNuisance = new RooRealVar("CMS_hgg_nuisance_deltafracright","CMS_hgg_nuisance_deltafracright",0.,-1.,1.);
	vertexNuisance->setConstant(true);
	r9barrelNuisance = new RooRealVar(Form("CMS_hgg_nuisance_%dTeVdeltar9barrel",sqrts_),Form("CMS_hgg_nuisance_%dTeVdeltar9barrel",sqrts_),0.,-1.,1.);
	r9mixedNuisance = new RooRealVar(Form("CMS_hgg_nuisance_%dTeVdeltar9mixed",sqrts_),Form("CMS_hgg_nuisance_%dTeVdeltar9mixed",sqrts_),0.,-1.,1.);
	r9barrelNuisance->setConstant(true);
	r9mixedNuisance->setConstant(true);
	systematicsSet_=true;
}

double SpinModelConstruction::getRvFractionFunc(){
  
	double rvN = rvDatasets[mh_]->sumEntries();
	double wvN = wvDatasets[mh_]->sumEntries();
	double rvF = rvN/(rvN+wvN);
	return rvF;
}


void SpinModelConstruction::buildRvWvPdf(string name, int nGrv, int nGwv, bool recursive) {
  if (!systematicsSet_) setupSystematics();
  RooFormulaVar *rvFraction = new RooFormulaVar(Form("%s_%s_cat%d_rvFrac",name.c_str(),proc_.c_str(),cat_),Form("%s_%s_cat%d_rvFrac",name.c_str(),proc_.c_str(),cat_),"TMath::Min(@0+@1,1.0)",RooArgList(*vertexNuisance,RooConst(getRvFractionFunc())));
  RooAddPdf* rvPdf = buildPdf(name,nGrv,recursive,fitParamsRV,Form("_rv_%dTeV",sqrts_)); 
  RooAddPdf* wvPdf = buildPdf(name,nGwv,recursive,fitParamsWV,Form("_wv_%dTeV",sqrts_)); 
  finalPdf = new RooAddPdf(Form("%s_%s_cat%d",name.c_str(),proc_.c_str(),cat_),Form("%s_%s_cat%d",name.c_str(),proc_.c_str(),cat_),RooArgList(*rvPdf,*wvPdf),RooArgList(*rvFraction));
}

void SpinModelConstruction::printMap(map<string,RooRealVar*> m){
	
	for (map<string,RooRealVar*>::iterator it=m.begin(); it!=m.end(); it++){
		cout << it->first << " "; 
		it->second->Print();
	}
}

RooAddPdf* SpinModelConstruction::buildPdf(string name, int nGaussians, bool recursive, map<int,map<string,RooRealVar*> > params, string add) {

  RooArgList *gaussians = new RooArgList();
  RooArgList *coeffs = new RooArgList();
  string ext = Form("%s_cat%d%s",proc_.c_str(),cat_,add.c_str());

	for (int g=0; g<nGaussians; g++){
		RooRealVar *fitDM = params[mh_][Form("dm_mh%d_g%d",mh_,g)];
		fitDM->SetName(Form("dm_g%d_%s",g,ext.c_str()));
		fitDM->setConstant(true);
		RooRealVar *fitSigma = params[mh_][Form("sigma_mh%d_g%d",mh_,g)];
		fitSigma->SetName(Form("sigma_g%d_%s",g,ext.c_str()));
		fitSigma->setConstant(true);

		RooAbsReal *mean =  getMeanWithPhotonSyst(fitDM,Form("mean_g%d_%s",g,ext.c_str()));
		RooAbsReal *sigma =  getSigmaWithPhotonSyst(fitSigma,Form("sig_g%d_%s",g,ext.c_str()));
		RooGaussian *gaus = new RooGaussian(Form("gaus_g%d_%s",g,ext.c_str()),Form("gaus_g%d_%s",g,ext.c_str()),*mass,*mean,*sigma);
    gaussians->add(*gaus);
    
		if (g<nGaussians-1) {
			RooRealVar *frac = params[mh_][Form("frac_mh%d_g%d",mh_,g)]; 
      frac->SetName(Form("frac_g%d_%s",g,ext.c_str()));
			frac->setConstant(true);
      coeffs->add(*frac);
		}
	}
  assert(gaussians->getSize()==nGaussians && coeffs->getSize()==nGaussians-1);
  RooAddPdf *pdf = new RooAddPdf(Form("%s_%s",name.c_str(),ext.c_str()),Form("%s_%s",name.c_str(),ext.c_str()),*gaussians,*coeffs,recursive);
	return pdf;
}

void SpinModelConstruction::getNormalization(){
	
	RooDataSet *data = stdDatasets[mh_];
	double effAcc = (data->sumEntries()/(intLumi->getVal()*norm->GetXsection(mh_,proc_)*norm->GetBR(mh_)));

	RooSpline1D *xs = xsSplines[proc_];
	RooAbsReal *rateNuisTerm = getRateWithPhotonSyst(Form("rate_%s_cat%d_%dTeV",proc_.c_str(),cat_,sqrts_));

	RooRealVar *scaleFac = new RooRealVar("scaleFac","scaleFac",1.);
	if (proc_=="gg_grav") scaleFac->setVal(scaleGravGG_);
	if (proc_=="qq_grav") scaleFac->setVal(scaleGravQQ_);
	scaleFac->setConstant(true);

	finalNorm = new RooFormulaVar(Form("%s_norm",finalPdf->GetName()),Form("%s_norm",finalPdf->GetName()),"@0*@1*@2*@3*@4",RooArgList(*xs,*brSpline,RooConst(effAcc),*rateNuisTerm,*scaleFac));
	finalNormThisLum = new RooFormulaVar(Form("%s_normThisLumi",finalPdf->GetName()),Form("%s_normThisLumi",finalPdf->GetName()),"@0*@1*@2*@3*@4*@5",RooArgList(*xs,*brSpline,RooConst(effAcc),*rateNuisTerm,*intLumi,*scaleFac));

	extendPdfRel = new RooExtendPdf(Form("extend%s",finalPdf->GetName()),Form("extend%s",finalPdf->GetName()),*finalPdf,*finalNorm);
  extendPdf = new RooExtendPdf(Form("extend%sThisLumi",finalPdf->GetName()),Form("extend%sThisLumi",finalPdf->GetName()),*finalPdf,*finalNormThisLum);
}

void SpinModelConstruction::plotPdf(string outDir, int mhLow, int mhHigh){
  system(Form("mkdir -p %s",outDir.c_str()));
  
  TCanvas *canv = new TCanvas();
  RooPlot *dataPlot = mass->frame(Title(Form("%s_cat%d",proc_.c_str(),cat_)),Range(100,160));
	stdDatasets[mh_]->plotOn(dataPlot,Binning(160));
	MH->setVal(mh_);
	extendPdf->plotOn(dataPlot);
  dataPlot->Draw();
  canv->Print(Form("%s/%s_cat%d_fits.pdf",outDir.c_str(),proc_.c_str(),cat_));
  canv->Print(Form("%s/%s_cat%d_fits.png",outDir.c_str(),proc_.c_str(),cat_));
  
  RooPlot *pdfPlot = mass->frame(Title(Form("%s_cat%d",proc_.c_str(),cat_)),Range(100,160));
	pdfPlot->GetYaxis()->SetTitle(Form("Pdf projection / %2.1f GeV",(mass->getMax()-mass->getMin())/160.));
  for (int mh=mhLow; mh<=mhHigh; mh++){
    MH->setVal(mh);
		// to get correct normlization need to manipulate with bins and range
    extendPdf->plotOn(pdfPlot,Normalization(mass->getBins()/160.*(mass->getMax()-mass->getMin())/60.,RooAbsReal::RelativeExpected));
  }
  pdfPlot->Draw();
  canv->Print(Form("%s/%s_cat%d_interp.pdf",outDir.c_str(),proc_.c_str(),cat_));
  canv->Print(Form("%s/%s_cat%d_interp.png",outDir.c_str(),proc_.c_str(),cat_));
  delete canv;

}

void SpinModelConstruction::save(RooWorkspace *work){
  work->import(*finalPdf,RecycleConflictNodes());
  work->import(*finalNorm,RecycleConflictNodes());
  work->import(*finalNormThisLum,RecycleConflictNodes());
  work->import(*extendPdf,RecycleConflictNodes());
  work->import(*extendPdfRel,RecycleConflictNodes());
  for (map<int,RooDataSet*>::iterator it=stdDatasets.begin(); it!=stdDatasets.end(); it++){
    work->import(*(it->second));
  }
}

