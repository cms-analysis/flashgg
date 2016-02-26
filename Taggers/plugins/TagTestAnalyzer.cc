// TagTestAnalyzer.cc by S. Zenz
//
// * Tests getting tags out of the event, sorting them, casting them
// * Dumps debugging output to the screen
// * Useful for quick tests of code changes, and should be kept up-to-date as tags are added/changed
// * Should NOT be included in productions
//
// Adapted from globelikeTreeMakerWithTagSorter code by L. D. Corpe, which was
// Adapted from the flashggCommissioning tree maker code  by C. Favaro et al.

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"

#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/UntaggedTag.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/TTHHadronicTag.h"
#include "flashgg/DataFormats/interface/TTHLeptonicTag.h"
#include "flashgg/DataFormats/interface/VHTightTag.h"
#include "flashgg/DataFormats/interface/VHEtTag.h"
#include "flashgg/DataFormats/interface/VHLooseTag.h"
#include "flashgg/DataFormats/interface/VHHadronicTag.h"
#include "flashgg/DataFormats/interface/VBFTagTruth.h"
#include "flashgg/DataFormats/interface/ZPlusJetTag.h"

using namespace std;
using namespace edm;

// **********************************************************************

namespace flashgg {

    class TagTestAnalyzer : public edm::EDAnalyzer
    {
    public:
        explicit TagTestAnalyzer( const edm::ParameterSet & );
        ~TagTestAnalyzer();

        static void fillDescriptions( edm::ConfigurationDescriptions &descriptions );


    private:

        edm::Service<TFileService> fs_;

        virtual void beginJob() override;
        virtual void analyze( const edm::Event &, const edm::EventSetup & ) override;
        virtual void endJob() override;

        edm::EDGetTokenT<edm::View<flashgg::DiPhotonTagBase> > TagSorterToken_;
        bool expectMultiples_;
    };

// ******************************************************************************************
// ******************************************************************************************

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
    TagTestAnalyzer::TagTestAnalyzer( const edm::ParameterSet &iConfig ):
        TagSorterToken_( consumes<edm::View<flashgg::DiPhotonTagBase> >( iConfig.getParameter<InputTag> ( "TagSorter" ) ) ),
        expectMultiples_( iConfig.getUntrackedParameter<bool>( "ExpectMultiples", false) )
    {
    }

    TagTestAnalyzer::~TagTestAnalyzer()
    {

    }

