#include "Fitter.h"
#include <RooFit.h>
#include <RooAbsPdf.h>
#include "../ReportMaker.h"

#include <boost/foreach.hpp>
#include <boost/format.hpp>

using namespace std;

//----------------------------------------------------------------------

Fitter::Fitter(ParametricSignalModelConfig &config_,
               DataSetList &_dataSets,
               int mh,
         const string& catname,
         const string& mergedSigProcName,
         RooRealVar* mnom,
         const std::vector<RooRealVar*> &fitVariables,

         FitFunction* rightpdf,
         FitFunction* wrongpdf,

         RooRealVar& fracright,

         // TODO: should we split this into right and wrong vertex ?
         SignalModelParameterInterpolator *interpolator_,

         AbstractSignalFitModel *signalFitModel,

         ReportMaker *reportMaker,

	 RooWorkspace *debuggingWorkspace

         ) :
         config(config_),
        interpolator(interpolator_),
        dataSets(_dataSets),
        fitStatusRightVertex(-1000),
        fitStatusWrongVertex(-1000)

{
    std::set < TString > simpleset;
    std::stringstream numstringstr;
    numstringstr << mh;
    TString numstring(numstringstr.str());
    TString vbfstring = numstring;
    //define aggregate weight, so far using xsec, pileup, pt-reweighting and efficiency scale factors
    //RooFormulaVar totweight("totweight","totweight","xsecweight*puweight(ngenvtx-1)*ptweight(genhpt,isgf)*effweight(iseb1,iseb2)",RooArgList(xsecweight,ngenvtx,genhpt,isgf,iseb1,iseb2));
    //          RooDataSet *mcsigdata = NULL;
    //          RooDataSet *mcsigwdata = NULL;
    //          RooDataSet *mcsigwrongdata = NULL;
    //          RooDataSet *mcsigwrongwdata = NULL;
    //--------------------
    // set higgs mass hypothesis and fit range
    mnom->setVal(mh);

    // the PDF to be fitted for the events with right vertex assignment
    RooDataSet* mcsigwdata = this->dataSets.getMergedWeightedDataSet("mcsigwdata" + boost::lexical_cast < string > (mh) + catname + "_" + mergedSigProcName);
    RooDataSet *mcsigwrongwdata = NULL;
    if (config.useRightWrongVertex)
      mcsigwrongwdata = dataSets.getMergedWeightedDataSet("mcsigwrongwdata" + boost::lexical_cast < string > (mh) + catname + "_" + mergedSigProcName);

    //----------------------------------------
    // IT'S HERE WHERE FITTING IS HAPPENING !!!
    // perform fits
    //----------------------------------------
    cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
    rightpdf->func->Print();
    rightpdf->func->Print("V");
    rightpdf->func->printCompactTree();
    cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
    cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
    // fit the (mass) distribution of events
    // with CORRECT vertex assignment
    RooFitResult* fitres = NULL, *fitreswrong = NULL;

    if (! config.skipFits)
    {
      signalFitModel->prepareParametersForFitting(mh, catname, true /* rightVertex */, mergedSigProcName, rightpdf->params);

      // print parameters for debugging
      cout << "----------------------------------------" << endl;
      cout << "fit parameters after prepareParametersForFitting mh/cat/rightVertex/proc = " << mh << "/" << catname << "/" << true << "/" << mergedSigProcName << endl;
      cout << "----------------------------------------" << endl;
      printParameters(rightpdf->params);
      cout << "----------------------------------------" << endl;

      if (debuggingWorkspace != NULL)
	debuggingWorkspace->import(*mcsigwdata);

      fitres = rightpdf->func->fitTo(*mcsigwdata,
          RooFit::Strategy(0),
          RooFit::Minimizer("Minuit2", ""),
          RooFit::Minos(kFALSE),
          RooFit::SumW2Error(kFALSE),
          RooFit::Save(kTRUE),
	  RooFit::NumCPU(config.numCPU));

      cout << "----------------------------------------" << endl;
      cout << "fit parameters after fit mh/cat/rightVertex/proc = " << mh << "/" << catname << "/" << true << "/" << mergedSigProcName << endl;
      cout << "----------------------------------------" << endl;
      printParameters(rightpdf->params);
      cout << "----------------------------------------" << endl;


      // only report failed fits

      if (fitres->status() != FIT_STATUS_OK)
      {        // try again with a different strategy
        fitres = rightpdf->func->fitTo(*mcsigwdata,
            RooFit::Strategy(1),
            RooFit::Minimizer("Minuit2", ""),
            RooFit::Minos(kFALSE),
            RooFit::SumW2Error(kFALSE),
            RooFit::Save(kTRUE),
            RooFit::NumCPU(config.numCPU));

        if (fitres->status() != FIT_STATUS_OK && reportMaker != NULL)
        {
          if (reportMaker != NULL)
            reportMaker->addFitStatus(string("/fitstatus") +
                "/" + toStr(mh) +
                "/" + catname +
                "/" + mergedSigProcName +
                "/rightVertex/2nd",
                fitres->status() == FIT_STATUS_OK? "ok" : "FAILED"
            );
        }
      }

      this->fitStatusRightVertex = fitres->status();

      // user defined post-processing of the fitted parameters
      signalFitModel->postProcessFittedParameters(catname, true /* rightVertex */, mergedSigProcName, rightpdf->params);

      //----------

      // fit the (mass) distribution of events
      // with WRONG vertex assignment

      if (config.useRightWrongVertex)
      {
        ASSERT(mcsigwrongwdata != NULL);

	signalFitModel->prepareParametersForFitting(mh, catname, false /* rightVertex */, mergedSigProcName, wrongpdf->params);

	if (debuggingWorkspace != NULL)
	  debuggingWorkspace->import(*mcsigwrongwdata);

        fitreswrong = wrongpdf->func->fitTo(*mcsigwrongwdata, 
					    RooFit::Strategy(1), 
					    RooFit::Minimizer("Minuit2", ""), 
					    RooFit::Minos(kFALSE), 
					    RooFit::SumW2Error(kFALSE), 
					    RooFit::Save(kTRUE), 
					    RooFit::NumCPU(config.numCPU)
					    );

        if (fitreswrong->status() != FIT_STATUS_OK)
        {          // try again

          fitreswrong = wrongpdf->func->fitTo(*mcsigwrongwdata, RooFit::Strategy(0), RooFit::Minimizer("Minuit2", ""), RooFit::Minos(kFALSE), RooFit::SumW2Error(kFALSE), RooFit::Save(kTRUE), RooFit::NumCPU(config.numCPU));

          if (fitreswrong->status() != FIT_STATUS_OK)
            if (reportMaker != NULL)
              reportMaker->addFitStatus(string("/fitstatus") +
                  "/" + toStr(mh) +
                  "/" + catname +
                  "/" + mergedSigProcName +
                  "/wrongVertex/2nd",
                                      fitreswrong->status() == FIT_STATUS_OK ? "ok" : "FAILED"
              );
        }

        this->fitStatusWrongVertex = fitreswrong->status();

	// user defined post-processing of the fitted parameters
	signalFitModel->postProcessFittedParameters(catname, false /* rightVertex */, mergedSigProcName, wrongpdf->params);

      } // if distinguish between right and wrong vertex
    }
    //----------------------------------------
    // dm2.removeRange();

    //----------------------------------------
    // the fraction of signal events with their vertex correctly assigned ?
    //double fright = mcsigwdata->sumEntries()/(mcsigwdata->sumEntries()+mcsigwrongwdata->sumEntries());
    double fractionRightNumerator;
    double fractionRightDenominator;

    // check whether there are any entries at all
    if (mcsigwdata->numEntries() + (config.useRightWrongVertex ? mcsigwrongwdata->numEntries() : 0) == 0)
    {
      // both right and wrong datasets are empty,
      // set the 'fraction of right vertices' to one to avoid problems
      // with NaNs at this and other mass points
      fractionRightNumerator   = 1;
      fractionRightDenominator = 1;
    }
    else
    {
      if (config.useRightWrongVertex)
      {
        // distinguishing between right and wrong vertices
        fractionRightNumerator   = mcsigwdata->sumEntries();
        fractionRightDenominator = mcsigwdata->sumEntries() + mcsigwrongwdata->sumEntries();
      }
      else
      {
        // NOT distinguishing between right and wrong vertices
        ASSERT(mcsigwrongwdata == NULL);
        fractionRightNumerator   = mcsigwdata->sumEntries();      
        fractionRightDenominator = fractionRightNumerator;
      }
    }

    cout << "mass=" << mh 
         << " cat=" << catname 
         << " mergedSigProc=" << mergedSigProcName
         << " frightden=" << fractionRightDenominator 
         << " frightnum=" << fractionRightNumerator << endl;
    // fraction of events with correctly assigned vertices
    assignRatio(fracright, fractionRightNumerator, fractionRightDenominator);

    //----------
    // correct negative resolution terms which screw up interpolation
    // (convert to absolute values)

    // TODO: fit parameter post processing
#if 0
    fitparms.at(3)->setVal(TMath::Abs(fitparms.at(3)->getVal()));
    fitparms.at(4)->setVal(TMath::Abs(fitparms.at(4)->getVal()));
    fitparms.at(5)->setVal(TMath::Abs(fitparms.at(5)->getVal()));
    fitparms.at(10)->setVal(TMath::Abs(fitparms.at(10)->getVal()));
    fitparms.at(11)->setVal(TMath::Abs(fitparms.at(11)->getVal()));
#endif
    //----------------------------------------
    //fill histograms with fit parameters in 5 (10 ?) GeV steps
    // (fill fitparmhists from fitparms)
    //----------------------------------------

    if (! config.skipFits)
    {
      fillParameterHistograms(catname, rightpdf, mh);

      if (config.useRightWrongVertex)
        // also fill the parameters for the wrong vertex
        fillParameterHistograms(catname, wrongpdf, mh);

    }

    if (config.useRightWrongVertex)
      // also add the fraction of 'right vertex events'
      interpolator->addValue(catname + "_" + "fracright", mh, fracright.getVal(), fracright.getError());

    // draw the mass distribution for the events with correctly assigned vertex
    if (reportMaker != NULL)
    {
      signalFitModel->drawFittedMass("rightvtx", mh, mergedSigProcName, 100, fitVariables, mcsigwdata, rightpdf, catname, reportMaker);

      if (config.useRightWrongVertex)
        // also draw the mass distribution comparison for the events with a wrongly assigned veretx
        signalFitModel->drawFittedMass("wrongvtx", mh, mergedSigProcName, 40, fitVariables, mcsigwrongwdata, wrongpdf, catname, reportMaker);
      }


    // commented out for the moment as mcsigwrongwdata may be NULL
    // printf ("right = %5f, wrong = %5f, all = %5f, right+wrong = %5f\n",mcsigwdata->sumEntries(),mcsigwrongwdata->sumEntries(), mcsigallwdata->sumEntries(),mcsigwdata->sumEntries()+mcsigwrongwdata->sumEntries());
    printf("data weights = %5e\n", mcsigwdata->sumEntries());

    // commented out for the moment as fitreswrong may be NULL
    // printf("mass = %i, status = %i, statuswrong = %i\n",mhs.at(i),fitres->status(),fitreswrong->status());
  }

