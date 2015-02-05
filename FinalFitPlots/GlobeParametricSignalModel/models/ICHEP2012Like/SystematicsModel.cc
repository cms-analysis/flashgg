#include "SystematicsModel.h"


#include <RooGaussian.h>
#include <RooFormulaVar.h>
#include <RooConstVar.h>
#include <RooExtendPdf.h>


#include "FitParameterSystematics.h"

#ifndef __CINT__
#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>
using namespace boost::assign;

#include <boost/algorithm/string/predicate.hpp>

#endif

#include <utils.h>

using namespace std;
using namespace ICHEP2012LikeModel;

//----------------------------------------------------------------------
SystematicsModel::SystematicsModel(const Config &_config,
    const std::vector<std::string> &_catnames, RooRealVar *_mnom,
    const std::vector<std::string> &_inputSigProcessNames,
    // std::map<unsigned, std::map<std::string, RooHistFunc *> > &_effaccfuncs,
    const SignalModelParameterInterpolatorMap &_effAccs,
    const SignalModelParameterInterpolatorMap &_fitparmfuncs,
    RooRealVar *_hmass
) :
  config(_config),
  catnames(_catnames),
  nominalMass(_mnom),
  inputSigProcessNames(_inputSigProcessNames),
  effAccs(_effAccs),
  fitparmfuncs(_fitparmfuncs),
  hmass(_hmass)
{
  fitParameterSystematics = new FitParameterSystematics(config, catnames, nominalMass);

  // run some calculations
  step1();
  step2();

  BOOST_FOREACH(string inputSigProcName, inputSigProcessNames)
  {
    for (unsigned cat = 0; cat < catnames.size(); ++cat)
    {
      nuisancedEffAccs[catnames.at(cat)][inputSigProcName] = step3MakeEffAccWithLumiAndXsectNuisance(cat, inputSigProcName);
    } // loop over categories

  } // loop over unmerged signal process names

}
//----------------------------------------------------------------------

RooAddPdf*
SystematicsModel::makeNuisancedSumOfGaussians(const std::string &title,
    const std::string &catname,
    const std::string &catPrefix,
    const std::string &inputSigProcName,
    map<std::string, map<unsigned, RooFormulaVar *> > &meanWithNuisance,
    map<std::string, map<unsigned, RooFormulaVar *> > &sigmaWithNuisance,
    SignalModelParameterInterpolator *paramFuncs)
{
    map<unsigned, RooGaussian *> gSlides;
    RooArgList components;
    for (unsigned i = 1;; ++i)
    {
      if (meanWithNuisance[catname].count(i) == 0)
        break;

      cout << "FOUND NUISANCE FOR " << i << endl;

      // create Gaussian component from parameters which have nuisances included
      // TODO: IT IS IMPORTANT TO ALSO USE RIGHT/WRONG PREFIX IN THIS FUNCTION'S
      //       NAME ! OTHERWISE, WE'LL SHARE IT WITH THE RIGHT VERTEX
      gSlides[i] = new RooGaussian(TString("g" + toStr(i) + "slide") + catPrefix + "_" + inputSigProcName,
          "",
          *hmass, *meanWithNuisance[catname][i], *sigmaWithNuisance[catname][i]);
      components.add(*gSlides[i]);
    }

    RooArgList fractions;
    for (unsigned i = 0; i < components.getSize() - 1; ++i)
      fractions.add(*paramFuncs->getFunction(catPrefix + "f" + toStr(i + 1)));

  RooAddPdf *combSlides = new RooAddPdf((title + catname + "_" + inputSigProcName).c_str(), "",
      components,
      fractions, kTRUE);

  return combSlides;
}

//----------------------------------------------------------------------

void 
SystematicsModel::step1()
{
  // index is the category number

  // nuissanceDeltaSmears = new RooRealVar*[catnames.size()];      // nuisance parameter

  for (UInt_t icat = 0; icat < catnames.size(); ++icat)
  {
    string catname = catnames.at(icat);
    string catPrefix = catname + "_";

    //----------

    BOOST_FOREACH(string inputSigProcName, inputSigProcessNames)
    {

      // we take the fit parameters (apart from the normalization which is NOT fitted)
      // from the merged processes while building everything else
      // per input process name
      string mergedSigProcName = this->config.globalConfig->signalProcessMergingMapping.at(inputSigProcName);

      SignalModelParameterInterpolator *paramFuncs = fitparmfuncs.get(catname, mergedSigProcName);

      //--------------------
      // fitted delta means (w.r.t to generated Higgs mass), depending on nuisance parameter                                                          fitted delta mean                 nuisance parameter
      //--------------------
      fitParameterSystematics->makeNuisancedMeans(catname, catPrefix, inputSigProcName, paramFuncs);

      //--------------------
      // resolution smearing uncertainties
      // sqrt[ (fitted sigma)^2 - (mHgen * smears)^2 + smearmods^2 ]
      //--------------------
      fitParameterSystematics->makeNuisancedSigmas(icat, catname, catPrefix, inputSigProcName, paramFuncs);

      //--------------------

      effAccFunctions[catname][inputSigProcName] = effAccs.get(catname, inputSigProcName)->getFunction(0);



    } // loop over signal processes
  } // loop over categories
}

