#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/StageOneTag.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

StageOneTag::StageOneTag() {}

StageOneTag::~StageOneTag() {}

StageOneTag::StageOneTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes ) :
    StageOneTag::StageOneTag( diPho, *mvaRes ) {}

StageOneTag::StageOneTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) :
    DiPhotonTagBase::DiPhotonTagBase( dipho, mvares ) {
}

StageOneTag::StageOneTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes, edm::Ptr<VBFDiPhoDiJetMVAResult> vbfDiPhoDiJet_mvaRes ) :
    StageOneTag::StageOneTag( diPho, *mvaRes, *vbfDiPhoDiJet_mvaRes ) {}

StageOneTag::StageOneTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares, VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mvaRes ) :
    StageOneTag::StageOneTag( dipho, mvares )
{
    vbfDiPhoDiJet_mva_result_ = vbfDiPhoDiJet_mvaRes;
}

const VBFDiPhoDiJetMVAResult StageOneTag::VBFDiPhoDiJetMVA() const
{
    return vbfDiPhoDiJet_mva_result_;
}
const VBFMVAResult StageOneTag::VBFMVA() const
{
    return vbfDiPhoDiJet_mva_result_.vbfMvaResult;
}

void StageOneTag::computeStage1Kinematics( const edm::Ptr<flashgg::Jet> j0, const edm::Ptr<flashgg::Jet> j1, float ptV, float lepeta1, float lepphi1, float lepeta2, float lepphi2 ) {
    stage1recoTag_ = stage1recoTag::LOGICERROR;
    float ptH = this->diPhoton()->pt();
    unsigned int nJ = 0;
    float mjj = 0.;
    float ptHjj = 0.;
    float mvaScore = this->diPhotonMVA().mvaValue(); //FIXME for compilation
    //float mvaScore = this->diPhotonMVA().xgbMvaValue(); // modified to give the xgboost score
    mvaScore = 1. / ( 1. + exp( 0.5*log( 2./(mvaScore+1.) - 1 ) ) ); //invert this: https://github.com/jpata/mlglue/blob/master/mlglue/tree.py#L400-L409
    float dijetScore = this->VBFMVA().VBFMVAValue();
    float leadMvaScore = this->diPhotonMVA().leadmva;
    float subleadMvaScore = this->diPhotonMVA().subleadmva;
    float leadPToM = this->diPhotonMVA().leadptom;
    float subleadPToM = this->diPhotonMVA().subleadptom;
    
    if ( !j0.isNull() ) {
        if ( j0->pt() > 30. ) { nJ += 1; }
    }
    if ( !j1.isNull() ) {
        if ( j1->pt() > 30. ) { nJ += 1; }
    }

    unsigned nlep = 0;
    if (lepphi1 > -998. ) nlep++;
    if (lepphi2 > -998. ) nlep++;
    string nlepstring = std::to_string(nlep)+"LEP";

    if ( nJ >= 2 ) {
        mjj = ( j0->p4() + j1->p4() ).mass();
        ptHjj = ( j0->p4() + j1->p4() + this->diPhoton()->p4() ).pt();
    }
    // have now added two categories for each RECO tag, using the moment diphoton MVA, with boundaries currently hard-coded below..
    if ( ptV < -0.5 ) {
        if (nJ == 0) {
            if (mvaScore > 0.851) {
                stage1recoTag_ = stage1recoTag::RECO_0J_Tag0;
            }
            else if (mvaScore > 0.796) {
                stage1recoTag_ = stage1recoTag::RECO_0J_Tag1;
            }
            else if (mvaScore > 0.586) {
                stage1recoTag_ = stage1recoTag::RECO_0J_Tag2;
            }
            else { 
                stage1recoTag_ = stage1recoTag::NOTAG;
            }
        } else if ( nJ == 1 ) {
            if ( ptH > 200 ) {
                if (mvaScore > 0.917) {
                    stage1recoTag_ = stage1recoTag::RECO_1J_PTH_GT200;
                }
                else { 
                    stage1recoTag_ = stage1recoTag::NOTAG;
                }
            } else if ( ptH > 120. ) {
                if (mvaScore > 0.908) {
                    stage1recoTag_ = stage1recoTag::RECO_1J_PTH_120_200_Tag0;
                }
                else if (mvaScore > 0.810) {
                    stage1recoTag_ = stage1recoTag::RECO_1J_PTH_120_200_Tag1;
                }
                else { 
                    stage1recoTag_ = stage1recoTag::NOTAG;
                }
            } else if ( ptH > 60. ) {
                if (mvaScore > 0.866) {
                    stage1recoTag_ = stage1recoTag::RECO_1J_PTH_60_120_Tag0;
                }
                else if (mvaScore > 0.749) {
                    stage1recoTag_ = stage1recoTag::RECO_1J_PTH_60_120_Tag1;
                }
                else { 
                    stage1recoTag_ = stage1recoTag::NOTAG;
                }
            } else {
                if (mvaScore > 0.832) {
                    stage1recoTag_ = stage1recoTag::RECO_1J_PTH_0_60_Tag0;
                }
                else if (mvaScore > 0.718) {
                    stage1recoTag_ = stage1recoTag::RECO_1J_PTH_0_60_Tag1;
                }
                else { 
                    stage1recoTag_ = stage1recoTag::NOTAG;
                }
            }
        } else { // 2 jets
            bool reProcess = false;
            if ( mjj > 400. && j0->p4().pt() > 40. && j1->p4().pt() > 30. && leadMvaScore > -0.2 && subleadMvaScore > -0.2 ) { //cuts optimised using data-driven dijet BDT plus new diphoton BDT
                if ( j0->p4().pt() > 200. ) {
                    if (dijetScore > -0.412 && mvaScore > 0.728) {
                        stage1recoTag_ = stage1recoTag::RECO_VBFTOPO_BSM;
                    }
                    else { 
                        reProcess = true;
                    }
                }
                else if ( ptHjj > 0. && ptHjj < 25.) {
                    if (dijetScore > 0.120 && mvaScore > 0.623) {
                        stage1recoTag_ = stage1recoTag::RECO_VBFTOPO_JET3VETO_Tag0;
                    }
                    else if (dijetScore > -0.890 && mvaScore > 0.720) {
                        stage1recoTag_ = stage1recoTag::RECO_VBFTOPO_JET3VETO_Tag1;
                    }
                    else { 
                        reProcess = true;
                    }
                } else if ( ptHjj > 25. ) {
                    if (dijetScore > 0.481 && mvaScore > 0.607) {
                        stage1recoTag_ = stage1recoTag::RECO_VBFTOPO_JET3_Tag0;
                    }
                    else if (dijetScore > -0.844 && mvaScore > 0.739) {
                        stage1recoTag_ = stage1recoTag::RECO_VBFTOPO_JET3_Tag1;
                    }
                    else { 
                        reProcess = true;
                    }
                }
            }
            else if ( mjj > 250. && j0->p4().pt() > 40. && j1->p4().pt() > 30. && leadMvaScore > -0.2 && subleadMvaScore > -0.2 ) { //cuts optimised using data-driven dijet BDT plus new diphoton BDT
                if ( j0->p4().pt() > 200. ) {
                    if (dijetScore > -0.412 && mvaScore > 0.728) {
                        stage1recoTag_ = stage1recoTag::RECO_VBFTOPO_BSM;
                    }
                    else { 
                        reProcess = true;
                    }
                }
                else if (dijetScore > -0.737 && mvaScore > 0.768) {
                    stage1recoTag_ = stage1recoTag::RECO_VBFTOPO_REST;
                }
                else { 
                    reProcess = true;
                }
            }
            else {
               reProcess = true;
            }
            if ( reProcess ) {
                if ( ptH > 200 ) {
                    if (mvaScore > 0.938) {
                        stage1recoTag_ = stage1recoTag::RECO_GE2J_PTH_GT200_Tag0;
                    }
                    else if (mvaScore > 0.865) {
                        stage1recoTag_ = stage1recoTag::RECO_GE2J_PTH_GT200_Tag1;
                    }
                    else { 
                        stage1recoTag_ = stage1recoTag::NOTAG;
                    }
                } else if ( ptH > 120. ) {
                    if (mvaScore > 0.910) {
                        stage1recoTag_ = stage1recoTag::RECO_GE2J_PTH_120_200_Tag0;
                    }
                    else if (mvaScore > 0.811) {
                        stage1recoTag_ = stage1recoTag::RECO_GE2J_PTH_120_200_Tag1;
                    }
                    else { 
                        stage1recoTag_ = stage1recoTag::NOTAG;
                    }
                } else if ( ptH > 60. ) {
                    if (mvaScore > 0.869) {
                        stage1recoTag_ = stage1recoTag::RECO_GE2J_PTH_60_120_Tag0;
                    }
                    else if (mvaScore > 0.757) {
                        stage1recoTag_ = stage1recoTag::RECO_GE2J_PTH_60_120_Tag1;
                    }
                    else { 
                        stage1recoTag_ = stage1recoTag::NOTAG;
                    }
                } else {
                    if (mvaScore > 0.833) {
                        stage1recoTag_ = stage1recoTag::RECO_GE2J_PTH_0_60_Tag0;
                    }
                    else if (mvaScore > 0.709) {
                        stage1recoTag_ = stage1recoTag::RECO_GE2J_PTH_0_60_Tag1;
                    }
                    else { 
                        stage1recoTag_ = stage1recoTag::NOTAG;
                    }
                }
            }
        }
    } else { // Leptonic vector boson assigned. Leave this up to existing VH tags for now
        stage1recoTag_ = stage1recoTag::NOTAG;
    }
    // reject events not passing the scaled pT cuts
    if ( leadPToM < 1./3. || subleadPToM < 1./4. ) {
        stage1recoTag_ = stage1recoTag::NOTAG;
    }
}

