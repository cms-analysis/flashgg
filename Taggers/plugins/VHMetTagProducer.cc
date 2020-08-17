#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"


#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Met.h"
//#include "DataFormats/PatCandidates/interface/MET.h"
//#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
//#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/DataFormats/interface/VHMetTag.h"

#include "flashgg/DataFormats/interface/VHTagTruth.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "SimDataFormats/HTXS/interface/HiggsTemplateCrossSections.h"

#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class VHMetTagProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        VHMetTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        //EDGetTokenT<View<pat::MET> > METToken_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<flashgg::Met> > METToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;
        edm::InputTag photonCollection_;
        edm::EDGetTokenT<edm::TriggerResults> triggerRECO_;
        edm::EDGetTokenT<edm::TriggerResults> triggerPAT_;
        edm::EDGetTokenT<edm::TriggerResults> triggerFLASHggMicroAOD_;

        EDGetTokenT<int> stage0catToken_, stage1catToken_, njetsToken_;
        EDGetTokenT<HTXS::HiggsClassification> newHTXSToken_;
        EDGetTokenT<float> pTHToken_,pTVToken_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;        

        //configurable selection variables
        bool useVertex0only_;
        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;
        double diphoMVAThreshold_;
        double metPtThreshold_;
        double jetEtaThreshold_;
        double jetPtThreshold_;
        double deltaRPhoLeadJet_;
        double deltaRPhoSubLeadJet_;
        double deltaPhiJetMetThreshold_;
        double phoIdMVAThreshold_;
        double dPhiDiphotonMetThreshold_;
    };

    VHMetTagProducer::VHMetTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        //METToken_( consumes<View<pat::MET> >( iConfig.getParameter<InputTag> ( "METTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        METToken_( consumes<View<flashgg::Met> >( iConfig.getParameter<InputTag> ( "METTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) ),
        triggerRECO_( consumes<edm::TriggerResults>(iConfig.getParameter<InputTag>("RECOfilters") ) ),
        triggerPAT_( consumes<edm::TriggerResults>(iConfig.getParameter<InputTag>("PATfilters") ) ),
        triggerFLASHggMicroAOD_( consumes<edm::TriggerResults>( iConfig.getParameter<InputTag>("FLASHfilters") ) )
    {
        useVertex0only_              = iConfig.getParameter<bool>("useVertex0only");
        leadPhoOverMassThreshold_    = iConfig.getParameter<double>( "leadPhoOverMassThreshold" );
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold" );
        diphoMVAThreshold_           = iConfig.getParameter<double>( "diphoMVAThreshold" );
        metPtThreshold_              = iConfig.getParameter<double>( "metPtThreshold" );
        jetPtThreshold_              = iConfig.getParameter<double>( "jetPtThreshold");
        jetEtaThreshold_             = iConfig.getParameter<double>( "jetEtaThreshold");
        deltaRPhoLeadJet_            = iConfig.getParameter<double>( "deltaRPhoLeadJet");
        deltaRPhoSubLeadJet_         = iConfig.getParameter<double>( "deltaRPhoSubLeadJet");
        deltaPhiJetMetThreshold_     = iConfig.getParameter<double>( "dPhiJetMetThreshold");
        phoIdMVAThreshold_           = iConfig.getParameter<double>( "phoIdMVAThreshold" );
        dPhiDiphotonMetThreshold_    = iConfig.getParameter<double>( "dPhiDiphotonMetThreshold" );

        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
        
        ParameterSet HTXSps = iConfig.getParameterSet( "HTXSTags" );
        stage0catToken_ = consumes<int>( HTXSps.getParameter<InputTag>("stage0cat") );
        stage1catToken_ = consumes<int>( HTXSps.getParameter<InputTag>("stage1cat") );
        njetsToken_ = consumes<int>( HTXSps.getParameter<InputTag>("njets") );
        pTHToken_ = consumes<float>( HTXSps.getParameter<InputTag>("pTH") );
        pTVToken_ = consumes<float>( HTXSps.getParameter<InputTag>("pTV") );
        newHTXSToken_ = consumes<HTXS::HiggsClassification>( HTXSps.getParameter<InputTag>("ClassificationObj") );


        produces<vector<VHMetTag> >();
        produces<vector<VHTagTruth> >();
        photonCollection_=iConfig.getParameter<InputTag> ( "DiPhotonTag" );

    }
    
    void VHMetTagProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<int> stage0cat, stage1cat, njets;
        Handle<float> pTH, pTV;
        evt.getByToken(stage0catToken_, stage0cat);
        evt.getByToken(stage1catToken_,stage1cat);
        evt.getByToken(njetsToken_,njets);
        evt.getByToken(pTHToken_,pTH);
        evt.getByToken(pTVToken_,pTV);

        Handle<HTXS::HiggsClassification> htxsClassification;
        evt.getByToken(newHTXSToken_,htxsClassification);


        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) 
            {
                evt.getByToken( tokenJets_[j], Jets[j] );
            }
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );

        //Handle<View<pat::MET> > METs;
        Handle<View<flashgg::Met> > METs;
        evt.getByToken( METToken_, METs );
        if( METs->size() != 1 )
        { std::cout << "WARNING number of MET is not equal to 1" << std::endl; }
        Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );
        //Ptr<pat::MET> theMET = METs->ptrAt( 0 );

        Handle<View<reco::GenParticle> > genParticles;

        std::unique_ptr<vector<VHMetTag> > vhettags( new vector<VHMetTag> );
        std::unique_ptr<vector<VHTagTruth> > truths( new vector<VHTagTruth> );
        
        Point higgsVtx;
        bool associatedZ=0;
        bool associatedW=0;
        bool VhasDaughters=0;
        bool VhasNeutrinos=0;
        bool VhasLeptons=0;
        bool VhasHadrons=0;
        bool VhasMissingLeptons=0;
        float Vpt=0;
        bool passMETfilters=1;
        //Get trigger results relevant to MET filters
        
        edm::Handle<edm::TriggerResults> triggerBits;
        if(! evt.isRealData() )
            evt.getByToken( triggerPAT_, triggerBits );
        else
            evt.getByToken( triggerRECO_, triggerBits );
        
        edm::Handle<edm::TriggerResults> triggerFLASHggMicroAOD;
        evt.getByToken( triggerFLASHggMicroAOD_, triggerFLASHggMicroAOD );
        const edm::TriggerNames &triggerNames = evt.triggerNames( *triggerBits );
                
        std::vector<std::string> flagList {"Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_goodVertices","Flag_eeBadScFilter"};
        for( unsigned int i = 0; i < triggerNames.triggerNames().size(); i++ ) 
            {
                if(!triggerBits->accept(i))
                for(size_t j=0;j<flagList.size();j++)
                    {
                        if(flagList[j]==triggerNames.triggerName(i))
                            {
                                passMETfilters=0;
                                break;
                            }
                    }
            }
        
        std::vector<std::string> flashggFlagList {"flag_BadChargedCandidateFilter","flag_BadPFMuonFilter","flag_globalTightHalo2016Filter"};
        const edm::TriggerNames &flashggtriggerNames = evt.triggerNames( *triggerFLASHggMicroAOD );
        for( unsigned int i = 0; i < flashggtriggerNames.triggerNames().size(); i++ ) 
            {
                if(!triggerFLASHggMicroAOD->accept(i))
                for(size_t j=0;j<flagList.size();j++)
                    {
                        if(flagList[j]==flashggtriggerNames.triggerName(i))
                            {
                                passMETfilters=0;
                                break;
                            }
                    }
            }
        if( ! evt.isRealData() ) 
            {
                evt.getByToken( genParticleToken_, genParticles );
                for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) 
                    {
                        int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                        int dpdgid[2] = {0,0};
                        if(pdgid ==23) //z-boson
                            {
                                associatedZ=1;
                                if( genParticles->ptrAt( genLoop )->numberOfDaughters()==2)
                                    {
                                        VhasDaughters=1;  
                                        dpdgid[0]=genParticles->ptrAt(genLoop)->daughter(0)->pdgId();
                                        //dpdgid[1]=genParticles->ptrAt(genLoop)->daughter(1)->pdgId();
                                        Vpt=genParticles->ptrAt( genLoop )->pt();
                                        if(fabs(dpdgid[0])==12||fabs(dpdgid[0])==14||fabs(dpdgid[0])==16) //look for neutrino decay of Z
                                            {VhasNeutrinos=1;}
                                        if(fabs(dpdgid[0])==11||fabs(dpdgid[0])==13||fabs(dpdgid[0])==15) //look for lepton decay of Z
                                            {VhasLeptons=1;}
                                        if(fabs(dpdgid[0])>0&&fabs(dpdgid[0])<9) //look for quark decay of Z
                                            { VhasHadrons=1;}
                                    }
                            }
                        if(fabs(pdgid)==24) //look for W
                            {
                                associatedW=1;
                                if( genParticles->ptrAt( genLoop )->numberOfDaughters()==2)
                                    {
                                        VhasDaughters=1;
                                        Vpt=genParticles->ptrAt( genLoop )->pt();
                                        dpdgid[0]=genParticles->ptrAt(genLoop)->daughter(0)->pdgId();
                                        //dpdgid[1]=genParticles->ptrAt(genLoop)->daughter(1)->pdgId();
                                        if(fabs(dpdgid[0])==12||fabs(dpdgid[0])==14||fabs(dpdgid[0])==16) //look for neutrino decay of W
                                            {VhasNeutrinos=1;VhasLeptons=1;}
                                        if(fabs(dpdgid[0])==11||fabs(dpdgid[0])==13||fabs(dpdgid[0])==15) //look for lepton decay of W
                                            {VhasNeutrinos=1;VhasLeptons=1;}
                                        if(fabs(dpdgid[0])>0&&fabs(dpdgid[0])<9) //look for quark decay of W
                                            {VhasHadrons=1;}
                                        
                                    }
                            }
                        if( pdgid == 25 || pdgid == 22 ) 
                            {
                                higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                                continue;
                            }
                    }
            }
        
            
        edm::RefProd<vector<VHTagTruth> > rTagTruth = evt.getRefBeforePut<vector<VHTagTruth> >();
        unsigned int idx = 0;
        
        
        assert( diPhotons->size() ==
                mvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception
        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );
            //skip events failing met filters
            if(!passMETfilters)   {continue;}
            //skip events where selected vertex is not vertex 0
            if(useVertex0only_)
                if(diPhotons->ptrAt(candIndex)->vertexIndex()!=0)   {continue;}
            //diphoton pt cuts
            if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ ) { continue; }
            if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ ) { continue; }
            //photon mva preselection
            if( dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() ) <= phoIdMVAThreshold_ ) { continue; }
            if( dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() ) <= phoIdMVAThreshold_ ) { continue; }
            //diphoton MVA preselection
            if( mvares->result < diphoMVAThreshold_ )          { continue; }

            unsigned int jetCollectionIndex = diPhotons->ptrAt( candIndex )->jetCollectionIndex();
            //edm::Ptr<flashgg::Jet> maxJet();
            std::vector<edm::Ptr<Jet> > tagJets;
            for( unsigned int jetIndex = 0; jetIndex < Jets[jetCollectionIndex]->size() ; jetIndex++ )
                {
                    edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( jetIndex );
                    if(!thejet->passesJetID  ( flashgg::Tight2017 ) ) { continue; }
                    if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }
                    if( thejet->pt() < jetPtThreshold_ ) { continue; }
                    float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), diPhotons->ptrAt( candIndex )->leadingPhoton()->superCluster()->eta(), diPhotons->ptrAt( candIndex )->leadingPhoton()->superCluster()->phi() ) ;
                    float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), diPhotons->ptrAt( candIndex )->subLeadingPhoton()->superCluster()->eta(),diPhotons->ptrAt( candIndex )->subLeadingPhoton()->superCluster()->phi() );
                    if( dRPhoLeadJet < deltaRPhoLeadJet_ || dRPhoSubLeadJet < deltaRPhoSubLeadJet_ ) { continue;}
                    //maxJet = thejet;
                    tagJets.push_back( thejet);
                }
            
            
            VHMetTag tag_obj( dipho, mvares );
            tag_obj.includeWeights( *dipho );
            tag_obj.setDiPhotonIndex( candIndex );
            tag_obj.setSystLabel( systLabel_ );
            tag_obj.setMet( theMET );
            if(tagJets.size())
                tag_obj.setJet(tagJets[0]);
            //float newPhi=theMET->corPhi(pat::MET::Type1XY);  //xy-correction is worse | don't use newPhi
            // if(fabs(deltaPhi(newPhi,dipho->phi()))<dPhiDiphotonMetThreshold_)                              
            if(fabs(deltaPhi(theMET->corPhi(),dipho->phi()))<dPhiDiphotonMetThreshold_)   { continue;}
            if(tagJets.size())
                if(fabs(deltaPhi(dipho->phi(),tagJets[0]->phi()))>deltaPhiJetMetThreshold_)    {continue;}
            if(theMET->getCorPt()< metPtThreshold_ )   {continue;}

            
            
            vhettags->push_back( tag_obj );
            if( ! evt.isRealData() ) 
                {
                    VHTagTruth truth_obj;
                    truth_obj.setGenPV( higgsVtx );
                    if ( stage0cat.isValid() ) {
                        truth_obj.setHTXSInfo( *( stage0cat.product() ),
                                               *( stage1cat.product() ),
                                               *( njets.product() ),
                                               *( pTH.product() ),
                                               *( pTV.product() ) );
                    } else if ( htxsClassification.isValid() ) {
                        truth_obj.setHTXSInfo( htxsClassification->stage0_cat,
                                               htxsClassification->stage1_cat_pTjet30GeV,
                                               htxsClassification->jets30.size(),
                                               htxsClassification->p4decay_higgs.pt(),
                                               htxsClassification->p4decay_V.pt() );

                    } else {
                        truth_obj.setHTXSInfo( 0, 0, 0, 0., 0. );
                    }
                    truth_obj.setAssociatedZ( associatedZ );
                    truth_obj.setAssociatedW( associatedW );
                    truth_obj.setVhasDaughters( VhasDaughters );
                    truth_obj.setVhasNeutrinos( VhasNeutrinos );
                    truth_obj.setVhasLeptons( VhasLeptons );
                    truth_obj.setVhasHadrons( VhasHadrons );
                    truth_obj.setVhasMissingLeptons( VhasMissingLeptons );
                    truth_obj.setVpt( Vpt );
                    truths->push_back( truth_obj );
                    vhettags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<VHTagTruth> >( rTagTruth, idx++ ) ) );
                }
        }
        evt.put( std::move( vhettags ) );
        evt.put( std::move( truths ) );
    }
}

typedef flashgg::VHMetTagProducer FlashggVHMetTagProducer;
DEFINE_FWK_MODULE( FlashggVHMetTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

