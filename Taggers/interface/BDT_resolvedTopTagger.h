#ifndef _TOPTAGGER_H_
#define _TOPTAGGER_H_

#include <memory>
#include <vector>
#include <string>
#include <utility>
#include "DataFormats/Math/interface/LorentzVector.h"
#include <DataFormats/Math/interface/deltaR.h>
#include "TMVA/Reader.h"
#include <iostream>
#include <algorithm>

namespace flashgg {

typedef math::PtEtaPhiMLorentzVectorD BDT_rTT_ptvec;

class BDT_rTT_Jet : public BDT_rTT_ptvec{
 public:
  BDT_rTT_Jet() : BDT_rTT_ptvec(0,0,0,0){};
  BDT_rTT_Jet(float pt,float eta, float phi, float mass, float deepcsv, float cvsl, float cvsb, float ptD, float axis1, int mult) // pass axis1 = -log(sqrt(...))
    : BDT_rTT_ptvec(pt,eta,phi,mass), _deepcsv(deepcsv), _cvsl(cvsl), _cvsb(cvsb), _ptD(ptD), _axis1(axis1), _mult(mult){};
  float _deepcsv = 0;
  float _cvsl = 0;
  float _cvsb = 0;
  float _ptD = 0;
  float _axis1 = 0;
  int _mult = 0;
  float deepcsv() const {return _deepcsv;}
  float cvsl() const {return _cvsl;}
  float cvsb() const {return _cvsb;}
  float ptD() const {return _ptD;}
  float axis1() const {return _axis1;}
  float mult() const {return _mult;}
};

class BDT_rTT_top {
public:
  BDT_rTT_top(){};
  BDT_rTT_top(std::shared_ptr<BDT_rTT_Jet> _b, std::shared_ptr<BDT_rTT_Jet> _j2, std::shared_ptr<BDT_rTT_Jet> _j3){
    b = _b;

    if (_j2->pt() > _j3->pt()) { j2 = _j2; j3 = _j3; }
    else { j2 = _j3; j3 = _j2; }
    *p4w = *(dynamic_cast<BDT_rTT_ptvec*>(j2.get()))+*(dynamic_cast<BDT_rTT_ptvec*>(j3.get()));
    *p4 = (b!=NULL) ? *p4w+*(dynamic_cast<BDT_rTT_ptvec*>(b.get())) : *p4w;

  }
  std::shared_ptr<BDT_rTT_ptvec> p4 = std::make_shared<BDT_rTT_ptvec>(0,0,0,0);
  std::shared_ptr<BDT_rTT_ptvec> p4w = std::make_shared<BDT_rTT_ptvec>(0,0,0,0);
  std::shared_ptr<BDT_rTT_Jet> b = nullptr;
  std::shared_ptr<BDT_rTT_Jet> j2 = nullptr;
  std::shared_ptr<BDT_rTT_Jet> j3 = nullptr;
  float score = -99;
  int j1idx = -99;
  int j2idx = -99;
  int j3idx = -99;
};

class BDT_resolvedTopTagger {
public:
  BDT_resolvedTopTagger(std::string weight_file_name){
    Init(weight_file_name);
  };
  ~BDT_resolvedTopTagger(){
    clear();
  };
  void addJet(float pt,float eta, float phi, float mass, float deepcsv, float cvsl, float cvsb, float ptD, float axis1, int mult){
    jets.push_back(std::make_shared<BDT_rTT_Jet>(pt,eta,phi,mass,deepcsv,cvsl,cvsb,ptD,axis1,mult));
  };
  void clear();
  void Init(std::string weight_file_name);
  std::vector<float> EvalMVA();
  void setDebug(bool val){debug = val;};

private:

  float EvalScore(const std::shared_ptr<BDT_rTT_top>);

  std::vector<std::shared_ptr<BDT_rTT_Jet>> jets;

  std::shared_ptr<TMVA::Reader> TMVAReader_ = nullptr;

  float var_b_pt = -99;
  float var_b_mass = -99;
  float var_b_ptD = -99;
  float var_b_axis1 = -99;
  float var_b_mult = -99;
  float var_b_csv = -99;
  float var_b_cvsb = -99;
  float var_b_cvsl = -99;

