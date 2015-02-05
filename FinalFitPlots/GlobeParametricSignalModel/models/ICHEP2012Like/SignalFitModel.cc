#include <iostream>

extern "C"
{
  // needed of Py_None
#include <Python.h>
}


#include "SignalFitModel.h"

#include <utils.h>

#include <boost/foreach.hpp>

#include <RooAddPdf.h>
#include <RooGaussian.h>

#include <TRandom.h>

#include <stdexcept>
#include <numeric>

#include <PythonConfigUtils.h>

#include <boost/assign/list_of.hpp>
using namespace boost::assign;


using namespace std;
using namespace ICHEP2012LikeModel;

//----------------------------------------------------------------------

SignalFitModel::SignalFitModel(const ParametricSignalModelConfig &_config, const Config &_modelConfig, const std::vector<std::string> &catnames_, RooRealVar *hmass, RooRealVar *mnom) :
  catnames(catnames_),
  config(_config),
  modelConfig(_modelConfig)

{
  // fill smearing uncertainties map
  
}

//----------------------------------------------------------------------

unsigned
SignalFitModel::getNumGaussians(const string &catname, bool rightVertex, const string &mergedSigProcName)
{
  // makes sure that the corresponding entry exists
  map<string, map<string, map<string, unsigned> > >::const_iterator it1 = modelConfig.numGaussians.find(catname);
  if (it1 != modelConfig.numGaussians.end())
  {
    // cout << "found cat=" << catname << endl;

    map<string, map<string, unsigned> >::const_iterator it2 = it1->second.find(rightVertex ? "right" : "wrong");
    if (it2 != it1->second.end())
    {
      // cout << "found right vertex=" << rightVertex << endl;

      map<string, unsigned>::const_iterator it3 = it2->second.find(mergedSigProcName);
      if (it3 != it2->second.end())
      {
        // cout << "found mergedSigProcName=" << mergedSigProcName << endl;
        return it3->second;
      }
    } // if process name found
  } // if category found

  throw std::runtime_error("number of Gaussians not found for cat " + catname + " proc " + mergedSigProcName + " " + toStr(rightVertex));
}

//----------------------------------------------------------------------

std::vector<std::string>
SignalFitModel::getFitFunctionParamNames(const std::string &catname, bool rightVertex, const std::string &mergedSigProcName)
{
  unsigned numGaussians = getNumGaussians(catname, rightVertex, mergedSigProcName);

  vector<string> paramNames;
  string paramPrefix;
  if (! rightVertex)
    paramPrefix = "w";

  for (unsigned i = 0; i < numGaussians; ++i)
  {
    paramNames.push_back(paramPrefix + "dm" + toStr(i+1));
    paramNames.push_back(paramPrefix + "sigma" + toStr(i+1));

    if (i < numGaussians - 1)
      paramNames.push_back(paramPrefix + "f" + toStr(i+1));
  }

  return paramNames;
}

//----------------------------------------------------------------------

RooAbsPdf*
SignalFitModel::makeFitFunction(RooRealVar *mnom, const std::vector<RooRealVar *>& fitVars, 
				const std::string &catname, bool rightVertex, const std::string &mergedSigProcName,
				const std::string &funcName, std::vector<RooAbsReal *> &params, const std::string &prefixForHiddenObjects)
{
  ASSERTBREAK(fitVars.size() == 1);
  RooRealVar *hmass = fitVars.at(0);
  unsigned numGaussians = getNumGaussians(catname, rightVertex, mergedSigProcName);

  unsigned nextParamIndex = 0;

  RooArgList gaussians;
  RooArgList fractions;

  for (unsigned i = 0; i < numGaussians; ++i)
  {
    // dm
    RooAbsReal *dm = params.at(nextParamIndex++);

    // produce a 'hidden' variable 'mean' based on dm
    RooFormulaVar *mean = new RooFormulaVar((prefixForHiddenObjects + "_" + "mean" + toStr(i+1)).c_str(), "",
        "@0+@1", RooArgList(*mnom, *dm));

    // sigma
    RooAbsReal *sigma = params.at(nextParamIndex++);

    RooGaussian *gaus = new RooGaussian((prefixForHiddenObjects + "_" + "g" + toStr(i+1)).c_str(),
                    ("Gaussian " + toStr(i+1)).c_str(),
                     *hmass, *mean, *sigma);

    gaussians.add(*gaus);

    // fraction
    if (i < numGaussians - 1)
      fractions.add(*params.at(nextParamIndex++));

  } // loop over Gaussians

  // make sure we got exactly the number of expected
  // parameters
  ASSERTBREAK(nextParamIndex == params.size());

  // build the overall pdf

  return new RooAddPdf(funcName.c_str(), "", gaussians, fractions, kTRUE);

}

