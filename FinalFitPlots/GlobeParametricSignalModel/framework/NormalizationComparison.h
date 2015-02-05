#ifndef _NormalizationComparison_h
#define _NormalizationComparison_h

#include <string>

#include "DataSetTable.h"

class ReportMaker;
class InputDataSetInfo;
class RooWorkspace;
class RooDataSet;
class RooRealVar;

/** class to compare the number of expected events
    in the input workspace (sum over event weights of
    the RooDataSets) and in the output workspace
    (from functions)
*/
class NormalizationComparison
{
protected:
  DataSetTable table;

  /** entry to be stored in the userdata of 'table' */
  class Entry
  {
  public:
    double numEventsInputDataset;
    double numEventsOutputWorkspace;
  };
  

public:
  void addInputDataset(RooDataSet *ds, int mh, const std::string &category, bool rightVertex, 
                       const std::string &signalProcessName);

  /** reads the normalization numbers corresponding to the input
      datasets added before from the output workspace 

      @param inputWsLumi is the luminosity (in /pb) from the input
      (background) workspace (the output workspace is normalized to
      one 1/pb while the input workspace has the number of signal
      events normalized to the recorded luminosity)
  */
  void getOutputNormalizations(bool useRightWrongVertex, RooWorkspace *wsig, RooRealVar *mhyp, double inputWsLumi);

  /** create a table for a report */
  std::string makeHTMLtable();

};



#endif
