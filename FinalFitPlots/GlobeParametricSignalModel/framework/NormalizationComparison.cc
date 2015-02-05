#include "NormalizationComparison.h"

#include <boost/foreach.hpp>

#include <RooWorkspace.h>
#include <RooDataSet.h>
#include <RooRealVar.h>

#include "ReportMaker.h"


#include "utils.h"

using namespace std;


//----------------------------------------------------------------------

void 
NormalizationComparison::addInputDataset(RooDataSet *ds, int mh, const std::string &category, bool rightVertex, 
                                         const std::string &signalProcessName)
{
  // TODO: should check whether we have already an entry 
  // (containing e.g. the number of events in the output workspace)
  Entry *entry = new Entry();
  entry->numEventsInputDataset = ds->sumEntries();
  entry->numEventsOutputWorkspace = -1;

  cout << "ADDING entry=" << entry << " for key=" << category << " " << rightVertex << " " << signalProcessName << " " << mh << endl;
  table.add(mh, category, rightVertex, signalProcessName, "", entry);
}

//----------------------------------------------------------------------

void 
NormalizationComparison::getOutputNormalizations(bool useRightWrongVertex, RooWorkspace *wsig, RooRealVar *mhyp, double inputWsLumi)
{
  double originalMhypValue = mhyp->getVal();

//  cout << "MHYP=";
//  mhyp->Print();
//  cout << endl;
  

  set<DataSetTable::CombinedKey, DataSetTable::CombinedKeyCompare> keys = table.getCombinedKeys();
  BOOST_FOREACH(DataSetTable::CombinedKey key, keys)
  {
    // build name of normalization function
    string funcName = normalizationFunctionName(key.dataset.category, key.dataset.signalProcessName);
    
    RooAbsReal *normFunc = wsig->function(funcName.c_str());
    if (normFunc == NULL)
    {
      cerr << "internal error: could not find normalization function " << funcName << " in output workspace" << endl;
      ASSERTBREAK(false);
    }

    Entry *entry = (Entry*) (table.getUserData(key));

    // set the mass hypothesis variable 
    mhyp->setVal(key.mass);

    // deal with right/wrong vertex
    double fracRightWrongVertex = 1;
    if (useRightWrongVertex)
    {
      // TODO: this should not be hardwired here
      // (note also that this is BEFORE the nuisance parameter)
      string fracFuncName = "func_" + key.dataset.category + "_" + "fracright" + "_" + key.dataset.category + "_" + key.dataset.signalProcessName;
      
      RooAbsReal *fracRightFunc = wsig->function(fracFuncName.c_str());
      if (fracRightFunc == NULL)
      {
        cerr << "internal error: could not find fraction of right vertices function " << fracFuncName << " in output workspace" << endl;
        ASSERTBREAK(false);
      }

      fracRightWrongVertex = fracRightFunc->getVal();

      if (! key.dataset.rightVertex)
        fracRightWrongVertex = 1 - fracRightWrongVertex;
    }

    // get the number of expected events from the output workspace
    entry->numEventsOutputWorkspace = normFunc->getVal() * inputWsLumi * fracRightWrongVertex;

    // cout << "YY got " << entry->numEventsOutputWorkspace << " for mh=" << mhyp->getVal() << endl;
    

  } // loop over all keys

  // restore mass hypothesis variable
  mhyp->setVal(originalMhypValue);

}

//----------------------------------------------------------------------
std::string 
NormalizationComparison::makeHTMLtable()
{
  // set the table contents
  set<DataSetTable::CombinedKey, DataSetTable::CombinedKeyCompare> keys = table.getCombinedKeys();
  BOOST_FOREACH(DataSetTable::CombinedKey key, keys)
  {
    Entry *entry = (Entry*)(table.getUserData(key));

    // cout << "GOT entry=" << entry << " for key=" << key << endl;
    
    ostringstream buf;
    buf << "in: " << entry->numEventsInputDataset << " / out: " << entry->numEventsOutputWorkspace;

    // TODO: add relative difference

    table.add(key, buf.str(), entry);
    
  }  // loop over all keys

  return table.makeHTMLtable("normalization comparison between input and output workspace");
}
