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
#include "flashgg/DataFormats/interface/VHEtTag.h"

#include "flashgg/DataFormats/interface/VHTagTruth.h"
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
        //EDGetTokenT<View<pat::MET> > METToken_;
        EDGetTokenT<View<flashgg::Met> > METToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;
        edm::InputTag photonCollection_;
        edm::EDGetTokenT<edm::TriggerResults> triggerRECO_;
        edm::EDGetTokenT<edm::TriggerResults> triggerPAT_;
        edm::EDGetTokenT<edm::TriggerResults> triggerFLASHggMicroAOD_;
        
        //configurable selection variables
        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;
        double diphoMVAThreshold_;
        double metPtThreshold_;
        double phoIdMVAThreshold_;
        double dPhiDiphotonMetThreshold_;
    };

    VHEtTagProducer::VHEtTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        //METToken_( consumes<View<pat::MET> >( iConfig.getParameter<InputTag> ( "METTag" ) ) ),
        METToken_( consumes<View<flashgg::Met> >( iConfig.getParameter<InputTag> ( "METTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) ),
        triggerRECO_( consumes<edm::TriggerResults>(iConfig.getParameter<InputTag>("RECOfilters") ) ),
        triggerPAT_( consumes<edm::TriggerResults>(iConfig.getParameter<InputTag>("PATfilters") ) ),
        triggerFLASHggMicroAOD_( consumes<edm::TriggerResults>( iConfig.getParameter<InputTag>("FLASHfilters") ) )
    {
        leadPhoOverMassThreshold_    = iConfig.getParameter<double>( "leadPhoOverMassThreshold" );
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold" );
        diphoMVAThreshold_           = iConfig.getParameter<double>( "diphoMVAThreshold" );
        metPtThreshold_              = iConfig.getParameter<double>( "metPtThreshold" );
        phoIdMVAThreshold_           = iConfig.getParameter<double>( "phoIdMVAThreshold" );
        dPhiDiphotonMetThreshold_    = iConfig.getParameter<double>( "dPhiDiphotonMetThreshold" );

        produces<vector<VHEtTag> >();
        produces<vector<VHTagTruth> >();
        photonCollection_=iConfig.getParameter<InputTag> ( "DiPhotonTag" );

    }
    
    void VHEtTagProducer::produce( Event &evt, const EventSetup & )
    {
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

        std::unique_ptr<vector<VHEtTag> > vhettags( new vector<VHEtTag> );
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
                            //if(flagList(j)==triggerNames.triggerName(i))
                            if(flagList[j]==triggerNames.triggerName(i))
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
                                            {
                                                VhasNeutrinos=1;
                                            }
                                        if(fabs(dpdgid[0])==11||fabs(dpdgid[0])==13||fabs(dpdgid[0])==15) //look for lepton decay of Z
                                            {
                                                VhasLeptons=1;
                                            }
                                        if(fabs(dpdgid[0])>0&&fabs(dpdgid[0])<9) //look for quark decay of Z
                                            {                                        
                                                VhasHadrons=1;
                                            }
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
                                            {
                                                VhasNeutrinos=1;
                                                VhasLeptons=1;
                                            }
                                        if(fabs(dpdgid[0])==11||fabs(dpdgid[0])==13||fabs(dpdgid[0])==15) //look for lepton decay of W
                                            {
                                                VhasNeutrinos=1;
                                                VhasLeptons=1;
                                            }
                                        if(fabs(dpdgid[0])>0&&fabs(dpdgid[0])<9) //look for quark decay of W
                                            {
                                                VhasHadrons=1;
                                            }
                                        
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
            if(!passMETfilters)
                {
                    continue;
                }
            /*
              if(candIndex==0)
                {
                    if(VhasDaughters==0)
                        std::cout << "--------------V has no children ------------" << std::endl;
                    if(VhasLeptons && associatedW)
                        std::cout << "W-> leptons ---tight with    met>45" << std::endl;
                    if(VhasLeptons && associatedZ)
                        std::cout << "Z-> leptons ---tight with 2 leptons" << std::endl;
                    if(VhasHadrons)
                        std::cout << "V-> hadrons ---          hadron tag" << std::endl;
            }
            */
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
            
            //apply phi correction
           float px_0 = 2.74771;
           float px_1 = 0.00842948;
           float py_0 = 1.46669;
           float py_1 = 0.0295922;
           if( ! evt.isRealData() ) 
               {
                   px_0 = -3.17373;
                    px_1 = -0.024752;
                    py_0 = 0.480606;
                    py_1 = 0.0297115;   
               }  
           /*
           float oldPx = theMET->corPt()*cos(theMET->corPhi());
           float oldPy = theMET->corPt()*sin(theMET->corPhi());
           float newPx = oldPx - (px_0 +px_1*theMET->corSumEt());
           float newPy = oldPy - (py_0 +py_1*theMET->corSumEt());
           float newPhi=theMET->corPhi();
           */
           float oldPx = theMET->getCorPx();
           float oldPy = theMET->getCorPy();
           float newPx = oldPx - (px_0 +px_1*theMET->corSumEt());
           float newPy = oldPy - (py_0 +py_1*theMET->corSumEt());
           float newPhi=theMET->getCorPhi();
           
           newPhi = atan(newPy/newPx); //px>0
           if(newPx<0&&newPy<0)
               newPhi = -3.14159 + newPhi;
           else if(newPx<0&&newPy>=0)
               newPhi = 3.14159 + newPhi;
           //std::cout << "metPT: " << theMET->getCorPt() << std::endl;
           
           if(fabs(newPhi-dipho->phi())<dPhiDiphotonMetThreshold_)  //skip if close
               if(fabs(newPhi-dipho->phi())-3.14159<dPhiDiphotonMetThreshold_) //skip if close but on other side of phi=0
                   continue;
           if(theMET->getCorPt()< metPtThreshold_ )  
               continue;
           vhettags->push_back( tag_obj );
           if( ! evt.isRealData() ) 
               {
                   VHTagTruth truth_obj;
                   truth_obj.setGenPV( higgsVtx );
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

typedef flashgg::VHEtTagProducer FlashggVHEtTagProducer;
DEFINE_FWK_MODULE( FlashggVHEtTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

