#ifndef flashgg_CollectionDumper_h
#define flashgg_CollectionDumper_h

#include <map>
#include <string>

#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"

//#include "FWCore/Framework/interface/ConsumesCollector.h""

#include "PhysicsTools/UtilAlgos/interface/BasicAnalyzer.h"
/// #include "PhysicsTools/FWLite/interface/ScannerHelpers.h"

#include "CategoryDumper.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "flashgg/Taggers/interface/StringHelpers.h"

#include "RooWorkspace.h"
#include "RooMsgService.h"

#include "flashgg/MicroAOD/interface/CutBasedClassifier.h"
#include "flashgg/MicroAOD/interface/ClassNameClassifier.h"
#include "flashgg/MicroAOD/interface/StageOneNameClassifier.h"
#include "flashgg/MicroAOD/interface/CutAndClassBasedClassifier.h"
#include "flashgg/MicroAOD/interface/StageOneBasedClassifier.h"
#include "flashgg/Taggers/interface/GlobalVariablesDumper.h"
#include "flashgg/DataFormats/interface/PDFWeightObject.h"
#include "SimDataFormats/HTXS/interface/HiggsTemplateCrossSections.h"


#include "FWCore/Utilities/interface/Exception.h"

#include "TFile.h"
#include "TGraph.h"

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
        double eventGenWeight( const edm::EventBase &event );
        vector<double> pdfWeights( const edm::EventBase &event );
        int getStage0bin( const edm::EventBase &event );
        int getStage1bin( const edm::EventBase &event );
        int getStxsNJet( const edm::EventBase &event );
        float getStxsPtH( const edm::EventBase &event );

        classifier_type classifier_;

        edm::InputTag src_, genInfo_, pdfWeight_ , lheEvent_;

        edm::EDGetTokenT<collection_type> srcToken_;
        edm::EDGetTokenT<GenEventInfoProduct> genInfoToken_;
        edm::EDGetTokenT<std::vector<flashgg::PDFWeightObject> > pdfWeightToken_;
        edm::EDGetTokenT< LHEEventProduct > lheEventToken_;
        std::string LHEWeightName;
        int LHEWeightIndex;

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
        // bool dumpNNLOPSweight_;
        
        std::map< KeyT, bool> hasSubcat_;
        bool throwOnUnclassified_;
        
        // event weight
        float weight_;
        float genweight_;
        vector<double> pdfWeights_;
        int pdfWeightSize_;
        bool pdfWeightHistosBooked_;
        bool dumpPdfWeights_;
        int nPdfWeights_;
        int nAlphaSWeights_;
        int nScaleWeights_;
        bool splitPdfByStage0Bin_;
        int stage0bin_;
        edm::InputTag stage0binTag_;
        edm::EDGetTokenT<int> stage0binToken_;
        bool splitPdfByStage1Bin_;
        int stage1bin_;
        edm::InputTag stage1binTag_;
        edm::EDGetTokenT<int> stage1binToken_;
        edm::InputTag newHTXSTag_;
        edm::EDGetTokenT<HTXS::HiggsClassification> newHTXSToken_;  

        int stxsNJet_;
        edm::InputTag stxsNJetTag_;
        edm::EDGetTokenT<int> stxsNJetToken_;

        float stxsPtH_;
        edm::InputTag stxsPtHTag_;
        edm::EDGetTokenT<float> stxsPtHToken_;


        //        correctionFile_ = conf.getParameter<edm::FileInPath>("CorrectionFile")
        // edm::FileInPath NNLOPSWeightFile_;
        // std::vector<std::unique_ptr<TGraph> > NNLOPSWeights_;
        // std::string generatorToBeReweightedToNNLOPS_;        

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
        lheEvent_( cfg.getUntrackedParameter<edm::InputTag>("LHEEventProduct", edm::InputTag("externalLHEProducer") ) ),
        dumpGlobalVariables_( cfg.getUntrackedParameter<bool>( "dumpGlobalVariables", true ) ),
        // dumpNNLOPSweight_( cfg.getUntrackedParameter<bool>( "dumpNNLOPSweight", false ) ),
        globalVarsDumper_(0)
    {
        if( dumpGlobalVariables_ ) {
            // cout << "[in Taggers/interface/CollectionDumper.h 1st collectiondumper] - dumpGlobalVariables_ = True" << endl;
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
        lheEvent_( cfg.getUntrackedParameter<edm::InputTag>("LHEEventProduct", edm::InputTag("externalLHEProducer") ) ),
        srcToken_( cc.consumes<collection_type>( src_ ) ),
        genInfoToken_( cc.consumes<GenEventInfoProduct>( genInfo_ ) ),
        pdfWeightToken_( cc.consumes<std::vector<flashgg::PDFWeightObject> >( pdfWeight_ ) ),
        lheEventToken_( cc.consumes<LHEEventProduct>( lheEvent_ ) ),
        dumpGlobalVariables_( cfg.getUntrackedParameter<bool>( "dumpGlobalVariables", true ) ),
        stage0binTag_( cfg.getUntrackedParameter<edm::InputTag>( "stage0binTag", edm::InputTag("rivetProducerHTXS","stage0bin") ) ),
        stage0binToken_( cc.consumes<int>( stage0binTag_ ) ),
        stage1binTag_( cfg.getUntrackedParameter<edm::InputTag>( "stage1binTag", edm::InputTag("rivetProducerHTXS","stage1bin") ) ),
        stage1binToken_( cc.consumes<int>( stage1binTag_ ) ),
        newHTXSTag_( cfg.getUntrackedParameter<edm::InputTag>( "classificationObj", edm::InputTag("rivetProducerHTXS","HiggsClassification") ) ),
        newHTXSToken_( cc.consumes<HTXS::HiggsClassification>( newHTXSTag_ ) ),
        stxsNJetTag_( cfg.getUntrackedParameter<edm::InputTag>( "stxsNJetTag", edm::InputTag("rivetProducerHTXS","njets") ) ),
        stxsNJetToken_( cc.consumes<int>( stxsNJetTag_  ) ),
        stxsPtHTag_( cfg.getUntrackedParameter<edm::InputTag>( "stxsPtHTag", edm::InputTag("rivetProducerHTXS","pTH") ) ),
        stxsPtHToken_( cc.consumes<float>( stxsPtHTag_  ) ),
        globalVarsDumper_(0)
    {
        if( dumpGlobalVariables_ ) {
            // cout << "[in Taggers/interface/CollectionDumper.h 2nd collectiondumper] - dumpGlobalVariables_ = True" << endl;
            globalVarsDumper_ = new GlobalVariablesDumper( cfg.getParameter<edm::ParameterSet>( "globalVariables" ), std::forward<edm::ConsumesCollector>(cc) );
        }
        _init(cfg, fs);
    }

    template<class C, class T, class U>
    void CollectionDumper<C, T, U>::_init( const edm::ParameterSet &cfg, TFileDirectory &fs )
    {
        LHEWeightName = cfg.getUntrackedParameter<std::string>( "LHEWeightName", "" );
        LHEWeightIndex = -1;

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

        splitPdfByStage0Bin_ = cfg.getUntrackedParameter<bool>( "splitPdfByStage0Bin", false);
        splitPdfByStage1Bin_ = cfg.getUntrackedParameter<bool>( "splitPdfByStage1Bin", false);

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
            // globalVarsDumper_->bookNNLOPSweight(dumpNNLOPSweight_);
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
                // Already on default list anyway
                //                if (splitPdfByStage0Bin_ ) {
                //                    dynamic_cast<RooRealVar *>( ws_->factory( "stage0bin[1.]") )->setConstant( false );
                //                }
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
            // cout << "[in Taggers/interface/CollectionDumper.h] - &dumpers = " << &dumpers << endl;
            // cout << "[in Taggers/interface/CollectionDumper.h] - on &dumpers " << endl;
            for( auto &dumper : dumpers.second ) {
                // cout << "[in Taggers/interface/CollectionDumper.h] - &dumper = " << &dumper << endl;
                // cout << "[in Taggers/interface/CollectionDumper.h] - on &dumper " << endl;
                if( dumpWorkspace_ ) {
                    dumper.bookRooDataset( *ws_, "weight", replacements);
                }
                if( dumpTrees_ ) {
                    // debugging HHWWgg
                    // cout << "[in Taggers/interface/CollectionDumper.h] - dumpTrees_ = True " << endl;
                    TFileDirectory dir = fs.mkdir( "trees" );
                    // cout << "[in Taggers/interface/CollectionDumper.h] - dir = " << dir << endl;
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
                        if (ws_ != NULL) dumper.second[i].compressPdfWeightDatasets(ws_); 
                    }
                }
            }
        }
    }     
    template<class C, class T, class U>
        double CollectionDumper<C, T, U>::eventGenWeight( const edm::EventBase &event )
        {
            double genweight = 1.;
            if( ! event.isRealData() ) {
                edm::Handle<GenEventInfoProduct> genInfo;
                const edm::Event * fullEvent = dynamic_cast<const edm::Event *>(&event);
                if (fullEvent != 0) {
                    fullEvent->getByToken(genInfoToken_, genInfo);
                } else {
                    event.getByLabel(genInfo_,genInfo);
                }
                if( genInfo.isValid() ) {
                    const auto &weights = genInfo->weights();
                    // FIXME store alternative/all weight-sets
                    if( ! weights.empty() ) {
                        genweight = weights[0];
                    }
                }
            }
            return genweight;
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
            
            if( LHEWeightName != ""){
                edm::Handle<LHEEventProduct> product_lhe;
                if (fullEvent != 0) {
                    fullEvent->getByToken(lheEventToken_, product_lhe);
                } else {
                    event.getByLabel(lheEvent_,product_lhe);
                }
                if( LHEWeightIndex < 0 ){
                    for(uint wgt_id = 0 ; wgt_id < product_lhe->weights().size() ; wgt_id++){
                        auto wgt = product_lhe->weights()[wgt_id] ;
                        if( wgt.id == LHEWeightName ){
                            LHEWeightIndex = wgt_id ;
                        }
                    }
                    if( LHEWeightIndex > -1 ){
                        weight *= ( product_lhe->weights()[LHEWeightIndex].wgt/product_lhe->originalXWGTUP () );
                    }
                }
                if( LHEWeightIndex > -1 )
                    weight *= ( product_lhe->weights()[LHEWeightIndex].wgt/product_lhe->originalXWGTUP () );
            }
            
            if( genInfo.isValid() ) {
                const auto &weights = genInfo->weights();
                // FIXME store alternative/all weight-sets
                if( ! weights.empty() ) {
                    weight *= weights[0];
                }
                

                if( globalVarsDumper_ && globalVarsDumper_->puReWeight() ) {
                    if (globalVarsDumper_->cache().puweight > 999999. || globalVarsDumper_->cache().puweight < -999999.) {
                        weight = 0.;
                        std::cout << "WARNING we got a puweight of " << globalVarsDumper_->cache().puweight 
                                  << " for rho=" << globalVarsDumper_->cache().rho
                                  << " nvtx=" << globalVarsDumper_->cache().nvtx
                                  << " npu=" << globalVarsDumper_->cache().npu 
                                  << " so we set weight to 0!!!" << std::endl;

                    } else {
                        weight *= globalVarsDumper_->cache().puweight;
                    }

                }
            }
        }
        
        return weight;
    }

    
    template<class C, class T, class U>
    int CollectionDumper<C, T, U>::getStage0bin( const edm::EventBase &event ) {
        edm::Handle<int> stage0bin;
        const edm::Event * fullEvent = dynamic_cast<const edm::Event *>(&event);
        if (fullEvent != 0) {
            fullEvent->getByToken(stage0binToken_, stage0bin);
        } else {
            event.getByLabel(stage0binTag_, stage0bin);
        }
        if ( stage0bin.isValid() ) {
            return (*(stage0bin.product() ) );
        } else {
            edm::Handle<HTXS::HiggsClassification> htxsClassification;
            if (fullEvent != 0) {
                fullEvent->getByToken(newHTXSToken_,htxsClassification);
            } else {
                event.getByLabel(newHTXSTag_,htxsClassification);
            }
            return( htxsClassification->stage0_cat );
        }
    }

    template<class C, class T, class U>
    int CollectionDumper<C, T, U>::getStage1bin( const edm::EventBase &event ) {
        edm::Handle<int> stage1bin;
        const edm::Event * fullEvent = dynamic_cast<const edm::Event *>(&event);
        if (fullEvent != 0) {
            fullEvent->getByToken(stage1binToken_, stage1bin);
        } else {
            event.getByLabel(stage1binTag_, stage1bin);
        }
        return (*(stage1bin.product() ) );
    }

    template<class C, class T, class U>
    int CollectionDumper<C, T, U>::getStxsNJet( const edm::EventBase &event ) {
        edm::Handle<int> stxsNJet;
        const edm::Event * fullEvent = dynamic_cast<const edm::Event *>(&event);
        if (fullEvent != 0) {
            fullEvent->getByToken(stxsNJetToken_, stxsNJet);
        } else {
            event.getByLabel(stxsNJetTag_, stxsNJet);
        }
        if (stxsNJet.isValid()) {
            return (*(stxsNJet.product() ) );
        } else {
            edm::Handle<HTXS::HiggsClassification> htxsClassification;
            if (fullEvent != 0) {
                fullEvent->getByToken(newHTXSToken_,htxsClassification);
            } else {
                event.getByLabel(newHTXSTag_,htxsClassification);
            }
            return htxsClassification->jets30.size();
        }
    }
    
    template<class C, class T, class U>
    float CollectionDumper<C, T, U>::getStxsPtH( const edm::EventBase &event ) {
        edm::Handle<float> stxsPtH;
        const edm::Event * fullEvent = dynamic_cast<const edm::Event *>(&event);
        if (fullEvent != 0) {
            fullEvent->getByToken(stxsPtHToken_, stxsPtH);
        } else {
            event.getByLabel(stxsPtHTag_, stxsPtH);
        }
        if (stxsPtH.isValid()) {
            return (*(stxsPtH.product() ) );
        } else {
            edm::Handle<HTXS::HiggsClassification> htxsClassification;
            if (fullEvent != 0) {
                fullEvent->getByToken(newHTXSToken_,htxsClassification);
            } else {
                event.getByLabel(newHTXSTag_,htxsClassification);
            }
            return htxsClassification->p4decay_higgs.pt();
        }
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
                //                    std::cout << "pdfWeights " << j<< " " << uncompressed[j] << std::endl;
            }
            for( unsigned int j=0; j<(*WeightHandle)[weight_index].alpha_s_container.size();j++ ) {
                pdfWeights.push_back(uncompressed_alpha_s[j]);
                //                    std::cout << "alpha_s " << j << " " << uncompressed_alpha_s[j] << std::endl;
            }
            if ( (*WeightHandle)[weight_index].qcd_scale_container.size() == 0 ) {
                //                    std::cout << " QCD scale weight workaround, putting in 9 dummies " << std::endl;
                for ( unsigned int j = 0 ; j < 9 ; j++ ) {
                    pdfWeights.push_back(0.); // should never be used in case this workaround is in place
                }
            } else {
                for( unsigned int j=0; j<(*WeightHandle)[weight_index].qcd_scale_container.size();j++ ) {
                    pdfWeights.push_back(uncompressed_scale[j]);
                }
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
        genweight_ = eventGenWeight( event );
        if( dumpPdfWeights_){
            
            // want pdfWeights_ to be scale factors rather than akternative weights.
            // To do this, each PDF weight needs to be divided by the nominal MC weight
            // which is obtained by dividing through weight_ by the lumiweight...
            // The Scale Factor is then pdfWeight/nominalMC weight
            pdfWeights_ = pdfWeights( event );
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
                    which->second[isub].fill( cand, fillWeight, pdfWeights_, maxCandPerEvent_ - nfilled, splitPdfByStage0Bin_ ? stage0bin_ : stage1bin_,genweight_ );
                    --nfilled;
            } else if( throwOnUnclassified_ ) {
                throw cms::Exception( "Runtime error" ) << "could not find dumper for category [" << cat.first << "," << cat.second << "]"
                                                        << "If you want to allow this (eg because you don't want to dump some of the candidates in the collection)\n"
                                                        << "please set throwOnUnclassified in the dumper configuration\n";
            }
            if( ( maxCandPerEvent_ > 0 )  && nfilled == 0 ) { break; }
        }
            
        // if( collection.size() == 0 ) {
        //     cout << "No candidate found in " << src_.label()  << " for event " <<  event.id().event() << " " << event.id().luminosityBlock() << " "<< event.id().run() << endl;
        // }
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
