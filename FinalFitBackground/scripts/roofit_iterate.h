#include <list>
//#include "RooAbsData.h"
#include "TObject.h"
#include "RooAbsData.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooCategory.h"

#ifdef __CINT__
#pragma link C++ class std::list<RooAbsData*>::iterator;
#pragma link C++ class std::list<RooAbsPdf*>::iterator;
#pragma link C++ class std::list<RooCategory*>::iterator;
#pragma link C++ class std::list<RooRealVar*>::iterator;
#pragma link C++ class std::list<TObject*>::iterator;
#endif
