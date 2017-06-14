#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Common/interface/Handle.h"

#include "flashgg/DataFormats/interface/TagCandidate.h"

#include <vector>
#include <string>

using namespace edm;
using namespace std;

namespace flashgg {

    class TagCandidateProducer : public EDProducer
    {

    public:
        TagCandidateProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        edm::EDGetTokenT<edm::OwnVector<flashgg::DiPhotonTagBase> > tagSorterToken_;
        edm::EDGetTokenT<edm::TriggerResults> triggerResultsToken_;
        edm::Handle<edm::TriggerResults> triggerResultsHandle_;
        std::vector<std::string> pathNames_;        
    };

    TagCandidateProducer::TagCandidateProducer( const ParameterSet &iConfig ) :
        tagSorterToken_( consumes<edm::OwnVector<flashgg::DiPhotonTagBase> >( iConfig.getUntrackedParameter<InputTag> ( "TagSorter"))),
        triggerResultsToken_( consumes<edm::TriggerResults>(iConfig.getUntrackedParameter<edm::InputTag>("triggerResults"))), 
        pathNames_(iConfig.getUntrackedParameter<std::vector<std::string> >("pathNames"))
    {
        produces<std::vector<flashgg::TagCandidate> >();
        if (pathNames_.size() > 3) {
            std::cerr << "Too many trigger bits..." << std::endl;
            abort();
        }
    }

    void TagCandidateProducer::produce( Event &evt, const EventSetup & )
    {
        
        unique_ptr<std::vector<flashgg::TagCandidate> > tagsColl( new std::vector<flashgg::TagCandidate> );
        
        Handle<edm::OwnVector<flashgg::DiPhotonTagBase> > tagSorter;
        evt.getByToken( tagSorterToken_, tagSorter );
        
        if( tagSorter.product()->size() > 0 ) {
            int passSel[3];           
            for (int i=0; i<3; i++) 
                passSel[i] = 0;

            bool isData = (evt.eventAuxiliary().isRealData());
            evt.getByToken(triggerResultsToken_, triggerResultsHandle_);
            
            if (isData) {
                const edm::TriggerResults& trigResults = *triggerResultsHandle_;
                const edm::TriggerNames& trigNames = evt.triggerNames(trigResults);  
                
                for(size_t pathNr=0; pathNr<3; pathNr++){
                    passSel[pathNr] = 0;
                    
                    if (pathNr < pathNames_.size()) {
                        size_t pathIndex = trigNames.triggerIndex(pathNames_[pathNr]);
                        if(pathIndex<trigResults.size() &&  trigResults.accept(pathIndex)) 
                            passSel[pathNr] = 1;
                    }
                }
            }

            const flashgg::DiPhotonTagBase *chosenTag = &*( tagSorter.product()->begin() );
            TagCandidate tags( chosenTag, passSel );
            tagsColl->push_back( tags );
        }
 
        evt.put( std::move( tagsColl ) );                               
    }
}

typedef flashgg::TagCandidateProducer FlashggTagCandidateProducer;
DEFINE_FWK_MODULE( FlashggTagCandidateProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
