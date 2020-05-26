#include "flashgg/MicroAOD/interface/CutBasedElectronObjectSelector.h"
#include "FWCore/Framework/interface/Event.h"
#include "flashgg/MicroAOD/interface/ElectronUtils.h"

using namespace edm;
using namespace std;

namespace flashgg {
    CutBasedElectronObjectSelector::CutBasedElectronObjectSelector( const ParameterSet &cfg, edm::ConsumesCollector &cc ) :
        //        classifier_( cfg ),
        //        rhoToken_( cc.consumes<double>( cfg.getParameter<InputTag>( "rho" ) ) ),
        selector_( cfg.getParameter<string>( "cut" ) ),
        diphotonToken_( cc.consumes<View<flashgg::DiPhotonCandidate> >( cfg.getParameter<InputTag> ( "DiphotonTag" ) ) ),
        electronutil_(cfg)
    {
        //        jetutil_ = ElectronUtils(&cfg);
        
    }

    void CutBasedElectronObjectSelector::handle( const EventBase &ev ) const
    {
        //        edm::Handle<double> rho;
        const Event *ee = dynamic_cast<const Event *>( &ev );
        //        if( ee ) { ee->getByToken( rhoToken_, rho ); }
        //        else { assert( 0 );  } // FIXME need to complete for FWlite
        //        rho_ = *rho;
    }

    bool CutBasedElectronObjectSelector::operator()( const edm::Ref<edm::View<Electron> > candref, const edm::Event &ev ) const {
        return operator()(*candref, ev);
    }

    bool CutBasedElectronObjectSelector::operator()( const Electron &cand, const edm::Event &evt ) const
    {
        if( ! selector_( cand ) ) { return false; }

        //        Handle<View<flashgg::Electron> > theElectrons;
        //        evt.getByToken( muonToken_, theElectrons );

        Handle<View<flashgg::DiPhotonCandidate> > diphotons;
        evt.getByToken( diphotonToken_, diphotons );

        bool passSelection = electronutil_.passesElectronSelection(&cand);
        bool passPhoVeto = electronutil_.passesPhoVeto(&cand, diphotons->ptrs());
        
        //        std::cout<<"hello from CutBasedElectronObjectSelector.cc "<<std::endl;

//        if(passSelection && passPhoVeto){
//            std::cout<<"here is a successful electron"<<std::endl;
//            std::cout<<cand.pt()<<" "<<cand.eta()<<" "<<cand.phi()<<std::endl;
//        }
//        else{
//            std::cout<<"this electron"<<std::endl;
//            std::cout<<cand.pt()<<" "<<cand.eta()<<" "<<cand.phi()<<std::endl;
//            std::cout<<"did not pass"<<std::endl;
//        }
//        
        
        return (passSelection && passPhoVeto);
    }

}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

