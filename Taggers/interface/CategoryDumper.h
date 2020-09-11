#ifndef flashgg_CategoryDumper_h
#define flashgg_CategoryDumper_h

#include <tuple>
#include <vector>
#include <string>
#include <memory>

#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooBinning.h"

#include "CommonTools/Utils/interface/TFileDirectory.h"

#include "flashgg/Taggers/interface/StringHelpers.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "flashgg/Taggers/interface/GlobalVariablesDumper.h"
#include "flashgg/MicroAOD/interface/MVAComputer.h"
#include "flashgg/MicroAOD/interface/StepWiseFunctor.h"

namespace flashgg {


    template<class ObjectT> class FunctorTrait
    {

    public:
        virtual ~FunctorTrait() {};
        virtual float operator()( const ObjectT &obj ) const = 0;
    };



    template<class ObjectT, class FunctorT> class FunctorWrapper : public FunctorTrait<ObjectT>
    {

    public:
        FunctorWrapper( FunctorT *func = 0 ) // : std::unique_ptr<FunctorT>(func) {};
            : func_( func ) {};

        virtual ~FunctorWrapper() {} ; //delete func_; };

        virtual float operator()( const ObjectT &obj ) const { return ( *func_ )( obj ); };
        
        std::shared_ptr<FunctorT> ptr() { return func_; };

    private:
        // FunctorT * func_;
        std::shared_ptr<FunctorT> func_;
    };

    template<class ObjectT, class FunctorT> class FunctorMVAWrapper : public FunctorTrait<ObjectT>
    {

    public:
        FunctorMVAWrapper( FunctorT *func = 0 ) // : std::unique_ptr<FunctorT>(func) {};
            : func_( func ) {};

        virtual ~FunctorMVAWrapper() {} ; //delete func_; };

        virtual float operator()( const ObjectT &obj ) const { return ( *func_ )( obj )[0]; };
        
        std::shared_ptr<FunctorT> ptr() { return func_; };

    private:
        // FunctorT * func_;
        std::shared_ptr<FunctorT> func_;
    };

    template<class ObjectT> class GlobalVarWrapper : public FunctorTrait<ObjectT>
    {

    public:
        GlobalVarWrapper( GlobalVariablesDumper* globalDumper, const std::string & varname  ) // : std::unique_ptr<FunctorT>(func) {};
        //            : varname_(varname), globalDumper_(globalDumper) {
        {
            varname_ = varname;
            globalDumper_ = globalDumper;
        };

        virtual ~GlobalVarWrapper() {} ;

        virtual float operator()( const ObjectT &obj ) const { return globalDumper_->getExtraFloat(varname_); };

    private:
        // FunctorT * func_;
        std::string varname_;
        GlobalVariablesDumper * globalDumper_;
    };




    typedef std::tuple<std::string, int, std::vector<double>, int, std::vector<double>, TH1 *> histo_info;

    template<class FunctorT, class ObjectT>
    class CategoryDumper
    {
    public:
        typedef ObjectT object_type;
        typedef FunctorT functor_type;
        typedef StepWiseFunctor<ObjectT, FunctorT> stepwise_functor_type;
        typedef MVAComputer<object_type, functor_type> mva_type;


        typedef FunctorTrait<object_type> trait_type;
        typedef FunctorWrapper<object_type, functor_type> wrapped_functor_type;
        typedef FunctorWrapper<object_type, stepwise_functor_type> wrapped_stepwise_functor_type;
        typedef FunctorMVAWrapper<object_type, mva_type> wrapped_mva_type;
        typedef GlobalVarWrapper<object_type> wrapped_global_var_type;

        CategoryDumper( const std::string &name, const edm::ParameterSet &cfg, GlobalVariablesDumper *dumper = 0 );
        ~CategoryDumper();

        void bookHistos( TFileDirectory &fs, const std::map<std::string, std::string> &replacements );
        void bookTree( TFileDirectory &fs, const char *weightVar, const std::map<std::string, std::string> &replacements );
        void bookRooDataset( RooWorkspace &ws, const char *weightVar, const std::map<std::string, std::string> &replacements);
        void compressPdfWeightDatasets(RooWorkspace *ws);
        

        void fill( const object_type &obj, double weight, vector<double>, int n_cand = 0, int htxsBin = -999, double genweight = 1.);
        string  GetName();
        bool isBinnedOnly();