//----------------------------------------------------------------------
#include <boost/algorithm/string/predicate.hpp>

// void
// SignalFitModel::prepareParameterForFitting(const std::string &paramName, RooRealVar *param)
// {
//   string pname = paramName;
// 
//   if (boost::starts_with(pname,"w"))
//     pname = pname.substr(1);
// 
//   cout << "ADJUSTING PARAMETER " << pname << endl;
// 
//   if (boost::starts_with(pname, "sigma"))
//   {
//     unsigned number = boost::lexical_cast<unsigned>(pname.substr(5));
// 
//     // original initialization parameters:
//     // sigma1: sigma1init, 0.5, 5.0  where sigma1init = 1.0
//     // sigma2: sigma2init, 0.8, 7.0  where sigma1init = 1.2
//     // sigma3: sigma3init, 1.0, 10.0  where sigma1init = 2.0
// 
//     // NOTE: need to break the symmetry between the parameters
//     //       of the different Gaussian components
//     param->setVal(1 + 0.5 * gRandom->Uniform() );
//     // param->setVal(number);
// 
//     param->setRange(0.01, 20);
//     // param->removeRange();
//     // param->setConstant(false);
//   }
//   else if (boost::starts_with(pname, "dm"))
//   {
//     unsigned number = boost::lexical_cast<unsigned>(pname.substr(2));
// 
//     // original initialization:
//     //    dm1: dm1init, -12.0, 5.0 where dm1init =  0.5
//     //    dm2: dm2init, -12.0, 5.0 where dm2init = -1.0
//     //    dm3: dm3init,  -9.0, 5.0 where dm3init = -2.0
// 
//     // NOTE: need to break the symmetry between the parameters
//     //       of the different Gaussian components
//     param->setVal(0 + gRandom->Uniform());
//     // param->setVal(number - 1);
// 
//     // param->setRange(-20,20);
//     param->setRange(-5,5);
// 
//     // param->setConstant(false);
// 
//     // dm1, dm2 had this, dm3 not
//     // dm[i+1]->removeRange();
//   }
//   else if (boost::starts_with(pname, "f"))
//   {
//     // TODO: how do we deal with the initial values of the different parameters ?
//     // initially, these were initialized as follows (at least for the case of three Gaussians):
//     //    f1:   f1init, 0.52, 1.0 (where f1init = 0.60)
//     //    f2:   f2init, 0.0, 1.0 (where f2init = 0.90)
//     param->setVal(0.5);
//     param->setRange(0,1);
// 
//     // param->setConstant(false);
// 
//   }
//   else
//   {
//     // unexpected parameter
//     ASSERTBREAK(0);
//   }
// }

//----------------------------------------------------------------------

