#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

DiPhotonTagBase::DiPhotonTagBase()
{
    category_number_ = -1;
    isGold_ = -1;
}

DiPhotonTagBase::DiPhotonTagBase( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes )
    : DiPhotonTagBase::DiPhotonTagBase( diPho, *mvaRes ) {}

DiPhotonTagBase::DiPhotonTagBase( edm::Ptr<flashgg::DiPhotonCandidate> diPho, DiPhotonMVAResult mvaRes )
{
    mva_result_ = mvaRes;
    category_number_ = -1;
    dipho_ = diPho;
    isGold_ = -1;
}

DiPhotonTagBase::~DiPhotonTagBase()
{
}


bool DiPhotonTagBase::operator <( const DiPhotonTagBase &b ) const
{
    // For choosing which of two tags OF THE SAME TYPE are of higher priority
    // Comparison of different tags not currently supported - is it ever needed?
    // Overloading may be appropriate if different tags have different priorities

    if( categoryNumber() == b.categoryNumber() ) {
        return ( sumPt() < b.sumPt() );
    } else {
        return ( categoryNumber() < b.categoryNumber() );
    }
}

void DiPhotonTagBase::setIsGold( int runNumber ) {
    // Below is the subtraction between
    // https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver_v2.txt
    // and
    // https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt
    //
    // They were last changed on 18 December and still the most recent (as of 28 January)
    // I have confirmed that none of these runs are in the Gold at all, so checking runNumber without lumiSection suffices
    // Of course the result may be wrong for events not even in the Silver JSON
    // I am hard coding this to make dumping and comparing events faster, because this issue now needs intensive study
    //
    /*
      {"256729": [[1, 331], [346, 598], [600, 755], [758, 760], [765, 1165], [1167, 1292], [1295, 1327], [1329, 1732]],
 "256734": [[1, 57], [60, 213]],
 "257394": [[41, 72]],
 "257395": [[1, 13]],
 "257396": [[1, 216]],
 "257397": [[1, 119]],
 "257399": [[1, 271]],
 "257400": [[1, 291], [295, 819], [1011, 1418]],
 "257487": [[50, 102], [104, 202], [204, 1124]],
 "257490": [[1, 591]],
 "257822": [[1, 719], [721, 1389]],
 "257823": [[1, 171]],
 "258443": [[1, 291]]}
    */
    isGold_ = 1;
    if ( runNumber == 256729 ) { isGold_ = 0; }
    if ( runNumber == 256734 ) { isGold_ = 0; }
    if ( (runNumber >= 257394) && (runNumber <= 257397) ) { isGold_ = 0; }
    if ( (runNumber >= 257399) && (runNumber <=257400)) { isGold_ = 0; }
    if ( runNumber == 257487 ) { isGold_ = 0; }
    if ( runNumber == 257490 ) { isGold_ = 0; }
    if ( (runNumber >= 257822) && (runNumber <=257823)) { isGold_ = 0; }
    if ( runNumber == 258443 ) { isGold_ = 0; }
}

// theory weights (in TagTruthBase) calculated standalone without diphoton object weight applied, so multiply it in here
float DiPhotonTagBase::getTheoryWeight(std::string key) const
{
    float theoryWeight = truth_->weight(key);
    theoryWeight *= this->centralWeight();
    return theoryWeight;
}

// overrides default behaviour of returning 1 if weight doesn't exist to return central object weight instead
float DiPhotonTagBase::getObjectWeight(std::string key) const
{
    if( this->hasWeight(key) ) {
        return this->weight(key);
    }
    else {
        return this->centralWeight();
    }
}

