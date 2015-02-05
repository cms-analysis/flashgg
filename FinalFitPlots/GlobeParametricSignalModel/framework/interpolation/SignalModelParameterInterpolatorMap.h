#ifndef _SignalModelParameterInterpolatorMap_h
#define _SignalModelParameterInterpolatorMap_h

#include "SignalModelParameterInterpolator.h"

class ReportMaker;

/** class keeping track of all SignalModelParameterInterpolators.
 *  (Each SignalModelParameterInterpolator interpolates
 *   the set of parameters needed for one category / signal process,
 *   while this class keeps track of these, i.e. of the overall
 *   interpolation)
 */
class SignalModelParameterInterpolatorMap
{
protected:
  /** first index is the category name, second index
      is the name of the production mechanism */
  std::map<std::string, std::map<std::string, SignalModelParameterInterpolator *> > fitParamInterpolator;

public:

  void add(const std::string &catname, const std::string &mergedSigProcName, SignalModelParameterInterpolator *interpolator);

  /** TODO: in some cases (effacc ?), we don't use the merged but the input signal
      process name ? */
  SignalModelParameterInterpolator *get(const std::string &catname, const std::string &mergedSigProcName) const;

  void plotAll(ReportMaker *reportMaker);

};

#endif
