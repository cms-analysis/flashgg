#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "flashgg/Systematics/interface/GluonFusionTheoryWeights.h"
#include "FWCore/Utilities/interface/Exception.h"

using namespace flashgg;

TagTruthBase::TagTruthBase()
{
    genPV_ = Point();
    stage0bin_ = 0;
    stage1bin_ = 0;
    stage1p1bin_ = 0;
    stage1p1binFine_ = 0;
    stage1p2bin_ = 0;
    stage1p2binFine_ = 0;
    njets_ = -999;
    pTH_ = -999.;
    pTV_ = -999.;
}

TagTruthBase::TagTruthBase( const TagTruthBase &b )
{
    copyBaseInfo( b );
}

TagTruthBase *TagTruthBase::clone() const
{
    TagTruthBase *result = new TagTruthBase( *this );
    return result;
}

void TagTruthBase::setHTXSInfo( int stage0bin, int stage1bin, int stage1p1bin, int stage1p1binFine, int stage1p2bin, int stage1p2binFine, int njets, float pTH, float pTV ) {
    stage0bin_ = stage0bin;
    stage1bin_ = stage1bin;
    stage1p1bin_ = stage1p1bin;
    stage1p1binFine_ = stage1p1binFine;
    stage1p2bin_ = stage1p2bin;
    stage1p2binFine_ = stage1p2binFine;
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

    stage1p1map_[0] = 0; //UNKNOWN
    stage1p1map_[100] = -1; //GG2H_FWDH = 100,
    stage1p1map_[200] = -2; //QQ2HQQ_FWDH = 200,
    stage1p1map_[300] = -3; //QQ2HLNU_FWDH = 300,
    stage1p1map_[400] = -4; //QQ2HLL_FWDH = 400,
    stage1p1map_[500] = -5; //GG2HLL_FWDH = 500,
    stage1p1map_[600] = -6; //TTH_FWDH = 600,
    stage1p1map_[700] = -7; //BBH_FWDH = 700, 
    stage1p1map_[800] = -8; //TH_FWDH = 800, 
    stage1p1map_[101] = 1; //GG2H_PTH_GT200 = 101,
    stage1p1map_[102] = 2; //GG2H_0J_PTH_0_10   = 102,
    stage1p1map_[103] = 3; //GG2H_0J_PTH_GT10   = 103,
    stage1p1map_[104] = 4; //GG2H_1J_PTH_0_60 = 104,
    stage1p1map_[105] = 5; //GG2H_1J_PTH_60_120 = 105,
    stage1p1map_[106] = 6; //GG2H_1J_PTH_120_200 = 106,
    stage1p1map_[107] = 7; //GG2H_GE2J_MJJ_0_350_PTH_0_60 = 107,
    stage1p1map_[108] = 8; //GG2H_GE2J_MJJ_0_350_PTH_60_120 = 108,
    stage1p1map_[109] = 9; //GG2H_GE2J_MJJ_0_350_PTH_120_200 = 109,
    stage1p1map_[110] = 10; //GG2H_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25 = 110,
    stage1p1map_[111] = 11; //GG2H_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_GT25 = 111,
    stage1p1map_[112] = 12; //GG2H_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25 = 112,
    stage1p1map_[113] = 13; //GG2H_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_GT25 = 113,
    stage1p1map_[201] = 14; //QQ2HQQ_0J = 201,
    stage1p1map_[202] = 15; //QQ2HQQ_1J = 202,
    stage1p1map_[203] = 16; //QQ2HQQ_GE2J_MJJ_0_60 = 203,
    stage1p1map_[204] = 17; //QQ2HQQ_GE2J_MJJ_60_120 = 204,
    stage1p1map_[205] = 18; //QQ2HQQ_GE2J_MJJ_120_350 = 205,
    stage1p1map_[206] = 19; //QQ2HQQ_GE2J_MJJ_GT350_PTH_GT200 = 206,
    stage1p1map_[207] = 20; //QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25 = 207,
    stage1p1map_[208] = 21; //QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_GT25 = 208,
    stage1p1map_[209] = 22; //QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25 = 209,
    stage1p1map_[210] = 23; //QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_GT25 = 210,
    stage1p1map_[301] = 24; //QQ2HLNU_PTV_0_75 = 301,
    stage1p1map_[302] = 25; //QQ2HLNU_PTV_75_150 = 302,
    stage1p1map_[303] = 26; //QQ2HLNU_PTV_150_250_0J = 303,
    stage1p1map_[304] = 27; //QQ2HLNU_PTV_150_250_GE1J = 304,
    stage1p1map_[305] = 28; //QQ2HLNU_PTV_GT250 = 305,
    stage1p1map_[401] = 29; //QQ2HLL_PTV_0_75 = 401,
    stage1p1map_[402] = 30; //QQ2HLL_PTV_75_150 = 402,
    stage1p1map_[403] = 31; //QQ2HLL_PTV_150_250_0J = 403,
    stage1p1map_[404] = 32; //QQ2HLL_PTV_150_250_GE1J = 404,
    stage1p1map_[405] = 33; //QQ2HLL_PTV_GT250 = 405,
    stage1p1map_[501] = 34; //GG2HLL_PTV_0_75 = 501,
    stage1p1map_[502] = 35; //GG2HLL_PTV_75_150 = 502,
    stage1p1map_[503] = 36; //GG2HLL_PTV_150_250_0J = 503,
    stage1p1map_[504] = 37; //GG2HLL_PTV_150_250_GE1J = 504,
    stage1p1map_[505] = 38; //GG2HLL_PTV_GT250 = 505,
    stage1p1map_[601] = 39; //TTH = 601,
    stage1p1map_[701] = 40; //BBH = 701,
    stage1p1map_[801] = 41; //TH = 801

    //FIXME add the stage 1p1Fine map here

    stage1p2map_[0] = 0; //UNKNOWN
    stage1p2map_[100] = -1; //GG2H_FWDH = 100,
    stage1p2map_[101] = 1; //GG2H_PTH_200_300 = 101,
    stage1p2map_[102] = 2; //GG2H_PTH_300_450 = 102,
    stage1p2map_[103] = 3; //GG2H_PTH_450_650 = 103,
    stage1p2map_[104] = 4; //GG2H_PTH_GT650 = 104,
    stage1p2map_[105] = 5; //GG2H_0J_PTH_0_10   = 105,
    stage1p2map_[106] = 6; //GG2H_0J_PTH_GT10   = 106,
    stage1p2map_[107] = 7; //GG2H_1J_PTH_0_60 = 107,
    stage1p2map_[108] = 8; //GG2H_1J_PTH_60_120 = 108,
    stage1p2map_[109] = 9; //GG2H_1J_PTH_120_200 = 109,
    stage1p2map_[110] = 10; //GG2H_GE2J_MJJ_0_350_PTH_0_60 = 110,
    stage1p2map_[111] = 11; //GG2H_GE2J_MJJ_0_350_PTH_60_120 = 111,
    stage1p2map_[112] = 12; //GG2H_GE2J_MJJ_0_350_PTH_120_200 = 112,
    stage1p2map_[113] = 13; //GG2H_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25 = 113,
    stage1p2map_[114] = 14; //GG2H_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_GT25 = 114,
    stage1p2map_[115] = 15; //GG2H_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25 = 115,
    stage1p2map_[116] = 16; //GG2H_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_GT25 = 116,
    stage1p2map_[200] = -2; //QQ2HQQ_FWDH = 200,
    stage1p2map_[201] = 17; //QQ2HQQ_0J = 201,
    stage1p2map_[202] = 18; //QQ2HQQ_1J = 202,
    stage1p2map_[203] = 19; //QQ2HQQ_GE2J_MJJ_0_60 = 203,
    stage1p2map_[204] = 20; //QQ2HQQ_GE2J_MJJ_60_120 = 204,
    stage1p2map_[205] = 21; //QQ2HQQ_GE2J_MJJ_120_350 = 205,
    stage1p2map_[206] = 22; //QQ2HQQ_GE2J_MJJ_GT350_PTH_GT200 = 206,
    stage1p2map_[207] = 23; //QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_0_25 = 207,
    stage1p2map_[208] = 24; //QQ2HQQ_GE2J_MJJ_350_700_PTH_0_200_PTHJJ_GT25 = 208,
    stage1p2map_[209] = 25; //QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_0_25 = 209,
    stage1p2map_[210] = 26; //QQ2HQQ_GE2J_MJJ_GT700_PTH_0_200_PTHJJ_GT25 = 210,
    stage1p2map_[300] = -3; //QQ2HLNU_FWDH = 300,
    stage1p2map_[301] = 27; //QQ2HLNU_PTV_0_75 = 301,
    stage1p2map_[302] = 28; //QQ2HLNU_PTV_75_150 = 302,
    stage1p2map_[303] = 29; //QQ2HLNU_PTV_150_250_0J = 303,
    stage1p2map_[304] = 30; //QQ2HLNU_PTV_150_250_GE1J = 304,
    stage1p2map_[305] = 31; //QQ2HLNU_PTV_GT250 = 305,
    stage1p2map_[400] = -4; //QQ2HLL_FWDH = 400,
    stage1p2map_[401] = 32; //QQ2HLL_PTV_0_75 = 401,
    stage1p2map_[402] = 33; //QQ2HLL_PTV_75_150 = 402,
    stage1p2map_[403] = 34; //QQ2HLL_PTV_150_250_0J = 403,
    stage1p2map_[404] = 35; //QQ2HLL_PTV_150_250_GE1J = 404,
    stage1p2map_[405] = 36; //QQ2HLL_PTV_GT250 = 405,
    stage1p2map_[500] = -5; //GG2HLL_FWDH = 500,
    stage1p2map_[501] = 37; //GG2HLL_PTV_0_75 = 501,
    stage1p2map_[502] = 38; //GG2HLL_PTV_75_150 = 502,
    stage1p2map_[503] = 39; //GG2HLL_PTV_150_250_0J = 503,
    stage1p2map_[504] = 40; //GG2HLL_PTV_150_250_GE1J = 504,
    stage1p2map_[505] = 41; //GG2HLL_PTV_GT250 = 505,
    stage1p2map_[600] = -6; //TTH_FWDH = 600, 
    stage1p2map_[601] = 42; //TTH_PTH_0_60 = 601,
    stage1p2map_[602] = 43; //TTH_PTH_60_120 = 602,
    stage1p2map_[603] = 44; //TTH_PTH_120_200 = 603,
    stage1p2map_[604] = 45; //TTH_PTH_200_300 = 604,
    stage1p2map_[605] = 46; //TTH_PTH_GT300 = 605,
    stage1p2map_[700] = -7; //BBH_FWDH = 700
    stage1p2map_[701] = 47; //BBH = 701,
    stage1p2map_[800] = -8; //TH_FWDH = 800,
    stage1p2map_[801] = 48; //TH = 801

    //FIXME add the stage 1p2Fine map here
}