string DiPhotonTagBase::stage1KinematicLabel() const { 
    switch(stage1recoTag_) {
    case stage1recoTag::LOGICERROR:
        return string("LOGICERROR");
    case stage1recoTag::NOTAG:
        return string("NOTAG");
    case stage1recoTag::RECO_0J_PTH_0_10_Tag0:
        return string("RECO_0J_PTH_0_10_Tag0");
    case stage1recoTag::RECO_0J_PTH_0_10_Tag1:
        return string("RECO_0J_PTH_0_10_Tag1");
    case stage1recoTag::RECO_0J_PTH_0_10_Tag2:
        return string("RECO_0J_PTH_0_10_Tag2");
    case stage1recoTag::RECO_0J_PTH_GT10_Tag0:
        return string("RECO_0J_PTH_GT10_Tag0");
    case stage1recoTag::RECO_0J_PTH_GT10_Tag1:
        return string("RECO_0J_PTH_GT10_Tag1");
    case stage1recoTag::RECO_0J_PTH_GT10_Tag2:
        return string("RECO_0J_PTH_GT10_Tag2");
    case stage1recoTag::RECO_1J_PTH_0_60_Tag0:
        return string("RECO_1J_PTH_0_60_Tag0");
    case stage1recoTag::RECO_1J_PTH_0_60_Tag1:
        return string("RECO_1J_PTH_0_60_Tag1");
    case stage1recoTag::RECO_1J_PTH_0_60_Tag2:
        return string("RECO_1J_PTH_0_60_Tag2");
    case stage1recoTag::RECO_1J_PTH_60_120_Tag0:
        return string("RECO_1J_PTH_60_120_Tag0");
    case stage1recoTag::RECO_1J_PTH_60_120_Tag1:
        return string("RECO_1J_PTH_60_120_Tag1");
    case stage1recoTag::RECO_1J_PTH_60_120_Tag2:
        return string("RECO_1J_PTH_60_120_Tag2");
    case stage1recoTag::RECO_1J_PTH_120_200_Tag0:
        return string("RECO_1J_PTH_120_200_Tag0");
    case stage1recoTag::RECO_1J_PTH_120_200_Tag1:
        return string("RECO_1J_PTH_120_200_Tag1");
    case stage1recoTag::RECO_1J_PTH_120_200_Tag2:
        return string("RECO_1J_PTH_120_200_Tag2");
    case stage1recoTag::RECO_GE2J_PTH_0_60_Tag0:
        return string("RECO_GE2J_PTH_0_60_Tag0");
    case stage1recoTag::RECO_GE2J_PTH_0_60_Tag1:
        return string("RECO_GE2J_PTH_0_60_Tag1");
    case stage1recoTag::RECO_GE2J_PTH_0_60_Tag2:
        return string("RECO_GE2J_PTH_0_60_Tag2");
    case stage1recoTag::RECO_GE2J_PTH_60_120_Tag0:
        return string("RECO_GE2J_PTH_60_120_Tag0");
    case stage1recoTag::RECO_GE2J_PTH_60_120_Tag1:
        return string("RECO_GE2J_PTH_60_120_Tag1");
    case stage1recoTag::RECO_GE2J_PTH_60_120_Tag2:
        return string("RECO_GE2J_PTH_60_120_Tag2");
    case stage1recoTag::RECO_GE2J_PTH_120_200_Tag0:
        return string("RECO_GE2J_PTH_120_200_Tag0");
    case stage1recoTag::RECO_GE2J_PTH_120_200_Tag1:
        return string("RECO_GE2J_PTH_120_200_Tag1");
    case stage1recoTag::RECO_GE2J_PTH_120_200_Tag2:
        return string("RECO_GE2J_PTH_120_200_Tag2");
    case stage1recoTag::RECO_PTH_200_300_Tag0:
        return string("RECO_PTH_200_300_Tag0");
    case stage1recoTag::RECO_PTH_200_300_Tag1:
        return string("RECO_PTH_200_300_Tag1");
    case stage1recoTag::RECO_PTH_300_450_Tag0:
        return string("RECO_PTH_300_450_Tag0");
    case stage1recoTag::RECO_PTH_300_450_Tag1:
        return string("RECO_PTH_300_450_Tag1");
    case stage1recoTag::RECO_PTH_450_650_Tag0:
        return string("RECO_PTH_450_650_Tag0");
    case stage1recoTag::RECO_PTH_GT650_Tag0:
        return string("RECO_PTH_GT650_Tag0");
    case stage1recoTag::RECO_VBFTOPO_VHHAD_Tag0:
        return string("RECO_VBFTOPO_VHHAD_Tag0");
    case stage1recoTag::RECO_VBFTOPO_VHHAD_Tag1:
        return string("RECO_VBFTOPO_VHHAD_Tag1");
    case stage1recoTag::RECO_VBFTOPO_JET3VETO_LOWMJJ_Tag0:
        return string("RECO_VBFTOPO_JET3VETO_LOWMJJ_Tag0");
    case stage1recoTag::RECO_VBFTOPO_JET3VETO_LOWMJJ_Tag1:
        return string("RECO_VBFTOPO_JET3VETO_LOWMJJ_Tag1");
    case stage1recoTag::RECO_VBFTOPO_JET3VETO_HIGHMJJ_Tag0:
        return string("RECO_VBFTOPO_JET3VETO_HIGHMJJ_Tag0");
    case stage1recoTag::RECO_VBFTOPO_JET3VETO_HIGHMJJ_Tag1:
        return string("RECO_VBFTOPO_JET3VETO_HIGHMJJ_Tag1");
    case stage1recoTag::RECO_VBFTOPO_JET3_LOWMJJ_Tag0:
        return string("RECO_VBFTOPO_JET3_LOWMJJ_Tag0");
    case stage1recoTag::RECO_VBFTOPO_JET3_LOWMJJ_Tag1:
        return string("RECO_VBFTOPO_JET3_LOWMJJ_Tag1");
    case stage1recoTag::RECO_VBFTOPO_JET3_HIGHMJJ_Tag0:
        return string("RECO_VBFTOPO_JET3_HIGHMJJ_Tag0");
    case stage1recoTag::RECO_VBFTOPO_JET3_HIGHMJJ_Tag1:
        return string("RECO_VBFTOPO_JET3_HIGHMJJ_Tag1");
    case stage1recoTag::RECO_VBFTOPO_BSM_Tag0:
        return string("RECO_VBFTOPO_BSM_Tag0");
    case stage1recoTag::RECO_VBFTOPO_BSM_Tag1:
        return string("RECO_VBFTOPO_BSM_Tag1");
    case stage1recoTag::RECO_VBFLIKEGGH_Tag0:
        return string("RECO_VBFLIKEGGH_Tag0");
    case stage1recoTag::RECO_VBFLIKEGGH_Tag1:
        return string("RECO_VBFLIKEGGH_Tag1");
    case stage1recoTag::RECO_WH_LEP_LOW_Tag0:
        return string("RECO_WH_LEP_LOW_Tag0");
    case stage1recoTag::RECO_WH_LEP_LOW_Tag1:
        return string("RECO_WH_LEP_LOW_Tag1");
    case stage1recoTag::RECO_WH_LEP_LOW_Tag2:
        return string("RECO_WH_LEP_LOW_Tag2");
    case stage1recoTag::RECO_WH_LEP_HIGH_Tag0:
        return string("RECO_WH_LEP_HIGH_Tag0");
    case stage1recoTag::RECO_WH_LEP_HIGH_Tag1:
        return string("RECO_WH_LEP_HIGH_Tag1");
    case stage1recoTag::RECO_WH_LEP_HIGH_Tag2:
        return string("RECO_WH_LEP_HIGH_Tag2");
    case stage1recoTag::RECO_ZH_LEP_Tag0:
        return string("RECO_ZH_LEP_Tag0");
    case stage1recoTag::RECO_ZH_LEP_Tag1:
        return string("RECO_ZH_LEP_Tag1");
    case stage1recoTag::RECO_VH_MET_Tag0:
        return string("RECO_VH_MET_Tag0");
    case stage1recoTag::RECO_VH_MET_Tag1:
        return string("RECO_VH_MET_Tag1");
    case stage1recoTag::RECO_TTH_LEP_PTH_0_60_Tag0:
        return string("RECO_TTH_LEP_PTH_0_60_Tag0");
    case stage1recoTag::RECO_TTH_LEP_PTH_0_60_Tag1:
        return string("RECO_TTH_LEP_PTH_0_60_Tag1");
    case stage1recoTag::RECO_TTH_LEP_PTH_0_60_Tag2:
        return string("RECO_TTH_LEP_PTH_0_60_Tag2");
    case stage1recoTag::RECO_TTH_LEP_PTH_0_60_Tag3:
        return string("RECO_TTH_LEP_PTH_0_60_Tag3");
    case stage1recoTag::RECO_TTH_LEP_PTH_60_120_Tag0:
        return string("RECO_TTH_LEP_PTH_60_120_Tag0");
    case stage1recoTag::RECO_TTH_LEP_PTH_60_120_Tag1:
        return string("RECO_TTH_LEP_PTH_60_120_Tag1");
    case stage1recoTag::RECO_TTH_LEP_PTH_120_200_Tag0:
        return string("RECO_TTH_LEP_PTH_120_200_Tag0");
    case stage1recoTag::RECO_TTH_LEP_PTH_120_200_Tag1:
        return string("RECO_TTH_LEP_PTH_120_200_Tag1");
    case stage1recoTag::RECO_TTH_LEP_PTH_GT200_Tag0:
        return string("RECO_TTH_LEP_PTH_GT200_Tag0");
    case stage1recoTag::RECO_TTH_LEP_PTH_GT200_Tag1:
        return string("RECO_TTH_LEP_PTH_GT200_Tag1");
    case stage1recoTag::RECO_TTH_HAD_PTH_0_60_Tag0:
        return string("RECO_TTH_HAD_PTH_0_60_Tag0");
    case stage1recoTag::RECO_TTH_HAD_PTH_0_60_Tag1:
        return string("RECO_TTH_HAD_PTH_0_60_Tag1");
    case stage1recoTag::RECO_TTH_HAD_PTH_0_60_Tag2:
        return string("RECO_TTH_HAD_PTH_0_60_Tag2");
    case stage1recoTag::RECO_TTH_HAD_PTH_0_60_Tag3:
        return string("RECO_TTH_HAD_PTH_0_60_Tag3");
    case stage1recoTag::RECO_TTH_HAD_PTH_60_120_Tag0:
        return string("RECO_TTH_HAD_PTH_60_120_Tag0");
    case stage1recoTag::RECO_TTH_HAD_PTH_60_120_Tag1:
        return string("RECO_TTH_HAD_PTH_60_120_Tag1");
    case stage1recoTag::RECO_TTH_HAD_PTH_60_120_Tag2:
        return string("RECO_TTH_HAD_PTH_60_120_Tag2");
    case stage1recoTag::RECO_TTH_HAD_PTH_60_120_Tag3:
        return string("RECO_TTH_HAD_PTH_60_120_Tag3");
    case stage1recoTag::RECO_TTH_HAD_PTH_120_200_Tag0:
        return string("RECO_TTH_HAD_PTH_120_200_Tag0");
    case stage1recoTag::RECO_TTH_HAD_PTH_120_200_Tag1:
        return string("RECO_TTH_HAD_PTH_120_200_Tag1");
    case stage1recoTag::RECO_TTH_HAD_PTH_120_200_Tag2:
        return string("RECO_TTH_HAD_PTH_120_200_Tag2");
    case stage1recoTag::RECO_TTH_HAD_PTH_120_200_Tag3:
        return string("RECO_TTH_HAD_PTH_120_200_Tag3");
    case stage1recoTag::RECO_TTH_HAD_PTH_GT200_Tag0:
        return string("RECO_TTH_HAD_PTH_GT200_Tag0");
    case stage1recoTag::RECO_TTH_HAD_PTH_GT200_Tag1:
        return string("RECO_TTH_HAD_PTH_GT200_Tag1");
    case stage1recoTag::RECO_TTH_HAD_PTH_GT200_Tag2:
        return string("RECO_TTH_HAD_PTH_GT200_Tag2");
    case stage1recoTag::RECO_TTH_HAD_PTH_GT200_Tag3:
        return string("RECO_TTH_HAD_PTH_GT200_Tag3");
    case stage1recoTag::RECO_THQ_LEP:
        return string("RECO_THQ_LEP");
    default:
        break;
    }
    return string("TAG NOT ON LIST");
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

