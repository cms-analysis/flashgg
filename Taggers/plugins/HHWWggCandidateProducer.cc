// Abe Tishelman-Charny
// 5 Feb 2019

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/HHWWggCandidate.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "flashgg/MicroAOD/interface/CutBasedDiPhotonObjectSelector.h"

#include "SimDataFormats/HTXS/interface/HiggsTemplateCrossSections.h"
#include "flashgg/DataFormats/interface/WHLeptonicTag.h"

#include "flashgg/Taggers/interface/LeptonSelection.h"

#include <vector>
#include <algorithm>
#include "TGraph.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;

namespace flashgg {
  // HHWWggCandidateProducer is a sub class or derived class of EDProducer 
  class HHWWggCandidateProducer : public EDProducer
  {
  public:
    //---typedef
    typedef math::XYZTLorentzVector LorentzVector;

    //---ctors
    HHWWggCandidateProducer();
    HHWWggCandidateProducer( const ParameterSet & );

    //---Outtree 
    edm::Service<TFileService> fs;
    TH1F* vars; 
    // TH1F* cutFlow;
    // TH1F* WTags;

  private:
    double genTotalWeight;
    void produce( Event &, const EventSetup & ) override;

    // Adding Jets 
    std::vector<edm::EDGetTokenT<edm::View<flashgg::Jet> > > jetTokens_;

    EDGetTokenT<View<Photon> > photonToken_;
    Handle<View<flashgg::Photon> > photons;

    EDGetTokenT<View<DiPhotonCandidate> > diphotonToken_;
    Handle<View<flashgg::DiPhotonCandidate> > diphotons;

    EDGetTokenT<View<reco::Vertex> > vertexToken_;
    Handle<View<reco::Vertex> > vertex;

    EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
    Handle<View<reco::GenParticle> > genParticle;

    EDGetTokenT<View<Electron> > electronToken_;
    Handle<View<flashgg::Electron> > electrons;

    EDGetTokenT<View<Muon> > muonToken_;
    Handle<View<flashgg::Muon> > muons;

    EDGetTokenT<View<flashgg::Met> > METToken_;
    Handle<View<flashgg::Met> > METs;

    EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
    Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;

    Handle<View<reco::Vertex> > vertices;

    EDGetTokenT<double> rhoTag_;
    edm::EDGetTokenT<edm::TriggerResults> triggerRECO_;
    edm::EDGetTokenT<edm::TriggerResults> triggerPAT_;
    edm::EDGetTokenT<edm::TriggerResults> triggerFLASHggMicroAOD_;
    edm::Handle<double>  rho;

    std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > JetToken_;

    //---ID selector
    ConsumesCollector cc_;
    CutBasedDiPhotonObjectSelector idSelector_;

    //----output collection
    auto_ptr<vector<HHWWggCandidate> > HHWWggColl_;

    // variables from WHLeptonicTagProducer
    double leptonPtThreshold_;
    double muonEtaThreshold_;
    double leadPhoOverMassThreshold_;
    double subleadPhoOverMassThreshold_;
    double MVAThreshold_;
    double deltaRMuonPhoThreshold_;
    double jetsNumberThreshold_;
    double jetPtThreshold_;
    double jetEtaThreshold_;
    double muPFIsoSumRelThreshold_;
    double PhoMVAThreshold_;
    double METThreshold_;
    bool useVertex0only_;
    double deltaRJetMuonThreshold_;
    double deltaRPhoLeadJet_;
    double deltaRPhoSubLeadJet_;

    double DeltaRTrkElec_;
    double TransverseImpactParam_;
    double LongitudinalImpactParam_;

    double deltaRPhoElectronThreshold_;
    double deltaMassElectronZThreshold_;

    bool hasGoodElec = false;
    bool hasGoodMuons = false;

    vector<double> nonTrigMVAThresholds_;
    vector<double> nonTrigMVAEtaCuts_;

    double electronIsoThreshold_;
    double electronNumOfHitsThreshold_;
    vector<double> electronEtaThresholds_;
    bool useElectronMVARecipe_;
    bool useElectronLooseID_;

    edm::InputTag genInfo_;
    edm::EDGetTokenT<GenEventInfoProduct> genInfoToken_;
  };

  //---constructors
  HHWWggCandidateProducer::HHWWggCandidateProducer( ):
  photonToken_(),
  diphotonToken_(),
  genParticleToken_(),
  electronToken_(),
  muonToken_(),
  METToken_(),
  cc_( consumesCollector() ),
  idSelector_( ParameterSet(), cc_ )

