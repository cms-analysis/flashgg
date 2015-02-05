#ifndef _FitFunction_h
#define _FitFunction_h


#include <RooRealVar.h>
#include <vector>

/** a combination of a RooAbsPdf to be fitted and the parameters to be investigated.
 *
 *
 * */
class FitFunction
{
public:
  /** the PDF to be fitted */
  RooAbsPdf *func;

  /** the parameters to be fitted. Maps from short name (which should always be the same, no
   *  matter what the ROOT name of the parameter is) to the parameter */
  std::map<std::string, RooRealVar *> params;

  /** references to components if they should be drawn independently in the mass plots.
   *  TODO: we probably also want to be able to define the colors of the components ?
   * */
  std::vector<RooAbsPdf *> componentsToDraw;

  FitFunction(RooAbsPdf *func_,
              const std::map<std::string, RooRealVar *> &params_,
              const std::vector<RooAbsPdf *> &componentsToDraw_);

  /** convenience method to return a list of all fit parameter names.
   *  Note that these are the FULL names, not the 'simple' names
   *  */
  std::list<std::string> getParamNames();

  /** @return just the parameter objects, without the names */
  std::vector<RooRealVar *> getParameters();

  /** @return the short name of the given parameter param. Compares by pointer. */
  std::string getParameterShortName(RooRealVar *param);

};

#endif
