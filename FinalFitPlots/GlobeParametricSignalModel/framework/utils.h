#ifndef _utils_h
#define _utils_h

#include <RooDataSet.h>
#include <RooRealVar.h>
#include <TString.h>

#include <boost/lexical_cast.hpp>

#include <string>
#include <iostream>
#include <cstdlib>
#include <string>

//----------------------------------------------------------------------


/** running interactively with ROOT, the standard assert creates a confusing situation... */
#define ASSERT(condition) \
  if (! (condition)) { \
    std::cerr << "Assertion '" << #condition << "' failed in " << __FILE__ << ":" << __LINE__ << std::endl; \
    exit(1); \
  }

/** assert causing a break (useful when running inside a debugger) */
#define ASSERTBREAK(condition) \
  if (! (condition)) { \
    std::cerr << "Assertion '" << #condition << "' failed in " << __FILE__ << ":" << __LINE__ << std::endl; \
    raise(SIGINT); \
  }


//----------------------------------------------------------------------

/** this is actually guesswork (i.e. not documented anywhere): we think
 *  that if RooAbsPdf::fitTo()->status() is zero, that means that the
 *  fit converged.
 */
#define FIT_STATUS_OK 0

//----------------------------------------------------------------------

/** used for RooFormulaVar: creates a string of the form @i + ... + @j */
std::string makeArgSum(int first, int last);


/** assigns a ratio with ClopperPearson uncertainty to a RooRealVar */
void assignRatio(RooRealVar &var, double numerator, double denominator);

template<class T>
std::string toStr(const T& x)
{
  return boost::lexical_cast<std::string>(x);
}

//----------------------------------------------------------------------

inline void swapValues(RooRealVar &v1, RooRealVar &v2)
{
  // TODO: what about the errors / ranges ?
  double tmp = v2.getVal();
  v2.setVal(v1.getVal());
  v1.setVal(tmp);
}

//----------------------------------------------------------------------

/** tries to find a name which has not yet been used in the ROOT
 *  dictionary of names.
 */
std::string getUnusedROOTobjectName(const std::string &prefix = "o");

/** makes a piecewise linear function over the interpolating points */
RooAbsReal *makePiecewiseLinearFunction(const std::string &funcName, RooAbsReal &xvar, const std::vector<double> &xvalues,
    const std::vector<double> &yvalues, bool isSorted = true);


/** kept here to have a consistent naming scheme across the code for
    the normalization function of the per category top level
    pdfs */
inline std::string normalizationFunctionName(const std::string &catname, const std::string &inputSigProcName)
{
  return "hggpdfrel_" + catname + "_" + inputSigProcName + "_norm";
}

#endif
