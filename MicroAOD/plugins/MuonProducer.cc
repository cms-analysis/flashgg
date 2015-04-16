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

    };

    MuonProducer::MuonProducer( const ParameterSet &iConfig ):
        muonToken_( consumes<View<pat::Muon> >( iConfig.getUntrackedParameter<InputTag>( "muonTag", InputTag( "slimmedMuons" ) ) ) )
    {
        produces<vector<flashgg::Muon> >();
    }

    void MuonProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<pat::Muon> >  pmuons;
        evt.getByToken( muonToken_, pmuons );
        //	const PtrVector<pat::Muon> pmuonsPointers = pmuons->ptrVector();

        std::cout << "calling produce function " << std::endl;

        std::auto_ptr<vector<flashgg::Muon> > muColl( new vector<flashgg::Muon> );

        for( unsigned int muIndex = 0; muIndex < pmuons->size(); muIndex++ ) {
            Ptr<pat::Muon> pmu = pmuons->ptrAt( muIndex );//retain the same index as patMuon;
            flashgg::Muon fmu = flashgg::Muon( *pmu );

            muColl->push_back( fmu );
        }
        evt.put( muColl );
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