//----------------------------------------------------------------------
RooAbsReal*
SystematicsModel::step3MakeEffAccWithLumiAndXsectNuisance(unsigned cat, const string &inputSigProcName)
{
  // simpler treatment for ICHEP2012: no barrel/endcap and r9 migration
  //
  // taken from BasicModel01 step3MakeEffAccWithLumiAndXsectNuisance
  // for the case of the exclusive categories
  
  RooHistFunc *effAccFunc = effAccs.get(catnames.at(cat), inputSigProcName)->getFunction("effacc");
  return effAccFunc;

  // if (cat < config.numInclusiveCategories)
  // inclusive category
  // return step3MakeNsigCatInclusiveCategories(cat, inputSigProcName);

  // exclusive category, simpler treatment for the moment
  // exclusive categories
  // RooHistFunc *effAccFunc = effAccs.get(catnames.at(cat), inputSigProcName)->getFunction("effacc");
  // return effAccFunc;
}

//----------------------------------------------------------------------

void 
SystematicsModel::step2()
{
  //-----
  // IF THESE ARE *NOT* DECLARED AS POINTERS (BUT AS LOCAL VARIABLES), WE WILL
  // HAVE SEGMENTATION FAULTS AT FUNNY PLACES LATER !

  //-----
  // these (for the moment) do NOT depend on the signal production mechanism
  // sum over all production mechanism normalizations

  // Note that this adds up all finalnormslides functions
  // 4 categories: cats 0 and 1 are barrel
  // 8 categories: cats 0,1,4,5

  if (config.numInclusiveCategories == 4)
  {
    // cat 0 and 1 (barrel)
    // cat 0 [BB, high R9] / (cat 0 [BB, high R9] + cat 1 [BB, low R9])
    // r9fracbarrel = makeFracVar("r9fracbarrel", *nuissancedeltar9fracbarrel, list_of("cat0"), list_of("cat1"));

    // r9fracmixed = makeFracVar("r9fracmixed", *nuissancedeltar9fracmixed, list_of("cat2"), list_of("cat3"));
    //      r9fracmixed = new RooFormulaVar("r9fracmixed", "", "@0*(@1+@2+@3+@4)/(@1+@2+@3+@4 + @5+@6+@7+@8)", RooArgList(nuissancedeltar9fracmixed,
    //          // cat 2
    //          *finalnormslides[2][sigProcessNames[0]], *finalnormslides[2][sigProcessNames[1]], *finalnormslides[2][sigProcessNames[2]], *finalnormslides[2][sigProcessNames[3]],
    //
    //          // cat 3
    //          *finalnormslides[3][sigProcessNames[0]], *finalnormslides[3][sigProcessNames[1]], *finalnormslides[3][sigProcessNames[2]], *finalnormslides[3][sigProcessNames[3]]));

    // there is no split by pT here
    // ptfracbarrelhighr9 = new RooConstVar("ptfracbarrelhighr9", "", 1.0);
    // ptfracbarrelmixedr9 = new RooConstVar("ptfracbarrelmixedr9", "", 1.0);
    // ptfracmixedhighr9 = new RooConstVar("ptfracmixedhighr9", "", 1.0);
    // ptfracmixedmixedr9 = new RooConstVar("ptfracmixedmixedr9", "", 1.0);

    // ptfrac_B_barrelhighr9 = new RooConstVar("ptfrac_B_barrelhighr9", "", 0.0);
    // ptfrac_B_barrelmixedr9 = new RooConstVar("ptfrac_B_barrelmixedr9", "", 0.0);
    // ptfrac_B_mixedhighr9 = new RooConstVar("ptfrac_B_mixedhighr9", "", 0.0);
    // ptfrac_B_mixedmixedr9 = new RooConstVar("ptfrac_B_mixedmixedr9", "", 0.0);
  }
  else if (config.numInclusiveCategories == 8)
  {
    // build the quantities we want to use for systematic uncertainties from the per-category efficiencies
    // determined above (a kind of 'base transformation')

    /*
      r9fracbarrel = new RooFormulaVar("r9fracbarrel","","@0 * (@1 + @2)/(@1 +@2 +@3 + @4)",RooArgList(nuissancedeltar9fracbarrel,
            *finalnormslides[0],
            *finalnormslides[4],
            *finalnormslides[1],
            *finalnormslides[5]) );
    */
    // r9fracbarrel = makeFracVar("r9fracbarrel", *nuissancedeltar9fracbarrel, list_of("cat0")("cat4"), list_of("cat1")("cat5"));

    /*      r9fracmixed  = new RooFormulaVar("r9fracmixed", "","@0 * (@1 + @2)/(@1 +@2 +@3 + @4)",RooArgList(nuissancedeltar9fracmixed,
                *finalnormslides[2],
                *finalnormslides[6],
                *finalnormslides[3],
                *finalnormslides[7]));
    */
    // r9fracmixed = makeFracVar("r9fracmixed", *nuissancedeltar9fracmixed, list_of("cat2")("cat6"), list_of("cat3")("cat7"));

    // pt fraction variables (separate per (Barrel/Endcap categories) x (R9 categories) )
    /*
    ptfrac_barrel_highr9  = new RooFormulaVar("ptfracbarrelhighr9","","@0*@1/(@1+@2)", RooArgList(nuissancehighptfrac,
                                       *finalnormslides[0],*finalnormslides[4]));
    */
    //                                                                                        BB high R9 high pT
    //                                                                                                       BB high R9 low pT
    // ptfracbarrelhighr9 = makeFracVar("ptfracbarrelhighr9", *nuissancehighptfrac, list_of("cat0"), list_of("cat4"));

    /*      ptfrac_barrel_mixedr9 = new RooFormulaVar("ptfracbarrelmixedr9","","@0*@1/(@1+@2)",RooArgList(nuissancehighptfrac,
                                    *finalnormslides[1],*finalnormslides[5])); */
    //                                                                                        BB low R9 high pT
    //                                                                                                       BB low R9 low pT
    // ptfracbarrelmixedr9 = makeFracVar("ptfracbarrelmixedr9", *nuissancehighptfrac, list_of("cat1"), list_of("cat5"));

    /*      ptfrac_mixed_highr9   = new RooFormulaVar("ptfracmixedhighr9","","@0*@1/(@1+@2)",  RooArgList(nuissancehighptfrac,
                 *finalnormslides[2],*finalnormslides[6])); */
    //                                                                                        BE high R9 high pT
    //                                                                                                       BE high R9 low pT
    // ptfracmixedhighr9 = makeFracVar("ptfracmixedhighr9", *nuissancehighptfrac, list_of("cat2"), list_of("cat6"));

    /*      ptfrac_mixed_mixedr9  = new RooFormulaVar("ptfracmixedmixedr9","","@0*@1/(@1+@2)", RooArgList(nuissancehighptfrac,
             *finalnormslides[3],*finalnormslides[7])); */
    //                                                                                        BE low R9 high pT
    //                                                                                                       BE low R9 low pT
    // ptfracmixedmixedr9 = makeFracVar("ptfracmixedmixedr9", *nuissancehighptfrac, list_of("cat3"), list_of("cat7"));

    // ptfrac_B_barrelhighr9 = new RooConstVar("ptfrac_B_barrelhighr9", "", 0.0);
    // ptfrac_B_barrelmixedr9 = new RooConstVar("ptfrac_B_barrelmixedr9", "", 0.0);
    // ptfrac_B_mixedhighr9 = new RooConstVar("ptfrac_B_mixedhighr9", "", 0.0);
    // ptfrac_B_mixedmixedr9 = new RooConstVar("ptfrac_B_mixedmixedr9", "", 0.0);

  }
  else
  {
    cerr << "numInclusiveCategories = " << config.numInclusiveCategories << " is not yet supported here" << endl;
    exit(1);
  }


}
//----------------------------------------------------------------------

