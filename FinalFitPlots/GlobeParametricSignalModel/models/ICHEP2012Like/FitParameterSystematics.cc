#include "FitParameterSystematics.h"

#include <RooConstVar.h>
#include <RooRealVar.h>

#include <interpolation/SignalModelParameterInterpolator.h>
#include <utils.h>

using namespace std;
using namespace ICHEP2012LikeModel;

//----------------------------------------------------------------------
FitParameterSystematics::FitParameterSystematics(const Config &_config, const std::vector<string> &catnames,
    RooRealVar *_nominalMass) :
  config(_config),
  nominalMass(_nominalMass)
{
  for (unsigned icat = 0; icat < catnames.size(); ++icat)
  {
    string catname = catnames.at(icat);

    // a constant with the same value as in smearingv
    mcToDataMassSmearingVar[catname] = new RooConstVar(TString("smear") + catname, "", config.smearingv.at(catname));

    // these DO NOT depend on the signal process
    {
      deltaSmearNuisance[catname] = new RooRealVar(TString("CMS_hgg_nuissancedeltasmear") + catname, "", 0.0, - config.smearingv.at(catname), config.smearingv.at(catname));
      deltaSmearNuisance[catname]->setVal(0.0);
      deltaSmearNuisance[catname]->setConstant();

      // nuissanceDeltaMs[catname] = new RooRealVar(TString("CMS_hgg_nuissancedeltam") + catname, "", 0.0, -5.0, 5.0);
      // nuissanceDeltaMs[catname]->setConstant();

      globalScaleNuisance = new RooRealVar("CMS_hgg_globalscale","global energy scale nuisance ?", 0.0, -5, 5);
      globalScaleNuisance->setConstant();
    }

    smearmods[icat] = new RooFormulaVar(TString("smearmod") + catname, "",
    // mhGen * (mcToDataMassSmearingVar + nuissancedeltasmears)
        "@0 * (@1 + @2)", RooArgList(*nominalMass, *mcToDataMassSmearingVar[catname], *deltaSmearNuisance[catname]));

  } // end of loop over categories

}


//----------------------------------------------------------------------

void
FitParameterSystematics::makeNuisancedMeans(
    const std::string &catname,
    const std::string &catPrefix,
    const std::string &inputSigProcName,
    SignalModelParameterInterpolator *paramFuncs)
{
  // right vertex
  for (unsigned i = 1; ; ++i)
  {
    // depends on the number of Gaussians used
    if (! paramFuncs->hasParameter(catPrefix + "dm" + toStr(i)))
      break;

    deltaMwithNuisance[catname][inputSigProcName][i] = new RooFormulaVar(TString("mean" + toStr(i) + "slide") + catname + "_" + inputSigProcName, "",
        // "@0 + @1 + @0*@2", // with @0 = mnom, @1 = fitparmfuncs[icat][mergedSigProcName][0], @2 = nuissanceDeltaMs[icat])
        // "@0 * (1 + @2 ) + @1", // actual mean, not dm
        //"@0 * (1 + @2 )", // dm, not mean
        // "@0 + @1 + @0 * @2", // like in the official ICHEP2012 inputs
        "@1 + @0 * @2", // like in the official ICHEP2012 inputs but we need dm, not MH itself (otherwise, the combine will not converge because the mean of the Gaussian is at 2*MH)

        RooArgList(*nominalMass,
                   *paramFuncs->getFunction(catPrefix + "dm" + toStr(i)),

                   // before ICHEP2012
                   // *nuissanceDeltaMs[catname]

                   // for ICHEP2012: use a global scale nuisance: CHECK: is this absolute or relative ?
                   *globalScaleNuisance
                   ));

  }

  // wrong vertex
  for (unsigned i = 1; ; ++i)
  {
    // depends on the number of Gaussians used
    if (!paramFuncs->hasParameter(catPrefix + "wdm" + toStr(i)))
      break;

    if (config.globalConfig->useRightWrongVertex)
      wrongDeltaMwithNuisance[catname][inputSigProcName][i] = new RooFormulaVar(TString("wmean" + toStr(i) + "slide") + catname + "_" + inputSigProcName, "",
          // "@0 + @1 + @0*@2",
          // "@0 * (1 + @2 )+ @1",// actual mean, not dm
          // "@0 * (1 + @2 )", // dm, not mean
                                                              
          "@1 + @0 * @2", // see above (right vertex)

          RooArgList(*nominalMass,
                     *paramFuncs->getFunction(catPrefix + "wdm" + toStr(i)),

                     // before ICHEP2012: per category (uncorrelated) uncertainties
                     // *nuissanceDeltaMs[catname]
                     *globalScaleNuisance
                                       )
                     );
    else
      wrongDeltaMwithNuisance[catname][inputSigProcName][i] = NULL;
  }

}

//----------------------------------------------------------------------

void
FitParameterSystematics::makeNuisancedSigmas(unsigned icat,
    const std::string &catname,
        const std::string &catPrefix,
        const std::string &inputSigProcName,
        SignalModelParameterInterpolator *paramFuncs)
{
  // right vertex
  for (unsigned i = 1; ; ++i)
  {
    if (! paramFuncs->hasParameter(catPrefix + "sigma" + toStr(i)))
      break;

      sigmaWithNuisance[catname][inputSigProcName][i] = new RooFormulaVar(TString("sigma" + toStr(i) + "slide") + catname + "_" + inputSigProcName, "",
          //                    fitted sigma
          //                           nominal mass
          //
          //                                       smearmods
          "TMath::Max(0.01,sqrt(@0*@0-@3*@3*@2*@2 +@1*@1))",
          RooArgList(*paramFuncs->getFunction(catPrefix + "sigma" + toStr(i)),
                     *smearmods[icat],
                     *mcToDataMassSmearingVar[catname],
                     *nominalMass));
  }

  // wrong vertex

  for (unsigned i = 1; ; ++i)
  {
    if (! paramFuncs->hasParameter(catPrefix + "wsigma" + toStr(i)))
      break;

    if (config.globalConfig->useRightWrongVertex)
      wrongSigmaWithNuisance[catname][inputSigProcName][i] = new RooFormulaVar(TString("wsigma" + toStr(i) + "slide") + catname + "_" + inputSigProcName, "",
          "TMath::Max(0.01,sqrt(@0*@0-@3*@3*@2*@2 +@1*@1))",
          RooArgList(*paramFuncs->getFunction(catPrefix + "wsigma" + toStr(i)),
                     *smearmods[icat],
                     *mcToDataMassSmearingVar[catname],
                     *nominalMass));
    else
      wrongSigmaWithNuisance[catname][inputSigProcName][i] = NULL;
  }
}

//----------------------------------------------------------------------
