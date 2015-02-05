#include <sstream>
#include <iostream>
#include "TMath.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TTree.h"
#include "TNtuple.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooExponential.h"
#include "RooLandau.h"
#include "RooPlot.h"
#include "RooFit.h"
#include "RooAddPdf.h"
#include "RooGaussian.h"
#include "RooCBShape.h"
#include "RooFFTConvPdf.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooHistFunc.h"
#include "RooMoment.h"
#include "RooFitResult.h"
#include "RooExtendPdf.h"
#include "RooGenericPdf.h"
#include "RooBreitWigner.h"
#include "RooBifurGauss.h"
#include "RooProdPdf.h"
#include "RooCategory.h"
#include "RooSimultaneous.h"
#include "RooWorkspace.h"
#include "RooConstVar.h"
#include "TEfficiency.h"
#include "RooConstVar.h"


#include <dlfcn.h>

#include "ReportMaker.h"

#ifndef __CINT__
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/format.hpp>
using namespace boost::assign;
#endif

// #include "XsectDataOrig.C"


using namespace RooFit;
#include "utils.h"
#include "ParametricSignalModelConfig.h"

#include "fitting/FittingStep.h"
//----------------------------------------------------------------------
#include "AbstractSignalModel.h"

#include "AbstractSystematicsModel.h"
#include "fitting/AbstractSignalFitModel.h"

//----------------------------------------------------------------------
#include "XsectUtil.h"


//----------------------------------------------------------------------

#include "interpolation/SignalModelParameterInterpolator.h"

#include "DataSetList.h"
#include "InputDataSetInfo.h"
#include "NormalizationComparison.h"

#define IMPORT_ONLY_TOP_LEVEL_OBJECTS

using namespace std;


//----------------------------------------------------------------------
// fit parameter names
//----------------------------------------------------------------------

//const string PARAM_NAME_DELTA_MASS_1 = "dm1"; // 0: delta mean of 1st Gaussian (delta w.r.t. generated Higgs mass)
//
//fitparms.push_back(simpleGaussMassFunctionRight.dm1);
//fitparms.push_back(simpleGaussMassFunctionRight.dm2); // 1: delta mean of 2nd Gaussian (delta w.r.t. generated Higgs mass)
//fitparms.push_back(simpleGaussMassFunctionRight.dm3); // 2: delta mean of 3rd Gaussian (delta w.r.t. generated Higgs mass)
//fitparms.push_back(simpleGaussMassFunctionRight.sigma1); // 3: sigma of 1st Gaussian
//fitparms.push_back(simpleGaussMassFunctionRight.sigma2); // 4: sigma of 2nd Gaussian
//fitparms.push_back(simpleGaussMassFunctionRight.sigma3); // 5: sigma of 3rd Gaussian
//fitparms.push_back(&simpleGaussMassFunctionRight.f1); // 6
//fitparms.push_back(&simpleGaussMassFunctionRight.f2); // 7

// same parameters as above but for the wrong vertex mass distribution
//fitparms.push_back(&wdm1); // 8
//fitparms.push_back(&wdm2); // 9
//fitparms.push_back(&wsigma1); // 10
//fitparms.push_back(&wsigma2); // 11
//fitparms.push_back(&wf1); // 12
//fitparms.push_back(&fracright); // 13 fraction of events assigned to the correct vertex


//----------------------------------------------------------------------

// #include "fitting/Fitter.h"


// static const unsigned FITPARAM_INDEX_EFFACC = 14;
// #define FITPARAM_INDEX_EFFACC 14

/** implements the same functionality as found before in the single (big)
 *  function but allows splitting the code into several methods
 *  for easier maintainability.
 */
class FitHmassGlobe4CatClass
{
private:
  //----------------------------------------------------------------------
  // parameters
  //----------------------------------------------------------------------
  ParametricSignalModelConfig config;

  /** inverse mapping of the above */
  map<string, vector<string> > inverseSignalProcessMergingMapping;

  //----------------------------------------------------------------------

  /** the input workspace */
  RooWorkspace* win;

  /** variable containing the input workspace integrated luminosity */
  RooRealVar *IntLumi;

  std::vector<int> mhs;
  std::vector<std::string> catnames;

  /** with recent input files, the signal comes in per Feynman-Diagram datasets */
  vector<string> inputSigProcessNames;

  /** signal process names after merging */
  vector<string> mergedSigProcessNames;

  /** variable for the Higgs mass hypothesis */
  RooRealVar *mnom;

  /** variables to be used to fit the signal (e.g. reconstructed Higgs candidate
   *  mass variable etc.).
   *
   *  TODO: variable needs to be renamed
   *   */
  vector<RooRealVar *> hmass;

  //----------

  /** cross sections to be used to calculate the efficiencies from the
   *  expected number of events in the input workspace
   */
  XsectUtil *xsectsForEfficiencies;

  /** cross sections to be used to calculate the expected number of
   *  events in the output workspace.
   */
  XsectUtil *xsectsForNormalization;

  //----------
  
  ReportMaker *reportMaker;

  /** for reporting the number of MC events in the input datasets etc. */
  InputDataSetInfo inputDataSetInfo;

  /** for adding a comparison between the normalizations in the input 
      and output files to the report */
  NormalizationComparison normalizationComparison;

  //----------------------------------------------------------------------
  // classes for executing the different steps
  //----------------------------------------------------------------------

  FittingStep *fittingStep;

  /** the user specified signal model */
  AbstractSignalModel *signalModel;

  AbstractSystematicsModel *systematicsModel;

  /** nuisance parameter for varying the fraction of correctly assigned
   *  vertices.
   *
   */
  RooRealVar *nuisanceDeltaFracRight;

  //----------------------------------------------------------------------

  /** model-specific information (what to fit) */
  AbstractSignalFitModel *signalFitModel;

  /** to keep track of the input (MC) datasets to be fitted */
  DataSetList *dataSets;

  // we keep the eff*acc parameters separate because they can have a different set of
  // processes (they depend on the original list of signal processes,
  // not the merged ones)
  SignalModelParameterInterpolatorMap fitParamInterpolator;
  SignalModelParameterInterpolatorMap effaccInterpolator;

  /** the output workspace */
  RooWorkspace *wsig;
  //----------------------------------------------------------------------

