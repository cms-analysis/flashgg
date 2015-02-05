#ifndef logn_HH
#define logn_HH

#include <cmath>

double logn(double x, unsigned p=2) {
  static const double logx0(-10000.0);
  static const double x0(exp(logx0));
  static const double x02(x0*x0);

  if(x>=x0) return log(x);
  //return log(x);
  if(p==0) return logx0;
  if(p==1) return x/x0-1.0+logx0;
  return -0.5*x*x/x02+2.0*x/x0-1.5+logx0;
}
double poissonChiSquared(double m, unsigned n, unsigned p=2) {
  static const double chiSq0(100.0);
  static const double lnm0(-0.5*chiSq0);
  static const double m0(exp(lnm0));
  //static const double m0(0.08);
  static const double inverseM0(1.0/m0);

  //std::cout << "m0 = " << m0 << std::endl;
  //if(m<0.0) std::cout << "poissonChiSquared()  WARNING m = "
  //	      << m << ", n = " << n << ", p = " << p;

  double c2;

  // Treat zero events as special case
  if(n==0) {
    if(m>=m0) {
      c2=2.0*m;

    } else {
      if(p==0) {
	if(m>=0.0) c2=2.0*m;
	else       c2=1.0e300;
	
      } else if(p==1) {
	if(m>=0.0) c2=2.0*m;
	else       c2=-2.0*m*inverseM0;
	
      } else {
	c2=m*m*inverseM0+m0;
      }
    }

    if(c2<0.0) {
      std::cout << "poissonChiSquared(" << m << ", " << n << ", " << p
		<< ") gives chi-sq = " << c2 << ", forcing to ";
      std::cerr << "poissonChiSquared(" << m << ", " << n << ", " << p
		<< ") gives chi-sq = " << c2 << ", forcing to ";

      c2=0.0;

      std::cout << c2 << std::endl;
      std::cerr << c2 << std::endl;
    }

    // Non-zero events
  } else {
    double scale((m-n)/n);
    if(fabs(scale)<1.0e-6) c2=n*scale*scale*(1.0-2.0*scale/3.0);
    else c2=2.0*n*(scale-logn(1.0+scale,p));
    
    /*
      if(m>=m0) {
      c2=2.0*(m-n+n*log(n/m));

      } else {
      if(p==0) {
      c2=2.0*(m-n-n*logn(m/n,0));
	
      } else if(p==1) {
      c2=2.0*(m-(n*m/m0)+n*log(n/m0));
	
      } else {    
      c2=n*m*m*inverseM0*inverseM0+2.0*(m-2.0*n*m*inverseM0)+n+2.0*n*log(n*inverseM0);
      }
      }
    */

    if(c2<0.0) {
      std::cout << "poissonChiSquared(" << m << ", " << n << ", " << p
		<< ") gives chi-sq = " << c2 << ", forcing to ";
      std::cerr << "poissonChiSquared(" << m << ", " << n << ", " << p
		<< ") gives chi-sq = " << c2 << ", forcing to ";

      c2=(m-n)*(m-n)/n;

      std::cout << c2 << std::endl;
      std::cerr << c2 << std::endl;
    }
  }

  //assert(c2>=0.0);
  if(c2<0.0) {
    c2=1.0e200;
    std::cout << "WARNING Chi2 = " << c2 << std::endl;
  }

  return c2;
}

#endif
