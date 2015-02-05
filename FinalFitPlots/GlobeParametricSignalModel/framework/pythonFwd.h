#ifndef _PythonFwd_h
#define _PythonFwd_h

//----------
// avoid including Python.h in headers due to large number
// of warning messages
#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif
//----------


#endif
