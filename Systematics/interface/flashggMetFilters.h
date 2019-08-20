#ifndef flashggMetFilters_h
#define flashggMetFilters_h

// -*- C++ -*-
//
// Package:    flashggMetFilters
// Class:      flashggMetFilters
// 
/* 
 Description: filter events based on results of met filters 
 Implementation: inherits from generic EDFilter
     
*/
//
// Original Author:  Samuel May 
//         Created:  Fri Jul 12 12:06:00 CET 2019
//


// system include files
#include <memory>
#include <string>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// MET Filters are stored as triggers in PAT, so need some trigger headers
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

//
// class declaration
//

using namespace std;
using namespace edm;

class flashggMetFilters : public edm::EDFilter {
    public:
        explicit flashggMetFilters(const edm::ParameterSet&);
        ~flashggMetFilters();

        virtual bool filter(edm::Event&, const edm::EventSetup&);

    private:
        edm::InputTag filtersInputTag;
        edm::EDGetTokenT<edm::TriggerResults> filtersToken;

        std::vector<std::string> requiredFilterNames;

        edm::Handle<edm::TriggerResults> filterResultsHandle;
};
#endif