  float var_wj1_pt = -99;
  float var_wj1_mass = -99;
  float var_wj1_ptD = -99;
  float var_wj1_axis1 = -99;
  float var_wj1_mult = -99;
  float var_wj1_csv = -99;
  float var_wj1_cvsb = -99;
  float var_wj1_cvsl = -99;

  float var_wj2_pt = -99;
  float var_wj2_mass = -99;
  float var_wj2_ptD = -99;
  float var_wj2_axis1 = -99;
  float var_wj2_mult = -99;
  float var_wj2_csv = -99;
  float var_wj2_cvsb = -99;
  float var_wj2_cvsl = -99;

  float var_b_wj1_deltaR = -99;
  float var_b_wj1_mass = -99;
  float var_b_wj2_deltaR = -99;
  float var_b_wj2_mass = -99;
  float var_wcand_deltaR = -99;
  float var_wcand_mass = -99;
  float var_b_wcand_deltaR = -99;
  float var_topcand_mass = -99;

  bool debug = false;

};

inline
void BDT_resolvedTopTagger::Init(std::string weight_file_name){

  TMVAReader_ = std::make_shared<TMVA::Reader>( "!Color:!Silent" );

  TMVAReader_->AddVariable("var_b_pt",&var_b_pt);
  TMVAReader_->AddVariable("var_b_mass",&var_b_mass);
  TMVAReader_->AddVariable("var_b_ptD",&var_b_ptD);
  TMVAReader_->AddVariable("var_b_axis1",&var_b_axis1);
  TMVAReader_->AddVariable("var_b_mult",&var_b_mult);
  TMVAReader_->AddVariable("var_b_deepcsv_bvsall",&var_b_csv);
  TMVAReader_->AddVariable("var_b_deepcsv_cvsb",&var_b_cvsb);
  TMVAReader_->AddVariable("var_b_deepcsv_cvsl",&var_b_cvsl);

  TMVAReader_->AddVariable("var_wj1_pt",&var_wj1_pt);
  TMVAReader_->AddVariable("var_wj1_mass",&var_wj1_mass);
  TMVAReader_->AddVariable("var_wj1_ptD",&var_wj1_ptD);
  TMVAReader_->AddVariable("var_wj1_axis1",&var_wj1_axis1);
  TMVAReader_->AddVariable("var_wj1_mult",&var_wj1_mult);
  TMVAReader_->AddVariable("var_wj1_deepcsv_bvsall",&var_wj1_csv);
  TMVAReader_->AddVariable("var_wj1_deepcsv_cvsb",&var_wj1_cvsb);
  TMVAReader_->AddVariable("var_wj1_deepcsv_cvsl",&var_wj1_cvsl);

  TMVAReader_->AddVariable("var_wj2_pt",&var_wj2_pt);
  TMVAReader_->AddVariable("var_wj2_mass",&var_wj2_mass);
  TMVAReader_->AddVariable("var_wj2_ptD",&var_wj2_ptD);
  TMVAReader_->AddVariable("var_wj2_axis1",&var_wj2_axis1);
  TMVAReader_->AddVariable("var_wj2_mult",&var_wj2_mult);
  TMVAReader_->AddVariable("var_wj2_deepcsv_bvsall",&var_wj2_csv);
  TMVAReader_->AddVariable("var_wj2_deepcsv_cvsb",&var_wj2_cvsb);
  TMVAReader_->AddVariable("var_wj2_deepcsv_cvsl",&var_wj2_cvsl);

  TMVAReader_->AddVariable("var_b_wj1_deltaR",&var_b_wj1_deltaR);
  TMVAReader_->AddVariable("var_b_wj1_mass",&var_b_wj1_mass);
  TMVAReader_->AddVariable("var_b_wj2_deltaR",&var_b_wj2_deltaR);
  TMVAReader_->AddVariable("var_b_wj2_mass",&var_b_wj2_mass);
  TMVAReader_->AddVariable("var_wcand_deltaR",&var_wcand_deltaR);
  TMVAReader_->AddVariable("var_wcand_mass",&var_wcand_mass);
  TMVAReader_->AddVariable("var_b_wcand_deltaR",&var_b_wcand_deltaR);
  TMVAReader_->AddVariable("var_topcand_mass",&var_topcand_mass);

  TMVAReader_->BookMVA("BDT",weight_file_name);

};

inline
void BDT_resolvedTopTagger::clear(){

  jets.clear();

  var_b_pt = -99;
  var_b_mass = -99;
  var_b_ptD = -99;
  var_b_axis1 = -99;
  var_b_mult = -99;
  var_b_csv = -99;
  var_b_cvsb = -99;
  var_b_cvsl = -99;

  var_wj1_pt = -99;
  var_wj1_mass = -99;
  var_wj1_ptD = -99;
  var_wj1_axis1 = -99;
  var_wj1_mult = -99;
  var_wj1_csv = -99;
  var_wj1_cvsb = -99;
  var_wj1_cvsl = -99;

  var_wj2_pt = -99;
  var_wj2_mass = -99;
  var_wj2_ptD = -99;
  var_wj2_axis1 = -99;
  var_wj2_mult = -99;
  var_wj2_csv = -99;
  var_wj2_cvsb = -99;
  var_wj2_cvsl = -99;

  var_b_wj1_deltaR = -99;
  var_b_wj1_mass = -99;
  var_b_wj2_deltaR = -99;
  var_b_wj2_mass = -99;
  var_wcand_deltaR = -99;
  var_wcand_mass = -99;
  var_b_wcand_deltaR = -99;
  var_topcand_mass = -99;

}

inline
std::vector<float> BDT_resolvedTopTagger::EvalMVA(){

  int njets = jets.size();
  std::sort(jets.begin(),jets.end(),[](const std::shared_ptr<BDT_rTT_Jet> &a, const std::shared_ptr<BDT_rTT_Jet> &b){return a->deepcsv() > b->deepcsv();});

  std::vector<std::shared_ptr<BDT_rTT_top>> allcands;

  for (int i1=0; i1<njets-2; i1++) {
    for (int i2=i1+1; i2<njets-1; i2++){
      for (int i3=i2+1; i3<njets; i3++){
	auto topcand = std::make_shared<BDT_rTT_top>(jets.at(i1),jets.at(i2),jets.at(i3));
        if ((fabs(topcand->p4->mass()-175)>80)) continue;

        topcand->score = EvalScore(topcand);
        topcand->j1idx = i1;
        topcand->j2idx = i2;
        topcand->j3idx = i3;
        allcands.push_back(topcand);
      }
    }
  }

  if (debug) std::cout << "njets " << njets << std::endl;

  std::vector<float> output(16,-99);
  if (allcands.size()>0) {
    auto top = *std::min_element(allcands.begin(),allcands.end(),[](const std::shared_ptr<BDT_rTT_top> &a, const std::shared_ptr<BDT_rTT_top> &b){return a->score > b->score;});
    output.at(0) = top->score; // mvaValue
    output.at(1) = top->p4->pt(); // HadTop_pt
    output.at(2) = top->p4->eta(); // HadTop_eta
    output.at(3) = top->p4->phi(); // HadTop_phi
    output.at(4) = top->p4->mass(); // HadTop_mass
    output.at(5) = top->p4w->pt(); // W_fromHadTop_pt
    output.at(6) = top->p4w->eta(); // W_fromHadTop_eta
    output.at(7) = top->p4w->phi(); // W_fromHadTop_phi
    output.at(8) = top->p4w->mass(); // W_fromHadTop_mass
    output.at(9) = std::max(top->j2->deepcsv(),top->j3->deepcsv()); // W_fromHadTop_maxCSVjj
    output.at(10) = deltaR(top->j2->eta(),top->j2->phi(),top->j3->eta(),top->j3->phi()); // W_fromHadTop_dRjj
    output.at(11) = deltaR(top->b->eta(),top->b->phi(),top->p4w->eta(),top->p4w->phi()); // W_fromHadTop_dRb
    output.at(12) = top->b->deepcsv(); // b_fromHadTop_CSV
    output.at(13) = top->j1idx;
    output.at(14) = top->j2idx;
    output.at(15) = top->j3idx;
  }
  if (debug) std::cout << "returning " << output.at(0) << std::endl;
  return output;

};

inline
float BDT_resolvedTopTagger::EvalScore(const std::shared_ptr<BDT_rTT_top> top){

  var_b_pt = top->b->pt();
  var_b_mass = top->b->mass();
  var_b_ptD = top->b->ptD();
  var_b_axis1 = std::exp(-top->b->axis1()); // training uses definition of axis1 without -log
  var_b_mult = top->b->mult();
  var_b_csv = top->b->deepcsv();
  var_b_cvsb = top->b->cvsb();
  var_b_cvsl = top->b->cvsl();

  var_wj1_pt = top->j2->pt();
  var_wj1_mass = top->j2->mass();
  var_wj1_ptD = top->j2->ptD();
  var_wj1_axis1 = std::exp(-top->j2->axis1()); // training uses definition of axis1 without -log
  var_wj1_mult = top->j2->mult();
  var_wj1_csv = top->j2->deepcsv();
  var_wj1_cvsb = top->j2->cvsb();
  var_wj1_cvsl = top->j2->cvsl();

  var_wj2_pt = top->j3->pt();
  var_wj2_mass = top->j3->mass();
  var_wj2_ptD = top->j3->ptD();
  var_wj2_axis1 = std::exp(-top->j3->axis1()); // training uses definition of axis1 without -log
  var_wj2_mult = top->j3->mult();
  var_wj2_csv = top->j3->deepcsv();
  var_wj2_cvsb = top->j3->cvsb();
  var_wj2_cvsl = top->j3->cvsl();

  var_b_wj1_deltaR = deltaR(top->b->eta(),top->b->phi(),top->j2->eta(),top->j2->phi());
  var_b_wj1_mass = (*(dynamic_cast<BDT_rTT_ptvec*>(top->b.get()))+*(dynamic_cast<BDT_rTT_ptvec*>(top->j2.get()))).mass();
  var_b_wj2_deltaR = deltaR(top->b->eta(),top->b->phi(),top->j3->eta(),top->j3->phi());
  var_b_wj2_mass = (*(dynamic_cast<BDT_rTT_ptvec*>(top->b.get()))+*(dynamic_cast<BDT_rTT_ptvec*>(top->j3.get()))).mass();
  var_wcand_deltaR = deltaR(top->j2->eta(),top->j2->phi(),top->j3->eta(),top->j3->phi());
  var_wcand_mass = top->p4w->mass();
  var_b_wcand_deltaR = deltaR(top->b->eta(),top->b->phi(),top->p4w->eta(),top->p4w->phi());
  var_topcand_mass = top->p4->mass();

  float score = TMVAReader_->EvaluateMVA("BDT");

  if (debug) {
    std::cout <<  var_b_pt << " " ;
    std::cout <<  var_b_mass << " " ;
    std::cout <<  var_b_ptD << " " ;
    std::cout <<  var_b_axis1 << " " ;
    std::cout <<  var_b_mult << " " ;
    std::cout <<  var_b_csv << " " ;
    std::cout <<  var_b_cvsb << " " ;
    std::cout <<  var_b_cvsl << " " ;

    std::cout <<  var_wj1_pt << " " ;
    std::cout <<  var_wj1_mass << " " ;
    std::cout <<  var_wj1_ptD << " " ;
    std::cout <<  var_wj1_axis1 << " " ;
    std::cout <<  var_wj1_mult << " " ;
    std::cout <<  var_wj1_csv << " " ;
    std::cout <<  var_wj1_cvsb << " " ;
    std::cout <<  var_wj1_cvsl << " " ;

    std::cout <<  var_wj2_pt << " " ;
    std::cout <<  var_wj2_mass << " " ;
    std::cout <<  var_wj2_ptD << " " ;
    std::cout <<  var_wj2_axis1 << " " ;
    std::cout <<  var_wj2_mult << " " ;
    std::cout <<  var_wj2_csv << " " ;
    std::cout <<  var_wj2_cvsb << " " ;
    std::cout <<  var_wj2_cvsl << " " ;

    std::cout <<  var_b_wj1_deltaR << " " ;
    std::cout <<  var_b_wj1_mass << " " ;
    std::cout <<  var_b_wj2_deltaR << " " ;
    std::cout <<  var_b_wj2_mass << " " ;
    std::cout <<  var_wcand_deltaR << " " ;
    std::cout <<  var_wcand_mass << " " ;
    std::cout <<  var_b_wcand_deltaR << " " ;
    std::cout <<  var_topcand_mass << " " << std::endl;
    std::cout << score << std::endl;
  }

  return score;

};

}

#endif // _TOPTAGGER_H_
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
