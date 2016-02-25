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
        typedef FunctorWrapper<object_type, mva_type> wrapped_mva_type;

        CategoryDumper( const std::string &name, const edm::ParameterSet &cfg, GlobalVariablesDumper *dumper = 0 );
        ~CategoryDumper();

        void bookHistos( TFileDirectory &fs, const std::map<std::string, std::string> &replacements );
        void bookTree( TFileDirectory &fs, const char *weightVar, const std::map<std::string, std::string> &replacements );
        void bookRooDataset( RooWorkspace &ws, const char *weightVar, const std::map<std::string, std::string> &replacements);
        void compressPdfWeightDatasets(RooWorkspace *ws);
        

        void fill( const object_type &obj, double weight, vector<double>, int n_cand = 0);
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
        std::vector<std::tuple<float, std::shared_ptr<trait_type>, int, double, double> > variables_;
        std::vector<histo_info> histograms_;

        int n_cand_;
        float weight_;
        RooArgSet rooVars_;
        RooArgSet rooVars_pdfWeights_;
        RooArgSet rooVarsBinned_;
        RooAbsData *dataset_;
        RooAbsData *dataset_pdfWeights_;
        TTree *tree_;
        GlobalVariablesDumper *globalVarsDumper_;
        std::vector<std::shared_ptr<wrapped_mva_type> > mvas_;
        std::vector<std::shared_ptr<wrapped_functor_type> > functors_;
        std::vector<std::shared_ptr<wrapped_stepwise_functor_type> > stepwise_functors_;
        bool hbooked_;
        bool binnedOnly_;
        bool dumpPdfWeights_;
        int  nPdfWeights_;
        int  nAlphaSWeights_;
        int nScaleWeights_;
        bool pdfVarsAdded_;
        RooWorkspace * ws_;
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
        nPdfWeights_ (0), 
        nAlphaSWeights_(0),
        nScaleWeights_(0)
    {
        using namespace std;
        name_ = name;

        if( cfg.existsAs<vector<string> >( "dumpOnly" ) ) {
            dumpOnly_ = cfg.getParameter<vector<string> >( "dumpOnly" );
        }
        
        if( cfg.existsAs<bool >( "binnedOnly" ) ) {
            binnedOnly_ = cfg.getParameter<bool >( "binnedOnly" );
        }
        if( cfg.existsAs<bool >( "dumpPdfWeights" ) ) {
            dumpPdfWeights_ = cfg.getParameter<bool >( "dumpPdfWeights" );
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
        auto variables = cfg.getParameter<vector<edm::ParameterSet> >( "variables" );
        for( auto &var : variables ) {
            auto nbins = var.getUntrackedParameter<int>( "nbins", 0 );
            auto vmin = var.getUntrackedParameter<double>( "vmin", numeric_limits<double>::min() );
            auto vmax = var.getUntrackedParameter<double>( "vmax", numeric_limits<double>::max() );
            if( var.existsAs<edm::ParameterSet>( "expr" ) ) {
                auto expr = var.getParameter<edm::ParameterSet>( "expr" );
                auto name = var.getUntrackedParameter<string>( "name" );
                stepwise_functors_.push_back( std::shared_ptr<wrapped_stepwise_functor_type>( new wrapped_stepwise_functor_type( new stepwise_functor_type( expr ) ) ) );
                names_.push_back( name );
                variables_.push_back( make_tuple( 0., stepwise_functors_.back(), nbins, vmin, vmax ) );
            } else {
                auto expr = var.getParameter<string>( "expr" );
                auto name = var.getUntrackedParameter<string>( "name", expr );
                functors_.push_back( std::shared_ptr<wrapped_functor_type>( new wrapped_functor_type( new functor_type( expr ) ) ) );
                names_.push_back( name );
                variables_.push_back( make_tuple( 0., functors_.back(), nbins, vmin, vmax ) );
            }
        }

        if( cfg.existsAs<vector<edm::ParameterSet> >( "mvas" ) ) {
            auto mvas = cfg.getParameter<vector<edm::ParameterSet> >( "mvas" );
            for( auto &mva : mvas ) {
                auto name = mva.getUntrackedParameter<string>( "name" );
                auto nbins = mva.getUntrackedParameter<int>( "nbins", 0 );
                auto vmin = mva.getUntrackedParameter<double>( "vmin", numeric_limits<double>::min() );
                auto vmax = mva.getUntrackedParameter<double>( "vmax", numeric_limits<double>::max() );
                mvas_.push_back( std::shared_ptr<wrapped_mva_type>( new wrapped_mva_type( new mva_type( mva, globalVarsDumper_ ) ) ) );
                names_.push_back( name );
                variables_.push_back( make_tuple( 0., mvas_.back(), nbins, vmin, vmax ) );
            }
        }

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
        tree_ = fs.make<TTree>( formatString( name_, replacements ).c_str(), formatString( name_, replacements ).c_str() );
        tree_->Branch( "candidate_id", &n_cand_, "candidate_id/I" );
        tree_->Branch( weightName, &weight_ );
        for( size_t iv = 0; iv < names_.size(); ++iv ) {
            if( ! dumpOnly_.empty() && find( dumpOnly_.begin(), dumpOnly_.end(), names_[iv] ) == dumpOnly_.end() ) { continue; }
            tree_->Branch( names_[iv].c_str(), &std::get<0>( variables_[iv] ) );
        }
        if( globalVarsDumper_ ) {
            globalVarsDumper_->bookTreeVariables( tree_, replacements );
        }
    }
    
    template<class F, class O>
    void CategoryDumper<F, O>::compressPdfWeightDatasets( RooWorkspace * ws)
    {

          RooRealVar * mass =  ws->var("CMS_hgg_mass");
          RooRealVar * weight_central = ws->var("centralObjectWeight");
          RooRealVar * weight = new RooRealVar("weight","weight",0);
          RooRealVar * sumW = new RooRealVar("sumW","sumW",0);
          RooRealVar * numW = new RooRealVar("numW","numW",0);
          if (dataset_pdfWeights_){
            RooArgSet args = *(dataset_pdfWeights_->get());
            TIterator * iter = args.createIterator();
            sumW->setVal(dataset_pdfWeights_->sumEntries());
            numW->setVal(dataset_pdfWeights_->numEntries());
            RooRealVar *var;
            while((var=(RooRealVar*)iter->Next())) {
              if (var) {
                TString datasetname(var->GetName());
                TString query("mass");
                if (datasetname.Contains(query)) continue;
                RooDataSet *data_temp = new RooDataSet("temp","tempt",RooArgSet(*mass,*weight),weight->GetName());
                for(int i=0; i< dataset_pdfWeights_->numEntries() ; i++){
                  mass->setVal(dataset_pdfWeights_->get(i)->getRealValue("CMS_hgg_mass"));
                  float w_nominal =dataset_pdfWeights_->weight();
                  float w_up = dataset_pdfWeights_->get(i)->getRealValue(var->GetName());
                  float w_central = dataset_pdfWeights_->get(i)->getRealValue(weight_central->GetName());
                  if (w_central == 0.) continue;
                  var->setVal(w_nominal*(w_up/w_central));
                  weight->setVal( var->getVal());
                  data_temp->add(RooArgSet(*mass,*weight),var->getVal());
                }
                float final_result =data_temp->sumEntries()/dataset_pdfWeights_->sumEntries();
                dynamic_cast<RooRealVar &>(  rooVars_pdfWeights_[var->GetName()]).setVal(final_result);
              }
            }
            rooVars_pdfWeights_.add(*sumW);
            rooVars_pdfWeights_.add(*numW);
            RooDataSet newdset( dataset_pdfWeights_->GetName(), dataset_pdfWeights_->GetName(), rooVars_pdfWeights_, sumW->GetName());
            newdset.add(rooVars_pdfWeights_,sumW->getVal());
            ws->import(newdset);
          } else {
            std::cout << "[ERROR], no pdfweight dataset to compress!!" << std::endl;
          }
    }

    template<class F, class O>
    void CategoryDumper<F, O>::bookRooDataset( RooWorkspace &ws, const char *weightVar, const std::map<std::string, std::string> &replacements )
{
    rooVars_.add( *ws.var( weightVar ) );
    ws_= &ws;
    RooArgSet allvars0 = ws.allVars();
    RooArgSet allvars1 = ws_->allVars();
    RooArgSet  rooVars_pdfWeights0 ;
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
    }

    for( size_t iv = 0; iv < names_.size(); ++iv ) {
        auto &name = names_[iv];
        auto &var = variables_[iv];
        auto &nbins = std::get<2>( var );
        auto &vmin = std::get<3>( var );
        auto &vmax = std::get<4>( var );
        RooRealVar &rooVar = dynamic_cast<RooRealVar &>( *ws.factory( Form( "%s[0.]", name.c_str() ) ) );
        rooVar.setConstant( false );
        if(binnedOnly_ && (nbins==0)){
            throw cms::Exception( "Dumper Binning" ) << "One or more variable which is to be dumped in a RooDataHist has not been allocated any binning options. Please specify these in your dumper configuration using the format variable[nBins,min,max] := variable definition ";
        }
        if( nbins >= 0 ) { 
            rooVar.setMin( vmin );
            rooVar.setMax( vmax );
            rooVar.setBins( nbins );
        }
        rooVars_.add( rooVar, true );
        rooVars_pdfWeights0.add( rooVar, true );
        rooVarsBinned_.add( rooVar, true ); //all the same vars except weight
    }
    // globalVarsDumper_.bookRooVars(ws,rooVars_,replacements); 
    //rooVars_pdfWeights_ =  *((RooArgSet*) rooVars_pdfWeights0.selectByName("CMS_hgg_mass,central*,pdf*,alpha*")); // ok, maybe not the best way to do this. To be improved. scale is kept in main rooVars_
    //rooVars_pdfWeights_ =  *((RooArgSet*) rooVars_pdfWeights0.selectByName("CMS_hgg_mass,central*,pdf*,scale*,alpha*")); // ok, maybe not the best way to do this. To be improved.
    rooVars_pdfWeights_.add(*((RooArgSet*) rooVars_pdfWeights0.selectByName("CMS_hgg_mass")),true);
    rooVars_pdfWeights_.add(*((RooArgSet*) rooVars_pdfWeights0.selectByName("central*")),true);
    rooVars_pdfWeights_.add(*((RooArgSet*) rooVars_pdfWeights0.selectByName("pdf*,scale*,alpha*")),true); // eveuntally could renove scale... as it might not be useful to collapse it liek other pdf weights
    rooVars_pdfWeights_.add(*ws.var( weightVar ),true);
    RooDataSet dset( formatString( name_, replacements ).c_str(), formatString( name_, replacements ).c_str(), rooVars_, weightVar );

    RooDataHist dhist(formatString(name_,replacements).c_str(),formatString(name_,replacements).c_str(),rooVarsBinned_,dset);
    if( ! binnedOnly_ ) {
        ws.import( dset );
       //ws.import( dset_pdfWeights ); can't over-write datasets so only import this one at the end  once it has been collapsed
        RooDataSet * dset_pdfWeights = new RooDataSet( (formatString( name_, replacements )+"_pdfWeights").c_str(), (formatString( name_, replacements )+"_pdfWeights").c_str(), rooVars_pdfWeights_, weightVar ); // store in separate RooDataSet where we store all PDF weights as one. (because including it as a refular weight is too heavy and causes crashes)
    // the compressing of the entries in dset_pdfWeights into one event per dataset happens later.
        dataset_pdfWeights_ = dset_pdfWeights;
    }
    if( binnedOnly_ ) {
        ws.import( dhist );
    }

    dataset_ = ws.data( name_.c_str() );
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
void CategoryDumper<F, O>::fill( const object_type &obj, double weight, vector<double> pdfWeights, int n_cand)
{  
    n_cand_ = n_cand;
    weight_ = weight;
    if( dataset_ && (!binnedOnly_) ) {
        // don't fill RooDataHists with weight variable, only RooDatSets
        dynamic_cast<RooRealVar &>( rooVars_["weight"] ).setVal( weight_ );

        if (dumpPdfWeights_){
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
        }
    }
    for( size_t ivar = 0; ivar < names_.size(); ++ivar ) {
        auto name = names_[ivar].c_str();
        auto &var = variables_[ivar];
        auto &val = std::get<0>( var );
        val = ( *std::get<1>( var ) )( obj );
        if( dataset_ ) {
            if (!binnedOnly_)  dynamic_cast<RooRealVar &>( rooVars_[name] ).setVal( val );
            if (dumpPdfWeights_) {
            if (names_[ivar].compare("CMS_hgg_mass") ==0 || names_[ivar].compare("centralObjectWeight") ==0){ 
            // hard-coded for now, need to find a better way evenutally...
                if ( val == 0. ) {
                    std::cout << " WARNING we have a weight 0 that we're pushing back into rooVars_pdfWeights_[ " << name << " ] " << std::endl;
                }
              dynamic_cast<RooRealVar &>( rooVars_pdfWeights_[name] ).setVal( val ); 
             }
            }
            if (binnedOnly_)   dynamic_cast<RooRealVar &>( rooVarsBinned_[name] ).setVal( val );
        }
    }
    if( tree_ ) { tree_->Fill(); }
    if( dataset_ ) {
        if (!binnedOnly_) {
            dataset_->add( rooVars_, weight_ );
            dataset_pdfWeights_->add( rooVars_pdfWeights_, weight_ );
        }
        if (binnedOnly_)  dataset_->add( rooVarsBinned_, weight_ );
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

