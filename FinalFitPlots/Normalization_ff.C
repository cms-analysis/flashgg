#include <vector>
#include <map>
#include <iostream>

#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"

double GetBR(double mass) {

  map <double, double> BranchingRatioMap;

  BranchingRatioMap[105.0]=0.102900;
  BranchingRatioMap[105.5]=0.097280;
  BranchingRatioMap[106.0]=0.091960;
  BranchingRatioMap[106.5]=0.086960;
  BranchingRatioMap[107.0]=0.082270;
  BranchingRatioMap[107.5]=0.077870;
  BranchingRatioMap[108.0]=0.073730;
  BranchingRatioMap[108.5]=0.069840;
  BranchingRatioMap[109.0]=0.066190;
  BranchingRatioMap[109.5]=0.062760;
  BranchingRatioMap[110.0]=0.059540;
  BranchingRatioMap[110.5]=0.056510;
  BranchingRatioMap[111.0]=0.053660;
  BranchingRatioMap[111.5]=0.050980;
  BranchingRatioMap[112.0]=0.048460;
  BranchingRatioMap[112.5]=0.046090;
  BranchingRatioMap[113.0]=0.043860;
  BranchingRatioMap[113.5]=0.041760;
  BranchingRatioMap[114.0]=0.039780;
  BranchingRatioMap[114.5]=0.037910;
  BranchingRatioMap[115.0]=0.036150;
  BranchingRatioMap[115.5]=0.034490;
  BranchingRatioMap[116.0]=0.032920;
  BranchingRatioMap[116.5]=0.031430;
  BranchingRatioMap[117.0]=0.030030;
  BranchingRatioMap[117.5]=0.028710;
  BranchingRatioMap[118.0]=0.027460;
  BranchingRatioMap[118.5]=0.026270;
  BranchingRatioMap[119.0]=0.025150;
  BranchingRatioMap[119.5]=0.024080;
  BranchingRatioMap[120.0]=0.023070;
  BranchingRatioMap[120.5]=0.022120;
  BranchingRatioMap[121.0]=0.021210;
  BranchingRatioMap[121.5]=0.020340;
  BranchingRatioMap[122.0]=0.019520;
  BranchingRatioMap[122.5]=0.018740;
  BranchingRatioMap[123.0]=0.018000;
  BranchingRatioMap[123.5]=0.017300;
  BranchingRatioMap[124.0]=0.016630;
  BranchingRatioMap[124.5]=0.015990;
  BranchingRatioMap[125.0]=0.015380;
  BranchingRatioMap[125.5]=0.014800;
  BranchingRatioMap[126.0]=0.014240;
  BranchingRatioMap[126.5]=0.013710;
  BranchingRatioMap[127.0]=0.013210;
  BranchingRatioMap[127.5]=0.012720;
  BranchingRatioMap[128.0]=0.012260;
  BranchingRatioMap[128.5]=0.011820;
  BranchingRatioMap[129.0]=0.011400;
  BranchingRatioMap[129.5]=0.010990;
  BranchingRatioMap[130.0]=0.010610;
  BranchingRatioMap[130.5]=0.010240;
  BranchingRatioMap[131.0]=0.009880;
  BranchingRatioMap[131.5]=0.009539;
  BranchingRatioMap[132.0]=0.009212;
  BranchingRatioMap[132.5]=0.008897;
  BranchingRatioMap[133.0]=0.008595;
  BranchingRatioMap[133.5]=0.008305;
  BranchingRatioMap[134.0]=0.008026;
  BranchingRatioMap[134.5]=0.007758;
  BranchingRatioMap[135.0]=0.007500;
  BranchingRatioMap[135.5]=0.007251;
  BranchingRatioMap[136.0]=0.007012;
  BranchingRatioMap[136.5]=0.006781;
  BranchingRatioMap[137.0]=0.006558;
  BranchingRatioMap[137.5]=0.006344;
  BranchingRatioMap[138.0]=0.006137;
  BranchingRatioMap[138.5]=0.005937;
  BranchingRatioMap[139.0]=0.005744;
  BranchingRatioMap[139.5]=0.005557;
  BranchingRatioMap[140.0]=0.005377;
  BranchingRatioMap[141.0]=0.005030;
  BranchingRatioMap[142.0]=0.004710;
  BranchingRatioMap[143.0]=0.004410;
  BranchingRatioMap[144.0]=0.004120;
  BranchingRatioMap[145.0]=0.003860;
  BranchingRatioMap[146.0]=0.003600;
  BranchingRatioMap[147.0]=0.003360;
  BranchingRatioMap[148.0]=0.003130;
  BranchingRatioMap[149.0]=0.002910;
  BranchingRatioMap[150.0]=0.002700;
  BranchingRatioMap[151.0]=0.002490;
  BranchingRatioMap[152.0]=0.002300;
  BranchingRatioMap[153.0]=0.002110;
  BranchingRatioMap[154.0]=0.001920;
  BranchingRatioMap[155.0]=0.001730;
  BranchingRatioMap[156.0]=0.001550;
  BranchingRatioMap[157.0]=0.001360;
  BranchingRatioMap[158.0]=0.001180;
  BranchingRatioMap[159.0]=0.000991;
  BranchingRatioMap[160.0]=0.000817;

  for (std::map<double, double>::const_iterator iter = BranchingRatioMap.begin();  iter != BranchingRatioMap.end(); ++iter) {
    if (mass==iter->first) return iter->second;
    if (mass>iter->first) {
      double lowmass = iter->first;
      double lowbr = iter->second;
      ++iter;
      if (mass<iter->first) {
        double highmass = iter->first;
        double highbr = iter->second;
        double br = (highbr-lowbr)/(highmass-lowmass)*(mass-lowmass)+lowbr;
        return br;
      }
      --iter;
    }
  }
  
  std::cout << "Warning branching ratio outside range of 105-160GeV!!!!" << std::endl;
  exit(1);
  
}

