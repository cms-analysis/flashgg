#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <map>
#include <set>

#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/predicate.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string/replace.hpp"
#include "boost/algorithm/string/erase.hpp"

using namespace std;
using namespace boost;

void stripSpace(string &line){
	stringstream lineSt(line);
	line="";
	string word;
	while (lineSt >> word) {
		line.append(word).append(" ");
	}
	line = line.substr(0,line.size()-1);
}

void printVec(vector<string> vec){

	cout << "[";
	for (unsigned i=0; i<vec.size()-1;i++){
		cout << vec[i] << ",";
	}
	cout << vec[vec.size()-1] << "]" << endl;
}

void printMap(map<string,map<int,map<string,double> > > &theMap){

	for (map<string,map<int,map<string,double> > >::iterator p = theMap.begin(); p != theMap.end(); p++) {
		for (map<int,map<string,double> >::iterator c = p->second.begin(); c != p->second.end(); c++){
			cout << "Proc = " << p->first << "  DiphotonCat: " << c->first << endl;
			for (map<string,double>::iterator m = c->second.begin(); m != c->second.end(); m++){
				cout << "\t " << m->first << " : " << m->second << endl;
			}
		}
	}
}

void addToMap(map<string,map<int,map<string,double> > > &theMap, string proc, int diphotonCat, string phoSystName, double var){
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

void readSystDatFile(string datfilename) {

	vector<string> photonCats;
	//map<int,map<string,double> > meanSysts;
	//map<int,map<string,double> > sigmaSysts;
	//map<int,map<string,double> > rateSysts;
	map<string,map<int,map<string,double> > > meanSysts;
	map<string,map<int,map<string,double> > > sigmaSysts;
	map<string,map<int,map<string,double> > > rateSysts;

	// upstairs

	int diphotonCat=-1;
	string proc;
	ifstream datfile;
  datfile.open(datfilename.c_str());
  if (datfile.fail()) exit(1);
  while (datfile.good()){
	
		string line;
		getline(datfile,line);

		// The input file needs correct ordering
    if (line=="\n" || line.substr(0,1)=="#" || line==" " || line.empty()) continue;
		// First read the photon categories
		if (starts_with(line,"photonCats")){
			line = line.substr(line.find("=")+1,string::npos);
			split(photonCats,line,boost::is_any_of(","));
			cout << "PhotonVec: ";
			printVec(photonCats);
		}
		// Then read diphoton cat
		else if (starts_with(line,"diphotonCat")){
			diphotonCat = boost::lexical_cast<int>(line.substr(line.find("=")+1,string::npos));
		}
		// And the process
		else if (starts_with(line,"proc")){
			proc = line.substr(line.find('=')+1,string::npos);
			cout << "Process:  " << proc << "  DiphoCat: " << diphotonCat << endl;
		}
		// Then read values
		else {
			stripSpace(line);
			vector<string> els;
			split(els,line,boost::is_any_of(" "));
			printVec(els);
			if (els.size()!=4) {
				cout << "I cant read this datfile " << endl;
				exit(1);
			}
			string phoSystName = els[0];
			double meanCh = lexical_cast<double>(els[1]);
			double sigmaCh = lexical_cast<double>(els[2]);
			double rateCh = lexical_cast<double>(els[3]);
	
			addToMap(meanSysts,proc,diphotonCat,phoSystName,meanCh);
			addToMap(sigmaSysts,proc,diphotonCat,phoSystName,sigmaCh);
			addToMap(rateSysts,proc,diphotonCat,phoSystName,rateCh);

		}
	
	}
	datfile.close();
	
	cout << "MEAN: ";
	printMap(meanSysts);
	cout << "SIGMA: ";
	printMap(sigmaSysts);
	cout << "RATE: ";
	printMap(rateSysts);

}

int main(){
	readSystDatFile("dat/photonCatSyst.dat");
}
