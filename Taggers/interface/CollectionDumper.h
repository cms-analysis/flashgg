#ifndef flashgg_CollectionDumper_h
#define flashgg_CollectionDumper_h

#include <map>
#include <string>

#include "TH1.h"
#include "TTree.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"

#include "PhysicsTools/UtilAlgos/interface/BasicAnalyzer.h"
/// #include "PhysicsTools/FWLite/interface/ScannerHelpers.h"

#include "CategoryDumper.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "FWCore/Utilities/interface/TypeID.h"
#include <typeindex>
#include <unordered_map>

#include "flashgg/TagAlgos/interface/StringHelpers.h"

#include "RooWorkspace.h"
#include "RooMsgService.h"

#include "flashgg/MicroAODAlgos/interface/CutBasedClassifier.h"
#include "flashgg/TagAlgos/interface/GlobalVariablesDumper.h"


/**
   \class CollectionDumper
   \brief Example class that can be used to analyze pat::Photons both within FWLite and within the full framework

   This is an example for keeping classes that can be used both within FWLite and within the full
   framework. The class is derived from the BasicAnalyzer base class, which is an interface for
   the two wrapper classes EDAnalyzerWrapper and FWLiteAnalyzerWrapper. You can fin more information
   on this on WorkBookFWLiteExamples#ExampleFive.
*/

namespace flashgg {


	template <class T>
	class TrivialClassifier {
	public:
		TrivialClassifier(const edm::ParameterSet &cfg) {}
		
		std::pair<std::string,int> operator()(const T& obj) const { return std::make_pair("",0); }
	};
		
	template <class T>
	class ClassNameClassifier {
	public:
		ClassNameClassifier(const edm::ParameterSet &cfg) {
			auto remap = cfg.getUntrackedParameter<std::vector<edm::ParameterSet> >("remap",std::vector<edm::ParameterSet>());
			for( auto & rm : remap ) {
				remap_.insert( std::make_pair(rm.getUntrackedParameter<std::string>("src"),
							      rm.getUntrackedParameter<std::string>("dst")
						       ) );
			}
		}
		
		std::pair<std::string,int> operator()(const T& obj) const { 
			int id = (int)obj;
			std::type_index idx( typeid(obj) );
			auto cached = cache_.find(idx);
			if( cached != cache_.end() ) { return std::make_pair(cached->second,id); }
			auto ret=std::make_pair(edm::stripNamespace(edm::TypeID(obj).friendlyClassName()),id);
			auto rm = remap_.find(ret.first);
			if( rm != remap_.end() ) { ret.first = rm->second; }
			cache_.insert( std::make_pair(idx,ret.first) );
			return ret;
		}
		

	private:
		std::map<std::string,std::string> remap_;
		mutable std::unordered_map<std::type_index,std::string> cache_;
	};
			
	template<class CollectionT, class CandidateT=typename CollectionT::value_type, class ClassifierT=TrivialClassifier<CandidateT> > 
	class CollectionDumper : public edm::BasicAnalyzer {
		
	public:
		typedef CollectionT collection_type;
		typedef CandidateT candidate_type;
		typedef StringObjectFunction<CandidateT,true> function_type;
		typedef CategoryDumper<function_type,candidate_type> dumper_type;
		typedef ClassifierT classifier_type;
		
		/// default constructor
		CollectionDumper(const edm::ParameterSet& cfg, TFileDirectory& fs);
		CollectionDumper(const edm::ParameterSet& cfg, TFileDirectory& fs,const edm::ConsumesCollector & cc) : CollectionDumper(cfg,fs) {};
		/// default destructor
		/// ~CollectionDumper();
		/// everything that needs to be done before the event loop
		void beginJob();
		/// everything that needs to be done after the event loop
		void endJob();
		/// everything that needs to be done during the event loop
		void analyze(const edm::EventBase& event);
		
		
	protected:
		double getEventWeight(const edm::EventBase& event);
		
		/// float getEventWeight(const edm::EventBase& event);
		edm::InputTag src_, genInfo_;
		std::string processId_;
		double lumiWeight_;
		int maxCandPerEvent_;
		double sqrtS_;
		std::string nameTemplate_;

		bool dumpTrees_; 
		bool dumpWorkspace_;
		std::string workspaceName_;
		bool dumpHistos_, dumpGlobalVariables_;
		
		classifier_type classifier_;
		std::map<std::string,bool> hasSubcat_;
		
		// event weight
		float weight_;
		
		std::map<std::string, std::vector<dumper_type> > dumpers_;
		RooWorkspace * ws_;
		/// TTree * bookTree(const std::string & name, TFileDirectory& fs);
		/// void fillTreeBranches(const flashgg::Photon & pho)
		
		GlobalVariablesDumper * globalVarsDumper_;
	};
	