  /** opens the shared object specified in the configuration which
   *  must implement the signal model. Then instantiates an object
   *  of this class, sets some parameters and returns the instance.
   */
  AbstractSignalModel *makeSignalModelObject()
  {
    // open the shared library
    void* soHandle = dlopen(config.signalModelSharedObjectName.c_str(), RTLD_LAZY);

    if (! soHandle)
    {
      cerr << "could not open signal model shared object file '" << config.signalModelSharedObjectName << "': " << dlerror() << endl;
      exit(1);
    }

    // load the instantiation function (the function which creates
    // an instance of the needed class)
    typedef AbstractSignalModel *(*InstantiationFunctionType)();

    dlerror();

    const char *instantiationFunctionName = "makeInstance";

    InstantiationFunctionType instantiationFunction = (InstantiationFunctionType) dlsym(soHandle, instantiationFunctionName);
    const char *errMessage = dlerror();
    if (errMessage)
    {
      cerr << "could not find symbol '" << instantiationFunctionName << "' in signal model shared object file '" << config.signalModelSharedObjectName << "'. Exiting." << endl;
      dlclose(soHandle);
      exit(1);
    }

    signalModel = instantiationFunction();

    signalModel->setGlobalConfig(config);
    signalModel->setModelSpecificConfigDict(config.modelConfigDict);

    signalModel->setHiggsMassHypothesisVar(mnom);
    signalModel->setInputProcessNames(inputSigProcessNames);

    return signalModel;
  }


public:
  //----------------------------------------------------------------------
  FitHmassGlobe4CatClass(const ParametricSignalModelConfig &_config) :
    config(_config),
    wsig(NULL)
  {
    if (config.htmlReportFname != NULL)
      reportMaker = new ReportMaker();
    else
      reportMaker = NULL;

    // open the input file (containing the output of fitter.py)
    TFile *fdata = new TFile(config.inputFname.c_str());

    if (!fdata->IsOpen())
    {
      cerr << "could not open input root file '" << config.inputFname << "'" << endl;
      exit(1);
    }

    win = (RooWorkspace*) fdata->Get(config.inputWorkspaceName.c_str());

    // if we don't do this, we'll get a segmentation fault at the end
    // of the program (when RooFit cleans up)
    gROOT->cd();

    if (win == NULL)
    {
      cerr << "workspace '" << config.inputWorkspaceName << "' not found in file " << config.inputFname << endl;
      exit(1);
    }

    IntLumi = win->var("IntLumi");
    ASSERTBREAK(IntLumi != NULL);

    dataSets = new DataSetList(win, config);

    // perform initializations
    initializeMassPoints();
    initializeProcessNames();

    //----------
    // calculate the inverse mapping of the signal process mappings
    //----------
    for (map<string, string>::const_iterator it = config.signalProcessMergingMapping.begin(); it != config.signalProcessMergingMapping.end(); ++it)
    {
      inverseSignalProcessMergingMapping[it->second].push_back(it->first);
    }

    //----------

    // create the list of process names AFTER merging
    for (map<string, string>::const_iterator it = config.signalProcessMergingMapping.begin(); it != config.signalProcessMergingMapping.end(); ++it)
    {
      if (find(mergedSigProcessNames.begin(), mergedSigProcessNames.end(), it->second) == mergedSigProcessNames.end())
        // element not yet seen
        mergedSigProcessNames.push_back(it->second);
    }
    //--------------------

    // to keep the same normalization, take the cross sections for the efficiency calculation
    // AND the normalization in the output workspace from the same source.
    //
    // Currently, we don't have fine enough mass steps for the cross sections in the input files,
    // so we need those from the code for interpolation.
    this->xsectsForEfficiencies = XsectUtil::fromXsectCode(config.sqrts, config.fermiophobic);

    this->xsectsForNormalization = XsectUtil::fromXsectCode(config.sqrts, config.fermiophobic);

    //--------------------

    mnom = new RooRealVar("MH", "m_{h}", 110.0, 100.0, 200.0, "GeV");
    mnom->setConstant();
    //mnom->setRange("plotrange",110,140);
    //--------------------

    signalModel = this->makeSignalModelObject();

    list<string> signalFitVariables = signalModel->getSignalFitVarNames();

    BOOST_FOREACH(string signalFitVariable, signalFitVariables)
    {
      RooRealVar *tmp = win->var(signalFitVariable.c_str());
      if (tmp == NULL)
      {
        cerr << "the signal model specifies a variable '" << signalFitVariable << "' to be required for fitting the signal model but no such variable was found in the input workspace. Exiting." << endl;
        exit(1);
      }

      hmass.push_back(tmp);
    }

    signalModel->setSignalFitVariables(hmass);

    catnames = signalModel->getCategoryNames();



    signalFitModel = signalModel->getSignalFitModel();
    wsig = new RooWorkspace(config.signalWorkspaceName.c_str(), "signal workspace");
    
    wsig->import(*mnom);

  }
  //----------------------------------------------------------------------
private:

  void
  initializeProcessNames()
  {

    if (!config.fermiophobic)
    {
      inputSigProcessNames.push_back("ggh");
      inputSigProcessNames.push_back("tth");
    }

    inputSigProcessNames.push_back("vbf");
    inputSigProcessNames.push_back("wzh");

  }

  //----------------------------------------------------------------------

  /** initializes the mass points at which we should perform the fits */
  void
  initializeMassPoints()
  {
    // newer version depends on the given parameters
    // for (double mass = 110; mass <= 150; mass +=
    // config.massInterpolationWidth)
    for (double mass = config.massmin; mass <= config.massmax; mass += config.massInterpolationWidth)

      mhs.push_back((int) (mass + 0.5));

    // testing / debugging
    // mhs.clear();
    // mhs.push_back(110);
    // mhs.push_back(120);
  }
  //----------------------------------------------------------------------

