#include "SignalModelParameterInterpolatorMap.h"

#include "../utils.h"

#include <boost/foreach.hpp>

using namespace std;

//----------------------------------------------------------------------

void SignalModelParameterInterpolatorMap::add(const std::string &catname, const std::string &mergedSigProcName,
    SignalModelParameterInterpolator *interpolator)
{
  // TODO: should we check that we're not setting the same twice ?
  fitParamInterpolator[catname][mergedSigProcName] = interpolator;
}

//----------------------------------------------------------------------
SignalModelParameterInterpolator *
SignalModelParameterInterpolatorMap::get(const std::string &catname, const std::string &mergedSigProcName) const
{
  // sanity check: interpolator must be known
  std::map<std::string, std::map<string, SignalModelParameterInterpolator *> >::const_iterator it = fitParamInterpolator.find(catname);
  ASSERTBREAK(it != fitParamInterpolator.end());

  std::map<string, SignalModelParameterInterpolator *>::const_iterator it2 = it->second.find(mergedSigProcName);
  ASSERTBREAK(it2 != it->second.end());

  return it2->second;
}
//----------------------------------------------------------------------

void
SignalModelParameterInterpolatorMap::plotAll(ReportMaker *reportMaker)
{
  for (std::map<string, map<string, SignalModelParameterInterpolator *> >::iterator it = this->fitParamInterpolator.begin();
      it != this->fitParamInterpolator.end();
      ++it)
  {
    // BOOST_FOREACH(string sigProcName, mergedSigProcessNames)
    for (std::map<string, SignalModelParameterInterpolator *>::iterator it2 = it->second.begin();
        it2 != it->second.end();
        ++it2)
    {
      string sigProcName = it2->first;
      it2->second->makeAllPlots(reportMaker);
    } // loop over signal Feynman diagrams

  } // loop over categories

}
//----------------------------------------------------------------------