string StageOneTag::stage1KinematicLabel() const { 
    switch(stage1recoTag_) {
    case stage1recoTag::LOGICERROR:
        return string("LOGICERROR");
    case stage1recoTag::NOTAG:
        return string("NOTAG");
    case stage1recoTag::RECO_0J_Tag0:
        return string("RECO_0J_Tag0");
    case stage1recoTag::RECO_0J_Tag1:
        return string("RECO_0J_Tag1");
    case stage1recoTag::RECO_0J_Tag2:
        return string("RECO_0J_Tag2");
    case stage1recoTag::RECO_1J_PTH_0_60_Tag0:
        return string("RECO_1J_PTH_0_60_Tag0");
    case stage1recoTag::RECO_1J_PTH_0_60_Tag1:
        return string("RECO_1J_PTH_0_60_Tag1");
    case stage1recoTag::RECO_1J_PTH_60_120_Tag0:
        return string("RECO_1J_PTH_60_120_Tag0");
    case stage1recoTag::RECO_1J_PTH_60_120_Tag1:
        return string("RECO_1J_PTH_60_120_Tag1");
    case stage1recoTag::RECO_1J_PTH_120_200_Tag0:
        return string("RECO_1J_PTH_120_200_Tag0");
    case stage1recoTag::RECO_1J_PTH_120_200_Tag1:
        return string("RECO_1J_PTH_120_200_Tag1");
    case stage1recoTag::RECO_1J_PTH_GT200:
        return string("RECO_1J_PTH_GT200");
    case stage1recoTag::RECO_GE2J_PTH_0_60_Tag0:
        return string("RECO_GE2J_PTH_0_60_Tag0");
    case stage1recoTag::RECO_GE2J_PTH_0_60_Tag1:
        return string("RECO_GE2J_PTH_0_60_Tag1");
    case stage1recoTag::RECO_GE2J_PTH_60_120_Tag0:
        return string("RECO_GE2J_PTH_60_120_Tag0");
    case stage1recoTag::RECO_GE2J_PTH_60_120_Tag1:
        return string("RECO_GE2J_PTH_60_120_Tag1");
    case stage1recoTag::RECO_GE2J_PTH_120_200_Tag0:
        return string("RECO_GE2J_PTH_120_200_Tag0");
    case stage1recoTag::RECO_GE2J_PTH_120_200_Tag1:
        return string("RECO_GE2J_PTH_120_200_Tag1");
    case stage1recoTag::RECO_GE2J_PTH_GT200_Tag0:
        return string("RECO_GE2J_PTH_GT200_Tag0");
    case stage1recoTag::RECO_GE2J_PTH_GT200_Tag1:
        return string("RECO_GE2J_PTH_GT200_Tag1");
    case stage1recoTag::RECO_VBFTOPO_JET3VETO_Tag0:
        return string("RECO_VBFTOPO_JET3VETO_Tag0");
    case stage1recoTag::RECO_VBFTOPO_JET3VETO_Tag1:
        return string("RECO_VBFTOPO_JET3VETO_Tag1");
    case stage1recoTag::RECO_VBFTOPO_JET3_Tag0:
        return string("RECO_VBFTOPO_JET3_Tag0");
    case stage1recoTag::RECO_VBFTOPO_JET3_Tag1:
        return string("RECO_VBFTOPO_JET3_Tag1");
    case stage1recoTag::RECO_VBFTOPO_REST:
        return string("RECO_VBFTOPO_REST");
    case stage1recoTag::RECO_VBFTOPO_BSM:
        return string("RECO_VBFTOPO_BSM");
    case stage1recoTag::RECO_WHLEP:
        return string("RECO_WHLEP");
    case stage1recoTag::RECO_ZHLEP:
        return string("RECO_ZHLEP");
    case stage1recoTag::RECO_VHLEPLOOSE:
        return string("RECO_VHLEPLOOSE");
    case stage1recoTag::RECO_VHMET:
        return string("RECO_VHMET");
    case stage1recoTag::RECO_VHHAD:
        return string("RECO_VHHAD");
    case stage1recoTag::RECO_TTH_LEP:
        return string("RECO_TTH_LEP");
    case stage1recoTag::RECO_TTH_HAD:
        return string("RECO_TTH_HAD");
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

