#include "FittingStep.h"
#include <string>

#include <boost/foreach.hpp>

#include "AbstractSignalFitModel.h"


#include "Fitter.h"

#include "../OutputRedirector.h"

#include <TCanvas.h>

#ifndef __CINT__
#include <boost/assign/list_of.hpp>
using namespace boost::assign;
#endif

using namespace std;

//----------------------------------------------------------------------

FittingStep::FittingStep(ParametricSignalModelConfig &_config,
    DataSetList &_dataSets,
    const std::vector<std::string> &_catnames,
    const std::vector<std::string> &_mergedSigProcessNames,
    const std::vector<int> &_mhs,
    AbstractSignalFitModel *_signalFitModel, RooRealVar *_mnom,
    const std::vector<RooRealVar *> &_fitVariables,
    SignalModelParameterInterpolatorMap &_fitParamInterpolator,
    ReportMaker *_reportMaker) :

    config(_config),
    catnames(_catnames),
    mergedSigProcessNames(_mergedSigProcessNames),
    mhs(_mhs),
    signalFitModel(_signalFitModel),
    mnom(_mnom),
    fitVariables(_fitVariables),
    fitParamInterpolator(_fitParamInterpolator),
    dataSets(_dataSets),
    reportMaker(_reportMaker)
{
  initialize();
}

//----------------------------------------------------------------------

void FittingStep::initialize()
{
  // figure out which analytic forms we use for the fitting functions
  // this->signalFitModel->assignFittingFunctions(hmass, mnom, fitFunctionRightVertex, fitFunctionWrongVertex);

  //----------
  // collect the superset of all parameters per category
  //----------
  for (unsigned icat = 0; icat < catnames.size(); ++icat)
  {
    string catname = catnames.at(icat);
//    std::set<FitFunction *> allFitFunctions;
//
//    // ASSERT(this->fitFunctionRightVertex.count(icat) > 0);
//    // ASSERT(this->fitFunctionWrongVertex.count(icat) > 0);
//
//    // find all distinct interpolation functions for this category
//    allFitFunctions.insert(this->signalFitModel->getFitFunction(icat, true));
//    allFitFunctions.insert(this->signalFitModel->getFitFunction(icat, false));
//
//    // now create the list of all parameters to be interpolated
//    std::list<string> allParamNames;
//    BOOST_FOREACH(FitFunction *func, allFitFunctions)
//    {
//      // TODO: we should crosscheck that no parameter name
//      //       appears more than once (i.e. within a function
//      //       or across distinct functions)
//      list<string> tmp = func->getParamNames();
//      std::copy(tmp.begin(), tmp.end(), back_inserter(allParamNames));
//    }

    BOOST_FOREACH(string mergedSigProcName, mergedSigProcessNames)
    {
      // get the names of all parameters of the fitting function
      // TODO: check whether we should use the input or merged signal process name
      //       here

      // right vertex
      std::vector<string> allParamNames = signalFitModel->getFitFunctionParamNames(catname, true, mergedSigProcName);

      // add parameter for fraction of correct vertex assignment
      if (config.useRightWrongVertex)
      {
          // add wrong vertex
          std::vector<string> tmp = signalFitModel->getFitFunctionParamNames(catname, false, mergedSigProcName);

          std::copy(tmp.begin(), tmp.end(), back_inserter(allParamNames));

          // add the parameter for the fraction of right vertex events
          allParamNames.push_back("fracright");
      }

      //----------

      list<string> tmp;
      BOOST_FOREACH(string buf, allParamNames)
      {
	// TODO: should we also add the sqrts string here ?
	// (although this is just a parameter for fitting,
	// does not need to go to the final workspace 
	// in the output file ?!
	  tmp.insert(tmp.end(),catname + "_" + buf);
      }

      fitParamInterpolator.add(
          catname,
          mergedSigProcName,
          new SignalModelParameterInterpolator(config, tmp, mnom,

              catname + "_" + mergedSigProcName // prefix
              ));

    } // loop over signal processes

  } // loop over categories
}
//----------------------------------------------------------------------

FitFunction*
FittingStep::makeFittingFunction(const std::string &catname, bool rightVertex, const std::string &mergedSigProcName)
{
  std::string vertexName;
  if (rightVertex)
    vertexName = "rightVertex";
  else
    vertexName = "wrongVertex";

  std::vector<RooAbsReal *> paramsRightVertex;

  std::map<std::string, RooRealVar *> paramMap;

  std::vector<std::string> rightVertexParamNames = this->signalFitModel->getFitFunctionParamNames(catname, rightVertex,
      mergedSigProcName);
  BOOST_FOREACH(string paramName, rightVertexParamNames)
  {
    //string prefix = "fitParam_" + catname + "_" + vertexName + "_";

    // assume that the parameters for right and wrong vertex
    // already are named differently
    // string prefix = catname + "_";

    string prefix = "fitParam_" + catname + "_" + vertexName + "_" + mergedSigProcName + "_";

    // cout << "PARAM NAME " << paramName << endl;

    RooRealVar *param = new RooRealVar((prefix + paramName).c_str(), ("fitting parameter " + paramName).c_str(), -1000,1000);
    paramsRightVertex.push_back(param);

    paramMap[paramName] = param;
  }

  // note that we use the MERGED signal process name here as part
  // of the function name which is used for fitting only.
  //
  // See the other call to makeFitFunction(..) in fitSignal.cc
  // for the 'production' (final) function where the input signal
  // process name is used (but no fitting is done)
  RooAbsPdf *fitFuncRightPdf = this->signalFitModel->makeFitFunction(mnom, fitVariables,
      catname, rightVertex, mergedSigProcName,
      "fitFunc_" + catname + "_" + vertexName + "_" + mergedSigProcName, // function name
      paramsRightVertex,
      "fitFunc_" + catname + "_" + vertexName + "_" + mergedSigProcName + "_" /* prefixForHiddenObjects */
  );

  // TODO: must handle this
  const std::vector<RooAbsPdf *> componentsToDraw;

  FitFunction *rightpdf =
      new FitFunction(fitFuncRightPdf,
                      paramMap,
                      componentsToDraw);

  return rightpdf;
}

