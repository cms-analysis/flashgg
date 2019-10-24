#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "flashgg/Systematics/interface/GluonFusionTheoryWeights.h"
#include "FWCore/Utilities/interface/Exception.h"

using namespace flashgg;

TagTruthBase::TagTruthBase( const TagTruthBase &b )
{
    copyBaseInfo( b );
}

TagTruthBase *TagTruthBase::clone() const
{
    TagTruthBase *result = new TagTruthBase( *this );
    return result;
}

void TagTruthBase::setHTXSInfo( int stage0bin, int stage1bin, int njets, float pTH, float pTV ) {
    stage0bin_ = stage0bin;
    stage1bin_ = stage1bin;
    njets_ = njets;
    pTH_ = pTH;
    pTV_ = pTV;

    setGluonFusionWeights( njets, pTH, stage1bin );

    stage0map_[0]  = 0;  // unknown
    stage0map_[10] = -1; //GG2H_FWD
    stage0map_[11] = 1;  //GG2H
    stage0map_[20] = -2; //VBF_FWD
    stage0map_[21] = 2;  //VBF
    stage0map_[22] = -3; //VH2HQQ_FWD
    stage0map_[23] = 3;  //VH2HQQ
    stage0map_[30] = -4; //QQ2HLNU_FWDH
    stage0map_[31] = 4;  //QQ2HLNU
    stage0map_[40] = -5; //QQ2HLL_FWDH
    stage0map_[41] = 5;  //QQ2HLL
    stage0map_[50] = -6; //GG2HLL_FWDH
    stage0map_[51] = 6;  //GG2HLL
    stage0map_[60] = -7; //TTH_FWDH
    stage0map_[61] = 7;  //TTH
    stage0map_[70] = -8; //BBH_FWDH
    stage0map_[71] = 8;  //BBH
    stage0map_[80] = -9; //TH_FWDH
    stage0map_[81] = 9;  //TH

    stage1map_[0]   = 0;  // unknown
    stage1map_[100] = -1; //GG2H_FWDH
    stage1map_[101] = 1;  //GG2H_VBFTOPO_JET3VETO
    stage1map_[102] = 2;  //GG2H_VBFTOPO_JET3
    stage1map_[103] = 3;  //GG2H_0J  
    stage1map_[104] = 4;  //GG2H_1J_PTH_0_60
    stage1map_[105] = 5;  //GG2H_1J_PTH_60_120
    stage1map_[106] = 6;  //GG2H_1J_PTH_120_200
    stage1map_[107] = 7;  //GG2H_1J_PTH_GT200
    stage1map_[108] = 8;  //GG2H_GE2J_PTH_0_60
    stage1map_[109] = 9;  //GG2H_GE2J_PTH_60_120
    stage1map_[110] = 10; //GG2H_GE2J_PTH_120_200
    stage1map_[111] = 11; //GG2H_GE2J_PTH_GT200
    stage1map_[200] = -2; //QQ2HQQ_FWDH
    stage1map_[201] = 12; //QQ2HQQ_VBFTOPO_JET3VETO
    stage1map_[202] = 13; //QQ2HQQ_VBFTOPO_JET3
    stage1map_[203] = 14; //QQ2HQQ_VH2JET
    stage1map_[204] = 15; //QQ2HQQ_REST
    stage1map_[205] = 16; //QQ2HQQ_PTJET1_GT200
    stage1map_[300] = -3; //QQ2HLNU_FWDH
    stage1map_[301] = 17; //QQ2HLNU_PTV_0_150
    stage1map_[302] = 18; //QQ2HLNU_PTV_150_250_0J
    stage1map_[303] = 19; //QQ2HLNU_PTV_150_250_GE1J
    stage1map_[304] = 20; //QQ2HLNU_PTV_GT250
    stage1map_[400] = -4; //QQ2HLL_FWDH
    stage1map_[401] = 21; //QQ2HLL_PTV_0_150
    stage1map_[402] = 22; //QQ2HLL_PTV_150_250_0J
    stage1map_[403] = 23; //QQ2HLL_PTV_150_250_GE1J
    stage1map_[404] = 24; //QQ2HLL_PTV_GT250
    stage1map_[500] = -5; //GG2HLL_FWDH
    stage1map_[501] = 25; //GG2HLL_PTV_0_150
    stage1map_[502] = 26; //GG2HLL_PTV_GT150_0J
    stage1map_[503] = 27; //GG2HLL_PTV_GT150_GE1J
    stage1map_[600] = -6; //TTH_FWDH
    stage1map_[601] = 28; //TTH
    stage1map_[700] = -7; //BBH_FWDH
    stage1map_[701] = 29; //BBH
    stage1map_[800] = -8; //TH_FWDH
    stage1map_[801] = 30; //TH
}

