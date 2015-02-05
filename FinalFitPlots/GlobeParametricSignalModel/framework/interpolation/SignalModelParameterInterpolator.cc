#include "SignalModelParameterInterpolator.h"
#include <TCanvas.h>
#include <RooPlot.h>

#include <boost/foreach.hpp>

#include <algorithm>
#include "../utils.h"

#include "../ReportMaker.h"

using namespace std;

//----------------------------------------------------------------------

SignalModelParameterInterpolator::SignalModelParameterInterpolator(ParametricSignalModelConfig &config_, 
                                                                   const std::list<std::string> &paramNames_,
                                                                   RooRealVar *mnom_, 
                                                                   const std::string &suffix_) :
    config(config_),
    paramNames(paramNames_.begin(), paramNames_.end()),
    mnom(mnom_),
    isFrozen(false),
    suffix(suffix_)
{
  for (unsigned i = 0; i < paramNames.size(); ++i)
  {
    string histname = "hist_" + paramNames[i] + "_" + suffix;

    fitParamHists[paramNames[i]] = makeMassFunctionHist(histname.c_str());

  } // loop over fit parameters



}
//----------------------------------------------------------------------

bool
SignalModelParameterInterpolator::hasValueEntry(const std::string &paramName, int mh)
{
  map<string, map<unsigned, ValueEntry> >::const_iterator it = this->values.find(paramName);
  if (it == values.end())
    return false;

  return it->second.find(mh) != it->second.end();
}

//----------------------------------------------------------------------
void 
SignalModelParameterInterpolator::makePlot(const std::string &paramName, ReportMaker *reportMaker)
{
  string plotname = "func_" + paramName + "_" + suffix;

  TCanvas *cfunctest = new TCanvas;

  RooPlot *hploteffacc = mnom->frame(RooFit::Bins(90), RooFit::Range(105, 145));

  // workaround to avoid crash: disabled drawing of errors

  getFunction(paramName)->plotOn(hploteffacc, RooFit::LineColor(kBlue) /* , RooFit::DataError(RooAbsData::None) */);
  hploteffacc->SetTitle("");
  hploteffacc->GetYaxis()->SetTitle((paramName + "_" + suffix).c_str());
  hploteffacc->Draw();
  cfunctest->SaveAs(Form("plots/%s.svg",plotname.c_str()));
  cfunctest->SaveAs(Form("plots/%s.png",plotname.c_str()));
  delete cfunctest;

  // add the plot to the ReportMaker
  if (reportMaker != NULL)
  {
    reportMaker->addPlotSVG("func/" + paramName + "/" + suffix, plotname,"DESCRIPTION TO BE DETERMINED");
  }

}

//----------------------------------------------------------------------

void
SignalModelParameterInterpolator::makeAllPlots(ReportMaker *reportMaker)
{
  BOOST_FOREACH(string paramName, paramNames)
  {
    makePlot(paramName, reportMaker);
  }
}
//----------------------------------------------------------------------

void
SignalModelParameterInterpolator::addValue(const std::string &paramName, int mh, double value, double error)
{
  ensureHasParameter(paramName);

  assert(! isFrozen);

  // check that this is the first time we get a value for this pair
  // or (parameter name, mass point)
  assert(! hasValueEntry(paramName, mh));

  this->values[paramName][mh].value = value;
  this->values[paramName][mh].error = error;

  // also fill this into the histogram if this is at a
  // grid mass point
  if ((mh % ((int) (config.massInterpolationWidth + 0.5))) == 0)
  {
    TH1F *histo = fitParamHists[paramName];
    histo->Fill(mh, value);
    histo->SetBinError(histo->FindFixBin(mh), error);
  }

}
//----------------------------------------------------------------------

void
SignalModelParameterInterpolator::addValue(const std::string &paramName, int mh, const RooRealVar &value)
{
  double error;

  if (value.hasAsymError())
  {
    // calculate the average error from the asymmetric error bars
    error = (value.getErrorLo() + value.getErrorHi()) / 2.0;
  }
  else
    error = value.getError();

  this->addValue(paramName, mh, value.getVal(), error);

}
//----------------------------------------------------------------------

RooDataHist *
SignalModelParameterInterpolator::getHistogram(const std::string &paramName)
{
  std::map<std::string, RooDataHist *>::const_iterator it = fitparmdatas.find(paramName);
  if (it != fitparmdatas.end())
    return it->second;

  // must create the histogram
  isFrozen = true;

  // create a RooDataHist based on the TH1
  string dataname = "data_" + paramName + "_" + suffix;
  fitparmdatas[paramName] = new RooDataHist(dataname.c_str(), dataname.c_str(), RooArgList(*mnom), fitParamHists[paramName]);

  return fitparmdatas[paramName];
}


//----------------------------------------------------------------------

RooHistFunc *
SignalModelParameterInterpolator::getFunction(const std::string &paramName)
{
  std::map<std::string, RooHistFunc *>::const_iterator it = fitparmfuncs.find(paramName);
  if (it != fitparmfuncs.end())
    return it->second;

  // check that this parameter actually exists
  ensureHasParameter(paramName);

  // must create the function, based on the histogram
  isFrozen = true;

  string funcname = "func_" + paramName + "_" + suffix;
  fitparmfuncs[paramName] = new RooHistFunc(funcname.c_str(), funcname.c_str(), RooArgList(*mnom), *getHistogram(paramName), 1);

  return fitparmfuncs[paramName];
}

//----------------------------------------------------------------------

bool
SignalModelParameterInterpolator::hasParameter(const std::string &paramName)
{
  return std::find(paramNames.begin(), paramNames.end(), paramName) != paramNames.end();
}

//----------------------------------------------------------------------

void
SignalModelParameterInterpolator::ensureHasParameter(const std::string &paramName)
{
  if (hasParameter(paramName))
    return;

  std::cerr << "trying to use non-existing parameter '" << paramName << "'"
            << endl;
  std::cerr << "  known parameter names are:";
  BOOST_FOREACH(string p, this->paramNames)
  {
    std::cerr << ' ' << p;
  }

  std::cerr << std::endl;

  ASSERTBREAK(1 == 0);
}


//----------------------------------------------------------------------

RooHistFunc *
SignalModelParameterInterpolator::getFunction(unsigned paramIndex)
{
  return getFunction(paramNames.at(paramIndex));
}

//----------------------------------------------------------------------

TH1F *
SignalModelParameterInterpolator::makeMassFunctionHist(const std::string &name)
{
  // earlier, 100 was used instead of config.massmax
  //
  // before, the lower edge was shifted by half a bin width to the RIGHT
  // (not to the left) and similarly for the upper edge

  // make a histogram such that the first bin's center is at the
  // lowest value and the last bin's center at the highest value

  // number of bins                              add half a bin on the left and on the right                      rouding
  int nbins = (config.massmax - config.massmin + config.massInterpolationWidth) / config.massInterpolationWidth + 0.5;

  // lower edge
  double xlow = config.massmin - config.massInterpolationWidth / 2.0;
  
  // upper edge
  double xhigh = config.massmax + config.massInterpolationWidth / 2.0;

  return new TH1F(name.c_str(), name.c_str(), nbins, xlow, xhigh);
}


//----------------------------------------------------------------------
