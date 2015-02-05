extern "C"
{
#include <Python.h>
}

#include "AbstractSignalModel.h"

//----------------------------------------------------------------------

AbstractSignalModel::AbstractSignalModel() :
  globalConfig(NULL),
  mnom(NULL)
{
}

void
AbstractSignalModel::setHiggsMassHypothesisVar(RooRealVar *mnom)
{
  this->mnom = mnom;
}

//----------------------------------------------------------------------

void
AbstractSignalModel::setGlobalConfig(const ParametricSignalModelConfig &config)
{
  this->globalConfig = &config;
}

//----------------------------------------------------------------------
void
AbstractSignalModel::setInputProcessNames(const std::vector<std::string> &inputSigProcessNames)
{
  this->inputSigProcessNames = &inputSigProcessNames;
}

//----------------------------------------------------------------------