        void print()
        {
            //            std::cout << "Dumper name is " << name_ << std::endl;
            // for (unsigned int i =0; i<names_.size() ; i++){
            // std::cout << "Map content " << i << " name is " << names_[i] << std::endl;
            // }

        }

    private:
        std::string name_;
        std::vector<std::string> names_;
        std::vector<std::string> dumpOnly_;
        std::vector<std::tuple<float, std::shared_ptr<trait_type>, int, double, double, std::vector<double > > > variables_;
        std::vector<float> variables_pdfWeights_;
        std::vector<histo_info> histograms_;

        int n_cand_;
        float weight_;
        float genweight_;
        RooArgSet rooVars_;
        RooArgSet rooVars_pdfWeights_;        
        RooAbsData *dataset_;
        RooAbsData *dataset_pdfWeights_;
        TTree *tree_;
        GlobalVariablesDumper *globalVarsDumper_;
        std::vector<std::shared_ptr<wrapped_mva_type> > mvas_;
        std::vector<std::shared_ptr<wrapped_global_var_type> > extraglobalvars_;
        std::vector<std::shared_ptr<wrapped_functor_type> > functors_;
        std::vector<std::shared_ptr<wrapped_stepwise_functor_type> > stepwise_functors_;
        bool hbooked_;
        bool binnedOnly_;
        bool unbinnedSystematics_;
        bool dumpPdfWeights_;
        bool dumpGenWeight_;
        int  nPdfWeights_;
        int  nAlphaSWeights_;
        int nScaleWeights_;
        bool pdfVarsAdded_;
        bool splitPdfByStage0Bin_;
        bool splitPdfByStage1Bin_;
    };

