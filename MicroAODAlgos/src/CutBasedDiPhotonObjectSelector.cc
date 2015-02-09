#include "flashgg/MicroAODAlgos/interface/CutBasedDiPhotonObjectSelector.h"
#include "FWCore/Framework/interface/Event.h"

using namespace edm;
using namespace std;

namespace flashgg
{
	CutBasedDiPhotonObjectSelector::CutBasedDiPhotonObjectSelector(const ParameterSet & cfg, edm::ConsumesCollector & cc) : 
		rhoToken_(cc.consumes<double>(cfg.getParameter<InputTag>("rho"))),
		classifier_(cfg),
		selector_(cfg.getParameter<string>("cut"))
	{
		
		auto variables = cfg.getParameter<vector<string> >("variables");
		for( auto & expr : variables ) {
			//// cout << expr << endl;
			functors_.push_back(functor_type(expr));
		}
		
		auto categories = cfg.getParameter<vector<edm::ParameterSet> >("categories");
		
		for(auto & cat : categories ) {
			auto name = cat.getUntrackedParameter<string>("name",Form("cat%lu",selections_.size()));
			
			auto selection = cat.getParameter<vector<edm::ParameterSet> >("selection");
			assert( selection.size() == functors_.size() );
			auto & selval = selections_[name];
			for( auto & var : selection )  {
				auto minval = var.getParameter<string>("min");
				auto maxval = var.getParameter<string>("max");
				auto rhocorr = var.getParameter<string>("rhocorr");
				
				//// cout << minval << " " << maxval << " " << rhocorr << endl;
				//// 
				//// functor_type aa(minval);
				//// cout << "AA" << endl;
				//// functor_type bb(maxval);
				//// cout << "BB" << endl;
				//// functor_type cc(rhocorr);
				//// cout << "CC" << endl;
				
				selval.push_back(make_tuple(functor_type(minval),functor_type(maxval),functor_type(rhocorr)));
			}
		}
	}

	bool CutBasedDiPhotonObjectSelector::operator()(const DiPhotonCandidate & cand, const EventBase & ev) const
	{
		if( ! selector_(cand) ) { return false; }
		edm::Handle<double> rho;
		const Event * ee = dynamic_cast<const Event *>(&ev);
		if( ee ) { ee->getByToken(rhoToken_,rho); }
		else { assert(0);  } // FIXME need to complete for FWlite
		rho_ = *rho;
		auto leadingPhoton = cand.leadingPhoton();
		auto subleadingPhoton = cand.subLeadingPhoton();
		return pass(*leadingPhoton) && pass(*subleadingPhoton);
	}
	
	
	bool CutBasedDiPhotonObjectSelector::pass(const Photon & pho) const {
		auto cat = classifier_(pho);
		auto isel = selections_.find(cat.first);
		/// cout << cat.first << " " << (isel == selections_.end()) << endl;
		if( isel == selections_.end() ) {
			return false;
		}
		const auto & sel = isel->second;
		for(size_t iv=0; iv<sel.size(); ++iv) {
			auto val  = functors_[iv](pho) - rho_*get<2>(sel[iv])(pho);
			/// cout << iv << " "  << val << " " <<  get<0>(sel[iv])(pho) << " " << get<1>(sel[iv])(pho) << endl;
			if( val <= get<0>(sel[iv])(pho) || val >= get<1>(sel[iv])(pho) ) { return false; }
		}
		return true;
	}
}
