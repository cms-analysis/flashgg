#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TMVA/Reader.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;

namespace flashgg {

    class HiggsGenJetsSelector : public EDProducer
    {

    public:
        HiggsGenJetsSelector( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        
        reco::GenJet addNeutrinos( const reco::GenJet &, const View<reco::Candidate> &);
        
        EDGetTokenT<View<reco::GenParticle> > genPartToken_;
        EDGetTokenT<View<reco::GenJet> > genJetToken_;
        EDGetTokenT<View<reco::Candidate> > genNuToken_;
        
        bool usePartons_;
    };
    
    HiggsGenJetsSelector::HiggsGenJetsSelector( const ParameterSet &iConfig ) :
        genPartToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "src" ) ) ),
        genJetToken_( consumes<View<reco::GenJet> >( iConfig.getParameter<InputTag> ( "jets" ) ) ),
        genNuToken_( consumes<View<reco::Candidate> >( iConfig.getParameter<InputTag> ( "nus" ) ) ),
        usePartons_( true )
    {
        if( iConfig.exists("usePartons") ) { 
            usePartons_ = iConfig.getParameter<bool>("usePartons");
        }
        produces<vector<reco::GenJet> >();
    }
    
    void HiggsGenJetsSelector::produce( Event &evt, const EventSetup & )
    {
        Handle<View<reco::GenParticle> > genParts;
        evt.getByToken( genPartToken_, genParts );

        Handle<View<reco::GenJet> > jets;
        evt.getByToken( genJetToken_, jets );

        Handle<View<reco::Candidate> > genNus;
        evt.getByToken( genNuToken_, genNus );

        std::unique_ptr<vector<reco::GenJet> > hjets( new vector<reco::GenJet> );
        
        edm::Ptr<reco::GenParticle> bq, bbarq; // FIXME assumes only one b-bbar pair in the event, does not check if it comes from H
        for( size_t ip = 0 ; ip < genParts->size() ; ++ip ) {
            auto ipart = genParts->ptrAt(ip);
            if( ipart->motherRefVector().size() == 0 ) { continue; }
            bool higgschild = false;
            for( auto mom : ipart->motherRefVector() ) {
                if( mom->pdgId() == 25 ) { 
                    higgschild  = true;
                    break;
                }
            }
            if( ! higgschild ) { continue; }
            if( ipart->pdgId() == 5 && bq.isNull() ) { bq = ipart; }
            else if( ipart->pdgId() == -5 && bbarq.isNull() ) { bbarq = ipart; } // FIXME handle duplicates
        }
        // std::cout << "found b quarks " << bq.isNull() << " " << bbarq.isNull() << std::endl;

        if( !(bq.isNull()||bbarq.isNull()) ) {
            if( usePartons_ ) { 
                hjets->push_back( reco::GenJet(bq->p4(),bq->vertex(),reco::GenJet::Specific(),reco::GenJet::Constituents(1,bq)) );
                hjets->push_back( reco::GenJet(bbarq->p4(),bbarq->vertex(),reco::GenJet::Specific(),reco::GenJet::Constituents(1,bbarq)) );
            } else { 
                edm::Ptr<reco::GenJet> bj, bbarj;
                float bjDr = 0.4, bbarjDr = 0.4; // FIXME make configurable
                float minbbarDr = 999., minbDr = 999.;
                for( size_t ij = 0 ; ij < jets->size() ; ++ij ) {                
                    auto ijet = jets->ptrAt( ij );
                    float bDr = reco::deltaR(*ijet,*bq);
                    if( bDr < bjDr ) {
                        bj = ijet;
                        bjDr = bDr;
                    }
                    float bbarDr = reco::deltaR(*ijet,*bbarq);
                    if( bbarDr < bbarjDr ) {
                        bbarj = ijet;
                        bbarjDr = bbarDr;
                    }
                    minbDr = std::min( bDr, minbDr );
                    minbbarDr = std::min( bbarDr, minbbarDr );
                } // FIXME handle FSR
                /// std::cout << "found b jets " << bj.isNull() << " " << bbarj.isNull() << " " << minbDr << " " << minbbarDr << " " << bq->pt() << " " << bbarq->pt() << std::endl;
                if(! bj.isNull() ) { hjets->push_back(addNeutrinos(*bj,*genNus));  }
                if(! bbarj.isNull() ) { hjets->push_back(addNeutrinos(*bbarj,*genNus));  }
            }
        }
        evt.put( std::move( hjets ) );
    }
    
    reco::GenJet HiggsGenJetsSelector::addNeutrinos( const reco::GenJet & jet, const View<reco::Candidate> & nus)
    {
        auto p4 = jet.p4();
        auto constituents = jet.getJetConstituents();
        auto specific = jet.getSpecific();
        auto vertex = jet.vertex();
        for(size_t in = 0; in<nus.size(); in++) {
            auto inu = nus.ptrAt(in);
            if( reco::deltaR(*inu, jet) < 0.4 ) { // FIXME make configurable
                constituents.push_back(inu);
                specific.m_InvisibleEnergy += inu->energy();
                p4 += inu->p4();
            }
        }
        
        return reco::GenJet(p4,vertex,specific,constituents);
    }


}

typedef flashgg::HiggsGenJetsSelector FlashggHiggsGenJetsSelector;
DEFINE_FWK_MODULE( FlashggHiggsGenJetsSelector );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

