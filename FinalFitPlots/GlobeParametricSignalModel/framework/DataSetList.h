#ifndef _DataSetList_h
#define _DataSetList_h

#include <map>
#include <string>
#include <vector>

#include <RooDataSet.h>
#include <RooWorkspace.h>
#include "ParametricSignalModelConfig.h"

class DataSetList
{
protected:

  /** for adding together (and weighting) the signal datasets */
  std::map<std::string, RooDataSet *> mergedSignalDataSets;

  RooWorkspace *win;
  ParametricSignalModelConfig &config;

public:

  /** @param win input workspace containing the MC samples */
  DataSetList(RooWorkspace *win_, ParametricSignalModelConfig &config_);

  /** this uses an internal map of RooDataSets. vars are e.g. the reconstructed mass variable.  */
  RooDataSet *createOrAppendToDataset(RooDataSet *dataToAppend, const std::vector<RooRealVar *>& vars, RooRealVar *weightVar, const std::string &datasetName, Double_t weightscale);

  /** get a RooDataSet from mergedSignalDataSets (i.e. the map internal to this class).
   *  If it does not exist, exits the program. */
  RooDataSet *getMergedWeightedDataSet(const std::string &datasetName);
  
  /** @return the dataset with the given name from the input (background) workspace.
   *  If such a dataset does not exist, exits the program.
   */
  RooDataSet *getDataSet(const std::string &name);

  void makeWeightedDataSets(RooRealVar *weight, const std::vector<RooRealVar *> &vars, RooAbsPdf *allpdf,

      // TODO: the following parameters could be moved to the constructor
      std::vector<std::string> &catnames,
      std::vector<std::string> &inputSigProcessNames,
      std::vector<int> &mhs);

  inline RooDataSet *
  getDataSet(const TString &name)
  {
    return getDataSet(std::string((const char*) name));
  }

  inline RooDataSet *
  getDataSet(const char *name)
  {
    return getDataSet(std::string(name));
  }

};

/**
 * Creates a new RooDataSet(..), add the events contained in 'indata' with their weight
 * multiplied by 'weightscale'.
 *
 * @param name is the name of the returned RooDataSet */
RooDataSet *createWeightedDataset(RooDataSet *indata, RooRealVar *mvar, RooRealVar *wvar, TString name, Double_t weightscale);

/** appends a weighted copy of 'indata' to 'outdata'. Similar to createWeightedDataset but APPENDS instead
 *  to an existing dataset
 *
 *  @param wvar is the weight variable
 *  @param mvar is the mass variable
 *
 *  @return outdata
 *  */
RooDataSet *appendWeightedDataset(RooDataSet *outdata, RooDataSet *indata, const std::vector<RooRealVar *> &mvars, RooRealVar *wvar, Double_t weightscale);



#endif
