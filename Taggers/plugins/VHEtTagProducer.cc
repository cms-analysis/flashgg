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

#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class VHEtTagProducer : public EDProducer
    {

    public:
        VHEtTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<pat::MET> > METToken_;
        //configurable selection variables
        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;
        double diphoMVAThreshold_;
        double metPtThreshold_;
        double phoIdMVAThreshold_;
    };

    VHEtTagProducer::VHEtTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getUntrackedParameter<InputTag> ( "DiPhotonTag", InputTag( "flashggDiPhotons" ) ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getUntrackedParameter<InputTag> ( "MVAResultTag", InputTag( "flashggDiPhotonMVA" ) ) ) ),
        METToken_( consumes<View<pat::MET> >( iConfig.getUntrackedParameter<InputTag> ( "METTag", InputTag( "slimmedMETs" ) ) ) )
    {
        leadPhoOverMassThreshold_    = iConfig.getUntrackedParameter<double>( "leadPhoOverMassThreshold", 0.334 );
        subleadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>( "subleadPhoOverMassThreshold", 0.25 );
        diphoMVAThreshold_           = iConfig.getUntrackedParameter<double>( "diphoMVAThreshold", -1.0 );
        metPtThreshold_              = iConfig.getUntrackedParameter<double>( "metPtThreshold", 70.0 );
        phoIdMVAThreshold_           = iConfig.getUntrackedParameter<double>( "phoIdMVAThreshold", -0.2 );

        produces<vector<VHEtTag> >();
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

        std::auto_ptr<vector<VHEtTag> > vhettags( new vector<VHEtTag> );

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
            tag_obj.setDiPhotonIndex( candIndex );
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
            }
        }
        evt.put( vhettags );
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

