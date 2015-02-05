// Local regression!
//

#include <cmath>
#include <iostream>

// Is this necessary for a linear regression?
#include "TMinuit.h"
#include "TH2F.h"
#include "TMatrixD.h"

// Some global variables !(probably should throw them into a Struct)

TH2F *theHist;

int nBinsX;
int nBinsY;

double band;

int curX01;
int curX02;

double parAll[4];
double errAll[4];

double W(double d,double b){

	// Tri weight function
	double ret = 0.;
	double u = fabs(d)/b;
	if ( u > 1.) ret = 0;
	else {
		double u3 = u*u*u; 
		ret = (1-u3)*(1-u3)*(1-u3);
	}

	return ret;
}


double distance(int x1, int x2, int x01, int x02){
	
	double dx = (double)(x1-x01);
	double dy = (double)(x2-x02);
	
	return sqrt(dx*dx + dy*dy);
}
// MINUIT Fitting Function
void fcnFitRegression(Int_t &npar, Double_t *gin, Double_t &f, Double_t *p, Int_t iflag){

	// Loop over the bins in the Histogram
	
	double sum = 0;

	for (int k=1;k<=nBinsX;k++){
	  for (int l=1;l<=nBinsY;l++){
		double weight = W(distance(k,l,curX01,curX02),band);
		// linear approximation
		double taylor = p[0]+p[1]*(k-curX01)+p[2]*(l-curX02)+p[3]*(k-curX01)*(l-curX02)
			      + 0.5*p[4]*(k-curX01)*(k-curX01) + 0.5*p[5]*(l-curX02)*(l-curX02);

		double diff = (theHist->GetBinContent(k,l) - taylor);
		sum+=weight*diff*diff;
	  }
        }
	
	f = sum;
}


void runPolyTMINUIT(TH2F* newHist){
	  // Doesn't seem to work very well !

	  int ell[5]={0,0,0,0,0};
	  static Double_t p1=1;
	  static Double_t p5=1;

	  int nPar = 6; // in 2 dimensions

	  for (int k=1;k<=nBinsX;k++){
	    for (int l=1;l<=nBinsY;l++){
		
		curX01=k;
		curX02=l;
		
		// create a TMinuit!
		TMinuit tMinuit(nPar);
		tMinuit.mninit(5,6,7);
		tMinuit.SetFCN(fcnFitRegression);
		tMinuit.SetPrintLevel(-1); // shuts up TMVA
		tMinuit.SetMaxIterations(10000);

		tMinuit.DefineParameter(0,Form("Par%d",0),theHist->GetBinContent(k,l),0.001,0.000,0.000);
		for (int i=1;i<nPar;i++){
		  tMinuit.DefineParameter(i,Form("Par%d",i),0.01,0.001,0.000,0.000);
		}

		tMinuit.mnexcm("CALL FCN", &p1 ,1,ell[0]);
		tMinuit.mnexcm("SCAN"    , &p1 ,0,ell[1]);

		ell[3]=tMinuit.Migrad();
		tMinuit.mnexcm("CALL FCN", &p5 ,1,ell[4]);

		for (int i=0;i<nPar;i++){
		  tMinuit.GetParameter(i,parAll[i],errAll[i]);
		}

		// now set the bin content of the function at k,l
		newHist->SetBinContent(k,l,parAll[0]);
			
	    }
          }
}

