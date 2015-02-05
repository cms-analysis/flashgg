#include "ReportMaker.h"

#include <fstream>
#include <algorithm>

#include "utils.h"

using namespace std;

//----------------------------------------------------------------------

ReportMaker::ReportMaker() :
  written(false)
{
}

//----------------------------------------------------------------------
void
ReportMaker::addPlotPNG(const std::string &path, const std::string &imageFname, const std::string &description)
{
  ASSERTBREAK(! written);

  ostringstream buf;

  // standard: link to the external file
//  buf << "<img src=\"" << imageFname << "\"/><br/>"

  // data uri scheme: embed the image into the html file
  buf << "<img src=\"data:image/png;base64," << encodeBase64(readFile(imageFname)) << "\" /><br/>";

  buf  << description << "<br/>";

  elements.push_back(std::make_pair(path, buf.str()));

}

//----------------------------------------------------------------------

void
ReportMaker::addPlotSVG(const std::string &path, const std::string &imageFname, const std::string &description)
{
  ASSERTBREAK(! written);

  ostringstream buf;

  // read the svg file, assume that the first line only
  // contains the xml tag and remove it. Copy the rest
  // of the svg file into the html file.
  ifstream is(imageFname.c_str());

  // skip the first line (TODO: should catch problems here)
  string line;
  getline(is,line);

  while (getline(is, line))
  {
    buf << line << endl;
  } // loop over lines

  buf << "<br/>" << description << "<br/>";

  elements.push_back(std::make_pair(path, buf.str()));

}

//----------------------------------------------------------------------

void
ReportMaker::addFitStatus(const std::string &path, const std::string &status)
{
  ASSERTBREAK(! written);

  ostringstream buf;

  buf << "fit status " << path << ": " << status << "<br/>";

  elements.push_back(std::make_pair(path, buf.str()));

}
//----------------------------------------------------------------------

void
ReportMaker::addHTML(const std::string &path, const std::string &text)
{
  ASSERTBREAK(! written);
  elements.push_back(std::make_pair(path, text));
}

//----------------------------------------------------------------------

void
ReportMaker::write(const std::string &fname)
{
  written = true;
  // sort the items according to their path

  // this will sort according to 'first' first
  // see http://stackoverflow.com/a/4610244/288875
  std::sort(elements.begin(), elements.end());

  ofstream os(fname.c_str());

  os << "<html>" << endl;
  os << "<body>" << endl;

  os << "<h1>Signal fitting report</h1><br/>" << endl;

  writeTOC(os);

  // TODO: print a (linked) table of contents

  for (unsigned i = 0; i < elements.size(); ++i)
  {
    // separator line
    if (i > 0)
      os << "<hr/>" << endl;

    os << "<a name=\"" << pathToAnchorName(elements[i].first) << "\">" << endl;

    os << "<h2>" << elements[i].first << "</h2><br/><br/>" << endl;

    os << elements[i].second << "<br/>" << endl;

  } // loop over all items of the report

  os << "</body>" << endl;
  os << "</html>" << endl;
}

//----------------------------------------------------------------------

void
ReportMaker::writeTOC(std::ostream &os)
{
  // assume that the entries are already sorted
  os << "<h2>Table of contents</h2><br/>" << endl;

  os << "<ul>" << endl;

  for (unsigned i = 0; i < elements.size(); ++i)
  {
    os << "<li>"
         << "<a href=\"#" << pathToAnchorName(elements[i].first) << "\">"
         << elements[i].first
         << "</a>"
       << "</li>" << endl;

  } // loop over all items of the report
  os << "</ul>" << endl;

}

//----------------------------------------------------------------------

std::string
ReportMaker::pathToAnchorName(std::string path)
{
  // does SOME (but not all) escaping

  std::replace(path.begin(), path.end(), '/', '_');
  return path;
}

//----------------------------------------------------------------------
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>

std::string
ReportMaker::encodeBase64(const std::string &origText)
{
  // see http://stackoverflow.com/a/7053808/288875
  using namespace boost::archive::iterators;

  typedef
    insert_linebreaks<
      base64_from_binary<
        transform_width<
          const char *,
            6,             // encode to 6 bits
            8              // from 8 bits
        >
      >
    ,72                    // line width
  > transformer;

  stringstream buf;

  std::copy(
      transformer(origText.c_str()),
      transformer(origText.c_str() + origText.size()),
      std::ostream_iterator<char>(buf));

  return buf.str();
}

//----------------------------------------------------------------------

std::string
ReportMaker::readFile(const std::string &fname)
{
  std::ifstream is(fname.c_str());
  std::string buf((std::istreambuf_iterator<char>(is)),
                   std::istreambuf_iterator<char>());

  return buf;
}

//----------------------------------------------------------------------