RooAbsReal*
SystematicsModel::getNuisancedEffAcc(const std::string &catname, const string &inputSigProcName)
{
  return this->nuisancedEffAccs[catname][inputSigProcName];
}

//----------------------------------------------------------------------
RooAbsReal*
SystematicsModel::getNuisancedParameter(const std::string &catname, const std::string &inputSigProcName, const std::string &paramName)
{
  //--------------------
  // deltaMx
  //--------------------
  if (boost::starts_with(paramName,"dm"))
  {
    // TODO: why does this NOT depend on the signal process ?
    unsigned number = boost::lexical_cast<unsigned>(paramName.substr(2));

    ASSERTBREAK(this->fitParameterSystematics->deltaMwithNuisance[catname][inputSigProcName].count(number) > 0);
    return this->fitParameterSystematics->deltaMwithNuisance[catname][inputSigProcName][number];
  }

  //--------------------
  // deltaMx (wrong vertex)
  //--------------------
  else if (boost::starts_with(paramName,"wdm"))
  {
    // TODO: why does this NOT depend on the signal process ?
    unsigned number = boost::lexical_cast<unsigned>(paramName.substr(3));

    ASSERTBREAK(this->fitParameterSystematics->wrongDeltaMwithNuisance[catname][inputSigProcName].count(number) > 0);
    return this->fitParameterSystematics->wrongDeltaMwithNuisance[catname][inputSigProcName][number];
  }

  //--------------------
  // sigmaX
  //--------------------
  else if (boost::starts_with(paramName,"sigma"))
  {
    // TODO: why does this NOT depend on the signal process ?
    unsigned number = boost::lexical_cast<unsigned>(paramName.substr(5));

    ASSERTBREAK(this->fitParameterSystematics->sigmaWithNuisance[catname][inputSigProcName].count(number) > 0);
    return this->fitParameterSystematics->sigmaWithNuisance[catname][inputSigProcName][number];
  }

  //--------------------
  // sigmaX (wrong vertex)
  //--------------------
  else if (boost::starts_with(paramName,"wsigma"))
  {
    // TODO: why does this NOT depend on the signal process ?
    unsigned number = boost::lexical_cast<unsigned>(paramName.substr(6));

    ASSERTBREAK(this->fitParameterSystematics->wrongSigmaWithNuisance[catname][inputSigProcName].count(number) > 0);
    return this->fitParameterSystematics->wrongSigmaWithNuisance[catname][inputSigProcName][number];
  }

  //--------------------
  // fx / wfx (wrong vertex)
  //--------------------
  else if (boost::starts_with(paramName,"f") ||
           boost::starts_with(paramName,"wf"))
  {
    // looks like  there was no nuisance added to these fractions
    // in the original model, so return the original (fitted)
    // function

    // get the merged signal process name (we fitted the merged processes)
    string mergedSigProcName = this->config.globalConfig->signalProcessMergingMapping.at(inputSigProcName);

    return this->fitparmfuncs.get(catname, mergedSigProcName)->getFunction(catname + "_" + paramName);
  }
  else
  {
  cerr << "TRYING TO GET NUISANCED PARAMETER '" << paramName << "'" << endl;
  //  gSlides[i] = new RooGaussian(TString("g" + toStr(i) + "slide") + catPrefix + "_" + inputSigProcName,
//      "",
//      *hmass, *meanWithNuisance[catname][i], *sigmaWithNuisance[catname][i]);
//  components.add(*gSlides[i]);
//}
//
//RooArgList fractions;
//for (unsigned i = 0; i < components.getSize() - 1; ++i)
//  fractions.add(*paramFuncs->getFunction(catPrefix + "f" + toStr(i + 1)));

  // not implemented yet
  ASSERTBREAK(0);

  return NULL;
  }
}

//----------------------------------------------------------------------
std::vector<RooAbsReal *> 
SystematicsModel::getAllNuisanceParameters()
{
  // TODO: do we take all here ?
  std::vector<RooAbsReal *> retval;

  // retval.push_back(nuissancedeltaeffaccbarrel);
  // retval.push_back(nuissancedeltaeffaccmixed);
  // retval.push_back(nuissancedeltar9fracbarrel);
  // retval.push_back(nuissancedeltar9fracmixed);
  // retval.push_back(nuissancehighptfrac);

  for (map<std::string, RooRealVar *>::iterator it = fitParameterSystematics->deltaSmearNuisance.begin();
       it != fitParameterSystematics->deltaSmearNuisance.end();
       ++it
       )
    {
      retval.push_back(it->second);
    }

//  for (map<std::string, RooRealVar *>::iterator it = fitParameterSystematics->nuissanceDeltaMs.begin();
//       it != fitParameterSystematics->nuissanceDeltaMs.end();
//       ++it
//       )
//    {
//      retval.push_back(it->second);
//    }
//
  retval.push_back(fitParameterSystematics->globalScaleNuisance);
  
  return retval;
}

//----------------------------------------------------------------------

