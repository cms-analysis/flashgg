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

#include "TMVA/Reader.h"
#include "TMath.h"
#include "TH2.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include <string>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class PrefireDiPhotonProducer : public EDProducer
    {
    public:
        PrefireDiPhotonProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        double getProb( TH2F*, double, double );
        double getProbUnc( TH2F*, double, double );

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

        bool isRelevant_;
        bool applyToCentral_;

        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        std::vector<edm::InputTag> inputTagJets_;
        FileInPath photonFileName_;
        std::string photonHistName_;
        TH2F* photonHist_;
        FileInPath jetFileName_;
        std::string jetHistName_;
        TH2F* jetHist_;
    };

    PrefireDiPhotonProducer::PrefireDiPhotonProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        inputTagJets_ ( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) )
    {
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
        produces<std::vector<flashgg::DiPhotonCandidate> >();

        isRelevant_ = iConfig.getParameter<bool>("isRelevant"); // used to skip all this for 2018

        if (isRelevant_) {
            applyToCentral_ = iConfig.getParameter<bool>("applyToCentral");

            photonFileName_ = iConfig.getParameter<edm::FileInPath>( "photonFileName" );
            TFile* photonFile = TFile::Open(photonFileName_.fullPath().c_str());
            photonHistName_ = iConfig.getUntrackedParameter<std::string>("photonHistName");
            photonHist_ = (TH2F*)((TH2F*) photonFile->Get(photonHistName_.c_str()))->Clone();
            photonFile->Close();
            delete photonFile;

            jetFileName_  = iConfig.getParameter<edm::FileInPath>( "jetFileName" );
            TFile* jetFile = TFile::Open(jetFileName_.fullPath().c_str());
            jetHistName_ = iConfig.getUntrackedParameter<std::string>("jetHistName");
            jetHist_ = (TH2F*)((TH2F*) jetFile->Get(jetHistName_.c_str()))->Clone();
            jetFile->Close();
            delete jetFile;
        }

    }

    double PrefireDiPhotonProducer::getProb( TH2F* hist, double eta, double pt )
    {
        return hist->GetBinContent( hist->FindBin(eta, pt) );
    }

    double PrefireDiPhotonProducer::getProbUnc( TH2F* hist, double eta, double pt )
    {
        return hist->GetBinError( hist->FindBin(eta, pt) );
    }

    void PrefireDiPhotonProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByToken( tokenJets_[j], Jets[j] );
        }

        unique_ptr<std::vector<flashgg::DiPhotonCandidate> > updatedDiphotons( new std::vector<flashgg::DiPhotonCandidate>() );
        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt(candIndex);

            // calculate the prefire probability as
            // 1 - product_over_jets( 1 - p(jet_prefires) )

            double prefireProd = 1.;
            double prefireProdUnc = 0.;

            if (isRelevant_) {
                unsigned int jetCollectionIndex = diPhotons->ptrAt( candIndex )->jetCollectionIndex();
                for( UInt_t jetLoop = 0; jetLoop < Jets[jetCollectionIndex]->size() ; jetLoop++ ) {
                    Ptr<flashgg::Jet> jet  = Jets[jetCollectionIndex]->ptrAt( jetLoop );
                    // check if the jet overlaps with one of the photons
                    // if so use the photon probability map, else use the jet one
                    bool nearLead  = deltaR( dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi(), jet->eta(), jet->phi() ) < 0.4;
                    bool nearSublead = deltaR( dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi(), jet->eta(), jet->phi() ) < 0.4;
                    double objProb = -1.;
                    double objProbUnc = -1;
                    if (nearLead) {
                        objProb = getProb(photonHist_, dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->pt());
                        objProbUnc = getProbUnc(photonHist_, dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->pt());
                    }
                    else if (nearSublead) {
                        objProb = getProb(photonHist_, dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->pt());
                        objProbUnc = getProbUnc(photonHist_, dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->pt());
                    }
                    else {
                        objProb = getProb(jetHist_, jet->eta(), jet->pt());
                        objProbUnc = getProbUnc(jetHist_, jet->eta(), jet->pt());
                    }
                    prefireProd *= 1. - objProb;
                    prefireProdUnc += pow(objProbUnc, 2); // add statistical uncs in quadrature
                }
            }
      
            double prefireProbability = 1. - prefireProd;
            if (prefireProbability < 0. || prefireProbability > 1.) {
                throw cms::Exception("Unphysical probablility found");
            }

            prefireProdUnc = pow(prefireProdUnc, 0.5);

            double prefireProbabilityUnc = std::max(0.2 * prefireProbability, prefireProdUnc);

            flashgg::DiPhotonCandidate *updatedDipho = dipho->clone();
            WeightedObject prefireObject;
            if (applyToCentral_) {
                prefireObject.setCentralWeight( (1. - prefireProbability) );
                prefireObject.setWeight("prefireWeightUp01sigma",   std::min(1.,  1. - prefireProbability + prefireProbabilityUnc));
                prefireObject.setWeight("prefireWeightDown01sigma", std::max(0.,  1. - prefireProbability - prefireProbabilityUnc));
                updatedDipho->includeWeights(prefireObject);
            }
            else {
                prefireObject.setWeight("prefireWeight", (1. - prefireProbability) );
                prefireObject.setWeight("prefireWeightUp01sigma",   std::min(1.,  1. - prefireProbability + prefireProbabilityUnc));
                prefireObject.setWeight("prefireWeightDown01sigma", std::max(0.,  1. - prefireProbability - prefireProbabilityUnc));
                updatedDipho->includeWeights(prefireObject);
            }

            updatedDiphotons->push_back(*updatedDipho);
            delete updatedDipho;
        }

        evt.put( std::move(updatedDiphotons) );
    }
}

typedef flashgg::PrefireDiPhotonProducer FlashggPrefireDiPhotonProducer;
DEFINE_FWK_MODULE( FlashggPrefireDiPhotonProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