void TagTruthBase::copyBaseInfo( const TagTruthBase &b ) {
    setGenPV( b.genPV() );
    setHTXSInfo( b.HTXSstage0bin(), b.HTXSstage1bin(), b.HTXSnjets(), b.HTXSpTH(), b.HTXSpTV() );
}

int TagTruthBase::HTXSstage0orderedBin() const {
    auto theIt = stage0map_.find(stage0bin_);
    if( theIt != stage0map_.end() ) {
        return theIt->second;
    }
    else {
        throw cms::Exception( "Missing Data" ) << "Could not find value (in the TagTruthBase) for the STXS process: " << stage0bin_ << "\n";;
    }
}

int TagTruthBase::HTXSstage1orderedBin() const {
    auto theIt = stage1map_.find(stage1bin_);
    if( theIt != stage1map_.end() ) {
        return theIt->second;
    }
    else {
        throw cms::Exception( "Missing Data" ) << "Could not find value (in the TagTruthBase) for the STXS process: " << stage1bin_ << "\n";;
    }
}

//in order to set the ggH weights, use suggested code (in header file)
// order: THU_ggH_Mu, THU_ggH_Res, THU_ggH_Mig01, THU_ggH_Mig12, THU_ggH_VBF2j, THU_ggH_VBF3j, THU_ggH_PT60, THU_ggH_PT120, THU_ggH_qmtop
void TagTruthBase::setGluonFusionWeights( int njets, float pTH, int stage1bin )
{
    std::vector<float> theWeightsUp  = qcd_ggF_uncertSF_2017( njets, pTH, stage1bin, 1. );
    this->setWeight("THU_ggH_MuUp01sigma", theWeightsUp[0]);
    this->setWeight("THU_ggH_ResUp01sigma", theWeightsUp[1]);
    this->setWeight("THU_ggH_Mig01Up01sigma", theWeightsUp[2]);
    this->setWeight("THU_ggH_Mig12Up01sigma", theWeightsUp[3]);
    this->setWeight("THU_ggH_VBF2jUp01sigma", theWeightsUp[4]);
    this->setWeight("THU_ggH_VBF3jUp01sigma", theWeightsUp[5]);
    this->setWeight("THU_ggH_PT60Up01sigma", theWeightsUp[6]);
    this->setWeight("THU_ggH_PT120Up01sigma", theWeightsUp[7]);
    this->setWeight("THU_ggH_qmtopUp01sigma", theWeightsUp[8]);

    std::vector<float> theWeightsDown = qcd_ggF_uncertSF_2017( njets, pTH, stage1bin, -1. );
    this->setWeight("THU_ggH_MuDown01sigma", theWeightsDown[0]);
    this->setWeight("THU_ggH_ResDown01sigma", theWeightsDown[1]);
    this->setWeight("THU_ggH_Mig01Down01sigma", theWeightsDown[2]);
    this->setWeight("THU_ggH_Mig12Down01sigma", theWeightsDown[3]);
    this->setWeight("THU_ggH_VBF2jDown01sigma", theWeightsDown[4]);
    this->setWeight("THU_ggH_VBF3jDown01sigma", theWeightsDown[5]);
    this->setWeight("THU_ggH_PT60Down01sigma", theWeightsDown[6]);
    this->setWeight("THU_ggH_PT120Down01sigma", theWeightsDown[7]);
    this->setWeight("THU_ggH_qmtopDown01sigma", theWeightsDown[8]);
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
