#include "DataSetList.h"

#include <map>
#include <string>

#include "utils.h"

#include <boost/foreach.hpp>

using namespace std;

//----------------------------------------------------------------------
DataSetList::DataSetList(RooWorkspace *win_, ParametricSignalModelConfig &config_) :
    win(win_),
    config(config_)
{

}

//----------------------------------------------------------------------

RooDataSet *
createWeightedDataset(RooDataSet *indata, const std::vector<RooRealVar *> &mvars, RooRealVar *wvar, TString name, Double_t weightscale)
{
  ASSERT(indata != NULL);

  RooArgList args, argsWithoutWeight;
  BOOST_FOREACH(RooRealVar *mvar, mvars)
  {
    args.add(*mvar);
    argsWithoutWeight.add(*mvar);
  }

  args.add(*wvar);

  RooDataSet *outdata = new RooDataSet(name, "", args, wvar->GetName());

  for (Int_t ient = 0; ient < indata->numEntries(); ++ient)
  {
    // get the argset of the ient'th event/entry
    const RooArgSet *ent = indata->get(ient);

    BOOST_FOREACH(RooRealVar * mvar, mvars)
    {

      // set the value of mvar to the value of this entry in indata
      double value = static_cast<RooAbsReal*>(ent->find(mvar->GetName()))->getVal();

#warning HARDWIRED VALUES HERE
      if (value >= 100 && value <= 160)
        mvar->setVal(value);

    } // loop over all variables

    // add the value of the weight
    outdata->add(argsWithoutWeight, weightscale * indata->weight());

    } // loop over the entries of the input dataset

  return outdata;
}


//----------------------------------------------------------------------

RooDataSet *
appendWeightedDataset(RooDataSet *outdata, RooDataSet *indata, const vector<RooRealVar *> &mvars, RooRealVar *wvar, Double_t weightscale)
{
  ASSERT(indata != NULL);

  RooArgSet args, argsWithoutWeight;
  BOOST_FOREACH(RooRealVar *mvar, mvars)
  {
    args.add(*mvar);
    argsWithoutWeight.add(*mvar);
  }

  //RooDataSet *outdata = new RooDataSet(name,"",RooArgList(*mvar,*wvar),wvar->GetName());
  for (Int_t ient = 0; ient < indata->numEntries(); ++ient)
  {
    const RooArgSet *ent = indata->get(ient);

    BOOST_FOREACH(RooRealVar *mvar, mvars)
    {

    double value = static_cast<RooAbsReal*> (ent->find(mvar->GetName()))->getVal();

#warning HARDWIRED VALUES HERE
    if (value >= 100 && value <= 160)
      mvar->setVal(value);
    }

    outdata->add(argsWithoutWeight, weightscale * indata->weight());
  } // loop over the entries of the input dataset

  return outdata;
}
//----------------------------------------------------------------------

RooDataSet *
DataSetList::createOrAppendToDataset(RooDataSet *dataToAppend, const std::vector<RooRealVar *>& vars, RooRealVar *weightVar, const string &datasetName, Double_t weightscale)
  {
    // check whether the given dataset already exists or not
    map<string, RooDataSet *>::const_iterator it = mergedSignalDataSets.find(datasetName);
    if (it == mergedSignalDataSets.end())
    {
      // dataset not found, create a new one
      RooDataSet *retval = createWeightedDataset(dataToAppend, vars, weightVar, datasetName, weightscale);
      mergedSignalDataSets[datasetName] = retval;
      return retval;
    }
    else
      // dataset found, append to it
      return appendWeightedDataset(it->second, dataToAppend, vars, weightVar, weightscale);
  }

//----------------------------------------------------------------------

RooDataSet *
DataSetList::getMergedWeightedDataSet(const string &datasetName)
{
  map<string, RooDataSet *>::const_iterator it = mergedSignalDataSets.find(datasetName);
  if (it == mergedSignalDataSets.end())
    {
      cerr << "dataset '" << datasetName << "' does not exist in internal map" << endl;

      // for debugging only:
      cerr << "I only know about the following " << mergedSignalDataSets.size() << " datasets:" << endl;
      for (it = mergedSignalDataSets.begin(); it != mergedSignalDataSets.end(); ++it)
        cerr << "  " << it->first << endl;

      exit(1);
    }

  return it->second;
}

//----------------------------------------------------------------------

RooDataSet *
DataSetList::getDataSet(const string &name)
{
  RooDataSet *retval = (RooDataSet*) win->data(name.c_str());
  if (retval == NULL)
    {
      cerr << "dataset '" << name << "' does not exist in workspace '" << config.inputWorkspaceName << "' in file " << config.inputFname << endl;
      exit(1);
    }

  return retval;
}

//----------------------------------------------------------------------

