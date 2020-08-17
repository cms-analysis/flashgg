#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/GenDiPhoton.h"

#include "flashgg/DataFormats/interface/GenPhotonExtra.h"

#include "TMVA/Reader.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;

namespace flashgg {

    class GenDiPhotonProducer : public EDProducer
    {

    public:
        GenDiPhotonProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<GenPhotonExtra> > genPhotonToken_;
    };

    GenDiPhotonProducer::GenDiPhotonProducer( const ParameterSet &iConfig ) :
        genPhotonToken_( consumes<View<flashgg::GenPhotonExtra> >( iConfig.getParameter<InputTag> ( "src" ) ) )
    {
        produces<vector<GenDiPhoton> >(); // one per diphoton, always in same order, vector is more efficient than map
    }
    
    void GenDiPhotonProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::GenPhotonExtra> > photons;
        evt.getByToken( genPhotonToken_, photons );

        std::unique_ptr<vector<GenDiPhoton> > diphotons( new vector<GenDiPhoton> );

        for( size_t ii = 0 ; ii < photons->size() ; ++ii ) {
            auto pi = photons->ptrAt( ii );
            for( size_t jj = ii + 1 ; jj < photons->size() ; ++jj ) {
                auto pj = photons->ptrAt( jj );
                diphotons->push_back(GenDiPhoton(pi,pj));
            }
        }
        
        evt.put( std::move( diphotons ) );
    }
}

typedef flashgg::GenDiPhotonProducer FlashggGenDiPhotonProducer;
DEFINE_FWK_MODULE( FlashggGenDiPhotonProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

