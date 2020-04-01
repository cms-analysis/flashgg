#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/VHMetTag.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/Taggers/interface/LeptonSelection.h"

#include "flashgg/DataFormats/interface/VHTagTruth.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "TMath.h"
#include "TMVA/Reader.h"

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
        int  chooseCategory( float );

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<flashgg::Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<flashgg::Met> > METToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<double> rhoTag_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;        

        //configurable selection variables
        double leadPhoOverMassThreshold_   ;
        double subleadPhoOverMassThreshold_;
        double diphoMVAThreshold_          ;
        double metPtThreshold_             ;
        double jetEtaThreshold_            ;
        double jetPtThreshold_             ;
        double deltaRPhoLeadJet_           ;
        double deltaRPhoSubLeadJet_        ;
        double phoIdMVAThreshold_          ;
        double dPhiDiphotonMetThreshold_   ;

        //VHMetMVA
        unique_ptr<TMVA::Reader> VHMetMva_;

        float _pho1_eta         ; 
        float _pho2_eta         ; 
        float _pho1_ptoM        ; 
        float _pho2_ptoM        ; 
        float _min_phoId        ; 
        float _max_phoId        ; 
        float _dipho_cosphi     ; 
        float _njet             ;
        float _jet1_pt          ;
        float _max_jet_dCSV     ;
        float _met              ;
        float _met_sumEt        ;
        float _min_dPhi_jet_met ;
        float _dPhi_dipho_met   ;
        float _pt_balance       ;

        FileInPath VHMetMVAweightfile_ ;

        vector<double> boundaries;

    };

    VHMetTagProducer::VHMetTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag> ( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        METToken_( consumes<View<flashgg::Met> >( iConfig.getParameter<InputTag> ( "METTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        rhoTag_( consumes<double>( iConfig.getParameter<InputTag>( "rhoTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
    {
        leadPhoOverMassThreshold_    = iConfig.getParameter<double>( "leadPhoOverMassThreshold" );
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold" );
        phoIdMVAThreshold_           = iConfig.getParameter<double>( "phoIdMVAThreshold" );
        diphoMVAThreshold_           = iConfig.getParameter<double>( "diphoMVAThreshold" );
        jetPtThreshold_              = iConfig.getParameter<double>( "jetPtThreshold");
        jetEtaThreshold_             = iConfig.getParameter<double>( "jetEtaThreshold");
        deltaRPhoLeadJet_            = iConfig.getParameter<double>( "deltaRPhoLeadJet");
        deltaRPhoSubLeadJet_         = iConfig.getParameter<double>( "deltaRPhoSubLeadJet");
        metPtThreshold_              = iConfig.getParameter<double>( "metPtThreshold" );
        dPhiDiphotonMetThreshold_    = iConfig.getParameter<double>( "dPhiDiphotonMetThreshold" );

        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
        
        //VHMetMVA
        VHMetMVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "VHMetMVAweightfile" );
        _pho1_eta         = -999.; 
        _pho2_eta         = -999.; 
        _pho1_ptoM        = -999.; 
        _pho2_ptoM        = -999.; 
        _min_phoId        = -999.; 
        _max_phoId        = -999.; 
        _dipho_cosphi     = -999.; 
        _njet             = -999.; 
        _jet1_pt          = -999.; 
        _max_jet_dCSV     = -999.; 
        _met              = -999.; 
        _met_sumEt        = -999.; 
        _min_dPhi_jet_met = -999.; 
        _dPhi_dipho_met   = -999.; 
        _pt_balance       = -999.; 

        VHMetMva_.reset( new TMVA::Reader( "!Color:!Silent" ) );
        VHMetMva_->AddVariable( "pho1_eta"           ,&_pho1_eta         ); 
        VHMetMva_->AddVariable( "pho2_eta"           ,&_pho2_eta         ); 
        VHMetMva_->AddVariable( "pho1_ptoM"          ,&_pho1_ptoM        ); 
        VHMetMva_->AddVariable( "pho2_ptoM"          ,&_pho2_ptoM        ); 
        VHMetMva_->AddVariable( "min_phoId"          ,&_min_phoId        ); 
        VHMetMva_->AddVariable( "max_phoId"          ,&_max_phoId        ); 
        VHMetMva_->AddVariable( "dipho_cosphi"       ,&_dipho_cosphi     ); 
        VHMetMva_->AddVariable( "njet"               ,&_njet             ); 
        VHMetMva_->AddVariable( "jet1_pt"            ,&_jet1_pt          ); 
        VHMetMva_->AddVariable( "max_jet_dCSV"       ,&_max_jet_dCSV     ); 
        VHMetMva_->AddVariable( "met"                ,&_met              ); 
        VHMetMva_->AddVariable( "met_sumEt"          ,&_met_sumEt        ); 
        VHMetMva_->AddVariable( "min_dPhi_jet_met"   ,&_min_dPhi_jet_met ); 
        VHMetMva_->AddVariable( "dPhi_dipho_met"     ,&_dPhi_dipho_met   ); 
        VHMetMva_->AddVariable( "pt_balance"         ,&_pt_balance       );
        VHMetMva_->BookMVA( "BDT", VHMetMVAweightfile_.fullPath() );

        boundaries = iConfig.getParameter<vector<double > >( "Boundaries" );
        assert( is_sorted( boundaries.begin(), boundaries.end() ) ); // we are counting on ascending order - update this to give an error message or exception

        produces<vector<VHMetTag> >();
        produces<vector<VHTagTruth> >();

    }

    int VHMetTagProducer::chooseCategory( float mva )
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n;
        for( n = 0 ; n < ( int )boundaries.size() ; n++ ) {
            if( ( double )mva > boundaries[boundaries.size() - n - 1] ) { return n; }
        }
        return -1; // Does not pass, object will not be produced
    }
 
    void VHMetTagProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        Handle<View<flashgg::Electron> > theElectrons;
        evt.getByToken( electronToken_, theElectrons );
 
        Handle<View<flashgg::Muon> > theMuons;
        evt.getByToken( muonToken_, theMuons );

        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) 
            evt.getByToken( tokenJets_[j], Jets[j] );

        Handle<View<flashgg::Met> > METs;
        evt.getByToken( METToken_, METs );
        Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
        assert( diPhotons->size() == mvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception

        Handle<View<reco::GenParticle> > genParticles;
        evt.getByToken( genParticleToken_, genParticles );

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );
 
        edm::Handle<double>  rho;
        evt.getByToken(rhoTag_,rho);
        double rho_    = *rho;

        std::unique_ptr<vector<VHMetTag> > vhettags( new vector<VHMetTag> );
        //std::unique_ptr<vector<VHTagTruth> > truths( new vector<VHTagTruth> );
        //edm::RefProd<vector<VHTagTruth> > rTagTruth = evt.getRefBeforePut<vector<VHTagTruth> >();
        
        Point higgsVtx;
        bool associatedZ   = 0;
        bool associatedW   = 0;
        bool VhasDaughters = 0;
        bool VhasNeutrinos = 0;
        bool VhasLeptons   = 0;
        bool VhasHadrons   = 0;
        bool VhasMissingLeptons = 0;
        float Vpt = 0;

        if( ! evt.isRealData() ) {

            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                int dpdgid[2] = {0, 0};
 
                //Search for Z boson
                if(pdgid ==23) {
                    associatedZ=1;
                    if( genParticles->ptrAt( genLoop )->numberOfDaughters()==2) {
                        VhasDaughters = 1;
                        dpdgid[0] = genParticles->ptrAt(genLoop)->daughter(0)->pdgId();
                        Vpt = genParticles->ptrAt( genLoop )->pt();
                        if(fabs(dpdgid[0]) == 12 || fabs(dpdgid[0]) == 14 || fabs(dpdgid[0]) == 16) VhasNeutrinos = 1;//look for neutrino decay of Z
                        if(fabs(dpdgid[0]) == 11 || fabs(dpdgid[0]) == 13 || fabs(dpdgid[0]) == 15) VhasLeptons = 1;//look for lepton decay of Z  
                        if(fabs(dpdgid[0]) > 0 && fabs(dpdgid[0]) < 9) VhasHadrons = 1;//look for quark decay of Z   
                    }
                }
                //Search for W boson
                if(fabs(pdgid)==24) {
                    associatedW=1;
                    if( genParticles->ptrAt( genLoop )->numberOfDaughters()==2) {
                        VhasDaughters = 1;
                        Vpt = genParticles->ptrAt( genLoop )->pt();
                        dpdgid[0] = genParticles->ptrAt(genLoop)->daughter(0)->pdgId();
                        //dpdgid[1]=genParticles->ptrAt(genLoop)->daughter(1)->pdgId();
                        if(fabs(dpdgid[0]) == 12 || fabs(dpdgid[0]) == 14 || fabs(dpdgid[0]) == 16) { VhasNeutrinos = 1; VhasLeptons = 1; }//look for neutrino decay of W
                        if(fabs(dpdgid[0]) == 11 || fabs(dpdgid[0]) == 13 || fabs(dpdgid[0]) == 15) { VhasNeutrinos = 1; VhasLeptons = 1; }//look for lepton decay of W
                        if(fabs(dpdgid[0]) > 0 && fabs(dpdgid[0]) < 9) VhasHadrons = 1;//look for quark decay of W 
                    }
                }
                // Search for Higgs with decaying to diphoton
                if( pdgid == 25 || pdgid == 22 ) {
                    higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                    continue;
                }
            }
        }

        vector<double> electronEtaThresholds_;
        electronEtaThresholds_.push_back(1.4442);
        electronEtaThresholds_.push_back(1.566);
        electronEtaThresholds_.push_back(2.4);
        //unsigned int idx = 0;

        double idmva1 = 0.;
        double idmva2 = 0.;

        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );

            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );

            if( dipho->leadingPhoton()->pt() < dipho->mass() * leadPhoOverMassThreshold_ ) continue;
            if( dipho->subLeadingPhoton()->pt() < dipho->mass() * subleadPhoOverMassThreshold_ ) continue;
            if( idmva1 <= phoIdMVAThreshold_ || idmva2 <= phoIdMVAThreshold_ ) continue;
            if( mvares->result < diphoMVAThreshold_ ) continue;
            if( theMET->getCorPt() < metPtThreshold_ ) continue;
            if( fabs( deltaPhi(theMET->getCorPhi(), dipho->phi()) ) < dPhiDiphotonMetThreshold_ ) continue;

            //Lepton Veto
            std::vector<edm::Ptr<flashgg::Muon> > goodMuons = selectMuons( theMuons->ptrs(), 
                                        dipho, 
                                        vertices->ptrs(), 
                                        2.4, 
                                        10, 
                                        0.25,
                                        0.2, 
                                        0.2 
                                        );
            std::vector<edm::Ptr<Electron> >goodElectrons = selectStdElectrons( theElectrons->ptrs(), 
                                                   dipho, 
                                                   vertices->ptrs(), 
                                                   10,  
                                                   electronEtaThresholds_,
                                                   false,
                                                   true,
                                                   0.2,
                                                   0.35,
                                                   10,
                                                   rho_, 
                                                   evt.isRealData() 
                                                   );

            if( goodElectrons.size() != 0 || goodMuons.size() != 0 ) continue;

            float minDeltaPhiJetMet   = 3.2;
            float max_dcsv_val        = -1;

            unsigned int jetCollectionIndex = diPhotons->ptrAt( candIndex )->jetCollectionIndex();
            std::vector<edm::Ptr<Jet> > tagJets;
            for( unsigned int jetIndex = 0; jetIndex < Jets[jetCollectionIndex]->size() ; jetIndex++ ) {
                edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( jetIndex );
                if(!thejet->passesJetID  ( flashgg::Tight2017 ) ) continue;

                if( fabs( thejet->eta() ) > jetEtaThreshold_ ) continue;
                if( thejet->pt() < jetPtThreshold_ ) continue;

                float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), 
                                    diPhotons->ptrAt( candIndex )->leadingPhoton()->superCluster()->eta(), 
                                    diPhotons->ptrAt( candIndex )->leadingPhoton()->superCluster()->phi() ) ;
                float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), 
                                    diPhotons->ptrAt( candIndex )->subLeadingPhoton()->superCluster()->eta(),
                                    diPhotons->ptrAt( candIndex )->subLeadingPhoton()->superCluster()->phi() );

                if( dRPhoLeadJet < deltaRPhoLeadJet_ || dRPhoSubLeadJet < deltaRPhoSubLeadJet_ ) continue;

                float dcsv_val       = thejet->bDiscriminator("pfDeepCSVJetTags:probb") + thejet->bDiscriminator("pfDeepCSVJetTags:probbb");
                float deltaPhiMetJet = fabs(deltaPhi(thejet->phi(), theMET->getCorPhi()));

                if (deltaPhiMetJet < minDeltaPhiJetMet)     minDeltaPhiJetMet = deltaPhiMetJet;
                if (dcsv_val > max_dcsv_val)                max_dcsv_val = dcsv_val;

                tagJets.push_back( thejet );
            }

            //Tmp cut for VBF priority. It will be removed next iteration
            if( tagJets.size() > 1 ) {
                double mjj = (tagJets[0]->p4() + tagJets[1]->p4()).mass();
                if( mjj > 350. ) continue;
            }

            _pho1_eta         = dipho->leadingPhoton()->eta(); 
            _pho2_eta         = dipho->subLeadingPhoton()->eta();
            _pho1_ptoM        = dipho->leadingPhoton()->pt() / dipho->mass();
            _pho2_ptoM        = dipho->subLeadingPhoton()->pt() / dipho->mass();
            _min_phoId        = TMath::Min(idmva1, idmva2);
            _max_phoId        = TMath::Max(idmva1, idmva2);
            _dipho_cosphi     = TMath::Cos( deltaPhi(dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->phi()) );
            _njet             = tagJets.size();
            _jet1_pt          = tagJets.size() > 0 ? tagJets[0]->pt() : -50.;
            _max_jet_dCSV     = max_dcsv_val;
            _met              = theMET->getCorPt();
            _met_sumEt        = theMET->sumEt();
            _min_dPhi_jet_met = minDeltaPhiJetMet; 
            _dPhi_dipho_met   = fabs( deltaPhi(theMET->getCorPhi(), dipho->phi()) );
            _pt_balance       = (dipho->pt() - theMET->getCorPt()) / dipho->pt();

            float vhmetmva    = VHMetMva_->EvaluateMVA( "BDT" );

            // Categorization by ZHMVA
            int catnum = chooseCategory( vhmetmva );
            
            if ( catnum != -1 ) {
                VHMetTag tag_obj( dipho, mvares );
                tag_obj.includeWeights( *dipho );
                tag_obj.setDiPhotonIndex( candIndex );
                tag_obj.setCategoryNumber( catnum );
                tag_obj.setSystLabel( systLabel_ );
                tag_obj.setJets( tagJets );
                tag_obj.setMet( theMET );
                tag_obj.setMinDeltaPhiJetMet(minDeltaPhiJetMet);
                tag_obj.setMaxJetDeepCSV(max_dcsv_val);
                if( catnum == 0 ) { 
                    tag_obj.setStage1recoTag( DiPhotonTagBase::stage1recoTag::RECO_VH_MET_Tag0 );
                } else if ( catnum == 1 ) {
                    tag_obj.setStage1recoTag( DiPhotonTagBase::stage1recoTag::RECO_VH_MET_Tag1 );
                }
                vhettags->push_back( tag_obj );
                if( ! evt.isRealData() ) {
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
                    //truths->push_back( truth_obj );
                    //vhettags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<VHTagTruth> >( rTagTruth, idx++ ) ) );
                }
            }
        }
        evt.put( std::move( vhettags ) );
        //evt.put( std::move( truths ) );
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

