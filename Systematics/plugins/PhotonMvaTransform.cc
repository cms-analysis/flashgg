#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "TGraph.h"
#include "TFile.h"

namespace flashgg {

    class PhotonMvaTransform: public ObjectSystMethodBinnedByFunctor<flashgg::Photon, int>
    {

    public:
        typedef StringCutObjectSelector<Photon, true> selector_type;

        PhotonMvaTransform( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;

    private:
        selector_type overall_range_;
        edm::FileInPath correctionFile_;
        std::vector<std::unique_ptr<TGraph> > corrections_;
    };

    PhotonMvaTransform::PhotonMvaTransform( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv ) :
        ObjectSystMethodBinnedByFunctor( conf, std::forward<edm::ConsumesCollector>(iC), gv ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) )
    {
        correctionFile_ = conf.getParameter<edm::FileInPath>("CorrectionFile");
        std::cout<<correctionFile_.fullPath().c_str()<<std::endl;
        TFile* f = TFile::Open(correctionFile_.fullPath().c_str());
        //        f->Print();
        corrections_.emplace_back( (TGraph*)((TGraph*) f->Get("trasfhebup"))->Clone());
        corrections_.emplace_back((TGraph*)((TGraph*) f->Get("trasfhebdown"))->Clone() );
        corrections_.emplace_back((TGraph*)((TGraph*) f->Get("trasfheeup"))->Clone() );
        corrections_.emplace_back((TGraph*)((TGraph*) f->Get("trasfheedown"))->Clone() );
        f->Close();
    }

    std::string PhotonMvaTransform::shiftLabel( int syst_value ) const
    {
        std::string result;
        if( syst_value == 0 ) {
            result = Form( "%sCentral", label().c_str() );
        } else if( syst_value > 0 ) {
            result = Form( "%sUp%.2dsigma", label().c_str(), syst_value );
        } else {
            result = Form( "%sDown%.2dsigma", label().c_str(), -1 * syst_value );
        }
        return result;
    }

    void PhotonMvaTransform::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        //        if(syst_shift==0) return;
        if( overall_range_( y ) ) { 
            auto val_err = binContents( y );
            float shift_val = val_err.first[0]; 
            if (!applyCentralValue()) shift_val = 0.;
            //            std::cout<<"syst shift is "<<syst_shift<<std::endl;
            int correctionIndex = -1;
            if (y.isEB()) {
                if (syst_shift > 0)
                    correctionIndex = 0;
                else 
                    correctionIndex = 1;

            } else {
                if (syst_shift > 0)
                    correctionIndex = 2;
                else
                    correctionIndex = 3;
            }
            if (correctionIndex == -1) std::cerr << "ERROR: not enough corrections defined for IDMVA" << std::endl;            
         
            auto beforeMap = y.phoIdMvaD();
            std::vector<edm::Ptr<reco::Vertex> > keys;
            if( debug_ ) {
                std::cout << " PhoID MVA Syst shift from transformation : Photon has pt= " << y.pt() << " eta=" << y.eta();
                std::cout << "     MVA VALUES BEFORE: ";
                for(auto keyval = beforeMap.begin(); keyval != beforeMap.end(); ++keyval) {
                    keys.push_back( keyval->first );
                    std::cout << keyval->second << " ";
                }
                std::cout << std::endl;
            }
            edm::Ptr<reco::Vertex> vtx;
            float shift =0., mvaVal=0.;
            for(auto keyval = beforeMap.begin(); keyval != beforeMap.end(); ++keyval) {
                vtx = keyval->first;
                mvaVal = keyval->second;
                shift = shift_val + (mvaVal - corrections_[correctionIndex]->Eval(mvaVal))*abs(syst_shift);
                y.shiftMvaValueBy(shift, vtx);
            }
            if ( debug_) {
                auto afterMap = y.phoIdMvaD();
                std::cout << "     MVA VALUES AFTER: ";
                for(auto key = keys.begin() ; key != keys.end() ; ++key ) {
                    std::cout << afterMap.at(*key) << " ";
                }
                std::cout << std::endl;
            }
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonMvaTransform,
                   "FlashggPhotonMvaTransform" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

