#ifndef flashgg_CategoryDumper_h
#define flashgg_CategoryDumper_h

#include <tuple>
#include <vector>
#include <string>

#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooRealVar.h"

#include "CommonTools/Utils/interface/TFileDirectory.h"

#include "flashgg/TagAlgos/interface/StringHelpers.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

namespace flashgg {
	
	template<class FunctorT, class ObjectT>
	class CategoryDumper {
	public:
		typedef ObjectT object_type;
		typedef FunctorT functor_type;
		
		CategoryDumper(const std::string & name, const edm::ParameterSet & cfg);
		~CategoryDumper();
		
		void bookHistos(TFileDirectory &fs, const std::map<std::string,std::string> & replacements);
		void bookTree(TFileDirectory &fs, const char * weightVar, const std::map<std::string,std::string> & replacements);
		void bookRooDataset(RooWorkspace &ws, const char * weightVar, const std::map<std::string,std::string> & replacements);
		
		void fill(const object_type & obj, double weight);
		
	private:
		std::string name_;
		std::vector<std::string> names_;
		std::vector<std::tuple<float,functor_type,int,double,double> > variables_;
		std::vector<std::tuple<std::string,int,std::vector<double>,int,std::vector<double>,TH1 *> > histograms_;
		
		float weight_;
		RooArgSet rooVars_;
		RooAbsData * dataset_;
		TTree * tree_;
	};
	
	template<class F, class O>
	CategoryDumper<F,O>::CategoryDumper(const std::string & name, const edm::ParameterSet & cfg):
		dataset_(0), tree_(0)
	{
		using namespace std;
		name_ = name;

		auto variables = cfg.getParameter<vector<edm::ParameterSet> >("variables");
		for( auto & var : variables ) {
			auto expr = var.getParameter<string>("expr");
			auto name = var.getUntrackedParameter<string>("name",expr);
			auto nbins = var.getUntrackedParameter<int>("nbins",0);
			auto vmin = var.getUntrackedParameter<double>("vmin",numeric_limits<double>::min());
			auto vmax = var.getUntrackedParameter<double>("vmax",numeric_limits<double>::max());
			names_.push_back(name);
			variables_.push_back( make_tuple(0.,functor_type(expr),nbins,vmin,vmax) );
		}
		
		auto histograms = cfg.getParameter<vector<edm::ParameterSet> >("histograms");
		for( auto & histo : histograms ) {
			auto name = histo.getUntrackedParameter<string>("name");
			auto xname = histo.getUntrackedParameter<string>("x");
			auto yname = histo.getUntrackedParameter<string>("y","");
			auto nxbins = histo.getUntrackedParameter<int>("nxbins",0);
			auto nybins = histo.getUntrackedParameter<int>("nybins",0);
			vector<double> xbins, ybins;
			int xindex = find( names_.begin(), names_.end(), xname ) - names_.begin(); // FIXME: check validity
			int yindex = -1;
			if( nxbins <= 0 ) { 
				xbins = histo.getUntrackedParameter<vector<double> >("xbins");
			} else {
				auto xmin = histo.getUntrackedParameter<double>("xmin");
				auto xmax = histo.getUntrackedParameter<double>("xmax");
				auto step = (xmax - xmin)/(double)nxbins;
				for(auto bound=xmin; bound<=xmax; bound+=step) {
					xbins.push_back(bound);
				}
			}
			if( ! yname.empty() ) {
				yindex = find( names_.begin(), names_.end(), yname ) - names_.begin(); // FIXME: check validity
				if( nybins <= 0 ) { 
					ybins = histo.getUntrackedParameter<vector<double> >("ybins");
				} else {
					auto ymin = histo.getUntrackedParameter<double>("ymin");
					auto ymax = histo.getUntrackedParameter<double>("ymax");
					auto step = (ymax - ymin)/(double)nybins;
					for(auto bound=ymin; bound<=ymax; bound+=step) {
						ybins.push_back(bound);
					}
				}
			}
			histograms_.push_back( make_tuple(name,xindex,xbins,yindex,ybins,(TH1*)0) );
		}
	}
	