    template<class F, class O>
    CategoryDumper<F, O>::CategoryDumper( const std::string &name, const edm::ParameterSet &cfg, GlobalVariablesDumper *dumper ):
        dataset_( 0 ), 
        dataset_pdfWeights_( 0 ), 
        tree_( 0 ), 
        globalVarsDumper_( dumper ), 
        hbooked_( false ), 
        binnedOnly_ (false), 
        dumpPdfWeights_ (false ), 
        dumpGenWeight_ (false ), 
        nPdfWeights_ (0), 
        nAlphaSWeights_(0),
        nScaleWeights_(0),
        splitPdfByStage0Bin_( false ),
        splitPdfByStage1Bin_( false )
    {
        using namespace std;
        name_ = name;

        if( cfg.existsAs<vector<string> >( "dumpOnly" ) ) {
            dumpOnly_ = cfg.getParameter<vector<string> >( "dumpOnly" );
        }
        
        if( cfg.existsAs<bool >( "binnedOnly" ) ) {
            binnedOnly_ = cfg.getParameter<bool >( "binnedOnly" );
        }
        if( cfg.existsAs<bool >( "unbinnedSystematics" ) ) {
            unbinnedSystematics_ = cfg.getParameter<bool >( "unbinnedSystematics" );
        }
        if( cfg.existsAs<bool >( "dumpGenWeight" ) ) {
            dumpGenWeight_ = cfg.getParameter<bool >( "dumpGenWeight" );
        }
        if( cfg.existsAs<bool >( "dumpPdfWeights" ) ) {
            dumpPdfWeights_ = cfg.getParameter<bool >( "dumpPdfWeights" );
            //            if ( dumpPdfWeights_ ) {
                //                std::cout << " Created category dumper with dumpPdfWeights true! " << std::endl;
            //            }
        }
        if( cfg.existsAs<int >( "nPdfWeights" ) ) {
            nPdfWeights_ = cfg.getParameter<int >( "nPdfWeights" );
        }
        if( cfg.existsAs<int >( "nAlphaSWeights" ) ) {
            nAlphaSWeights_ = cfg.getParameter<int >( "nAlphaSWeights" );
        }
        if( cfg.existsAs<int >( "nScaleWeights" ) ) {
            nScaleWeights_ = cfg.getParameter<int >( "nScaleWeights" );
        }
        if ( cfg.existsAs<bool>( "splitPdfByStage0Bin") ) {
            splitPdfByStage0Bin_ = cfg.getParameter<bool>( "splitPdfByStage0Bin" );
        }
        if ( cfg.existsAs<bool>( "splitPdfByStage1Bin") ) {
            splitPdfByStage1Bin_ = cfg.getParameter<bool>( "splitPdfByStage1Bin" );
        }
        if ( splitPdfByStage0Bin_ && splitPdfByStage1Bin_ ) {
            throw cms::Exception( "Configuration" ) << " Set to split PDF by both stage 0 and stage 1 bin" << std::endl;
        }
        auto variables = cfg.getParameter<vector<edm::ParameterSet> >( "variables" );
        for( auto &var : variables ) {
            auto nbins = var.getUntrackedParameter<int>( "nbins", 0 );
            //            auto vmin = var.getUntrackedParameter<double>( "vmin", numeric_limits<double>::lowest() );
            //            auto vmax = var.getUntrackedParameter<double>( "vmax", numeric_limits<double>::max() );
            auto vmin = var.getUntrackedParameter<double>( "vmin", -99. );
            auto vmax = var.getUntrackedParameter<double>( "vmax", 99. );
            auto binning = var.getUntrackedParameter<std::vector<double > >("binning", std::vector<double >());
            //            vector<double > binning;
            if( var.existsAs<edm::ParameterSet>( "expr" ) ) {
                auto expr = var.getParameter<edm::ParameterSet>( "expr" );
                auto name = var.getUntrackedParameter<string>( "name" );
                stepwise_functors_.push_back( std::shared_ptr<wrapped_stepwise_functor_type>( new wrapped_stepwise_functor_type( new stepwise_functor_type( expr ) ) ) );
                names_.push_back( name );
                variables_.push_back( make_tuple( 0., stepwise_functors_.back(), nbins, vmin, vmax, binning ) );
            } else {
                auto expr = var.getParameter<string>( "expr" );
                auto name = var.getUntrackedParameter<string>( "name", expr );
                functors_.push_back( std::shared_ptr<wrapped_functor_type>( new wrapped_functor_type( new functor_type( expr ) ) ) );
                names_.push_back( name );
                variables_.push_back( make_tuple( 0., functors_.back(), nbins, vmin, vmax, binning ) );
            }
        }

        if( cfg.existsAs<vector<edm::ParameterSet> >( "mvas" ) ) {
            auto mvas = cfg.getParameter<vector<edm::ParameterSet> >( "mvas" );
            for( auto &mva : mvas ) {
                auto name = mva.getUntrackedParameter<string>( "name" );
                auto nbins = mva.getUntrackedParameter<int>( "nbins", 0 );
                auto vmin = mva.getUntrackedParameter<double>( "vmin", numeric_limits<double>::lowest() );
                auto vmax = mva.getUntrackedParameter<double>( "vmax", numeric_limits<double>::max() );
                vector<double > binning;
                mvas_.push_back( std::shared_ptr<wrapped_mva_type>( new wrapped_mva_type( new mva_type( mva, globalVarsDumper_ ) ) ) );
                names_.push_back( name );
                variables_.push_back( make_tuple( 0., mvas_.back(), nbins, vmin, vmax, binning ) );
            }
        }


        //##########
        auto globalExtraFloatNames = globalVarsDumper_->getExtraFloatNames();
        for( auto &extraFloatName : globalExtraFloatNames ) {
            //            std::cout<<"adding wrapper for extra float variable "<<extraFloatName<<std::endl; 
            //            auto name = mva.getUntrackedParameter<string>( "name" );
            auto nbins = globalVarsDumper_->getExtraFloatNBin(extraFloatName) ;//100;
            auto vmin =   globalVarsDumper_->getExtraFloatVmin(extraFloatName) ;//numeric_limits<double>::lowest();
            auto vmax =   globalVarsDumper_->getExtraFloatVmax(extraFloatName) ;//numeric_limits<double>::max();   
            auto binning = globalVarsDumper_->getExtraFloatBinning(extraFloatName);
            extraglobalvars_.push_back( std::shared_ptr<wrapped_global_var_type>( new wrapped_global_var_type( globalVarsDumper_ , extraFloatName ) ) );
            names_.push_back( extraFloatName );
            variables_.push_back( make_tuple( 0., extraglobalvars_.back(), nbins, vmin, vmax, binning ) );
            //            variables_.push_back( make_tuple( 0., extraglobalvars_.back()) );
        }
        //##########


        auto histograms = cfg.getParameter<vector<edm::ParameterSet> >( "histograms" );
        for( auto &histo : histograms ) {
            auto name = histo.getUntrackedParameter<string>( "name" );
            auto xname = histo.getUntrackedParameter<string>( "x" );
            auto yname = histo.getUntrackedParameter<string>( "y", "" );
            auto nxbins = histo.getUntrackedParameter<int>( "nxbins", 0 );
            auto nybins = histo.getUntrackedParameter<int>( "nybins", 0 );
            vector<double> xbins, ybins;
            auto pos = xname.find( "global." );
            int xindex = -1;
            if( pos == 0 ) {
                xindex = -globalVarsDumper_->indexOf( xname.substr( 7 ) ) - 2;
            } else {
                xindex = find( names_.begin(), names_.end(), xname ) - names_.begin(); // FIXME: check validity
            }
            int yindex = -1;
            if( ! yname.empty() ) {
                pos = yname.find( "global." );
                if( pos == 0 ) {
                    yindex = -globalVarsDumper_->indexOf( yname.substr( 7 ) ) - 2;
                } else {
                    yindex = find( names_.begin(), names_.end(), yname ) - names_.begin(); // FIXME: check validity
                }
            }
            if( find( histograms_.begin(), histograms_.end(), name ) != histograms_.end() ) {
                histograms_.push_back( make_tuple( name, xindex, xbins, yindex, ybins, ( TH1 * )0 ) );
                continue;
            }
            if( nxbins <= 0 ) {
                xbins = histo.getUntrackedParameter<vector<double> >( "xbins" );
            } else {
                auto xmin = histo.getUntrackedParameter<double>( "xmin" );
                auto xmax = histo.getUntrackedParameter<double>( "xmax" );
                auto step = ( xmax - xmin ) / ( double )nxbins;
                for( auto bound = xmin; bound <= xmax; bound += step ) {
                    xbins.push_back( bound );
                }
            }
            if( ! yname.empty() ) {
                if( nybins <= 0 ) {
                    ybins = histo.getUntrackedParameter<vector<double> >( "ybins" );
                } else {
                    auto ymin = histo.getUntrackedParameter<double>( "ymin" );
                    auto ymax = histo.getUntrackedParameter<double>( "ymax" );
                    auto step = ( ymax - ymin ) / ( double )nybins;
                    for( auto bound = ymin; bound <= ymax; bound += step ) {
                        ybins.push_back( bound );
                    }
                }
            }
            histograms_.push_back( make_tuple( name, xindex, xbins, yindex, ybins, ( TH1 * )0 ) );
        }
    }

