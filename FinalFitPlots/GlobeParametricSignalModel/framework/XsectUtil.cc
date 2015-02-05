#include "XsectUtil.h"

#include "XsectDataDetailed.C"
#include "ReportMaker.h"

#include "utils.h"

#include <TH1.h>
#include <TCanvas.h>

#include <RooRealVar.h>
#include <RooHistFunc.h>
#include <RooDataHist.h>
#include <RooPlot.h>
#include <RooWorkspace.h>

#include <iostream>

#include <boost/regex.hpp>
#include <boost/foreach.hpp>

#include <boost/assign/list_of.hpp>
using namespace boost::assign;

using namespace std;

//----------------------------------------------------------------------

XsectUtil*
XsectUtil::fromXsectCode(int sqrts, bool fermiophobic)
{

  unsigned numXsectPoints;
  float *xsectMasses;
  map<string, float *> sm_xs_br;
  map<string, float *> sm_xs;
  float *sm_br;

  if (sqrts == 7)
  {
    if (! fermiophobic)
      {
        numXsectPoints = SMCrossSections::Sqrts7TeV::numPoints;
        xsectMasses    = SMCrossSections::Sqrts7TeV::masses;

        sm_xs_br["ggh"] = SMCrossSections::Sqrts7TeV::xs_br_ggh;
        sm_xs_br["vbf"] = SMCrossSections::Sqrts7TeV::xs_br_vbf;
        sm_xs_br["wzh"] = SMCrossSections::Sqrts7TeV::xs_br_wzh;
        sm_xs_br["tth"] = SMCrossSections::Sqrts7TeV::xs_br_tth;
        sm_xs_br["sum"] = SMCrossSections::Sqrts7TeV::xs_br_sum;

        sm_xs["ggh"] = SMCrossSections::Sqrts7TeV::xs_ggh;
        sm_xs["vbf"] = SMCrossSections::Sqrts7TeV::xs_vbf;
        sm_xs["wzh"] = SMCrossSections::Sqrts7TeV::xs_wzh;
        sm_xs["tth"] = SMCrossSections::Sqrts7TeV::xs_tth;
        sm_xs["sum"] = SMCrossSections::Sqrts7TeV::xs_sum;

        sm_br = SMCrossSections::Sqrts7TeV::br;
      }
    else
      {
        numXsectPoints = FPCrossSections::Sqrts7TeV::numPoints;
        xsectMasses    = FPCrossSections::Sqrts7TeV::masses;

        sm_xs_br["vbf"] = FPCrossSections::Sqrts7TeV::xs_br_vbf;
        sm_xs_br["wzh"] = FPCrossSections::Sqrts7TeV::xs_br_wzh;
        sm_xs_br["sum"] = FPCrossSections::Sqrts7TeV::xs_br_sum;

        sm_xs["vbf"] = FPCrossSections::Sqrts7TeV::xs_vbf;
        sm_xs["wzh"] = FPCrossSections::Sqrts7TeV::xs_wzh;
        sm_xs["sum"] = FPCrossSections::Sqrts7TeV::xs_sum;

        sm_br = FPCrossSections::Sqrts7TeV::br;
      }
  }
  else if (sqrts == 8)
  {
    if (! fermiophobic)
      {
        numXsectPoints = SMCrossSections::Sqrts8TeV::numPoints;
        xsectMasses = SMCrossSections::Sqrts8TeV::masses;

        sm_xs_br["ggh"] = SMCrossSections::Sqrts8TeV::xs_br_ggh;
        sm_xs_br["vbf"] = SMCrossSections::Sqrts8TeV::xs_br_vbf;
        sm_xs_br["wzh"] = SMCrossSections::Sqrts8TeV::xs_br_wzh;
        sm_xs_br["tth"] = SMCrossSections::Sqrts8TeV::xs_br_tth;
        sm_xs_br["sum"] = SMCrossSections::Sqrts8TeV::xs_br_sum;

        sm_xs["ggh"] = SMCrossSections::Sqrts8TeV::xs_ggh;
        sm_xs["vbf"] = SMCrossSections::Sqrts8TeV::xs_vbf;
        sm_xs["wzh"] = SMCrossSections::Sqrts8TeV::xs_wzh;
        sm_xs["tth"] = SMCrossSections::Sqrts8TeV::xs_tth;
        sm_xs["sum"] = SMCrossSections::Sqrts8TeV::xs_sum;

        sm_br = SMCrossSections::Sqrts8TeV::br;
      }
    else
      {
        numXsectPoints = FPCrossSections::Sqrts8TeV::numPoints;
        xsectMasses = FPCrossSections::Sqrts8TeV::masses;

        sm_xs_br["vbf"] = FPCrossSections::Sqrts8TeV::xs_br_vbf;
        sm_xs_br["wzh"] = FPCrossSections::Sqrts8TeV::xs_br_wzh;
        sm_xs_br["sum"] = FPCrossSections::Sqrts8TeV::xs_br_sum;

        sm_xs["vbf"] = FPCrossSections::Sqrts8TeV::xs_vbf;
        sm_xs["wzh"] = FPCrossSections::Sqrts8TeV::xs_wzh;
        sm_xs["sum"] = FPCrossSections::Sqrts8TeV::xs_sum;

        sm_br = FPCrossSections::Sqrts8TeV::br;
      }
  }
  else
  {
    cerr << "cross sections for sqrts = " << sqrts << " TeV are not implemented" << endl;
    exit(1);
  }

  // fill values into an XsectUtil object
  XsectUtil *retval = new XsectUtil;

  //----------

  bool isFirstProc = true;
  for (map<string, float *>::iterator it = sm_xs_br.begin();
      it != sm_xs_br.end(); ++it)
  {
    const string &proc = it->first;

    for (unsigned ipoint = 0; ipoint < numXsectPoints; ++ipoint)
    {
      float mass = xsectMasses[ipoint];

      retval->sm_xs_br[proc][mass] = (it->second)[ipoint];

      retval->sm_xs[proc][mass] = (sm_xs[proc])[ipoint];

      if (isFirstProc)
        retval->sm_br[mass] = sm_br[ipoint];

    } // loop over all mass points

    isFirstProc = false;

  } // loop over all processes

  //----------

  retval->canMakeHistogramFunc = true;

  return retval;
}
//----------------------------------------------------------------------
XsectUtil *
XsectUtil::fromInputWorkspace(RooWorkspace *ws, bool fermiophobic)
{
  XsectUtil *retval = new XsectUtil;
  retval->canMakeHistogramFunc = false;

  list<string> inputSigProcessNames;

  string prefix = "XSBR_";
  if (fermiophobic)
  {
    prefix = "ff_" + fermiophobic;
    inputSigProcessNames = list_of("vbf")("wzh");
  }
  else
  {
    inputSigProcessNames = list_of("ggh")("vbf")("wzh")("tth");
  }

  // loop over all variables of the workspace and see whether they
  // match the given pattern
  const boost::regex pattern(prefix + "(\\S+)_(\\d+)");

  RooArgSet allVars = ws->allVars();
  RooLinkedListIter it = allVars.iterator();
  RooRealVar *var;
  while ((var = (RooRealVar*)(it.Next())) != NULL)
  {
    string name = var->GetName();

    // check whether this matches the expected pattern
    boost::cmatch mo;

    if (! boost::regex_match(name.c_str(), mo, pattern))
      continue;

    string sigProc = mo[1];

    if (std::find(inputSigProcessNames.begin(), inputSigProcessNames.end(), sigProc) == inputSigProcessNames.end())
      continue;

    int mass = boost::lexical_cast<unsigned>(mo[2]);
    retval->sm_xs_br[sigProc][mass] = var->getVal();

    // also update the sum over all processes
    map<float, float, FloatCompare> &m = retval->sm_xs_br["sum"];
    if (m.find(mass) == m.end())
      m[mass] = var->getVal();
    else
      m[mass] += var->getVal();

  } // end of loop over input variables

  return retval;
}



