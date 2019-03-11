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
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/DoubleHTag.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "flashgg/MicroAOD/interface/MVAComputer.h"


#include <vector>
#include <algorithm>
#include "TGraph.h"

using namespace std;
using namespace edm;

namespace flashgg {

    class DoubleHTagProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        DoubleHTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int chooseCategory( float mva, float mx );
        
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        std::vector<edm::EDGetTokenT<edm::View<flashgg::Jet> > > jetTokens_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;

        double minLeadPhoPt_, minSubleadPhoPt_;
        bool scalingPtCuts_, doPhotonId_, doMVAFlattening_, doCategorization_;
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

        GlobalVariablesDumper globalVariablesDumper_;
        MVAComputer<DoubleHTag> mvaComputer_;
        vector<double> mvaBoundaries_, mxBoundaries_;
        int multiclassSignalIdx_;

        FileInPath MVAFlatteningFileName_;
        TFile * MVAFlatteningFile_;
        TGraph * MVAFlatteningCumulative_;
        double MVAscaling_;
    };

    DoubleHTagProducer::DoubleHTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) ),
        minLeadPhoPt_( iConfig.getParameter<double> ( "MinLeadPhoPt" ) ),
        minSubleadPhoPt_( iConfig.getParameter<double> ( "MinSubleadPhoPt" ) ),
        scalingPtCuts_( iConfig.getParameter<bool> ( "ScalingPtCuts" ) ),
        vetoConeSize_( iConfig.getParameter<double> ( "VetoConeSize" ) ),
        minJetPt_( iConfig.getParameter<double> ( "MinJetPt" ) ),
        maxJetEta_( iConfig.getParameter<double> ( "MaxJetEta" ) ),
        bTagType_( iConfig.getParameter<vector<std::string>>( "BTagType") ),
        useJetID_( iConfig.getParameter<bool>   ( "UseJetID"     ) ),
        JetIDLevel_( iConfig.getParameter<string> ( "JetIDLevel"   ) ),
        globalVariablesDumper_(iConfig.getParameter<edm::ParameterSet>("globalVariables")),
        mvaComputer_(iConfig.getParameter<edm::ParameterSet>("MVAConfig"),  &globalVariablesDumper_)
    {
        mjjBoundaries_ = iConfig.getParameter<vector<double > >( "MJJBoundaries" ); 
        mvaBoundaries_ = iConfig.getParameter<vector<double > >( "MVABoundaries" );
        mxBoundaries_ = iConfig.getParameter<vector<double > >( "MXBoundaries" );
        mjjBoundariesLower_ = iConfig.getParameter<vector<double > >( "MJJBoundariesLower" ); 
        mjjBoundariesUpper_ = iConfig.getParameter<vector<double > >( "MJJBoundariesUpper" ); 
        multiclassSignalIdx_ = (iConfig.getParameter<edm::ParameterSet>("MVAConfig")).getParameter<int>("multiclassSignalIdx"); 

        auto jetTags = iConfig.getParameter<std::vector<edm::InputTag> > ( "JetTags" ); 
        for( auto & tag : jetTags ) { jetTokens_.push_back( consumes<edm::View<flashgg::Jet> >( tag ) ); }
        
        assert(is_sorted(mvaBoundaries_.begin(), mvaBoundaries_.end()) && "mva boundaries are not in ascending order (we count on that for categorization)");
        assert(is_sorted(mxBoundaries_.begin(), mxBoundaries_.end()) && "mx boundaries are not in ascending order (we count on that for categorization)");
        doPhotonId_ = iConfig.getUntrackedParameter<bool>("ApplyEGMPhotonID");        
        photonIDCut_ = iConfig.getParameter<double>("PhotonIDCut");
        
        doMVAFlattening_ = iConfig.getParameter<bool>("doMVAFlattening"); 
        doCategorization_ = iConfig.getParameter<bool>("doCategorization"); 
        photonElectronVeto_=iConfig.getUntrackedParameter<std::vector<int > >("PhotonElectronVeto");
        //needed for HHbbgg MVA
        if(doMVAFlattening_){
            MVAFlatteningFileName_=iConfig.getUntrackedParameter<edm::FileInPath>("MVAFlatteningFileName");
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

        //needed for ttH MVA
//        METToken_( consumes<View<flashgg::Met> >( iConfig.getParameter<InputTag> ( "METTag" ) ) );
//        electronToken_ = consumes<edm::View<flashgg::Electron> >( iConfig.getParameter<edm::InputTag> ( "ElectronTag" ) );
//        muonToken_ = consumes<edm::View<flashgg::Muon> >( iConfig.getParameter<edm::InputTag>( "MuonTag" ) );



        produces<vector<DoubleHTag> >();
        produces<vector<TagTruthBase> >();
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
        for( int n = 0 ; n < ( int )mxBoundaries_.size() ; n++ ) {
            if( ( double )mxvalue > mxBoundaries_[mxBoundaries_.size() - n - 1] ) {
                mxCat = n;
                break;
            }
        }


        if (mxCat==-1) return -1;// Does not pass, object will not be produced

        int cat=-1;
        cat = mvaCat*mxBoundaries_.size()+mxCat;

        //the schema is like this: (different from HHbbgg_ETH)
        //            "cat0 := MXbin0 * MVAcat0",   #High MX, High MVA
        //            "cat1 := MXbin1 * MVAcat0",   #High but lower MX , High MVA
        //            "cat2 := MXbin2 * MVAcat0",
        //            "cat3 := MXbin0 * MVAcat1",
        //            "cat4 := MXbin1 * MVAcat1",
        // [.................................]

        return cat;

    }

    void DoubleHTagProducer::produce( Event &evt, const EventSetup & )
    {
        // read diphotons
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        // prepare output
        std::unique_ptr<vector<DoubleHTag> > tags( new vector<DoubleHTag> );
        std::unique_ptr<vector<TagTruthBase> > truths( new vector<TagTruthBase> );
        edm::RefProd<vector<TagTruthBase> > rTagTruth = evt.getRefBeforePut<vector<TagTruthBase> >();
        
        // MC truth
        TagTruthBase truth_obj;
        double genMhh=0.;
        if( ! evt.isRealData() ) {
            Handle<View<reco::GenParticle> > genParticles;
            std::vector<edm::Ptr<reco::GenParticle> > selHiggses;
            evt.getByToken( genParticleToken_, genParticles );
            Point higgsVtx(0.,0.,0.);
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                if( pdgid == 25 || pdgid == 22 ) {
                    higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                    break;
                }
            }
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
            }
            truth_obj.setGenPV( higgsVtx );
            truths->push_back( truth_obj );
        }

        // loop over diphotons
        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );
            
            // kinematic cuts on diphotons
            auto & leadPho = *(dipho->leadingPhoton());
            auto & subleadPho = *(dipho->subLeadingPhoton());
            
            double leadPt = leadPho.pt();
            double subleadPt = subleadPho.pt();
            if( scalingPtCuts_ ) {
                leadPt /= dipho->mass();
                subleadPt /= dipho->mass();
            }
            if( leadPt <= minLeadPhoPt_ || subleadPt <= minSubleadPhoPt_ ) { continue; }
            //apply egm photon id with given working point
            if(doPhotonId_){
                if(leadPho.userFloat("EGMPhotonMVA")<photonIDCut_ || subleadPho.userFloat("EGMPhotonMVA")<photonIDCut_){
                    continue;
                }
            }
            //electron veto
            if(leadPho.passElectronVeto()<photonElectronVeto_[0] || subleadPho.passElectronVeto()<photonElectronVeto_[1]){
                continue;
            }
            
            
            // find vertex associated to diphoton object
            size_t vtx = (size_t)dipho->jetCollectionIndex();
            // and read corresponding jet collection
            edm::Handle<edm::View<flashgg::Jet> > jets;
            evt.getByToken( jetTokens_[vtx], jets);
            
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
            tag_obj.setSystLabel( systLabel_ );
            
            if (tag_obj.dijet().mass()<mjjBoundaries_[0] || tag_obj.dijet().mass()>mjjBoundaries_[1]) continue;

            // compute extra variables here
            tag_obj.setMX( tag_obj.p4().mass() - tag_obj.dijet().mass() - tag_obj.diPhoton()->mass() + 250. );
            tag_obj.setGenMhh( genMhh );
            
            if(doSigmaMDecorr_){
                tag_obj.setSigmaMDecorrTransf(transfEBEB_,transfNotEBEB_);
            }
            
            // eval MVA discriminant
            std::vector<float> mva_vector = mvaComputer_.predict_prob(tag_obj);
            double mva = mva_vector[multiclassSignalIdx_];
            if(doMVAFlattening_){
                double mvaScaled = mva/(mva*(1.-MVAscaling_)+MVAscaling_);
                mva = MVAFlatteningCumulative_->Eval(mvaScaled);
            }

            tag_obj.setMVA( mva );
            //            tag_obj.setMVAprob( mva_vector );
            
            // choose category and propagate weights
            int catnum = chooseCategory( tag_obj.MVA(), tag_obj.MX() );
            tag_obj.setCategoryNumber( catnum );
            tag_obj.includeWeights( *dipho );
            //            tag_obj.includeWeights( *leadJet );
            //            tag_obj.includeWeights( *subleadJet );

            //            tag_obj.includeWeightsByLabel( *leadJet ,"JetBTagReshapeWeight");
            //            tag_obj.includeWeightsByLabel( *subleadJet , "JetBTagReshapeWeight" );



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
        evt.put( std::move( truths ) );
        evt.put( std::move( tags ) );
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
