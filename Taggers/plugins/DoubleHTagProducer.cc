#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"

#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/DoubleHTag.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "flashgg/Taggers/interface/LeptonSelection.h"
#include "flashgg/MicroAOD/interface/MVAComputer.h"
#include "flashgg/DataFormats/interface/DoubleHttHTagger.h"

#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"

#include <vector>
#include <algorithm>
#include "TGraph.h"

using namespace std;
using namespace edm;

namespace flashgg {

    class DoubleHTagProducer : public EDProducer
    {

    public:
        DoubleHTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int chooseCategory( float mva, float mx );
        float EvaluateNN();
        float getGenCosThetaStar_CS(TLorentzVector h1, TLorentzVector h2);
        bool isclose(double a, double b, double rel_tol, double abs_tol);        
        void StandardizeHLF();
        void StandardizeParticleList();
        
        std::string inputJetsName_;
        std::vector<std::string> inputJetsSuffixes_;
        unsigned int inputJetsCollSize_;
        std::vector<edm::EDGetTokenT<edm::View<flashgg::Jet> > > jetTokens_;
        std::string inputDiPhotonName_;
        std::vector<std::string> inputDiPhotonSuffixes_;
       // EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        std::vector<edm::EDGetTokenT<edm::View<DiPhotonCandidate> > > diPhotonTokens_;

        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;

        std::vector< std::string > systematicsLabels;
        std::map<std::string, float> ttHVars;

        double minLeadPhoPt_, minSubleadPhoPt_;
        bool scalingPtCuts_, doPhotonId_, doMVAFlattening_, doCategorization_, dottHTagger_;
        double photonIDCut_;
        double vetoConeSize_;         
        unsigned int doSigmaMDecorr_;
        edm::FileInPath sigmaMDecorrFile_;
        std::vector<int> photonElectronVeto_;


        DecorrTransform* transfEBEB_;
        DecorrTransform* transfNotEBEB_;

        double minJetPt_;
        double maxJetEta_;
        vector<double>mjjBoundaries_;
        vector<double>mjjBoundariesLower_;
        vector<double>mjjBoundariesUpper_;
        vector<std::string> bTagType_;
        bool       useJetID_;
        string     JetIDLevel_;        

        ConsumesCollector cc_;
        GlobalVariablesComputer globalVariablesComputer_;
        MVAComputer<DoubleHTag> mvaComputer_;
        vector<double> mvaBoundaries_, mxBoundaries_;
        unsigned int nMX_;
        int multiclassSignalIdx_;
            
        //leptons selection
        double leptonPtThreshold;
        double muEtaThreshold;
        double muPFIsoSumRelThreshold; 

        double dRPhoElectronThreshold;
        double dRPhoMuonThreshold;
        double dRJetLeptonThreshold;

        bool useElecMVARecipe; 
        bool useElecLooseId;
        std::vector<double> elecEtaThresholds;

        double TTHLeptonictag_MuonPtCut_;
        double TTHLeptonictag_MuonEtaCut_;
        double TTHLeptonictag_MuonIsoCut_;
        double TTHLeptonictag_MuonPhotonDrCut_;
        double TTHLeptonictag_ElePtCut_;
        std::vector<double> TTHLeptonictag_EleEtaCuts_;
        double TTHLeptonictag_ElePhotonDrCut_;
        double TTHLeptonictag_ElePhotonZMassCut_;
        double TTHLeptonictag_DeltaRTrkEle_;


        FileInPath MVAFlatteningFileName_;
        TFile * MVAFlatteningFile_;
        TGraph * MVAFlatteningCumulative_;
        double MVAscaling_;

        vector< edm::EDGetTokenT<float> > reweights_;
        int doReweight_;
            
        DoubleHttHTagger tthKiller_;
        float ttHTagger;
        float ttHScoreThreshold;
        edm::EDGetTokenT<edm::View<flashgg::Met> > METToken_;
        edm::EDGetTokenT<edm::View<flashgg::Electron> > electronToken_;
        edm::EDGetTokenT<edm::View<flashgg::Muon> > muonToken_;
        edm::EDGetTokenT<edm::View<reco::Vertex> > vertexToken_;
        edm::EDGetTokenT<double> rhoToken_;

        std::vector<double> HLF_VectorVar_;
        std::vector<std::vector<double>> PL_VectorVar_;
        std::vector<double> x_mean_, x_std_, list_mean_, list_std_;
        FileInPath ttHWeightfileName_ ;
        tensorflow::GraphDef* graphDef_ttH;
        tensorflow::Session* session_ttH;

    };

