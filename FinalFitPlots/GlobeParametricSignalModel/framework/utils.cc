#include "utils.h"
#include <boost/lexical_cast.hpp>
#include <TEfficiency.h>

#include <algorithm>

using namespace std;

//----------------------------------------------------------------------

/** used for RooFormulaVar: creates a string of the form @i + ... + @j */
string
makeArgSum(int first, int last)
{
  ASSERT(first <= last);
  string retval = "@" + boost::lexical_cast<string>(first);

  for (int i = first + 1; i <= last; ++i)
    retval += " + @" + boost::lexical_cast<string>(i);

  return retval;
}

//----------------------------------------------------------------------

void
assignRatio(RooRealVar &var, double numerator, double denominator)
{
  double ratio = numerator / denominator;
  double errorLow = TEfficiency::ClopperPearson(Int_t(denominator), Int_t(numerator), 0.683, kFALSE) - ratio;
  double errorHigh = TEfficiency::ClopperPearson(Int_t(denominator), Int_t(numerator), 0.683, kTRUE) - ratio;

  var.setVal(ratio);
  var.setAsymError(errorLow, errorHigh);
}
//----------------------------------------------------------------------

class IndexedLess
{
protected:
  const std::vector<double> &values;

public:
  IndexedLess(const std::vector<double> &_values) :
    values(_values)
  {
  }

  bool operator()(unsigned i1, unsigned i2)
  {
    return values[i1] < values[i2];
  }

};

//----------------------------------------------------------------------

RooAbsReal *makePiecewiseLinearFunction(const std::string &funcName, RooAbsReal &xvar, const std::vector<double> &xvalues,
    const std::vector<double> &yvalues, bool isSorted)
{
  ASSERTBREAK(xvalues.size() == yvalues.size());
  unsigned numPoints = xvalues.size();

  ASSERTBREAK(numPoints >= 2);

  std::vector<unsigned> indices;
  for (unsigned i = 0; i < numPoints; ++i)
  {
    indices.push_back(i);
  }

  if (! isSorted)
  {
    std::sort(indices.begin(), indices.end(), IndexedLess(xvalues));
  }

  std::vector<std::string> parts;

  // make the function zero outside the limits
  // (this is taken into account automatically
  // by only letting the function be non-zero
  // on the internal intervals)
  for (unsigned i = 0; i < numPoints - 1; ++i)
  {
    double xleft = xvalues[indices[i]];
    double xright = xvalues[indices[i + 1]];

    double yleft  = yvalues[indices[i]];
    double yright = yvalues[indices[i + 1]];

    ostringstream buf;
    double slope = (yright - yleft) / (xright - xleft);
    buf
      << "(@0 >= " << xleft << " && @0 < " << right << ")" // indicator function
      << " * "
      << "((@0 - " << xleft << ") * " << slope << " + " << yleft << ")";

    parts.push_back(buf.str());
  }

  string overallExpr;
  for (unsigned i = 0; i < parts.size(); ++i)
  {
    if (i > 0)
      overallExpr += " + ";

    overallExpr += parts[i];
  }


  RooArgList argList(xvar);

  return new RooFormulaVar(funcName.c_str(), "",overallExpr.c_str(), argList);

}

//----------------------------------------------------------------------