//----------------------------------------------------------------------

void
FittingStep::run(RooWorkspace *debugWorkspace)
{
  // fraction of signal events assigned to the
  // correct vertex (seems to be fitted later on ?)
  const double fracrightinit = 0.9;
  RooRealVar fracright("fracright", "fracright", fracrightinit, 0.0, 1.0);

  //--------------------

  // loop over categories for fits
  BOOST_FOREACH(string catname, catnames)
  {

    // loop over all signal processes
    BOOST_FOREACH(string mergedSigProcName, mergedSigProcessNames)
    {

      //--------------------
      // produce a fit function for this category
      // (initialize the parameters later for each
      // mass point)
      //--------------------

      // create a set of RooRealVar parameters for fitting
      FitFunction* rightpdf, *wrongpdf = NULL;

      rightpdf = makeFittingFunction(catname, true, mergedSigProcName);

      rightpdf->func->Print("V");
      
      if (config.useRightWrongVertex)
	wrongpdf = makeFittingFunction(catname, false, mergedSigProcName);

      //--------------------

      // loop over mass points for fits
      for (UInt_t i = 0; i < mhs.size(); ++i)
      {
        int mh = mhs.at(i);

        std::cout << "----------------------------------------" << endl;
        std::cout << "fitting mh=" << mh << " mergedSigProcName=" << mergedSigProcName << " cat=" << catname << endl;
        std::cout << "----------------------------------------" << endl;

        // redirects cout / cerr to a buffer
        // OutputRedirector outRedir;

        //----------
        // get the initial values for the fit parameters
        //----------
        vector<FitFunction *> pdfs;
        pdfs.push_back(rightpdf);

        if (wrongpdf != NULL)
          pdfs.push_back(wrongpdf);

//        BOOST_FOREACH(FitFunction *pdf, pdfs)
//        {
//          for (std::map<std::string, RooRealVar *>::const_iterator it = pdf->params.begin();
//               it != pdf->params.end();
//               ++it)
//          {
//            // note that we are NOT using param->GetName() here
//            // as this might have a name with the category inside
//            // etc.
//            string name = it->first;
//
//            this->signalFitModel->prepareParameterForFitting(name, it->second);
//          }
//        }

        // perform the fit to the signal mass distribution
        Fitter singlePointFitter(config, dataSets, mh, catname,
            mergedSigProcName, mnom, 
            fitVariables,             /* e.g. reconstructed mh */
            rightpdf, wrongpdf,
            fracright,
            fitParamInterpolator.get(catname,mergedSigProcName),
            signalFitModel,
            reportMaker,
            debugWorkspace				 
        );

        singlePointFitter.fit();

        std::cout << "----------------------------------------" << endl;
        std::cout << "done fitting mh=" << mh << " mergedSigProcName=" << mergedSigProcName
            << " cat=" << catname
            << " status right vertex=" << (singlePointFitter.fitStatusRightVertex == FIT_STATUS_OK ? "ok" : "FAILED");
            ;

        if (config.useRightWrongVertex)
          std::cout << " status wrong vertex=" << (singlePointFitter.fitStatusWrongVertex == FIT_STATUS_OK ? "ok" : "FAILED");

        std::cout << endl;
        std::cout << "----------------------------------------" << endl;

        //-----
        if (singlePointFitter.fitStatusRightVertex != FIT_STATUS_OK)
          failedFitsDescriptions.push_back("mh=" + toStr(mh) + " mergedSigProcName=" + mergedSigProcName + " cat=" + catname + " right vertex: status=" + toStr(singlePointFitter.fitStatusRightVertex));

        if (config.useRightWrongVertex && singlePointFitter.fitStatusWrongVertex != FIT_STATUS_OK)
          failedFitsDescriptions.push_back("mh=" + toStr(mh) + " mergedSigProcName=" + mergedSigProcName + " cat=" + catname + " wrong vertex: status=" + toStr(singlePointFitter.fitStatusWrongVertex));

	if (debugWorkspace != NULL)
        {
	  // import into debugging workspace (e.g. for later re-fitting by hand
	  // to check fit behaviour)
	  debugWorkspace->import(*(rightpdf->func), RooFit::RecycleConflictNodes());

	  if (wrongpdf != NULL)
	    debugWorkspace->import(*(wrongpdf->func), RooFit::RecycleConflictNodes());
        }


      } // loop over merged signal Feynman diagrams

    } // loop over Higgs masses

  } // loop over categories

}
//----------------------------------------------------------------------
bool
FittingStep::printFailedFits(std::ostream &os)
{
  if (this->failedFitsDescriptions.empty())
    return false;

  os << "----------------------------------------" << endl;
  os << "failed fits:" << endl;
  os << "----------------------------------------" << endl;

  BOOST_FOREACH(string msg, failedFitsDescriptions)
  {
    os << "  " << msg << endl;
  }

  return true;
}