void
SignalFitModel::prepareParametersForFitting(int mhyp, const std::string &catname, bool rightVertex, const std::string &mergedSigProcName,
					    const std::map<std::string, RooRealVar *> &fitParams) 
{
  for (map<string, RooRealVar *>::const_iterator it = fitParams.begin();
       it != fitParams.end();
       ++it)
  {
    string paramName = it->first;
    RooRealVar *param = it->second;

    if (boost::starts_with(paramName,"w"))
      paramName = paramName.substr(1);

    // cout << "ADJUSTING PARAMETER " << paramName << endl;

    if (boost::starts_with(paramName, "sigma") ||
        boost::starts_with(paramName, "dm")
        )
    {
      if (modelConfig.fitParameterSettings == NULL)
        // was not specified by the user
        continue;

      // note that it looks like we should set the values in the right
      // order... otherwise some of them will be 'rounded'
      // (e.g. setting the minimum value above the current value
      // will move the current value to the minimum etc. or setting
      // the maximum below the minimum will be ignored)

      // take the initial values as default values
      double initialValue = param->getVal();
      double minValue = param->getMin();
      double maxValue = param->getMax();

      PyObject *value;

      value = PythonConfigUtils::getNestedDictValue(modelConfig.fitParameterSettings, 
                                 list_of(catname)
                                 (rightVertex ? "right" : "wrong")
                                 (mergedSigProcName)
                                 (paramName)
                                 ("initial"));
      // see http://docs.python.org/c-api/none.html for testing for None

      if (value != NULL && value != Py_None) 
        initialValue = PythonConfigUtils::castNumberToFloat(value);

      //-----

      value = PythonConfigUtils::getNestedDictValue(modelConfig.fitParameterSettings, 
                                 list_of(catname)
                                 (rightVertex ? "right" : "wrong")
                                 (mergedSigProcName)
                                 (paramName)
                                 ("min"));
      // see http://docs.python.org/c-api/none.html for testing for None

      if (value != NULL && value != Py_None) 
        minValue = PythonConfigUtils::castNumberToFloat(value);

      //-----

      value = PythonConfigUtils::getNestedDictValue(modelConfig.fitParameterSettings, 
                                 list_of(catname)
                                 (rightVertex ? "right" : "wrong")
                                 (mergedSigProcName)
                                 (paramName)
                                 ("max"));
      // see http://docs.python.org/c-api/none.html for testing for None
      if (value != NULL && value != Py_None) 
        maxValue = PythonConfigUtils::castNumberToFloat(value);

      // now update the settings
      // first set the range
      param->setRange(minValue, maxValue);
      
      // then the initial value
      param->setVal(initialValue);

  }
  


//    if (boost::starts_with(paramName, "sigma"))
//    {
//      unsigned number = boost::lexical_cast<unsigned>(paramName.substr(5));
//      
//      // original initialization parameters:
//      // sigma1: sigma1init, 0.5, 5.0  where sigma1init = 1.0
//      // sigma2: sigma2init, 0.8, 7.0  where sigma1init = 1.2
//      // sigma3: sigma3init, 1.0, 10.0  where sigma1init = 2.0
//      
//      // NOTE: need to break the symmetry between the parameters
//      //       of the different Gaussian components
//      // param->setVal(1 + 0.5 * gRandom->Uniform() );
//
//      param->setVal(4 + number);
//      
//      param->setRange(0.01, 20);
//
//      // param->removeRange();
//      // param->setConstant(false);
//    }
//    else if (boost::starts_with(paramName, "dm"))
//    {
//      // make this signed because otherwise, when we multiply 
//      // by -1, we'll get 0xffffffff....
//      int number = boost::lexical_cast<unsigned>(paramName.substr(2));
//      
//      // original initialization:
//      //    dm1: dm1init, -12.0, 5.0 where dm1init =  0.5
//      //    dm2: dm2init, -12.0, 5.0 where dm2init = -1.0
//      //    dm3: dm3init,  -9.0, 5.0 where dm3init = -2.0
//      
//      // NOTE: need to break the symmetry between the parameters
//      //       of the different Gaussian components
//      // param->setVal(0 + gRandom->Uniform());
//
//      ASSERTBREAK(number >= 1);
//
//      --number;
//      int sign;
//      if (number % 2 == 0)
//	sign = -1;
//      else
//	sign = 1;
//
//      param->setVal(((number + 1)/ 2) * sign);
//      
//      // param->setRange(-20,20);
//      param->setRange(-10,10);
//      
//      // param->setConstant(false);
//      
//      // dm1, dm2 had this, dm3 not
//      // dm[i+1]->removeRange();
//    }
    else if (boost::starts_with(paramName, "f"))
    {
      // TODO: how do we deal with the initial values of the different parameters ?
      // initially, these were initialized as follows (at least for the case of three Gaussians):
      //    f1:   f1init, 0.52, 1.0 (where f1init = 0.60)
      //    f2:   f2init, 0.0, 1.0 (where f2init = 0.90)
      param->setVal(0.5);
      param->setRange(0,1);



    }
    else
    {
      // unexpected parameter
      cerr << "UNEXPECTED PARAMETER " << paramName << endl;
      ASSERTBREAK(0);
    }
  } // loop over parameters
}


