#include "flashgg/Taggers/interface/XYMETCorrection.h"

using namespace flashgg;
//using namespace std;
//using namespace edm;

XYMETCorrection::XYMETCorrection(){}

XYMETCorrection::~XYMETCorrection() {}

enum TheRunEra{y2016B,y2016C,y2016D,y2016E,y2016F,y2016G,y2016H,y2017B,y2017C,y2017D,y2017E,y2017F,y2018A,y2018B,y2018C,y2018D,y2016MC,y2017MC,y2018MC};

std::pair<double,double> XYMETCorrection::METXYCorr_Met_MetPhi(double uncormet, double uncormet_phi, int runnb,unsigned int year, bool isMC, int npv){

    std::pair<double,double>  TheXYCorr_Met_MetPhi(uncormet,uncormet_phi);
  
    if(npv>100) npv=100;
    int runera =-1;
    bool usemetv2 =false;
    if(isMC && year == 2016) runera = y2016MC;
    else if(isMC && year == 2017) {runera = y2017MC; usemetv2 =true;}
    else if(isMC && year == 2018) runera = y2018MC;
    
    else if(!isMC && runnb >=272007 &&runnb<=275376  ) runera = y2016B;
    else if(!isMC && runnb >=275657 &&runnb<=276283  ) runera = y2016C;
    else if(!isMC && runnb >=276315 &&runnb<=276811  ) runera = y2016D;
    else if(!isMC && runnb >=276831 &&runnb<=277420  ) runera = y2016E;
    else if(!isMC && runnb >=277772 &&runnb<=278808  ) runera = y2016F;
    else if(!isMC && runnb >=278820 &&runnb<=280385  ) runera = y2016G;
    else if(!isMC && runnb >=280919 &&runnb<=284044  ) runera = y2016H;
    
    else if(!isMC && runnb >=297020 &&runnb<=299329 ){ runera = y2017B; usemetv2 =true;}
    else if(!isMC && runnb >=299337 &&runnb<=302029 ){ runera = y2017C; usemetv2 =true;}
    else if(!isMC && runnb >=302030 &&runnb<=303434 ){ runera = y2017D; usemetv2 =true;}
    else if(!isMC && runnb >=303435 &&runnb<=304826 ){ runera = y2017E; usemetv2 =true;}
    else if(!isMC && runnb >=304911 &&runnb<=306462 ){ runera = y2017F; usemetv2 =true;}
    
    else if(!isMC && runnb >=315252 &&runnb<=316995 ) runera = y2018A;
    else if(!isMC && runnb >=316998 &&runnb<=319312 ) runera = y2018B;
    else if(!isMC && runnb >=319313 &&runnb<=320393 ) runera = y2018C;
    else if(!isMC && runnb >=320394 &&runnb<=325273 ) runera = y2018D;

    else {
      //Couldn't find data/MC era => no correction applied
      return TheXYCorr_Met_MetPhi;
    }
    
    double METxcorr(0.),METycorr(0.);

    if(!usemetv2){//Current recommendation for 2016 and 2018
      if(runera==y2016B) METxcorr = -(-0.0478335*npv -0.108032);
      if(runera==y2016B) METycorr = -(0.125148*npv +0.355672);
      if(runera==y2016C) METxcorr = -(-0.0916985*npv +0.393247);
      if(runera==y2016C) METycorr = -(0.151445*npv +0.114491);
      if(runera==y2016D) METxcorr = -(-0.0581169*npv +0.567316);
      if(runera==y2016D) METycorr = -(0.147549*npv +0.403088);
      if(runera==y2016E) METxcorr = -(-0.065622*npv +0.536856);
      if(runera==y2016E) METycorr = -(0.188532*npv +0.495346);
      if(runera==y2016F) METxcorr = -(-0.0313322*npv +0.39866);
      if(runera==y2016F) METycorr = -(0.16081*npv +0.960177);
      if(runera==y2016G) METxcorr = -(0.040803*npv -0.290384);
      if(runera==y2016G) METycorr = -(0.0961935*npv +0.666096);
      if(runera==y2016H) METxcorr = -(0.0330868*npv -0.209534);
      if(runera==y2016H) METycorr = -(0.141513*npv +0.816732);
      if(runera==y2017B) METxcorr = -(-0.259456*npv +1.95372);
      if(runera==y2017B) METycorr = -(0.353928*npv -2.46685);
      if(runera==y2017C) METxcorr = -(-0.232763*npv +1.08318);
      if(runera==y2017C) METycorr = -(0.257719*npv -1.1745);
      if(runera==y2017D) METxcorr = -(-0.238067*npv +1.80541);
      if(runera==y2017D) METycorr = -(0.235989*npv -1.44354);
      if(runera==y2017E) METxcorr = -(-0.212352*npv +1.851);
      if(runera==y2017E) METycorr = -(0.157759*npv -0.478139);
      if(runera==y2017F) METxcorr = -(-0.232733*npv +2.24134);
      if(runera==y2017F) METycorr = -(0.213341*npv +0.684588);
      if(runera==y2018A) METxcorr = -(0.362865*npv -1.94505);
      if(runera==y2018A) METycorr = -(0.0709085*npv -0.307365);
      if(runera==y2018B) METxcorr = -(0.492083*npv -2.93552);
      if(runera==y2018B) METycorr = -(0.17874*npv -0.786844);
      if(runera==y2018C) METxcorr = -(0.521349*npv -1.44544);
      if(runera==y2018C) METycorr = -(0.118956*npv -1.96434);
      if(runera==y2018D) METxcorr = -(0.531151*npv -1.37568);
      if(runera==y2018D) METycorr = -(0.0884639*npv -1.57089);
      if(runera==y2016MC) METxcorr = -(-0.195191*npv -0.170948);
      if(runera==y2016MC) METycorr = -(-0.0311891*npv +0.787627);
      if(runera==y2017MC) METxcorr = -(-0.217714*npv +0.493361);
      if(runera==y2017MC) METycorr = -(0.177058*npv -0.336648);
      if(runera==y2018MC) METxcorr = -(0.296713*npv -0.141506);
      if(runera==y2018MC) METycorr = -(0.115685*npv +0.0128193);
    }
    else {//these are the corrections for v2 MET recipe (currently recommended for 2017)
      if(runera==y2016B) METxcorr = -(-0.0374977*npv +0.00488262);
      if(runera==y2016B) METycorr = -(0.107373*npv +-0.00732239);
      if(runera==y2016C) METxcorr = -(-0.0832562*npv +0.550742);
      if(runera==y2016C) METycorr = -(0.142469*npv +-0.153718);
      if(runera==y2016D) METxcorr = -(-0.0400931*npv +0.753734);
      if(runera==y2016D) METycorr = -(0.127154*npv +0.0175228);
      if(runera==y2016E) METxcorr = -(-0.0409231*npv +0.755128);
      if(runera==y2016E) METycorr = -(0.168407*npv +0.126755);
      if(runera==y2016F) METxcorr = -(-0.0161259*npv +0.516919);
      if(runera==y2016F) METycorr = -(0.141176*npv +0.544062);
      if(runera==y2016G) METxcorr = -(0.0583851*npv +-0.0987447);
      if(runera==y2016G) METycorr = -(0.0641427*npv +0.319112);
      if(runera==y2016H) METxcorr = -(0.0706267*npv +-0.13118);
      if(runera==y2016H) METycorr = -(0.127481*npv +0.370786);
      if(runera==y2017B) METxcorr = -(-0.19563*npv +1.51859);
      if(runera==y2017B) METycorr = -(0.306987*npv +-1.84713);
      if(runera==y2017C) METxcorr = -(-0.161661*npv +0.589933);
      if(runera==y2017C) METycorr = -(0.233569*npv +-0.995546);
      if(runera==y2017D) METxcorr = -(-0.180911*npv +1.23553);
      if(runera==y2017D) METycorr = -(0.240155*npv +-1.27449);
      if(runera==y2017E) METxcorr = -(-0.149494*npv +0.901305);
      if(runera==y2017E) METycorr = -(0.178212*npv +-0.535537);
      if(runera==y2017F) METxcorr = -(-0.165154*npv +1.02018);
      if(runera==y2017F) METycorr = -(0.253794*npv +0.75776);
      if(runera==y2018A) METxcorr = -(0.362642*npv +-1.55094);
      if(runera==y2018A) METycorr = -(0.0737842*npv +-0.677209);
      if(runera==y2018B) METxcorr = -(0.485614*npv +-2.45706);
      if(runera==y2018B) METycorr = -(0.181619*npv +-1.00636);
      if(runera==y2018C) METxcorr = -(0.503638*npv +-1.01281);
      if(runera==y2018C) METycorr = -(0.147811*npv +-1.48941);
      if(runera==y2018D) METxcorr = -(0.520265*npv +-1.20322);
      if(runera==y2018D) METycorr = -(0.143919*npv +-0.979328);
      if(runera==y2016MC) METxcorr = -(-0.159469*npv +-0.407022);
      if(runera==y2016MC) METycorr = -(-0.0405812*npv +0.570415);
      if(runera==y2017MC) METxcorr = -(-0.182569*npv +0.276542);
      if(runera==y2017MC) METycorr = -(0.155652*npv +-0.417633);
      if(runera==y2018MC) METxcorr = -(0.299448*npv +-0.13866);
      if(runera==y2018MC) METycorr = -(0.118785*npv +0.0889588);
    }

    double CorrectedMET_x = uncormet *cos( uncormet_phi)+METxcorr;
    double CorrectedMET_y = uncormet *sin( uncormet_phi)+METycorr;
    
    double CorrectedMET = sqrt(CorrectedMET_x*CorrectedMET_x+CorrectedMET_y*CorrectedMET_y);
    double CorrectedMETPhi;
    if(CorrectedMET_x==0 && CorrectedMET_y>0) CorrectedMETPhi = TMath::Pi();
    else if(CorrectedMET_x==0 && CorrectedMET_y<0 )CorrectedMETPhi = -TMath::Pi();
    else if(CorrectedMET_x >0) CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x);
    else if(CorrectedMET_x <0&& CorrectedMET_y>0) CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x) + TMath::Pi();
    else if(CorrectedMET_x <0&& CorrectedMET_y<0) CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x) - TMath::Pi();
    else CorrectedMETPhi =0;
    
    TheXYCorr_Met_MetPhi.first= CorrectedMET;
    TheXYCorr_Met_MetPhi.second= CorrectedMETPhi;
    return TheXYCorr_Met_MetPhi;
    
}

