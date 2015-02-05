#include <fstream>
#include <sstream>

#include "TVectorT.h"
#include "TMatrixTSym.h"
#include "TCanvas.h"
#include "TF1.h"
#include "RooPlot.h"
#include "RooVoigtian.h"
#include "RooProduct.h"
#include "RooAddition.h"
#include "RooConstVar.h"
#include "RooFormulaVar.h"

#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/predicate.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string/replace.hpp"
#include "boost/algorithm/string/erase.hpp"

#include "../interface/FinalModelConstruction.h"

using namespace std;
using namespace RooFit;
using namespace boost;

template<class ResultT, class SourceT, class PredicateT> typename ResultT::iterator split_append(ResultT & dst, const SourceT & src, PredicateT pred)
{
	ResultT tmp;
	split( tmp, src, pred );
	size_t orig_size = dst.size();
	copy(tmp.begin(), tmp.end(), back_inserter(dst));	
	return dst.begin()+orig_size;
}

FinalModelConstruction::FinalModelConstruction(RooRealVar *massVar, RooRealVar *MHvar, RooRealVar *intL, int mhLow, int mhHigh, string proc, int cat, bool doSecMods, string systematicsFileName, vector<int> skipMasses, int verbosity, bool isCB, bool is2011, bool quadraticSigmaSum):
  mass(massVar),
  MH(MHvar),
  intLumi(intL),
  mhLow_(mhLow),
  mhHigh_(mhHigh),
  proc_(proc),
  cat_(cat),
  doSecondaryModels(doSecMods),
  isCutBased_(isCB),
	is2011_(is2011),
	quadraticSigmaSum_(quadraticSigmaSum),
	skipMasses_(skipMasses),
  verbosity_(verbosity),
  systematicsSet_(false),
  rvFractionSet_(false)
{
  allMH_ = getAllMH();
	if (is2011_) sqrts_ = 7;
	else sqrts_ = 8;
  // load xs and br info from Normalization_8TeV
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

  vector<string> files;
  split( files, systematicsFileName, boost::is_any_of(",") );
  for(vector<string>::iterator fi=files.begin(); fi!=files.end(); ++fi ) {
	  loadSignalSystematics(*fi);
  }
  if (verbosity_) printSignalSystematics();
}

FinalModelConstruction::~FinalModelConstruction(){}

void FinalModelConstruction::addToSystematicsList(vector<string>::iterator begin, vector<string>::iterator end){
	for (vector<string>::iterator it=begin; it!=end; it++){
		string name = *it;
		float corr = 0.;
		int index = -1;
		if( it->find(":") != string::npos ) {
			vector<string> toks;
			split(toks,*it,boost::is_any_of(":"));
			name = toks[0];
			corr = boost::lexical_cast<float>(toks[1]);
			index = boost::lexical_cast<int>(toks[2]); 
			/// *it = Form("%dTeV%s",sqrts_,name.c_str());
			*it = name;
		}
		if (find(systematicsList.begin(),systematicsList.end(),name)!=systematicsList.end()) {
			cout << "ERROR - duplicate systematic names! " << *it << " already found in systematics list." << endl;
			exit(1);
		}
		else {
			systematicsList.push_back(name);
			systematicsCorr.push_back(corr);
			systematicsIdx.push_back(index);
		}
	}
}

void FinalModelConstruction::addToSystematicsList(vector<string> systs){
	addToSystematicsList( systs.begin(), systs.end() );
	/// for (vector<string>::iterator it=systs.begin(); it!=systs.end(); it++){
	/// 	if (find(systematicsList.begin(),systematicsList.end(),*it)!=systematicsList.end()) {
	/// 		cout << "ERROR - duplicate systematic names! " << *it << " already found in systematics list." << endl;
	/// 		exit(1);
	/// 	}
	/// 	else {
	/// 		systematicsList.push_back(*it);
	/// 	}
	/// }
}

bool FinalModelConstruction::isGlobalSyst(string name){
	for (vector<string>::iterator it=globalScales.begin(); it!=globalScales.end(); it++){
		if (*it==name) return true;
	}
	for (vector<string>::iterator it=globalScalesCorr.begin(); it!=globalScalesCorr.end(); it++){
		if (*it==name) return true;
	}
	return false;
}

