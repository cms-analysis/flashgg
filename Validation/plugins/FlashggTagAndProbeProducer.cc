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
        EDGetTokenT<vector<reco::GenParticle> > genPartToken_;
        Handle<vector<reco::GenParticle> > genPartHandle_;

        //---options
        int maxDiphotons_;
        selector_type tagSelector_;
        selector_type probeSelector_;

        //---ID selector
        ConsumesCollector cc_;
        CutBasedPhotonViewSelector idSelector_;

        //---output TnP collection
        unique_ptr<vector<TagAndProbeCandidate> > tnpColl_;
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
        genPartToken_( consumes<vector<reco::GenParticle> >( pSet.getParameter<InputTag> ( "genParticlesSrc" ) ) ),        
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
        vector<reco::GenParticle> genParticles;
        if( ! event.isRealData() )
        {
            event.getByToken( genPartToken_, genPartHandle_ );
            genParticles = *genPartHandle_.product();
        }

        //---output collection
        std::unique_ptr<vector<TagAndProbeCandidate> > tnpColl_( new vector<TagAndProbeCandidate> );

        //---search for gen electron and positron
        bool genEleFound = false;
        bool genPosFound = false;
        reco::GenParticle genEle;
        reco::GenParticle genPos;
        if( ! event.isRealData() ) {
            for( auto& gen : genParticles ) {
                int status = gen.status();
                int pdgid  = gen.pdgId();
                if ( abs(pdgid)==11 && status==23 ) {
                    if ( gen.mother(0) &&
                         gen.mother(0)->pdgId()==23) {
                        if (pdgid==11)  {
                            genPos = gen;
                            genPosFound = true;
                        }
                        if (pdgid==-11) {
                            genEle = gen;
                            genEleFound = true;
                        }
                    }
                }
            }
        }

        //---loop over diphoton candidates (max number specified from config)
        int nDP = maxDiphotons_ == -1 ? diphotons.size() : std::min(int(diphotons.size()), maxDiphotons_);
        for(int iDP=0; iDP<nDP; ++iDP)
        {
            auto diphoPtr = diphotons.ptrAt(iDP);
            auto lead = diphoPtr->leadingView();
            auto sublead = diphoPtr->subLeadingView();

            //---gen match (simulation only)
            float minDR=999;
            int leadGenMatch=0, subleadGenMatch=0;
            if(genPosFound && reco::deltaR(genPos, *lead->photon()) < 0.3)
            {
                minDR = reco::deltaR(genPos, *lead->photon());
                leadGenMatch = 1;
            }
            if(genEleFound && reco::deltaR(genEle, *lead->photon()) < 0.3 && reco::deltaR(genEle, *lead->photon()) < minDR)
                leadGenMatch = -1;
            minDR = 999;
            if(genPosFound && reco::deltaR(genPos, *lead->photon()) < 0.3)
            {
                minDR = reco::deltaR(genPos, *sublead->photon());
                subleadGenMatch = 1;
            }
            if(genEleFound && reco::deltaR(genEle, *sublead->photon()) < 0.3 && reco::deltaR(genEle, *sublead->photon()) < minDR)
                subleadGenMatch = -1;            
            
            //---check which photon is tag/probe (both combination are allowed)
            //   - TagAndProbeCandidate(diphoPtr, bool leadIsTag)
            //   - Compute id-selection
            //   - Single id variable passed/failed status is set by the selector as a UserInt
            if(tagSelector_(*lead->photon()) && probeSelector_(*sublead->photon()))
            {
                TagAndProbeCandidate cand(diphoPtr, true);
                auto idResults = idSelector_.computeSelections(*sublead->photon(), event);
                for(auto& sel : idResults)
                    cand.addUserInt("probe_pass_"+sel.first, sel.second);
                cand.addUserInt("tagGenMatch", leadGenMatch);
                cand.addUserInt("probeGenMatch", subleadGenMatch);                
                tnpColl_->push_back(cand);
            }
            if(tagSelector_(*sublead->photon()) && probeSelector_(*lead->photon()))
            {
                TagAndProbeCandidate cand(diphoPtr, false);
                auto idResults = idSelector_.computeSelections(*lead->photon(), event);
                for(auto& sel : idResults)
                    cand.addUserInt("probe_pass_"+sel.first, sel.second);                
                cand.addUserInt("tagGenMatch", subleadGenMatch);
                cand.addUserInt("probeGenMatch", leadGenMatch);                
                tnpColl_->push_back(cand);
            }
        }
        //---put the colletion in the Event
        event.put( std::move(tnpColl_) );
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
