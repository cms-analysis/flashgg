#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/TagFormats/interface/VBFMVAResult.h"
#include "flashgg/TagFormats/interface/VBFTag.h"

#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

  class VBFTagProducer : public EDProducer {

  public:
    VBFTagProducer( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;
    int chooseCategory(float);

    EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
    EDGetTokenT<View<VBFDiPhoDiJetMVAResult> > vbfDiPhoDiJetMvaResultToken_;
    EDGetTokenT<View<VBFMVAResult> > vbfMvaResultToken_;
    EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;

    vector<double> boundaries;

  };

  VBFTagProducer::VBFTagProducer(const ParameterSet & iConfig) :
    diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons")))),
    vbfDiPhoDiJetMvaResultToken_(consumes<View<flashgg::VBFDiPhoDiJetMVAResult> >(iConfig.getUntrackedParameter<InputTag> ("VBFDiPhoDiJetMVAResultTag", InputTag("flashggVBFDiPhoDiJetMVA")))),
    //    vbfMvaResultToken_(consumes<View<flashgg::VBFMVAResult> >(iConfig.getUntrackedParameter<InputTag> ("VBFMVAResultTag", InputTag("flashggVBFMVA")))),
    mvaResultToken_(consumes<View<flashgg::DiPhotonMVAResult> >(iConfig.getUntrackedParameter<InputTag> ("MVAResultTag", InputTag("flashggDiPhotonMVA"))))

  {
    vector<double> default_boundaries;
    default_boundaries.push_back(0.52);
    default_boundaries.push_back(0.85);
    default_boundaries.push_back(0.915);
    default_boundaries.push_back(1);// from here
		//https://github.com/h2gglobe/h2gglobe/blob/master/AnalysisScripts/massfac_mva_binned/massfactorizedmvaanalysis.dat#L32

    // getUntrackedParameter<vector<float> > has no library, so we use double transiently
    boundaries = iConfig.getUntrackedParameter<vector<double > >("Boundaries",default_boundaries);

    assert(is_sorted(boundaries.begin(),boundaries.end())); // we are counting on ascending order - update this to give an error message or exception

    produces<vector<VBFTag> >();
  }
  
  int VBFTagProducer::chooseCategory(float mvavalue) {
    // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
    int n;
    for (n = 0 ; n < (int)boundaries.size() ; n++) {
      if ((double)mvavalue > boundaries[boundaries.size()-n-1]) return n;
    }
    return -1; // Does not pass, object will not be produced
  }

  void VBFTagProducer::produce( Event & evt, const EventSetup & ) {

    Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
    evt.getByToken(diPhotonToken_,diPhotons);
    const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

    Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
    evt.getByToken(mvaResultToken_,mvaResults);
    const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();

    Handle<View<flashgg::VBFDiPhoDiJetMVAResult> > vbfDiPhoDiJetMvaResults;
    evt.getByToken(vbfDiPhoDiJetMvaResultToken_,vbfDiPhoDiJetMvaResults);
    const PtrVector<flashgg::VBFDiPhoDiJetMVAResult>& vbfDiPhoDiJetMvaResultPointers = vbfDiPhoDiJetMvaResults->ptrVector();
    
    //		Handle<View<flashgg::VBFMVAResult> > vbfMvaResults;
    //    evt.getByToken(vbfMvaResultToken_,vbfMvaResults);
    //    const PtrVector<flashgg::VBFMVAResult>& vbfMvaResultPointers = vbfMvaResults->ptrVector();


    std::auto_ptr<vector<VBFTag> > tags(new vector<VBFTag>); 

    assert(diPhotonPointers.size() == vbfDiPhoDiJetMvaResultPointers.size()); // We are relying on corresponding sets - update this to give an error/exception
    //    assert(diPhotonPointers.size() == vbfMvaResultPointers.size()); // We are relying on corresponding sets - update this to give an error/exception
    assert(diPhotonPointers.size() == mvaResultPointers.size()); // We are relying on corresponding sets - update this to give an error/exception                                  

    for (unsigned int candIndex =0; candIndex < diPhotonPointers.size() ; candIndex++) {
      edm::Ptr<flashgg::VBFDiPhoDiJetMVAResult> vbfdipho_mvares = vbfDiPhoDiJetMvaResultPointers[candIndex];
      //      edm::Ptr<flashgg::VBFMVAResult> vbf_mvares = vbfMvaResultPointers[candIndex];
      edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResultPointers[candIndex];
     
      edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotonPointers[candIndex];

      VBFTag tag_obj(dipho,mvares,vbfdipho_mvares);
      tag_obj.setDiPhotonIndex(candIndex);
      
      int catnum = chooseCategory(vbfdipho_mvares->vbfDiPhoDiJetMvaResult);
      tag_obj.setCategoryNumber(catnum);
      
      // Leave in debugging statement temporarily while tag framework is being developed
      //std::cout << "[VBF] MVA is "<< mvares->vbfDiPhoDiJetMvaResult << " and VBF category is " << tag_obj.getCategoryNumber() << std::endl;

      if (tag_obj.getCategoryNumber() >= 0) {
	tags->push_back(tag_obj);
      }
    }
    evt.put(tags);
  }
}

typedef flashgg::VBFTagProducer FlashggVBFTagProducer;
DEFINE_FWK_MODULE(FlashggVBFTagProducer);
