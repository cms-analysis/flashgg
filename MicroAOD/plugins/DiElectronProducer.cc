#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "flashgg/DataFormats/interface/DiElectronCandidate.h"

using namespace edm;
using namespace std;

namespace flashgg {

    class DiElectronProducer : public EDProducer
    {

    public:
        DiElectronProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<flashgg::Electron> > electronToken_;

        double minElectronPt_;
        double maxElectronEta_;

    };
    
    DiElectronProducer::DiElectronProducer( const ParameterSet &iConfig ) :
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag> ( "ElectronTag" ) ) )
    {
        minElectronPt_ = iConfig.getParameter<double>( "minElectronPt" );
        maxElectronEta_ = iConfig.getParameter<double>( "maxElectronEta" );
        produces<vector<flashgg::DiElectronCandidate> >();
    }

    void DiElectronProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::Electron> > electrons;
        evt.getByToken( electronToken_, electrons );
        const std::vector<edm::Ptr<flashgg::Electron> > &electronPointers = electrons->ptrs();

        unique_ptr<vector<flashgg::DiElectronCandidate> > diElectronColl( new vector<flashgg::DiElectronCandidate> );

        for( unsigned int i = 0 ; i < electronPointers.size() ; i++ ) {
            Ptr<flashgg::Electron> electron1 = electronPointers[i];
            double pt1 = electron1->pt();
            double eta1 = electron1->eta();
            if( pt1 < minElectronPt_ || fabs( eta1 ) > maxElectronEta_ ) { continue; }
            for( unsigned int j = i + 1 ; j < electronPointers.size() ; j++ ) {
                Ptr<flashgg::Electron> electron2 = electronPointers[j];
                double pt2 = electron2->pt();
                double eta2 = electron2->eta();
                if( pt2 < minElectronPt_ || fabs( eta2 ) > maxElectronEta_ ) { continue; }

                Ptr<flashgg::Electron> leadElectron = electronPointers[i];
                Ptr<flashgg::Electron> subLeadElectron = electronPointers[j];
                if( pt2 > pt1 ) {
                    leadElectron = electronPointers[j];
                    subLeadElectron = electronPointers[i];
                }

                DiElectronCandidate diEle( leadElectron, subLeadElectron );

                // store the dielectron into the collection
                diElectronColl->push_back( diEle );
            }
        }
        evt.put( std::move( diElectronColl ) );

    }
}

typedef flashgg::DiElectronProducer FlashggDiElectronProducer;
DEFINE_FWK_MODULE( FlashggDiElectronProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