bool FinalModelConstruction::isPerCatSyst(string name){
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

float FinalModelConstruction::getRequiredAddtionalGlobalScaleFactor(string name){
	float retVal=-999;
	// check non correlated
	if (globalScalesOpts.find(name)!=globalScalesOpts.end()) {
		for (vector<pair<int,float> >::iterator it=globalScalesOpts[name].begin(); it!=globalScalesOpts[name].end(); it++){
			if (cat_==it->first) return it->second;
		}
	}
	// check correlated
	if (globalScalesCorrOpts.find(name)!=globalScalesCorrOpts.end()) {
		for (vector<pair<int,float> >::iterator it=globalScalesCorrOpts[name].begin(); it!=globalScalesCorrOpts[name].end(); it++){
			if (cat_==it->first) return it->second;
		}
	}
	return retVal;
}

void FinalModelConstruction::setHighR9cats(string catString){
	vector<string> cats;
	split(cats,catString,boost::is_any_of(","));
	for (unsigned int i=0; i<cats.size(); i++){
		highR9cats.push_back(boost::lexical_cast<int>(cats[i]));
	}
}

void FinalModelConstruction::setLowR9cats(string catString){
	vector<string> cats;
	split(cats,catString,boost::is_any_of(","));
	for (unsigned int i=0; i<cats.size(); i++){
		lowR9cats.push_back(boost::lexical_cast<int>(cats[i]));
	}
}

bool FinalModelConstruction::isHighR9cat(){
	for (vector<int>::iterator it=highR9cats.begin(); it!=highR9cats.end(); it++){
		if (*it==cat_) return true;
	}
	return false;
}

bool FinalModelConstruction::isLowR9cat(){
	for (vector<int>::iterator it=lowR9cats.begin(); it!=lowR9cats.end(); it++){
		if (*it==cat_) return true;
	}
	return false;
}

void FinalModelConstruction::printSignalSystematics(){

	cout << "Signal systematics info..." << endl;
	// names in systematicsList
	cout << "The following systematic names are stored" << endl;
	for (vector<string>::iterator sys=systematicsList.begin(); sys!=systematicsList.end(); sys++){
		if (isGlobalSyst(*sys)) cout << "\t " << Form("%-50s -- global",sys->c_str()) << endl;
		if (isPerCatSyst(*sys)) cout << "\t " << Form("%-50s -- photon cat",sys->c_str()) << endl;
	}
	// nuisance parameters
	cout << "Implementing the following floating nuisance parameters" << endl;
	for (map<string,RooAbsReal*>::iterator sys=photonSystematics.begin(); sys!=photonSystematics.end(); sys++){
		cout << "\t " << Form("%-50s",sys->first.c_str()) << " -- "; sys->second->Print();
	}
	// const parameters
	cout << "Implementing the following constant parameters" << endl;
	for (map<string,RooConstVar*>::iterator sys=photonSystematicConsts.begin(); sys!=photonSystematicConsts.end(); sys++){
		cout << "\t " << Form("%-50s",sys->first.c_str()) << " -- "; sys->second->Print();
	}
}

void FinalModelConstruction::loadSignalSystematics(string filename){
	
	int diphotonCat=-1;
	string proc;
	ifstream datfile;
	datfile.open(filename.c_str());
	if (datfile.fail()) {
		cout << "Failed to load " << filename.c_str();
		exit(1);
	}
	while (datfile.good()){
		
		string line;
		getline(datfile,line);
		
		// The input file needs correct ordering
		if (line=="\n" || line.substr(0,1)=="#" || line==" " || line.empty()) continue;
		
		// First read the various categories and names
		if (starts_with(line,"photonCatScales=")){
			line = line.substr(line.find("=")+1,string::npos);
			if (line.empty()) continue;
			vector<string>::iterator beg = split_append(photonCatScales,line,boost::is_any_of(","));
			addToSystematicsList(beg,photonCatScales.end());
			if (verbosity_){
				cout << "PhotonCatScales: ";
				if (verbosity_) printVec(photonCatScales);
			}
		}
		else if (starts_with(line,"photonCatScalesCorr=")){
			line = line.substr(line.find("=")+1,string::npos);
			if (line.empty()) continue;
			vector<string>::iterator beg = split_append(photonCatScalesCorr,line,boost::is_any_of(","));
			addToSystematicsList(beg,photonCatScalesCorr.end());
			if (verbosity_){
				cout << "PhotonCatScalesCorr: ";
				if (verbosity_) printVec(photonCatScalesCorr);
			}
		}
		else if (starts_with(line,"photonCatSmears=")){
			line = line.substr(line.find("=")+1,string::npos);
			if (line.empty()) continue;
			vector<string>::iterator beg = split_append(photonCatSmears,line,boost::is_any_of(","));
			addToSystematicsList(beg,photonCatSmears.end());
			if (verbosity_){
				cout << "PhotonCatSmears: ";
				if (verbosity_) printVec(photonCatSmears);
			}
		}
		else if (starts_with(line,"photonCatSmearsCorr=")){
			line = line.substr(line.find("=")+1,string::npos);
			if (line.empty()) continue;
			vector<string>::iterator beg = split_append(photonCatSmearsCorr,line,boost::is_any_of(","));
			addToSystematicsList(beg,photonCatSmearsCorr.end());
			if (verbosity_){
				cout << "PhotonCatSmearsCorr: ";
				if (verbosity_) printVec(photonCatSmearsCorr);
			}
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
				cout << "I cant read this datfile " << line << endl;
				exit(1);
			}
			string phoSystName = els[0];
			double meanCh = lexical_cast<double>(els[1]);
			// round up scale syst if less than 1.e-4
			if( fabs(meanCh)<1.e-4 && fabs(meanCh)>=5.e-5 && phoSystName.find("scale")!=string::npos ) { 
				meanCh=( meanCh>0. ? 1.e-4 : -1.e-4 ); 
			} else if( fabs(meanCh)<1.e-4 && phoSystName.find("smear")!=string::npos ) { 
				meanCh = 0.;
			}
			if( meanCh != 0. ) { 
				addToSystMap(meanSysts,proc,diphotonCat,phoSystName,meanCh);
				RooConstVar *meanChVar = new RooConstVar(Form("const_%s_cat%d_%dTeV_mean_%s",proc.c_str(),diphotonCat,sqrts_,phoSystName.c_str()),Form("const_%s_cat%d_%dTeV_mean_%s",proc.c_str(),diphotonCat,sqrts_,phoSystName.c_str()),meanCh);
				photonSystematicConsts.insert(make_pair(meanChVar->GetName(),meanChVar));
			}
			double sigmaCh = lexical_cast<double>(els[2]);
			if( sigmaCh != 0. ) {
				addToSystMap(sigmaSysts,proc,diphotonCat,phoSystName,sigmaCh);
				RooConstVar *sigmaChVar = new RooConstVar(Form("const_%s_cat%d_%dTeV_sigma_%s",proc.c_str(),diphotonCat,sqrts_,phoSystName.c_str()),Form("const_%s_cat%d_%dTeV_sigma_%s",proc.c_str(),diphotonCat,sqrts_,phoSystName.c_str()),sigmaCh);
				photonSystematicConsts.insert(make_pair(sigmaChVar->GetName(),sigmaChVar));
			}
			double rateCh = lexical_cast<double>(els[3]);
			if( rateCh != 0. ) {
				addToSystMap(rateSysts,proc,diphotonCat,phoSystName,rateCh);
				RooConstVar *rateChVar = new RooConstVar(Form("const_%s_cat%d_%dTeV_rate_%s",proc.c_str(),diphotonCat,sqrts_,phoSystName.c_str()),Form("const_%s_cat%d_%dTeV_rate_%s",proc.c_str(),diphotonCat,sqrts_,phoSystName.c_str()),rateCh);
				photonSystematicConsts.insert(make_pair(rateChVar->GetName(),rateChVar));
			}
		}
	}
	datfile.close();

	// now make the actual systematics
	// for (vector<string>::iterator it=systematicsList.begin(); it!=systematicsList.end(); it++){
	for (size_t is=0; is<systematicsList.size(); ++is) {
		std::string & name = systematicsList[is];
		float & corr = systematicsCorr[is];
		int   & idx = systematicsIdx[is];
		if( corr != 0. ) {
			TString nuname = name;
			nuname = nuname.ReplaceAll(Form("%dTeV",sqrts_),"");
			RooRealVar *var1 = new RooRealVar(Form("CMS_hgg_nuisance_eigen1_%s",nuname.Data()),Form("CMS_hgg_nuisance_eigen1_%s",nuname.Data()),0.,-5.,5.);
			var1->setConstant(true);
			photonSystematics.insert(make_pair(var1->GetName(),var1));
			RooRealVar *var2 = new RooRealVar(Form("CMS_hgg_nuisance_eigen2_%s",nuname.Data()),Form("CMS_hgg_nuisance_eigen2_%s",nuname.Data()),0.,-5.,5.);
			var2->setConstant(true);
			photonSystematics.insert(make_pair(var2->GetName(),var2));
			
			TMatrixTSym<double> varmat(2);
			varmat[0][0] = 1.;//sigmaX*sigmaX;
			varmat[0][1] = corr;//*sigmaX*sigmaY;
			varmat[1][0] = corr;//*sigmaX*sigmaY;
			varmat[1][1] = 1.;//sigmaY*sigmaY;

			//// // Print Matrix 
			//// std::cout << "Covariance Matrix" << std::endl;
			//// varmat.Print();
			//// std::cout << "-----------------" << std::endl;
			//// 
			//// // 2 Get the eigenvalues/eigenvectors 
			TVectorT<double> eval;
			TMatrixT<double> evec = varmat.EigenVectors(eval);
			//// std::cout << "Matrix of Eigenvectors" << std::endl;
			//// evec.Print();
			//// std::cout << "----------------------" << std::endl;
			double cs = evec[idx][0]*sqrt(eval[0]) , ss = evec[idx][1]*sqrt(eval[1]);
			//// double tot = sqrt( cs*cs + ss*ss );
			//// std::cout << "size of eigenvec " << tot << std::endl;
			/// cs /= tot; ss /= tot;
			
			RooFormulaVar *var = new RooFormulaVar(Form("CMS_hgg_nuisance_%s",name.c_str()),
							       Form("CMS_hgg_nuisance_%s",name.c_str()),
							       Form("%1.3g * @0 + %1.3g * @1",cs,ss), RooArgList(*var1,*var2) );
			photonSystematics.insert(make_pair(var->GetName(),var));
			// var->Print("V");
		} else { 
			RooRealVar *var = new RooRealVar(Form("CMS_hgg_nuisance_%s",name.c_str()),Form("CMS_hgg_nuisance_%s",name.c_str()),0.,-5.,5.);
			var->setConstant(true);
			photonSystematics.insert(make_pair(var->GetName(),var));
		}
	}
}