  //----------------------------------------------------------------------
  void
  makeCombCatDatasets(RooRealVar *weight, const vector<RooRealVar *> &hmass, RooWorkspace *wextra)
  {
    RooArgList args;
    BOOST_FOREACH(RooRealVar *var, hmass)
    {
      args.add(*var);
    }

    args.add(*weight);

    for (UInt_t i = 0; i < mhs.size(); ++i)
    {
      std::stringstream numstringstr;
      numstringstr << mhs.at(i);
      TString numstring(numstringstr.str());
      //--------------------
      // take the datasets with the signal events from the input workspace
      //--------------------

      // create a new RooDataSet for signal MC, correct vertex assignment, all categories combined ?
      // (not clear where this is used here, seems only to be imported into the workspace ?)
      RooDataSet *rvdata = new RooDataSet(TString::Format("sig_mass_m%i_rv_AllCats", mhs.at(i)), "", args, weight->GetName());

      // create a new RooDataSet for signal MC, wrong vertex assignment, all categories combined ?
      // (not clear where this is used here, seems only to be imported into the workspace ?)
      RooDataSet *wvdata = new RooDataSet(TString::Format("sig_mass_wv_m%i_AllCats", mhs.at(i)), "", args, weight->GetName());

      // create a new RooDataSet for signal MC, both correct and wrong vertex assignments, all categories combined
      // (used for doing plots)
      RooDataSet *alldata = new RooDataSet(TString::Format("sig_mass_m%i_AllCats", mhs.at(i)), "dataset of all categories, sig procs and vertices at this mass", args, weight->GetName());

      //--------------------
      // merging (again) the input datasets
      BOOST_FOREACH(string catname, catnames)
      {
        // dataset with all events of this category (unweighted)
        RooDataSet *thisCatData = new RooDataSet(Form("sig_mass_m%i_%s", mhs.at(i), catname.c_str()), "", args, weight->GetName());

        BOOST_FOREACH(string sigProcName, inputSigProcessNames)
        {
          // Note that the IC inputs seem not to distinguish between
          // right and wrong vertex assignment ?! (i.e. these
          // are fit together, not separately)

          if (config.useRightWrongVertex)
          {
            RooDataSet *mcsigdata = dataSets->getDataSet(TString("sig_") + sigProcName + "_mass_m" + numstring + TString("_rv_") + catname);
            RooDataSet *mcsigwrongdata = dataSets->getDataSet(TString("sig_") + sigProcName + "_mass_m" + numstring + TString("_wv_") + catname);

            // add to per vertex type dataset
            appendWeightedDataset(rvdata, mcsigdata, hmass, weight, 1.0);
            appendWeightedDataset(wvdata, mcsigwrongdata, hmass, weight, 1.0);

            // add to per-category dataset
            appendWeightedDataset(thisCatData, mcsigdata,      hmass, weight, 1.0);
            appendWeightedDataset(thisCatData, mcsigwrongdata, hmass, weight, 1.0);
            
            // append to overall dataset
            appendWeightedDataset(alldata, mcsigdata,      hmass, weight, 1.0);
            appendWeightedDataset(alldata, mcsigwrongdata, hmass, weight, 1.0);

	    // add for reporting
	    inputDataSetInfo.add(mcsigdata,      mhs.at(i), catname, true, sigProcName);
	    inputDataSetInfo.add(mcsigwrongdata, mhs.at(i), catname, false, sigProcName);
            normalizationComparison.addInputDataset(mcsigdata,      mhs.at(i), catname, true, sigProcName);
            normalizationComparison.addInputDataset(mcsigwrongdata, mhs.at(i), catname, false, sigProcName);
	    
            // add the dataset from the input file to debugging workspace
	    wextra->import(*mcsigdata);
	    wextra->import(*mcsigwrongdata);
          }
          else
          {
            // for the sake of simplicity, just assign ALL events to the
            // 'right' vertex datasets
            RooDataSet *mcsigdata = dataSets->getDataSet(TString("sig_") + sigProcName + "_mass_m" + numstring + TString("_") + catname);

            // add to per vertex type dataset
            appendWeightedDataset(rvdata, mcsigdata, hmass, weight, 1.0);

            // add to per-category dataset
            appendWeightedDataset(thisCatData, mcsigdata,      hmass, weight, 1.0);

            // add to overall dataset
            appendWeightedDataset(alldata, mcsigdata,          hmass, weight, 1.0);

	    // add for reporting
	    inputDataSetInfo.add(mcsigdata,      mhs.at(i), catname, true, sigProcName);
            normalizationComparison.addInputDataset(mcsigdata,      mhs.at(i), catname, true, sigProcName);

	    wextra->import(*mcsigdata);
          }

          // TODO: what do we need this for ?!
          {
            // RooDataSet *mcsigalldata = dataSets->getDataSet(TString("sig_") + sigProcName + "_mass_m" + numstring + TString("_") + catname);
            
            //rvdata->append(*mcsigdata);
            //wvdata->append(*mcsigwrongdata);
            //alldata->append(*mcsigalldata);
            // appendWeightedDataset(alldata, mcsigalldata, hmass, weight, 1.0);
          }

        } // loop over signal process names

        wsig->import(*thisCatData);

      } // loop over categories

      // add these combined datasets to the (input) workspace
      win->import(*rvdata);
      win->import(*wvdata);
      win->import(*alldata);

      // also to the extra workspace
      wextra->import(*rvdata);
      wextra->import(*wvdata);
      wextra->import(*alldata);

      // also to the standard output workspace
      wsig->import(*alldata);
      //--------------------


    } // loop over categories

  }

  //----------------------------------------------------------------------

  /** this function can be used to avoid filling the interpolating
      histograms more than once for the same bin */
  bool isInterpolationPoint(int mh)
  {
    // original version, before allowing mass range starting
    // at anything other than 100 GeV
    // return (mh % ((int) (config.massInterpolationWidth + 0.5))) == 0;

    // ugly stuff here, we should definitively move to piecewise linear
    // interpolating functions
    bool retval = (
            ((int)(mh - config.massmin + 0.5)) % ((int) (config.massInterpolationWidth + 0.5))
            ) == 0;

    // cout <<  "UUUU returning " << retval << " for mh=" << mh << endl;

    return retval;
  }

  //----------------------------------------------------------------------

