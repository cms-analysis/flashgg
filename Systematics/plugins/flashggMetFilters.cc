#include "flashgg/Systematics/interface/flashggMetFilters.h"

flashggMetFilters::flashggMetFilters(const edm::ParameterSet& iConfig)
{
    filtersToken = consumes<edm::TriggerResults>(iConfig.getParameter<InputTag>("filtersInputTag"));

    requiredFilterNames = iConfig.getUntrackedParameter<std::vector<std::string>>("requiredFilterNames");
}

flashggMetFilters::~flashggMetFilters()
{
}

// member functions
//

// ------------ method called on each new Event  ------------
bool
flashggMetFilters::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    iEvent.getByToken(filtersToken, filterResultsHandle);

    if (!filterResultsHandle.isValid()) {
        throw cms::Exception("flashggMetFilters::filter: error getting TriggerResults from event!");
    }

    edm::TriggerNames filterNames = iEvent.triggerNames(*filterResultsHandle);

    for (unsigned int i = 0; i < filterNames.size(); i++) {
        if (std::find(requiredFilterNames.begin(), requiredFilterNames.end(), filterNames.triggerName(i)) != requiredFilterNames.end()) { // Required filter
            if (filterResultsHandle->accept(i)) {
                continue; 
            }
            else {
                return false;
            }
        }
        else {
            continue;
        }
    }

    return true; 

}

DEFINE_FWK_MODULE(flashggMetFilters);
