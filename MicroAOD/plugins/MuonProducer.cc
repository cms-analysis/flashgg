// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/DataFormats/interface/Muon.h"

using namespace std;
using namespace edm;

namespace flashgg {

    class MuonProducer : public edm::EDProducer
    {
    public:
        MuonProducer( const edm::ParameterSet & );

    private:
        void produce( edm::Event &, const edm::EventSetup & );

        edm::EDGetTokenT<View<pat::Muon> > muonToken_;
        edm::EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;

    };

    MuonProducer::MuonProducer( const ParameterSet &iConfig ):
        muonToken_( consumes<View<pat::Muon> >( iConfig.getParameter<InputTag>( "muonTag" ) ) ),
        pfcandidateToken_( consumes<View<pat::PackedCandidate> >( iConfig.getParameter<InputTag> ( "pfCandidatesTag" ) ) )
    {

        produces<vector<flashgg::Muon> >();
    }

    void MuonProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<pat::Muon> >  pmuons;
        evt.getByToken( muonToken_, pmuons );
        //	const PtrVector<pat::Muon> pmuonsPointers = pmuons->ptrVector();        

        Handle<View<pat::PackedCandidate> > pfcandidates;
        evt.getByToken( pfcandidateToken_, pfcandidates );
        const std::vector<edm::Ptr<pat::PackedCandidate> > &pfcands = pfcandidates->ptrs();

        //        std::cout << "calling produce function " << std::endl;

        std::unique_ptr<vector<flashgg::Muon> > muColl( new vector<flashgg::Muon> );

        for( unsigned int muIndex = 0; muIndex < pmuons->size(); muIndex++ ) {
            Ptr<pat::Muon> pmu = pmuons->ptrAt( muIndex );//retain the same index as patMuon;
            flashgg::Muon fmu = flashgg::Muon( *pmu );

            float fggPFIsoSumRelR04_ = ( pmu->pfIsolationR04().sumChargedHadronPt + max( 0., pmu->pfIsolationR04().sumNeutralHadronEt + pmu->pfIsolationR04().sumPhotonEt - 0.5 * pmu->pfIsolationR04().sumPUPt ) ) / ( pmu->pt() );
            fmu.setFggPFIsoSumRelR04( fggPFIsoSumRelR04_ );

            float fggTrkIsoSumRelR03_ = pmu->isolationR03().sumPt / pmu->pt() ;
            fmu.setFggTrkIsoSumRelR03( fggTrkIsoSumRelR03_ );


            muColl->push_back( fmu );
        }
        evt.put( std::move( muColl ) );
    }
}

typedef flashgg::MuonProducer FlashggMuonProducer;
DEFINE_FWK_MODULE( FlashggMuonProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