  void
  fillEffAcc(int mh, unsigned icat, const string &inputSigProcName,
      RooRealVar *IntLumi,
      SignalModelParameterInterpolatorMap &effaccHistsX)
  {
    const double effaccinit = 0.9;

    string catname = catnames.at(icat);

    // TODO: is it ok to declare the same variable over and over here ?
    RooRealVar effacc("effacc", "effacc", effaccinit, 0.0, 1.0);

    // TODO: note that the total cross section is NOT used as a variable
    // (but only its value -- this was the case already before we at UCSD
    // manipulated the macro), so this is currently NOT suitable
    // for determining RELATIVE limits (just absolute ones where
    // the theoretical cross section uncertainty does not enter)

    // IC also has separated this into the cross section per Feynman diagram.
    // for the moment, just add them together (no possibility to vary them
    // independently for the moment)

    // double totalCrossSectionValue = this->xsectsForEfficiencies->getSum(mh);
    double crossSectionValue = this->xsectsForEfficiencies->get(mh, inputSigProcName);

    //compute acceptance * efficiency and right vertex fraction
    double effAccNumerator = 0;

    // now get the original datasets from the background input working space
    // (the weighted ones are merged already so do not contain the per
    // signal process information any more)

    if (config.useRightWrongVertex)
    {
      effAccNumerator =
      // right vertex
      dataSets->getDataSet(Form("sig_%s_mass_m%d_rv_%s", inputSigProcName.c_str(), mh, catname.c_str()))->sumEntries() +

      // wrong vertex
      dataSets->getDataSet(Form("sig_%s_mass_m%d_wv_%s", inputSigProcName.c_str(), mh, catname.c_str()))->sumEntries();
    }
    else
    {
      // just use the 'correct' ones
      // ASSERT(mcsigwrongwdata == NULL);
      effAccNumerator = dataSets->getDataSet(Form("sig_%s_mass_m%d_%s", inputSigProcName.c_str(), mh, catname.c_str()))->sumEntries();

    }

    // double effAccDenominator = IntLumi->getVal() * totalCrossSectionValue;
    double effAccDenominator = IntLumi->getVal() * crossSectionValue;

    //double eaccden = (gfxsecs.at(i)+vbfxsecs.at(i)+vhxsecs.at(i))*1e6;
    //double eaccden = (vbfxsecs.at(i)+vhxsecs.at(i))*1e6;

    // set the value of efficiency times acceptance
    assignRatio(effacc, effAccNumerator, effAccDenominator);

    // cout << "UUU mass=" << mh << " cat=" << icat << " proc=" << inputSigProcName << " effAccNumerator=" << effAccNumerator << " effAccDenominator=" << effAccDenominator << " ratio=" << effAccNumerator / effAccDenominator << endl;
    // cout << "UUU0 totalCrossSectionValue=" << totalCrossSectionValue << endl;

    // make sure we do NOT fill values into a bin more than once
    if (isInterpolationPoint(mh))
    {
      // fill the efficiency (and its asymmetric error) into
      // the efficiency histogram for category 'numInclusiveCategories'

      //TH1F *histo = NULL;
      SignalModelParameterInterpolator *histo;

      // MUST USE INPUT SIGNAL PROCESSES HERE
      histo = effaccHistsX.get(catname,inputSigProcName);

      // fill the histogram
      // histo->Fill(mh, effacc.getVal());

      // set the error in this bin (average of positive and negative error bars)
      double avgerror = (effacc.getErrorLo() + effacc.getErrorHi()) / 2.0;
      // histo->SetBinError(histo->FindFixBin(mh), avgerror);

      // cout<< "PPPP filling " << effacc.getVal() << endl;

      histo->addValue("effacc", mh, effacc.getVal(), avgerror);

    } // if this is a mass point used in the histogram

  }

  //----------------------------------------------------------------------


  /** fills all the efficiency and acceptance variables */
  void fillAllEffAcc()
  {
    for (UInt_t icat = 0; icat < catnames.size(); ++icat)
    {
      const string catname = catnames.at(icat);
      // also create histograms for effacc (note that we loop over the INPUT
      // process names here, not the merged signal process names
      BOOST_FOREACH(string inputSigProcName, inputSigProcessNames)
      {
        effaccInterpolator.add(catname,inputSigProcName, new SignalModelParameterInterpolator(config, list_of("effacc"),
            mnom,
            catname + "_" + inputSigProcName));
      }

    } // loop over categories


    //----------
    for (UInt_t icat = 0; icat < catnames.size(); ++icat)
    {
      string catname = catnames.at(icat);

      BOOST_FOREACH(string inputSigProcName, inputSigProcessNames)
      {
        for (UInt_t i = 0; i < mhs.size(); ++i)
        {
          int mh = mhs.at(i);

          fillEffAcc(mh, icat, inputSigProcName,
              IntLumi,
              effaccInterpolator);
        } // loop over Higgs masses
      } // loop over input signal Feynman diagrams
    } // loop over categories

    //----------

  }

  //----------------------------------------------------------------------

  void wsigImport(RooAbsArg &object, Bool_t replaceExisting = kFALSE)
  {
    bool importError = wsig->import(object, replaceExisting);
    if (! importError)
      return;

    cerr << "ERROR importing " << object.GetName() << " into signal workspace, see error message above" << endl;
    exit(1);
  }


  //----------------------------------------------------------------------

public:

