#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "flashgg/TagFormats/interface/VBFMVAResult.h"
#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "flashgg/TagFormats/interface/DiPhotonUntaggedCategory.h"
#include "flashgg/TagFormats/interface/VBFTag.h"

#include "TMVA/Reader.h"
#include "TMath.h"
//#include <typeinfo>

#include <algorithm>


using namespace std;
using namespace edm;

namespace flashgg {

	class TagSorter : public EDProducer {

		public:
			TagSorter( const ParameterSet & );
		private:
			void produce( Event &, const EventSetup & ) override;

			EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
			std::vector<edm::EDGetTokenT<View<flashgg::DiPhotonTagBase> > > TagList_;	

			double massCutUpper;//configurable in python
			double massCutLower;
	};

	TagSorter::TagSorter(const ParameterSet & iConfig) :
		diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons"))))
	{

		massCutUpper=iConfig.getUntrackedParameter<double>("massCutUpper", 180.); //default values are set to 100 -> 180, but can be chnaged in python config.
		massCutLower=iConfig.getUntrackedParameter<double>("massCutLower", 100.);

		typedef std::vector<edm::InputTag> VInputTag;
		VInputTag tags  = iConfig.getUntrackedParameter<VInputTag>("TagVectorTag");

		for (unsigned int i=0; i<tags.size(); ++i){
		// loop through priority-ordered list of available tags to fill vector of Tag Vectors.
			TagList_.push_back(consumes<View<flashgg::DiPhotonTagBase> >(tags[i]));
		}

		produces<edm::Ptr<flashgg::DiPhotonTagBase> >();

	} 

	void TagSorter::produce( Event & evt, const EventSetup & ) {

	  //		Handle<View<flashgg::DiPhotonCandidate> > diPhotons; 
	  //		evt.getByToken(diPhotonToken_,diPhotons); 
	  //		const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

	  auto_ptr<edm::Ptr<flashgg::DiPhotonTagBase> > SelectedTag (new edm::Ptr<flashgg::DiPhotonTagBase>); 

		for( unsigned int TagListLoop=0 ; TagListLoop< TagList_.size() ; TagListLoop++){
			// Loop through the available Tags in the order they are specified in the python config

			Handle<View<flashgg::DiPhotonTagBase> > TagVectorEntry;
			evt.getByToken(TagList_[TagListLoop],TagVectorEntry);
			const PtrVector<flashgg::DiPhotonTagBase>& TagPointers =  TagVectorEntry->ptrVector();
			// get Tags by requesting them as a DiPhotonTagBase, from which they inherit.

			auto_ptr<edm::Ptr<flashgg::DiPhotonTagBase> > SelectedTag (new edm::Ptr<flashgg::DiPhotonTagBase>);

			if( TagPointers.size() >0) { 
			  // Looking from highest priority to lowest, check if the tag has any entries.

			  //			  float maxSumPt =0.; //this will be used to find highest sumPt candidate tag.
			  int chosenIndex = -1 ; //this will become the index of the highest sumPt candidate.
			  
			  for (unsigned int  TagPointerLoop = 0; TagPointerLoop < TagPointers.size() ; TagPointerLoop++)	{
			    
			    // If the tag does have entries, loop over them.
			    //			    int diPhotonIndex =TagPointers[TagPointerLoop]->getDiPhotonIndex();
			    //			    float mass = diPhotonPointers[diPhotonIndex]->mass();
			    float mass =  TagPointers[TagPointerLoop]->diPhoton()->mass();
			    
			    if((mass < massCutLower) || (mass > massCutUpper )) {continue ;}
			    // ignore candidate tags with diphoton outside of the allowed mass range.
			    
			    if (chosenIndex == -1 || (TagPointers[TagPointerLoop].get() > TagPointers[chosenIndex].get()))
			      chosenIndex = TagPointerLoop;
			    }

			    /*
			    if( diPhotonPointers[diPhotonIndex]->getSumPt() > maxSumPt ){
			      // identify and pick the valid candidate with highest sumPt
			      maxSumPt = diPhotonPointers[diPhotonIndex]->getSumPt() ; 
			      chosenIndex = TagPointerLoop;
			    }
			    */
			  if (chosenIndex != -1 ) {
			    // chosenIndex should only be -1 if all candidates have sumPT exactly zero, in which case we are not interested and continue the loop
			    //					*SelectedTag.get() = TagPointers[0];	
			    SelectedTag.reset(new edm::Ptr<flashgg::DiPhotonTagBase>(TagPointers[chosenIndex]));
			    //debug message:
			    std::cout << "[DEBUG] Priority " << TagListLoop << " Tag Found! Tag entry "<< chosenIndex  << " with sumPt " 
				      << TagPointers[chosenIndex]->getSumPt() << std::endl;
			    break;
			  }
			} else {
			  //debug message
			  std::cout << "[DEBUG] No Priority " << TagListLoop << " Tag ..., looking for Priority " << (TagListLoop+1) << " Tag.. " << std::endl;
			}
		}
		evt.put(SelectedTag);
	}
}

typedef flashgg::TagSorter FlashggTagSorter;
DEFINE_FWK_MODULE(FlashggTagSorter);