    DoubleHTagProducer::DoubleHTagProducer( const ParameterSet &iConfig ) :
      //  diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        minLeadPhoPt_( iConfig.getParameter<double> ( "MinLeadPhoPt" ) ),
        minSubleadPhoPt_( iConfig.getParameter<double> ( "MinSubleadPhoPt" ) ),
        scalingPtCuts_( iConfig.getParameter<bool> ( "ScalingPtCuts" ) ),
        vetoConeSize_( iConfig.getParameter<double> ( "VetoConeSize" ) ),
        minJetPt_( iConfig.getParameter<double> ( "MinJetPt" ) ),
        maxJetEta_( iConfig.getParameter<double> ( "MaxJetEta" ) ),
        bTagType_( iConfig.getParameter<vector<std::string>>( "BTagType") ),
        useJetID_( iConfig.getParameter<bool>   ( "UseJetID"     ) ),
        JetIDLevel_( iConfig.getParameter<string> ( "JetIDLevel"   ) ),
        cc_( consumesCollector() ),
        globalVariablesComputer_(iConfig.getParameter<edm::ParameterSet>("globalVariables"), cc_),
        mvaComputer_(iConfig.getParameter<edm::ParameterSet>("MVAConfig"),  &globalVariablesComputer_)
        //mvaComputer_(iConfig.getParameter<edm::ParameterSet>("MVAConfig"))
    {
        mjjBoundaries_ = iConfig.getParameter<vector<double > >( "MJJBoundaries" ); 
        mvaBoundaries_ = iConfig.getParameter<vector<double > >( "MVABoundaries" );
        mxBoundaries_ = iConfig.getParameter<vector<double > >( "MXBoundaries" );
        nMX_ = iConfig.getParameter<unsigned int >( "nMX" );
        mjjBoundariesLower_ = iConfig.getParameter<vector<double > >( "MJJBoundariesLower" ); 
        mjjBoundariesUpper_ = iConfig.getParameter<vector<double > >( "MJJBoundariesUpper" ); 
        multiclassSignalIdx_ = (iConfig.getParameter<edm::ParameterSet>("MVAConfig")).getParameter<int>("multiclassSignalIdx"); 
        doReweight_ = (iConfig.getParameter<int>("doReweight")); 
        auto names = iConfig.getParameter<vector<string>>("reweight_names");
        for (auto & name : names ) {
            reweights_.push_back(consumes<float>(edm::InputTag(iConfig.getParameter<string>("reweight_producer") , name))) ;
        }

        TTHLeptonictag_MuonPtCut_=iConfig.getParameter<double> ("TTHLeptonictag_MuonPtCut");
        TTHLeptonictag_MuonEtaCut_=iConfig.getParameter<double> ("TTHLeptonictag_MuonEtaCut");
        TTHLeptonictag_MuonIsoCut_=iConfig.getParameter<double> ("TTHLeptonictag_MuonIsoCut");
        TTHLeptonictag_MuonPhotonDrCut_=iConfig.getParameter<double> ("TTHLeptonictag_MuonPhotonDrCut");
        TTHLeptonictag_ElePtCut_=iConfig.getParameter<double> ("TTHLeptonictag_ElePtCut");
        TTHLeptonictag_EleEtaCuts_=iConfig.getParameter<vector<double> > ("TTHLeptonictag_EleEtaCuts");
        TTHLeptonictag_ElePhotonDrCut_=iConfig.getParameter<double> ("TTHLeptonictag_ElePhotonDrCut");
        TTHLeptonictag_ElePhotonZMassCut_=iConfig.getParameter<double> ("TTHLeptonictag_ElePhotonZMassCut");
        TTHLeptonictag_DeltaRTrkEle_=iConfig.getParameter<double> ("TTHLeptonictag_DeltaRTrkEle");

      //  diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        inputDiPhotonName_= iConfig.getParameter<std::string > ( "DiPhotonName" );
        inputDiPhotonSuffixes_= iConfig.getParameter<std::vector<std::string> > ( "DiPhotonSuffixes" );
        std::vector<edm::InputTag>  diPhotonTags;
        for (auto & suffix : inputDiPhotonSuffixes_){ 
            systematicsLabels.push_back(suffix);
            std::string inputName = inputDiPhotonName_;
            inputName.append(suffix);
            if (!suffix.empty()) diPhotonTags.push_back(edm::InputTag(inputName));
            else  diPhotonTags.push_back(edm::InputTag(inputDiPhotonName_));
        }
        for( auto & tag : diPhotonTags ) { diPhotonTokens_.push_back( consumes<edm::View<flashgg::DiPhotonCandidate> >( tag ) ); }

        inputJetsName_= iConfig.getParameter<std::string> ( "JetsName" );
        inputJetsCollSize_= iConfig.getParameter<unsigned int> ( "JetsCollSize" );
        inputJetsSuffixes_= iConfig.getParameter<std::vector<std::string> > ( "JetsSuffixes" );
        std::vector<edm::InputTag>  jetTags;
        for (auto & suffix : inputJetsSuffixes_) {
            if (!suffix.empty()) systematicsLabels.push_back(suffix);  //nominal is already put in the diphoton loop
            for (unsigned int i = 0; i < inputJetsCollSize_ ; i++) {
                  std::string bregtag = suffix;
                  bregtag.append(std::to_string(i));
                  jetTags.push_back(edm::InputTag(inputJetsName_,bregtag));
            }         
        }
        for( auto & tag : jetTags ) { jetTokens_.push_back( consumes<edm::View<flashgg::Jet> >( tag ) ); }

      //  auto jetTags = iConfig.getParameter<std::vector<edm::InputTag> > ( "JetTags" ); 
      //  for( auto & tag : jetTags ) { jetTokens_.push_back( consumes<edm::View<flashgg::Jet> >( tag ) ); }

        assert(is_sorted(mvaBoundaries_.begin(), mvaBoundaries_.end()) && "mva boundaries are not in ascending order (we count on that for categorization)");
        doPhotonId_ = iConfig.getUntrackedParameter<bool>("ApplyEGMPhotonID");        
        photonIDCut_ = iConfig.getParameter<double>("PhotonIDCut");

        doMVAFlattening_ = iConfig.getParameter<bool>("doMVAFlattening"); 
        doCategorization_ = iConfig.getParameter<bool>("doCategorization"); 
        dottHTagger_ = iConfig.getParameter<bool>("dottHTagger"); 
        photonElectronVeto_=iConfig.getUntrackedParameter<std::vector<int > >("PhotonElectronVeto");
        //needed for HHbbgg MVA
        if(doMVAFlattening_){
            MVAFlatteningFileName_ = iConfig.getUntrackedParameter<edm::FileInPath>("MVAFlatteningFileName");
            MVAFlatteningFile_ = new TFile((MVAFlatteningFileName_.fullPath()).c_str(),"READ");
            MVAFlatteningCumulative_ = (TGraph*)MVAFlatteningFile_->Get("cumulativeGraph"); 
        }
        MVAscaling_ = iConfig.getParameter<double>("MVAscaling");

        doSigmaMDecorr_ = iConfig.getUntrackedParameter<unsigned int>("DoSigmaMDecorr");
        if(doSigmaMDecorr_){
            sigmaMDecorrFile_ = iConfig.getUntrackedParameter<edm::FileInPath>("SigmaMDecorrFile");
            TFile* f_decorr = new TFile((sigmaMDecorrFile_.fullPath()).c_str(), "READ");
            TH2D* h_decorrEBEB_ = (TH2D*)f_decorr->Get("hist_sigmaM_M_EBEB"); 
            TH2D* h_decorrNotEBEB_ = (TH2D*)f_decorr->Get("hist_sigmaM_M_notEBEB");

            if(h_decorrEBEB_ && h_decorrNotEBEB_){
                transfEBEB_ = new DecorrTransform(h_decorrEBEB_ , 125., 1, 0);
                transfNotEBEB_ = new DecorrTransform(h_decorrNotEBEB_ , 125., 1, 0);

            } else {
                throw cms::Exception( "Configuration" ) << "The file "<<sigmaMDecorrFile_.fullPath()<<" provided for sigmaM/M decorrelation does not contain the expected histograms."<<std::endl;
            }
        }



        if(dottHTagger_)
        {

            //leptons selection
            leptonPtThreshold = iConfig.getParameter<double>("looseLeptonPtThreshold");
            muEtaThreshold = iConfig.getParameter<double>("muonEtaThreshold");
            muPFIsoSumRelThreshold = iConfig.getParameter<double>("muPFIsoSumRelThreshold");

            dRPhoElectronThreshold = iConfig.getParameter<double>("deltaRPhoElectronThreshold");
            dRPhoMuonThreshold = iConfig.getParameter<double>("deltaRPhoMuonThreshold");
            dRJetLeptonThreshold = iConfig.getParameter<double>("deltaRJetLepThreshold");

            useElecMVARecipe = iConfig.getParameter<bool>("useElectronMVARecipe"); 
            useElecLooseId = iConfig.getParameter<bool>("useElectronLooseID");
            elecEtaThresholds = iConfig.getParameter<std::vector<double > >("electronEtaThresholds");
            
            //needed for ttH killer
            METToken_= consumes<View<flashgg::Met> >( iConfig.getParameter<InputTag> ("METTag") ) ;
            electronToken_ = consumes<edm::View<flashgg::Electron> >( iConfig.getParameter<edm::InputTag> ("ElectronTag") );
            muonToken_ = consumes<edm::View<flashgg::Muon> >( iConfig.getParameter<edm::InputTag>("MuonTag") );
            vertexToken_ = consumes<edm::View<reco::Vertex> >( iConfig.getParameter<edm::InputTag> ("VertexTag") );
            rhoToken_ = consumes<double>( iConfig.getParameter<edm::InputTag>( "rhoTag" ) );
            
            ttHWeightfileName_ = iConfig.getUntrackedParameter<FileInPath>("ttHWeightfile");
            ttHScoreThreshold = iConfig.getParameter<double>("ttHScoreThreshold");
            x_mean_ = iConfig.getParameter<std::vector<double>> ("ttHKiller_mean");
            x_std_ = iConfig.getParameter<std::vector<double>> ("ttHKiller_std");
            list_mean_ = iConfig.getParameter<std::vector<double>> ("ttHKiller_listmean");
            list_std_ = iConfig.getParameter<std::vector<double>> ("ttHKiller_liststd");
            graphDef_ttH = tensorflow::loadGraphDef((ttHWeightfileName_.fullPath()).c_str());
            session_ttH = tensorflow::createSession(graphDef_ttH);
        }

       // produces<vector<DoubleHTag>>();
        for (auto & systname : systematicsLabels) {
            produces<vector<DoubleHTag>>(systname);
        }
        produces<vector<TagTruthBase>>();
    }