//----------------------------------------------------------------------

void
Fitter::fit()
{
}
//----------------------------------------------------------------------

void
Fitter::fillParameterHistograms(const std::string &catname, FitFunction *pdf, int& mh)
{
  std::vector<RooRealVar *> fitparms = pdf->getParameters();

  for (unsigned iparm = 0; iparm < fitparms.size(); ++iparm)
  {
    // WATCH OUT NOT TO FILL THE SAME HISTOGRAM MORE THAN ONCE !

    RooRealVar *thisParam = fitparms.at(iparm);

    // old version
    // string shortParamName = thisParam->GetName();
    string shortParamName = pdf->getParameterShortName(thisParam);

    this->interpolator->addValue(catname + "_" + shortParamName, mh, *fitparms.at(iparm));

  } // loop over parameters
}

//----------------------------------------------------------------------
void 
Fitter::printParameters(const std::map<std::string, RooRealVar *> &fitParams) const
{
  for (map<string, RooRealVar *>::const_iterator it = fitParams.begin();
       it != fitParams.end();
       ++it)
  {
    RooRealVar *param = it->second;
    cout << "  " << boost::format("%-40s / %-40s: %f (%f..%f)") % it->first % param->GetName() % param->getVal() % param->getMin() % param->getMax() << endl;
  } // loop over all parameters
}


