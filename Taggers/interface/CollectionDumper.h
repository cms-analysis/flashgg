#ifndef flashgg_CollectionDumper_h
#define flashgg_CollectionDumper_h

#include <map>
#include <string>

#include "TH1.h"
#include "TTree.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"

//#include "FWCore/Framework/interface/ConsumesCollector.h""

#include "PhysicsTools/UtilAlgos/interface/BasicAnalyzer.h"
/// #include "PhysicsTools/FWLite/interface/ScannerHelpers.h"

#include "CategoryDumper.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "flashgg/Taggers/interface/StringHelpers.h"

#include "RooWorkspace.h"
#include "RooMsgService.h"

#include "flashgg/MicroAOD/interface/CutBasedClassifier.h"
#include "flashgg/MicroAOD/interface/ClassNameClassifier.h"
#include "flashgg/MicroAOD/interface/CutAndClassBasedClassifier.h"
#include "flashgg/Taggers/interface/GlobalVariablesDumper.h"
#include "flashgg/DataFormats/interface/PDFWeightObject.h"

#include "FWCore/Utilities/interface/Exception.h"

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
    class TrivialClassifier
    {
    public:
        TrivialClassifier( const edm::ParameterSet &cfg ) {}

        std::pair<std::string, int> operator()( const T &obj ) const { return std::make_pair( "", 0 ); }
    };

    template<class CollectionT, class CandidateT = typename CollectionT::value_type, class ClassifierT = TrivialClassifier<CandidateT> >
    class CollectionDumper : public edm::BasicAnalyzer
    {

    public:
        typedef CollectionT collection_type;
        typedef CandidateT candidate_type;
        typedef StringObjectFunction<CandidateT, true> function_type;
        typedef CategoryDumper<function_type, candidate_type> dumper_type;
        typedef ClassifierT classifier_type;
        // typedef std::pair<std::string, std::string> KeyT;
        typedef std::string KeyT;

        /// default constructor
        CollectionDumper( const edm::ParameterSet &cfg, TFileDirectory &fs );
        CollectionDumper( const edm::ParameterSet &cfg, TFileDirectory &fs, edm::ConsumesCollector &&cc );
        /// default destructor
        /// ~CollectionDumper();
        /// everything that needs to be done before the event loop
        void beginJob();
        /// everything that needs to be done after the event loop
        void endJob();
        /// everything that needs to be done during the event loop
        void analyze( const edm::EventBase &event );


    protected:
        double eventWeight( const edm::EventBase &event );
        vector<double> pdfWeights( const edm::EventBase &event );

        classifier_type classifier_;

        edm::InputTag src_, genInfo_, pdfWeight_;

        edm::EDGetTokenT<collection_type> srcToken_;
        edm::EDGetTokenT<GenEventInfoProduct> genInfoToken_;
        edm::EDGetTokenT<std::vector<flashgg::PDFWeightObject> > pdfWeightToken_;

        std::string processId_;
        int processIndex_;
        double lumiWeight_;
        bool splitLumiWeight_;
        int maxCandPerEvent_;
        double sqrtS_;
        double intLumi_;
        std::string nameTemplate_;
        
        bool dumpTrees_;
        bool dumpWorkspace_;
        std::string workspaceName_;
        bool dumpHistos_, dumpGlobalVariables_;

        std::map< KeyT, bool> hasSubcat_;
        bool throwOnUnclassified_;
        
        // event weight
        float weight_;
        vector<double> pdfWeights_;
        int pdfWeightSize_;
        bool pdfWeightHistosBooked_;
        bool dumpPdfWeights_;
        int nPdfWeights_;
        int nAlphaSWeights_;
        int nScaleWeights_;

        //std::map<std::string, std::vector<dumper_type> > dumpers_; FIXME template key
        std::map< KeyT, std::vector<dumper_type> > dumpers_;
        RooWorkspace *ws_;
        /// TTree * bookTree(const std::string & name, TFileDirectory& fs);
        /// void fillTreeBranches(const flashgg::Photon & pho)

        GlobalVariablesDumper *globalVarsDumper_;

    private:
        void _init( const edm::ParameterSet &cfg, TFileDirectory &fs );
    };

    template<class C, class T, class U>
    CollectionDumper<C, T, U>::CollectionDumper( const edm::ParameterSet &cfg, TFileDirectory &fs ) :
        edm::BasicAnalyzer::BasicAnalyzer( cfg, fs ),
        classifier_( cfg.getParameter<edm::ParameterSet>( "classifierCfg" ) ),
        src_( cfg.getParameter<edm::InputTag>( "src" ) ),
        genInfo_( cfg.getParameter<edm::InputTag>( "generatorInfo" ) ),
        pdfWeight_( cfg.getUntrackedParameter<edm::InputTag>("flashggPDFWeightObject", edm::InputTag("flashggPDFWeightObject") ) ),
        dumpGlobalVariables_( cfg.getUntrackedParameter<bool>( "dumpGlobalVariables", true ) ),
        globalVarsDumper_(0)
    {
        if( dumpGlobalVariables_ ) {
            globalVarsDumper_ = new GlobalVariablesDumper( cfg.getParameter<edm::ParameterSet>( "globalVariables" ) );
        }
        _init(cfg, fs);
    }

    template<class C, class T, class U>
    CollectionDumper<C, T, U>::CollectionDumper( const edm::ParameterSet &cfg, TFileDirectory &fs, edm::ConsumesCollector && cc) :
        edm::BasicAnalyzer::BasicAnalyzer( cfg, fs ),
        classifier_( cfg.getParameter<edm::ParameterSet>( "classifierCfg" ) ),
        src_( cfg.getParameter<edm::InputTag>( "src" ) ),
        genInfo_( cfg.getParameter<edm::InputTag>( "generatorInfo" ) ),
        pdfWeight_( cfg.getUntrackedParameter<edm::InputTag>("flashggPDFWeightObject", edm::InputTag("flashggPDFWeightObject") ) ),
        srcToken_( cc.consumes<collection_type>( src_ ) ),
        genInfoToken_( cc.consumes<GenEventInfoProduct>( genInfo_ ) ),
        pdfWeightToken_( cc.consumes<std::vector<flashgg::PDFWeightObject> >( pdfWeight_ ) ),
        dumpGlobalVariables_( cfg.getUntrackedParameter<bool>( "dumpGlobalVariables", true ) ),
        globalVarsDumper_(0)
    {
        if( dumpGlobalVariables_ ) {
            globalVarsDumper_ = new GlobalVariablesDumper( cfg.getParameter<edm::ParameterSet>( "globalVariables" ), std::forward<edm::ConsumesCollector>(cc) );
        }
        _init(cfg, fs);
    }

    template<class C, class T, class U>
    void CollectionDumper<C, T, U>::_init( const edm::ParameterSet &cfg, TFileDirectory &fs )
    {

        processId_           = cfg.getParameter<std::string>( "processId" );
        processIndex_        = cfg.exists("processIndex") ? cfg.getParameter<int>("processIndex") : 999;
        lumiWeight_          = cfg.getParameter<double>( "lumiWeight" );
        splitLumiWeight_     = cfg.getUntrackedParameter<bool>( "splitLumiWeight", false );
        maxCandPerEvent_     = cfg.getParameter<int>( "maxCandPerEvent" );
        sqrtS_               = cfg.getUntrackedParameter<double>( "sqrtS", 13. );
        intLumi_             = cfg.getUntrackedParameter<double>( "intLumi",1000. );
        nameTemplate_        = cfg.getUntrackedParameter<std::string>( "nameTemplate", "$COLLECTION" );
        dumpTrees_           = cfg.getUntrackedParameter<bool>( "dumpTrees", false );
        dumpWorkspace_       = cfg.getUntrackedParameter<bool>( "dumpWorkspace", false );
        workspaceName_       = cfg.getUntrackedParameter<std::string>( "workspaceName", src_.label() );
        dumpHistos_          = cfg.getUntrackedParameter<bool>( "dumpHistos", false );
        classifier_          = cfg.getParameter<edm::ParameterSet>( "classifierCfg" );
        throwOnUnclassified_ = cfg.exists("throwOnUnclassified") ? cfg.getParameter<bool>("throwOnUnclassified") : false;

        if( cfg.getUntrackedParameter<bool>( "quietRooFit", false ) ) {
            RooMsgService::instance().setGlobalKillBelow( RooFit::WARNING );
        }
	    
        nPdfWeights_=0;
        nAlphaSWeights_=0;
        nScaleWeights_=0;
        dumpPdfWeights_=false;
        
        std::map<std::string, std::string> replacements;
        replacements.insert( std::make_pair( "$COLLECTION", src_.label() ) );
        replacements.insert( std::make_pair( "$PROCESS", processId_ ) );
        replacements.insert( std::make_pair( "$SQRTS", Form( "%1.0fTeV", sqrtS_ ) ) );
        nameTemplate_ = formatString( nameTemplate_, replacements );

        if( dumpGlobalVariables_ ) {
            if( splitLumiWeight_ ) {
                globalVarsDumper_->dumpLumiFactor(lumiWeight_);
                lumiWeight_ = 1.;
            }
            globalVarsDumper_->setProcessIndex(processIndex_);
        } else if ( splitLumiWeight_ ) {
            throw cms::Exception("Configuration error") << "You specified the splitLumiWeight option but not the dumpGlobalVariables one. I can split the weight only if you also set the latter.\n";
        }
       
        pdfWeightHistosBooked_=false;

        auto categories = cfg.getParameter<std::vector<edm::ParameterSet> >( "categories" );
        for( auto &cat : categories ) {
            auto label   = cat.getParameter<std::string>( "label" );
            auto subcats = cat.getParameter<int>( "subcats" );
            
            //------->
            //if any of the categories need pdfWeight dumping, we want to have the info in the WS
            //the logic of this code is that dumpPdfWeights_ is false or nPdfWeights_ is 0
            //we check to see what the category says. If any of the categories have more than 0 pdfWeight
            //and want dumpPdfWeights_ true, then the if fails and we don't check: the value stays true or >0
            //this relies on the fact that we want nPdfWeights the same for all cats.
            if (nPdfWeights_ == 0) {
                nPdfWeights_ = cat.exists("nPdfWeights") ?  cat.getParameter<int>( "nPdfWeights" ) : 0;
            }
            if (nAlphaSWeights_ == 0) {
                nAlphaSWeights_ = cat.exists("nAlphaSWeights") ?  cat.getParameter<int>( "nAlphaSWeights" ) : 0;
            }
            if (nScaleWeights_ == 0) {
                nScaleWeights_ = cat.exists("nScaleWeights") ? cat.getParameter<int>( "nScaleWeights" ) : 0;
            }
            if (dumpPdfWeights_ == false ) {
		    dumpPdfWeights_ = cat.exists("dumpPdfWeights")? cat.getParameter<bool>( "dumpPdfWeights" ) : false;
            }
            std::string classname = ( cat.exists("className") ? cat.getParameter<std::string>( "className" ) : "" );
            //<------
            
            auto name = nameTemplate_;
            if( ! label.empty() ) {
                name = replaceString( name, "$LABEL", label );
            } else {
                name = replaceString( replaceString( replaceString( name, "_$LABEL", "" ), "$LABEL_", "" ), "$LABEL", "" );
            }
            if( ! classname.empty() ) {
                name = replaceString( name, "$CLASSNAME", classname );
            } else {
                name = replaceString( replaceString( replaceString( name, "_$CLASSNAME", "" ), "$CLASSNAME_", "" ), "$CLASSNAME", "" );
            }
            
            KeyT key = classname;
            if( ! label.empty() ) {
                if( ! key.empty() ) { key += ":"; } // FIXME: define ad-hoc method with dedicated + operator
                key += label;
            }
            
            hasSubcat_[key] = ( subcats > 0 );
            auto &dumpers = dumpers_[key];
            if( subcats == 0 ) {
                name = replaceString( replaceString( replaceString( name, "_$SUBCAT", "" ), "$SUBCAT_", "" ), "$SUBCAT", "" );
                dumpers.push_back( dumper_type( name, cat, globalVarsDumper_ ) );
            } else {
                for( int isub = 0; isub < subcats; ++isub ) {
                    auto subcatname = replaceString( name, "$SUBCAT", Form( "%d", isub ) );
                    dumpers.push_back( dumper_type( subcatname, cat, globalVarsDumper_ ) );
                }
            }
        }

        workspaceName_ = formatString( workspaceName_, replacements );
        if( dumpWorkspace_ ) {
            ws_ = fs.make<RooWorkspace>( workspaceName_.c_str(), workspaceName_.c_str() );
            dynamic_cast<RooRealVar *>( ws_->factory( "weight[1.]" ) )->setConstant( false );
            if (dumpPdfWeights_){
                for( int j=0; j<nPdfWeights_;j++ ) {
                    dynamic_cast<RooRealVar *>( ws_->factory( Form("pdfWeight_%d[1.]",j)) )->setConstant( false );
                }
                for( int j=0; j<nAlphaSWeights_;j++ ) {
                    dynamic_cast<RooRealVar *>( ws_->factory( Form("alphaSWeight_%d[1.]",j)) )->setConstant( false );
                }
                for( int j=0; j<nScaleWeights_;j++ ) {
                    dynamic_cast<RooRealVar *>( ws_->factory( Form("scaleWeight_%d[1.]",j)) )->setConstant( false );
                }
            }
            RooRealVar* intLumi = new RooRealVar("IntLumi","IntLumi",intLumi_);
            //workspace expects intlumi in /pb
            intLumi->setConstant(); 
            // don't want this param to float in the fits at any point
            ws_->import(*intLumi);
        } else {
            ws_ = 0;
        }
        for( auto &dumpers : dumpers_ ) {
            for( auto &dumper : dumpers.second ) {
                if( dumpWorkspace_ ) {
                    dumper.bookRooDataset( *ws_, "weight", replacements);
                }
                if( dumpTrees_ ) {
                    TFileDirectory dir = fs.mkdir( "trees" );
                    dumper.bookTree( dir, "weight", replacements );
                }
                if( dumpHistos_ ) {
                    TFileDirectory dir = fs.mkdir( "histograms" );
                    dumper.bookHistos( dir, replacements );
                }
            }
        }
    }

    //// template<class C, class T, class U>
    //// CollectionDumper<C,T,U>::~CollectionDumper()
    //// {
    //// }

    template<class C, class T, class U>
        void CollectionDumper<C, T, U>::beginJob()
        {
        }

    template<class C, class T, class U>
        void CollectionDumper<C, T, U>::endJob()
        {
         if(dumpPdfWeights_){
          for (auto &dumper: dumpers_){
            for (unsigned int i =0; i < dumper.second.size() ; i++){
              if (dumper.second[i].isBinnedOnly()) continue;
              else {
                dumper.second[i].compressPdfWeightDatasets(ws_); 
              }
            }
           }
          }
        }
         

    template<class C, class T, class U>
        double CollectionDumper<C, T, U>::eventWeight( const edm::EventBase &event )
        {
            double weight = 1.;
            if( ! event.isRealData() ) {
                edm::Handle<GenEventInfoProduct> genInfo;
                const edm::Event * fullEvent = dynamic_cast<const edm::Event *>(&event);
                if (fullEvent != 0) {
                    fullEvent->getByToken(genInfoToken_, genInfo);
                } else {
                    event.getByLabel(genInfo_,genInfo);
                }

                weight = lumiWeight_;

                if( genInfo.isValid() ) {
                    const auto &weights = genInfo->weights();
                    // FIXME store alternative/all weight-sets
                    if( ! weights.empty() ) {
                        weight *= weights[0];
                    }
                }
                
                if( globalVarsDumper_ && globalVarsDumper_->puReWeight() ) {
                    weight *= globalVarsDumper_->cache().puweight;
                }
            }
            return weight;
        }

    template<class C, class T, class U>

        vector<double> CollectionDumper<C, T, U>::pdfWeights( const edm::EventBase &event )
        {   
            vector<double> pdfWeights;
            edm::Handle<vector<flashgg::PDFWeightObject> > WeightHandle;
            const edm::Event * fullEvent = dynamic_cast<const edm::Event *>(&event);
            if (fullEvent != 0) {
                fullEvent->getByToken(pdfWeightToken_, WeightHandle);
            } else {
                event.getByLabel(pdfWeight_, WeightHandle);
            }

            for( unsigned int weight_index = 0; weight_index < (*WeightHandle).size(); weight_index++ ){

                vector<uint16_t> compressed_weights = (*WeightHandle)[weight_index].pdf_weight_container; 
                vector<uint16_t> compressed_alpha_s_weights = (*WeightHandle)[weight_index].alpha_s_container; 
                vector<uint16_t> compressed_scale_weights = (*WeightHandle)[weight_index].qcd_scale_container;

                std::vector<float> uncompressed = (*WeightHandle)[weight_index].uncompress( compressed_weights );
                std::vector<float> uncompressed_alpha_s = (*WeightHandle)[weight_index].uncompress( compressed_alpha_s_weights );
                std::vector<float> uncompressed_scale = (*WeightHandle)[weight_index].uncompress( compressed_scale_weights );

                for( unsigned int j=0; j<(*WeightHandle)[weight_index].pdf_weight_container.size();j++ ) {
                    pdfWeights.push_back(uncompressed[j]);
                }
                for( unsigned int j=0; j<(*WeightHandle)[weight_index].alpha_s_container.size();j++ ) {
                    pdfWeights.push_back(uncompressed_alpha_s[j]);
                }
                for( unsigned int j=0; j<(*WeightHandle)[weight_index].qcd_scale_container.size();j++ ) {
                    pdfWeights.push_back(uncompressed_scale[j]);
                }
            }


            return pdfWeights;
        }
        


    template<class C, class T, class U>
        void CollectionDumper<C, T, U>::analyze( const edm::EventBase &event )
        {
            edm::Handle<collection_type> collectionH;

            const edm::Event * fullEvent = dynamic_cast<const edm::Event *>(&event);
            if (fullEvent != 0) {
                fullEvent->getByToken(srcToken_, collectionH);
            } else {
                event.getByLabel(src_,collectionH);
            }
            const auto & collection = *collectionH;

            if( globalVarsDumper_ ) { globalVarsDumper_->fill( event ); }

            weight_ = eventWeight( event );
            if( dumpPdfWeights_){
                
                // want pdfWeights_ to be scale factors rather than akternative weights.
                // To do this, each PDF weight needs to be divided by the nominal MC weight
                // which is obtained by dividing through weight_ by the lumiweight...
                // The Scale Factor is then pdfWeight/nominalMC weight
                pdfWeights_ =pdfWeights( event );
                for (unsigned int i = 0; i < pdfWeights_.size() ; i++){
                pdfWeights_[i]= (pdfWeights_[i] )*(lumiWeight_/weight_); // ie pdfWeight/nominal MC weight
                }
                
            }

            int nfilled = maxCandPerEvent_;

            for( auto &cand : collection ) {
                auto cat = classifier_( cand );
                auto which = dumpers_.find( cat.first );

                if( which != dumpers_.end() ) {
                    int isub = ( hasSubcat_[cat.first] ? cat.second : 0 );
                   double fillWeight =weight_;
                   const  WeightedObject* tag = dynamic_cast<const WeightedObject* >( &cand );
                    if ( tag != NULL ){

                    fillWeight =fillWeight*(tag->centralWeight());
                    }
                    which->second[isub].fill( cand, fillWeight, pdfWeights_, maxCandPerEvent_ - nfilled );
                    --nfilled;
                } else if( throwOnUnclassified_ ) {
                    throw cms::Exception( "Runtime error" ) << "could not find dumper for category [" << cat.first << "," << cat.second << "]"
                        << "If you want to allow this (eg because you don't want to dump some of the candidates in the collection)\n"
                        << "please set throwOnUnclassified in the dumper configuration\n";
                }
                if( ( maxCandPerEvent_ > 0 )  && nfilled == 0 ) { break; }
            }
        }

}

#endif // flashgg_CollectionDumper_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