    int DoubleHTagProducer::chooseCategory( float mvavalue, float mxvalue)
    {
        //// should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        //this is for mva, then you have mx
        if (!doCategorization_) {
            return 0;
        }
        int mvaCat=-1;
        for( int n = 0 ; n < ( int )mvaBoundaries_.size() ; n++ ) {
            if( ( double )mvavalue > mvaBoundaries_[mvaBoundaries_.size() - n - 1] ) {
                mvaCat = n;
                break;
            }
        }

        if (mvaCat==-1) return -1;// Does not pass, object will not be produced

        int mxCat=-1;
        for( unsigned int n = 0 ; n < nMX_ ; n++ ) {
            if( ( double )mxvalue > mxBoundaries_[(mvaCat+1)*nMX_ - n - 1] ) {
                mxCat = n;
                break;
            }
        }


        if (mxCat==-1) return -1;// Does not pass, object will not be produced

        int cat=-1;
        cat = mvaCat*nMX_+mxCat;

        //the schema is like this: (different from HHbbgg_ETH)
        //            "cat0 := MXbin0 * MVAcat0",   #High MX, High MVA
        //            "cat1 := MXbin1 * MVAcat0",   #High but lower MX , High MVA
        //            "cat2 := MXbin2 * MVAcat0",
        //            "cat3 := MXbin0 * MVAcat1",
        //            "cat4 := MXbin1 * MVAcat1",
        // [.................................]

        return cat;

    }


    float DoubleHTagProducer::getGenCosThetaStar_CS(TLorentzVector h1, TLorentzVector h2)
    {
    // cos theta star angle in the Collins Soper frame
        TLorentzVector hh = h1 + h2;
        h1.Boost(-hh.BoostVector());                     
        return h1.CosTheta();
    }

    bool DoubleHTagProducer::isclose(double a, double b, double rel_tol=1e-09, double abs_tol=0.0)
    {
        return fabs(a-b) <= max(rel_tol * max(fabs(a), fabs(b)), abs_tol);
    }
        
