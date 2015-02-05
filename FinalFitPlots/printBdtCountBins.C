// A script to dump the optimized Bin edges for the inclusive BDT part of the full-mva analysis (to save on running over background to rederive these edges)

#include <iostream>
#include "TH1F.h"
#include "TFile.h"

void printBdtCountBins(std::string filename){
	TFile *_file0 = TFile::Open(filename.c_str());
	int masses[8]={110,115,120,125,130,135,140,150};

	for (int i=0;i<8;i++){
		TH1F *hist_grad = (TH1F*)_file0->Get(Form("th1f_data_grad_%3.1f_cat0",float(masses[i])));
		TH1F *hist_ada = (TH1F*)_file0->Get(Form("th1f_data_ada_%3.1f_cat0",float(masses[i])));
		std::cout << "GradBinEdges_"<<masses[i]<<"=";
		for (int j=1;j<=hist_grad->GetNbinsX();j++){
			if (j!=hist_grad->GetNbinsX()) std::cout << hist_grad->GetBinLowEdge(j)<<",";
			else std::cout << hist_grad->GetBinLowEdge(j);
		}
		//std::cout <<"1"<<std::endl;
		std::cout <<std::endl;
		std::cout << "AdaBinEdges_"<<masses[i]<<"=";
		for (int j=1;j<=hist_ada->GetNbinsX();j++){
			if (j!=hist_ada->GetNbinsX()) std::cout << hist_ada->GetBinLowEdge(j)<<",";
			else  std::cout << hist_ada->GetBinLowEdge(j);

		}
		//std::cout<<"1"<<std::endl;
		std::cout <<std::endl;
	}
}
