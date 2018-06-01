// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/DataFormats/interface/Muon.h"

using namespace std;
using namespace edm;

namespace flashgg {

    class MuonProducer : public edm::EDProducer
    {
    public:
        MuonProducer( const edm::ParameterSet & );

    private:
        void produce( edm::Event &, const edm::EventSetup & );

        edm::EDGetTokenT<View<pat::Muon> > muonToken_;
        edm::EDGetTokenT<View<pat::PackedCandidate> > pfcandidateToken_;
        double muminiso_r_min_ = 0.05;
        double muminiso_r_max_ = 0.2;
        double muminiso_kt_scale_ = 10.;
        double muminiso_deadcone_ch_ = 0.0001;
        double muminiso_deadcone_pu_ = 0.01;
        double muminiso_deadcone_ph_ = 0.01;
        double muminiso_deadcone_nh_ = 0.01;
        double muminiso_ptThresh_ = 0.5;
        double muminiso_ptThresh_phot_ = 1.0;

    };

    MuonProducer::MuonProducer( const ParameterSet &iConfig ):
        muonToken_( consumes<View<pat::Muon> >( iConfig.getParameter<InputTag>( "muonTag" ) ) ),
        pfcandidateToken_( consumes<View<pat::PackedCandidate> >( iConfig.getParameter<InputTag> ( "pfCandidatesTag" ) ) )
    {

        muminiso_r_min_ = iConfig.getParameter<double>( "muminiso_r_min" );
        muminiso_r_max_ = iConfig.getParameter<double>( "muminiso_r_max" );
        muminiso_kt_scale_ = iConfig.getParameter<double>( "muminiso_kt_scale" );
        muminiso_deadcone_ch_ = iConfig.getParameter<double>( "muminiso_deadcone_ch" );
        muminiso_deadcone_pu_ = iConfig.getParameter<double>( "muminiso_deadcone_pu" );
        muminiso_deadcone_ph_ = iConfig.getParameter<double>( "muminiso_deadcone_ph" );
        muminiso_deadcone_nh_ = iConfig.getParameter<double>( "muminiso_deadcone_nh" );
        muminiso_ptThresh_ = iConfig.getParameter<double>( "muminiso_ptThresh" );
        muminiso_ptThresh_phot_ = iConfig.getParameter<double>( "muminiso_ptThresh_phot" );

        produces<vector<flashgg::Muon> >();
    }

    void MuonProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<pat::Muon> >  pmuons;
        evt.getByToken( muonToken_, pmuons );
        //	const PtrVector<pat::Muon> pmuonsPointers = pmuons->ptrVector();        

        Handle<View<pat::PackedCandidate> > pfcandidates;
        evt.getByToken( pfcandidateToken_, pfcandidates );
        const std::vector<edm::Ptr<pat::PackedCandidate> > &pfcands = pfcandidates->ptrs();

        //        std::cout << "calling produce function " << std::endl;

        std::unique_ptr<vector<flashgg::Muon> > muColl( new vector<flashgg::Muon> );

        for( unsigned int muIndex = 0; muIndex < pmuons->size(); muIndex++ ) {
            Ptr<pat::Muon> pmu = pmuons->ptrAt( muIndex );//retain the same index as patMuon;
            flashgg::Muon fmu = flashgg::Muon( *pmu );

            float fggPFIsoSumRelR04_ = ( pmu->pfIsolationR04().sumChargedHadronPt + max( 0., pmu->pfIsolationR04().sumNeutralHadronEt + pmu->pfIsolationR04().sumPhotonEt - 0.5 * pmu->pfIsolationR04().sumPUPt ) ) / ( pmu->pt() );
            fmu.setFggPFIsoSumRelR04( fggPFIsoSumRelR04_ );

            float fggTrkIsoSumRelR03_ = pmu->isolationR03().sumPt / pmu->pt() ;
            fmu.setFggTrkIsoSumRelR03( fggTrkIsoSumRelR03_ );

            //MiniIsolation : https://twiki.cern.ch/twiki/bin/view/CMS/MiniIsolationSUSY
            float fggMiniIsoSumRel_ = 99999.;
            double iso_nh_(0.); double iso_ch_(0.); 
            double iso_ph_(0.); double iso_pu_(0.); double iso_ph2_(0.); 
            double par_pt_ =  pmu->pt();
            if(par_pt_ > 5.){
                double r_iso_ = max(muminiso_r_min_,min(muminiso_r_max_, muminiso_kt_scale_/par_pt_ ));
                for( unsigned int ipfc = 0 ; ipfc < pfcands.size() ; ipfc++ ) {
                    Ptr<pat::PackedCandidate> pfc = pfcands[ ipfc ];
                    if (abs(pfc->pdgId())<7) continue;
                    
                    double dr_ = reco::deltaR( *pfc, *pmu );
                    //double dr_ = deltaR(pfc->eta(), pfc->phi(), pmu->eta(), pmu->phi() );
                    if (dr_ > r_iso_) continue;
                    
                    //////////////////  NEUTRALS  /////////////////////////
                    if (pfc->charge()==0){
                        if (pfc->pt()>muminiso_ptThresh_) {
                            /////////// PHOTONS ////////////
                            if (abs(pfc->pdgId())==22) {
                                if(dr_ < muminiso_deadcone_ph_) continue;
                                iso_ph_ += pfc->pt();
                                if(pfc->pt()>muminiso_ptThresh_phot_) {
                                    iso_ph2_ += pfc->pt();
                                }
                                /////////// NEUTRAL HADRONS ////////////
                            } else if (abs(pfc->pdgId())==130) {
                                if(dr_ < muminiso_deadcone_nh_) continue;
                                iso_nh_ += pfc->pt();
                            }
                        }
                        //////////////////  CHARGED from PV  /////////////////////////
                    } else if (pfc->fromPV()>1){
                        if (abs(pfc->pdgId())==211) {
                            if(dr_ < muminiso_deadcone_ch_) continue;
                            iso_ch_ += pfc->pt();
                        }
                        //////////////////  CHARGED from PU  /////////////////////////
                    } else {
                        if (pfc->pt()>muminiso_ptThresh_){
                            if(dr_ < muminiso_deadcone_pu_) continue;
                            iso_pu_ += pfc->pt();
                        }
                    }
                }// pfcand loop
                fggMiniIsoSumRel_ = ( iso_ch_ + max( 0., iso_nh_ + iso_ph_ - 0.5 * iso_pu_) ) / par_pt_ ;
            }
            fmu.setFggMiniIsoSumRel( fggMiniIsoSumRel_ );
            fmu.setFggMiniIsoCharged( iso_ch_ );
            fmu.setFggMiniIsoNeutrals( iso_nh_ );
            fmu.setFggMiniIsoPhotons( iso_ph_ );
            fmu.setFggMiniIsoPUCharged( iso_pu_ );
            fmu.setFggMiniIsoPhotons2( iso_ph2_ );

            muColl->push_back( fmu );
        }
        evt.put( std::move( muColl ) );
    }
}

typedef flashgg::MuonProducer FlashggMuonProducer;
DEFINE_FWK_MODULE( FlashggMuonProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

