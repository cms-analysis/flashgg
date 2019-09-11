#include "flashgg/MicroAOD/interface/CutBasedMuonObjectSelector.h"
#include "FWCore/Framework/interface/Event.h"
#include "flashgg/MicroAOD/interface/MuonUtils.h"

using namespace edm;
using namespace std;

namespace flashgg {
    CutBasedMuonObjectSelector::CutBasedMuonObjectSelector( const ParameterSet &cfg, edm::ConsumesCollector &cc ) :
        //        classifier_( cfg ),
        //        rhoToken_( cc.consumes<double>( cfg.getParameter<InputTag>( "rho" ) ) ),
        selector_( cfg.getParameter<string>( "cut" ) ),
        vertexToken_( cc.consumes<View<reco::Vertex> >( cfg.getParameter<InputTag> ( "VertexTag" ) ) ),
        muonutil_(cfg)
    {
        //        jetutil_ = MuonUtils(&cfg);
        
    }

    void CutBasedMuonObjectSelector::handle( const EventBase &ev ) const
    {
        //        edm::Handle<double> rho;
        const Event *ee = dynamic_cast<const Event *>( &ev );
        //        if( ee ) { ee->getByToken( rhoToken_, rho ); }
        //        else { assert( 0 );  } // FIXME need to complete for FWlite
        //        rho_ = *rho;
    }

    bool CutBasedMuonObjectSelector::operator()( const edm::Ref<edm::View<Muon> > candref, const edm::Event &ev ) const {
        return operator()(*candref, ev);
    }

    bool CutBasedMuonObjectSelector::operator()( const Muon &cand, const edm::Event &evt ) const
    {
        if( ! selector_( cand ) ) { return false; }

        //        Handle<View<flashgg::Muon> > theMuons;
        //        evt.getByToken( muonToken_, theMuons );

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );

        bool passSelection = muonutil_.passesMuonSelection(&cand, vertices->ptrs());

        return passSelection;
    }

}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

