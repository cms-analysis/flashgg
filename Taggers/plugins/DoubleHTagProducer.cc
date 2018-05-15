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
        bool scalingPtCuts_, doMVAFlattening_;
        double vetoConeSize_;         

        flashgg::MVAComputer<DoubleHTag> mvaComputer_;
        vector<double> mvaBoundaries_, mxBoundaries_;

        edm::FileInPath MVAFlatteningFileName_;
        TFile * MVAFlatteningFile_;
        TGraph * MVAFlatteningCumulative_;
    };

    DoubleHTagProducer::DoubleHTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) ),
        minLeadPhoPt_( iConfig.getParameter<double> ( "MinLeadPhoPt" ) ),
        minSubleadPhoPt_( iConfig.getParameter<double> ( "MinSubleadPhoPt" ) ),
        scalingPtCuts_( iConfig.getParameter<bool> ( "ScalingPtCuts" ) ),
        vetoConeSize_( iConfig.getParameter<double> ( "VetoConeSize" ) ),
        mvaComputer_( iConfig.getParameter<edm::ParameterSet>("MVAConfig") )
    {
        mvaBoundaries_ = iConfig.getParameter<vector<double > >( "MVABoundaries" );
        mxBoundaries_ = iConfig.getParameter<vector<double > >( "MXBoundaries" );

        auto jetTags = iConfig.getParameter<std::vector<edm::InputTag> > ( "JetTags" ); 
        for( auto & tag : jetTags ) { jetTokens_.push_back( consumes<edm::View<flashgg::Jet> >( tag ) ); }
        
        assert(is_sorted(mvaBoundaries_.begin(), mvaBoundaries_.end()) && "mva boundaries are not in ascending order (we count on that for categorization)");
        assert(is_sorted(mxBoundaries_.begin(), mxBoundaries_.end()) && "mx boundaries are not in ascending order (we count on that for categorization)");
        
        doMVAFlattening_ = iConfig.getParameter<bool>("doMVAFlattening"); 
        //MVAFlatteningFile_ = iConfig.getParameter<edm::FileInPath>("MVAFlatteningFile");
        
        if(doMVAFlattening_){
            MVAFlatteningFileName_=iConfig.getUntrackedParameter<edm::FileInPath>("MVAFlatteningFileName");
            MVAFlatteningFile_ = new TFile((MVAFlatteningFileName_.fullPath()).c_str(),"READ");
            MVAFlatteningCumulative_ = (TGraph*)MVAFlatteningFile_->Get("cumulativeGraph"); 
        }

        // SigmaMpTTag
        produces<vector<DoubleHTag> >();
        produces<vector<TagTruthBase> >();
    }

    int DoubleHTagProducer::chooseCategory( float mvavalue, float mxvalue)
    {
        //// should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        //this is for mva, then you have mx
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

        //the schema is like this:
        //            "cat0 := MXbin0 * MVAcat0",
        //            "cat1 := MXbin1 * MVAcat0",
        //            "cat2 := MXbin2 * MVAcat0",
        //            "cat3 := MXbin0 * MVAcat1",
        //            "cat4 := MXbin1 * MVAcat1",
        // [.................................]

        return cat;

    }

    void DoubleHTagProducer::produce( Event &evt, const EventSetup & )
    {
        std::cout<<"starting producer"<<std::endl;
        // read diphotons
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        // prepare output
        std::unique_ptr<vector<DoubleHTag> > tags( new vector<DoubleHTag> );
        std::unique_ptr<vector<TagTruthBase> > truths( new vector<TagTruthBase> );
        edm::RefProd<vector<TagTruthBase> > rTagTruth = evt.getRefBeforePut<vector<TagTruthBase> >();
        
        // MC truth
        TagTruthBase truth_obj;
        if( ! evt.isRealData() ) {
            Handle<View<reco::GenParticle> > genParticles;
            evt.getByToken( genParticleToken_, genParticles );
            Point higgsVtx(0.,0.,0.);
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                if( pdgid == 25 || pdgid == 22 ) {
                    higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                    break;
                }
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
            
            
            // find vertex associated to diphoton object
            size_t vtx = (size_t)dipho->vertexIndex();
            if( vtx >= jetTokens_.size() ) { vtx = 0; }
            // and read corresponding jet collection
            edm::Handle<edm::View<flashgg::Jet> > jets;
            evt.getByToken( jetTokens_[vtx], jets);
            
            // photon-jet cross-cleaning
            std::vector<edm::Ptr<flashgg::Jet> > cleaned_jets;
            for( size_t ijet=0; ijet < jets->size(); ++ijet ) {
                auto jet = jets->ptrAt(ijet);
                if( reco::deltaR( *jet, *(dipho->leadingPhoton()) ) > vetoConeSize_ && reco::deltaR( *jet, *(dipho->subLeadingPhoton()) ) > vetoConeSize_ ) {
                    cleaned_jets.push_back( jet );
                }
            }
            if( cleaned_jets.size() < 2 ) { continue; }
            auto & leadJet = cleaned_jets[0];
            auto & subleadJet = cleaned_jets[1];

            // prepare tag object
            DoubleHTag tag_obj( dipho, leadJet, subleadJet );
            tag_obj.setDiPhotonIndex( candIndex );
            tag_obj.setSystLabel( systLabel_ );
            
            // compute extra variables here
            tag_obj.setMX( tag_obj.dijet().mass() + tag_obj.diPhoton()->mass() - 250. );

            //            std::cout<<tag_obj.getCosThetaStar_CS(tag_obj.diPhoton()->p4(),tag_obj.dijet(),6500)<<std::endl;
            // eval MVA discriminant
            std::cout<<"before computer"<<std::endl;
            double mva = mvaComputer_(tag_obj);
            std::cout<<"mva"<<std::endl;
            if(doMVAFlattening_){
                mva = MVAFlatteningCumulative_->Eval(mva);
            }

            tag_obj.setMVA( mva );
            
            // choose category and propagate weights
            int catnum = chooseCategory( tag_obj.MVA(), tag_obj.MX() );
            std::cout<<"cat:"<<catnum<<" "<<tag_obj.MVA()<<" "<<tag_obj.MX()<<std::endl;
            tag_obj.setCategoryNumber( catnum );
            tag_obj.includeWeights( *dipho );
            tag_obj.includeWeights( *leadJet );
            tag_obj.includeWeights( *subleadJet );

            if (catnum>-1){
                tags->push_back( tag_obj );
                // link mc-truth
                if( ! evt.isRealData() ) {
                    tags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );                   }
            }
        }
        std::cout<<"done"<<std::endl;
        evt.put( std::move( truths ) );
        evt.put( std::move( tags ) );
        std::cout<<"done"<<std::endl;
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