	template<class F, class O>
	CategoryDumper<F,O>::~CategoryDumper()
	{}

	template<class F, class O>
	void CategoryDumper<F,O>::bookHistos(TFileDirectory &fs, const std::map<std::string,std::string> & replacements)
	{
		using namespace std;
		for( auto & histo : histograms_ ) {
			auto name = formatString(name_+get<0>(histo),replacements);
			auto & xbins = get<2>(histo);
			auto & th1 = get<5>(histo);
			if( get<3>(histo) >= 0 ) {
				auto & ybins = get<4>(histo);
				th1 = fs.make<TH2F>(name.c_str(),name.c_str(),xbins.size()-1,&xbins[0],ybins.size()-1,&ybins[0]);
			} else {
				th1 = fs.make<TH1F>(name.c_str(),name.c_str(),xbins.size()-1,&xbins[0]);
			}
		}
	}

	template<class F, class O>
	void CategoryDumper<F,O>::bookTree(TFileDirectory &fs, const char * weightName, const std::map<std::string,std::string> & replacements)
	{
		tree_ = fs.make<TTree>( formatString(name_,replacements).c_str(),formatString(name_,replacements).c_str() );
		tree_->Branch(weightName,&weight_);
		for(size_t iv=0; iv<names_.size(); ++iv) {
			tree_->Branch(names_[iv].c_str(),&std::get<0>(variables_[iv]));
		}
	}

	template<class F, class O>
	void CategoryDumper<F,O>::bookRooDataset(RooWorkspace &ws, const char * weightVar, const std::map<std::string,std::string> & replacements)
	{
		rooVars_.add( *ws.var(weightVar) );
		
		for(size_t iv=0; iv<names_.size(); ++iv) {
			auto & name = names_[iv];
			auto & var = variables_[iv];
			auto & nbins = std::get<2>(var);
			auto & vmin = std::get<3>(var);
			auto & vmax = std::get<4>(var);
			RooRealVar & rooVar = dynamic_cast<RooRealVar&>(*ws.factory(Form("%s[0.]",name.c_str())));
			rooVar.setConstant(false);
			if( nbins >= 0 ) {
				rooVar.setMin(vmin);
				rooVar.setMax(vmax);
				rooVar.setBins(nbins);
			}
			rooVars_.add(rooVar,true);
		}
		/// if( ! binnedOnly_ ) {
		RooDataSet dset(formatString(name_,replacements).c_str(),formatString(name_,replacements).c_str(),rooVars_,weightVar);
		ws.import( dset );
		/// }
		/// if( binned_ ) { FIXME implement binned datasets handling
		/// 	ws.import( RooDataHist(formatString(name_,replacements),formatString(name_,replacements),rooVars_,weightVar) );
		/// }
		dataset_ = ws.data(name_.c_str());
	}
	
	template<class F, class O>
	void CategoryDumper<F,O>::fill(const object_type & obj, double weight)
	{
		weight_ = weight;
		if( dataset_ ) {
			dynamic_cast<RooRealVar&>(rooVars_["weight"]).setVal(weight_);
		}
		// for( auto & var : variables_ ) {
		for(size_t ivar=0; ivar<names_.size(); ++ivar) {
			auto name = names_[ivar].c_str();
			auto & var = variables_[ivar];
			auto & val = std::get<0>(var);
			val = std::get<1>(var)(obj);
			if( dataset_ ) {
				dynamic_cast<RooRealVar&>(rooVars_[name]).setVal(val);
			}
		}
		if( tree_ ) { tree_->Fill(); }
		if( dataset_ ) { dataset_->add(rooVars_,weight_); }
		for( auto & histo : histograms_ ) {
			auto & th1 = *std::get<5>(histo);
			auto xv = std::get<1>(histo);
			auto yv = std::get<3>(histo);
			if( yv >= 0 ) {
				dynamic_cast<TH2&>(th1).Fill( std::get<0>(variables_[xv]), std::get<0>(variables_[yv]), weight_ );
			} else {
				th1.Fill( std::get<0>(variables_[xv]), weight_ );
			}
		}	
	}

}

#endif  // flashgg_CategoryDumper_h