    template<class F, class O>
    CategoryDumper<F, O>::~CategoryDumper()
    {}

    template<class F, class O>
    void CategoryDumper<F, O>::bookHistos( TFileDirectory &fs, const std::map<std::string, std::string> &replacements )
    {
        using namespace std;
        for( auto &histo : histograms_ ) {
            auto name = formatString( name_ + std::get<0>( histo ), replacements );
            auto &xbins = std::get<2>( histo );
            auto &th1 = std::get<5>( histo );
            try {
                th1 = fs.getObject<TH1>( name );
            } catch( ... ) {
                if( std::get<3>( histo ) >= 0 ) {
                    auto &ybins = std::get<4>( histo );
                    th1 = fs.make<TH2F>( name.c_str(), name.c_str(), xbins.size() - 1, &xbins[0], ybins.size() - 1, &ybins[0] );
                } else {
                    th1 = fs.make<TH1F>( name.c_str(), name.c_str(), xbins.size() - 1, &xbins[0] );
                }
                th1->Sumw2( true );
            }
        }
        hbooked_ = true;
    }

    template<class F, class O>
    void CategoryDumper<F, O>::bookTree( TFileDirectory &fs, const char *weightName, const std::map<std::string, std::string> &replacements )
    {   
        // cout << "[in Taggers/interface/CategoryDumper.h - bookTree] - Tree is getting booked" << endl;
        tree_ = fs.make<TTree>( formatString( name_, replacements ).c_str(), formatString( name_, replacements ).c_str() );
        tree_->Branch( "candidate_id", &n_cand_, "candidate_id/I" );
        tree_->Branch( weightName, &weight_ );
        if( dumpGenWeight_ ) {
          tree_->Branch("genweight" , &genweight_ );
        }
        for( size_t iv = 0; iv < names_.size(); ++iv ) {
            if( ! dumpOnly_.empty() && find( dumpOnly_.begin(), dumpOnly_.end(), names_[iv] ) == dumpOnly_.end() ) { continue; }
            tree_->Branch( names_[iv].c_str(), &std::get<0>( variables_[iv] ) );
        }
        if( globalVarsDumper_ ) {
            globalVarsDumper_->bookTreeVariables( tree_, replacements );
        }
        if( dumpPdfWeights_ ) {
            variables_pdfWeights_.resize(nPdfWeights_+nAlphaSWeights_+nScaleWeights_);
            if( nPdfWeights_ > 0 ) {
                tree_->Branch( "pdfWeights", &variables_pdfWeights_[0], Form("pdfWeights[%d]/F",nPdfWeights_) );
            }
            if( nAlphaSWeights_ > 0 ) {
                tree_->Branch( "alphaSWeights", &variables_pdfWeights_[nPdfWeights_], Form("alphaSWeights[%d]/F",nAlphaSWeights_) );
            }
            if( nScaleWeights_ > 0 ) {
                tree_->Branch( "scaleWeights", &variables_pdfWeights_[nPdfWeights_+nAlphaSWeights_], Form("scaleWeights[%d]/F",nScaleWeights_) );
            }
        }
    }
    