  /** perform the calculations */
  void
  run()
  {
    gStyle->SetErrorX(0);
    gStyle->SetOptStat(0);
    gROOT->ForceStyle();

    //--------------------

    gStyle->SetOptStat(1110);

    //define signal pdf variables

    // parameter initialization for wrong vertex Mass
    const double wdm1init = 0.5;
    RooRealVar wdm1("wdm1", "", wdm1init, -12.0, 5.0);
    wdm1.removeRange();

    const double wdm2init = -0.7;
    RooRealVar wdm2("wdm2", "", wdm2init, -12.0, 5.0);
    wdm2.removeRange();

    //--------------------

    // RooFormulaVar wmean1("wmean1", "", "@0+@1", RooArgList(*mnom, wdm1));
    // RooFormulaVar wmean2("wmean2", "", "@0+@1", RooArgList(*mnom, wdm2));

    const double wsigma1init = 4.0;
    RooRealVar wsigma1("wsigma1", "", wsigma1init, 0.0, 10.0); //2.0
    wsigma1.removeRange();

    const double wsigma2init = 2.0;
    RooRealVar wsigma2("wsigma2", "", wsigma2init, 0.0, 10.0); //3.0
    wsigma2.removeRange();

    // some fraction for adding wg1 and wg2
    const double wf1init = 0.6;
    RooRealVar wf1("wf1", "", wf1init, 0.52, 1.0);

    //                      indep var      sigma
    //                              mean
    // RooGaussian wg1("wg1", "", *hmass, wmean1, wsigma1);
    // RooGaussian wg2("wg2", "", *hmass, wmean2, wsigma2);

    //--------------------

    // AH: we could actually use funceffacc_catX instead...
//    for (unsigned j = config.numInclusiveCategories; j < catnames.size(); ++j)
//    {
//      BOOST_FOREACH(string inputSigProcName, inputSigProcessNames)
//      {
//        const string catname = catnames.at(j);
//        effaccInterpolator.add(catname, inputSigProcName, new SignalModelParameterInterpolator(config,
//            list_of("effacc"),
//            mnom,
//            catname + "_" + inputSigProcName));
//      }
//    } // loop over all categories

    RooRealVar *weight = new RooRealVar("weight", "", 1.0);
    weight->removeRange();

    // loop over categories for fits
    RooWorkspace *wextra = new RooWorkspace("wextra", "");

    makeCombCatDatasets(weight, hmass, wextra);

    //----------------------------------------
    // define histograms to keep track of fit parameters for each mass point
    //----------------------------------------

    // indexing is:
    //   first index is the category number
    //   second index is the signal process name
    //   third index is the number of the ` to be fit

    // map<unsigned, map<string, map<unsigned, TH1F *> > > fitparmhistsX;
    // map<unsigned, map<string, TH1F *> > effaccHistsX;


    //----------

#ifdef TEST_SINGLE_MASS_POINT
    std::vector<RooDataSet*> testdsets;
    std::vector<RooDataSet*> testdsetswrong;
    std::vector<RooDataSet*> testdsetsall;
#endif

    dataSets->makeWeightedDataSets(weight, hmass, NULL, catnames,
        inputSigProcessNames,
        mhs);

    // add information about the datasets to output report
    if (reportMaker != NULL)
      reportMaker->addHTML("input datasets information", inputDataSetInfo.makeHTMLtable());

    fillAllEffAcc();

    //----------------------------------------------------------------------
    // fit and then fill the interpolation functions
    //----------------------------------------------------------------------

    this->fittingStep = new FittingStep(
        config,
        *dataSets,
        catnames,
        mergedSigProcessNames,
        mhs,
        signalFitModel,
        mnom,
        hmass,

        fitParamInterpolator,
        reportMaker
    );
    fittingStep->run(wextra);

    //----------------------------------------------------------------------
    // plot evolution of each fit parameter in each category as function of higgs mass
    //----------------------------------------------------------------------
    if (! config.skipInterpolatedParameterPlots && reportMaker != NULL)
    {
      this->fitParamInterpolator.plotAll(reportMaker); // over merged signal process names
      this->effaccInterpolator.plotAll(reportMaker);   // over input signal process names
    }

    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    // consistency checks
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------

    // integrated luminosity (that's actually just the scaling parameter with respect to the actual luminosity
    // set in the input data cards for combine ?!)
    RooRealVar *lumiNuisance = new RooRealVar("intlumi", "int. lumi nuisance parameter", 1.0, 0.0, 100e3);
    lumiNuisance->setConstant();
#ifndef IMPORT_ONLY_TOP_LEVEL_OBJECTS
    wsigImport(*lumiNuisance);
#endif

    // total Higgs cross-section nuisance parameter -- this is ONE and not changed (set constant)
    RooRealVar *xsectNuisance = new RooRealVar("totalxsec", "cross section nuisance parameter", 1.0, 0.0, 100.0);
    xsectNuisance->setConstant();
#ifndef IMPORT_ONLY_TOP_LEVEL_OBJECTS
    wsigImport(*xsectNuisance);
#endif

    //--------------------

    // a 'histogram function' containing the (interpolated) cross sections
    map<string, RooHistFunc *> funcXsecNorm;

    RooHistFunc *funcBr = NULL;

    // TODO: for the moment, we loop over the input signal processes, in the future we
    // could sum the input contributions of the output groups
    // (currently, these are not needed)
    BOOST_FOREACH(string sigProcName, inputSigProcessNames)
    {
      funcXsecNorm[sigProcName] = this->xsectsForNormalization->makeCrossSectionFunctionUsingHistogram(sigProcName, reportMaker, mnom);
    } // loop over signal diagrams

    funcXsecNorm["sum"] = this->xsectsForNormalization->makeCrossSectionFunctionUsingHistogram("sum", reportMaker, mnom);
    funcBr = this->xsectsForNormalization->makeBranchingRatioFunctionUsingHistogram(reportMaker, mnom);

    //--------------------

    //----------------------------------------------------------------------
    // define final pdfs (including the nuisance parameters) in each category
    //----------------------------------------------------------------------

    if (config.useRightWrongVertex)
    {
      nuisanceDeltaFracRight = new RooRealVar("CMS_hgg_nuissancedeltafracright", "", 1.0, 0.1, 10.0);
      nuisanceDeltaFracRight->setConstant();
    }
    else
      nuisanceDeltaFracRight = NULL;
    this->systematicsModel = signalModel->getSystematicsModel(fitParamInterpolator, effaccInterpolator);

    doTests();

    //--------------------

    /** these are efficiency with
     *     - lumi nuisance
     *     - xsect nuisance
     *
     *  first index is category name, second index is the
     *  input (unmerged) signal process name
     */
    map<std::string, map<string, RooAbsReal*> > nuisancedEffAccs;

    // define RooCategory for simultaneous fit
    RooCategory finalcat("finalcat", "finalcat");
    for (unsigned j = 0; j < catnames.size(); ++j)
      finalcat.defineType(catnames.at(j).c_str());

    //----------
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //
    // THIS IS THE NORMALIZATION (hggpdfrel_catX_<proc>_norm)
    // USED IN THE LIMIT SETTING
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //
    // cross section (*BR ?) * nuisancedEffAccs
    // (i.e. expected events for unit integrated luminosity)
    //
    // the first index is the category name, the second index is
    // the input signal process name
    map<string, map<string, RooFormulaVar *> > unitLumiExpectedEvents;

    //----------------------------------------
    // make normalization variables
    //----------------------------------------
    // produce quantities which are per unmerged signal process

    BOOST_FOREACH(string inputSigProcName, inputSigProcessNames)
    {
      BOOST_FOREACH(string catname, catnames)
      {
        RooAbsReal *tmp = this->systematicsModel->getNuisancedEffAcc(catname, inputSigProcName);

        // add lumi and signal cross section nuisances
        nuisancedEffAccs[catname][inputSigProcName] = new RooFormulaVar(Form("hggpdf_%s_%s_norm", catname.c_str(), inputSigProcName.c_str()), "",
            "@0*@1*@2",
            RooArgList(*lumiNuisance, *xsectNuisance, *tmp));

#ifndef IMPORT_ONLY_TOP_LEVEL_OBJECTS
        wsigImport(*nuisancedEffAccs[catname][inputSigProcName]);
#endif

        unitLumiExpectedEvents[catname][inputSigProcName] = new RooFormulaVar(
             normalizationFunctionName(catname, inputSigProcName).c_str(), "",
            "@0*@1*@2",

            RooArgList(
                *funcBr,                         // branching ratio
                *funcXsecNorm[inputSigProcName], // xsect
                *nuisancedEffAccs[catname][inputSigProcName] // eff*acc
            ));

      } // loop over categories

    } // loop over unmerged signal process names

    //----------------------------------------
    // create the PDFs
    //----------------------------------------

    // for the sum over all PDFs (i.e. over all signal processes, right/wrong vertex and categories)
    // according to http://root.cern.ch/root/html/tutorials/roofit/rf202_extendedmlfit.C.html
    // one can just build a RooAddPdf from RooExtendPdf and the normalization should be ok
    RooArgList *allSigProcCatVertexPdfs = new RooArgList();

    BOOST_FOREACH(string catname, catnames)
    {
      // for summing over all vertex types and signal processes
      // (but only one category). Keep them in a vector
      RooArgList *allCatVertexPdfs = new RooArgList();

      BOOST_FOREACH(string inputSigProcName, inputSigProcessNames)
      {
        string mergedSigProcName = config.signalProcessMergingMapping[inputSigProcName];

        // final extended pdf's in each category

        // relative cross section version of pdfs and normalization

        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        //
        // THIS (hggpdfrel_catX_<proc>) IS THE (PER CATEGORY) PDF USED FOR LIMIT SETTING
        // (WHERE THE NORMALIZATION IS TAKEN FROM hggpdfrel_catX_<proc>_norm)
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////

        RooAbsPdf *hggpdfrel_cat = makeNuisancedPerCategoryPdf(catname, inputSigProcName);

#ifndef IMPORT_ONLY_TOP_LEVEL_OBJECTS
        wsigImport(*hggpdfrel_cat);
#endif
        ASSERTBREAK(unitLumiExpectedEvents.count(catname) > 0);
        ASSERTBREAK(unitLumiExpectedEvents[catname].count(inputSigProcName) > 0);
        ASSERTBREAK(unitLumiExpectedEvents[catname][inputSigProcName] != NULL);

        // build an extended PDF of pdf times normalization
        RooExtendPdf *sigPdfRelCat = new RooExtendPdf(Form("sigpdfrel%s_%s", catname.c_str(), inputSigProcName.c_str()), "",
            *hggpdfrel_cat, *unitLumiExpectedEvents[catname][inputSigProcName]);

        // this is the per category top-level object (?!)
        // wsigImport(*sigPdfRelCat);
        // TODO: why do we need RecycleConflictNodes() here ? The object already existing
        //       in the workspace should be the same ?!
        wsig->import(*sigPdfRelCat, RecycleConflictNodes());

        // add to the list of components of the sum pdfs
        allCatVertexPdfs->add(*sigPdfRelCat);

        // this seems to cause funny effects (random strings in the output
        // of sigpdfrelcatX->Print("V") when the clients are printed,
        // the sum over categories and processes pdf suddenly
        // disappearing from the workspace.
        // Thus we make the sum over all categories
        // from the sum over processes further down
        //
        // allSigProcCatVertexPdfs->add(*sigPdfRelCat);

      } // loop over signal processes

      // build the pdf summed over vertex types and signal processes
      // for this category
      RooAddPdf *sumPdf = new RooAddPdf(Form("sigpdfrel%s_allProcs", catname.c_str()),
                        Form("sum over right/wrong vertices and signal processes %s", catname.c_str()),
                        *allCatVertexPdfs);
      wsig->import(*sumPdf , RecycleConflictNodes());
      allSigProcCatVertexPdfs->add(*sumPdf);
    } // loop over categories

    // build the pdf summed over vertex types, signal processes and categories
    {
      RooAddPdf *sumPdf = new RooAddPdf("sigpdfrelAllCats_allProcs",
                        "sum over right/wrong vertices, signal processes and categories",
                        *allSigProcCatVertexPdfs);
      wsig->import(*sumPdf , RecycleConflictNodes());
    }

    //----------------------------------------


    // the overall normalization sum (?!)
    RooFormulaVar *hggpdf_combcat_norm = 0;

    RooArgList addnorm;
    {
      std::string buf;
      bool isFirst = true;

      unsigned k = 0;
      assert(catnames.size() == nuisancedEffAccs.size());

      BOOST_FOREACH(string catname, catnames)
      {
        BOOST_FOREACH(string sigProcName, mergedSigProcessNames)
        // for (unsigned sigProcessIndex = 0; sigProcessIndex < sigProcessNames.size(); ++sigProcessIndex)
        {
          // compose the overall sum expression
          if (isFirst)
            isFirst = false;
          else
            buf += "+";

          buf += Form("@%d", k++);

          addnorm.add(*nuisancedEffAccs[catname][sigProcName]);

        } // loop over signal processes
      } // loop over categories

      hggpdf_combcat_norm = new RooFormulaVar("hggpdf_combcat_norm", "", buf.c_str(), addnorm);

    } // end of block

    //----------
    // write the RooFit objects out to the workspaces
    writeOut(effaccInterpolator, wextra);

    //----------------------------------------
    // add comparison input to output normalizations to
    // report (note that this needs some objects
    // to be present in the output workspace so
    // we do it only here when all objects
    // were imported into the output workspace)
    if (reportMaker != NULL)
    {
      // why does passing mnom directly not work ? Because the object
      // in the output workspace is a (detached) copy ?
      RooRealVar *MH = wsig->var(mnom->GetName());
      ASSERTBREAK(MH != NULL);
      normalizationComparison.getOutputNormalizations(config.useRightWrongVertex, wsig, MH, IntLumi->getVal());
      reportMaker->addHTML("input datasets/output workspace normalization comparison", normalizationComparison.makeHTMLtable());
    }

#if 0
    // print some information
    BOOST_FOREACH(string inputSigProcName, inputSigProcessNames)
    {
      // for (unsigned j = 0; j < catnames.size(); ++j)
      BOOST_FOREACH(string catname, catnames)
      {
        ASSERTBREAK(systematicsModel->effAccFunctions.count(catname) > 0);
        ASSERTBREAK(systematicsModel->effAccFunctions[catname].count(inputSigProcName) > 0);
        ASSERTBREAK(systematicsModel->effAccFunctions[catname][inputSigProcName] != NULL);

        printf("%s proc %s: finalnormslides = %5f, n = %5f\n", catname.c_str(), inputSigProcName.c_str(),
            systematicsModel->effAccFunctions[catname][inputSigProcName]->getVal(), nuisancedEffAccs[catname][inputSigProcName]->getVal());
      }
    }
#endif

    // write the report
    if (this->reportMaker != NULL)
      this->reportMaker->write(*config.htmlReportFname);
    
    //----------------------------------------
    // write information about the nuisance parameters
    // TODO: could we find them ourselves ? Leaves in
    //       the tree (some of them might actually be constants) ?
    //----------------------------------------

    {
      cout << "----------------------------------------" << endl;
      cout << "nuisance parameters declared by model:" << endl;
      cout << "----------------------------------------" << endl;
      
      vector<RooAbsReal *> nuisanceParams = systematicsModel->getAllNuisanceParameters();
      BOOST_FOREACH(RooAbsReal *param, nuisanceParams)
      {
        cout << "  " << boost::format("%-40s: %s") % param->GetName() % param->GetTitle() << endl;
      }

      cout << "----------------------------------------" << endl;
      cout << "nuisance parameters declared by framework:" << endl;
      cout << "----------------------------------------" << endl;
      
      if (config.useRightWrongVertex)
      {
	RooAbsReal *param = nuisanceDeltaFracRight;
        cout << "  " << boost::format("%-40s: %s") % param->GetName() % param->GetTitle() << endl;
      }
      else
	cout << "none" << endl;
    }

    // if there are failed fits, print the information about them
    // TODO: should we exit with a non-zero value if there were failed fits ?
    this->fittingStep->printFailedFits(cerr);
  }