    void DoubleHTagProducer::produce( Event &evt, const EventSetup & )
    {

        // update global variables
        globalVariablesComputer_.update(evt);

        //read reweighting
        vector<float> reweight_values;
        if (doReweight_>0) 
        {
           for (auto & reweight_token : reweights_)
           {
                edm::Handle<float> reweight_hadle;
                evt.getByToken(reweight_token, reweight_hadle);
                reweight_values.push_back(*reweight_hadle);
            }
        }
        

        // prepare output
        std::unique_ptr<vector<TagTruthBase> > truths( new vector<TagTruthBase> );
        edm::RefProd<vector<TagTruthBase> > rTagTruth = evt.getRefBeforePut<vector<TagTruthBase> >();

        // MC truth
        TagTruthBase truth_obj;
        double genMhh=0.;
        double genCosThetaStar_CS=0.;
        if( ! evt.isRealData() ) {
            Handle<View<reco::GenParticle> > genParticles;
            std::vector<edm::Ptr<reco::GenParticle> > selHiggses;
            evt.getByToken( genParticleToken_, genParticles );
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
               edm::Ptr<reco::GenParticle> genPar = genParticles->ptrAt(genLoop);
               if (selHiggses.size()>1) break;
              if (genPar->pdgId()==25 && genPar->isHardProcess()){
                  selHiggses.push_back(genPar);
              }   
            }
            if (selHiggses.size()==2){
                TLorentzVector H1,H2;
                H1.SetPtEtaPhiE(selHiggses[0]->p4().pt(),selHiggses[0]->p4().eta(),selHiggses[0]->p4().phi(),selHiggses[0]->p4().energy());
                H2.SetPtEtaPhiE(selHiggses[1]->p4().pt(),selHiggses[1]->p4().eta(),selHiggses[1]->p4().phi(),selHiggses[1]->p4().energy());
                genMhh  = (H1+H2).M();
                genCosThetaStar_CS = getGenCosThetaStar_CS(H1,H2);   
            }
            truths->push_back( truth_obj );
        }

      // read diphotons
      for (unsigned int diphoton_idx = 0; diphoton_idx < diPhotonTokens_.size(); diphoton_idx++) {//looping over all diphoton systematics
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonTokens_[diphoton_idx], diPhotons );
        
        unsigned int loopOverJets = 1;
        if (inputDiPhotonSuffixes_[diphoton_idx].empty()) loopOverJets = inputJetsSuffixes_.size();
        for (unsigned int jet_col_idx = 0; jet_col_idx < loopOverJets; jet_col_idx++) {//looping over all jet systematics, only for nominal diphotons
        std::unique_ptr<vector<DoubleHTag> > tags( new vector<DoubleHTag> );

        // loop over diphotons
        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );

            // kinematic cuts on diphotons
            auto leadPho = dipho->leadingPhoton();
            auto subleadPho = dipho->subLeadingPhoton();

            double leadPt = leadPho->pt();
            double subleadPt = subleadPho->pt();
            if( scalingPtCuts_ ) {
                leadPt /= dipho->mass();
                subleadPt /= dipho->mass();
            }
            if( leadPt <= minLeadPhoPt_ || subleadPt <= minSubleadPhoPt_ ) { continue; }
            //apply egm photon id with given working point
            if(doPhotonId_){
                if(leadPho->userFloat("EGMPhotonMVA")<photonIDCut_ || subleadPho->userFloat("EGMPhotonMVA")<photonIDCut_){
                    continue;
                }
            }
            //electron veto
            if(leadPho->passElectronVeto()<photonElectronVeto_[0] || subleadPho->passElectronVeto()<photonElectronVeto_[1]){
                continue;
            }

            //lepton veto
            Handle<View<reco::Vertex> > vertices;
            evt.getByToken( vertexToken_, vertices );

            std::vector<edm::Ptr<flashgg::Muon> >     Muons2018;
            Handle<View<flashgg::Muon> > theMuons;
            evt.getByToken( muonToken_, theMuons );

            std::vector<edm::Ptr<flashgg::Electron> > Electrons2018;
            Handle<View<flashgg::Electron> > theElectrons;
            evt.getByToken( electronToken_, theElectrons );
            

            if(theMuons->size()>0) {
                Muons2018 = LeptonSelection2018::selectMuons(theMuons->ptrs(), dipho, vertices->ptrs(), TTHLeptonictag_MuonPtCut_, TTHLeptonictag_MuonEtaCut_, TTHLeptonictag_MuonIsoCut_, TTHLeptonictag_MuonPhotonDrCut_, 0);
            }
            if(theElectrons->size()>0) {
               Electrons2018 = LeptonSelection2018::selectElectrons(theElectrons->ptrs(), dipho, TTHLeptonictag_ElePtCut_, TTHLeptonictag_EleEtaCuts_, TTHLeptonictag_ElePhotonDrCut_, TTHLeptonictag_ElePhotonZMassCut_, TTHLeptonictag_DeltaRTrkEle_, 0);
            }


            // find vertex associated to diphoton object
            size_t vtx = (size_t)dipho->jetCollectionIndex();
            // and read corresponding jet collection
    

            edm::Handle<edm::View<flashgg::Jet> > jets;
            evt.getByToken( jetTokens_[jet_col_idx*inputJetsCollSize_+vtx], jets);  //take the corresponding vertex of current systematic

