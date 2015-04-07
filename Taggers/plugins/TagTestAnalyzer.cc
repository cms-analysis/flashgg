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
#include "flashgg/DataFormats/interface/DiPhotonUntaggedCategory.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/TTHHadronicTag.h"
#include "flashgg/DataFormats/interface/TTHLeptonicTag.h"
#include "flashgg/DataFormats/interface/VHTightTag.h"
#include "flashgg/DataFormats/interface/VHLooseTag.h"
#include "flashgg/DataFormats/interface/VHHadronicTag.h"

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

        edm::EDGetTokenT<edm::OwnVector<flashgg::DiPhotonTagBase> > TagSorterToken_;
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
        TagSorterToken_( consumes<edm::OwnVector<flashgg::DiPhotonTagBase> >( iConfig.getUntrackedParameter<InputTag> ( "TagSorter",
                         InputTag( "flashggTagSorter" ) ) ) )
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

        Handle<edm::OwnVector<flashgg::DiPhotonTagBase> > TagSorter;
        iEvent.getByToken( TagSorterToken_, TagSorter );

        if( TagSorter.product()->size() > 0 ) {
            const flashgg::DiPhotonTagBase *chosenTag = &*( TagSorter.product()->begin() );

            const	DiPhotonUntaggedCategory *untagged = dynamic_cast<const DiPhotonUntaggedCategory *>( chosenTag );
            if( untagged != NULL ) {
                std::cout << "[UNTAGGED] category " << untagged->categoryNumber() << std::endl;
            }

            const	VBFTag *vbftag = dynamic_cast<const VBFTag *>( chosenTag );
            if( vbftag != NULL ) {
                std::cout << "[VBF] Category " << vbftag->categoryNumber() << " with lead jet eta "
                          << vbftag->leadingJet().pt() << " and sublead jet eta " << vbftag->subLeadingJet().pt() << std::endl;
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
                          << std::endl;
            }

            const   VHHadronicTag *vhhadronictag = dynamic_cast<const VHHadronicTag *>( chosenTag );
            if( vhhadronictag != NULL ) {
                std::cout << "[VHhadronic] Category "    << vhhadronictag->categoryNumber()
                          << " with leadingJet    pt = " << vhhadronictag->leadingJet()->pt()
                          << " and  subleadingJet pt = " << vhhadronictag->subLeadingJet()->pt()
                          << std::endl;
            }

            // IMPORTANT: All future Tags must be added in the way of untagged and vbftag.

            if( untagged == NULL && vbftag == NULL && tthhadronictag == NULL && tthleptonictag == NULL && vhtighttag == NULL && vhloosetag == NULL &&
                    vhhadronictag == NULL ) {
                std::cout << "[FAILED TO CONVERT TAG] with SumPt " << chosenTag->sumPt() << std::endl;
            }

        } else { //case where TagSorter[0] doesn't exist
            std::cout << "[NO TAG]" << std::endl;
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

