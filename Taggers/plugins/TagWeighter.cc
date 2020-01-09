#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/UntaggedTag.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/NoTag.h"
#include "flashgg/DataFormats/interface/StageOneTag.h"

#include "SimDataFormats/HTXS/interface/HiggsTemplateCrossSections.h"

#include "TMVA/Reader.h"
#include "TMath.h"
#include "TGraph.h"
//#include <typeinfo>

#include <algorithm>


using namespace std;
using namespace edm;

namespace flashgg {

    class TagWeighter : public EDProducer
    {

    public:
        TagWeighter( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<flashgg::DiPhotonTagBase> diphotonTagToken_;
        EDGetTokenT<flashgg::TagTruthBase> tagTruthToken_;
        EDGetTokenT<HTXS::HiggsClassification> HTXSToken_;

        bool isGluonFusion_;
        edm::FileInPath NNLOPSWeightFile_;
        std::vector<std::unique_ptr<TGraph> > NNLOPSWeights_;
        bool applyNNLOPSweight_;

        bool debug_;
    };

    TagWeighter::TagWeighter( const ParameterSet &iConfig ) :
        diphotonTagToken_( consumes<flashgg::DiPhotonTagBase>( iConfig.getParameter<InputTag> ( "diphotonTag" ) ) ),
        tagTruthToken_( consumes<flashgg::TagTruthBase>( iConfig.getParameter<InputTag> ( "tagTruth" ) ) )
    {
        ParameterSet HTXSps = iConfig.getParameterSet( "HTXSTags" );
        HTXSToken_ = consumes<HTXS::HiggsClassification>( HTXSps.getParameter<InputTag>("ClassificationObj") );

        isGluonFusion_ = iConfig.getParameter<bool>("isGluonFusion");
        NNLOPSWeightFile_ = iConfig.getParameter<edm::FileInPath>( "NNLOPSWeightFile" );
        TFile* f = TFile::Open(NNLOPSWeightFile_.fullPath().c_str());
        NNLOPSWeights_.emplace_back((TGraph*)((TGraph*) f->Get("gr_NNLOPSratio_pt_mcatnlo_0jet"))->Clone() );
        NNLOPSWeights_.emplace_back((TGraph*)((TGraph*) f->Get("gr_NNLOPSratio_pt_mcatnlo_1jet"))->Clone() );
        NNLOPSWeights_.emplace_back((TGraph*)((TGraph*) f->Get("gr_NNLOPSratio_pt_mcatnlo_2jet"))->Clone() );
        NNLOPSWeights_.emplace_back((TGraph*)((TGraph*) f->Get("gr_NNLOPSratio_pt_mcatnlo_3jet"))->Clone() );
        f->Close();
        delete f;
        applyNNLOPSweight_ = iConfig.getParameter<bool>("applyNNLOPSweight");

        debug_ = iConfig.getUntrackedParameter<bool>( "Debug", false );

        produces<flashgg::DiPhotonTagBase> ();
        produces<flashgg::TagTruthBase> ();
    }

    void TagWeighter::produce( Event &evt, const EventSetup & )
    {
        Handle<flashgg::DiPhotonTagBase> diphotonTag;
        evt.getByToken( diphotonTagToken_, diphotonTag);
        auto newDiphotonTag = diphotonTag->clone();

        Handle<flashgg::TagTruthBase> tagTruth;
        evt.getByToken( tagTruthToken_, tagTruth);
        auto newTagTruth = tagTruth->clone();

        Handle<HTXS::HiggsClassification> htxsClassification;
        evt.getByToken(HTXSToken_,htxsClassification);

        // set the HTXS info, which includes the relevant bins at different stages
        // as well as the ggH theory weights and the NNLOPS weights
        if( ! evt.isRealData() ) {
            if ( !htxsClassification.isValid() ) { throw cms::Exception("Need HTXS info here!"); }
            newTagTruth->setHTXSInfo( htxsClassification->stage0_cat,
                                   htxsClassification->stage1_cat_pTjet30GeV,
                                   htxsClassification->stage1_1_cat_pTjet30GeV,
                                   htxsClassification->stage1_1_fine_cat_pTjet30GeV,
                                   htxsClassification->jets30.size(),
                                   htxsClassification->p4decay_higgs.pt(),
                                   htxsClassification->p4decay_V.pt() );

            if( isGluonFusion_ ) {
                int stxsNjets = htxsClassification->jets30.size();
                float stxsPtH = htxsClassification->p4decay_higgs.pt();
                float NNLOPSweight = 1.;
                if ( stxsNjets == 0) NNLOPSweight = NNLOPSWeights_[0]->Eval(min(stxsPtH,float(125.0)));
                else if ( stxsNjets == 1) NNLOPSweight = NNLOPSWeights_[1]->Eval(min(stxsPtH,float(625.0)));
                else if ( stxsNjets == 2) NNLOPSweight = NNLOPSWeights_[2]->Eval(min(stxsPtH,float(800.0)));
                else if ( stxsNjets >= 3) NNLOPSweight = NNLOPSWeights_[3]->Eval(min(stxsPtH,float(925.0)));
                newTagTruth->setWeight("NNLOPS", NNLOPSweight);

                // if specified, apply the NNLOPS weight to the central object weight 
                if( applyNNLOPSweight_ ) {
                    float newCentralWeight = newTagTruth->weight("NNLOPS") * newDiphotonTag->centralWeight();
                    newDiphotonTag->setCentralWeight( newCentralWeight );
                }
            }
        }

        std::unique_ptr<flashgg::DiPhotonTagBase> weightedTag( new flashgg::DiPhotonTagBase( *newDiphotonTag->clone() ) );
        std::unique_ptr<flashgg::TagTruthBase> weightedTagTruth( new flashgg::TagTruthBase( *newTagTruth->clone() ) );
        evt.put( std::move( weightedTag ) );
        evt.put( std::move( weightedTagTruth ) );
    }


}

typedef flashgg::TagWeighter FlashggTagWeighter;
DEFINE_FWK_MODULE( FlashggTagWeighter );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