  {}

    //---standard
    HHWWggCandidateProducer::HHWWggCandidateProducer( const ParameterSet & pSet):
    photonToken_( consumes<View<Photon> >( pSet.getParameter<InputTag> ( "PhotonTag" ) ) ),
    diphotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
    vertexToken_( consumes<View<reco::Vertex> >( pSet.getParameter<InputTag> ( "VertexTag" ) ) ),
    genParticleToken_( consumes<View<reco::GenParticle> >( pSet.getParameter<InputTag> ( "GenParticleTag" ) ) ),
    electronToken_( consumes<View<Electron> >( pSet.getParameter<InputTag> ( "ElectronTag" ) ) ), 
    muonToken_( consumes<View<Muon> >( pSet.getParameter<InputTag> ( "MuonTag" ) ) ),
    METToken_( consumes<View<Met> >( pSet.getParameter<InputTag> ( "METTag" ) ) ),
    mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( pSet.getParameter<InputTag> ( "MVAResultTag" ) ) ),
    rhoTag_( consumes<double>( pSet.getParameter<InputTag>( "rhoTag" ) ) ),
    triggerRECO_( consumes<edm::TriggerResults>(pSet.getParameter<InputTag>("RECOfilters") ) ),
    triggerPAT_( consumes<edm::TriggerResults>(pSet.getParameter<InputTag>("PATfilters") ) ),
    triggerFLASHggMicroAOD_( consumes<edm::TriggerResults>( pSet.getParameter<InputTag>("FLASHfilters") ) ),
    cc_( consumesCollector() ),
    idSelector_( pSet.getParameter<ParameterSet> ( "idSelection" ), cc_ )

    {
      genInfo_ = pSet.getUntrackedParameter<edm::InputTag>( "genInfo", edm::InputTag("generator") );
      genInfoToken_ = consumes<GenEventInfoProduct>( genInfo_ );
      vars = fs->make<TH1F> ("vars","vars",10,0,10);
      // cutFlow = fs->make<TH1F> ("cutFlow","Cut Flow",10,0,10);
      // WTags = fs->make<TH1F> ("WTags","W Tags",4,0,4);

      leptonPtThreshold_ = pSet.getParameter<double>( "leptonPtThreshold");
      muonEtaThreshold_ = pSet.getParameter<double>( "muonEtaThreshold");
      leadPhoOverMassThreshold_ = pSet.getParameter<double>( "leadPhoOverMassThreshold");
      subleadPhoOverMassThreshold_ = pSet.getParameter<double>( "subleadPhoOverMassThreshold");
      MVAThreshold_ = pSet.getParameter<double>( "MVAThreshold");
      deltaRMuonPhoThreshold_ = pSet.getParameter<double>( "deltaRMuonPhoThreshold");
      jetsNumberThreshold_ = pSet.getParameter<double>( "jetsNumberThreshold");
      jetPtThreshold_ = pSet.getParameter<double>( "jetPtThreshold");
      jetEtaThreshold_ = pSet.getParameter<double>( "jetEtaThreshold");
      muPFIsoSumRelThreshold_ = pSet.getParameter<double>( "muPFIsoSumRelThreshold");
      PhoMVAThreshold_ = pSet.getParameter<double>( "PhoMVAThreshold");
      METThreshold_ = pSet.getParameter<double>( "METThreshold");
      useVertex0only_              = pSet.getParameter<bool>("useVertex0only");
      deltaRJetMuonThreshold_ = pSet.getParameter<double>( "deltaRJetMuonThreshold");
      deltaRPhoLeadJet_ = pSet.getParameter<double>( "deltaRPhoLeadJet");
      deltaRPhoSubLeadJet_ = pSet.getParameter<double>( "deltaRPhoSubLeadJet");

      DeltaRTrkElec_ = pSet.getParameter<double>( "DeltaRTrkElec");
      TransverseImpactParam_ = pSet.getParameter<double>( "TransverseImpactParam");
      LongitudinalImpactParam_ = pSet.getParameter<double>( "LongitudinalImpactParam");

      deltaRPhoElectronThreshold_ = pSet.getParameter<double>( "deltaRPhoElectronThreshold");
      deltaMassElectronZThreshold_ = pSet.getParameter<double>( "deltaMassElectronZThreshold");

      nonTrigMVAThresholds_ =  pSet.getParameter<vector<double > >( "nonTrigMVAThresholds");
      nonTrigMVAEtaCuts_ =  pSet.getParameter<vector<double > >( "nonTrigMVAEtaCuts");
      electronIsoThreshold_ = pSet.getParameter<double>( "electronIsoThreshold");
      electronNumOfHitsThreshold_ = pSet.getParameter<double>( "electronNumOfHitsThreshold");
      electronEtaThresholds_ = pSet.getParameter<vector<double > >( "electronEtaThresholds");
      useElectronMVARecipe_=pSet.getParameter<bool>("useElectronMVARecipe");
      useElectronLooseID_=pSet.getParameter<bool>("useElectronLooseID");

      auto jetTags = pSet.getParameter<std::vector<edm::InputTag> > ( "JetTags" ); 
      for( auto & tag : jetTags ) { jetTokens_.push_back( consumes<edm::View<flashgg::Jet> >( tag ) ); }

      produces<vector<HHWWggCandidate> > ();
    }

    void HHWWggCandidateProducer::produce( Event &event, const EventSetup & )
    {

      // Get particle objects
      event.getByToken( photonToken_, photons );
      event.getByToken( diphotonToken_, diphotons );
      event.getByToken( vertexToken_, vertex );
      event.getByToken( genParticleToken_, genParticle );
      event.getByToken( electronToken_, electrons );
      event.getByToken( muonToken_, muons );
      event.getByToken( METToken_, METs );
      event.getByToken( mvaResultToken_, mvaResults );
      event.getByToken( vertexToken_, vertices );
      event.getByToken( rhoTag_, rho);

      double rho_    = *rho;

      // Set cut booleans
      std::vector<double> Cut_Results = {1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; // Cut_Results[i] = 1: Event Passed Cut i 
      std::vector<double> Cut_Variables = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; // Cut_Results[i] = 1: Event Passed Cut i 
      double SLW_Tag = 0.; // Semi-Leptonic W Tag  
      double FLW_Tag = 0.; // Fully-Leptonic W Tag
      double FHW_Tag = 0.; // Fully-Hadronic W Tag 
      bool PS_dipho_tag = 0; // preselected diphoton 
      // bool Pass_PS = 0; // Preselected Diphoton (may also pass SLW_tag)

      //---output collection
      std::unique_ptr<vector<HHWWggCandidate> > HHWWggColl_( new vector<HHWWggCandidate> );
      //edm::Ptr<reco::Vertex> vertex_zero = vertex->ptrAt(0);
      //reco::GenParticle::Point genVertex;
      int n_electrons = electrons->size();
      int n_muons = muons->size();
      int n_photons = photons->size();
      int n_diphotons = diphotons->size();
      int n_METs = METs->size(); // Should be 1, but using as a way to obtain met four vector 
      int n_good_electrons = 0;
      int n_good_muons = 0;
      int n_good_leptons = 0;
      int n_good_jets = 0;
      std::vector<flashgg::Photon> phoVector;
      std::vector<flashgg::DiPhotonCandidate> diphoVector;
      std::vector<flashgg::Electron> electronVector;
      std::vector<flashgg::Muon> muonVector;
      std::vector<flashgg::Met> METVector;
      std::vector<flashgg::Jet> JetVector;

      // Saved Objects after selections
      std::vector<flashgg::Jet> tagJets_;
      std::vector<flashgg::Muon> goodMuons_;
      std::vector<flashgg::Electron> goodElectrons_; 
      std::vector<flashgg::Met> theMET_;
      std::vector<flashgg::DiPhotonCandidate> diphoVector_; // diphoton from W tagged events  

//-----------------------------------------------------------------------------------------------------------

      // METfilters 
      bool passMETfilters=1;
      //Get trigger results relevant to MET filters                                                                                                                                              

      edm::Handle<edm::TriggerResults> triggerBits;
      if(! event.isRealData() )
          event.getByToken( triggerPAT_, triggerBits );
      else
          event.getByToken( triggerRECO_, triggerBits );

      edm::Handle<edm::TriggerResults> triggerFLASHggMicroAOD;
      event.getByToken( triggerFLASHggMicroAOD_, triggerFLASHggMicroAOD );
      const edm::TriggerNames &triggerNames = event.triggerNames( *triggerBits );

      //check if passMETfilters 
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
      const edm::TriggerNames &flashggtriggerNames = event.triggerNames( *triggerFLASHggMicroAOD );
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

      bool photonSelection = false;
      double idmva1 = 0.;
      double idmva2 = 0.;
      // bool checked_first = false; 
      // Pass_PS = false;
      bool one_FH_dr = false;
      bool one_FL_dr = false;
      float dr_ll = 0;
      double j_mass = 0.0;     

      // Check if event passes W taggers 
      // for( unsigned int diphoIndex = 0; diphoIndex < diphotons->size(); diphoIndex++ ) { // look at all diphotons 
      if (diphotons->size() > 0){
      for( unsigned int diphoIndex = 0; diphoIndex < 1; diphoIndex++ ) { // only look at highest pt dipho
        // cout << "checked_first = " << checked_first << endl;
        // cout << "PS_dipho_tag = " << PS_dipho_tag << endl;
        // if (checked_first) continue; // If already checked highest pT preselected diphoton, continue 
        // checked_first = true; 
        // Diphoton 
        edm::Ptr<flashgg::DiPhotonCandidate> dipho = diphotons->ptrAt( diphoIndex ); 
        edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );   

        // l_eta = dipho->leadingPhoton()->superCluster()->eta();
        // l_r9 = dipho->leadingPhoton()->full5x5_r9();
        // sl_eta = dipho->leadingPhoton()->superCluster()->eta();
        // sl_r9 = dipho->leadingPhoton()->full5x5_r9();
        // Check that diphoton is preselected 
        // flashgg::DiPhotonCandidate * DPPointer = const_cast<flashgg::DiPhotonCandidate *>(dipho.get());
        //Pass_PS |= idSelector_(*DPPointer, event);

        //if (!Pass_PS) continue;

        // else if ( Pass_PS ){ 
        //   // abs(dipho->leadingPhoton()->superCluster()->eta()) > 1.4;
        //   // (dipho->leadingPhoton()->full5x5_r9()) < 0.9;
        //   // abs(dipho->subLeadingPhoton()->superCluster()->eta());
        //   // (dipho->subLeadingPhoton()->full5x5_r9())< 0.9;
        //   //if ((abs(dipho->leadingPhoton()->superCluster()->eta()) > 1.4) && ((dipho->leadingPhoton()->full5x5_r9()) < 0.9) && (abs(dipho->subLeadingPhoton()->superCluster()->eta())) && ((dipho->subLeadingPhoton()->full5x5_r9())< 0.9))
        //   //{
        //   PS_dipho_tag = true;
        //   checked_first = true;
        //   //}
        // }

        hasGoodElec = false;
        hasGoodMuons = false;

        // Check MET Filters
        //if(!passMETfilters) {continue;}

        // Check if useVertex0only_
        if(useVertex0only_) // If only 0th vertex of diphotons is used, continue on all other vertex indices 
            if(diphotons->ptrAt(diphoIndex)->vertexIndex()!=0)
                {continue;}     

        // leading/subleading photon pt 
        if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ ) { continue; }
        if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ ) { continue; }

        // leading/subleading photon MVA
        idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
        idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
        if( idmva1 <= PhoMVAThreshold_ || idmva2 <= PhoMVAThreshold_ ) { continue; }

        // Diphoton MVA 
        if( mvares->result < MVAThreshold_ ) { continue; }
        photonSelection = true;

        // Electrons 
        std::vector<edm::Ptr<Electron> > goodElectrons = selectStdElectrons( electrons->ptrs(), dipho, vertices->ptrs(), leptonPtThreshold_, electronEtaThresholds_,
                                                                          useElectronMVARecipe_,useElectronLooseID_,
                                                                          deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_,
                                                                          rho_, event.isRealData() );

        
        // Muons                                                                   
        std::vector<edm::Ptr<flashgg::Muon> > goodMuons = selectMuons( muons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_, leptonPtThreshold_,
        muPFIsoSumRelThreshold_, deltaRMuonPhoThreshold_, deltaRMuonPhoThreshold_ );

        n_good_electrons = goodElectrons.size();
        n_good_muons = goodMuons.size();
        n_good_leptons = n_good_electrons + n_good_muons;
        hasGoodElec = ( goodElectrons.size() > 0 );
        hasGoodMuons = ( goodMuons.size() > 0 );
              
        // FL: Require at dr(l,l) > 0.4 
        // For which pairs of the >=2 good leptons should dr be greater than 4?

        if (hasGoodElec && hasGoodMuons){
          for (unsigned int ei = 0; ei < goodElectrons.size(); ei++){
            Ptr<flashgg::Electron> electron = goodElectrons[ei];
            for (unsigned int mi = 0; mi < goodMuons.size(); mi++){
              Ptr<flashgg::Muon> muon = goodMuons[mi];
              dr_ll = deltaR(electron->eta(), electron->phi(), muon->eta(), muon->phi()); 
              if (dr_ll > 0.4){ 
                one_FL_dr = true;
                break;
              }
            }
          }
        }

        else if (hasGoodElec && !hasGoodMuons){
          for (unsigned int ei = 0; ei < goodElectrons.size() - 1; ei++){ // the last electron cannot be the first one in the dR calculation 
            Ptr<flashgg::Electron> electroni = goodElectrons[ei];
            for (unsigned int ej = ei + 1; ej < goodElectrons.size(); ej++){
              Ptr<flashgg::Electron> electronj = goodElectrons[ej];
              dr_ll = deltaR(electroni->eta(), electroni->phi(), electronj->eta(), electronj->phi()); 
              if (dr_ll > 0.4){ 
                one_FL_dr = true;
                break;
              }
            }
          }
        }

        else if (!hasGoodElec && hasGoodMuons){
          for (unsigned int mi = 0; mi < goodMuons.size() - 1; mi++){
            Ptr<flashgg::Muon> muoni = goodMuons[mi];
            for (unsigned int mj = mi + 1; mj < goodMuons.size(); mj++){
              Ptr<flashgg::Muon> muonj = goodMuons[mj];
              dr_ll = deltaR(muoni->eta(), muoni->phi(), muonj->eta(), muonj->phi()); 
              if (dr_ll > 0.4){ 
                one_FL_dr = true;
                break;
              }
            }
          }
        }

        // Jets 
        unsigned int jetCollectionIndex = diphotons->at( diphoIndex ).jetCollectionIndex();
        edm::Handle<edm::View<flashgg::Jet> > Jets_;
        event.getByToken( jetTokens_[jetCollectionIndex], Jets_);
        std::vector<edm::Ptr<Jet> > tagJets;

        // Jet Selections
        for( unsigned int candIndex_outer = 0; candIndex_outer <  Jets_->size() ; candIndex_outer++ ) 
            {
                bool keepJet=true;
                edm::Ptr<flashgg::Jet> thejet = Jets_->ptrAt( candIndex_outer );
                if(!thejet->passesJetID  ( flashgg::Tight2017 ) ) { continue; }
                if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { keepJet=false; }

                if( thejet->pt() < jetPtThreshold_ ) { keepJet=false; }
                float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),
                                                dipho->subLeadingPhoton()->superCluster()->phi() );
                
                if( dRPhoLeadJet < deltaRPhoLeadJet_ || dRPhoSubLeadJet < deltaRPhoSubLeadJet_ ) { keepJet=false; }
                if( hasGoodElec ) 
                    for( unsigned int electronIndex = 0; electronIndex < goodElectrons.size(); electronIndex++ ) 
                        {
                            Ptr<flashgg::Electron> electron = goodElectrons[electronIndex];
                            float dRJetElectron = deltaR( thejet->eta(), thejet->phi(), electron->eta(), electron->phi() ) ;
                            if( dRJetElectron < deltaRJetMuonThreshold_ ) { keepJet=false; }
                        }
                if( hasGoodMuons ) 
                    for( unsigned int muonIndex = 0; muonIndex < goodMuons.size(); muonIndex++ ) 
                        {
                            Ptr<flashgg::Muon> muon = goodMuons[muonIndex];
                            float dRJetMuon = deltaR( thejet->eta(), thejet->phi(), muon->eta(), muon->phi() ) ;
                            if( dRJetMuon < deltaRJetMuonThreshold_ ) { keepJet=false; }
                        }
                if(keepJet)
                    tagJets.push_back( thejet );

            }

        n_good_jets = tagJets.size();
        if (n_good_jets == 1){
          edm::Ptr<flashgg::Jet> theOnejet = tagJets[0];
          j_mass = theOnejet->mass(); 
          // cout << "pt = " << theOnejet->pt() << endl;      
        }

        // FH: Require at least one delta r pair with dr > 0.4 
        float dr_jj = 0;
        // bool one_FH_dr = false;
        if (tagJets.size() >= 2){
          for (unsigned int ji = 0; ji < tagJets.size() - 1; ji++){
            Ptr<flashgg::Jet> jeti = tagJets[ji];
            for (unsigned int jj = ji + 1; jj < tagJets.size(); jj++){
              Ptr<flashgg::Jet> jetj = tagJets[jj];
              dr_jj = deltaR(jeti->eta(), jeti->phi(), jetj->eta(), jetj->phi()); 
              if (dr_jj > 0.4){ 
                one_FH_dr = true;
                break; 
              } 
            }
          }
        }

        // MET 
        if( METs->size() != 1 ) { std::cout << "WARNING - #MET is not 1" << std::endl;}
        Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );

        // Check W tags 
        if ( (n_good_leptons == 1) && (theMET->getCorPt() >= 45) && (n_good_jets >= 1) ) SLW_Tag = 1.0; // Passed SL selections 
        if ( (n_good_leptons >= 2) && (theMET->getCorPt() >= 70) && (n_good_jets < 2) && (one_FL_dr) ) FLW_Tag = 1.0; // Passed FL selections 
        if ( (n_good_leptons == 0) && (theMET->getCorPt() < 45) && (n_good_jets >= 3) && (one_FH_dr) ) FHW_Tag = 1.0; // Passed FH selections 

        // how to go from pointer 'jet' to object '*thisJetPointer'
        //flashgg::Jet * thisJetPointer = const_cast<flashgg::Jet *>(jet.get());
        //JetVector.push_back(*thisJetPointer);

        for (unsigned int i = 0; i < tagJets.size(); i++){
          auto tag_jet = tagJets[i];
          flashgg::Jet * thisJetPointer = const_cast<flashgg::Jet *>(tag_jet.get());
          tagJets_.push_back(*thisJetPointer);
        }
        for (unsigned int i = 0; i < goodElectrons.size(); i++){
          auto good_elec = goodElectrons[i];
          flashgg::Electron * thisElectronPointer = const_cast<flashgg::Electron *>(good_elec.get());
          goodElectrons_.push_back(*thisElectronPointer);
        }
        for (unsigned int i = 0; i < goodMuons.size(); i++){
          auto good_muon = goodMuons[i];
          flashgg::Muon * thisMuonPointer = const_cast<flashgg::Muon *>(good_muon.get());
          goodMuons_.push_back(*thisMuonPointer);
        }
        // Store MET as one element in MET vector 
        if( METs->size() != 1 ) { std::cout << "WARNING - #MET is not 1" << std::endl;}
        for( int METIndex = 0; METIndex < n_METs; METIndex++ )
        {
          edm::Ptr<flashgg::Met> m_entry = METs->ptrAt( METIndex );
          flashgg::Met * thisMETPointer = const_cast<flashgg::Met *>(m_entry.get());
          theMET_.push_back(*thisMETPointer);
        }

        // dipho
        edm::Ptr<flashgg::DiPhotonCandidate> dipho_ = dipho;
        flashgg::DiPhotonCandidate * thisDPPointer = const_cast<flashgg::DiPhotonCandidate *>(dipho_.get());
        diphoVector_.push_back(*thisDPPointer);

        //}

      } // Diphoton loop 
    } // if at least 1 PS diphoton 

          // Diphotons
          // Pass_PS = false;
          for( int diphoIndex = 0; diphoIndex < n_diphotons; diphoIndex++ )
          {
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diphotons->ptrAt( diphoIndex );
            flashgg::DiPhotonCandidate * thisDPPointer = const_cast<flashgg::DiPhotonCandidate *>(dipho.get());
            diphoVector.push_back(*thisDPPointer);
            //Pass_PS |= idSelector_(*thisDPPointer, event);
            //if ( Pass_PS ) diphoVector.push_back(*thisDPPointer);                     
          }

          // Photons   
          for( int phoIndex = 0; phoIndex < n_photons; phoIndex++ )
          {
            edm::Ptr<flashgg::Photon> pho = photons->ptrAt( phoIndex );
            flashgg::Photon * thisPPointer = const_cast<flashgg::Photon *>(pho.get());
            phoVector.push_back(*thisPPointer);
          }

          // Electrons
          for( int electronIndex = 0; electronIndex < n_electrons; electronIndex++ )
          {
            edm::Ptr<flashgg::Electron> elec = electrons->ptrAt( electronIndex );
            flashgg::Electron * thisElecPointer = const_cast<flashgg::Electron *>(elec.get());
            //if ( elec->pt() >= leptonPtThreshold_) lep_thres_cut = 1; // check lepton pt cut 
            electronVector.push_back(*thisElecPointer);
          }

          // Muons 
          for( int muonIndex = 0; muonIndex < n_muons; muonIndex++ )
          {
            edm::Ptr<flashgg::Muon> mlep = muons->ptrAt( muonIndex );
            flashgg::Muon * thisMuonPointer = const_cast<flashgg::Muon *>(mlep.get());
            //if ( mlep->pt() >= leptonPtThreshold_) lep_thres_cut = 1; // check lepton pt cut 
            muonVector.push_back(*thisMuonPointer); 
          }

          // MET
          // Store MET as one element in MET vector 
          if( METs->size() != 1 ) { std::cout << "WARNING - #MET is not 1" << std::endl;}
          for( int METIndex = 0; METIndex < n_METs; METIndex++ )
          {
            edm::Ptr<flashgg::Met> m_entry = METs->ptrAt( METIndex );
            flashgg::Met * thisMETPointer = const_cast<flashgg::Met *>(m_entry.get());
            METVector.push_back(*thisMETPointer);
          }

        // } // Done saving all objects 