            // photon-jet cross-cleaning and pt/eta/btag/jetid cuts for jets
            std::vector<edm::Ptr<flashgg::Jet> > cleaned_jets;
            for( size_t ijet=0; ijet < jets->size(); ++ijet ) {//jets are ordered in pt
                auto jet = jets->ptrAt(ijet);
                if (jet->pt()<minJetPt_ || fabs(jet->eta())>maxJetEta_)continue;
                double btag=0.;
                for (unsigned int btag_num=0;btag_num<bTagType_.size();btag_num++)
                        btag+=jet->bDiscriminator(bTagType_[btag_num]); 
                if (btag<0) continue;//FIXME threshold might not be 0? For CMVA and DeepCSV it is 0.
                if( useJetID_ ){
                    if( JetIDLevel_ == "Loose" && !jet->passesJetID  ( flashgg::Loose ) ) continue;
                    if( JetIDLevel_ == "Tight" && !jet->passesJetID  ( flashgg::Tight ) ) continue;
                    if( JetIDLevel_ == "Tight2017" && !jet->passesJetID  ( flashgg::Tight2017 ) ) continue;
                    if( JetIDLevel_ == "Tight2018" && !jet->passesJetID  ( flashgg::Tight2018 ) ) continue;
                }
                if( reco::deltaR( *jet, *(dipho->leadingPhoton()) ) > vetoConeSize_ && reco::deltaR( *jet, *(dipho->subLeadingPhoton()) ) > vetoConeSize_ ) {
                    cleaned_jets.push_back( jet );
                }
            }
            if( cleaned_jets.size() < 2 ) { continue; }
            //dijet pair selection. Do pair according to pt and choose the pair with highest b-tag
            double sumbtag_ref = -999;
            bool hasDijet = false;
            edm::Ptr<flashgg::Jet>  jet1, jet2;
            for( size_t ijet=0; ijet < cleaned_jets.size()-1;++ijet){
                auto jet_1 = cleaned_jets[ijet];
                for( size_t kjet=ijet+1; kjet < cleaned_jets.size();++kjet){
                    auto jet_2 = cleaned_jets[kjet];
                    auto dijet_mass = (jet_1->p4()+jet_2->p4()).mass(); 
                    if (dijet_mass<mjjBoundaries_[0] || dijet_mass>mjjBoundaries_[1]) continue;
                    double sumbtag=0.;
                    for (unsigned int btag_num=0;btag_num<bTagType_.size();btag_num++)
                        sumbtag+=jet_1->bDiscriminator(bTagType_[btag_num]) + jet_2->bDiscriminator(bTagType_[btag_num]);
                    if (sumbtag > sumbtag_ref) {
                        hasDijet = true;
                        sumbtag_ref = sumbtag;
                        jet1 = jet_1;
                        jet2 = jet_2;
                    }
                }
            }
            if (!hasDijet)  continue;             

            auto & leadJet = jet1; 
            auto & subleadJet = jet2; 
 
            // prepare tag object
            DoubleHTag tag_obj( dipho, leadJet, subleadJet );
            tag_obj.setDiPhotonIndex( candIndex );
            if (loopOverJets == 1) 
                tag_obj.setSystLabel( inputDiPhotonSuffixes_[diphoton_idx] );
            else  
                tag_obj.setSystLabel( inputJetsSuffixes_[jet_col_idx]);

            if (tag_obj.dijet().mass()<mjjBoundaries_[0] || tag_obj.dijet().mass()>mjjBoundaries_[1]) continue;

            // compute extra variables here
            tag_obj.setMX( tag_obj.p4().mass() - tag_obj.dijet().mass() - tag_obj.diPhoton()->mass() + 250. );
            tag_obj.setGenMhh( genMhh );
            tag_obj.setGenCosThetaStar_CS( genCosThetaStar_CS );
            if (doReweight_>0) tag_obj.setBenchmarkReweight( reweight_values );
            
            if(doSigmaMDecorr_){
                tag_obj.setSigmaMDecorrTransf(transfEBEB_,transfNotEBEB_);
            }


            // eval MVA discriminant
            std::vector<float> mva_vector = mvaComputer_(tag_obj);
            double mva = mva_vector[multiclassSignalIdx_];
            if(doMVAFlattening_){
                double mvaScaled = mva/(mva*(1.-MVAscaling_)+MVAscaling_);
                mva = MVAFlatteningCumulative_->Eval(mvaScaled);
            }

            tag_obj.setEventNumber(evt.id().event() );
            tag_obj.setMVA( mva );
           
            tag_obj.nMuons2018_ = Muons2018.size();
            tag_obj.nElectrons2018_ = Electrons2018.size();

 
            // tag_obj.setMVAprob( mva_vector );

            // tth Tagger
            if (dottHTagger_) 
            {
                HLF_VectorVar_.resize(9);  // High-level features. 9 at the moment
                PL_VectorVar_.resize(8);
                for (int i = 0; i < 8; i++)
                    PL_VectorVar_[i].resize(7); // List of particles. 8 objects. Each object has 7 attributes.

                float sumEt=0.,njets=0.;
                njets = cleaned_jets.size();
                std::vector<flashgg::Jet> cleanedDR_jets;
                std::vector<flashgg::Jet> cleaned_physical_jets; // for Xtt calculation who doesn't take edm::Ptr
                for( size_t ijet=0; ijet < cleaned_jets.size();++ijet){
                    auto jet = cleaned_jets[ijet];
                    cleaned_physical_jets.push_back(*jet);
                    if( reco::deltaR(*jet, *leadJet)< vetoConeSize_) continue;
                    if( reco::deltaR(*jet, *subleadJet)< vetoConeSize_) continue;
                    sumEt+=jet->p4().pt();
                    cleanedDR_jets.push_back(*jet);
                }
                ttHVars["sumET"] = sumEt;
                edm::Handle<View<flashgg::Met> > METs;
                evt.getByToken( METToken_, METs );
                if( METs->size() != 1 )
                { std::cout << "WARNING number of MET is not equal to 1" << std::endl; }
                Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );
                auto p4MET=theMET->p4();
                ttHVars["MET"]=p4MET.pt();
                ttHVars["phiMET"]=p4MET.phi();

                ttHVars["dPhi1"] = reco::deltaPhi(p4MET.Phi(), leadJet->p4().phi());
                ttHVars["dPhi2"] = reco::deltaPhi(p4MET.Phi(), subleadJet->p4().phi());
                ttHVars["PhoJetMinDr"] = tag_obj.getPhoJetMinDr();
                ttHVars["njets"] = njets;
                
