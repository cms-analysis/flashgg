#include <iostream>

extern "C"
{
#include <Python.h>
}

#include "PythonConfigUtils.h"

#include <boost/foreach.hpp>
#include "utils.h"

using namespace std;

//----------------------------------------------------------------------
PyObject *
PythonConfigUtils::getOptionalDict(PyObject *parentDict, const std::string &keyName)
{
  PyObject *childDict = PyDict_GetItemString(parentDict, keyName.c_str());
  if (childDict == NULL)
    return NULL;

  // make sure that this is a dict
  if (! PyDict_Check(childDict))
  {
    cerr << "object '" << keyName << "' must be a dict." << endl;
    exit(1);
  }

  return childDict;
}

//----------------------------------------------------------------------

PyObject*
PythonConfigUtils::getDictValue(PyObject *dict, const std::string &keyName)
{
  PyObject *obj = PyDict_GetItemString(dict, keyName.c_str());
  if (obj == NULL)
  {
    cerr << "key '" << keyName << "' not found in dict, exiting." << endl;
    exit(1);
  }

  return obj;
}

//----------------------------------------------------------------------

std::string
PythonConfigUtils::getDictStringValue(PyObject *dict, const std::string &keyName)
{
  PyObject *obj = getDictValue(dict, keyName);

  // make sure this is a string
  if (! PyString_Check(obj))
  {
    cerr << "object '" << keyName << "' in dict must be a string. Exiting." << endl;
    exit(1);
  }

  return PyString_AsString(obj);
}

//----------------------------------------------------------------------

std::string
PythonConfigUtils::getDictStringValue(PyObject *dict, const std::string &keyName, const std::string &defaultValue)
{
  PyObject *obj = PyDict_GetItemString(dict, keyName.c_str());
  if (obj == NULL)
    return defaultValue;

  // make sure this is a string
  if (! PyString_Check(obj))
  {
    cerr << "object '" << keyName << "' in dict must be a string. Exiting." << endl;
    exit(1);
  }

  return PyString_AsString(obj);

}

//----------------------------------------------------------------------

bool
PythonConfigUtils::getDictBooleanValue(PyObject *dict, const std::string &keyName)
{
  PyObject *obj = getDictValue(dict, keyName);

  // make sure this is a string
  if (! PyBool_Check(obj))
  {
    cerr << "object '" << keyName << "' in dict must be a boolean. Exiting." << endl;
    exit(1);
  }

  // booleans in python are also integers (see also http://www.python-forum.org/pythonforum/viewtopic.php?f=3&t=7094)
  // or try isinstance(True,int)
  return PyInt_AsLong(obj) != 0;
}

//----------------------------------------------------------------------

bool
PythonConfigUtils::getDictBooleanValue(PyObject *dict, const std::string &keyName, bool defaultValue)
{
  PyObject *obj = PyDict_GetItemString(dict, keyName.c_str());

  if (obj == NULL)
    return defaultValue;

  // make sure this is a string
  if (! PyBool_Check(obj))
  {
    cerr << "object '" << keyName << "' in dict must be a boolean. Exiting." << endl;
    exit(1);
  }

  return PyInt_AsLong(obj) != 0;
}

//----------------------------------------------------------------------

long
PythonConfigUtils::getDictIntValue(PyObject *dict, const std::string &keyName)
{
  PyObject *obj = getDictValue(dict, keyName);

  if (! PyInt_Check(obj))
  {
    cerr << "object '" << keyName << "' in dict must be an integer. Exiting." << endl;
    exit(1);
  }

  return PyInt_AsLong(obj);
}

//----------------------------------------------------------------------

long 
PythonConfigUtils::getDictIntValue(PyObject *dict, const std::string &keyName, long defaultValue)
{
  PyObject *obj = PyDict_GetItemString(dict, keyName.c_str());
  if (obj == NULL)
    return defaultValue;
  
  return getDictIntValue(dict, keyName);
}

//----------------------------------------------------------------------

double
PythonConfigUtils::castNumberToFloat(PyObject *obj)
{
  if (PyFloat_Check(obj))
    return PyFloat_AsDouble(obj);

  if (PyInt_Check(obj))
    return (double) PyInt_AsLong(obj);

  if (PyLong_Check(obj))
    return PyLong_AsDouble(obj);


  // it's neither of these types
  cerr << "object could not be converted to a double. Exiting." << endl;
  exit(1);
}

//----------------------------------------------------------------------

std::string 
PythonConfigUtils::castToString(PyObject *obj)
{
  return PyString_AsString(obj);  

}
//----------------------------------------------------------------------