  //----------------------------------------------------------------------

  /** makes a signal shape function with the nuisanced parameters (i.e. the final signal PDF) */
  RooAbsPdf *makeNuisancedSignalFunction(const std::string &catname, bool rightVertex, const std::string &inputSigProcName)
  {
    std::string vertexName;
    if (rightVertex)
      vertexName = "rightVertex";
    else
      vertexName = "wrongVertex";

    std::vector<RooAbsReal *> paramsRightVertex;

    std::map<std::string, RooAbsReal *> paramMap;

    std::vector<std::string> rightVertexParamNames = this->signalFitModel->getFitFunctionParamNames(
    		catname, rightVertex, inputSigProcName);
    BOOST_FOREACH(string paramName, rightVertexParamNames)
    {
      //string prefix = "fitParam_" + catname + "_" + vertexName + "_";

      // assume that the parameters for right and wrong vertex
      // already are named differently
      string prefix = catname + "_";

      cout << "PARAM NAME " << paramName << endl;

      RooAbsReal *param = this->systematicsModel->getNuisancedParameter(catname, inputSigProcName, paramName);
      paramsRightVertex.push_back(param);

      paramMap[paramName] = param;
    }

    // TODO: is this really the input process name, not the merged one ?
    //       (we're fitting the merged sample anyway !)
    //
    //       yes, this is not used for fitting here, see FittingStep.cc
    //       where the merged signal process name is used
    RooAbsPdf *fitFuncRightPdf = this->signalFitModel->makeFitFunction(mnom, hmass,
    		catname, rightVertex, inputSigProcName,
        "finalFunc_" + catname + "_" + vertexName + "_" + inputSigProcName , // function name
        paramsRightVertex,
        "finalFunc_" + catname + "_" + vertexName + "_" + inputSigProcName + "_" /* prefixForHiddenObjects */
    );

    return fitFuncRightPdf;
  }

