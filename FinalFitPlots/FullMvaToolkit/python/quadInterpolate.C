#include "TMatrixD.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLatex.h"
#include <iostream>
#include <string>

using namespace std;

double quadInterpolate(double C, double X1,double X2,double X3,double Y1,double Y2,double Y3){

        gROOT->SetStyle("Plain");
        gROOT->SetBatch(true);
        gStyle->SetOptStat(0);
        // Use the 3 points to determine a,b,c
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

	// Check for Nan
	double RESULT = func.Eval(C);
	if (RESULT != RESULT || fabs(1-RESULT/Y2) < 0.00001 ) RESULT=Y2;

        //delete [] entries;
        return RESULT/Y2; // relative difference
}

