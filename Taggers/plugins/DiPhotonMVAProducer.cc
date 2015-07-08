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

#include "TMVA/Reader.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;

namespace flashgg {

    class DiPhotonMVAProducer : public EDProducer
    {

    public:
        DiPhotonMVAProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<reco::BeamSpot > beamSpotToken_;

        unique_ptr<TMVA::Reader>DiphotonMva_;
        FileInPath diphotonMVAweightfile_;

        float sigmarv_;
        float sigmawv_;
        float vtxprob_;
        float leadptom_;
        float subleadptom_;
        float leadeta_;
        float subleadeta_;
        float CosPhi_;
        float leadmva_;
        float subleadmva_;

        float nConv_;
        float vtxProbMVA_;
        vector<double> vertex_prob_params_conv;
        vector<double> vertex_prob_params_noConv;

        std::string Version_;
    };

    DiPhotonMVAProducer::DiPhotonMVAProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        beamSpotToken_( consumes<reco::BeamSpot >( iConfig.getUntrackedParameter<InputTag>( "BeamSpotTag", InputTag( "offlineBeamSpot" ) ) ) )
    {
        vertex_prob_params_conv = iConfig.getParameter<vector<double>>( "VertexProbParamsConv" );
        vertex_prob_params_noConv = iConfig.getParameter<vector<double>>( "VertexProbParamsNoConv" );

        diphotonMVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "diphotonMVAweightfile" );

        Version_ = iConfig.getParameter<string>( "Version" );

        //        std::cout << "Version" << Version_ << std::endl;

        sigmarv_ = 0.;
        sigmawv_ = 0.;
        vtxprob_ = 0.;
        leadptom_ = 0.;
        subleadptom_ = 0.;
        leadeta_ = 0.;
        subleadeta_ = 0.;
        CosPhi_ = 0.;
        leadmva_ = 0.;
        subleadmva_ = 0.;

        std::string version_old = "old";
        std::string version_new = "new";

        if( version_old.compare( Version_ ) == 0 ) {
            DiphotonMva_.reset( new TMVA::Reader( "!Color:Silent" ) );
            DiphotonMva_->AddVariable( "masserrsmeared/mass", &sigmarv_ );
            DiphotonMva_->AddVariable( "masserrsmearedwrongvtx/mass", &sigmawv_ );
            DiphotonMva_->AddVariable( "vtxprob", &vtxprob_ );
            DiphotonMva_->AddVariable( "ph1.pt/mass", &leadptom_ );
            DiphotonMva_->AddVariable( "ph2.pt/mass", &subleadptom_ );
            DiphotonMva_->AddVariable( "ph1.eta", &leadeta_ );
            DiphotonMva_->AddVariable( "ph2.eta", &subleadeta_ );
            DiphotonMva_->AddVariable( "TMath::Cos(ph1.phi-ph2.phi)", &CosPhi_ );
            DiphotonMva_->AddVariable( "ph1.idmva", &leadmva_ );
            DiphotonMva_->AddVariable( "ph2.idmva", &subleadmva_ );
            DiphotonMva_->BookMVA( "BDT", diphotonMVAweightfile_.fullPath() );
            //            std::cout << "finished reading mva" << std::endl;
        }