                std::vector<flashgg::Jet> DiJet;
                DiJet.push_back(tag_obj.leadJet());
                DiJet.push_back(tag_obj.subleadJet());
                std::vector<float> Xtt = tthKiller_.XttCalculation(cleaned_physical_jets,DiJet);
                if(Xtt.size()>3){
                    ttHVars["Xtt0"] = Xtt[0];
                    ttHVars["Xtt1"] = Xtt[3];
                }else{
                    ttHVars["Xtt0"] = 1000;
                    ttHVars["Xtt1"] = 1000;
                }
                

                edm::Handle<double>  rho;
                evt.getByToken(rhoToken_,rho);
            
                ttHVars["ptjet1"] = leadJet->p4().pt();
                ttHVars["etajet1"] = leadJet->p4().eta();
                ttHVars["phijet1"] = leadJet->p4().phi();

                ttHVars["ptjet2"] = subleadJet->p4().pt();
                ttHVars["etajet2"] = subleadJet->p4().eta();
                ttHVars["phijet2"] = subleadJet->p4().phi();

                ttHVars["ptdipho"] = dipho->p4().pt();
                ttHVars["etadipho"] = dipho->p4().eta();
                ttHVars["phidipho"] = dipho->p4().phi();

                std::vector<edm::Ptr<flashgg::Electron> > selectedElectrons = selectStdAllElectrons( theElectrons->ptrs(), vertices->ptrs(), leptonPtThreshold, elecEtaThresholds, useElecMVARecipe, useElecLooseId, *rho, evt.isRealData() );
                std::vector<edm::Ptr<flashgg::Electron> > tagElectrons = tthKiller_.filterElectrons( selectedElectrons, *tag_obj.diPhoton(), leadJet->p4(), subleadJet->p4(), dRPhoElectronThreshold, dRJetLeptonThreshold);

                if (tagElectrons.size() > 0) 
                {
                    ttHVars["pte1"] = tagElectrons.at( 0 )->p4().pt();
                    ttHVars["etae1"] = tagElectrons.at( 0 )->p4().eta();
                    ttHVars["phie1"] = tagElectrons.at( 0 )->p4().phi();
                }
                else 
                {
                    ttHVars["pte1"] = 0.;
                    ttHVars["etae1"] = 0.;
                    ttHVars["phie1"] = 0.;
                }
                if (tagElectrons.size() > 1) 
                {
                    ttHVars["pte2"] = tagElectrons.at( 1 )->p4().pt();     
                    ttHVars["etae2"] = tagElectrons.at( 1 )->p4().eta();     
                    ttHVars["phie2"] = tagElectrons.at( 1 )->p4().phi();     
                }
                else 
                {
                    ttHVars["pte2"] = 0.;
                    ttHVars["etae2"] = 0.;
                    ttHVars["phie2"] = 0.;
                } 
                std::vector<edm::Ptr<flashgg::Muon> > selectedMuons = selectAllMuons( theMuons->ptrs(), vertices->ptrs(), muEtaThreshold, leptonPtThreshold, muPFIsoSumRelThreshold);
                std::vector<edm::Ptr<flashgg::Muon> > tagMuons = tthKiller_.filterMuons( selectedMuons, *tag_obj.diPhoton(), leadJet->p4(), subleadJet->p4(), dRPhoMuonThreshold, dRJetLeptonThreshold);

                if (tagMuons.size() > 0) 
                {
                    ttHVars["ptmu1"] = tagMuons.at( 0 )->p4().pt();
                    ttHVars["etamu1"] = tagMuons.at( 0 )->p4().eta();
                    ttHVars["phimu1"] = tagMuons.at( 0 )->p4().phi();
                }
                else 
                {
                    ttHVars["ptmu1"] = 0.;
                    ttHVars["etamu1"] = 0.;
                    ttHVars["phimu1"] = 0.;
                }
                if (tagMuons.size() > 1) 
                {
                    ttHVars["ptmu2"] = tagMuons.at( 1 )->p4().pt();    
                    ttHVars["etamu2"] = tagMuons.at( 1 )->p4().eta();    
                    ttHVars["phimu2"] = tagMuons.at( 1 )->p4().phi();    
                }
                else 
                {
                    ttHVars["ptmu2"] = 0.;
                    ttHVars["etamu2"] = 0.;
                    ttHVars["phimu2"] = 0.;
                }

                ttHVars["fabs_CosThetaStar_CS"] = abs(tag_obj.getCosThetaStar_CS_old(6500));//FIXME don't do hardcoded
                ttHVars["fabs_CosTheta_bb"] = abs(tag_obj.CosThetaAngles()[1]);
                
                tag_obj.sumET_ = ttHVars["sumET"];
                tag_obj.MET_ = ttHVars["MET"];
                tag_obj.phiMET_ = ttHVars["phiMET"];
                tag_obj.dPhi1_ = ttHVars["dPhi1"];
                tag_obj.dPhi2_ = ttHVars["dPhi2"];
                tag_obj.PhoJetMinDr_ = ttHVars["PhoJetMinDr"];
                tag_obj.njets_ = ttHVars["njets"];
                tag_obj.Xtt0_ = ttHVars["Xtt0"];
                tag_obj.Xtt1_ = ttHVars["Xtt1"];
                tag_obj.pte1_ = ttHVars["pte1"];
                tag_obj.pte2_ = ttHVars["pte2"];
                tag_obj.ptmu1_ = ttHVars["ptmu1"];
                tag_obj.ptmu2_ = ttHVars["ptmu2"];
                tag_obj.ptdipho_ = ttHVars["ptdipho"];
                tag_obj.etae1_ = ttHVars["etae1"];
                tag_obj.etae2_ = ttHVars["etae2"];
                tag_obj.etamu1_ = ttHVars["etamu1"];
                tag_obj.etamu2_ = ttHVars["etamu2"];
                tag_obj.etadipho_ = ttHVars["etadipho"];
                tag_obj.phie1_ = ttHVars["phie1"];
                tag_obj.phie2_ = ttHVars["phie2"];
                tag_obj.phimu1_ = ttHVars["phimu1"];
                tag_obj.phimu2_ = ttHVars["phimu2"];
                tag_obj.phidipho_ = ttHVars["phidipho"];
                tag_obj.fabs_CosThetaStar_CS_ = ttHVars["fabs_CosThetaStar_CS"];
                tag_obj.fabs_CosTheta_bb_ = ttHVars["fabs_CosTheta_bb"];
                tag_obj.ptjet1_ = ttHVars["ptjet1"];
                tag_obj.ptjet2_ = ttHVars["ptjet2"];
                tag_obj.etajet1_ = ttHVars["etajet1"];
                tag_obj.etajet2_ = ttHVars["etajet2"];
                tag_obj.phijet1_ = ttHVars["phijet1"];
                tag_obj.phijet2_ = ttHVars["phijet2"];
                
