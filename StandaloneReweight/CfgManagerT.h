//NOTE: this class is taken from https://github.com/simonepigazzini/CfgManager.git whose main editor is Simone Pigazzini

#ifndef __CFG_MANAGER_T__
#define __CFG_MANAGER_T__

#include "CfgManager.h"

//**********getters***********************************************************************

//----------get option by name------------------------------------------------------------
template<typename T>
T CfgManager::GetOpt(std::string key, int opt)
{
    key = "opts."+key;
    Errors(key, opt);
    T opt_val;
    std::istringstream buffer(opts_[key][opt]);
    buffer >> opt_val;
    
    return opt_val;
}

template<> inline std::string CfgManager::GetOpt(std::string key, int opt)
{
    key = "opts."+key;
    Errors(key, opt);

    return opts_[key][opt];
}   

template<> inline std::vector<float> CfgManager::GetOpt(std::string key, int opt)
{
    key = "opts."+key;
    Errors(key, opt);
    std::vector<float> optsVect;
    for(unsigned int iOpt=opt; iOpt<opts_[key].size(); ++iOpt)
    {
        double opt_val;
        std::istringstream buffer(opts_[key][iOpt]);
        buffer >> opt_val;
        optsVect.push_back(opt_val);
    }
    
    return optsVect;
}    

template<> inline std::vector<double> CfgManager::GetOpt(std::string key, int opt)
{
    key = "opts."+key;
    Errors(key, opt);
    std::vector<double> optsVect;
    for(unsigned int iOpt=opt; iOpt<opts_[key].size(); ++iOpt)
    {
        double opt_val;
        std::istringstream buffer(opts_[key][iOpt]);
        buffer >> opt_val;
        optsVect.push_back(opt_val);
    }
    
    return optsVect;
}    

template<> inline std::vector<std::string> CfgManager::GetOpt(std::string key, int opt)
{
    key = "opts."+key;
    Errors(key, opt);
    
    return opts_[key];
}    

#endif