void
DataSetList::makeWeightedDataSets(RooRealVar *weight, const std::vector<RooRealVar *> &vars, RooAbsPdf *allpdf,
    std::vector<std::string> &catnames,
    std::vector<std::string> &inputSigProcessNames,
    std::vector<int> &mhs)
{
  RooDataSet *mcsigdata, *mcsigwrongdata, // before weighting
      *mcsigwdata, *mcsigwrongwdata; // weighted

  // loop over categories for fits
  BOOST_FOREACH(string catname, catnames)
  {

    // loop over input process names but potentially merge to output process names
    BOOST_FOREACH(string inputSigProcName, inputSigProcessNames)
    // for (unsigned sigProcessIndex = 0; sigProcessIndex < sigProcessNames.size(); ++sigProcessIndex)
    {
      // apply the mapping between input signal process names and output
      // signal process names
      ASSERT(config.signalProcessMergingMapping.find(inputSigProcName) != config.signalProcessMergingMapping.end());
      string outputSigProcName = config.signalProcessMergingMapping[inputSigProcName];
      
      // loop over mass points for fits
      for (UInt_t i = 0; i < mhs.size(); ++i)
	{
	  std::stringstream numstringstr;
	  numstringstr << mhs.at(i);
	  string numstring(numstringstr.str());
	  
	  if (config.useRightWrongVertex)
	    {
	      //----------
	      //right vertex
	      string name = "sig_" + inputSigProcName + "_mass_m" + numstring + "_rv_" + catname;
	      mcsigdata = this->getDataSet(name);
	      
	      mcsigwdata = this->createOrAppendToDataset(mcsigdata, vars, weight, "mcsigwdata" + numstring + catname + "_" + outputSigProcName, config.weightscale);
	      
	      //----------
	      //wrong vertex
	      name = "sig_" + inputSigProcName + "_mass_m" + numstring + "_wv_" + catname;
	      mcsigwrongdata = this->getDataSet(name);
	      
	      mcsigwrongwdata = this->createOrAppendToDataset(mcsigwrongdata, vars, weight, "mcsigwrongwdata" + numstring + catname + "_" + outputSigProcName, config.weightscale);
	      
	    }
	  else
	    {
	      // for the sake of simplicity, just assume ALL signal events
	      // have the correctly assigned vertex
	      
	      string name = "sig_" + inputSigProcName + "_mass_m" + numstring + "_" + catname;
	      mcsigdata = this->getDataSet(name);
	      
	      mcsigwdata = this->createOrAppendToDataset(mcsigdata, vars, weight, "mcsigwdata" + numstring + catname + "_" + outputSigProcName, config.weightscale);
	    } // if no distinction between right and wrong vertex
	  
              //----------
              //combined (right plus wrong vertex)
	  
              // for IC inputs: loop over different signal Feynman diagrams
              // this is just used for plotting apparently..
	  RooDataSet *mcsigallwdata = NULL;
	  // for (unsigned sigProcessIndex = 0; sigProcessIndex < sigProcessNames.size(); ++sigProcessIndex)
	  {
	    // string sigProcName = sigProcessNames[sigProcessIndex];
	    
	    string name = "sig_" + inputSigProcName + "_mass_m" + numstring + "_" + catname;
	    RooDataSet *mcsigalldata = this->getDataSet(name);
	    
	    mcsigallwdata = this->createOrAppendToDataset(mcsigalldata, vars, weight, "mcsigallwdata" + numstring + catname, config.weightscale);
	    
	  } // loop over signal Feynman diagrams
	  
	  //----------
	  
	  //track some test datasets for later tests
#ifdef TEST_SINGLE_MASS_POINT
	  if (mhs.at(i)==115)
	    {
	      testdsets.push_back(mcsigwdata);
	      testdsetswrong.push_back(mcsigwrongwdata);
	      testdsetsall.push_back(mcsigallwdata);
	      
	      //continue;
	    }
#endif
	  
#if 0
	  // commented out for the moment
	  //--------------------
	  // plotting
	  //--------------------
	  if (allpdf != NULL)
	    {
	      TCanvas *chfitall = new TCanvas;
	      TString plotnameall = TString("allvtx") + numstring + catname + "_" + sigProcName + TString(".png");
	      RooPlot *hplotall = hmass->frame(Bins(100), Range("plotrange"));
	      mcsigallwdata->plotOn(hplotall);
	      allpdf->plotOn(hplotall, RooFit::LineColor(kBlue), Range("higgsrange"), NormRange("higgsrange"));
	      hplotall->SetTitle("");
	      hplotall->Draw();
	      chfitall->SaveAs(plotnameall);
	    }
	  //--------------------
#endif
	  
	} // loop over mass points
    } // loop over signal input processes
  } // loop over categories
}

//----------------------------------------------------------------------
