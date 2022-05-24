#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Jet.h"

#include "TMVA/Reader.h"
#include "TMath.h"
#include "TH2.h"
#include "TF1.h"
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

        double getPrefiringRateEcal(double eta, double pt, TH2F* h_prefmap, std::string fluctuation = "") const;
        double getPrefiringRateMuon(double eta, double phi, double pt, std::string fluctuation = "") const;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

        bool isECALRelevant_;
        bool applyToCentral_;

        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        EDGetTokenT<View<flashgg::DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<flashgg::Muon> > MuonToken_;
        std::vector<edm::InputTag> inputTagJets_;

        edm::FileInPath ECALFileName_;
        edm::FileInPath MuonFileName_;
        std::string dataeraEcal_;
        std::string dataeraMuon_;

        std::unique_ptr<TFile> file_prefiringmaps_;
        std::unique_ptr<TFile> file_prefiringparams_;

        TH2F* h_prefmap_photon_;
        TH2F* h_prefmap_jet_;

        TF1* parametrization0p0To0p2_;
        TF1* parametrization0p2To0p3_;
        TF1* parametrization0p3To0p55_;
        TF1* parametrization0p55To0p83_;
        TF1* parametrization0p83To1p24_;
        TF1* parametrization1p24To1p4_;
        TF1* parametrization1p4To1p6_;
        TF1* parametrization1p6To1p8_;
        TF1* parametrization1p8To2p1_;
        TF1* parametrization2p1To2p25_;
        TF1* parametrization2p25To2p4_;
        TF1* parametrizationHotSpot_;

    };

    PrefireDiPhotonProducer::PrefireDiPhotonProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        MuonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag> ( "MuonTag" ) ) ),
        inputTagJets_ ( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) )
    {
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
        produces<std::vector<flashgg::DiPhotonCandidate> >();

        isECALRelevant_ = iConfig.getParameter<bool>("isECALRelevant"); // used to skip all ECAL L1 prefire for 2018
        applyToCentral_ = iConfig.getParameter<bool>("applyToCentral");

        ECALFileName_ = iConfig.getParameter<edm::FileInPath>( "ECALFileName" );
        dataeraEcal_  = iConfig.getParameter<std::string>( "dataeraEcal" );

        if (isECALRelevant_) {
            file_prefiringmaps_ = std::make_unique<TFile>(ECALFileName_.fullPath().c_str(), "read");
            h_prefmap_photon_ = (TH2F*)file_prefiringmaps_->Get( ("L1prefiring_photonptvseta_" + dataeraEcal_).c_str() );
            h_prefmap_jet_    = (TH2F*)file_prefiringmaps_->Get( ("L1prefiring_jetptvseta_" + dataeraEcal_   ).c_str() );
            file_prefiringmaps_->Close();
        }

        MuonFileName_ = iConfig.getParameter<edm::FileInPath>( "MuonFileName" );
        dataeraMuon_  = iConfig.getParameter<std::string>( "dataeraMuon" );

        file_prefiringparams_ = std::make_unique<TFile>(MuonFileName_.fullPath().c_str(), "read");
        parametrization0p0To0p2_   = (TF1*)file_prefiringparams_->Get( ("L1prefiring_muonparam_0.0To0.2_"   + dataeraMuon_).c_str() );
        parametrization0p2To0p3_   = (TF1*)file_prefiringparams_->Get( ("L1prefiring_muonparam_0.2To0.3_"   + dataeraMuon_).c_str() );
        parametrization0p3To0p55_  = (TF1*)file_prefiringparams_->Get( ("L1prefiring_muonparam_0.3To0.55_"  + dataeraMuon_).c_str() );
        parametrization0p55To0p83_ = (TF1*)file_prefiringparams_->Get( ("L1prefiring_muonparam_0.55To0.83_" + dataeraMuon_).c_str() );
        parametrization0p83To1p24_ = (TF1*)file_prefiringparams_->Get( ("L1prefiring_muonparam_0.83To1.24_" + dataeraMuon_).c_str() );
        parametrization1p24To1p4_  = (TF1*)file_prefiringparams_->Get( ("L1prefiring_muonparam_1.24To1.4_"  + dataeraMuon_).c_str() );
        parametrization1p4To1p6_   = (TF1*)file_prefiringparams_->Get( ("L1prefiring_muonparam_1.4To1.6_"   + dataeraMuon_).c_str() );
        parametrization1p6To1p8_   = (TF1*)file_prefiringparams_->Get( ("L1prefiring_muonparam_1.6To1.8_"   + dataeraMuon_).c_str() );
        parametrization1p8To2p1_   = (TF1*)file_prefiringparams_->Get( ("L1prefiring_muonparam_1.8To2.1_"   + dataeraMuon_).c_str() );
        parametrization2p1To2p25_  = (TF1*)file_prefiringparams_->Get( ("L1prefiring_muonparam_2.1To2.25_"  + dataeraMuon_).c_str() );
        parametrization2p25To2p4_  = (TF1*)file_prefiringparams_->Get( ("L1prefiring_muonparam_2.25To2.4_"  + dataeraMuon_).c_str() );
        parametrizationHotSpot_    = (TF1*)file_prefiringparams_->Get( ("L1prefiring_muonparam_HotSpot_"    + dataeraMuon_).c_str() );
        file_prefiringparams_->Close();

    }

    double PrefireDiPhotonProducer::getPrefiringRateEcal(double eta,
                                                           double pt,
                                                           TH2F* h_prefmap,
                                                           std::string fluctuation) const {
        //Check pt is not above map overflow
        int nbinsy = h_prefmap->GetNbinsY();
        double maxy = h_prefmap->GetYaxis()->GetBinLowEdge(nbinsy + 1);
        if (pt >= maxy) pt = maxy - 0.01;
        int thebin = h_prefmap->FindBin(eta, pt);
    
        double prefrate = h_prefmap->GetBinContent(thebin);
    
        double statuncty = h_prefmap->GetBinError(thebin);
        double systuncty = 0.2 * prefrate; // 20% systematic unc.
    
        if (fluctuation == "up")
            prefrate = std::min(1., prefrate + sqrt(pow(statuncty, 2) + pow(systuncty, 2)));
        else if (fluctuation == "down")
            prefrate = std::max(0., prefrate - sqrt(pow(statuncty, 2) + pow(systuncty, 2)));
        if (prefrate > 1.) {
            edm::LogWarning("L1PrefireWeightProducer") << "Found a prefiring probability > 1. Setting to 1." << std::endl;
            return 1.;
        }
        return prefrate;
    }

    double PrefireDiPhotonProducer::getPrefiringRateMuon(double eta,
                                                           double phi,
                                                           double pt,
                                                           std::string fluctuation) const {
        double prefrate;
        double statuncty;
        if ((dataeraMuon_.find("2016") != std::string::npos) && (eta > 1.24 && eta < 1.6) && (phi > 2.44346 && phi < 2.79253)) {
            prefrate = parametrizationHotSpot_->Eval(pt);
            statuncty = parametrizationHotSpot_->GetParError(2);
        } else if (std::abs(eta) < 0.2) {
            prefrate = parametrization0p0To0p2_->Eval(pt);
            statuncty = parametrization0p0To0p2_->GetParError(2);
        } else if (std::abs(eta) < 0.3) {
            prefrate = parametrization0p2To0p3_->Eval(pt);
            statuncty = parametrization0p2To0p3_->GetParError(2);
        } else if (std::abs(eta) < 0.55) {
            prefrate = parametrization0p3To0p55_->Eval(pt);
            statuncty = parametrization0p3To0p55_->GetParError(2);
        } else if (std::abs(eta) < 0.83) {
            prefrate = parametrization0p55To0p83_->Eval(pt);
            statuncty = parametrization0p55To0p83_->GetParError(2);
        } else if (std::abs(eta) < 1.24) {
            prefrate = parametrization0p83To1p24_->Eval(pt);
            statuncty = parametrization0p83To1p24_->GetParError(2);
        } else if (std::abs(eta) < 1.4) {
            prefrate = parametrization1p24To1p4_->Eval(pt);
            statuncty = parametrization1p24To1p4_->GetParError(2);
        } else if (std::abs(eta) < 1.6) {
            prefrate = parametrization1p4To1p6_->Eval(pt);
            statuncty = parametrization1p4To1p6_->GetParError(2);
        } else if (std::abs(eta) < 1.8) {
            prefrate = parametrization1p6To1p8_->Eval(pt);
            statuncty = parametrization1p6To1p8_->GetParError(2);
        } else if (std::abs(eta) < 2.1) {
            prefrate = parametrization1p8To2p1_->Eval(pt);
            statuncty = parametrization1p8To2p1_->GetParError(2);
        } else if (std::abs(eta) < 2.25) {
            prefrate = parametrization2p1To2p25_->Eval(pt);
            statuncty = parametrization2p1To2p25_->GetParError(2);
        } else if (std::abs(eta) < 2.4) {
            prefrate = parametrization2p25To2p4_->Eval(pt);
            statuncty = parametrization2p25To2p4_->GetParError(2);
        } else {
            LogDebug("L1PrefireWeightProducer") << "Muon outside of |eta| <= 2.4. Prefiring weight set to 0." << std::endl;
            return 0.;
        }
        double systuncty = 0.2 * prefrate; // 20% systematic unc.
    
        if (fluctuation == "up")
            prefrate = std::min(1., prefrate + sqrt(pow(statuncty, 2) + pow(systuncty, 2)));
        else if (fluctuation == "down")
            prefrate = std::max(0., prefrate - sqrt(pow(statuncty, 2) + pow(systuncty, 2)));
 
        if (prefrate > 1.) {
            edm::LogWarning("L1PrefireWeightProducer") << "Found a prefiring probability > 1. Setting to 1." << std::endl;
            return 1.;
        }
        return prefrate;
    }

    void PrefireDiPhotonProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        Handle<View<flashgg::Muon> > Muons;
        evt.getByToken( MuonToken_, Muons );

        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByToken( tokenJets_[j], Jets[j] );
        }

        unique_ptr<std::vector<flashgg::DiPhotonCandidate> > updatedDiphotons( new std::vector<flashgg::DiPhotonCandidate>() );
        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt(candIndex);

            // calculate the non-prefire probability as
            // Product over photons, jets and muons ( 1 - p(photon,jet,muon prefires) ) with proper overlap removal.
            // Code reference : https://github.com/cms-sw/cmssw/blob/CMSSW_10_6_X/PhysicsTools/PatUtils/plugins/L1PrefiringWeightProducer.cc

            double nonprefireProd = 1.;
            double nonprefireProdUncUp   = 1.;
            double nonprefireProdUncDown = 1.;

            //Calculate prefiring weights for muons
            double nonPrefireProdMuon = 1.;
            double nonPrefireProdMuonUp = 1.;
            double nonPrefireProdMuonDown = 1.;
            std::vector<edm::Ptr<flashgg::Muon>> selectedMuons;
            for ( const auto& muon : Muons->ptrs() ) {
                double pt = muon->pt();
                double phi = muon->phi();
                double eta = muon->eta();
                // Remove crappy tracker muons which would not have prefired the L1 trigger
                if (pt < 5 || !muon->isLooseMuon()) continue;
                nonPrefireProdMuon     *= (1. - getPrefiringRateMuon(eta, phi, pt));
                nonPrefireProdMuonUp   *= (1. - getPrefiringRateMuon(eta, phi, pt, "up"));
                nonPrefireProdMuonDown *= (1. - getPrefiringRateMuon(eta, phi, pt, "down"));

                selectedMuons.push_back( muon );
            }

            //Calculate prefiring weights for ECAL (photons, jets)
            double nonPrefireProdECAL = 1.;
            double nonPrefireProdECALUp = 1.;
            double nonPrefireProdECALDown = 1.;
            if (isECALRelevant_) {

                double pho1_pt  = dipho->leadingPhoton()->pt();
                double pho1_eta = dipho->leadingPhoton()->eta();
                double pho1_phi = dipho->leadingPhoton()->phi();
                double pho2_pt  = dipho->subLeadingPhoton()->pt();
                double pho2_eta = dipho->subLeadingPhoton()->eta();
                double pho2_phi = dipho->subLeadingPhoton()->phi();

                bool is_pho1_prefire = false;
                bool is_pho2_prefire = false;
                if (pho1_pt > 20. && fabs(pho1_eta) > 2. && fabs(pho1_eta) < 3.) {
                    nonPrefireProdECAL     *= (1. - getPrefiringRateEcal(pho1_eta, pho1_pt, h_prefmap_photon_));
                    nonPrefireProdECALUp   *= (1. - getPrefiringRateEcal(pho1_eta, pho1_pt, h_prefmap_photon_, "up"));
                    nonPrefireProdECALDown *= (1. - getPrefiringRateEcal(pho1_eta, pho1_pt, h_prefmap_photon_, "down"));
                    is_pho1_prefire = true;
                }
                if (pho2_pt > 20. && fabs(pho2_eta) > 2. && fabs(pho2_eta) < 3.) {
                    nonPrefireProdECAL     *= (1. - getPrefiringRateEcal(pho2_eta, pho2_pt, h_prefmap_photon_));
                    nonPrefireProdECALUp   *= (1. - getPrefiringRateEcal(pho2_eta, pho2_pt, h_prefmap_photon_, "up"));
                    nonPrefireProdECALDown *= (1. - getPrefiringRateEcal(pho2_eta, pho2_pt, h_prefmap_photon_, "down"));
                    is_pho2_prefire = true;
                }

                unsigned int jetCollectionIndex = diPhotons->ptrAt( candIndex )->jetCollectionIndex();
                for( UInt_t jetLoop = 0; jetLoop < Jets[jetCollectionIndex]->size() ; jetLoop++ ) {
                    Ptr<flashgg::Jet> jet  = Jets[jetCollectionIndex]->ptrAt( jetLoop );

                    if (jet->pt() < 20. || fabs(jet->eta()) > 3. || fabs(jet->eta()) < 2.) continue;

                    //Remove jets overlapping with two photons and all muons
                    bool mu_jet_overlop = false;
                    for ( const auto& muon : selectedMuons ) {
                        if ( deltaR( muon->eta(), muon->phi(), jet->eta(), jet->phi() ) < 0.4 ) {
                            mu_jet_overlop = true;
                            break;
                        }
                    }
                    if (mu_jet_overlop) continue;

                    if ( deltaR( pho1_eta, pho1_phi, jet->eta(), jet->phi() ) < 0.4 && is_pho1_prefire ) continue;
                    if ( deltaR( pho2_eta, pho2_phi, jet->eta(), jet->phi() ) < 0.4 && is_pho2_prefire ) continue;

                    nonPrefireProdECAL     *= (1. - getPrefiringRateEcal(jet->eta(), jet->pt(), h_prefmap_jet_));
                    nonPrefireProdECALUp   *= (1. - getPrefiringRateEcal(jet->eta(), jet->pt(), h_prefmap_jet_, "up"));
                    nonPrefireProdECALDown *= (1. - getPrefiringRateEcal(jet->eta(), jet->pt(), h_prefmap_jet_, "down"));

                }
            }

            nonprefireProd        = nonPrefireProdMuon * nonPrefireProdECAL;
            nonprefireProdUncUp   = nonPrefireProdMuonUp * nonPrefireProdECALUp;
            nonprefireProdUncDown = nonPrefireProdMuonDown * nonPrefireProdECALDown;

            flashgg::DiPhotonCandidate *updatedDipho = dipho->clone();
            WeightedObject prefireObject;
            if (applyToCentral_) {
                prefireObject.setCentralWeight( nonprefireProd );
                prefireObject.setWeight("prefireWeightUp01sigma",   nonprefireProdUncUp);
                prefireObject.setWeight("prefireWeightDown01sigma", nonprefireProdUncDown);
                updatedDipho->includeWeights(prefireObject);
            }
            else {
                prefireObject.setWeight("prefireWeight", nonprefireProd);
                prefireObject.setWeight("prefireWeightUp01sigma",   nonprefireProdUncUp);
                prefireObject.setWeight("prefireWeightDown01sigma", nonprefireProdUncDown);
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

