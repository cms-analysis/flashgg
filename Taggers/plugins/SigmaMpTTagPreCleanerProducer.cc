#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/SigmaMpTTag.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"


#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class SigmaMpTTagPreCleanerProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        SigmaMpTTagPreCleanerProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int chooseCategory( float, float );

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        std::vector<EDGetTokenT<View<reco::CompositeCandidate> > > compositeCandidatesTokens_;
        string systLabel_;
        bool requireScaledPtCuts_;

        std::vector<std::string> compCandNames_;

        vector<double> boundaries_sigmaMoM;
        vector<double> boundaries_pToM;
        bool integratepT_;
    };

    SigmaMpTTagPreCleanerProducer::SigmaMpTTagPreCleanerProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) ),
        requireScaledPtCuts_   ( iConfig.getParameter<bool> ( "RequireScaledPtCuts" ) )
    {
        //        compositeCandidateToken_( consumes<View<reco::CompositeCandidate> >( iConfig.getParameter<InputTag> ( "CompositeCandidateTag" ) ) ),
        if( iConfig.exists("CompositeCandidateTags") ){
            const auto CompositeCandidateTags = iConfig.getParameter<ParameterSet>("CompositeCandidateTags");
            compCandNames_ =  CompositeCandidateTags.getParameterNamesForType<InputTag>();
            for(auto & compCandName : compCandNames_){
                compositeCandidatesTokens_.push_back( consumes<View<reco::CompositeCandidate> >( CompositeCandidateTags.getParameter<InputTag> ( compCandName ) )  ) ;
            }
        }
        boundaries_sigmaMoM = iConfig.getParameter<vector<double > >( "BoundariesSigmaMoM" );
        //integratepT_ = iConfig.existsAs<vector<double >>("BoundariespToM") ? true: false;
        integratepT_ = true;
        if(!integratepT_){
            boundaries_pToM = iConfig.getParameter<vector<double > >( "BoundariespToM" );
        }
        assert( is_sorted( boundaries_sigmaMoM.begin(), boundaries_sigmaMoM.end() ) ); // we are counting on ascending order - update this to give an error message or exception
        assert( is_sorted( boundaries_pToM.begin(), boundaries_pToM.end() ) ); // we are counting on ascending order - update this to give an error message or exception
        produces<vector<SigmaMpTTag> >();
        produces<vector<TagTruthBase> >();
    }

    int SigmaMpTTagPreCleanerProducer::chooseCategory( float sigmaMoMvalue, float pToMvalue )
    {
        /// std::cout<<"sigmaM/M "<<sigmaMoMvalue<<", pT/M "<<pToMvalue<<std::endl;
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n1;
        int n2;
        for( n1 = 0 ; n1 < ( int )boundaries_sigmaMoM.size() ; n1++ ) {
            if( ( double )sigmaMoMvalue < boundaries_sigmaMoM[n1] ) { break; }
        }
        if(n1==0 || n1==(int)boundaries_sigmaMoM.size()){return -1;}//boundaries have to be provided including lowest and highest: if below lowest (0) or above highest (size) return -1 

        if(!integratepT_){
            for( n2 = 0 ; n2 < ( int )boundaries_pToM.size() ; n2++ ) {
                if( ( double )pToMvalue < boundaries_pToM[n2] ) { break; }
            }
            if(n2==0 || n2==(int)boundaries_pToM.size()){return -1;}//boundaries have to be provided including lowest and highest: if below lowest (0) or above highest (size) return -1 
        
            // std::cout<<"if we are here, we didn't throw away the event, n1 "<<n1<<", n2 "<<n2<<" and the cat number is  "<<((n1-1)*(boundaries_sigmaMoM.size()-1) + (n2-1))<<std::endl;
        return (n1-1)*((int)boundaries_sigmaMoM.size()-1) + (n2-1); // Does not pass, object will not be produced
        }
        else{
            return n1-1;
        }

    }

    void SigmaMpTTagPreCleanerProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        //  const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();



        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
//   const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();

        Handle<View<reco::GenParticle> > genParticles;

        std::vector<Handle<View<reco::CompositeCandidate> > > compositeCandidateHandles(compositeCandidatesTokens_.size());
        for(size_t itok=0; itok<compositeCandidatesTokens_.size(); itok++){
            evt.getByToken(compositeCandidatesTokens_[itok], compositeCandidateHandles[itok]);
        }

        std::unique_ptr<vector<SigmaMpTTag> > tags( new vector<SigmaMpTTag> );
        std::unique_ptr<vector<TagTruthBase> > truths( new vector<TagTruthBase> );

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

        assert( diPhotons->size() == mvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception

        unsigned int idx = 0;

        // Je ne comprends pas ces RefProds, mais je le fais
        edm::RefProd<vector<TagTruthBase> > rTagTruth = evt.getRefBeforePut<vector<TagTruthBase> >();

        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );
            
            //edm::Ptr<reco::CompositeCandidate> > compcands = compositeCandidates->ptrAt( candIndex );
            
            std::map<std::string, edm::Ptr<reco::CompositeCandidate> > compObjMap;
            for (size_t itok=0; itok<compositeCandidatesTokens_.size(); itok++){
                edm::Ptr<reco::CompositeCandidate> compcand = compositeCandidateHandles[itok]->ptrAt(candIndex);
                compObjMap.insert(std::pair<std::string, edm::Ptr<reco::CompositeCandidate> >(compCandNames_[itok], compcand) );
            }
            

            //            SigmaMpTTag tag_obj( dipho, mvares );
            SigmaMpTTag tag_obj( dipho, mvares, compObjMap );
            tag_obj.setDiPhotonIndex( candIndex );

            tag_obj.setSystLabel( systLabel_ );

            int catnum = chooseCategory( mvares->decorrSigmarv, dipho->pt()/dipho->mass() );
            /// std::cout<<"cat num assigned is "<<catnum<<std::endl;
            tag_obj.setCategoryNumber( catnum );

            tag_obj.includeWeights( *dipho );

            bool passScaledPtCuts = 1;
            if ( requireScaledPtCuts_ ) {

                float pt_over_mgg_1 = (dipho->leadingPhoton()->pt() / dipho->mass());
                float pt_over_mgg_2 = (dipho->subLeadingPhoton()->pt() / dipho->mass());

                passScaledPtCuts = ( pt_over_mgg_1 > (1./3) && pt_over_mgg_2 > (1./4) );
                //                std::cout << " pt_over_mgg_1=" << pt_over_mgg_1 << " pt_over_mgg_2=" << pt_over_mgg_2 << " passScaledPtCuts=" << passScaledPtCuts << std::endl;
            }

            if( passScaledPtCuts && tag_obj.categoryNumber() >= 0 ) {
                tags->push_back( tag_obj );
                if( ! evt.isRealData() ) {
                    TagTruthBase truth_obj;
                    truth_obj.setGenPV( higgsVtx );
                    truths->push_back( truth_obj );
                    tags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, idx++ ) ) );
                }
            }
        }
        evt.put( std::move(tags) );
        evt.put( std::move(truths) );
    }
}

typedef flashgg::SigmaMpTTagPreCleanerProducer FlashggSigmaMpTTagPreCleanerProducer;
DEFINE_FWK_MODULE( FlashggSigmaMpTTagPreCleanerProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
