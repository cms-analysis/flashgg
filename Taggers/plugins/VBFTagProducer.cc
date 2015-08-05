#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/VBFTagTruth.h"

#include "DataFormats/Common/interface/RefToPtr.h"

#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class VBFTagProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        VBFTagProducer( const ParameterSet & );

    private:
        void produce( Event &, const EventSetup & ) override;
        int chooseCategory( float );

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<VBFDiPhoDiJetMVAResult> > vbfDiPhoDiJetMvaResultToken_;
        EDGetTokenT<View<VBFMVAResult> > vbfMvaResultToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<reco::GenParticle> > genPartToken_;
        EDGetTokenT<View<reco::GenJet> > genJetToken_;
        string systLabel_;
        int systLabelHash_;

        vector<double> boundaries;

    };

    VBFTagProducer::VBFTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        vbfDiPhoDiJetMvaResultToken_( consumes<View<flashgg::VBFDiPhoDiJetMVAResult> >( iConfig.getParameter<InputTag> ( "VBFDiPhoDiJetMVAResultTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        genPartToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        genJetToken_( consumes<View<reco::GenJet> >( iConfig.getParameter<InputTag> ( "GenJetTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) ),
        systLabelHash_( iConfig.getParameter<int> ( "SystLabelHash" ) )
    {
        vector<double> default_boundaries;
        default_boundaries.push_back( 0.52 );
        default_boundaries.push_back( 0.85 );
        default_boundaries.push_back( 0.915 );
        default_boundaries.push_back( 1 ); // from here
        //https://github.com/h2gglobe/h2gglobe/blob/master/AnalysisScripts/massfac_mva_binned/massfactorizedmvaanalysis.dat#L32

        // getUntrackedParameter<vector<float> > has no library, so we use double transiently
        boundaries = iConfig.getUntrackedParameter<vector<double > >( "Boundaries", default_boundaries );

        assert( is_sorted( boundaries.begin(), boundaries.end() ) ); // we are counting on ascending order - update this to give an error message or exception

        produces<vector<VBFTag> >();
        produces<vector<VBFTagTruth> >();
    }

    int VBFTagProducer::chooseCategory( float mvavalue )
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n;
        for( n = 0 ; n < ( int )boundaries.size() ; n++ ) {
            if( ( double )mvavalue > boundaries[boundaries.size() - n - 1] ) { return n; }
        }
        return -1; // Does not pass, object will not be produced
    }

    void VBFTagProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );

        Handle<View<flashgg::VBFDiPhoDiJetMVAResult> > vbfDiPhoDiJetMvaResults;
        evt.getByToken( vbfDiPhoDiJetMvaResultToken_, vbfDiPhoDiJetMvaResults );

        Handle<View<reco::GenParticle> > genParticles;
        evt.getByToken( genPartToken_, genParticles );

        Handle<View<reco::GenJet> > genJets;
        evt.getByToken( genJetToken_, genJets );

        std::auto_ptr<vector<VBFTag> > tags( new vector<VBFTag> );
        std::auto_ptr<vector<VBFTagTruth> > truths( new vector<VBFTagTruth> );

        unsigned int idx = 0;
        edm::RefProd<vector<VBFTagTruth> > rTagTruth = evt.getRefBeforePut<vector<VBFTagTruth> >();

        unsigned int index_leadq = std::numeric_limits<unsigned int>::max(), index_subleadq = std::numeric_limits<unsigned int>::max();
        float pt_leadq = 0., pt_subleadq = 0.;
        Point higgsVtx;

        if( ! evt.isRealData() ) {
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                if( pdgid == 25 || pdgid == 22 ) {
                    higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                    break;
                }
            }
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                edm::Ptr<reco::GenParticle> part = genParticles->ptrAt( genLoop );
                if( part->isHardProcess() ) {
                    if( abs( part->pdgId() ) <= 5 ) {
                        if( part->pt() > pt_leadq ) {
                            index_subleadq = index_leadq;
                            pt_subleadq = pt_leadq;
                            index_leadq = genLoop;
                            pt_leadq = part->pt();
                        } else if( part->pt() > pt_subleadq ) {
                            index_subleadq = genLoop;
                            pt_subleadq = part->pt();
                        }
                    }
                }
            }
        }
        assert( diPhotons->size() == vbfDiPhoDiJetMvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception
        assert( diPhotons->size() ==
                mvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception

        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::VBFDiPhoDiJetMVAResult> vbfdipho_mvares = vbfDiPhoDiJetMvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );

            VBFTag tag_obj( dipho, mvares, vbfdipho_mvares );
            tag_obj.setDiPhotonIndex( candIndex );

            tag_obj.setSystLabel( systLabel_ );
            tag_obj.setSystLabelHash( systLabelHash_ );

            int catnum = chooseCategory( vbfdipho_mvares->vbfDiPhoDiJetMvaResult );
            tag_obj.setCategoryNumber( catnum );
            unsigned int index_gp_leadjet = std::numeric_limits<unsigned int>::max();
            unsigned int index_gp_subleadjet = std::numeric_limits<unsigned int>::max();
            unsigned int index_gp_leadphoton = std::numeric_limits<unsigned int>::max();
            unsigned int index_gp_subleadphoton = std::numeric_limits<unsigned int>::max();
            unsigned int index_gj_leadjet = std::numeric_limits<unsigned int>::max();
            unsigned int index_gj_subleadjet = std::numeric_limits<unsigned int>::max();
            float dr_gp_leadjet = 999.;
            float dr_gp_subleadjet = 999.;
            float dr_gp_leadphoton = 999.;
            float dr_gp_subleadphoton = 999.;
            float dr_gj_leadjet = 999.;
            float dr_gj_subleadjet = 999.;
            VBFTagTruth truth_obj;
            if( ! evt.isRealData() ) {
                for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                    edm::Ptr<reco::GenParticle> part = genParticles->ptrAt( genLoop );
                    if( part->isHardProcess() ) {
                        float dr = deltaR( tag_obj.leadingJet().eta(), tag_obj.leadingJet().phi(), part->eta(), part->phi() );
                        if( dr < dr_gp_leadjet ) {
                            dr_gp_leadjet = dr;
                            index_gp_leadjet = genLoop;
                        }
                        dr = deltaR( tag_obj.subLeadingJet().eta(), tag_obj.subLeadingJet().phi(), part->eta(), part->phi() );
                        if( dr < dr_gp_subleadjet ) {
                            dr_gp_subleadjet = dr;
                            index_gp_subleadjet = genLoop;
                        }
                        dr = deltaR( tag_obj.diPhoton()->leadingPhoton()->eta(), tag_obj.diPhoton()->leadingPhoton()->phi(), part->eta(), part->phi() );
                        if( dr < dr_gp_leadphoton ) {
                            dr_gp_leadphoton = dr;
                            index_gp_leadphoton = genLoop;
                        }
                        dr = deltaR( tag_obj.diPhoton()->subLeadingPhoton()->eta(), tag_obj.diPhoton()->subLeadingPhoton()->phi(), part->eta(), part->phi() );
                        if( dr < dr_gp_subleadphoton ) {
                            dr_gp_subleadphoton = dr;
                            index_gp_subleadphoton = genLoop;
                        }
                    }
                }

                if( index_gp_leadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToLeadingJet( genParticles->ptrAt( index_gp_leadjet ) ); }
                if( index_gp_subleadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToSubLeadingJet( genParticles->ptrAt( index_gp_subleadjet ) ); }
                if( index_gp_leadphoton < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToLeadingPhoton( genParticles->ptrAt( index_gp_leadphoton ) ); }
                if( index_gp_subleadphoton < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestParticleToSubLeadingPhoton( genParticles->ptrAt( index_gp_subleadphoton ) ); }
                for( unsigned int gjLoop = 0 ; gjLoop < genJets->size() ; gjLoop++ ) {
                    edm::Ptr<reco::GenJet> gj = genJets->ptrAt( gjLoop );
                    float dr = deltaR( tag_obj.leadingJet().eta(), tag_obj.leadingJet().phi(), gj->eta(), gj->phi() );
                    if( dr < dr_gj_leadjet ) {
                        dr_gj_leadjet = dr;
                        index_gj_leadjet = gjLoop;
                    }
                    dr = deltaR( tag_obj.subLeadingJet().eta(), tag_obj.subLeadingJet().phi(), gj->eta(), gj->phi() );
                    if( dr < dr_gj_subleadjet ) {
                        dr_gj_subleadjet = dr;
                        index_gj_subleadjet = gjLoop;
                    }
                }
                if( index_gj_leadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestGenJetToLeadingJet( genJets->ptrAt( index_gj_leadjet ) ); }
                if( index_gj_subleadjet < std::numeric_limits<unsigned int>::max() ) { truth_obj.setClosestGenJetToSubLeadingJet( genJets->ptrAt( index_gj_subleadjet ) ); }

                if( index_leadq < std::numeric_limits<unsigned int>::max() ) { truth_obj.setLeadingQuark( genParticles->ptrAt( index_leadq ) ); }
                if( index_subleadq < std::numeric_limits<unsigned int>::max() ) { truth_obj.setSubLeadingQuark( genParticles->ptrAt( index_subleadq ) ); }
            }
            truth_obj.setGenPV( higgsVtx );
            if( tag_obj.categoryNumber() >= 0 ) {
                tags->push_back( tag_obj );
                if( ! evt.isRealData() ) {
                    truths->push_back( truth_obj );
                    tags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<VBFTagTruth> >( rTagTruth, idx++ ) ) );
                }
            }
        }

        evt.put( tags );
        evt.put( truths );
    }
}

typedef flashgg::VBFTagProducer FlashggVBFTagProducer;
DEFINE_FWK_MODULE( FlashggVBFTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