                StandardizeHLF();
                
                //10 HLFs: 'sumEt','dPhi1','dPhi2','PhoJetMinDr','njets','Xtt0',
                //'Xtt1','fabs_CosThetaStar_CS','fabs_CosTheta_bb'
                HLF_VectorVar_[0] = ttHVars["sumET"];
                HLF_VectorVar_[1] = ttHVars["dPhi1"];
                HLF_VectorVar_[2] = ttHVars["dPhi2"];
                HLF_VectorVar_[3] = ttHVars["PhoJetMinDr"];
                HLF_VectorVar_[4] = ttHVars["njets"];
                HLF_VectorVar_[5] = ttHVars["Xtt0"];
                HLF_VectorVar_[6] = ttHVars["Xtt1"];
                HLF_VectorVar_[7] = ttHVars["fabs_CosThetaStar_CS"];
                HLF_VectorVar_[8] = ttHVars["fabs_CosTheta_bb"];

                // 6 objects: ele1, ele2, mu1, mu2, dipho, MET
                // Each object has 7 attributes: pt, eta, phi, isele, ismuon, isdipho, isMET
                //
                // 0: leading ele
                PL_VectorVar_[0][0] = ttHVars["pte1"];
                PL_VectorVar_[0][1] = ttHVars["etae1"];
                PL_VectorVar_[0][2] = ttHVars["phie1"];
                PL_VectorVar_[0][3] = (isclose(ttHVars["pte1"],0)) ? 0 : 1; // isEle
                PL_VectorVar_[0][4] = 0; // isMuon
                PL_VectorVar_[0][5] = 0; // isDiPho
                PL_VectorVar_[0][6] = 0; // isMET

                // 1: subleading ele
                PL_VectorVar_[1][0] = ttHVars["pte2"];
                PL_VectorVar_[1][1] = ttHVars["etae2"];
                PL_VectorVar_[1][2] = ttHVars["phie2"];
                PL_VectorVar_[1][3] = (isclose(ttHVars["pte2"],0)) ? 0 : 1; // isEle
                PL_VectorVar_[1][4] = 0; // isMuon
                PL_VectorVar_[1][5] = 0; // isDiPho
                PL_VectorVar_[1][6] = 0; // isMET

                // 2: leading muon
                PL_VectorVar_[2][0] = ttHVars["ptmu1"];
                PL_VectorVar_[2][1] = ttHVars["etamu1"];
                PL_VectorVar_[2][2] = ttHVars["phimu1"];
                PL_VectorVar_[2][3] = 0; // isEle
                PL_VectorVar_[2][4] = (isclose(ttHVars["ptmu1"],0)) ? 0 : 1; // isMuon
                PL_VectorVar_[2][5] = 0; // isDiPho
                PL_VectorVar_[2][6] = 0; // isMET

                // 3: subleading muon
                PL_VectorVar_[3][0] = ttHVars["ptmu2"];
                PL_VectorVar_[3][1] = ttHVars["etamu2"];
                PL_VectorVar_[3][2] = ttHVars["phimu2"];
                PL_VectorVar_[3][3] = 0; //isEle
                PL_VectorVar_[3][4] = (isclose(ttHVars["ptmu2"],0)) ? 0 : 1; // isMuon
                PL_VectorVar_[3][5] = 0; // isDiPho
                PL_VectorVar_[3][6] = 0; // isMET

                // 4: dipho
                PL_VectorVar_[4][0] = ttHVars["ptdipho"];
                PL_VectorVar_[4][1] = ttHVars["etadipho"];
                PL_VectorVar_[4][2] = ttHVars["phidipho"];
                PL_VectorVar_[4][3] = 0; // isEle
                PL_VectorVar_[4][4] = 0; // isMuon
                PL_VectorVar_[4][5] = (isclose(ttHVars["ptdipho"],0)) ? 0 : 1; // isDiPho
                PL_VectorVar_[4][6] = 0; // isMET

                // 5: MET
                PL_VectorVar_[5][0] = ttHVars["MET"];
                PL_VectorVar_[5][1] = 0; // MET eta
                PL_VectorVar_[5][2] = ttHVars["phiMET"];
                PL_VectorVar_[5][3] = 0; //isEle
                PL_VectorVar_[5][4] = 0; // isMuon
                PL_VectorVar_[5][5] = 0; // isDiPho
                PL_VectorVar_[5][6] = (isclose(ttHVars["MET"],0)) ? 0 : 1; // isMET

                // 6: leading jet
                PL_VectorVar_[6][0] = ttHVars["ptjet1"];
                PL_VectorVar_[6][1] = ttHVars["etajet1"];
                PL_VectorVar_[6][2] = ttHVars["phijet1"];
                PL_VectorVar_[6][3] = 0; //isEle
                PL_VectorVar_[6][4] = 0; // isMuon
                PL_VectorVar_[6][5] = 0; // isDiPho
                PL_VectorVar_[6][6] = 0; // isMET 

