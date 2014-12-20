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

		//		produces<edm::Ptr<flashgg::DiPhotonTagBase> >();
		produces<edm::OwnVector<flashgg::DiPhotonTagBase> >();
	} 

	void TagSorter::produce( Event & evt, const EventSetup & ) {

	  //		Handle<View<flashgg::DiPhotonCandidate> > diPhotons; 
	  //		evt.getByToken(diPhotonToken_,diPhotons); 
	  //		const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

		auto_ptr<edm::OwnVector<flashgg::DiPhotonTagBase> > SelectedTag (new edm::OwnVector<flashgg::DiPhotonTagBase>); 

		for( unsigned int TagListLoop=0 ; TagListLoop< TagList_.size() ; TagListLoop++){
		  // Loop through the available Tags in the order they are specified in the python config

			Handle<View<flashgg::DiPhotonTagBase> > TagVectorEntry;
			evt.getByToken(TagList_[TagListLoop],TagVectorEntry);
			const PtrVector<flashgg::DiPhotonTagBase>& TagPointers =  TagVectorEntry->ptrVector();
			// get Tags by requesting them as a DiPhotonTagBase, from which they inherit.

			int chosenIndex = -1 ; //this will become the index of the highest priority candidate (operator<)

			// Looking from highest priority to lowest, check if the tag has any entries.
			for (unsigned int  TagPointerLoop = 0; TagPointerLoop < TagPointers.size() ; TagPointerLoop++)        {

				float mass = TagPointers[TagPointerLoop]->diPhoton()->mass();

			    	// ignore candidate tags with diphoton outside of the allowed mass range.			    
				if((mass < massCutLower) || (mass > massCutUpper )) {continue ;}
			
				// All the real work is done inside DiPhotonTagBase::operator< 
				if (chosenIndex == -1 || (TagPointers[chosenIndex].get() < TagPointers[TagPointerLoop].get()));
				  chosenIndex = TagPointerLoop;
			}
		
			if (chosenIndex != -1 ) {
		  
		 		SelectedTag->push_back(*TagPointers[chosenIndex]);
				//debug message:
			//	std::cout << "[DEBUG] Priority " << TagListLoop << " Tag Found! Tag entry "<< chosenIndex  << " with sumPt " 
				//					  << TagPointers[chosenIndex]->getSumPt() << std::endl;
				break;
			} else {
			  //debug message
			 // std::cout << "[DEBUG] No Priority " << TagListLoop << " Tag ..., looking for Priority " << (TagListLoop+1) << " Tag.. " << std::endl;
			}
		}
		
		assert (SelectedTag->size() == 1 || SelectedTag->size() == 0);
		evt.put(SelectedTag);
	}
}

typedef flashgg::TagSorter FlashggTagSorter;
DEFINE_FWK_MODULE(FlashggTagSorter);