void FinalModelConstruction::stripSpace(string &line){
	stringstream lineSt(line);
	line="";
	string word;
	while (lineSt >> word) {
		line.append(word).append(" ");
	}
	line = line.substr(0,line.size()-1);
}

void FinalModelConstruction::printVec(vector<string> vec){

	cout << "[";
	for (unsigned i=0; i<vec.size()-1;i++){
		cout << vec[i] << ",";
	}
	cout << vec[vec.size()-1] << "]" << endl;
}

void FinalModelConstruction::printSystMap(map<string,map<int,map<string,double> > > &theMap){

	for (map<string,map<int,map<string,double> > >::iterator p = theMap.begin(); p != theMap.end(); p++) {
		for (map<int,map<string,double> >::iterator c = p->second.begin(); c != p->second.end(); c++){
			cout << "Proc = " << p->first << "  DiphotonCat: " << c->first << endl;
			for (map<string,double>::iterator m = c->second.begin(); m != c->second.end(); m++){
				cout << "\t " << m->first << " : " << m->second << endl;
			}
		}
	}
}

void FinalModelConstruction::addToSystMap(map<string,map<int,map<string,double> > > &theMap, string proc, int diphotonCat, string phoSystName, double var){
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

bool FinalModelConstruction::skipMass(int mh){
	for (vector<int>::iterator it=skipMasses_.begin(); it!=skipMasses_.end(); it++) {
		if (*it==mh) return true;
	}
	return false;
}

vector<int> FinalModelConstruction::getAllMH(){
  vector<int> result;
  for (int m=mhLow_; m<=mhHigh_; m+=5){
		if (skipMass(m)) continue;
    if (verbosity_>=1) cout << "FinalModelConstruction - Adding mass: " << m << endl;
    result.push_back(m);
  }
  return result;
}

void FinalModelConstruction::setSecondaryModelVars(RooRealVar *mh_sm, RooRealVar *deltam, RooAddition *mh_2, RooRealVar *width){
  MH_SM = mh_sm;
  DeltaM = deltam;
  MH_2 = mh_2;
  higgsDecayWidth = width;
  
  TGraph *brGraph = norm->GetBrGraph();
	brSpline_SM = graphToSpline(Form("fbr_%dTeV_SM",sqrts_),brGraph,MH_SM);
	brSpline_2 = graphToSpline(Form("fbr_%dTeV_2",sqrts_),brGraph,MH_2);
	brSpline_NW = graphToSpline(Form("fbr_%dTeV_NW",sqrts_),brGraph,MH);
  
	string procs[8] = {"ggh","vbf","wzh","wh","zh","tth","gg_grav","qq_grav"};
  for (int i=0; i<8; i++){
    TGraph *xsGraph = norm->GetSigmaGraph(procs[i].c_str());
    RooSpline1D *xsSpline_SM = graphToSpline(Form("fxs_%s_%dTeV_SM",procs[i].c_str(),sqrts_),xsGraph,MH_SM);
    RooSpline1D *xsSpline_2 = graphToSpline(Form("fxs_%s_%dTeV_2",procs[i].c_str(),sqrts_),xsGraph,MH_2); 
    RooSpline1D *xsSpline_NW = graphToSpline(Form("fxs_%s_%dTeV_NW",procs[i].c_str(),sqrts_),xsGraph,MH);
    xsSplines_SM.insert(pair<string,RooSpline1D*>(procs[i],xsSpline_SM));
    xsSplines_2.insert(pair<string,RooSpline1D*>(procs[i],xsSpline_2));
    xsSplines_NW.insert(pair<string,RooSpline1D*>(procs[i],xsSpline_NW));
  }
  secondaryModelVarsSet=true;
}

void FinalModelConstruction::getRvFractionFunc(string name){
  
  assert(allMH_.size()==rvDatasets.size());
  assert(allMH_.size()==wvDatasets.size());
  vector<double> mhValues, rvFracValues;
  for (unsigned int i=0; i<allMH_.size(); i++){
    int mh = allMH_[i];
    mhValues.push_back(mh);
    double rvN = rvDatasets[mh]->sumEntries();
    double wvN = wvDatasets[mh]->sumEntries();
		double rvF = rvN/(rvN+wvN);
		if (rvF != rvF) rvF=1.; // incase nan when no entries
    rvFracValues.push_back(rvF);
  }
  rvFracFunc = new RooSpline1D(name.c_str(),name.c_str(),*MH,mhValues.size(),&(mhValues[0]),&(rvFracValues[0]));
	if (doSecondaryModels){
		rvFracFunc_SM = new RooSpline1D(Form("%s_SM",name.c_str()),name.c_str(),*MH_SM,mhValues.size(),&(mhValues[0]),&(rvFracValues[0]));
		rvFracFunc_2 = new RooSpline1D(Form("%s_2",name.c_str()),name.c_str(),*MH_2,mhValues.size(),&(mhValues[0]),&(rvFracValues[0]));
		rvFracFunc_NW = new RooSpline1D(Form("%s_NW",name.c_str()),name.c_str(),*MH,mhValues.size(),&(mhValues[0]),&(rvFracValues[0]));
	}
  rvFractionSet_=true;
}

RooAbsReal* FinalModelConstruction::getMeanWithPhotonSyst(RooAbsReal *dm, string name, bool isMH2, bool isMHSM){

	if (!doSecondaryModels && (isMH2 || isMHSM)) {
		cout << "ERROR -- for some reason your asking for a dependence on MH_2 or MH_SM but are not running secondary models" << endl;
		exit(1);
	}
	if (isMH2 && isMHSM) {
		cout << "ERROR -- for some reason your asking for a dependence on MH_2 and MH_SM but both cannot be true" << endl;
		exit(1);
	}

	string formula="(@0+@1)*(1.";
	RooArgList *dependents = new RooArgList();
	if (isMH2) dependents->add(*MH_2);
	else if (isMHSM) dependents->add(*MH_SM);
	else dependents->add(*MH); // MH sits at @0
	dependents->add(*dm); // dm sits at @1

	// check for global scales first
	for (unsigned int i=0; i<systematicsList.size(); i++){
		string syst = systematicsList[i];
		int formPlace = dependents->getSize();
		if (isGlobalSyst(syst)) {
			RooAbsReal *nuisScale = photonSystematics[Form("CMS_hgg_nuisance_%s",syst.c_str())];
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
			if (photonSystematicConsts.find(Form("const_%s_cat%d_%dTeV_mean_%s",proc_.c_str(),cat_,sqrts_,syst.c_str())) != photonSystematicConsts.end() ) {
				RooConstVar *constVar = photonSystematicConsts[Form("const_%s_cat%d_%dTeV_mean_%s",proc_.c_str(),cat_,sqrts_,syst.c_str())];
				RooAbsReal *nuisVar = photonSystematics[Form("CMS_hgg_nuisance_%s",syst.c_str())];
				if( verbosity_ ) { 
					std::cout << "Systematic " << syst << std::endl;
					nuisVar->Print("V");
				}
				if ( fabs(constVar->getVal())>=5.e-5) { 
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

RooAbsReal* FinalModelConstruction::getSigmaWithPhotonSyst(RooAbsReal *sig_fit, string name){

	string formula="@0*";
	RooArgList *dependents = new RooArgList();
	dependents->add(*sig_fit); // sig_fit sits at @0
	if (quadraticSigmaSum_) formula += "TMath::Sqrt(TMath::Max(1.e-4,1.";
	else formula += "TMath::Max(1.e-2,(1.";
	
	for (unsigned int i=0; i<systematicsList.size(); i++){
		string syst = systematicsList[i];
		int formPlace = dependents->getSize();
		bool hasEffect = false;
		if (isPerCatSyst(syst)) {
			if (photonSystematicConsts.find(Form("const_%s_cat%d_%dTeV_sigma_%s",proc_.c_str(),cat_,sqrts_,syst.c_str())) != photonSystematicConsts.end() ) {
				RooConstVar *constVar = photonSystematicConsts[Form("const_%s_cat%d_%dTeV_sigma_%s",proc_.c_str(),cat_,sqrts_,syst.c_str())];
				RooAbsReal *nuisVar = photonSystematics[Form("CMS_hgg_nuisance_%s",syst.c_str())];
				if (constVar->getVal()>=1.e-4) {
					hasEffect = true;
					if( quadraticSigmaSum_ ) { 
						formula += Form("+@%d*@%d*(2.+@%d)",formPlace,formPlace+1,formPlace+1);
					} else {
						formula += Form("+@%d*@%d",formPlace,formPlace+1);
					}
					dependents->add(*nuisVar);
					dependents->add(*constVar);
				}
			}
			if (verbosity_ && !hasEffect) {
				cout << "WARNING -- systematic " << syst << " is found to have no effect on the resolution for category " << cat_ << " and process " << proc_ << " so it is being skipped." << endl;
			}
		}
	}
	formula+="))";
	RooFormulaVar *formVar = new RooFormulaVar(name.c_str(),name.c_str(),formula.c_str(),*dependents);
	return formVar;
}

RooAbsReal* FinalModelConstruction::getRateWithPhotonSyst(string name){
	
	string formula="(1.";
	RooArgList *dependents = new RooArgList();

	for (unsigned int i=0; i<systematicsList.size(); i++){
		string syst = systematicsList[i];
		int formPlace = dependents->getSize();
		bool hasEffect = false;
		if (isPerCatSyst(syst)) {
			if (photonSystematicConsts.find(Form("const_%s_cat%d_%dTeV_rate_%s",proc_.c_str(),cat_,sqrts_,syst.c_str())) != photonSystematicConsts.end() ) {
				RooConstVar *constVar = photonSystematicConsts[Form("const_%s_cat%d_%dTeV_rate_%s",proc_.c_str(),cat_,sqrts_,syst.c_str())];
				RooAbsReal *nuisVar = photonSystematics[Form("CMS_hgg_nuisance_%s",syst.c_str())];
				if (constVar->getVal()>=5.e-4) {
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
	if (isCutBased_){
		if (isHighR9cat()) {
			formula += Form("*(1.+@%d)",dependents->getSize());
			dependents->add(*r9barrelNuisance);
		}
		if (isLowR9cat()) {
			formula += Form("*(1.+@%d)",dependents->getSize());
			dependents->add(*r9mixedNuisance);
		}
	}
	RooFormulaVar *formVar = new RooFormulaVar(name.c_str(),name.c_str(),formula.c_str(),*dependents);
	return formVar;
}

void FinalModelConstruction::setupSystematics(){
  
	vertexNuisance = new RooRealVar(Form("CMS_hgg_nuisance_deltafracright"),Form("CMS_hgg_nuisance_deltafracright"),0.,-1.,1.);
	vertexNuisance->setConstant(true);
	if (isCutBased_) {
		r9barrelNuisance = new RooRealVar(Form("CMS_hgg_nuisance_%dTeVdeltar9barrel",sqrts_),Form("CMS_hgg_nuisance_%dTeVdeltar9barrel",sqrts_),0.,-1.,1.);
		r9mixedNuisance = new RooRealVar(Form("CMS_hgg_nuisance_%dTeVdeltar9mixed",sqrts_),Form("CMS_hgg_nuisance_%dTeVdeltar9mixed",sqrts_),0.,-1.,1.);
		r9barrelNuisance->setConstant(true);
		r9mixedNuisance->setConstant(true);
	}
  systematicsSet_=true;
}

void FinalModelConstruction::buildStdPdf(string name, int nGaussians, bool recursive){

  if (!systematicsSet_) setupSystematics();
  vector<RooAddPdf*> pdfs;
  pdfs = buildPdf(name.c_str(),nGaussians,recursive,stdSplines);
  finalPdf = pdfs[0];
  if (doSecondaryModels){
    assert(secondaryModelVarsSet);
    finalPdf_SM = pdfs[1];
    finalPdf_2 = pdfs[2];
    finalPdf_NW = pdfs[3];
  }
}

void FinalModelConstruction::buildRvWvPdf(string name, int nGrv, int nGwv, bool recursive){

  if (!rvFractionSet_) getRvFractionFunc(Form("%s_%s_cat%d_rvFracFunc",name.c_str(),proc_.c_str(),cat_));
  if (!systematicsSet_) setupSystematics();
  RooFormulaVar *rvFraction = new RooFormulaVar(Form("%s_%s_cat%d_rvFrac",name.c_str(),proc_.c_str(),cat_),Form("%s_%s_cat%d_rvFrac",name.c_str(),proc_.c_str(),cat_),"TMath::Min(@0+@1,1.0)",RooArgList(*vertexNuisance,*rvFracFunc));
  vector<RooAddPdf*> rvPdfs = buildPdf(name,nGrv,recursive,rvSplines,Form("_rv_%dTeV",sqrts_)); 
  vector<RooAddPdf*> wvPdfs = buildPdf(name,nGwv,recursive,wvSplines,Form("_wv_%dTeV",sqrts_)); 
  finalPdf = new RooAddPdf(Form("%s_%s_cat%d",name.c_str(),proc_.c_str(),cat_),Form("%s_%s_cat%d",name.c_str(),proc_.c_str(),cat_),RooArgList(*rvPdfs[0],*wvPdfs[0]),RooArgList(*rvFraction));
  if (doSecondaryModels){
    assert(secondaryModelVarsSet);
		RooFormulaVar *rvFraction_SM = new RooFormulaVar(Form("%s_%s_cat%d_rvFrac_SM",name.c_str(),proc_.c_str(),cat_),Form("%s_%s_cat%d_rvFrac",name.c_str(),proc_.c_str(),cat_),"TMath::Min(@0+@1,1.0)",RooArgList(*vertexNuisance,*rvFracFunc_SM));
		RooFormulaVar *rvFraction_2 = new RooFormulaVar(Form("%s_%s_cat%d_rvFrac_2",name.c_str(),proc_.c_str(),cat_),Form("%s_%s_cat%d_rvFrac",name.c_str(),proc_.c_str(),cat_),"TMath::Min(@0+@1,1.0)",RooArgList(*vertexNuisance,*rvFracFunc_2));
		RooFormulaVar *rvFraction_NW = new RooFormulaVar(Form("%s_%s_cat%d_rvFrac_NW",name.c_str(),proc_.c_str(),cat_),Form("%s_%s_cat%d_rvFrac",name.c_str(),proc_.c_str(),cat_),"TMath::Min(@0+@1,1.0)",RooArgList(*vertexNuisance,*rvFracFunc_NW));
		// buildNew Pdfs
    finalPdf_SM = new RooAddPdf(Form("%s_%s_cat%d_SM",name.c_str(),proc_.c_str(),cat_),Form("%s_%s_cat%d_SM",name.c_str(),proc_.c_str(),cat_),RooArgList(*rvPdfs[1],*wvPdfs[1]),RooArgList(*rvFraction_SM));
    finalPdf_2 = new RooAddPdf(Form("%s_%s_cat%d_2",name.c_str(),proc_.c_str(),cat_),Form("%s_%s_cat%d_2",name.c_str(),proc_.c_str(),cat_),RooArgList(*rvPdfs[2],*wvPdfs[2]),RooArgList(*rvFraction_2));
    finalPdf_NW = new RooAddPdf(Form("%s_%s_cat%d_NW",name.c_str(),proc_.c_str(),cat_),Form("%s_%s_cat%d_NW",name.c_str(),proc_.c_str(),cat_),RooArgList(*rvPdfs[3],*wvPdfs[3]),RooArgList(*rvFraction_NW));
  }
}

vector<RooAddPdf*> FinalModelConstruction::buildPdf(string name, int nGaussians, bool recursive, map<string,RooSpline1D*> splines, string add){
  
  vector<RooAddPdf*> result;

  RooArgList *gaussians = new RooArgList();
  RooArgList *coeffs = new RooArgList();
  string ext = Form("%s_cat%d%s",proc_.c_str(),cat_,add.c_str());
  
  // for SM Higgs as Background
  RooArgList *gaussians_SM = new RooArgList();
  RooArgList *coeffs_SM = new RooArgList();
  
  // for Second Higgs
  RooArgList *gaussians_2 = new RooArgList();
  RooArgList *coeffs_2 = new RooArgList();

  // for Natural Width
  RooArgList *voigtians_NW = new RooArgList();
  RooArgList *coeffs_NW = new RooArgList();

  for (int g=0; g<nGaussians; g++){
    RooAbsReal *dm = splines[Form("dm_g%d",g)];
    dm->SetName(Form("dm_g%d_%s",g,ext.c_str()));
    RooAbsReal *mean = getMeanWithPhotonSyst(dm,Form("mean_g%d_%s",g,ext.c_str()));
    RooAbsReal *sig_fit = splines[Form("sigma_g%d",g)];
    sig_fit->SetName(Form("sigma_g%d_%s",g,ext.c_str()));
    RooAbsReal *sigma = getSigmaWithPhotonSyst(sig_fit,Form("sig_g%d_%s",g,ext.c_str()));
		RooGaussian *gaus = new RooGaussian(Form("gaus_g%d_%s",g,ext.c_str()),Form("gaus_g%d_%s",g,ext.c_str()),*mass,*mean,*sigma);
    gaussians->add(*gaus);
    // add secondary models as well
    if (doSecondaryModels){
      assert(secondaryModelVarsSet);
      // sm higgs as background
      RooAbsReal *dmSM = splines[Form("dm_g%d_SM",g)];
      dmSM->SetName(Form("dm_g%d_%s_SM",g,ext.c_str()));
			RooAbsReal *meanSM = getMeanWithPhotonSyst(dmSM,Form("mean_g%d_%s_SM",g,ext.c_str()),false,true);
      RooAbsReal *sig_fitSM = splines[Form("sigma_g%d_SM",g)];
      sig_fitSM->SetName(Form("sigma_g%d_%s_SM",g,ext.c_str()));
			RooAbsReal *sigmaSM = getSigmaWithPhotonSyst(sig_fitSM,Form("sig_g%d_%s_SM",g,ext.c_str()));
      RooGaussian *gausSM = new RooGaussian(Form("gaus_g%d_%s_SM",g,ext.c_str()),Form("gaus_g%d_%s_SM",g,ext.c_str()),*mass,*meanSM,*sigmaSM);
      gaussians_SM->add(*gausSM);
      // second degen higgs
      RooAbsReal *dm2 = splines[Form("dm_g%d_2",g)];
      dm2->SetName(Form("dm_g%d_%s_2",g,ext.c_str()));
			RooAbsReal *mean2 = getMeanWithPhotonSyst(dm2,Form("mean_g%d_%s_2",g,ext.c_str()),true,false);
      RooAbsReal *sig_fit2 = splines[Form("sigma_g%d_2",g)];
      sig_fit2->SetName(Form("sigma_g%d_%s_2",g,ext.c_str()));
			RooAbsReal *sigma2 = getSigmaWithPhotonSyst(sig_fit2,Form("sig_g%d_%s_2",g,ext.c_str()));
      RooGaussian *gaus2 = new RooGaussian(Form("gaus_g%d_%s_2",g,ext.c_str()),Form("gaus_g%d_%s_2",g,ext.c_str()),*mass,*mean2,*sigma2);
      gaussians_2->add(*gaus2);
      // natural width
      RooVoigtian *voigNW = new RooVoigtian(Form("voig_g%d_%s_NW",g,ext.c_str()),Form("voig_g%d_%s_NW",g,ext.c_str()),*mass,*mean,*higgsDecayWidth,*sigma);
      voigtians_NW->add(*voigNW);
    }
    if (g<nGaussians-1) {
      RooAbsReal *frac = splines[Form("frac_g%d",g)];
      frac->SetName(Form("frac_g%d_%s",g,ext.c_str()));
      coeffs->add(*frac);
      // add secondary models as well
      if (doSecondaryModels){
        assert(secondaryModelVarsSet);
        // sm higgs as background
        RooAbsReal *fracSM = splines[Form("frac_g%d_SM",g)];
        fracSM->SetName(Form("frac_g%d_%s_SM",g,ext.c_str()));
        coeffs_SM->add(*fracSM);
        // second degen higgs
        RooAbsReal *frac2 = splines[Form("frac_g%d_2",g)];
        frac2->SetName(Form("frac_g%d_%s_2",g,ext.c_str()));
        coeffs_2->add(*frac2);
        // natural width
        coeffs_NW->add(*frac);
      }
    }
  }
  assert(gaussians->getSize()==nGaussians && coeffs->getSize()==nGaussians-1);
  RooAddPdf *pdf = new RooAddPdf(Form("%s_%s",name.c_str(),ext.c_str()),Form("%s_%s",name.c_str(),ext.c_str()),*gaussians,*coeffs,recursive);
  result.push_back(pdf);
  
  // add secondary models as well
  if (doSecondaryModels){
    assert(secondaryModelVarsSet);
    // sm higgs as background
    RooAddPdf *pdf_SM = new RooAddPdf(Form("%s_%s_SM",name.c_str(),ext.c_str()),Form("%s_%s_SM",name.c_str(),ext.c_str()),*gaussians_SM,*coeffs_SM,recursive);
    result.push_back(pdf_SM);
    // second degen higgs
    RooAddPdf *pdf_2 = new RooAddPdf(Form("%s_%s_2",name.c_str(),ext.c_str()),Form("%s_%s_2",name.c_str(),ext.c_str()),*gaussians_2,*coeffs_2,recursive);
    result.push_back(pdf_2);
    // natural width
    RooAddPdf *pdf_NW = new RooAddPdf(Form("%s_%s_NW",name.c_str(),ext.c_str()),Form("%s_%s_NW",name.c_str(),ext.c_str()),*voigtians_NW,*coeffs_NW,recursive);
    result.push_back(pdf_NW);
  }

  return result;
}

void FinalModelConstruction::setRVsplines(map<string,RooSpline1D*> splines){
  rvSplines = splines;
}

void FinalModelConstruction::setWVsplines(map<string,RooSpline1D*> splines){
  wvSplines = splines;
}

void FinalModelConstruction::setSTDsplines(map<string,RooSpline1D*> splines){
  stdSplines = splines;
}

void FinalModelConstruction::setRVdatasets(map<int,RooDataSet*> data){
  rvDatasets = data;
}

void FinalModelConstruction::setWVdatasets(map<int,RooDataSet*> data){
  wvDatasets = data;
}

void FinalModelConstruction::setSTDdatasets(map<int,RooDataSet*> data){
  stdDatasets = data;
}

void FinalModelConstruction::makeSTDdatasets(){
  for (unsigned int i=0; i<allMH_.size(); i++){
    int mh=allMH_[i];
		RooDataSet *data = (RooDataSet*)rvDatasets[mh]->Clone(Form("sig_%s_mass_m%d_cat%d",proc_.c_str(),mh,cat_));
		data->append(*wvDatasets[mh]);
		stdDatasets.insert(pair<int,RooDataSet*>(mh,data));
	}	
}

void FinalModelConstruction::plotPdf(string outDir){
  system(Form("mkdir -p %s",outDir.c_str()));
  
  TCanvas *canv = new TCanvas();
  RooPlot *dataPlot = mass->frame(Title(Form("%s_cat%d",proc_.c_str(),cat_)),Range(100,160));
  for (unsigned int i=0; i<allMH_.size(); i++){
    int mh=allMH_[i];
    stdDatasets[mh]->plotOn(dataPlot,Binning(160));
    MH->setVal(mh);
    extendPdf->plotOn(dataPlot);
  }
  dataPlot->Draw();
  canv->Print(Form("%s/%s_cat%d_fits.pdf",outDir.c_str(),proc_.c_str(),cat_));
  canv->Print(Form("%s/%s_cat%d_fits.png",outDir.c_str(),proc_.c_str(),cat_));
  
  RooPlot *pdfPlot = mass->frame(Title(Form("%s_cat%d",proc_.c_str(),cat_)),Range(100,160));
	pdfPlot->GetYaxis()->SetTitle(Form("Pdf projection / %2.1f GeV",(mass->getMax()-mass->getMin())/160.));
  for (int mh=mhLow_; mh<=mhHigh_; mh++){
    MH->setVal(mh);
		// to get correct normlization need to manipulate with bins and range
    extendPdf->plotOn(pdfPlot,Normalization(mass->getBins()/160.*(mass->getMax()-mass->getMin())/60.,RooAbsReal::RelativeExpected));
  }
  pdfPlot->Draw();
  canv->Print(Form("%s/%s_cat%d_interp.pdf",outDir.c_str(),proc_.c_str(),cat_));
  canv->Print(Form("%s/%s_cat%d_interp.png",outDir.c_str(),proc_.c_str(),cat_));
  delete canv;

}

RooSpline1D* FinalModelConstruction::graphToSpline(string name, TGraph *graph){
  
  vector<double> xValues, yValues;
  for (double mh=mhLow_; mh<(mhHigh_+0.25); mh+=0.5){
    xValues.push_back(mh);
    yValues.push_back(graph->Eval(mh));
  }
  RooSpline1D *res = new RooSpline1D(name.c_str(),name.c_str(),*MH,xValues.size(),&(xValues[0]),&(yValues[0]));
  return res;
}

RooSpline1D* FinalModelConstruction::graphToSpline(string name, TGraph *graph, RooAbsReal *var){
  
  vector<double> xValues, yValues;
  for (double mh=mhLow_; mh<(mhHigh_+0.25); mh+=0.5){
    xValues.push_back(mh);
    yValues.push_back(graph->Eval(mh));
  }
  RooSpline1D *res = new RooSpline1D(name.c_str(),name.c_str(),*var,xValues.size(),&(xValues[0]),&(yValues[0]));
  return res;
}

void FinalModelConstruction::getNormalization(){
 
  TGraph *temp = new TGraph();
  TF1 *pol2 = new TF1("pol","pol2",110,150);
  for (unsigned int i=0; i<allMH_.size(); i++){
    double mh = double(allMH_[i]);
    RooDataSet *data = stdDatasets[mh];
    // calcu eA as sumEntries / totalxs * totalbr * intL
    double effAcc = (data->sumEntries()/(intLumi->getVal()*norm->GetXsection(mh,proc_)*norm->GetBR(mh)));
    temp->SetPoint(i,mh,effAcc);
  }
  verbosity_ >=2 ?
    temp->Fit(pol2,"EMFEX0") :
    temp->Fit(pol2,"QEMFEX0")
  ;
  TGraph *eaGraph = new TGraph(pol2);
  RooSpline1D *eaSpline = graphToSpline(Form("fea_%s_cat%d_%dTeV",proc_.c_str(),cat_,sqrts_),eaGraph);
  RooSpline1D *xs = xsSplines[proc_];
	RooAbsReal *rateNuisTerm = getRateWithPhotonSyst(Form("rate_%s_cat%d_%dTeV",proc_.c_str(),cat_,sqrts_));
  
	finalNorm = new RooFormulaVar(Form("%s_norm",finalPdf->GetName()),Form("%s_norm",finalPdf->GetName()),"@0*@1*@2*@3",RooArgList(*xs,*brSpline,*eaSpline,*rateNuisTerm));
  // these are for plotting
  finalNormThisLum = new RooFormulaVar(Form("%s_normThisLumi",finalPdf->GetName()),Form("%s_normThisLumi",finalPdf->GetName()),"@0*@1*@2*@3*@4",RooArgList(*xs,*brSpline,*eaSpline,*rateNuisTerm,*intLumi));
  
	extendPdfRel = new RooExtendPdf(Form("extend%s",finalPdf->GetName()),Form("extend%s",finalPdf->GetName()),*finalPdf,*finalNorm);
  extendPdf = new RooExtendPdf(Form("extend%sThisLumi",finalPdf->GetName()),Form("extend%sThisLumi",finalPdf->GetName()),*finalPdf,*finalNormThisLum);
  // do secondary models
  if (doSecondaryModels){
    assert(secondaryModelVarsSet);
    // sm higgs as bkg
    RooSpline1D *eaSpline_SM = graphToSpline(Form("fea_%s_cat%d_%dTeV_SM",proc_.c_str(),cat_,sqrts_),eaGraph,MH_SM);
    RooSpline1D *xs_SM = xsSplines_SM[proc_];
    finalNorm_SM = new RooFormulaVar(Form("%s_norm",finalPdf_SM->GetName()),Form("%s_norm",finalPdf_SM->GetName()),"@0*@1*@2*@3",RooArgList(*xs_SM,*brSpline_SM,*eaSpline_SM,*rateNuisTerm));
    // second degen higgs
    RooSpline1D *eaSpline_2 = graphToSpline(Form("fea_%s_cat%d_%dTeV_2",proc_.c_str(),cat_,sqrts_),eaGraph,MH_2);
    RooSpline1D *xs_2 = xsSplines_2[proc_];
    finalNorm_2 = new RooFormulaVar(Form("%s_norm",finalPdf_2->GetName()),Form("%s_norm",finalPdf_2->GetName()),"@0*@1*@2*@3",RooArgList(*xs_2,*brSpline_2,*eaSpline_2,*rateNuisTerm));
    // natural width
    RooSpline1D *eaSpline_NW = graphToSpline(Form("fea_%s_cat%d_%dTeV_NW",proc_.c_str(),cat_,sqrts_),eaGraph,MH);
    RooSpline1D *xs_NW = xsSplines_NW[proc_];
    finalNorm_NW = new RooFormulaVar(Form("%s_norm",finalPdf_NW->GetName()),Form("%s_norm",finalPdf_NW->GetName()),"@0*@1*@2*@3",RooArgList(*xs_NW,*brSpline_NW,*eaSpline_NW,*rateNuisTerm));
  }

}

void FinalModelConstruction::save(RooWorkspace *work){
  work->import(*finalPdf,RecycleConflictNodes());
  work->import(*finalNorm,RecycleConflictNodes());
  work->import(*finalNormThisLum,RecycleConflictNodes());
  work->import(*extendPdf,RecycleConflictNodes());
  work->import(*extendPdfRel,RecycleConflictNodes());
  for (map<int,RooDataSet*>::iterator it=stdDatasets.begin(); it!=stdDatasets.end(); it++){
    work->import(*(it->second));
  }

  // do secondary models
	if (doSecondaryModels){
		work->import(*finalPdf_SM,RecycleConflictNodes());
		work->import(*finalPdf_2,RecycleConflictNodes());
		work->import(*finalPdf_NW,RecycleConflictNodes());
		work->import(*finalNorm_SM,RecycleConflictNodes());
		work->import(*finalNorm_2,RecycleConflictNodes());
		work->import(*finalNorm_NW,RecycleConflictNodes());
	}
}