//----------------------------------------------------------------------

//double
//getTotalCrossSectionFromInputFile(int mass)
//{
//  std::stringstream numstringstr;
//  numstringstr << mass;
//  TString numstring(numstringstr.str());
//
//  double totalCrossSectionValue = 0;
//
//  BOOST_FOREACH(string sigProcName, inputSigProcessNames)
//  {
//    // cross section times BR variable
//    RooRealVar *mtotalxsec = NULL;
//
//    if (!config.fermiophobic)
//      // standard model
//      mtotalxsec = win->var(TString("XSBR_") + sigProcName + "_" + numstring);
//    else
//      // fermiophobic cross section times BR
//      mtotalxsec = win->var(TString("ff_XSBR_") + sigProcName + "_" + numstring);
//
//    if (mtotalxsec == NULL)
//      cerr << "WARNING: cross section does not exist for process " + sigProcName + " and mass " + numstring << endl;
//    ASSERT(mtotalxsec != NULL);
//
//    totalCrossSectionValue += mtotalxsec->getVal();
//
//  } // loop over signal Feynman diagrams
//
//  return totalCrossSectionValue;
//}

//----------------------------------------------------------------------
TH1F *
XsectUtil::makeCrossSectionHistogram(const std::string &sigProcName)
{
  TH1F *crossSectionHistogram = new TH1F(Form("hsmxsecs_%s", sigProcName.c_str()), "", 81, 109.75, 150.25);

  const map<float, float, FloatCompare> &xs = this->sm_xs[sigProcName];

  for (map<float, float>::const_iterator it = xs.begin();
       it != xs.end();
       ++it)
  {
    double mass = it->first;
    double xsValue = it->second;

    //hsmxsecs->Fill(smmasses[ipoint],smxsecs[ipoint]*smbrs[ipoint]);

    // cout << "FILLING XSECT, fermiophobic=" << fermiophobic << endl;

    crossSectionHistogram->Fill(mass, xsValue);

    // FOUR GENERATIONS STANDARD MODEL
    //hsmxsecs->Fill(smmasses[ipoint],sm4xsbr[ipoint]);

  } // loop over all mass points

  return crossSectionHistogram;
}
//----------------------------------------------------------------------