double getB0poly(){

	// Will be finding a load of summations
	double sumw = 0.;

	double sumdx     = 0.;
	double sumdx2    = 0.;
	double sumdx3    = 0.;
	double sumdx2dy  = 0.;
	double sumdx3dy  = 0.;
	double sumdx4  = 0.;

	double sumdy   = 0.;
	double sumdy2  = 0.;
	double sumdy3  = 0.;
	double sumdy2dx  = 0.;
	double sumdy3dx  = 0.;
	double sumdy4  = 0.;

	double sumdydx = 0.;
	double sumdy2dx2 = 0.;

	double sumz   = 0.;
	double sumdxz = 0.;
	double sumdyz = 0.;
	double sumdydxz = 0.;
	double sumdx2z = 0.;
	double sumdy2z = 0.;

	for (int k=1;k<=nBinsX;k++){
	   for (int l=1;l<=nBinsY;l++){
	
		double weight = W(distance(k,l,curX01,curX02),band);
		sumw+=weight;

		sumdx+=weight*(k-curX01);
		sumdx2+=weight*(k-curX01)*(k-curX01);
		sumdx2dy+=weight*(k-curX01)*(k-curX01)*(l-curX02);
		sumdx3+=weight*(k-curX01)*(k-curX01)*(k-curX01);
		sumdx3dy+=weight*(k-curX01)*(k-curX01)*(k-curX01)*(l-curX02);
		sumdx4+=weight*(k-curX01)*(k-curX01)*(k-curX01)*(k-curX01);

		sumdy+=weight*(l-curX02);
		sumdy2+=weight*(l-curX02)*(l-curX02);
		sumdy2dx+=weight*(l-curX02)*(l-curX02)*(k-curX01);
		sumdy3+=weight*(l-curX02)*(l-curX02)*(l-curX02);
		sumdy3dx+=weight*(l-curX02)*(l-curX02)*(l-curX02)*(k-curX01);
		sumdy4+=weight*(l-curX02)*(l-curX02)*(l-curX02)*(l-curX02);

		sumdydx+=weight*(l-curX02)*(k-curX01);
		sumdy2dx2+=weight*(l-curX02)*(k-curX01)*(l-curX02)*(k-curX01);

		double z = theHist->GetBinContent(k,l);
		sumz+=weight*z;
		sumdyz+=weight*z*(l-curX02);
		sumdxz+=weight*z*(k-curX01);
		sumdydxz+=weight*z*(k-curX01)*(l-curX02);
		sumdx2z+=weight*z*(k-curX01)*(k-curX01);
		sumdy2z+=weight*z*(l-curX02)*(l-curX02);
	   }		
	}

	double entries[36];
        entries[0]=sumw;         entries[1]=sumdx;          entries[2]=sumdy;          entries[3]=sumdydx;       entries[4]=0.5*sumdx2;          entries[5]=0.5*sumdy2;
        entries[6]=sumdx;        entries[7]=sumdx2;         entries[8]=sumdydx;        entries[9]=sumdx2dy;      entries[10]=0.5*sumdx3;         entries[11]=0.5*sumdy2dx;
        entries[12]=sumdy;       entries[13]=sumdydx;       entries[14]=sumdy2;        entries[15]=sumdy2dx;     entries[16]=0.5*sumdx2dy;       entries[17]=0.5*sumdy3;
        entries[18]=sumdydx;     entries[19]=sumdx2dy;      entries[20]=sumdy2dx;      entries[21]=sumdy2dx2;    entries[22]=0.5*sumdx3dy;       entries[23]=0.5*sumdy3dx;
        entries[24]=0.5*sumdx2;  entries[25]=0.5*sumdx3;    entries[26]=0.5*sumdx2dy;  entries[27]=0.5*sumdx3dy; entries[28]=0.5*0.5*sumdx4;     entries[29]=0.5*0.5*sumdy2dx2;
        entries[30]=0.5*sumdy2;  entries[31]=0.5*sumdy2dx;  entries[32]=0.5*sumdy3;    entries[33]=0.5*sumdy3dx; entries[34]=0.5*0.5*sumdy2dx2;  entries[35]=0.5*0.5*sumdy4;

	TMatrixD M(6,6);
	M.SetMatrixArray(entries);
        M.Invert();

	double a = ( M(0,0)*sumz + M(0,1)*sumdxz + M(0,2)*sumdyz + M(0,3)*sumdydxz + M(0,4)*0.5*sumdx2z + M(0,5)*0.5*sumdy2z);
	if (a < 0) {return 0;}
	else return a;
}

double getB0(){

	// Will be finding a load of summations
	double sumw = 0.;

	double sumdx   = 0.;
	double sumdx2  = 0.;
	double sumdy   = 0.;
	double sumdy2  = 0.;
	double sumdydx = 0.;

	double sumz   = 0.;
	double sumdxz = 0.;
	double sumdyz = 0.;

	for (int k=1;k<=nBinsX;k++){
	   for (int l=1;l<=nBinsY;l++){
	
		double weight = W(distance(k,l,curX01,curX02),band);
		sumw+=weight;
		sumdx+=weight*(k-curX01);
		sumdx2+=weight*(k-curX01)*(k-curX01);
		sumdy+=weight*(l-curX02);
		sumdy2+=weight*(l-curX02)*(l-curX02);
		sumdydx+=weight*(l-curX02)*(k-curX01);

		sumz+=weight*theHist->GetBinContent(k,l);
		sumdyz+=weight*theHist->GetBinContent(k,l)*(l-curX02);
		sumdxz+=weight*theHist->GetBinContent(k,l)*(k-curX01);
	   }		
	}

	double entries[9];
        entries[0]=sumw;  entries[1]=sumdx;  entries[2]=sumdy;
        entries[3]=sumdx; entries[4]=sumdx2; entries[5]=sumdydx;
        entries[6]=sumdy; entries[7]=sumdydx; entries[8]=sumdy2;

	TMatrixD M(3,3);
	M.SetMatrixArray(entries);
        M.Invert();
        double a = ( M(0,0)*sumz + M(0,1)*sumdxz + M(0,2)*sumdyz );
	if (a < 0) {return 0;}
	else return a;

}

void runLinearRegression(TH2F* newHist, int mode){

	  for (int k=1;k<=nBinsX;k++){
	    for (int l=1;l<=nBinsY;l++){
			
		curX01=k;
		curX02=l;
		
		// now set the bin content of the function at k,l
		if (mode == 0)	newHist->SetBinContent(k,l,getB0());
		else newHist->SetBinContent(k,l,getB0poly());
	    }
	  }

}

void smoothedHistogram(TH2F * myHist, double bandwidth=0.1, double iflag_ = 0){

	TH2F *newHist = (TH2F*) myHist->Clone();
	theHist = myHist;

	if (bandwidth >= 1. || bandwidth <= 0 ){
		std::cout << "bandwidth must be between 0 and 1, will not smooth!" << std::endl;
		return ;
	}
	
	nBinsX = newHist->GetNbinsX();
	nBinsY = newHist->GetNbinsY();
	int nTotalBins = nBinsX*nBinsY;
	band = bandwidth*nTotalBins;
	
	// Use a Local Regression technique !


	if (iflag_ == 0 || iflag_== 1){ // Linear Regression
	  runLinearRegression(newHist,iflag_);
	}
	else {		// 2nd order TMINUIT (slow)
	  runPolyTMINUIT(newHist);
	}

	// only care for shape info
	double integral = theHist->Integral();

	for (int k=1;k<=nBinsX;k++){
	  for (int l=1;l<=nBinsY;l++){
		theHist->SetBinContent(k,l,newHist->GetBinContent(k,l));
	  }
	}
	
	theHist->Scale(integral/theHist->Integral());

	delete newHist;
	//return newHist;

}
