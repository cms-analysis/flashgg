#include "DataSetTable.h"
#include <sstream>
#include <set>

#include <boost/foreach.hpp>

using namespace std;

//----------------------------------------------------------------------
bool 
DataSetTable::KeyCompare::operator()(const Key &k1, const Key &k2) const
{
  // this determines the order in which these entries appear in the table

  if (k1.category < k2.category)
    return true;
  if (k1.category > k2.category)
    return false;

  // inverted here (make right vertex appear
  // before wrong vertex)
  if (k1.rightVertex < k2.rightVertex)
    return false;
  
  if (k1.rightVertex > k2.rightVertex)
    return true;

  return k1.signalProcessName < k2.signalProcessName;
}

//----------------------------------------------------------------------
bool 
DataSetTable::CombinedKeyCompare::operator()(const CombinedKey &k1, const CombinedKey &k2) const
{
  // chained comparison

  static KeyCompare cmp;
  if (cmp(k1.dataset, k2.dataset))
    // k1.dataset < k2.dataset
    return true;
  
  if (cmp(k2.dataset, k1.dataset))
    // k1.dataset > k2.dataset
    return false;
  
  // now compare the masses
  return k1.mass < k2.mass;
}

//----------------------------------------------------------------------


void 
DataSetTable::add(int mh, const std::string &category, bool rightVertex, const std::string &signalProcessName, 
                  const std::string &cellContent, void *userData)
{
  Entry *entry = new Entry();
  entry->cellContent = cellContent;
  entry->userData = userData;
  
  // entry.dsName = ds->GetName();

  Key key;
  // key.mass = mh;
  key.category = category;
  key.rightVertex = rightVertex;
  key.signalProcessName = signalProcessName;

  data[key][mh] = entry;
} 

//----------------------------------------------------------------------

std::string 
DataSetTable::makeHTMLtable(const std::string &title)
{
  ostringstream os;

  // rows are everything but the mass,
  // columns are mass

  // get the list of all masses
  set<int> masses;
  for (map<Key, map<int, Entry *>, KeyCompare >::const_iterator it = data.begin(); it != data.end(); ++it)
  {
    for (map<int, Entry *>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
    {
      masses.insert(it2->first);
    }
  }
  os << title << endl;
  os << "<table border=\"1\">" << endl;
  
  //----------
  // print the table header
  //----------

  os << "<tr>";
  os << "<th>dataset</th>";

  BOOST_FOREACH(int mass, masses)
  {
    os << "<th>" << mass << " GeV" << "</th>";
  }
  os << "</tr>" << endl;

  //----------

  // now loop over all rows
  for (map<Key, map<int, Entry *>, KeyCompare>::const_iterator it = data.begin(); it != data.end(); ++it)
  {
    os << "<tr>";
    
    const Key &rowKey(it->first);
    os << "<td>" << rowKey.category << " " 
       << (rowKey.rightVertex ? "rightVtx" : "wrongVtx") << " "
       << rowKey.signalProcessName
       << "</td>"
      ;

    BOOST_FOREACH(int mass, masses)
    {
      os << "<th>";
      
      map<int, Entry *>::const_iterator it2 = it->second.find(mass);
      if (it2 != it->second.end())
        {
	  // print the information about this dataset
            // << " (" << it2->second.dsName << ")"
          os << it2->second->cellContent;
	}
      else
	os << "&nbsp;";
      
      os << "</th>";
    } // loop over masses

    os << "</tr>" << endl;

  } // loop over rows

  os << "</table>" << endl;
  return os.str();
}

//----------------------------------------------------------------------

std::set<DataSetTable::CombinedKey, DataSetTable::CombinedKeyCompare> 
DataSetTable::getCombinedKeys()
{
  std::set<CombinedKey, CombinedKeyCompare> retval;
  
  for (map<Key, map<int, Entry *>, KeyCompare>::const_iterator it = data.begin();
       it != data.end();
       ++it)
  {
    CombinedKey key;
    key.dataset = it->first;

    for (map<int, Entry *>::const_iterator it2 = it->second.begin();
         it2 != it->second.end();
         ++it2)
    {
      key.mass = it2->first;
      retval.insert(key);
    }

  }
  
  return retval;
}

//----------------------------------------------------------------------
DataSetTable::Entry *
DataSetTable::getEntry(const CombinedKey &combinedKey)
{
  map<Key, map<int, Entry *>, KeyCompare>::iterator it = data.find(combinedKey.dataset);
  if (it == data.end())
    return NULL;
  
  map<int, Entry *>::iterator it2 = it->second.find(combinedKey.mass);
  
  if (it2 == it->second.end())
    return NULL;
  else
    return it2->second;
}

//----------------------------------------------------------------------