    template<class F, class O>
    void CategoryDumper<F, O>::compressPdfWeightDatasets( RooWorkspace * ws)
    {
        const char * weight_central = "centralObjectWeight";
        RooRealVar * sumW = new RooRealVar("sumW","sumW",0);
        RooRealVar * numW = new RooRealVar("numW","numW",0);
        //        std::cout << " Calling CategoryDumper<F, O>::compressPdfWeightDatasets " << std::endl;
        //        std::cout << " sumW=" << dataset_pdfWeights_->sumEntries() << " numW=" << dataset_pdfWeights_->numEntries() << std::endl;
        //        rooVars_pdfWeights_.Print("v");
        rooVars_pdfWeights_.add(*sumW);
        rooVars_pdfWeights_.add(*numW);
        if (dataset_pdfWeights_){
            bool hasCentralWeight = dataset_pdfWeights_->get()->find(weight_central) != 0;
            RooArgSet args; args.add( *(dataset_pdfWeights_->get()->selectByName("*Weight*")) );
            if ( splitPdfByStage0Bin_ || splitPdfByStage1Bin_ ) {
                map <int,double> sumwei;
                map <int,double> sumn;
                map <string,map<int,double> > avgwei;
                for(int i=0; i< dataset_pdfWeights_->numEntries() ; i++){
                    auto * ivars = dataset_pdfWeights_->get(i);
                    float w_nominal =dataset_pdfWeights_->weight();
                    int htxsBin; 
                    if (splitPdfByStage1Bin_) {
                        htxsBin = (int)( ivars->getRealValue("stage1p2bin") + 0.01 );
                    } else {
                        htxsBin = (int)( ivars->getRealValue("stage0bin") + 0.01 );
                    }
                    if (sumwei.count( htxsBin ) ) {
                        sumwei[htxsBin] += w_nominal;
                        sumn[htxsBin] += 1;
                    } else {
                        sumwei[htxsBin] = w_nominal;
                        sumn[htxsBin] = 1;
                    }
                    float w_central = ( hasCentralWeight ? ivars->getRealValue(weight_central) : 1. );
                    if (w_central == 0.) continue;
                    TIterator * iter = args.createIterator();
                    RooRealVar *var;
                    while((var=(RooRealVar*)iter->Next())) {
                        float w_up = ivars->getRealValue(var->GetName());
                        string sname = string(var->GetName());
                        if (! avgwei.count(sname) ) {
                            avgwei[sname] = map<int,double>();
                        }
                        if ( avgwei[sname].count(htxsBin) ) {
                            avgwei[sname][htxsBin] += w_nominal*(w_up/w_central);
                        } else {
                            avgwei[sname][htxsBin] = w_nominal*(w_up/w_central);
                        }
                    }
                }
                for ( auto it1 = avgwei.begin() ; it1 != avgwei.end() ; it1++ ) {
                    for ( auto it2 = it1->second.begin() ; it2 != it1->second.end() ; it2++ ) {
                        avgwei[it1->first][it2->first] = it2->second / sumwei[it2->first];
                    }
                }
                RooDataSet newdset( dataset_pdfWeights_->GetName(), dataset_pdfWeights_->GetName(), rooVars_pdfWeights_, sumW->GetName());
                for ( auto it2 = sumwei.begin() ; it2 != sumwei.end() ; it2++ ) {
                    for ( auto it1 = avgwei.begin() ; it1 != avgwei.end() ; it1++ ) {
                        dynamic_cast<RooRealVar &>( rooVars_pdfWeights_[it1->first.c_str()] ).setVal( avgwei[it1->first][it2->first] );
                    }
                    if ( splitPdfByStage0Bin_ ) {
                        dynamic_cast<RooRealVar &>( rooVars_pdfWeights_["stage0bin"] ).setVal( it2->first );
                    } else {
                        dynamic_cast<RooRealVar &>( rooVars_pdfWeights_["stage1p2bin"] ).setVal( it2->first );
                    }
                    newdset.add(rooVars_pdfWeights_,it2->second);
                }
                ws->import(newdset);
            } else {
                sumW->setVal(dataset_pdfWeights_->sumEntries());
                numW->setVal(dataset_pdfWeights_->numEntries());
                TIterator * iter = args.createIterator();
                RooRealVar *var;
                while((var=(RooRealVar*)iter->Next())) {
                    double avgwei = 0.;
                    for(int i=0; i< dataset_pdfWeights_->numEntries() ; i++){ // this is rather suboptimal as it loops several times over the dataset
                        auto * ivars = dataset_pdfWeights_->get(i);
                        float w_nominal =dataset_pdfWeights_->weight();
                        float w_up = ivars->getRealValue(var->GetName());
                        float w_central = ( hasCentralWeight ? ivars->getRealValue(weight_central) : 1. );
                        if (w_central == 0.) { continue; }
                        avgwei += w_nominal*(w_up/w_central);
                    }
                    avgwei /= sumW->getVal();
                    dynamic_cast<RooRealVar &>( rooVars_pdfWeights_[var->GetName()] ).setVal(avgwei);
                }
                RooDataSet newdset( dataset_pdfWeights_->GetName(), dataset_pdfWeights_->GetName(), rooVars_pdfWeights_, sumW->GetName());
                newdset.add(rooVars_pdfWeights_,sumW->getVal());
                ws->import(newdset);
            }
            delete dataset_pdfWeights_;
            dataset_pdfWeights_ = 0;
        } else {
            std::cout << "[ERROR], no pdfweight dataset to compress!!" << std::endl;
        }        
    }

