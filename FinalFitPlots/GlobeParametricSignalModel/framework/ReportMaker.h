#ifndef _ReportMaker_h
#define _ReportMaker_h

#include <iostream>

#include <vector>
#include <string>

/** class for summarizing the results in a (HTML) report */
class ReportMaker
{
protected:
  /** this is used to check that no things are added after the report
   *  was written out.
   */
  bool written;

  /** items added in order. First is the 'path', second is the html text */
  std::vector<std::pair<std::string, std::string> > elements;

public:

  ReportMaker();

  /** can be called to add a plot to be included into the report */
  void addPlotPNG(const std::string &path, const std::string &imageFname, const std::string &description);

  /** adds a SVG inline. This has the advantage that after compression
      of the entire HTML file, the file is smaller compared
      to when PNGs are added with base64 encoding. */
  void addPlotSVG(const std::string &path, const std::string &imageFname, const std::string &description);

  void addFitStatus(const std::string &path, const std::string &status);

  /** generates the report and writes the report to a file */
  void write(const std::string &fname);

  /** adds plain html (e.g. tabular data) */
  void addHTML(const std::string &path, const std::string &text);

protected:
  std::string pathToAnchorName(std::string path);

  void writeTOC(std::ostream &os);

  static std::string encodeBase64(const std::string &origText);

  static std::string readFile(const std::string &fname);

};


#endif
