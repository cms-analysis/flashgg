#ifndef __IsolationCorrection__
#define __IsolationCorrection__

#include "TFile.h"
#include "TVectorD.h"
#include "TH1.h"
#include "TRandom.h"
    
#include <iostream>
    
std::vector<float> convertToStd(TVectorD * input)
{
	std::vector<float> ret(input->GetNrows());
	for(int iel=0; iel < input->GetNrows(); ++iel) { ret[iel] = (*input)[iel]; }
    
	return ret;
}

std::vector<int> convertToStdInt(TVectorD * input)
{
	std::vector<int> ret(input->GetNrows());
	for(int iel=0; iel < input->GetNrows(); ++iel) { ret[iel] = (*input)[iel]; }
    
	return ret;
}

class IsolationCorrection
{
public:
	IsolationCorrection(const char * fname) {
		auto * fin = TFile::Open(fname);

		eta_centers_ = convertToStd(dynamic_cast<TVectorD*>(fin->Get("eta_centers"))); 
		rho_centers_eb_ = convertToStd(dynamic_cast<TVectorD*>(fin->Get("rho_centers_eb"))); 
		rho_centers_ee_ = convertToStd(dynamic_cast<TVectorD*>(fin->Get("rho_centers_ee")));    
		extra_multiplicity_ = convertToStd(dynamic_cast<TVectorD*>(fin->Get("extra_multiplicity")));    
		auto * multiplicity_offset = dynamic_cast<TVectorD*>(fin->Get("multiplicity_offset"));
		if( multiplicity_offset != 0 ) {
			multiplicity_offset_ = convertToStdInt(multiplicity_offset);
		}
		auto * extra_multiplicity_slope = dynamic_cast<TVectorD*>(fin->Get("extra_multiplicity_slope"));
		if( extra_multiplicity_slope != 0 ) {
			extra_multiplicity_slope_ = convertToStd(extra_multiplicity_slope);
		}
		
		n_rho_centers_ = std::max(rho_centers_eb_.size(),rho_centers_ee_.size());
        
		histograms_.resize(n_rho_centers_*eta_centers_.size(),0);
        
		for(size_t ieta=0; ieta<eta_centers_.size(); ++ieta) {
			auto & eta = eta_centers_[ieta];
			// std::cerr << eta << std::endl;
			auto & rho_centers = ( eta<1.5 ? rho_centers_eb_ : rho_centers_ee_ );
            
			for(size_t irho=0; irho<rho_centers.size(); ++irho) {
				auto ibin = index(ieta,irho);
				auto & rho = rho_centers[irho];

				/// std::cerr << rho << std::endl;
				TString name = TString::Format("hist_%1.3f_%1.2f",eta,rho);
				name.ReplaceAll(".","p");
				// std::cerr << name << std::endl;
				auto hist = dynamic_cast<TH1*>(fin->Get(name)->Clone());
				// std::cerr << name << " " << hist << std::endl;
				hist->SetDirectory(0);
                
				histograms_[ibin] = hist;
			}
		} 
        
		fin->Close();
	}

	
	float getExtra(float eta, float rho, float extraMult=0) {
        
		int ieta = findIndex(eta,eta_centers_);
        
		if( extraMult <= 0 ) {
			extraMult = extra_multiplicity_[ieta] + ( ! extra_multiplicity_slope_.empty() ? extra_multiplicity_slope_[ieta] * rho : 0. );
		}
		float floor = std::floor(extraMult);
		int ngen = int(floor);
		// std::cout << extraMult << " " << floor << " " << ngen << std::endl;
		float rnd = gRandom->Uniform();
		if( rnd < extraMult - floor ) { ngen += 1; }
		// std::cout << extraMult << " " << floor << " " << rnd << " " << ngen << std::endl;
		
		if( ngen == 0 ) { return 0.; }


		auto & rho_centers = eta_centers_[ieta] < 1.5 ? rho_centers_eb_ : rho_centers_ee_;
		int irho = findIndex(rho, rho_centers);
		if ( ! multiplicity_offset_.empty() ) {
			irho = std::max(0,( multiplicity_offset_[ieta] < 10 ? irho - multiplicity_offset_[ieta] : 0 ));
		}
		
		auto ibin = index(ieta,irho);        
		auto * hist = histograms_[ibin];
        
		float extra = 0.;
		float thr = hist->GetXaxis()->GetBinLowEdge(2);
		while( ngen > 0 ) {
			--ngen;
			float gen = hist->GetRandom();
			if( gen > thr ) extra += gen;
		}
	
		return extra;
	}
	
	//private:    
	int index(int ieta, int irho) {
		return ieta*n_rho_centers_ + irho;
	}
        
	int findIndex(float val, std::vector<float> & vec) {
	    
		if( val < vec.front() ) return 0;
		if( val >= vec.back() ) return vec.size()-1;
		int lb = std::distance(vec.begin(), std::lower_bound(vec.begin(),vec.end(), val) );
		return ( fabs(vec[lb] - val) < fabs(vec[lb-1]-val) ? lb : lb -1) ;  
	}
	
	std::vector<TH1 *> histograms_; 
	size_t n_rho_centers_;

	std::vector<float>  eta_centers_, rho_centers_eb_, rho_centers_ee_, extra_multiplicity_,  extra_multiplicity_slope_;
	std::vector<int> multiplicity_offset_;
    
};

#endif // __IsolationCorrection__
