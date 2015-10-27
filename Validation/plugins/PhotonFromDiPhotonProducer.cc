#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace flashgg {

    class PhotonFromDiPhotonProducer : public edm::EDProducer {
        
    public:
        PhotonFromDiPhotonProducer(const edm::ParameterSet &);
    private:
        void produce(edm::Event &, const edm::EventSetup &) override;
        edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diPhotonToken_;
        edm::EDGetTokenT<edm::View<DiPhotonMVAResult> > diPhotonMVAToken_;
        StringCutObjectSelector<flashgg::Photon> candSelector_;
        StringCutObjectSelector<flashgg::DiPhotonCandidate> leadingPreselection_;
        StringCutObjectSelector<flashgg::DiPhotonCandidate> subleadingPreselection_;
        int vtx_;
        float diphotonMVAThreshold_;
    };
    
    PhotonFromDiPhotonProducer::PhotonFromDiPhotonProducer(const edm::ParameterSet &iConfig) :
        diPhotonToken_(consumes<edm::View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<edm::InputTag> ("src"))),        
        diPhotonMVAToken_( consumes<edm::View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<edm::InputTag> ( "diPhotonMVATag" ) ) ),
        candSelector_(iConfig.getParameter<std::string>("cut")),
        leadingPreselection_(iConfig.getParameter<std::string>("leadingPreselection")),
        subleadingPreselection_(iConfig.getParameter<std::string>("subleadingPreselection")),
        vtx_(iConfig.getParameter<int>("vertexSelection")),
        diphotonMVAThreshold_(iConfig.getParameter<double>("diphotonMVAThreshold"))
    {
        produces<std::vector<flashgg::Photon> >(); 
        produces<edm::ValueMap<float> >("idmva");
        produces<edm::ValueMap<bool> >("preselection");
    }
    
    void PhotonFromDiPhotonProducer::produce(edm::Event &evt, const edm::EventSetup &) {

        edm::Handle<edm::View<flashgg::DiPhotonMVAResult> > mvaResults;
        edm::Handle<edm::View<flashgg::DiPhotonCandidate> > diPhotons;
        std::vector<float> idValues;
        std::vector<bool> preselValues;
        evt.getByToken(diPhotonToken_, diPhotons);

        std::auto_ptr<std::vector<flashgg::Photon> > photonColl(new std::vector<flashgg::Photon>);

        // MATTEO FIXME IN THE MICROAOD PRODUCER
        if (!diPhotons.failedToGet()) {
            edm::Handle<edm::View<flashgg::DiPhotonMVAResult> > mvaResults;
            evt.getByToken(diPhotonMVAToken_, mvaResults);

            for(unsigned int i = 0 ; i<diPhotons->size(); i++) {
                edm::Ptr<flashgg::DiPhotonCandidate> diphoton = diPhotons->ptrAt(i);
                edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( i );
                if( mvares->result < diphotonMVAThreshold_ ) { continue; } 
                
                if ((vtx_ == -1) || (diphoton->vertexIndex() == vtx_)) {
                    const flashgg::SinglePhotonView leading = *(diphoton->leadingView());
                    const flashgg::SinglePhotonView subLeading = *(diphoton->subLeadingView());
                    if (candSelector_(*(leading.photon())) && candSelector_(*(subLeading.photon()))) {
                        preselValues.push_back(leadingPreselection_(*diphoton));
                        preselValues.push_back(subleadingPreselection_(*diphoton));
                        
                        idValues.push_back(leading.phoIdMvaWrtChosenVtx());
                        idValues.push_back(subLeading.phoIdMvaWrtChosenVtx());
                        
                        photonColl->push_back(*(leading.photon()));
                        photonColl->push_back(*(subLeading.photon()));
                    }
                }
            }
        }
        
        edm::OrphanHandle<std::vector<flashgg::Photon> > photonCollH = evt.put(photonColl); 

        std::auto_ptr<edm::ValueMap<float> > idValMap(new edm::ValueMap<float>());
        edm::ValueMap<float>::Filler idFiller(*idValMap);
        idFiller.insert(photonCollH, idValues.begin(), idValues.end());
        idFiller.fill();
        evt.put(idValMap, "idmva");

        std::auto_ptr<edm::ValueMap<bool> > preselValMap(new edm::ValueMap<bool>());
        edm::ValueMap<bool>::Filler preselFiller(*preselValMap);
        preselFiller.insert(photonCollH, preselValues.begin(), preselValues.end());
        preselFiller.fill();
        evt.put(preselValMap, "preselection");        
    }
}

typedef flashgg::PhotonFromDiPhotonProducer FlashggPhotonFromDiPhotonProducer;
DEFINE_FWK_MODULE(FlashggPhotonFromDiPhotonProducer);
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

