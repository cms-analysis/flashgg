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

#include "TLorentzVector.h"


#include <vector>
#include <algorithm>
#include "TH2F.h"
#include "TFile.h"

using namespace std;
using namespace edm;

namespace flashgg {

    class DoubleHReweighter : public EDProducer
    {

    public:
        DoubleHReweighter( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        float getWeight( int targetNode,float gen_mHH, float gen_cosTheta);
        float getCosThetaStar_CS(TLorentzVector h1, TLorentzVector h2, float ebeam);

        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        int targetNode_;
        edm::FileInPath fullGridWeightsFile_;
        edm::FileInPath benchmarksWeightsFile_;
        const unsigned int NUM_benchmarks = 12; // number of becnhmarks for reweighting
        const unsigned int NUM_gridsize = 1507; // size of the grid used for reweighting
        std::vector<TH2F*> hists_fullgrid;
        TFile* f_fullgrid_ ;
        TFile* f_benchmarks_ ;

    };

    DoubleHReweighter::DoubleHReweighter( const ParameterSet &iConfig ) :
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        targetNode_( iConfig.getParameter<int> ( "targetNode" ) ),
        fullGridWeightsFile_(iConfig.getUntrackedParameter<edm::FileInPath>("fullGridWeightsFile")),
        benchmarksWeightsFile_(iConfig.getUntrackedParameter<edm::FileInPath>("benchmarksWeightsFile"))
    {
        //Get the taget node from the config file
        //also write in the config file the path and name for the root files needed for the reweighting

            f_fullgrid_ = new TFile((fullGridWeightsFile_.fullPath()).c_str(), "READ");
            f_benchmarks_ = new TFile((benchmarksWeightsFile_.fullPath()).c_str(), "READ");
            
            for (unsigned int n=0; n<NUM_gridsize; n++){
                hists_fullgrid.push_back((TH2F*)f_fullgrid_->Get(Form("point_%i_weights",n)));
                if (!(hists_fullgrid[n])) throw cms::Exception( "Configuration" ) << "The file "<<fullGridWeightsFile_.fullPath()<<" provided for reweighting full grid does not contain the expected histograms."<<std::endl;
            }
            for (unsigned int n=0; n<NUM_benchmarks; n++){
                hists_fullgrid.push_back((TH2F*)f_benchmarks_->Get(Form("point_%i_weights",n)));
                if (!(hists_fullgrid[n])) throw cms::Exception( "Configuration" ) << "The file "<<benchmarksWeightsFile_.fullPath()<<" provided for reweighting benchmarks does not contain the expected histograms."<<std::endl;
            }

        produces<float>();
    }
    

    float DoubleHReweighter::getWeight( int targetNode,float gen_mHH, float gen_cosTheta)
    {
        float w = 0.;
          // The points do not exist in the input file provided by Alexandra (and wont ever be added)
        if (targetNode==324 || targetNode==910 || targetNode==985 || targetNode==990)  w=0;
        else 
        {
            unsigned int binNum = hists_fullgrid[targetNode]->FindBin(gen_mHH, fabs(gen_cosTheta));
            w = hists_fullgrid[targetNode]->GetBinContent(binNum);
        }
  
    return w;
    }

    
    float DoubleHReweighter::getCosThetaStar_CS(TLorentzVector h1, TLorentzVector h2, float ebeam)
    {
    // cos theta star angle in the Collins Soper frame
    // Copied directly from here: https://github.com/ResonantHbbHgg/Selection/blob/master/selection.h#L3367-L3385
        TLorentzVector p1, p2;
        p1.SetPxPyPzE(0, 0,  ebeam, ebeam);
        p2.SetPxPyPzE(0, 0, -ebeam, ebeam);

        TLorentzVector hh;
        hh = h1 + h2;

        TVector3 boost = - hh.BoostVector();
        p1.Boost(boost);
        p2.Boost(boost);
        h1.Boost(boost);

        TVector3 CSaxis = p1.Vect().Unit() - p2.Vect().Unit();
        CSaxis.Unit();

        return cos(   CSaxis.Angle( h1.Vect().Unit() )    );
    }



    void DoubleHReweighter::produce( Event &evt, const EventSetup & )
    {

        // prepare output
      //  std::unique_ptr<vector<float> > final_weight( new vector<float> );
        std::unique_ptr<float>  final_weight( new float(0.) );
        std::vector<edm::Ptr<reco::GenParticle> > selHiggses;
        Handle<View<reco::GenParticle> > genParticles;
        evt.getByToken( genParticleToken_, genParticles );
        for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
            edm::Ptr<reco::GenParticle> genPar = genParticles->ptrAt(genLoop);
            if (genPar->pdgId()==25 && genPar->isHardProcess()){
                if (selHiggses.size()==0) selHiggses.push_back(genPar);
                if (selHiggses.size()==1) selHiggses.push_back(genPar);
                if (selHiggses.size()==2) break; 
            }   
        }
       
        if (selHiggses.size()==2){
            TLorentzVector H1,H2;
            H1.SetPtEtaPhiE(selHiggses[0]->p4().pt(),selHiggses[0]->p4().eta(),selHiggses[0]->p4().phi(),selHiggses[0]->p4().energy());
            H2.SetPtEtaPhiE(selHiggses[1]->p4().pt(),selHiggses[1]->p4().eta(),selHiggses[1]->p4().phi(),selHiggses[1]->p4().energy());
            float gen_mHH  = (H1+H2).M();
            // histograms are provided only up to gen_mHH < 1800 GeV
            if (gen_mHH<1800) {
                float gen_cosTheta = getCosThetaStar_CS(H1,H2,6500.);   //beam energy 6500
                // Now, lets fill in the weigts for the 12 benchmarks.
                std::vector<float> NRWeights; //we will use this in the future when we would like to save all weights
                for (unsigned int n=0; n<NUM_benchmarks; n++) 
                    NRWeights.push_back(getWeight(NUM_gridsize+n, gen_mHH, gen_cosTheta));
                float NRWeight_target =  NRWeights[NUM_gridsize-2+targetNode_]; //In our convention nodes start from node_2 and go up to node_13, therefore shift -2
                ( *final_weight ) = NRWeight_target;
            }
        } 
        evt.put( std::move( final_weight ) );
     //   for ....
    //    evt.put( std::move( final_weight ,number) );
    }
}




typedef flashgg::DoubleHReweighter FlashggDoubleHReweighter;
DEFINE_FWK_MODULE( FlashggDoubleHReweighter );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