//----------------------------------------------------------------------
#include <TCanvas.h>
#include <RooPlot.h>
#include <TLegend.h>

void
SignalFitModel::drawFittedMass(const string &plotNamePart,
               int mh,
               const string &mergedSigProcName,
               int bins,
               const std::vector<RooRealVar *> &fitVariables,

               // the corresponding dataset we fitted to
               RooDataSet *mcsigwdata,
               FitFunction *rightpdf,
               const std::string &catname,
               ReportMaker *reportMaker)
{
  RooRealVar *hmass = fitVariables.at(0);

  hmass->setRange("plotrange", 
                  // TMath::Max(100.0, mh - 30.0),         // start
                  // TMath::Min(config.massmax, mh + 20.0) // end

                  mh - 30.0,
                  mh + 30.0
                  );


  //plot fit results for this mass point
  TCanvas *chfit = new TCanvas;
  string plotname = "mass_";
  plotname += plotNamePart;
  plotname += "_";

  plotname += "m" + toStr(mh) + "_";
  // plotname += catname + "_" + mergedSigProcName + ".png";
  plotname += catname + "_" + mergedSigProcName;

  RooPlot *hplot = hmass->frame(RooFit::Bins(bins), RooFit::Range("plotrange"));

  mcsigwdata->plotOn(hplot);

  BOOST_FOREACH(RooAbsPdf *component, rightpdf->componentsToDraw)
  {
  // rightpdf->plotOn(hplot, Components(*g1), LineColor(kOrange), Range("higgsrange"), NormRange("higgsrange"));
  // rightpdf->plotOn(hplot, Components(*g2), LineColor(kMagenta), Range("higgsrange"), NormRange("higgsrange"));
    rightpdf->func->plotOn(hplot, RooFit::Components(*component),
                                  RooFit::Range("higgsrange"),
                                  RooFit::NormRange("higgsrange"));

  }
  // if (g3 != NULL)
  //   rightpdf->plotOn(hplot, Components(*g3), LineColor(kRed), Range("higgsrange"), NormRange("higgsrange"));


  // plot the overall PDF itself
  rightpdf->func->plotOn(hplot, RooFit::LineColor(kBlue), RooFit::Range("higgsrange"), RooFit::NormRange("higgsrange"));
  hplot->SetTitle(("mass fit comparison " + plotNamePart + " " + catname + " " + mergedSigProcName).c_str());
  hplot->Draw();

  // add a legend (see e.g. http://root.cern.ch/phpBB3/viewtopic.php?p=31694 and
  // the attached code http://root.cern.ch/phpBB3/download/file.php?id=6019 )
  //    for (int i=0; i<hplot->numItems(); i++)
  //    {
  //      TString obj_name = hplot->nameOf(i);
  //      if (obj_name=="")
  //        continue;
  //      cout << Form("ITEM: %d. '%s'\n",i,obj_name.Data());
  //    }

  // the items are of the form (example):
  // combh_Norm[CMS_hgg_mass]_Comp[g1]_Range[higgsrange]_NormRange[higgsrange]
  // or they seem to be items 1..3

  TLegend *leg = new TLegend(0.15, 0.5, 0.3, 0.75);

  for (unsigned i = 0; i < rightpdf->componentsToDraw.size(); ++i)
  {
    leg->AddEntry(hplot->getObject(i+1), rightpdf->componentsToDraw[i]->GetTitle(), "l");
  }

  leg->Draw();

  chfit->SaveAs(Form("plots/%s.svg",plotname.c_str()));
  chfit->SaveAs(Form("plots/%s.png",plotname.c_str()));

  if (reportMaker != NULL)
    reportMaker->addPlotSVG("mass/" + plotNamePart + "/" + "m" + toStr(mh) + "/" + catname + "/" + mergedSigProcName,
        plotname, "mass distribution with fit");

}

