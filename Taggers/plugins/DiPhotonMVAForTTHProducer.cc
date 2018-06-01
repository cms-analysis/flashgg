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

#include "flashgg/DataFormats/interface/SinglePhotonView.h"

#include "flashgg/Taggers/interface/FunctionHelpers.h"

#include "TFile.h"

#include "TMVA/Reader.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TH2D.h"

using namespace std;
using namespace edm;

namespace flashgg {

    class DiPhotonMVAForTTHProducer : public EDProducer
    {

    public:
        DiPhotonMVAForTTHProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;

        unique_ptr<TMVA::Reader> DiphotonMva_;
        FileInPath diphotonMVAweightfile_;

        float sigmarv_;
        float leadptom_;
        float subleadptom_;
        float leadeta_;
        float subleadeta_;
        float leadsigmaEoE_;
        float subleadsigmaEoE_;
        float CosPhi_;
        float leadmva_;
        float subleadmva_;
    };

    DiPhotonMVAForTTHProducer::DiPhotonMVAForTTHProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) )
    {
        diphotonMVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "diphotonMVAweightfile" );

        sigmarv_ = 0.;
        leadptom_ = 0.;
        subleadptom_ = 0.;
        leadeta_ = 0.;
        subleadeta_ = 0.;
        leadsigmaEoE_ = 0;
        subleadsigmaEoE_ = 0;
        CosPhi_ = 0.;
        leadmva_ = 0.;
        subleadmva_ = 0.;

        DiphotonMva_.reset( new TMVA::Reader( "!Color:Silent" ) );
        DiphotonMva_->AddVariable( "dipho_leadEta", &leadeta_ );
        DiphotonMva_->AddVariable( "dipho_subleadEta", &subleadeta_ );
        DiphotonMva_->AddVariable( "dipho_lead_ptoM", &leadptom_ );
        DiphotonMva_->AddVariable( "dipho_sublead_ptoM", &subleadptom_ );
        DiphotonMva_->AddVariable( "dipho_leadIDMVA", &leadmva_ );
        DiphotonMva_->AddVariable( "dipho_subleadIDMVA", &subleadmva_ );
        DiphotonMva_->AddVariable( "dipho_lead_sigmaEoE", &leadsigmaEoE_ );
        DiphotonMva_->AddVariable( "dipho_sublead_sigmaEoE", &subleadsigmaEoE_ );
        DiphotonMva_->AddVariable( "dipho_sigmaRV", &sigmarv_ );
        DiphotonMva_->AddVariable( "dipho_cosDeltaphi", &CosPhi_ );

        DiphotonMva_->BookMVA( "BDT", diphotonMVAweightfile_.fullPath() );

        produces<vector<DiPhotonMVAResult> >();
    }

    void DiPhotonMVAForTTHProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        //    std::auto_ptr<DiPhotonMVAResultMap> assoc(new DiPhotonMVAResultMap);
        std::unique_ptr<vector<DiPhotonMVAResult> > results( new vector<DiPhotonMVAResult> ); // one per diphoton, always in same order, vector is more efficient than map

        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ )
        {
            flashgg::DiPhotonMVAResult mvares;
            edm::Ptr<reco::Vertex> vtx = diPhotons->ptrAt( candIndex )->vtx();

            const flashgg::Photon *g1 = diPhotons->ptrAt( candIndex )->leadingPhoton();
            const flashgg::Photon *g2 = diPhotons->ptrAt( candIndex )->subLeadingPhoton();
            float SigmaM = 0.5 * TMath::Sqrt( g1->sigEOverE() * g1->sigEOverE() + g2->sigEOverE() * g2->sigEOverE() );

            leadptom_       = g1->pt() / ( diPhotons->ptrAt( candIndex )->mass() );
            subleadptom_    = g2->pt() / ( diPhotons->ptrAt( candIndex )->mass() );

            leadmva_        = g1->phoIdMvaDWrtVtx( vtx );
            subleadmva_     = g2->phoIdMvaDWrtVtx( vtx );

            leadeta_        = g1->eta();
            subleadeta_     = g2->eta();

            leadsigmaEoE_ = g2 -> sigEOverE();
            subleadsigmaEoE_ = g2 -> sigEOverE();

            sigmarv_        = SigmaM;
            CosPhi_         = TMath::Cos( deltaPhi( g1->phi(), g2->phi() ) );

            mvares.result = DiphotonMva_->EvaluateMVA( "BDT" );

            mvares.leadptom = leadptom_;
            mvares.subleadptom = subleadptom_;
            mvares.leadmva = leadmva_;
            mvares.subleadmva = subleadmva_;
            mvares.leadeta = leadeta_;
            mvares.subleadeta = subleadeta_;
            mvares.leadSigmaEoE = leadsigmaEoE_;
            mvares.subleadSigmaEoE = subleadsigmaEoE_;
            mvares.sigmarv = sigmarv_;
            mvares.CosPhi = CosPhi_;

            results->push_back( mvares );
        }
        evt.put( std::move(results) );
    }
}

typedef flashgg::DiPhotonMVAForTTHProducer FlashggDiPhotonMVAForTTHProducer;
DEFINE_FWK_MODULE( FlashggDiPhotonMVAForTTHProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

