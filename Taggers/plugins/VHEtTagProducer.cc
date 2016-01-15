#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/PatCandidates/interface/MET.h"
//#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
//#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/DataFormats/interface/VHEtTag.h"

#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class VHEtTagProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        VHEtTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<pat::MET> > METToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;

        //configurable selection variables
        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;
        double diphoMVAThreshold_;
        double metPtThreshold_;
        double phoIdMVAThreshold_;
    };

    VHEtTagProducer::VHEtTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        METToken_( consumes<View<pat::MET> >( iConfig.getParameter<InputTag> ( "METTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
    {
        leadPhoOverMassThreshold_    = iConfig.getParameter<double>( "leadPhoOverMassThreshold" );
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold" );
        diphoMVAThreshold_           = iConfig.getParameter<double>( "diphoMVAThreshold" );
        metPtThreshold_              = iConfig.getParameter<double>( "metPtThreshold" );
        phoIdMVAThreshold_           = iConfig.getParameter<double>( "phoIdMVAThreshold" );

        produces<vector<VHEtTag> >();
        produces<vector<TagTruthBase> >();
    }

    void VHEtTagProducer::produce( Event &evt, const EventSetup & )
    {
        //        std::cout << "starting met tagger" << std::endl;
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );

        Handle<View<pat::MET> > METs;
        evt.getByToken( METToken_, METs );

        if( METs->size() != 1 )
        { std::cout << "WARNING number of MET is not equal to 1" << std::endl; }
        Ptr<pat::MET> theMET = METs->ptrAt( 0 );

        Handle<View<reco::GenParticle> > genParticles;

        std::auto_ptr<vector<VHEtTag> > vhettags( new vector<VHEtTag> );
        std::auto_ptr<vector<TagTruthBase> > truths( new vector<TagTruthBase> );

        Point higgsVtx;

        if( ! evt.isRealData() ) {
            evt.getByToken( genParticleToken_, genParticles );
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                if( pdgid == 25 || pdgid == 22 ) {
                    higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                    break;
                }
            }
        }

        edm::RefProd<vector<TagTruthBase> > rTagTruth = evt.getRefBeforePut<vector<TagTruthBase> >();
        unsigned int idx = 0;

        assert( diPhotons->size() ==
                mvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception

        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {


            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );

            //diphoton pt cuts
            if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ )
            { continue; }
            if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ )
            { continue; }
            //photon mva preselection
            if( dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() ) <= phoIdMVAThreshold_ )
            { continue; }
            if( dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() ) <= phoIdMVAThreshold_ )
            { continue; }
            //diphoton MVA preselection
            if( mvares->result < diphoMVAThreshold_ )
            { continue; }



            VHEtTag tag_obj( dipho, mvares );
            tag_obj.includeWeights( *dipho );
            tag_obj.setDiPhotonIndex( candIndex );
            tag_obj.setSystLabel( systLabel_ );
            tag_obj.setMet( theMET );


            //MetCorrections2012_Simple(leadp4,subleadp4);
            //if(diphoton.mass()>100&&diphoton.mass()<180)
            {
                //calculate met
            }
            //std::cout << "Met: " << theMET->pt() << std::endl;
            if( theMET->pt() > metPtThreshold_ ) {
                //setdiphotonindex
                //setMET
                vhettags->push_back( tag_obj );
                if( ! evt.isRealData() ) {
                    TagTruthBase truth_obj;
                    truth_obj.setGenPV( higgsVtx );
                    truths->push_back( truth_obj );
                    vhettags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, idx++ ) ) );
                }
            }
        }
        evt.put( vhettags );
        evt.put( truths );
    }
}

typedef flashgg::VHEtTagProducer FlashggVHEtTagProducer;
DEFINE_FWK_MODULE( FlashggVHEtTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