double
PythonConfigUtils::getDictDoubleValue(PyObject *dict, const std::string &keyName)
{
  PyObject *obj = PyDict_GetItemString(dict, keyName.c_str());

  if (obj == NULL)
  {
    cerr << "object '" << keyName << "' not found in dict. Exiting." << endl;
    exit(1);
  }

  return castNumberToFloat(obj);

}

//----------------------------------------------------------------------

double
PythonConfigUtils::getDictDoubleValue(PyObject *dict, const std::string &keyName, double defaultValue)
{
  PyObject *obj = PyDict_GetItemString(dict, keyName.c_str());

  if (obj == NULL)
    return defaultValue;

  return getDictDoubleValue(dict, keyName);
}

//----------------------------------------------------------------------
double *
PythonConfigUtils::getDictDoubleValuePointer(PyObject *dict, const std::string &keyName)
{
  PyObject *obj = PyDict_GetItemString(dict, keyName.c_str());

  if (obj == NULL)
    return NULL;

  return new double(getDictDoubleValue(dict, keyName));
}

//----------------------------------------------------------------------

string *
PythonConfigUtils::getDictStringValuePointer(PyObject *dict, const std::string &keyName)
{
  PyObject *obj = PyDict_GetItemString(dict, keyName.c_str());

  if (obj == NULL)
    return NULL;

  return new string(getDictStringValue(dict, keyName));
}

//----------------------------------------------------------------------

bool *
PythonConfigUtils::getDictBooleanValuePointer(PyObject *dict, const std::string &keyName)
{
  PyObject *obj = PyDict_GetItemString(dict, keyName.c_str());

  if (obj == NULL)
    return NULL;

  return new bool(getDictBooleanValue(dict, keyName));
}

//----------------------------------------------------------------------

vector<int>
PythonConfigUtils::getDictIntListValue(PyObject *dict, const std::string &keyName, const std::vector<int> &defaultValue)
{
  PyObject *obj = PyDict_GetItemString(dict, keyName.c_str());

  if (obj == NULL)
    return defaultValue;

  vector<int> retval;

  // insist that this is a list (in the future we could relax this to
  // require merely an iterable item)
  // make sure that this is a dict
  PyObject* sequence = PySequence_Fast(obj, ("object '" + keyName + "' must be a sequence.").c_str());
  int numElelements = PySequence_Size(obj);
  for (int index = 0; index < numElelements; index++)
  {
    PyObject *element = PySequence_Fast_GET_ITEM(sequence, index);

    if (! PyInt_Check(element))
    {
      cerr << "found a non-integer object in list '" + keyName + "'." << endl;
      exit(1);
    }

    retval.push_back(PyInt_AsLong(element));
  } // loop over all elements of the list

  Py_DECREF(sequence);

  return retval;
}

//----------------------------------------------------------------------

vector<string>
PythonConfigUtils::getDictStringListValue(PyObject *dict, const std::string &keyName, const std::vector<string> &defaultValue)
{
  PyObject *obj = PyDict_GetItemString(dict, keyName.c_str());

  if (obj == NULL)
    return defaultValue;

  vector<string> retval;

  // insist that this is a list (in the future we could relax this to
  // require merely an iterable item)
  // make sure that this is a dict
  PyObject* sequence = PySequence_Fast(obj, ("object '" + keyName + "' must be a sequence.").c_str());
  int numElelements = PySequence_Size(obj);
  for (int index = 0; index < numElelements; index++)
  {
    PyObject *element = PySequence_Fast_GET_ITEM(sequence, index);

    if (! PyString_Check(element))
    {
      cerr << "found a non-string object in list '" + keyName + "'." << endl;
      exit(1);
    }

    retval.push_back(PyString_AsString(element));
  } // loop over all elements of the list

  Py_DECREF(sequence);

  return retval;
}

//----------------------------------------------------------------------

PyObject *
PythonConfigUtils::getNestedDictValue(PyObject *dict, const std::list<std::string> &keys)
{
   PyObject *retval = dict;
 
   BOOST_FOREACH(const string &key, keys)
   {
     // use call __getitem__ instead of using PyDict_GetItemString(..)
     // (which seems NOT to work for defaultdicts)

     retval = PyObject_CallMethodObjArgs(retval,
                                         PyString_FromString("__getitem__"),
                                         PyString_FromString(key.c_str()), 
                                         NULL);
     
     ASSERTBREAK(retval != NULL);
   }

   return retval;
}
