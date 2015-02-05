#ifndef _OutputRedirector_h
#define _OutputRedirector_h

#include <iostream>
#include <sstream>

class OutputRedirector
{
protected:
  std::streambuf* coutOrigBuf;
  std::streambuf* cerrOrigBuf;
  std::streambuf* clogOrigBuf;

  /** for the moment, we just try with one
   *  buffer for both cout and cerr.
   */
  std::ostringstream outBuf;

public:

  /** redirects cout and cerr to internal memory buffers */
  OutputRedirector();

  /** undoes the redirection */
  ~OutputRedirector();

};

#endif