void TagTruthBase::copyBaseInfo( const TagTruthBase &b ) {
    setGenPV( b.genPV() );
    setHTXSInfo( b.HTXSstage0bin(), b.HTXSstage1bin(), b.HTXSstage1p1bin(), b.HTXSstage1p1binFine(), b.HTXSstage1p2bin(), b.HTXSstage1p2binFine(), b.HTXSnjets(), b.HTXSpTH(), b.HTXSpTV() );
    this->setWeight("NNLOPSweight", b.weight("NNLOPSweight"));
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

int TagTruthBase::HTXSstage1p1orderedBin() const {
    auto theIt = stage1p1map_.find(stage1p1bin_);
    if( theIt != stage1p1map_.end() ) {
        return theIt->second;
    }
    else {
        throw cms::Exception( "Missing Data" ) << "Could not find value (in the TagTruthBase) for the STXS process: " << stage1p1bin_ << "\n";;
    }
}

int TagTruthBase::HTXSstage1p1orderedBinFine() const {
    auto theIt = stage1p1mapFine_.find(stage1p1binFine_);
    if( theIt != stage1p1mapFine_.end() ) {
        return theIt->second;
    }
    else {
        throw cms::Exception( "Missing Data" ) << "Could not find value (in the TagTruthBase) for the STXS process: " << stage1p1binFine_ << "\n";;
    }
}

int TagTruthBase::HTXSstage1p2orderedBin() const {
    auto theIt = stage1p2map_.find(stage1p2bin_);
    if( theIt != stage1p2map_.end() ) {
        return theIt->second;
    }
    else {
        throw cms::Exception( "Missing Data" ) << "Could not find value (in the TagTruthBase) for the STXS process: " << stage1p2bin_ << "\n";;
    }
}

int TagTruthBase::HTXSstage1p2orderedBinFine() const {
    auto theIt = stage1p2mapFine_.find(stage1p2binFine_);
    if( theIt != stage1p2mapFine_.end() ) {
        return theIt->second;
    }
    else {
        throw cms::Exception( "Missing Data" ) << "Could not find value (in the TagTruthBase) for the STXS process: " << stage1p2binFine_ << "\n";;
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