//----------------------------------------------------------------------

void 
SignalFitModel::postProcessFittedParameters(const string &catname, bool rightVertex, const string &mergedSigProcName,
					    const std::map<std::string, RooRealVar *> &fittedParams)
{
  unsigned numGaussians = getNumGaussians(catname, rightVertex, mergedSigProcName);

  string prefix;
  if (! rightVertex)
    prefix = "w";

  // get dms and sigmas
  vector<double> means, sigmas;
  vector<unsigned> indices;
  vector<double> absoluteCoefficients;

  vector<RooRealVar *> muParams, sigmaParams, fParams;

  // sort the parameters e.g. by increasing order of the sigmas
  // in order to have a smooth evolution as function
  // of the Higgs mass hypothesis 
  //
  // IMPORTANT: also the recursive fractions must be 
  // recalculated and re-assigned
  // 
  // it's easiest to first calculate the overall
  // coefficient for each pdf component, then reorder
  // and calculate the new recursive fraction
  // values from the new absolute factors

  double overallFactor = 1;

  for (int i = 1; i <= (int)numGaussians; ++i)
  {
    indices.push_back(means.size());

    // get the delta mean
    string name = prefix + "dm" + toStr(i);
    ASSERTBREAK(fittedParams.count(name) > 0);
    muParams.push_back(fittedParams.find(name)->second);
    means.push_back(muParams.back()->getVal());

    name = prefix + "sigma" + toStr(i);
    ASSERTBREAK(fittedParams.count(name) > 0);
    sigmaParams.push_back(fittedParams.find(name)->second);
    sigmas.push_back(sigmaParams.back()->getVal());

    if (i < numGaussians)
    {
      name = prefix + "f" + toStr(i);
      ASSERTBREAK(fittedParams.count(name) > 0);
      fParams.push_back(fittedParams.find(name)->second);
      double thisF = fParams.back()->getVal();

      absoluteCoefficients.push_back(overallFactor * thisF);
      overallFactor *= (1 - thisF);
    }
    else
      // the last coefficient
      absoluteCoefficients.push_back(overallFactor);
  }

  // cross check: sum of absolute coefficients must be close to one
  {
    for (unsigned i = 0; i < absoluteCoefficients.size(); ++i)
      cout << "UUU " << absoluteCoefficients[i] << endl;
    // note that the . after the zero is important, otherwise one gets an integer sum
    // which is only converted to a float after summing (and the result is zero...)
    double sum = std::accumulate(absoluteCoefficients.begin(), absoluteCoefficients.end(), 0.);
    ASSERTBREAK(fabs(sum - 1) < 1e5);
    // cout << "sum=" << sum << endl;
    // exit(1);
  }

  // sort the indices in increasing order of sigmas
  // sort(indices.begin(), indices.end(), ValueComparator(sigmas));

  // sort the indices in increasing order of means
  sort(indices.begin(), indices.end(), ValueComparator(means));

  // re-assign sigmas and mus to the parameters
  overallFactor = 1;
  for (int i = 0; i < muParams.size(); ++i)
  {
    // TODO: should we also copy other things like
    //       the range ?!
    unsigned ind = indices[i];
    muParams[i]->setVal(means[ind]);
    sigmaParams[i]->setVal(sigmas[ind]);

    // recalculate the recursive fractions from the
    // (reordered) absolute fractions
    if (i + 1 < muParams.size())
    {
      // TODO: can we actually reorder the PDFs in
      //       any order from the point of view
      //       of the recursive fraction ?
      double absCoeff = absoluteCoefficients[ind];
      double newF = absCoeff / overallFactor;
      ASSERTBREAK(newF <= 1);
      fParams[i]->setVal(newF);

      overallFactor *= (1 - newF);
    }
  }
}

//----------------------------------------------------------------------
