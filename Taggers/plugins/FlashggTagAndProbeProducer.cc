//----------------------------------------------------------------------------------------
// Produce tag and probe pairs out of DiPhotons candidate
//
//----------------------------------------------------------------------------------------

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/TagAndProbeCandidate.h"
#include "flashgg/MicroAOD/interface/CutBasedPhotonViewSelector.h"

using namespace edm;
using namespace std;

namespace flashgg {

    class TagAndProbeProducer : public EDProducer
    {
    public:
        //---typedef
        typedef StringCutObjectSelector<Photon, true> selector_type;
        
        //---ctors
        TagAndProbeProducer( );
        TagAndProbeProducer( const ParameterSet & );
        
    private:
        //---FW methods
        void produce( Event &, const EventSetup & ) override;

        //---data
        EDGetTokenT<View<DiPhotonCandidate> > diphotonsToken_;
        Handle<View<DiPhotonCandidate> > diphotonsHandle_;

        //---options
        int maxDiphotons_;
        selector_type tagSelector_;
        selector_type probeSelector_;

        //---ID selector
        ConsumesCollector cc_;
        CutBasedPhotonViewSelector idSelector_;

        //---output TnP collection
        auto_ptr<vector<TagAndProbeCandidate> > tnpColl_;
    };

    //---constructors
    //---dummy
    TagAndProbeProducer::TagAndProbeProducer( ):
        diphotonsToken_(),
        maxDiphotons_(-1),
        tagSelector_("1"),
        probeSelector_("1"),
        cc_( consumesCollector() ),
        idSelector_( ParameterSet(), cc_ )
    {}
    //---standard
    TagAndProbeProducer::TagAndProbeProducer( const ParameterSet & pSet):
        diphotonsToken_( consumes<View<DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "diphotonsSrc" ) ) ),
        maxDiphotons_( pSet.getParameter<int> ( "maxDiphotons" ) ),        
        tagSelector_( pSet.getParameter<string> ( "tagSelection" ) ),
        probeSelector_( pSet.getParameter<string> ( "probeSelection" ) ),
        cc_( consumesCollector() ),
        idSelector_( pSet.getParameter<ParameterSet> ( "idSelection" ), cc_ )
    {
        produces<vector<TagAndProbeCandidate> >();
    }

    //---FW produce method
    void TagAndProbeProducer::produce( Event & event, const EventSetup & setup )
    {
        //---input 
        event.getByToken( diphotonsToken_, diphotonsHandle_ );
        auto diphotons = *diphotonsHandle_.product();

        //---output collection
        tnpColl_ =  auto_ptr<vector<TagAndProbeCandidate> >( new vector<TagAndProbeCandidate> );
        
        //---loop over diphoton candidates (max number specified from config)
        int nDP = maxDiphotons_ == -1 ? diphotons.size() : std::min(int(diphotons.size()), maxDiphotons_);
        for(int iDP=0; iDP<nDP; ++iDP)
        {
            auto diphoPtr = diphotons.ptrAt(iDP);
            auto lead = diphoPtr->leadingView();
            auto sublead = diphoPtr->subLeadingView();

            //---check which photon is tag/probe (both combination are allowed)
            //   - TagAndProbeCandidate(diphoPtr, bool leadIsTag)
            //   - Compute id-selection
            //   - Single id variable passed/failed status is set by the selector as a UserFloat
            if(tagSelector_(*lead->photon()) && probeSelector_(*sublead->photon()))
            {
                TagAndProbeCandidate cand(diphoPtr, true);
                auto idResults = idSelector_.computeSelections(*sublead->photon(), event);
                for(auto& sel : idResults)
                    cand.addUserFloat("probe_pass_"+sel.first, sel.second);
                tnpColl_->push_back(cand);
            }
            if(tagSelector_(*sublead->photon()) && probeSelector_(*lead->photon()))
            {
                TagAndProbeCandidate cand(diphoPtr, false);
                auto idResults = idSelector_.computeSelections(*lead->photon(), event);
                for(auto& sel : idResults)
                    cand.addUserFloat("probe_pass_"+sel.first, sel.second);                
                tnpColl_->push_back(cand);
            }
        }
                    
        //---put the colletion in the Event
        event.put( tnpColl_ );
    }
}

typedef flashgg::TagAndProbeProducer FlashggTagAndProbeProducer;
DEFINE_FWK_MODULE( FlashggTagAndProbeProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
