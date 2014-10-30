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

using namespace std;
using namespace edm;

namespace flashgg {

	class TagSorter : public EDProducer {

		public:
			TagSorter( const ParameterSet & );
		private:
			void produce( Event &, const EventSetup & ) override;

			EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
			EDGetTokenT<edm::View<flashgg::DiPhotonTagBase> > TagVectorToken_; //
			//typedef std::vector<edm::InputTag> VInputTag;
		std::vector<edm::EDGetTokenT<View<flashgg::DiPhotonTagBase> > > TagList_;	


	};

	TagSorter::TagSorter(const ParameterSet & iConfig) :
		diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons"))))
		//		jetTokenDz_(consumes<View<flashgg::Jet> >(iConfig.getUntrackedParameter<InputTag>("JetTag", InputTag("flashggJets"))))
	{

		typedef std::vector<edm::InputTag> VInputTag;
		VInputTag tags  = iConfig.getUntrackedParameter<VInputTag>("TagVectorTag");
		for (unsigned int i=0; i<tags.size(); ++i)
		{
			TagList_.push_back(consumes<View<flashgg::DiPhotonTagBase> >(tags[i]));
		}

	produces<edm::Ptr<flashgg::DiPhotonTagBase> >();

	} 

	void TagSorter::produce( Event & evt, const EventSetup & ) {

		std::cout << "LOUIE C "<< std::endl;
		//	std::cout << "louie 2" <<std::endl;
		Handle<View<flashgg::DiPhotonCandidate> > diPhotons; 
		evt.getByToken(diPhotonToken_,diPhotons); 
		const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();
		if (diPhotonPointers.size() > 0) ;

		auto_ptr<edm::Ptr<flashgg::DiPhotonTagBase> > SelectedTag (new edm::Ptr<flashgg::DiPhotonTagBase>); 

for( unsigned int il=0 ; il< TagList_.size() ; il++)
		{
			Handle<View<flashgg::DiPhotonTagBase> > TagVectorEntry;
			evt.getByToken(TagList_[il],TagVectorEntry);
			const PtrVector<flashgg::DiPhotonTagBase>& TagPointers =  TagVectorEntry->ptrVector();
			
			if( TagPointers.size() >0) { 
			
			*SelectedTag.get() = TagPointers[0];	
				std::cout << "Priority " << il << " Tag Found!"  <<std::endl;
		//	DiPhotonTagBase *louie = TagPointers[0];	
		//	VBFTag test = dynamic_cast<VBFTag*>(louie);
	//		VBFTag *test = dynamic_cast<VBFTag>(TagPointers[0].get());
if(il==0){//	const	VBFTag *test = dynamic_cast<const VBFTag*>(TagPointers[0].get());	
	
//			std::cout<< "leading jet pt " << test->leadingJet().pt() << std::endl;
//			std::cout << "test " << test->VBFMVA().dijet_leadEta <<std::endl;

	//			SelectedTag->push_back(*test);	
			}
//if(il==1){	const	DiPhotonUntaggedCategory *test = dynamic_cast<const DiPhotonUntaggedCategory*>(TagPointers[0].get());	
	
		//	std::cout<< "leading jet pt " << test->leadingJet().pt() << std::endl;
		//	std::cout << "test " << test->VBFMVA().dijet_leadEta <<std::endl;;
		//		SelectedTag->push_back(*test);	
	//		}
				//std::cout<< "subling jet pt " << test->subLeadingJet().pt() << std::endl;
				break;
			} else {
				std::cout << "No Priority " << il << " Tag ..., looking for Priority " << (il+1) << " Tag.. " << std::endl;
			}
		}





		evt.put(SelectedTag);
	}
}

typedef flashgg::TagSorter FlashggTagSorter;
DEFINE_FWK_MODULE(FlashggTagSorter);
