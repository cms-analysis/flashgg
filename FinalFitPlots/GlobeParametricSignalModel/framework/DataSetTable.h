#ifndef _DataSetTable_h
#define _DataSetTable_h

#include <string>
#include <map>
#include <set>

/** generic class to produce a table with information about the input
    datasets.

    The table has columns for the different mass points and 
    rows for the other attributes (category, signal process,
    right/wrong vertex)
*/
    
class DataSetTable
{
public:
  /** key for a row. Does not include the mass
      (which is used for the column) */
  struct Key
  {
    std::string category;
    bool rightVertex;
    std::string signalProcessName;
  };

  class KeyCompare
  {
  public:
    bool operator()(const Key &k1, const Key &k2) const;
  };

  //----------------------------------------

  struct Entry
  {
    // std::string dsName;
    // unsigned numEntries;
    
    std::string cellContent;

    /** some other user data. We could actually make this class templated. */
    void *userData;
  };

  //----------------------------------------

  /** this is mostly for looping over ALL datasets */
  struct CombinedKey
  {
    Key dataset;
    int mass;
  };
  
  //----------------------------------------

  class CombinedKeyCompare
  {
  public:
    bool operator()(const CombinedKey &k1, const CombinedKey &k2) const;
  };

  //----------------------------------------

protected:
  /** first index is row key,
      second index is the mass,
      value is the information about the dataset */
  std::map<Key, std::map<int, Entry *>, KeyCompare> data;

public:

  /** adds information about a dataset to the table */
  void add(int mh, const std::string &category, bool rightVertex, const std::string &signalProcessName, 
           const std::string &cellContent, void *userData = NULL);

  inline void add(const CombinedKey &key, const std::string &cellContent, void *userData = NULL)
  {
    add(key.mass, key.dataset.category, key.dataset.rightVertex, key.dataset.signalProcessName, 
        cellContent, userData);
  }

  /** @return a string with the actual HTML table with the information
   */
  std::string makeHTMLtable(const std::string &title);


  /** this is for looping over all known datasets */
  std::set<CombinedKey, CombinedKeyCompare> getCombinedKeys();

  /** @return the speified entry or NULL if it does not exist. Do NOT
      delete the returned object. */
  Entry *getEntry(const CombinedKey &combinedKey);

  inline void *getUserData(const CombinedKey &key)
  {
    Entry *entry = getEntry(key);
    if (entry == NULL)
      return NULL;
    else
      return entry->userData;
  }
};

#include <iostream>

inline std::ostream &operator<<(std::ostream &os, const DataSetTable::CombinedKey &key)
{
  os << "cat=" << key.dataset.category 
     << " vertex=" << (key.dataset.rightVertex ? "right" : "wrong")
     << " proc=" << key.dataset.signalProcessName
     << " mass=" << key.mass
    ;
  
  return os;
}


#endif
