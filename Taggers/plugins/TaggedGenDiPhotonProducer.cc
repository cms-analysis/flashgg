#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/MicroAOD/interface/ClassNameClassifier.h"

#include "flashgg/DataFormats/interface/GenDiPhoton.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

#include "flashgg/DataFormats/interface/GenPhotonExtra.h"

#include "TMVA/Reader.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;

namespace flashgg {


    class TaggedGenDiPhotonProducer : public EDProducer
    {
        typedef ClassNameClassifier<DiPhotonTagBase> classifier_t;
    public:
        TaggedGenDiPhotonProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        float getCosThetaStar_CS(TLorentzVector h1, const TLorentzVector &h2);

        EDGetTokenT<View<GenDiPhoton> > src_;
        EDGetTokenT<View<DiPhotonTagBase> > tags_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        classifier_t classifier_;

        vector< edm::EDGetTokenT<float> > HHbbgg_reweights_;
        int doReweight_;
        bool ForceGenDiphotonProduction_;
    };

    TaggedGenDiPhotonProducer::TaggedGenDiPhotonProducer( const ParameterSet &iConfig ) :
        src_( consumes<View<flashgg::GenDiPhoton> >( iConfig.getParameter<InputTag> ( "src" ) ) ),
        tags_( consumes<View<flashgg::DiPhotonTagBase> >( iConfig.getParameter<InputTag> ( "tags" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        classifier_(iConfig)
    {
        doReweight_ = (iConfig.getParameter<int>("HHbbgg_doReweight")); 
        ForceGenDiphotonProduction_ = (iConfig.getParameter<bool>("ForceGenDiphotonProduction"));

        auto names = iConfig.getParameter<vector<string>>("HHbbgg_reweight_names");
        for (auto & name : names ) {
            HHbbgg_reweights_.push_back(consumes<float>(edm::InputTag(iConfig.getParameter<string>("HHbbgg_reweight_producer") , name))) ;
        }
        produces<vector<GenDiPhoton> >();
    }
    
    void TaggedGenDiPhotonProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonTagBase> > tags;
        evt.getByToken( tags_, tags );
        if(tags.isValid())
            assert( tags->size() < 2 );
        
        double weight = 1.;
        std::pair<std::string,int> info("",0);
        if( tags.isValid() && tags->size() > 0 ) {
            info = classifier_(tags->at(0));
            weight = tags->at(0).centralWeight();
        }
        
        //find gen higgs particles
        float genMhh=-999;
        float genCosThetaStar_CS=-999;
        float ptH1=-999;
        float ptH2=-999;

        if( ! evt.isRealData() ) {
            TLorentzVector H1,H2;
            Handle<View<reco::GenParticle> > genParticles;
            std::vector<edm::Ptr<reco::GenParticle> > selHiggses;
            evt.getByToken( genParticleToken_, genParticles );
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
               edm::Ptr<reco::GenParticle> genPar = genParticles->ptrAt(genLoop);
               if (selHiggses.size()>1) break;
               if (genPar->pdgId()==25 && genPar->isHardProcess() ){
                   selHiggses.push_back(genPar);
               }   
            }

            if (selHiggses.size()>0) {
                ptH1 = selHiggses[0]->p4().pt();
                if (selHiggses.size()==2){
                    H1.SetPtEtaPhiE(selHiggses[0]->p4().pt(),selHiggses[0]->p4().eta(),selHiggses[0]->p4().phi(),selHiggses[0]->p4().energy());
                    H2.SetPtEtaPhiE(selHiggses[1]->p4().pt(),selHiggses[1]->p4().eta(),selHiggses[1]->p4().phi(),selHiggses[1]->p4().energy());
                    genMhh  = (H1+H2).M();
                    genCosThetaStar_CS = this->getCosThetaStar_CS(H1,H2);   
                    ptH1 = selHiggses[0]->p4().pt();
                    ptH2 = selHiggses[1]->p4().pt();
                }
            }
        }


        Handle<View<flashgg::GenDiPhoton> > src;
        evt.getByToken( src_, src );
        std::unique_ptr<vector<GenDiPhoton> > diphotons( new vector<GenDiPhoton> );

        //---In order to count also Dalitz decay events we create a dummy diphoton object 
        //   that will be placed into the event stream
        GenDiPhoton newdipho;
        if(src->size()==1)
            newdipho = src->at(0);
        else{
            if(!ForceGenDiphotonProduction_){
                evt.put( std::move( diphotons ) );
                return;
            }
        }

        newdipho.setTag(info.first);
        newdipho.setCategoryNumber(info.second);
        newdipho.setCentralWeight(weight);
        newdipho.setmHHgen(genMhh);
        newdipho.setcosthetaHHgen(genCosThetaStar_CS);
        newdipho.setptHsgen(ptH1,ptH2);
        if( tags.isValid() && tags->size()>0 ) { 
            newdipho.setTagObj(tags->ptrAt(0)); 
        }
        //read reweighting
        vector<float> reweight_values;
        if (doReweight_>0) {
            for (auto & reweight_token : HHbbgg_reweights_){
                edm::Handle<float> reweight_handle;
                evt.getByToken(reweight_token, reweight_handle);
                if(reweight_handle.isValid())
                    reweight_values.push_back(*reweight_handle);
            }
            newdipho.setHHbbggBenchmarkReweight( reweight_values );
        }
        diphotons->push_back(newdipho);
    
        evt.put( std::move( diphotons ) );
    }

    float TaggedGenDiPhotonProducer::getCosThetaStar_CS(TLorentzVector h1, const TLorentzVector &h2)
    {
        // cos theta star angle in the Collins Soper frame
        TLorentzVector hh = h1 + h2;
        h1.Boost(-hh.BoostVector());                     
        return h1.CosTheta();
    }

}


typedef flashgg::TaggedGenDiPhotonProducer FlashggTaggedGenDiPhotonProducer;
DEFINE_FWK_MODULE( FlashggTaggedGenDiPhotonProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

