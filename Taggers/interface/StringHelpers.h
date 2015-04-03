#ifndef flashgg_StringHelpers_h
#define flashgg_StringHelpers_h


#include <string>
#include <map>


namespace flashgg {
    std::string formatString( const std::string &inp, const std::map<std::string, std::string> &replacements );

    std::string replaceString( const std::string &inp, const std::string &src, const std::string &replace );

}


#endif // flashgg_StringHelpers_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

