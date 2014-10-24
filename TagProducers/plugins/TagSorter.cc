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
			EDGetTokenT<edm::View<vector<flashgg::DiPhotonTagBase> >  >  TagVectorToken_; //
			//typedef std::vector<edm::InputTag> VInputTag;
		std::vector < edm::EDGetTokenT<edm::View<vector<flashgg::DiPhotonTagBase> > > > TagList_;	


	};

	TagSorter::TagSorter(const ParameterSet & iConfig) :
		diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons"))))
		//		jetTokenDz_(consumes<View<flashgg::Jet> >(iConfig.getUntrackedParameter<InputTag>("JetTag", InputTag("flashggJets"))))
	{

		typedef edm::View<std::vector<edm::InputTag> > VInputTag;
		VInputTag tags  = iConfig.getUntrackedParameter<VInputTag>("TagVectorTag");

		for (unsigned int i=0; i<tags.size(); ++i)
		{
			TagList_.push_back(consumes<vector<flashgg::DiPhotonTagBase> >(tags[i]));

		}

		produces<vector<vector<flashgg::DiPhotonTagBase> > >();

	} 

	void TagSorter::produce( Event & evt, const EventSetup & ) {

		//	std::cout << "louie 2" <<std::endl;
		Handle<View<flashgg::DiPhotonCandidate> > diPhotons; 
		evt.getByToken(diPhotonToken_,diPhotons); 
		const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();
		if (diPhotonPointers.size() > 0) ;
		std::cout << TagList_.size() << std::endl;

		for( unsigned int il=0 ; il< TagList_.size() ; il++)
		{
			Handle<View<vector<flashgg::DiPhotonTagBase> > > TagVectorEntry;
			std::cout << "Louie a" << std::endl;
			evt.getByToken(TagList_[il],TagVectorEntry);
			std::cout << "Louie b" << std::endl;
			const PtrVector<vector<flashgg::DiPhotonTagBase> >& TagPointers =  (PtrVector<vector<flashgg::DiPhotonTagBase> >) TagVectorEntry->ptrVector();
			std::cout << "Louie c" << std::endl;
			std::cout << TagPointers.size() << std::endl;
			if( TagPointers.size() >0) { 

				std::cout << "Priority " << il << " Tag Found!"  <<std::endl;
				break;
			} else {
				std::cout << "No Priority " << il << "Tag ..., looking for Priority " << (il+1) << " Tag.. " << std::endl;
			}
		}

			std::cout << "Louie e" << std::endl;
		std::auto_ptr<vector<vector<flashgg::DiPhotonTagBase> > > vbf_results(new vector<vector<flashgg::DiPhotonTagBase> >); 


			std::cout << "Louie f" << std::endl;



		/*	Handle<View<auto> > Tags;
				iEvent.getByToken(TagVectorToken_,Tags);
				const PtrVector<auto>& TagList = Tags->ptrVector();

				for (int n=0; n < TagList.size() ; n++)
				{

				n++;
				std::cout << n << std::endl;


				}*/




		evt.put(vbf_results);
	}
}

typedef flashgg::TagSorter FlashggTagSorter;
DEFINE_FWK_MODULE(FlashggTagSorter);
