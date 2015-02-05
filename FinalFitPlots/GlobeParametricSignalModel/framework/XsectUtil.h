#ifndef _XsectUtil_h
#define _XsectUtil_h

#include <string>
#include <map>

#include <cmath>

class ReportMaker;
class RooHistFunc;
class RooRealVar;
class RooWorkspace;
class TH1F;

/** class containing functions related to cross sections */
class XsectUtil
{
protected:

  class FloatCompare
  {
  public:
    bool operator()(float x1, float x2)
    {
      const float tolerance = 0.01;

      return (std::abs(x1 - x2) > tolerance) && (x1 < x2);
    }
  };

  bool canMakeHistogramFunc;

  /** cross sections to be used (not necessarily SM though,
      can also be fermiophobic, thus the variable should
      be renamed).

      First index is the signal process name,
      the second index is the mass point,
      the value is cross section times branching ratio.
       */
  std::map<std::string, std::map<float, float, FloatCompare> > sm_xs_br;

  /** cross sections only (without multiplication of the branching ratio).
   *  Indexing is the same as for sm_xs_br.
   *  */
  std::map<std::string, std::map<float, float, FloatCompare> > sm_xs;

  /** branching ratio into photons: index is the mass */
  std::map<float, float, FloatCompare> sm_br;

public:

  /** produces an instance from the cross section in the C code */
  static XsectUtil* fromXsectCode(int sqrts, bool fermiophobic);

  /** gets the cross sections from the input workspace */
  static XsectUtil *fromInputWorkspace(RooWorkspace *ws, bool fermiophobic);

protected:
  TH1F *makeCrossSectionHistogram(const std::string &sigProcName);

public:


  /**
   * This only works reliably when the cross sections were read from the
   * code table, NOT when they were read from the input file (which typically
   * has a much coarser grid). If you want get a function based on the cross
   * sections in the input file, use the other method based on linear interpolation.
   *
   * mnom and reportMaker can be null */
  RooHistFunc *makeCrossSectionFunctionUsingHistogram(const std::string &sigProcName, ReportMaker *reportMaker,
      RooRealVar *mnom);

protected:
  TH1F *makeBranchingRatioHistogram();

public:
  RooHistFunc* makeBranchingRatioFunctionUsingHistogram(ReportMaker *reportMaker, RooRealVar *mnom);

   /** @return the sum of the cross sections of all relevant production mechanisms
   *  for the given mass.
   */
  double getSum(int mass);

  /** @return a per-signal process cross section */
  double get(int mass, const std::string &process);

};

#endif
