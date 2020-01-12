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

        //EDGetTokenT<OwnVector<flashgg::DiPhotonTagBase> > diphotonTagToken_;
        EDGetTokenT<OwnVector<flashgg::StageOneTag> > diphotonTagToken_;
        EDGetTokenT<OwnVector<flashgg::TagTruthBase> > tagTruthToken_;
        EDGetTokenT<HTXS::HiggsClassification> HTXSToken_;

        bool isGluonFusion_;
        FileInPath NNLOPSWeightFile_;
        std::vector<std::unique_ptr<TGraph> > NNLOPSWeights_;
        bool applyNNLOPSweight_;

        bool debug_;
    };

    TagWeighter::TagWeighter( const ParameterSet &iConfig ) :
        //diphotonTagToken_( consumes<OwnVector<flashgg::DiPhotonTagBase> >( iConfig.getParameter<InputTag> ( "diphotonTag" ) ) ),
        diphotonTagToken_( consumes<OwnVector<flashgg::StageOneTag> >( iConfig.getParameter<InputTag> ( "diphotonTag" ) ) ),
        tagTruthToken_( consumes<OwnVector<flashgg::TagTruthBase> >( iConfig.getParameter<InputTag> ( "tagTruth" ) ) )
    {
        ParameterSet HTXSps = iConfig.getParameterSet( "HTXSTags" );
        HTXSToken_ = consumes<HTXS::HiggsClassification>( HTXSps.getParameter<InputTag>("ClassificationObj") );

        isGluonFusion_ = iConfig.getParameter<bool>("isGluonFusion");
        NNLOPSWeightFile_ = iConfig.getParameter<FileInPath>( "NNLOPSWeightFile" );
        TFile* f = TFile::Open(NNLOPSWeightFile_.fullPath().c_str());
        NNLOPSWeights_.emplace_back((TGraph*)((TGraph*) f->Get("gr_NNLOPSratio_pt_mcatnlo_0jet"))->Clone() );
        NNLOPSWeights_.emplace_back((TGraph*)((TGraph*) f->Get("gr_NNLOPSratio_pt_mcatnlo_1jet"))->Clone() );
        NNLOPSWeights_.emplace_back((TGraph*)((TGraph*) f->Get("gr_NNLOPSratio_pt_mcatnlo_2jet"))->Clone() );
        NNLOPSWeights_.emplace_back((TGraph*)((TGraph*) f->Get("gr_NNLOPSratio_pt_mcatnlo_3jet"))->Clone() );
        f->Close();
        delete f;
        applyNNLOPSweight_ = iConfig.getParameter<bool>("applyNNLOPSweight");

        debug_ = iConfig.getUntrackedParameter<bool>( "Debug", false );

        //produces<OwnVector<flashgg::DiPhotonTagBase> >();
        produces<OwnVector<flashgg::StageOneTag> >();
        produces<OwnVector<flashgg::TagTruthBase> >();
    }

    void TagWeighter::produce( Event &evt, const EventSetup & )
    {
        std::cout << "ED DEBUG into TagWeighter" << std::endl;
        //Handle<OwnVector<flashgg::DiPhotonTagBase> > diphotonTags;
        Handle<OwnVector<flashgg::StageOneTag> > diphotonTags;
        evt.getByToken( diphotonTagToken_, diphotonTags);
        //auto diphotonTag = diphotonTags->ptrAt(0);
        //auto newDiphotonTag = diphotonTag->clone();

        Handle<OwnVector<flashgg::TagTruthBase> > tagTruths;
        evt.getByToken( tagTruthToken_, tagTruths);
        //auto tagTruth = tagTruths->ptrAt(0);
        //auto newTagTruth = tagTruth->clone();

        Handle<HTXS::HiggsClassification> htxsClassification;
        evt.getByToken(HTXSToken_,htxsClassification);

        //unique_ptr<edm::OwnVector<flashgg::DiPhotonTagBase> > weightedTags( new edm::OwnVector<flashgg::DiPhotonTagBase> );
        unique_ptr<edm::OwnVector<flashgg::StageOneTag> > weightedTags( new edm::OwnVector<flashgg::StageOneTag> );
        unique_ptr<edm::OwnVector<flashgg::TagTruthBase> > weightedTagTruths( new edm::OwnVector<flashgg::TagTruthBase> );

        //FIXME need to figure out how to put zeros inn showhow
        std::cout << "ED DEBUG numbee of tags is "  <<  diphotonTags->size() << std::endl;
        std::cout << "ED DEBUG numbee of truth tags is "  <<  tagTruths->size() << std::endl;

        std::vector<float> NNLOPSweights;
        for( auto it = tagTruths->begin(); it != tagTruths->end(); ++it ) {
            auto newTagTruth = it->clone();
            // set the HTXS info, which includes the relevant bins at different stages
            // as well as the ggH theory weights and the NNLOPS weights
            if( ! evt.isRealData() ) {
                if ( !htxsClassification.isValid() ) { throw cms::Exception("Need HTXS info here!"); }
                std::cout << "ED DEBUG" << std::endl;
                std::cout << "ED DEBUG stage 1p1 process is " << htxsClassification->stage1_1_cat_pTjet30GeV << std::endl;
                std::cout << "ED DEBUG" << std::endl;
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
                    NNLOPSweights.push_back(NNLOPSweight);

                }
            }
            weightedTagTruths->push_back( newTagTruth );
        }

        // if specified, apply the NNLOPS weight to the central object weight 
        int counter = 0;
        for( auto it = diphotonTags->begin(); it != diphotonTags->end(); ++it ) {
            auto newDiphotonTag = it->clone();
            if( isGluonFusion_ && applyNNLOPSweight_ ) {
                float newCentralWeight = NNLOPSweights[counter] * newDiphotonTag->centralWeight();
                newDiphotonTag->setCentralWeight( newCentralWeight );
            }
            weightedTags->push_back( newDiphotonTag );
            counter += 1;
        }

        evt.put( std::move( weightedTags ) );
        evt.put( std::move( weightedTagTruths ) );
        std::cout << "ED DEBUG outo TagWeighter" << std::endl;
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