                // 7: subleading jet
                PL_VectorVar_[7][0] = ttHVars["ptjet2"];
                PL_VectorVar_[7][1] = ttHVars["etajet2"];
                PL_VectorVar_[7][2] = ttHVars["phijet2"];
                PL_VectorVar_[7][3] = 0; //isEle
                PL_VectorVar_[7][4] = 0; // isMuon
                PL_VectorVar_[7][5] = 0; // isDiPho
                PL_VectorVar_[7][6] = 0; // isMET

                // Sort by pT
                std::sort(PL_VectorVar_.rbegin(), PL_VectorVar_.rend()); 

                StandardizeParticleList();

                float ttHScore = EvaluateNN();
                if (ttHScore < ttHScoreThreshold) continue;
               
                tag_obj.ntagMuons_ = tagMuons.size();
                tag_obj.ntagElectrons_ = tagElectrons.size();

                tag_obj.ttHScore_ = ttHScore;
                PL_VectorVar_.clear();
                HLF_VectorVar_.clear();
            }
            
            // choose category and propagate weights
            int catnum = chooseCategory( tag_obj.MVA(), tag_obj.MX() );
            tag_obj.setCategoryNumber( catnum );
            tag_obj.includeWeights( *dipho );
            //tag_obj.includeWeights( *leadJet );
            //tag_obj.includeWeights( *subleadJet );
            
           // tag_obj.includeWeightsByLabel( *leadJet ,"JetBTagReshapeWeight",false);
          //  tag_obj.includeWeightsByLabel( *subleadJet , "JetBTagReshapeWeight",false );
            tag_obj.includeWeightsByLabel( *leadJet ,"JetBTagReshapeWeight");
            tag_obj.includeWeightsByLabel( *subleadJet , "JetBTagReshapeWeight" );



          if (catnum>-1){
                if (doCategorization_) {
                    if (tag_obj.dijet().mass()<mjjBoundariesLower_[catnum] || tag_obj.dijet().mass()>mjjBoundariesUpper_[catnum]) continue;
                }
                tags->push_back( tag_obj );
                // link mc-truth
                if( ! evt.isRealData() ) {
                    tags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );                 
                }
          }
        }
        if (loopOverJets == 1) 
            evt.put( std::move( tags ),inputDiPhotonSuffixes_[diphoton_idx] );
        else  
            evt.put( std::move( tags ),inputJetsSuffixes_[jet_col_idx] );
        }
        }   
        evt.put( std::move( truths ) );
    }
    
    void DoubleHTagProducer::StandardizeHLF()
    {
        // Standardize the HLF inputs. NOTE: We don't standardize pt, eta, phi of physics object here.
        ttHVars["sumET"] = (ttHVars["sumET"] - x_mean_[0])/x_std_[0];
        ttHVars["dPhi1"] = (ttHVars["dPhi1"] - x_mean_[3])/x_std_[3];
        ttHVars["dPhi2"] = (ttHVars["dPhi2"] - x_mean_[4])/x_std_[4];
        ttHVars["PhoJetMinDr"] = (ttHVars["PhoJetMinDr"] - x_mean_[5])/x_std_[5];
        ttHVars["njets"] = (ttHVars["njets"] - x_mean_[6])/x_std_[6];
        ttHVars["Xtt0"] = (ttHVars["Xtt0"] - x_mean_[7])/x_std_[7];
        ttHVars["Xtt1"] = (ttHVars["Xtt1"] - x_mean_[8])/x_std_[8];
        ttHVars["fabs_CosThetaStar_CS"] = (ttHVars["fabs_CosThetaStar_CS"] - x_mean_[24])/x_std_[24];
        ttHVars["fabs_CosTheta_bb"] = (ttHVars["fabs_CosTheta_bb"] - x_mean_[25])/x_std_[25];
    }
    
    void DoubleHTagProducer::StandardizeParticleList()
    {
        // Standardize pt, eta, phi of physics objects
        for (unsigned int i = 0; i < 8; i++) // 8 objects
        {
            if (!isclose(PL_VectorVar_[i][0],0)) // only standardize object that exists (non-zero pt)
            for (unsigned int j = 0; j < 3; j++) // pt, eta, phi for each objects
            {
                 PL_VectorVar_[i][j] = (PL_VectorVar_[i][j] - list_mean_[j])/(list_std_[j]);
            }
        }
        
    }

    float DoubleHTagProducer::EvaluateNN()
    {
        unsigned int shape = HLF_VectorVar_.size();
        unsigned int plshape1 = PL_VectorVar_.size();
        unsigned int plshape2 = PL_VectorVar_[0].size();
        
        tensorflow::Tensor HLFinput(tensorflow::DT_FLOAT, {1,shape});
        //std::cout << "Input high level feature: ";
        for (unsigned int i = 0; i < shape; i++){
            HLFinput.matrix<float>()(0,i) =  float(HLF_VectorVar_[i]);
            //std::cout << HLF_VectorVar_[i] << "  ";
        }
        tensorflow::Tensor PLinput(tensorflow::DT_FLOAT, tensorflow::TensorShape({1,plshape1, plshape2}));
        //std::cout << "\nInput particle list: \n";
        for (unsigned int i = 0; i < plshape1; i++)
        {
            for (unsigned int j = 0; j < plshape2; j++)
            {
                PLinput.tensor<float,3>()(0, i, j) = float(PL_VectorVar_[i][j]);
                //std::cout << PL_VectorVar_[i][j] << "  ";
            }
            //std::cout << std::endl;
        }
        std::vector<tensorflow::Tensor> outputs;

        tensorflow::run(session_ttH, { {"input_1:0", PLinput}, {"input_2:0", HLFinput} }, { "dense_4/Sigmoid" }, &outputs);
        //std::cout << "EvaluateNN result: " << outputs[0].matrix<float>()(0, 0) << std::endl;
        float NNscore = outputs[0].matrix<float>()(0, 0);
        return NNscore;
    }
    
}

typedef flashgg::DoubleHTagProducer FlashggDoubleHTagProducer;
DEFINE_FWK_MODULE( FlashggDoubleHTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