//////////////////////////////////////////////////////////////////

        //-- Get GEN particles 

        // Want to save GEN particles to compare variables to RECO 
        // Only want to save GEN particles coming from mother particles of interest

        // Mother Daughter pdgid pairs 
        // if a particle of abs(pdgid) = [0] came from abs(pdgid) = [1], store it 
        std::vector<std::vector<int>> md_pairs = {}; // mother daughter pairs 
        md_pairs.push_back({24,25}); // W from H 
        md_pairs.push_back({22,25}); // Photon from H 
        md_pairs.push_back({11,24}); // Electron from W
        md_pairs.push_back({12,24}); // Electron neutrino from W
        md_pairs.push_back({13,24}); // Muon from W
        md_pairs.push_back({14,24}); // Muon neutrino from W 
        
        //std::vector<int> quark_pdgids = {1,2,3,4,5}; // if you want to look for b quarks coming from W's 
        std::vector<int> quark_pdgids = {1,2,3,4}; // down, up, strange, charm 

        for (unsigned int i = 0; i < quark_pdgids.size(); i++){
          int qid = quark_pdgids[i];
          md_pairs.push_back({qid,24}); // Quark from W
        }

        // vector to store genparticles in 
        vector<reco::GenParticle> genParticlesVector;
        // float higgs_eta = -99;
        // If MC event 
        if (! event.isRealData() ){
          // For each gen particle in event 
          for(size_t g=0; g < genParticle->size(); g++){
            auto gen = genParticle->ptrAt(g);
            if (gen->pdgId() == 25){ // add higgs' to genvector 
              // cout << "Found Higgs" << endl;
              reco::GenParticle * thisGENPointer = const_cast<reco::GenParticle *>(gen.get());
              if (gen->daughter(0)->pdgId() == 22){
                // cout << "higgs eta = " << gen->eta() << endl;
                // higgs_eta = gen->eta();
              }
              genParticlesVector.push_back(*thisGENPointer);
            }
            // If the particle has a mother 
            if (gen->mother(0) != 0){
              int pid = gen->pdgId(); 
              int pmotid = gen->mother(0)->pdgId();   

              if ( ( abs(pid) == 5) && (abs(pmotid) == 24) ) cout << "***B quark Found!***" << endl;           

              for(unsigned int i = 0; i < md_pairs.size(); i++){
                int vec_id = md_pairs[i][0];
                int vec_mid = md_pairs[i][1]; 

                // if event gen particle and mother are on list of desired particles, add to genParticlesVector 
                if ( (abs(pid) == abs(vec_id)) && (abs(pmotid) == abs(vec_mid))){ 
                  //cout << "Found " << abs(pid) << " from " << abs(pmotid) << endl;
                  reco::GenParticle * thisGENPointer = const_cast<reco::GenParticle *>(gen.get());
                  genParticlesVector.push_back(*thisGENPointer);                
                }
              }
            }
          }
        }

        // Save Output Tags and Variables 
        double d_n_good_jets = (double)n_good_jets;
        double d_n_good_electrons = (double)n_good_electrons;
        double d_n_good_muons = (double)n_good_muons;
        double d_n_good_leptons = (double)n_good_leptons;

        // for (unsigned int i = 0; i < 3; i++){
        //   if (i == 0) WTags->Fill(i,SLW_Tag);
        //   if (i == 1) WTags->Fill(i,FLW_Tag);
        //   if (i == 2) WTags->Fill(i,FHW_Tag);
        //   if (i == 3){
        //     if (one_FL_dr == 1) WTags->Fill(i,1.0);
        //   }
        //   if (i == 4){
        //     if (one_FH_dr == 1) WTags->Fill(i,1.0);
        //   }
        // }

        if (SLW_Tag == 1.0) Cut_Variables[0] = 1.0;
        if (FLW_Tag == 1.0) Cut_Variables[1] = 1.0;
        if (FHW_Tag == 1.0) Cut_Variables[2] = 1.0;
        if (one_FL_dr == true) Cut_Variables[3] = 1.0;
        if (one_FH_dr == true) Cut_Variables[4] = 1.0;
        // cout << "passMETfilters: " << (double)passMETfilters << endl;
        // cout << "(double)PS_dipho_tag" << (double)PS_dipho_tag << endl;
        Cut_Variables[5] = d_n_good_jets; 
        Cut_Variables[6] = d_n_good_electrons; 
        Cut_Variables[7] = d_n_good_muons; 
        Cut_Variables[8] = d_n_good_leptons; 
        Cut_Variables[9] = (double)PS_dipho_tag;
        Cut_Variables[10] = j_mass;
        Cut_Variables[11] = (double)passMETfilters;

        // if (n_diphotons > 0) vars->Fill(1.0);
        // if (n_photons > 1) vars->Fill(2.0);
        // if ( ( abs(higgs_eta) < 2.5 ) ) vars->Fill(3.0);

        //if ( (n_diphotons > 0) && (n_photons > 1) && (abs(higgs_eta) < 2.5)) vars->Fill(1.0); // number of events that passed cuts and have a preselected diphoton. numerator.
        //if ( (n_photons > 1) && (abs(higgs_eta) < 2.5)) vars->Fill(2.0); // number of events that passed cuts. Denominator 
        // cout << "photonSelection = " << photonSelection << endl;
        if ( (d_n_good_leptons >= 1) && (n_diphotons > 0) && (photonSelection) ) vars->Fill(1.0); // numerator
        if ( (n_diphotons > 0) && (photonSelection)) vars->Fill(2.0); // denominator 

        // ndpho->Fill(n_diphotons);

        // numerator equals number of events with at least 0 diphotons (after cuts)
        // denominator equals number of events that pass cuts 

        // int e_thres = 1; 
        // if (n_photons >= 2) Cut_Results[1] = 1.0;
        // if (n_good_electrons >= e_thres) Cut_Results[2] = 1.0;
        // if (n_good_muons >= 1) Cut_Results[3] = 1.0;
        // if (n_good_jets >= 1) Cut_Results[4] = 1.0;
        // if (PS_dipho_tag) Cut_Results[5] = 1.0;
        // if (SLW_Tag == 1.0) Cut_Results[6] = 1.0;
        // if ( (PS_dipho_tag) and (SLW_Tag) ) Cut_Results[7] = 1.0;
        // bool pass_cut = 0;
        // double gen_value = 0;
        
        // // Data 
        // if (event.isRealData()){
        //   for (unsigned int i = 0; i < Cut_Results.size(); i++){
        //     pass_cut = Cut_Results[i]; // 1 = pass, 0 = fail 
        //     cutFlow->Fill(i,pass_cut);
        //   }
        // }

        // // MC 
        // if (!event.isRealData()){
        //   Handle<GenEventInfoProduct> genInfo;
        //   event.getByToken(genInfoToken_, genInfo);
        //   genTotalWeight = genInfo->weight();
        //   for (unsigned int i = 0; i < Cut_Results.size(); i++){
        //     pass_cut = Cut_Results[i]; // 1 = pass, 0 = fail 
        //     gen_value = pass_cut*genTotalWeight;
        //     cutFlow->Fill(i,gen_value);
        //   }
        // }

        // Create HHWWggCandidate Object 
        HHWWggCandidate HHWWgg(diphoVector_, phoVector, electronVector, muonVector, theMET_, genParticlesVector, tagJets_, Cut_Results, Cut_Variables);
        HHWWggColl_->push_back(HHWWgg);

      event.put( std::move(HHWWggColl_) );
    }
  }
  typedef flashgg::HHWWggCandidateProducer FlashggHHWWggCandidateProducer;
  DEFINE_FWK_MODULE( FlashggHHWWggCandidateProducer );