RooHistFunc*
XsectUtil::makeCrossSectionFunctionUsingHistogram(const string &sigProcName, ReportMaker *reportMaker, RooRealVar *mnom)
{
  ASSERTBREAK(canMakeHistogramFunc);

  // histogram of cross section as function of the Higgs mass.
  TH1F *crossSectionHistogram = makeCrossSectionHistogram(sigProcName);
  RooDataHist *xsecsDataHist;

  xsecsDataHist = new RooDataHist(Form("dsmxsecs_%s", sigProcName.c_str()), "", RooArgList(*mnom), crossSectionHistogram);

  RooHistFunc *funcXsecNorm = new RooHistFunc(Form("fsmxsecs_%s", sigProcName.c_str()), "", RooArgList(*mnom), *xsecsDataHist, 1);
  //--------------------
  // plot the cross section times branching ratio
  //--------------------
  if (reportMaker != NULL)
  {
    TCanvas *csmxsecs = new TCanvas;
    RooPlot *plotsmxsecs = mnom->frame();

    xsecsDataHist->plotOn(plotsmxsecs);
    funcXsecNorm->plotOn(plotsmxsecs);

    plotsmxsecs->Draw();
    TString fname = Form("xsecs_%s", sigProcName.c_str());
    csmxsecs->SaveAs(Form("plots/%s.svg",fname.Data()));
    csmxsecs->SaveAs(Form("plots/%s.png",fname.Data()));

    reportMaker->addPlotSVG("xsects/" + sigProcName, (const char *)fname,"cross sections for process " + sigProcName);

  }

  return funcXsecNorm;
}

//----------------------------------------------------------------------
TH1F *
XsectUtil::makeBranchingRatioHistogram()
{
  TH1F *branchingRatioHistogram = new TH1F(Form("hsmbrs"), "", 81, 109.75, 150.25);

  for (map<float, float>::const_iterator it = sm_br.begin();
       it != sm_br.end();
       ++it)
  {
    double mass = it->first;
    double br = it->second;

    branchingRatioHistogram->Fill(mass, br);

  } // loop over all mass points

  return branchingRatioHistogram;
}
//----------------------------------------------------------------------

RooHistFunc*
XsectUtil::makeBranchingRatioFunctionUsingHistogram(ReportMaker *reportMaker, RooRealVar *mnom)
{
  ASSERTBREAK(canMakeHistogramFunc);

  // histogram of cross section as function of the Higgs mass.
  TH1F *branchingRatioHistogram = makeBranchingRatioHistogram();
  RooDataHist *brDataHist;

  brDataHist = new RooDataHist(Form("dsmBR"), "", RooArgList(*mnom), branchingRatioHistogram);

  // TODO: add sqrts name
  RooHistFunc *funcBR = new RooHistFunc(Form("fsmbrs"), "", RooArgList(*mnom), *brDataHist, 1);

  //--------------------
  // plot the branching ratio
  //--------------------
  if (reportMaker != NULL)
  {
    TCanvas *canvas = new TCanvas;
    RooPlot *plot = mnom->frame();

    brDataHist->plotOn(plot);
    funcBR->plotOn(plot);

    plot->Draw();
    TString fname = "branchingRatios";
    canvas->SaveAs(Form("plots/%s.svg",fname.Data()));
    canvas->SaveAs(Form("plots/%s.png",fname.Data()));

    reportMaker->addPlotSVG("branchinRatios", (const char *)fname,"branching ratios");

  }

  return funcBR;
}


//----------------------------------------------------------------------

double
XsectUtil::getSum(int mass)
{
  // TO BE IMPLEMENTED
  // ASSERTBREAK(0);

  // TODO: should check that this point is available
  return this->sm_xs_br["sum"][mass];
}


//----------------------------------------------------------------------

double
XsectUtil::get(int mass, const std::string &process)
{
  // TO BE IMPLEMENTED
  // ASSERTBREAK(0);

  // TODO: should check that this point is available
  return this->sm_xs_br[process][mass];
}


//----------------------------------------------------------------------