double GetXsection(double mass) {

  map <double, double> XSectionMap;
  XSectionMap[105.0]=3.034900;
  XSectionMap[105.5]=3.005960;
  XSectionMap[106.0]=2.977020;
  XSectionMap[106.5]=2.948080;
  XSectionMap[107.0]=2.919140;
  XSectionMap[107.5]=2.890200;
  XSectionMap[108.0]=2.861260;
  XSectionMap[108.5]=2.832320;
  XSectionMap[109.0]=2.803380;
  XSectionMap[109.5]=2.774440;
  XSectionMap[110.0]=2.745500;
  XSectionMap[110.5]=2.720680;
  XSectionMap[111.0]=2.695860;
  XSectionMap[111.5]=2.671040;
  XSectionMap[112.0]=2.646220;
  XSectionMap[112.5]=2.621400;
  XSectionMap[113.0]=2.596580;
  XSectionMap[113.5]=2.571760;
  XSectionMap[114.0]=2.546940;
  XSectionMap[114.5]=2.522120;
  XSectionMap[115.0]=2.497300;
  XSectionMap[115.5]=2.476060;
  XSectionMap[116.0]=2.454820;
  XSectionMap[116.5]=2.433580;
  XSectionMap[117.0]=2.412340;
  XSectionMap[117.5]=2.391100;
  XSectionMap[118.0]=2.369860;
  XSectionMap[118.5]=2.348620;
  XSectionMap[119.0]=2.327380;
  XSectionMap[119.5]=2.306140;
  XSectionMap[120.0]=2.284900;
  XSectionMap[120.5]=2.266380;
  XSectionMap[121.0]=2.247860;
  XSectionMap[121.5]=2.229340;
  XSectionMap[122.0]=2.210820;
  XSectionMap[122.5]=2.192300;
  XSectionMap[123.0]=2.173780;
  XSectionMap[123.5]=2.155260;
  XSectionMap[124.0]=2.136740;
  XSectionMap[124.5]=2.118220;
  XSectionMap[125.0]=2.099700;
  XSectionMap[125.5]=2.082990;
  XSectionMap[126.0]=2.066280;
  XSectionMap[126.5]=2.049570;
  XSectionMap[127.0]=2.032860;
  XSectionMap[127.5]=2.016150;
  XSectionMap[128.0]=1.999440;
  XSectionMap[128.5]=1.982730;
  XSectionMap[129.0]=1.966020;
  XSectionMap[129.5]=1.949310;
  XSectionMap[130.0]=1.932600;
  XSectionMap[130.5]=1.917770;
  XSectionMap[131.0]=1.902940;
  XSectionMap[131.5]=1.888110;
  XSectionMap[132.0]=1.873280;
  XSectionMap[132.5]=1.858450;
  XSectionMap[133.0]=1.843620;
  XSectionMap[133.5]=1.828790;
  XSectionMap[134.0]=1.813960;
  XSectionMap[134.5]=1.799130;
  XSectionMap[135.0]=1.784300;
  XSectionMap[135.5]=1.771360;
  XSectionMap[136.0]=1.758420;
  XSectionMap[136.5]=1.745480;
  XSectionMap[137.0]=1.732540;
  XSectionMap[137.5]=1.719600;
  XSectionMap[138.0]=1.706660;
  XSectionMap[138.5]=1.693720;
  XSectionMap[139.0]=1.680780;
  XSectionMap[139.5]=1.667840;
  XSectionMap[140.0]=1.654900;
  XSectionMap[141.0]=1.631200;
  XSectionMap[142.0]=1.607000;
  XSectionMap[143.0]=1.583200;
  XSectionMap[144.0]=1.559700;
  XSectionMap[145.0]=1.537600;
  XSectionMap[146.0]=1.515600;
  XSectionMap[147.0]=1.494400;
  XSectionMap[148.0]=1.473500;
  XSectionMap[149.0]=1.453100;
  XSectionMap[150.0]=1.433100;
  XSectionMap[151.0]=1.413100;
  XSectionMap[152.0]=1.393300;
  XSectionMap[153.0]=1.373700;
  XSectionMap[154.0]=1.354300;
  XSectionMap[155.0]=1.335100;
  XSectionMap[156.0]=1.314800;
  XSectionMap[157.0]=1.295100;
  XSectionMap[158.0]=1.276100;
  XSectionMap[159.0]=1.258100;
  XSectionMap[160.0]=1.241200;

  for (std::map<double, double>::const_iterator iter = XSectionMap.begin();  iter != XSectionMap.end(); ++iter) {
    if (mass==iter->first) return iter->second;
    if (mass>iter->first) {
      double lowmass = iter->first;
      double lowxsec = iter->second;
      ++iter;
      if (mass<iter->first) {
        double highmass = iter->first;
        double highxsec = iter->second;
        double xsec = (highxsec-lowxsec)/(highmass-lowmass)*(mass-lowmass)+lowxsec;
        return xsec;
      }
      --iter;
    }
  }

  std::cout << "Warning branching ratio outside range of 105-160GeV!!!!" << std::endl;
  exit(1);

}