        if( version_new.compare( Version_ ) == 0 ) {
            //            std::cout << "Reading MVA variables " << std::endl;
            DiphotonMva_.reset( new TMVA::Reader( "!Color:Silent" ) );
            DiphotonMva_->AddVariable( "leadptom", &leadptom_ );
            DiphotonMva_->AddVariable( "subleadptom", &subleadptom_ );
            DiphotonMva_->AddVariable( "leadmva", &leadmva_ );
            DiphotonMva_->AddVariable( "subleadmva", &subleadmva_ );
            DiphotonMva_->AddVariable( "leadeta", &leadeta_ );
            DiphotonMva_->AddVariable( "subleadeta", &subleadeta_ );
            DiphotonMva_->AddVariable( "sigmarv", &sigmarv_ );
            DiphotonMva_->AddVariable( "sigmawv", &sigmawv_ );
            DiphotonMva_->AddVariable( "CosPhi", &CosPhi_ );
            DiphotonMva_->AddVariable( "vtxprob", &vtxprob_ );
            DiphotonMva_->BookMVA( "BDT", diphotonMVAweightfile_.fullPath() );
            //            std::cout << "finished reading mva" << std::endl;
        }
        produces<vector<DiPhotonMVAResult> >(); // one per diphoton, always in same order, vector is more efficient than map
    }

    void DiPhotonMVAProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        // const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

        Handle<reco::BeamSpot> recoBeamSpotHandle;
        evt.getByToken( beamSpotToken_, recoBeamSpotHandle );
        float beamsig;
        if( recoBeamSpotHandle.isValid() ) {
            beamsig = recoBeamSpotHandle->sigmaZ();
        } else {
            beamsig = -9999; // I hope this never happens! But it seems to in our default test, what's going wrong??
        }

        //    std::auto_ptr<DiPhotonMVAResultMap> assoc(new DiPhotonMVAResultMap);
        std::auto_ptr<vector<DiPhotonMVAResult> > results( new vector<DiPhotonMVAResult> ); // one per diphoton, always in same order, vector is more efficient than map

        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            flashgg::DiPhotonMVAResult mvares;
            edm::Ptr<reco::Vertex> vtx = diPhotons->ptrAt( candIndex )->vtx();

            const flashgg::Photon *g1 = diPhotons->ptrAt( candIndex )->leadingPhoton();
            const flashgg::Photon *g2 = diPhotons->ptrAt( candIndex )->subLeadingPhoton();

            //used for photon resolution wrt to correct vertex//
            TVector3 Photon1Dir;
            TVector3 Photon1Dir_uv;
            TVector3 Photon2Dir;
            TVector3 Photon2Dir_uv;
            TLorentzVector p14;
            TLorentzVector p24;
            Photon1Dir.SetXYZ( g1->superCluster()->position().x() - vtx->position().x(), g1->superCluster()->position().y() - vtx->position().y(),
                               g1->superCluster()->position().z() - vtx->position().z() );
            Photon2Dir.SetXYZ( g2->superCluster()->position().x() - vtx->position().x(), g2->superCluster()->position().y() - vtx->position().y(),
                               g2->superCluster()->position().z() - vtx->position().z() );
            Photon1Dir_uv = Photon1Dir.Unit() * g1->superCluster()->rawEnergy();
            Photon2Dir_uv = Photon2Dir.Unit() * g2->superCluster()->rawEnergy();
            p14.SetPxPyPzE( Photon1Dir_uv.x(), Photon1Dir_uv.y(), Photon1Dir_uv.z(), g1->superCluster()->rawEnergy() );
            p24.SetPxPyPzE( Photon2Dir_uv.x(), Photon2Dir_uv.y(), Photon2Dir_uv.z(), g2->superCluster()->rawEnergy() );
            //photon 4-vector with respect to correct vertex and superCluster hit//

            //

            //float angle = p14.Angle(p24.Vect());

            float x1 = g1->superCluster()->position().x() - vtx->position().x();
            float y1 = g1->superCluster()->position().y() - vtx->position().y();
            float z1 = g1->superCluster()->position().z() - vtx->position().z();

            float x2 = g2->superCluster()->position().x() - vtx->position().x();
            float y2 = g2->superCluster()->position().y() - vtx->position().y();
            float z2 = g2->superCluster()->position().z() - vtx->position().z();

            float r1 = TMath::Sqrt( x1 * x1 + y1 * y1 + z1 * z1 );
            float r2 = TMath::Sqrt( x2 * x2 + y2 * y2 + z2 * z2 );

            float cos_term = TMath::Cos( p14.Phi() - p24.Phi() );
            float sech1 = 1.0 / TMath::CosH( p14.Eta() );
            float sech2 = 1.0 / TMath::CosH( p24.Eta() );
            float tanh1 = 1.0 / TMath::TanH( p14.Phi() );
            float tanh2 = 1.0 / TMath::TanH( p24.Phi() );
            float numerator1 = sech1 * ( sech1 * tanh2 - tanh1 * sech2 * cos_term );
            float numerator2 = sech2 * ( sech2 * tanh1 - tanh2 * sech1 * cos_term );
            float denominator = 1. - tanh1 * tanh2 - sech1 * sech2 * cos_term;
            float angleResolutionWrgVtx = ( ( -1.*beamsig * TMath::Sqrt( .2 ) ) / denominator ) * ( numerator1 / r1 + numerator2 / r2 ); //dz = sigmabeamspot*sqrt(2)
            //float angleResolutionCorrVtx = ((-1.*0.1*TMath::Sqrt(.2))/denominator)*(numerator1/r1 + numerator2/r2);//dz = 0.1
            float alpha_sig_wrg = 0.5 * angleResolutionWrgVtx;
            //float alpha_sig_corr = angleResolutionCorrVtx;
            float SigmaM = 0.5 * TMath::Sqrt( g1->sigEOverE() * g1->sigEOverE() + g2->sigEOverE() * g2->sigEOverE() );
            float MassResolutionWrongVtx = TMath::Sqrt( ( SigmaM * SigmaM ) + ( alpha_sig_wrg * alpha_sig_wrg ) );


            leadptom_       = g1->pt() / ( diPhotons->ptrAt( candIndex )->mass() );
            subleadptom_    = g2->pt() / ( diPhotons->ptrAt( candIndex )->mass() );
            subleadmva_     = g2->phoIdMvaDWrtVtx( vtx );
            leadmva_        = g1->phoIdMvaDWrtVtx( vtx );
            leadeta_        = g2->eta();
            subleadeta_     = g1->eta();
            sigmarv_        = .5 * sqrt( ( g1->sigEOverE() ) * ( g1->sigEOverE() ) + ( g2->sigEOverE() ) * ( g2->sigEOverE() ) );
            sigmawv_        = MassResolutionWrongVtx;
            CosPhi_         = TMath::Cos( deltaPhi( g1->phi(), g2->phi() ) );

            vtxProbMVA_ = diPhotons->ptrAt( candIndex )->vtxProbMVA();
            nConv_ = diPhotons->ptrAt( candIndex )->nConv();

            if( nConv_ > 0 ) {
                vtxprob_        = ( 1 + vertex_prob_params_conv.at( 0 ) - vertex_prob_params_conv.at( 1 ) + vertex_prob_params_conv.at( 2 ) - vertex_prob_params_conv.at(
                                        3 ) ) + vertex_prob_params_conv.at( 0 ) * vtxProbMVA_ + vertex_prob_params_conv.at( 1 ) * pow( vtxProbMVA_,
                                                2 ) + vertex_prob_params_conv.at( 2 ) * pow( vtxProbMVA_, 3 ) + vertex_prob_params_conv.at( 3 ) * pow( vtxProbMVA_, 4 );
            }

            else {
                vtxprob_        = ( 1 + vertex_prob_params_noConv.at( 0 ) - vertex_prob_params_noConv.at( 1 ) + vertex_prob_params_noConv.at(
                                        2 ) - vertex_prob_params_noConv.at( 3 ) ) + vertex_prob_params_noConv.at( 0 ) * vtxProbMVA_ + vertex_prob_params_noConv.at( 1 ) * pow( vtxProbMVA_,
                                                2 ) + vertex_prob_params_noConv.at( 2 ) * pow( vtxProbMVA_, 3 ) + vertex_prob_params_noConv.at( 3 ) * pow( vtxProbMVA_, 4 );
            }

            mvares.result = DiphotonMva_->EvaluateMVA( "BDT" );

            mvares.leadptom = leadptom_;
            mvares.subleadptom = subleadptom_;
            mvares.leadmva = leadmva_;
            mvares.subleadmva = subleadmva_;
            mvares.leadeta = leadeta_;
            mvares.subleadeta = subleadeta_;
            mvares.sigmarv = sigmarv_;
            mvares.sigmawv = sigmawv_;
            mvares.CosPhi = CosPhi_;
            mvares.vtxprob = vtxprob_;

            results->push_back( mvares );
        }
        evt.put( results );
    }
}

typedef flashgg::DiPhotonMVAProducer FlashggDiPhotonMVAProducer;
DEFINE_FWK_MODULE( FlashggDiPhotonMVAProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