    template<class F, class O>
    void CategoryDumper<F, O>::bookRooDataset( RooWorkspace &ws, const char *weightVar, const std::map<std::string, std::string> &replacements )
{
    if( ! binnedOnly_ ) {
        rooVars_.add( *ws.var( weightVar ) );
    }

    RooArgSet  rooVars_pdfWeights0 ;
    
    for( size_t iv = 0; iv < names_.size(); ++iv ) {
        auto &name = names_[iv];
        auto &var = variables_[iv];
        auto &nbins = std::get<2>( var );
        auto &vmin = std::get<3>( var );
        auto &vmax = std::get<4>( var );
        auto &binning = std::get<5>( var );
        RooRealVar &rooVar = dynamic_cast<RooRealVar &>( *ws.factory( Form( "%s[0.]", name.c_str() ) ) );
        //        std::cout << " after factory for " << name << std::endl;
        rooVar.setConstant( false );
        if(binnedOnly_ && (nbins==0)){
            //            throw cms::Exception( "Dumper Binning" ) << "One or more variable which is to be dumped in a RooDataHist has not been allocated any binning options. Please specify these in your dumper configuration using the format variable[nBins,min,max] := variable definition ";
            nbins=1;
        }
        if(!unbinnedSystematics_){
            if( nbins > 0 ) { 
                rooVar.setBins( nbins );
            }
            if(nbins == -1){
                rooVar.setMin( binning.at(0)  );
                rooVar.setMax( binning.at(binning.size()-1) );
                //            RooBinning* rooBinning = new RooBinning(int(binning.size()), &binning[0]);
                //            rooVar.setBinning(*rooBinning);
                RooBinning* rooBinning = new RooBinning(binning.at(0), binning.at(binning.size()-1));
                for(int ib =1; ib< (int)binning.size()-1; ib++){
                    rooBinning->addBoundary(binning.at(ib));
                    rooBinning->Print();
                }
                rooVar.setBinning(*rooBinning);
                rooVar.Print("v");
            }
        }
        else{
            rooVar.setMin( vmin );
            rooVar.setMax( vmax );
        }
        

        rooVars_.add( rooVar, true );
        rooVars_pdfWeights0.add( rooVar, true );
    }

    if (dumpPdfWeights_){
        for (int i =0; i< nPdfWeights_ ; i++) {
            rooVars_pdfWeights0.add( *ws.var( Form("pdfWeight_%d",i) ) );
        }
        for (int i =0; i< nAlphaSWeights_ ; i++) {
            rooVars_pdfWeights0.add( *ws.var( Form("alphaSWeight_%d",i) ) );
        }
        for (int i =0; i< nScaleWeights_ ; i++) {
            rooVars_.add( *ws.var( Form("scaleWeight_%d",i) ) ); // scale Weights need to be included in main dataset to be used correctly downstream. i think...
            rooVars_pdfWeights0.add( *ws.var( Form("scaleWeight_%d",i) ) ); // maybe do both and check we get same result ??
        }
        if ( splitPdfByStage0Bin_ ) {
            rooVars_pdfWeights0.add( *ws.var( "stage0bin" ) );
        }
        if ( splitPdfByStage1Bin_ ) {
            rooVars_pdfWeights0.add( *ws.var( "stage1p2bin" ) );
        }
    }

    rooVars_pdfWeights_.add(*((RooArgSet*) rooVars_pdfWeights0.selectByName("central*")),true);
    rooVars_pdfWeights_.add(*((RooArgSet*) rooVars_pdfWeights0.selectByName("pdf*,scale*,alpha*")),true); // eveuntally could remove scale... as it might not be useful to collapse it like other pdf weights
    if ( splitPdfByStage1Bin_ ) { 
        rooVars_pdfWeights_.add(*((RooArgSet*) rooVars_pdfWeights0.selectByName("stage1p2bin")),true);
    }
    else { 
        rooVars_pdfWeights_.add(*((RooArgSet*) rooVars_pdfWeights0.selectByName("stage0bin")),true);
    }
    rooVars_pdfWeights_.add(*ws.var( weightVar ),true);
    
    std::string dsetName = formatString( name_, replacements );
    if( ! binnedOnly_ || unbinnedSystematics_) {
        RooDataSet dset( dsetName.c_str(), dsetName.c_str(), rooVars_, weightVar );
        ws.import( dset );
    } else {
        RooDataHist dhist(dsetName.c_str(),dsetName.c_str(),rooVars_);
        ws.import( dhist );
    }
    dataset_ = ws.data( dsetName.c_str() );

    if( dumpPdfWeights_ ) {
        RooDataSet * dset_pdfWeights = new RooDataSet( (dsetName+"_pdfWeights").c_str(), (dsetName+"_pdfWeights").c_str(), rooVars_pdfWeights_, weightVar ); // store in separate RooDataSet where we store all PDF weights as one. (because including it as a refular weight is too heavy and causes crashes)
        // the compression of the entries in dset_pdfWeights into one event per dataset happens later.
        dataset_pdfWeights_ = dset_pdfWeights;
    }

}

