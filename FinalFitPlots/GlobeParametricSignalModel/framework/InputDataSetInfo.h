#ifndef _InputDataSetInfo_h
#define _InputDataSetInfo_h

#include <string>
#include <map>
#include <RooDataSet.h>

#include "DataSetTable.h"

/** class to keep track how many MC events
    were present in each input dataset.
    Can summarize this information as a HTML
    table which can be appended to the 
    output report. This can be useful to
    determine which dataset has small number
    of entries and thus can lead to funny results
    when fitting its mass distribution. */
class InputDataSetInfo
{
protected:
  DataSetTable table;

public:
  /** adds information about a dataset to the table */
  void add(RooDataSet *ds, int mh, const std::string &category, bool rightVertex, const std::string &signalProcessName); 

  inline std::string makeHTMLtable()
  {
    return table.makeHTMLtable("number of MC events in input datasets");
  }
  
};

#endif