  //----------------------------------------------------------------------

  /**
   *  - produces the nuisanced pdfs for right and wrong vertices,
   *  - adds them together with the corresponding fraction of right vertices (which is also nuisanced here)
   */
  RooAbsPdf *makeNuisancedPerCategoryPdf(const std::string &catname, const std::string &inputSigProcName)
  {
    string catPrefix = catname + "_";
    string mergedSigProcName = this->config.signalProcessMergingMapping[inputSigProcName];
    // SignalModelParameterInterpolator *paramFuncs = this->systematicsModel->fitparmfuncs.get(catname, mergedSigProcName);
    SignalModelParameterInterpolator *paramFuncs = fitParamInterpolator.get(catname, mergedSigProcName);


    // nuisanced fraction of right vertices
    RooFormulaVar *fracrightmodslides = NULL;
    if (config.useRightWrongVertex)
      fracrightmodslides = new RooFormulaVar(TString("fracrightmodslide") + catname + "_" + inputSigProcName, "",
        "@0*@1",
        RooArgList(*this->nuisanceDeltaFracRight, *paramFuncs->getFunction(catPrefix + "fracright")));
    else
      fracrightmodslides = NULL;

    //--------------------
    // right vertex
    //--------------------

    RooAbsPdf *combSlides = this->makeNuisancedSignalFunction(catname, true, mergedSigProcName);

    if (config.useRightWrongVertex)
    {
      RooAbsPdf *combhWrongSlides = this->makeNuisancedSignalFunction(catname, false, mergedSigProcName);
      return new RooAddPdf(Form("hggpdfrel_%s_%s", catname.c_str(), inputSigProcName.c_str()),
              "",
              RooArgList(*combSlides,
                         *combhWrongSlides),
              RooArgList(*fracrightmodslides), kTRUE);
    }
    else
    {
      // do not distinguish between right and wrong vertex, just return another
      // PDF with a different name
      return new RooAddPdf(Form("hggpdfrel_%s_%s", catname.c_str(), inputSigProcName.c_str()),
              "",
              RooArgList(*combSlides),
              RooArgList(), kTRUE);

    }

  }



  //----------------------------------------------------------------------

  /** writes the workspaces to the corresponding files */
  void writeOut(SignalModelParameterInterpolatorMap &effaccInterpolator,
      RooWorkspace *wextra)
  {
    // set the default Higgs mass hypothesis
    mnom->setVal(130);

    //save everything to file with RooWorkspace

    // w->import(fullsigpdf,RecycleConflictNodes());
    // w->import(fullsigpdfrel,RecycleConflictNodes());

    for (unsigned icat = 0; icat < catnames.size(); ++icat)
    {
      BOOST_FOREACH(string sigProcName, inputSigProcessNames)
      {
        // funceffacc_catX
        // w->import(*effaccFuncsX[icat][sigProcName], RecycleConflictNodes());
        wsig->import(*effaccInterpolator.get(catnames.at(icat),sigProcName)->getFunction(0), RecycleConflictNodes());
      }
    }

    wsig->Print();

    wsig->writeToFile(config.outputWorkspaceFname.c_str());

    if (config.outputExtraWorkspaceFname != NULL)
      wextra->writeToFile(config.outputExtraWorkspaceFname->c_str());
  }

