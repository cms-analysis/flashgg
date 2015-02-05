#include "TSpline.h"
#include "TGraph.h"
#include <iostream>

class  GraphToTF1 {
public:
	GraphToTF1( TString name, TGraph * g ) { sp_ = new TSpline3(name,g); };
	double operator() (double *x, double *p) {
		return  sp_->Eval( x[0] ) - p[0];
	};
	TSpline * sp_;
};