    template<class F, class O>
string CategoryDumper<F, O>::GetName( )
{   
    return name_ ;
}

    template<class F, class O>
bool CategoryDumper<F, O>::isBinnedOnly( )
{   
    return binnedOnly_ ;
}

    template<class F, class O>
    void CategoryDumper<F, O>::fill( const object_type &obj, double weight, vector<double> pdfWeights, int n_cand, int htxsBin, double genweight)
{  
    // cout << "[in Taggers/interface/CategoryDumper.h - CategoryDumper<F, O>::fill]" << endl;
    n_cand_ = n_cand;
    weight_ = weight;
    genweight_ = genweight;
    if( dataset_ && (!binnedOnly_) ) {
        dynamic_cast<RooRealVar &>( rooVars_["weight"] ).setVal( weight_ );
    }
    if (dumpPdfWeights_){
        if( tree_ ) {
            std::copy(pdfWeights.begin(),pdfWeights.end(),variables_pdfWeights_.begin());
        }
        if( dataset_pdfWeights_ ) {
            dynamic_cast<RooRealVar &>( rooVars_pdfWeights_["weight"] ).setVal( weight_ );
            if ((nPdfWeights_+ nAlphaSWeights_ + nScaleWeights_) != (int) (pdfWeights.size())){ 
                throw cms::Exception( "Configuration" ) << " Specified number of pdfWeights (" << nPdfWeights_ <<") plus alphaSWeights ("<<nAlphaSWeights_
                                                        <<") plus scaleWeights (" << nScaleWeights_ << ") does not match length of pdfWeights Vector ("
                                                        << pdfWeights.size() << ")." ;
            }
            for ( int i =0; i< nPdfWeights_; i++) {
                dynamic_cast<RooRealVar &>( rooVars_pdfWeights_[Form("pdfWeight_%d",i)] ).setVal( pdfWeights[i] );
            }
            for ( int i =0; i<nAlphaSWeights_  ; i++) {
                dynamic_cast<RooRealVar &>( rooVars_pdfWeights_[Form("alphaSWeight_%d",i)] ).setVal( pdfWeights[i+nPdfWeights_] ); // alpha S weights currently stored at the end of pdfweight vector 
            }
            for ( int i =0; i< nScaleWeights_; i++) {
                dynamic_cast<RooRealVar &>( rooVars_pdfWeights_[Form("scaleWeight_%d",i)] ).setVal( pdfWeights[i+nPdfWeights_+nAlphaSWeights_] ); // and scale weights stored after that!
                dynamic_cast<RooRealVar &>( rooVars_[Form("scaleWeight_%d",i)] ).setVal( pdfWeights[i+nPdfWeights_+nAlphaSWeights_] ); // and scale weights stored after that!
            }
            if ( splitPdfByStage0Bin_ && htxsBin > -1 ) {
                dynamic_cast<RooRealVar &>( rooVars_pdfWeights_["stage0bin"]).setVal( htxsBin );
            }
            if ( splitPdfByStage1Bin_ && htxsBin > -1 ) {
                dynamic_cast<RooRealVar &>( rooVars_pdfWeights_["stage1p2bin"]).setVal( htxsBin );
            }
        }
    }
    
    for( size_t ivar = 0; ivar < names_.size(); ++ivar ) {
        auto name = names_[ivar].c_str();
        auto &var = variables_[ivar];
        auto &val = std::get<0>( var );
        val = ( *std::get<1>( var ) )( obj );
        if( dataset_ ) {
            dynamic_cast<RooRealVar &>( rooVars_[name] ).setVal( val );
            if (dumpPdfWeights_) {
                if( rooVars_pdfWeights_.find(name) != 0 ) {
                    if ( val == 0. ) { std::cout << " WARNING we have a weight 0 that we're pushing back into rooVars_pdfWeights_[ " << name << " ] " << std::endl; }
                    dynamic_cast<RooRealVar &>( rooVars_pdfWeights_[name] ).setVal( val ); 
                }
            }
        }
    }
    if( tree_ ) tree_->Fill(); 
        // Debugging HHWWgg 
        // cout << "[in Taggers/interface/CategoryDumper.h] - Filling tree " << endl;
        // cout << "[in Taggers/interface/CategoryDumper.h] - tree_ = " << tree_ << endl;
        
        
    if( dataset_ ) {
        dataset_->add( rooVars_, weight_ );
        if (dumpPdfWeights_ && dataset_pdfWeights_) {
            dataset_pdfWeights_->add( rooVars_pdfWeights_, weight_ );
        }
    }
    if( hbooked_ ) {
        for( auto &histo : histograms_ ) {
            auto &th1 = *std::get<5>( histo );
            auto xv = std::get<1>( histo );
            auto yv = std::get<3>( histo );
            float xval = ( xv >= 0 ? std::get<0>( variables_[xv] ) : globalVarsDumper_->valueOf( -xv - 2 ) );
            if( yv != -1 ) {
                float yval = ( yv >= 0 ? std::get<0>( variables_[yv] ) : globalVarsDumper_->valueOf( -yv - 2 ) );
                // dynamic_cast<TH2 &>( th1 ).Fill( std::get<0>( variables_[xv] ), std::get<0>( variables_[yv] ), weight_ );
                dynamic_cast<TH2 &>( th1 ).Fill( xval, yval, weight_ );
            } else {
                /// th1.Fill( std::get<0>( variables_[xv] ), weight_ );
                th1.Fill( xval, weight_ );
            }
        }
    }
}

}

inline bool operator==( const flashgg::histo_info &lh, const std::string &rh ) { return std::get<0>( lh ) == rh; };
inline bool operator==( const std::string &lh, const flashgg::histo_info &rh ) { return lh == std::get<0>( rh ); };


#endif  // flashgg_CategoryDumper_h
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

