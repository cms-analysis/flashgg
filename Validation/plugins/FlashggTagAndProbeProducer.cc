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

using namespace edm;
using namespace std;

namespace flashgg {

    class TagAndProbeProducer : public EDProducer
    {
    public:
        //---typedef
        typedef StringCutObjectSelector<const Photon*, true> selector_type;
        
        //---ctors
        TagAndProbeProducer( );
        TagAndProbeProducer( const ParameterSet & );
        
    private:
        //---FW methods
        void produce( Event &, const EventSetup & ) override;

        //---data
        EDGetTokenT<vector<DiPhotonCandidate> > diphotonsToken_;
        Handle<vector<DiPhotonCandidate> > diphotonsHandle_;
        int maxDiphotons_;
        selector_type tagSelector_;
        selector_type probeSelector_;        
    };

    //---constructors
    //---dummy
    TagAndProbeProducer::TagAndProbeProducer( ):
        diphotonsToken_(),
        maxDiphotons_(-1),
        tagSelector_(""),
        probeSelector_("")
    {}
    //---standard
    TagAndProbeProducer::TagAndProbeProducer( const ParameterSet & pSet):
        diphotonsToken_( consumes<vector<DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "diphotonsSrc" ) ) ),
        maxDiphotons_( pSet.getParameter<int> ( "maxDiphotons" ) ),        
        tagSelector_( pSet.getParameter<string> ( "tagSelection" ) ),
        probeSelector_( pSet.getParameter<string> ( "probeSelection" ) )
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
        auto_ptr<vector<TagAndProbeCandidate> > tnpColl( new vector<TagAndProbeCandidate> );
        
        //---loop over diphoton candidates (max number specified from config)
        int nDP = maxDiphotons_ == -1 ? diphotons.size() : std::min(int(diphotons.size()), maxDiphotons_);
        for(int iDP=0; iDP<nDP; ++iDP)
        {
            auto lead = diphotons[iDP].getLeadingView();
            auto sublead = diphotons[iDP].getSubLeadingView();

            if(tagSelector_(lead.photon()) && probeSelector_(sublead.photon()))
                tnpColl->push_back(TagAndProbeCandidate(lead.originalPhoton(), sublead.originalPhoton()));
            if(tagSelector_(sublead.photon()) && probeSelector_(lead.photon()))
                tnpColl->push_back(TagAndProbeCandidate(sublead.originalPhoton(), lead.originalPhoton()));
        }

        //---put the colletion in the Event
        event.put( tnpColl );
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