double GetNorm(double mass1, TH1F* hist1, double mass2, TH1F* hist2, double mass) {

  double br = GetBR(mass);
  double br1 = GetBR(mass1);
  double br2 = GetBR(mass2);
  
  double xsec = GetXsection(mass);
  double xsec1 = GetXsection(mass1);
  double xsec2 = GetXsection(mass2);
  
  double alpha = 1.0*(mass-mass1)/(mass2-mass1);
  double effAcc1 = hist1->Integral()/(xsec1*br1);
  double effAcc2 = hist2->Integral()/(xsec2*br2);

  double Normalization = (xsec*br)*(effAcc1 + alpha * (effAcc2 - effAcc1));
  return Normalization;
  
}

void CheckNorm(double Min, double Max, double Step) {

  vector <double> Mass;
  vector <double> BranchingRatio;
  vector <double> XSection;
  for (double i=Min; i<Max; i+=Step) {
    Mass.push_back(i);
    BranchingRatio.push_back(GetBR(i));
    XSection.push_back(GetXsection(i));
  }

  TGraph* BranchGraph = new TGraph(Mass.size(),&Mass[0],&BranchingRatio[0]);
  TGraph* XSectionGraph = new TGraph(Mass.size(),&Mass[0],&XSection[0]);
  BranchGraph->SetTitle("Interpolated Branching Ratios");
  XSectionGraph->SetTitle("Interpolated Cross Sections");
  BranchGraph->SetMarkerStyle(20);
  XSectionGraph->SetMarkerStyle(20);
  BranchGraph->SetMarkerSize(1);
  XSectionGraph->SetMarkerSize(1);

  TCanvas* c1 = new TCanvas("c1","c1",800,650);
  c1->cd();
  BranchGraph->Draw("AP");
  c1->SaveAs("BranchingRatios.png");
  c1->Clear();
  XSectionGraph->Draw("AP");
  c1->SaveAs("XSections.png");

  delete BranchGraph;
  delete XSectionGraph;
  delete c1;

}
