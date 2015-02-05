#include "OutputRedirector.h"

#include <iostream>

using namespace std;

//----------------------------------------------------------------------

OutputRedirector::OutputRedirector()
{
  // keep track of original buffers
  coutOrigBuf = cout.rdbuf();
  cerrOrigBuf = cerr.rdbuf();
  clogOrigBuf = clog.rdbuf();

  // assign our own, new buffer
  cout.rdbuf(outBuf.rdbuf());
  cerr.rdbuf(outBuf.rdbuf());
  clog.rdbuf(outBuf.rdbuf());


}

//----------------------------------------------------------------------
OutputRedirector::~OutputRedirector()
{
  // restore the original buffers
  cout.rdbuf(coutOrigBuf);
  cerr.rdbuf(cerrOrigBuf);
  clog.rdbuf(clogOrigBuf);
}

//----------------------------------------------------------------------
