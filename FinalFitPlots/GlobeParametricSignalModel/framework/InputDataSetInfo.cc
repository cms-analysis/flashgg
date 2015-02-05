#include "InputDataSetInfo.h"


#include "utils.h"

using namespace std;

//----------------------------------------------------------------------
void 
InputDataSetInfo::add(RooDataSet *ds, int mh, const std::string &category, bool rightVertex, const std::string &signalProcessName)
{
  table.add(mh, category, rightVertex, signalProcessName, toStr(ds->numEntries()));
}

