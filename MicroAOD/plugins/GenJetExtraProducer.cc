#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "flashgg/DataFormats/interface/GenJetExtra.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "flashgg/MicroAOD/interface/PhotonMCUtils.h"
#include "PhysicsTools/JetMCUtils/interface/CandMCTag.h"
#include "CommonTools/CandUtils/interface/pdgIdUtils.h"
//#include "flashgg/MicroAOD/interface/JetMCUtils.h"


using namespace std;
using namespace edm;

namespace flashgg {

    class GenJetExtraProducer : public EDProducer
    {

    public:
        GenJetExtraProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        //        EDGetTokenT<View<pat::PackedGenParticle> > genJetsToken_;
        //        EDGetTokenT<View<pat::PackedGenParticle> > genParticlesToken_;
        
        //        edm::EDGetTokenT<reco::GenParticleCollection> genJetsToken_;
        edm::EDGetTokenT<View<reco::GenJet> > genJetsToken_;
        double isoConeSize_, dressingConeSize_;
        std::vector<int> promptMothers_;
        //        flashgg::GenJetExtra::match_type defaultType_;
    };


    GenJetExtraProducer::GenJetExtraProducer( const ParameterSet &iConfig ) :
        //        genJetsToken_( consumes<View<reco::GenJet> >( iConfig.getParameter<InputTag>( "genJets" ) ) )
        genJetsToken_( consumes<View<reco::GenJet> >( iConfig.getParameter<edm::InputTag>( "genJets" ) ) )
    {
        produces<vector<flashgg::GenJetExtra> >();
    }

    void GenJetExtraProducer::produce( Event &evt, const EventSetup &iSetup )
    {

        Handle<View<reco::GenJet> > genJets;
        evt.getByToken( genJetsToken_, genJets );
        unique_ptr<vector<flashgg::GenJetExtra> > extraColl( new vector<flashgg::GenJetExtra> );
        std::vector<edm::Ptr<reco::Candidate> > constituents;
        
        for( auto &genJet : *genJets ){
            flashgg::GenJetExtra extra( genJet );
            bool hasBottom = false;
            bool hasBquark = false;
            constituents = genJet.getJetConstituents(); 

            //first, look at mother of constituents of jet and look for B hadrons/mesons
            for(auto &constituent : constituents){

                if (!(constituent.isNonnull()) ) continue;   
                for (size_t m = 0; m < (*(constituent)).numberOfMothers(); m++){                       int mpdgid =  abs((*(constituent)).mother(m)->pdgId());
                    if(CandMCTagUtils::hasBottom( *( (*(constituent)).mother(m) ) ) ){                     
                        hasBottom=true;
                        break;
                    } 
                }
                if(hasBottom == true) break;
            }
            extra.setHasBottom( hasBottom );
            
            double dR_jb = 9999.;
            double jetPtOverBquarkPt = -999.;
            for(auto &constituent : constituents){
                if (!(constituent.isNonnull()) ) continue;
                for (size_t m = 0; m < (*(constituent)).numberOfMothers(); m++){    
                    int mpdgid =  abs((*(constituent)).mother(m)->pdgId());
                    if( mpdgid ==5 ){ 
                        //if it is a bquark, calculate dR wrt the jet...
                        double thisdR_jb = reco::deltaR(genJet,  *((*(constituent)).mother(m)) );
                        if(thisdR_jb < dR_jb){
                            dR_jb = thisdR_jb;
                            jetPtOverBquarkPt = genJet.pt()/((*(constituent)).mother(m)->pt());
                            hasBquark = true;
                        }
                    } 
                }
            }
            //if there is at least a b quark
            if(hasBquark){
                //take the one closest to the jet in dR
                extra.setHasBquark(true);
                extra.setDeltaRBquarkGenjet(dR_jb);
                extra.setJetPtOverBquarkPt(jetPtOverBquarkPt);
            }
            else{
                extra.setHasBquark(false);
                extra.setDeltaRBquarkGenjet(-999.);
                extra.setJetPtOverBquarkPt(-999.);
            }            
            

            extraColl->push_back( extra );
        }

        evt.put( std::move( extraColl ) );

        /// orig_collection = 0;
    }
}



typedef flashgg::GenJetExtraProducer FlashggGenJetExtraProducer;
DEFINE_FWK_MODULE( FlashggGenJetExtraProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