    void
    TagTestAnalyzer::analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup )
    {

        // ********************************************************************************
        // access edm objects

        Handle<edm::View<flashgg::DiPhotonTagBase> > TagSorter;
        iEvent.getByToken( TagSorterToken_, TagSorter );

        if (!expectMultiples_) {
            assert (TagSorter.product()->size() <= 1);
            if ( TagSorter.product()->size() == 0) std::cout << "[NO TAG]" << std::endl;
        } else {
            std::cout << "Multiple tags allowed and we have a total of " << TagSorter.product()->size() << std::endl;
        }

        for ( auto tag = TagSorter.product()->begin() ; tag != TagSorter.product()->end() ; tag++ ) {
            const flashgg::DiPhotonTagBase *chosenTag = &*( tag );

            const ZPlusJetTag *zplusjet = dynamic_cast<const ZPlusJetTag *>( chosenTag );
            if( zplusjet != NULL ) {
                std::cout << "[ZPLUSJET] njets=" << zplusjet->nJets() << " jet pt=" << zplusjet->jet()->pt() << " dipho mass=" << zplusjet->diPhoton()->mass() << std::endl;
            }

            const	UntaggedTag *untagged = dynamic_cast<const UntaggedTag *>( chosenTag );
            if( untagged != NULL ) {
                std::cout << "[UNTAGGED] category " << untagged->categoryNumber() << " mass=" << untagged->diPhoton()->mass() <<
                          ", systLabel " << untagged->systLabel() <<  std::endl;
                if( untagged->tagTruth().isNonnull() ) {
                    std::cout << "\t[UNTAGGED TRUTH]: genPV=" << untagged->diPhoton()->genPV() << std::endl;
                }
            }

            const	VBFTag *vbftag = dynamic_cast<const VBFTag *>( chosenTag );
            if( vbftag != NULL ) {
                std::cout << "[VBF] Category " << vbftag->categoryNumber() << " with lead jet pt eta "
                          << vbftag->leadingJet().pt() << " " << vbftag->leadingJet().eta()
                          << " and sublead jet eta " << vbftag->subLeadingJet().pt() << " " << vbftag->subLeadingJet().eta() << " mass=" << vbftag->diPhoton()->mass()
                          << ", systLabel " << vbftag->systLabel() << std::endl;
                std::cout << "    VBFDiPhoDiJetMVA=" << vbftag->VBFDiPhoDiJetMVA().VBFDiPhoDiJetMVAValue()
                          << " VBFMVA=" << vbftag->VBFMVA().VBFMVAValue()
                          << std::endl;

                if( vbftag->tagTruth().isNonnull() ) {
                    const VBFTagTruth *truth = dynamic_cast<const VBFTagTruth *>( &*vbftag->tagTruth() );
                    assert( truth != NULL );  // If we stored a VBFTag with a nonnull pointer, we either have VBFTagTruth or a nutty bug
                    std::cout << "\t[VBF TRUTH]: genPV=" << truth->genPV() << std::endl;
                    std::cout << "\t[VBF DIPHOTON]: genPV=" << vbftag->diPhoton()->genPV() << std::endl;
                    std::cout << "\t\t------------------------------------------" << std::endl;
                    if( truth->closestGenJetToLeadingJet().isNonnull() ) {
                        std::cout << "\t\tclosestGenJetToLeadingJet pt eta " << truth->closestGenJetToLeadingJet()->pt() << " " << truth->closestGenJetToLeadingJet()->eta() <<
                                  std::endl;
                    }
                    if( truth->closestParticleToLeadingJet().isNonnull() ) {
                        std::cout << "\t\tclosestParticleToLeadingJet pt eta id "   << truth->closestParticleToLeadingJet()->pt() << " " << truth->closestParticleToLeadingJet()->eta()
                                  << " " << truth->closestParticleToLeadingJet()->pdgId() << std::endl;
                    }
                    std::cout << "\t\t------------------------------------------" << std::endl;
                    if( truth->closestGenJetToSubLeadingJet().isNonnull() ) {
                        std::cout << "\t\tclosestGenJetToSubLeadingJet pt eta id " << truth->closestGenJetToSubLeadingJet()->pt() << " " << truth->closestGenJetToSubLeadingJet()->eta()
                                  << std::endl;
                    }
                    if( truth->closestParticleToSubLeadingJet().isNonnull() ) {
                        std::cout << "\t\tclosestParticleToSubLeadingJet pt eta " << truth->closestParticleToSubLeadingJet()->pt() << " " <<
                                  truth->closestParticleToSubLeadingJet()->eta()
                                  << " " << truth->closestParticleToSubLeadingJet()->pdgId() << std::endl;
                    }
                    std::cout << "\t\t------------------------------------------" << std::endl;
                    if( truth->closestParticleToLeadingPhoton().isNonnull() ) {
                        std::cout << "\t\tclosestParticleToLeadingPhoton pt eta id " << truth->closestParticleToLeadingPhoton()->pt() << " " <<
                                  truth->closestParticleToLeadingPhoton()->eta()
                                  << " " << truth->closestParticleToLeadingPhoton()->pdgId() << std::endl;
                    }
                    std::cout << "\t\t------------------------------------------" << std::endl;
                    if( truth->closestParticleToSubLeadingPhoton().isNonnull() ) {
                        std::cout << "\t\tclosestParticleToSubLeadingPhoton pt eta id " << truth->closestParticleToSubLeadingPhoton()->pt() << " " <<
                                  truth->closestParticleToSubLeadingPhoton()->eta()
                                  << " " << truth->closestParticleToSubLeadingPhoton()->pdgId() << std::endl;
                    }
                    std::cout << "\t\t------------------------------------------" << std::endl;
                    if( truth->leadingParton().isNonnull() ) {
                        std::cout << "\t\tleadingParton pt eta id " << truth->leadingParton()->pt() << " " << truth->leadingParton()->eta()
                                  << " " << truth->leadingParton()->pdgId() << std::endl;
                    }
                    if( truth->subLeadingParton().isNonnull() ) {
                        std::cout << "\t\tsubLeadingQuark pt eta id "  << truth->subLeadingParton()->pt() << " " << truth->subLeadingParton()->eta()
                                  << " " << truth->subLeadingParton()->pdgId() << std::endl;
                    }
                    if( truth->leadingParton().isNonnull() && truth->subLeadingParton().isNonnull() ) {
                        std::cout << "\t\tDiquark mass: " << ( truth->leadingParton()->p4() + truth->subLeadingParton()->p4() ).mass() << std::endl;
                    }
                }

            }

            const   TTHHadronicTag *tthhadronictag = dynamic_cast<const TTHHadronicTag *>( chosenTag );
            if( tthhadronictag != NULL ) {
                std::cout << "[TTHhadronic] Category " << tthhadronictag->categoryNumber()
                          << " with NJet=" << tthhadronictag->jetVector().size()
                          << " and NBLoose= " << tthhadronictag->nBLoose()
                          << " and NBMedium= " << tthhadronictag->nBMedium()
                          << std::endl;
            }

            const   TTHLeptonicTag *tthleptonictag = dynamic_cast<const TTHLeptonicTag *>( chosenTag );
            if( tthleptonictag != NULL ) {
                std::cout << "[TTHleptonic] Category " << tthleptonictag->categoryNumber()
                          << " nelectrons=" << tthleptonictag->electrons().size()
                          << " nmuons=" << tthleptonictag->muons().size()
                          << std::endl;
            }

            const   VHTightTag *vhtighttag = dynamic_cast<const VHTightTag *>( chosenTag );
            if( vhtighttag != NULL ) {
                std::cout << "[VHtight] Category " << vhtighttag->categoryNumber()
                          << " nmuons=" << vhtighttag->muons().size()
                          << std::endl;
            }

            const   VHLooseTag *vhloosetag = dynamic_cast<const VHLooseTag *>( chosenTag );
            if( vhloosetag != NULL ) {
                std::cout << "[VHloose] Category " << vhloosetag->categoryNumber()
                          << " nmuons=" << vhloosetag->muons().size()
                          << " nelectrons=" << vhloosetag->electrons().size()
                          << " systLabel " << vhloosetag->systLabel()
                          << std::endl;
            }

            const   VHHadronicTag *vhhadronictag = dynamic_cast<const VHHadronicTag *>( chosenTag );
            if( vhhadronictag != NULL ) {
                std::cout << "[VHhadronic] Category "    << vhhadronictag->categoryNumber()
                          << " with leadingJet    pt = " << vhhadronictag->leadingJet()->pt()
                          << " and  subleadingJet pt = " << vhhadronictag->subLeadingJet()->pt()
                          << std::endl;
            }
            const    VHEtTag *vhettag = dynamic_cast<const VHEtTag *>( chosenTag );
            if( vhettag != NULL ) {
                std::cout << "[VHEt] Category "      << vhettag->categoryNumber()
                          //<< " with MEt        = "   << vhettag->met()
                          << std::endl;
            }

            // IMPORTANT: All future Tags must be added in the way of untagged and vbftag.

            if( untagged == NULL && vbftag == NULL && tthhadronictag == NULL && tthleptonictag == NULL && vhtighttag == NULL && vhloosetag == NULL &&
                    vhhadronictag == NULL && vhettag == NULL && zplusjet==NULL ) {
                std::cout << "[FAILED TO CONVERT TAG] with SumPt " << chosenTag->sumPt() << std::endl;
            }

        } 
    } // analyze

    void
    TagTestAnalyzer::beginJob()
    {
    }

    void
    TagTestAnalyzer::endJob()
    {
    }

    void
    TagTestAnalyzer::fillDescriptions( edm::ConfigurationDescriptions &descriptions )
    {
        //The following says we do not know what parameters are allowed so do no validation
        // Please change this to state exactly what you do use, even if it is no parameters
        edm::ParameterSetDescription desc;
        desc.setUnknown();
        descriptions.addDefault( desc );
    }

} // namespace flashgg

typedef flashgg::TagTestAnalyzer FlashggTagTestAnalyzer;
DEFINE_FWK_MODULE( FlashggTagTestAnalyzer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