  //----------------------------------------------------------------------

  /** comparison tests of interpolated shape vs actual fit */
  void doTests()
  {
    for (UInt_t icat = 0; icat < catnames.size(); ++icat)
    {
      BOOST_FOREACH(string sigProcName, mergedSigProcessNames)
      // for (unsigned sigProcessIndex = 0; sigProcessIndex < sigProcessNames.size(); ++sigProcessIndex)
      {
#ifdef TEST_SINGLE_MASS_POINT
        //nuissancedeltasmears[icat]->setVal(-smearingv[icat]);

        // RooAbsPdf *wrongpdf = 0;
        RooAbsPdf *interpdf = 0;

        Bool_t usecomplexmodel = this->signalFitModel->complexset.count(catnames.at(icat));
        Bool_t usesimplemodel = this->signalFitModel->simpleset.count(catnames.at(icat));

        if (usecomplexmodel)
          interpdf = systematicsModel->combHVtxSlides[icat][sigProcName];
        else if (usesimplemodel)
          interpdf = systematicsModel->combhvtxsimpleslides[icat][sigProcName];
        else
        {
          // the 'neither simple nor complex' model
          interpdf = systematicsModel->combhvtxminslides[icat][sigProcName];

          // TODO: parameter initialization
          // simpleGaussMassFunctionRightVertex.sigma[1]->setVal(2.0);
          // simpleGaussMassFunctionRightVertex.sigma[2]->setVal(3.0);
          // simpleGaussMassFunctionRightVertex.f[1]->setRange(0.0, 1.0);
        }

        mnom->setVal(115);
        hmass->setRange("higgsrange", /* 100.0 */ config.massmin, config.massmax);

        // TODO: why are these values hardwired ?
        hmass->setRange("plotrange", 100.0, 115 + 20.0);

        //----------------------------------------
        // another fit is happening here !!!
        //----------------------------------------
        RooFitResult *fitres = rightpdf->fitTo(*testdsets.at(icat),
            Strategy(1), Minimizer("Minuit2", ""), Minos(kFALSE),
            Range("higgsrange"), SumW2Error(kTRUE), Save(kTRUE), NumCPU(config.numCPU));
        RooFitResult *fitreswrong = NULL;
        if (useRightWrongVertex)
        {
          ASSERT(testdsetswrong.at(icat) != NULL);
          wrongpdf->fitTo(*testdsetswrong.at(icat), Strategy(1),
              Minimizer("Minuit2", ""), Minos(kFALSE), Range("higgsrange"),
              SumW2Error(kTRUE), Save(kTRUE), NumCPU(config.numCPU));
        }
        //----------------------------------------

        double fright = 0;
        if (useRightWrongVertex)
        {
          ASSERT(testdsetswrong.at(icat) != NULL);
          fright = testdsets.at(icat)->sumEntries()
          / (testdsets.at(icat)->sumEntries()
              + testdsetswrong.at(icat)->sumEntries());
        }
        else
        fright = 1;

        fracright.setVal(fright);

        TCanvas *ccompint = new TCanvas;
        TString plotname = TString("inttest") + catnames.at(icat) + "_" + sigProcName + TString(".png");
        RooPlot *hplotcompint = hmass->frame(Bins(100), Range("plotrange"));
        testdsetsall.at(icat)->plotOn(hplotcompint);
        allpdf->plotOn(hplotcompint, RooFit::LineColor(kBlue),
            NormRange("higgsrange"), Range("higgsrange"));
        interpdf->plotOn(hplotcompint, RooFit::LineColor(kRed),
            RooFit::LineStyle(kDashed), NormRange("higgsrange"),
            Range("higgsrange"));
        hplotcompint->SetTitle("");
        hplotcompint->Draw();

        TLegend *legmc = new TLegend(0.62, 0.75, 0.92, 0.9);
        legmc->AddEntry(hplotcompint->getObject(0), "MC", "LPE");
        legmc->AddEntry(hplotcompint->getObject(1), "Fit", "L");
        legmc->AddEntry(hplotcompint->getObject(2), "Interpolated Fit", "L");
        legmc->SetBorderSize(0);
        legmc->SetFillStyle(0);
        legmc->Draw();

        ccompint->SaveAs(plotname);
        TCanvas *csmear = new TCanvas;
        TString smearplotname = TString("smeartest") + catnames.at(icat) + "_"
        + sigProcName + TString(".png");
        RooPlot *hplotsmear = hmass->frame(Bins(100), Range("plotrange"));
        testdsetsall.at(icat)->plotOn(hplotsmear);
        interpdf->plotOn(hplotsmear, RooFit::LineColor(kBlue),
            NormRange("higgsrange"), Range("higgsrange"));
        nuissancedeltasmears[icat]->setVal(smearingv[icat]);
        //nuissancedeltasmears[icat]->setVal(0.0);
        interpdf->plotOn(hplotsmear, RooFit::LineColor(kRed),
            NormRange("higgsrange"), Range("higgsrange"));
        hplotsmear->Draw();

        TLegend *legsmear = new TLegend(0.62, 0.75, 0.92, 0.9);
        legsmear->AddEntry(hplotsmear->getObject(0), "MC", "LPE");
        legsmear->AddEntry(hplotsmear->getObject(1), "Fit", "L");
        legsmear->AddEntry(hplotsmear->getObject(2), "Smeared Fit", "L");
        legsmear->SetBorderSize(0);
        legsmear->SetFillStyle(0);
        legsmear->Draw();
        csmear->SaveAs(smearplotname);
#endif

      } // loop over all signal processes
    } // loop over categories
  }
};

//----------------------------------------------------------------------
int
main(int argc, char **argv)
{
  if (argc != 2)
  {
    cerr << "usage: makeParametricSignalModel config.xml" << endl;
    exit(1);
  }

  ParametricSignalModelConfig config = ParametricSignalModelConfig::readPython(argv[1]);

  gROOT->SetStyle("Plain");

//  config.skipFits = true;
//  config.skipInterpolatedParameterPlots = true;


  FitHmassGlobe4CatClass fitter(config);
  fitter.run();

  cout << "----------------------------------------" << endl;
  cout << "signal fitting done" << endl;
  cout << "----------------------------------------" << endl;

  gROOT->cd();
}
//----------------------------------------------------------------------