	template<class C, class T, class U>
	CollectionDumper<C,T,U>::CollectionDumper(const edm::ParameterSet& cfg, TFileDirectory& fs): 
		edm::BasicAnalyzer::BasicAnalyzer(cfg, fs),
		src_(cfg.getParameter<edm::InputTag>("src")),
		genInfo_(cfg.getParameter<edm::InputTag>("generatorInfo")),
		processId_(cfg.getParameter<std::string>("processId")),
		lumiWeight_(cfg.getParameter<double>("lumiWeight")),
		maxCandPerEvent_(cfg.getParameter<int>("maxCandPerEvent")),
		sqrtS_(cfg.getUntrackedParameter<double>("sqrtS",13.)),
		nameTemplate_(cfg.getUntrackedParameter<std::string>("nameTemplate","$COLLECTION")),
		dumpTrees_(cfg.getUntrackedParameter<bool>("dumpTrees",false)),
		dumpWorkspace_(cfg.getUntrackedParameter<bool>("dumpWorkspace",false)),
		workspaceName_(cfg.getUntrackedParameter<std::string>("workspaceName",src_.label())),
		dumpHistos_(cfg.getUntrackedParameter<bool>("dumpHistos",false)),
		dumpGlobalVariables_(cfg.getUntrackedParameter<bool>("dumpGlobalVariables",true)),
		classifier_(cfg.getParameter<edm::ParameterSet>("classifierCfg")),
		globalVarsDumper_(0)
		
	{
		if( cfg.getUntrackedParameter<bool>("quietRooFit",false) ) {
			RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
		}

		std::map<std::string, std::string> replacements;
		replacements.insert(std::make_pair("$COLLECTION",src_.label()));
		replacements.insert(std::make_pair("$PROCESS",processId_));
		replacements.insert(std::make_pair("$SQRTS",Form("%1.0fTeV",sqrtS_)));
		nameTemplate_ = formatString(nameTemplate_,replacements);
		
		if(dumpGlobalVariables_) {
			globalVarsDumper_ = new GlobalVariablesDumper(cfg.getParameter<edm::ParameterSet>("globalVariables"));
		}

		auto categories = cfg.getParameter<std::vector<edm::ParameterSet> >("categories");
		for( auto & cat : categories ) {
			auto label   = cat.getParameter<std::string>("label");
			auto subcats = cat.getParameter<int>("subcats");
			auto name = replaceString( nameTemplate_, "$LABEL", label );
			hasSubcat_[label] = ( subcats > 0 );
			auto & dumpers = dumpers_[label];
			if(subcats == 0 ) {
				name=replaceString( replaceString( replaceString( name, "_$SUBCAT", "" ), "$SUBCAT_", ""), "$SUBCAT", "");
				dumpers.push_back(dumper_type(name,cat,globalVarsDumper_));
			} else {
				for(int isub=0; isub<subcats; ++isub) {
					name=replaceString( name, "$SUBCAT", Form("%d",isub) );
					dumpers.push_back(dumper_type(name,cat,globalVarsDumper_));
				}
			}
		}
		
		workspaceName_ = formatString(workspaceName_,replacements);
		if( dumpWorkspace_ ) {
			ws_ = fs.make<RooWorkspace>(workspaceName_.c_str(),workspaceName_.c_str());
			dynamic_cast<RooRealVar*>(ws_->factory("weight[1.]"))->setConstant(false);
		} else {
			ws_ = 0;
		}
		for( auto & dumpers : dumpers_ ) {
			for( auto & dumper : dumpers.second ) { 
				if( dumpWorkspace_ ) { 
					dumper.bookRooDataset(*ws_,"weight",replacements);
				}
				if( dumpTrees_ ) {
					TFileDirectory dir = fs.mkdir("trees");
					dumper.bookTree(dir,"weight",replacements);
				}
				if( dumpHistos_ ) {
					TFileDirectory dir = fs.mkdir("histograms");
					dumper.bookHistos(dir,replacements);
				}
			}
		}
	}

	//// template<class C, class T, class U>
	//// CollectionDumper<C,T,U>::~CollectionDumper()
	//// {
	//// }

	template<class C, class T, class U>
	void CollectionDumper<C,T,U>::beginJob() 
	{
	}

	template<class C, class T, class U>
	void CollectionDumper<C,T,U>::endJob() 
	{
	}
	
	template<class C, class T, class U>
	double CollectionDumper<C,T,U>::getEventWeight(const edm::EventBase& event)
	{
		// FIMXE per-event weight(s)
		return lumiWeight_;
	}
	
	template<class C, class T, class U>
	void CollectionDumper<C,T,U>::analyze(const edm::EventBase& event)
	{
		edm::Handle<collection_type> collectionH;
		event.getByLabel(src_,collectionH);
		const auto & collection = *collectionH;
		
		weight_ = getEventWeight(event);
		
		if(globalVarsDumper_) { globalVarsDumper_->fill(event); }
		
		int nfilled = maxCandPerEvent_;
		for(auto & cand : collection) {
			auto cat = classifier_(cand);
			auto which = dumpers_.find(cat.first);
			if( which != dumpers_.end() ) {
				int isub = ( hasSubcat_[cat.first] ? cat.second : 0 );
				// FIXME per-candidate weights
				which->second[isub].fill(cand,weight_);
				--nfilled;
			}
			if( nfilled == 0 ) { break; }
		}
	}
	
}

#endif // flashgg_CollectionDumper_h
