#include "Normalization_8TeV.h"

#include "TSystem.h"

Normalization_8TeV::Normalization_8TeV(){
}

void Normalization_8TeV::Init(int sqrtS){

    //TPython::Exec("import $(CMSSW_BASE).src.h2gglobe.AnalysisScripts.AnalysisScripts.python.buildSMHiggsSignalXSBR");
    TPython::Exec("import os,imp");
    const char * env = gSystem->Getenv("H2GGLOBE_RUNTIME");
    std::string globeRt = ( env != 0 ? env : H2GGLOBE_BASE "/AnalysisScripts");
    if( ! TPython::Exec(Form("buildSMHiggsSignalXSBR = imp.load_source('*', '%s/python/buildSMHiggsSignalXSBR.py')",globeRt.c_str())) ) {
	    return;
    }
    TPython::Eval(Form("buildSMHiggsSignalXSBR.Init%dTeV()", sqrtS));
    
    for (double mH=90.0;mH<=250.0;mH+=0.1){ // Do we need this up to 250 ?
	double valBR    =  (double)TPython::Eval(Form("buildSMHiggsSignalXSBR.getBR(%f)",mH));
	double valXSggH =  (double)TPython::Eval(Form("buildSMHiggsSignalXSBR.getXS(%f,'%s')",mH,"ggH"));
	double valXSqqH =  (double)TPython::Eval(Form("buildSMHiggsSignalXSBR.getXS(%f,'%s')",mH,"qqH"));
	double valXSttH =  (double)TPython::Eval(Form("buildSMHiggsSignalXSBR.getXS(%f,'%s')",mH,"ttH"));
	double valXSWH  =  (double)TPython::Eval(Form("buildSMHiggsSignalXSBR.getXS(%f,'%s')",mH,"WH"));
	double valXSZH  =  (double)TPython::Eval(Form("buildSMHiggsSignalXSBR.getXS(%f,'%s')",mH,"ZH"));
	BranchingRatioMap[mH]	= valBR;
        XSectionMap_ggh[mH]	= valXSggH; 	
        XSectionMap_vbf[mH]	= valXSqqH; 	
        XSectionMap_tth[mH]	= valXSttH; 	
        XSectionMap_wh[mH]	= valXSWH; 	
        XSectionMap_zh[mH]	= valXSZH;	
        XSectionMap_wzh[mH]	= valXSWH+valXSZH;	
	
    }

    //Graviton X-Sections - assume the same as SM
    for (std::map<double, double>::const_iterator iter = XSectionMap_ggh.begin(); iter != XSectionMap_ggh.end(); ++iter)
      XSectionMap_sm[iter->first]=iter->second+XSectionMap_vbf[iter->first]+XSectionMap_wzh[iter->first]+XSectionMap_tth[iter->first];
   
}


void Normalization_8TeV::FillSignalTypes(){

  SignalTypeMap[-73]=std::make_pair<TString,double>("ggh",124);
  SignalTypeMap[-74]=std::make_pair<TString,double>("vbf",124);
  SignalTypeMap[-76]=std::make_pair<TString,double>("wzh",124);
  SignalTypeMap[-75]=std::make_pair<TString,double>("tth",124);
  SignalTypeMap[-77]=std::make_pair<TString,double>("ggh",126);
  SignalTypeMap[-78]=std::make_pair<TString,double>("vbf",126);
  SignalTypeMap[-80]=std::make_pair<TString,double>("wzh",126);
  SignalTypeMap[-79]=std::make_pair<TString,double>("tth",126);
  
  SignalTypeMap[-57]=std::make_pair<TString,double>("ggh",123);
  SignalTypeMap[-58]=std::make_pair<TString,double>("vbf",123);
  SignalTypeMap[-60]=std::make_pair<TString,double>("wzh",123);
  SignalTypeMap[-59]=std::make_pair<TString,double>("tth",123);
  SignalTypeMap[-53]=std::make_pair<TString,double>("ggh",121);
  SignalTypeMap[-54]=std::make_pair<TString,double>("vbf",121);
  SignalTypeMap[-56]=std::make_pair<TString,double>("wzh",121);
  SignalTypeMap[-55]=std::make_pair<TString,double>("tth",121);
  SignalTypeMap[-65]=std::make_pair<TString,double>("ggh",160);
  SignalTypeMap[-66]=std::make_pair<TString,double>("vbf",160);
  SignalTypeMap[-68]=std::make_pair<TString,double>("wzh",160);
  SignalTypeMap[-67]=std::make_pair<TString,double>("tth",160);
  SignalTypeMap[-61]=std::make_pair<TString,double>("ggh",155);
  SignalTypeMap[-62]=std::make_pair<TString,double>("vbf",155);
  SignalTypeMap[-64]=std::make_pair<TString,double>("wzh",155);
  SignalTypeMap[-63]=std::make_pair<TString,double>("tth",155);
  SignalTypeMap[-49]=std::make_pair<TString,double>("ggh",150);
  SignalTypeMap[-50]=std::make_pair<TString,double>("vbf",150);
  SignalTypeMap[-52]=std::make_pair<TString,double>("wzh",150);
  SignalTypeMap[-51]=std::make_pair<TString,double>("tth",150);
  SignalTypeMap[-45]=std::make_pair<TString,double>("ggh",145);
  SignalTypeMap[-46]=std::make_pair<TString,double>("vbf",145);
  SignalTypeMap[-48]=std::make_pair<TString,double>("wzh",145);
  SignalTypeMap[-47]=std::make_pair<TString,double>("tth",145);
  SignalTypeMap[-33]=std::make_pair<TString,double>("ggh",140);
  SignalTypeMap[-34]=std::make_pair<TString,double>("vbf",140);
  SignalTypeMap[-36]=std::make_pair<TString,double>("wzh",140);
  SignalTypeMap[-35]=std::make_pair<TString,double>("tth",140);
  SignalTypeMap[-41]=std::make_pair<TString,double>("ggh",135);
  SignalTypeMap[-42]=std::make_pair<TString,double>("vbf",135);
  SignalTypeMap[-44]=std::make_pair<TString,double>("wzh",135);
  SignalTypeMap[-43]=std::make_pair<TString,double>("tth",135);
  SignalTypeMap[-29]=std::make_pair<TString,double>("ggh",130);
  SignalTypeMap[-30]=std::make_pair<TString,double>("vbf",130);
  SignalTypeMap[-32]=std::make_pair<TString,double>("wzh",130);
  SignalTypeMap[-31]=std::make_pair<TString,double>("tth",130);
  SignalTypeMap[-37]=std::make_pair<TString,double>("ggh",125);
  SignalTypeMap[-38]=std::make_pair<TString,double>("vbf",125);
  SignalTypeMap[-40]=std::make_pair<TString,double>("wzh",125);
  SignalTypeMap[-39]=std::make_pair<TString,double>("tth",125);
  SignalTypeMap[-25]=std::make_pair<TString,double>("ggh",120);
  SignalTypeMap[-26]=std::make_pair<TString,double>("vbf",120);
  SignalTypeMap[-28]=std::make_pair<TString,double>("wzh",120);
  SignalTypeMap[-27]=std::make_pair<TString,double>("tth",120);
  SignalTypeMap[-21]=std::make_pair<TString,double>("ggh",115);
  SignalTypeMap[-22]=std::make_pair<TString,double>("vbf",115);
  SignalTypeMap[-24]=std::make_pair<TString,double>("wzh",115);
  SignalTypeMap[-23]=std::make_pair<TString,double>("tth",115);
  SignalTypeMap[-17]=std::make_pair<TString,double>("ggh",110);
  SignalTypeMap[-18]=std::make_pair<TString,double>("vbf",110);
  SignalTypeMap[-19]=std::make_pair<TString,double>("wzh",110);
  SignalTypeMap[-20]=std::make_pair<TString,double>("tth",110);
  SignalTypeMap[-13]=std::make_pair<TString,double>("ggh",105);
  SignalTypeMap[-14]=std::make_pair<TString,double>("vbf",105);
  SignalTypeMap[-16]=std::make_pair<TString,double>("wzh",105);
  SignalTypeMap[-15]=std::make_pair<TString,double>("tth",105);

  SignalTypeMap[-69]=std::make_pair<TString,double>("ggh",100);
  SignalTypeMap[-70]=std::make_pair<TString,double>("vbf",100);
  SignalTypeMap[-72]=std::make_pair<TString,double>("wzh",100);
  SignalTypeMap[-71]=std::make_pair<TString,double>("tth",100);

  SignalTypeMap[-81]=std::make_pair<TString,double>("ggh",90);
  SignalTypeMap[-82]=std::make_pair<TString,double>("vbf",90);
  SignalTypeMap[-84]=std::make_pair<TString,double>("wzh",90);
  SignalTypeMap[-83]=std::make_pair<TString,double>("tth",90);

  SignalTypeMap[-101]=std::make_pair<TString,double>("ggh",125);
  SignalTypeMap[-102]=std::make_pair<TString,double>("ggh",125);
  SignalTypeMap[-103]=std::make_pair<TString,double>("ggh",125);

  SignalTypeMap[-137]=std::make_pair<TString,double>("gg_grav",125);
  SignalTypeMap[-138]=std::make_pair<TString,double>("qq_grav",125);
  SignalTypeMap[-177]=std::make_pair<TString,double>("gg_grav",126);
  SignalTypeMap[-178]=std::make_pair<TString,double>("qq_grav",126);

}

TGraph * Normalization_8TeV::GetSigmaGraph(TString process)
{
  TGraph * gr = new TGraph();
	std::map<double, double> * XSectionMap = 0 ;
	if ( process == "ggh") {
		XSectionMap = &XSectionMap_ggh;
	} else if ( process == "vbf") {
		XSectionMap = &XSectionMap_vbf;
    } else if ( process == "vbfold") {
      XSectionMap = &XSectionMap_vbfold;
	} else if ( process == "wzh") {
		XSectionMap = &XSectionMap_wzh;
	} else if ( process == "tth") {
		XSectionMap = &XSectionMap_tth;
	} else if ( process == "wh") {
		XSectionMap = &XSectionMap_wh;
	} else if ( process == "zh") {
		XSectionMap = &XSectionMap_zh;
	} else if (process.Contains("grav")){
    XSectionMap = &XSectionMap_sm;
  } else {
    std::cout << "Warning ggh, vbf, wh, zh, wzh, tth or grav not found in histname!!!!" << std::endl;
    //exit(1);
  }
  
  for (std::map<double, double>::const_iterator iter = XSectionMap->begin();  iter != XSectionMap->end(); ++iter) {
    gr->SetPoint(gr->GetN(),iter->first, iter->second );
  }
	
	return gr;
}

TGraph * Normalization_8TeV::GetBrGraph()
{
	TGraph * gr = new TGraph();
	for (std::map<double, double>::const_iterator iter = BranchingRatioMap.begin();  iter != BranchingRatioMap.end(); ++iter) {
		gr->SetPoint(gr->GetN(),iter->first, iter->second );
	}
	return gr;
}

double Normalization_8TeV::GetBR(double mass) {

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
  
  std::cout << "Warning branching ratio outside range of 90-250GeV!!!!" << std::endl;
  //std::exit(1);
  return -1;
  
}


double Normalization_8TeV::GetXsection(double mass, TString HistName) {

  std::map<double,double> *XSectionMap;

  if (HistName.Contains("ggh")) {
    XSectionMap = &XSectionMap_ggh;
  } else if (HistName.Contains("vbf") && !HistName.Contains("vbfold")) {
    XSectionMap = &XSectionMap_vbf;
  } else if (HistName.Contains("vbfold")) {
    XSectionMap = &XSectionMap_vbfold;
  } else if (HistName.Contains("wh") && !HistName.Contains("wzh")) {
    XSectionMap = &XSectionMap_wh;
  } else if (HistName.Contains("zh") && !HistName.Contains("wzh")) {
    XSectionMap = &XSectionMap_zh;
  } else if (HistName.Contains("wzh")) {
    XSectionMap = &XSectionMap_wzh;
  } else if (HistName.Contains("tth")) {
    XSectionMap = &XSectionMap_tth;
  } else if (HistName.Contains("grav")) {
    XSectionMap = &XSectionMap_sm;
  } else {
    std::cout << "Warning ggh, vbf, wh, zh, wzh, tth or grav not found in " << HistName << std::endl;
    //exit(1);
  }

  for (std::map<double, double>::const_iterator iter = XSectionMap->begin();  iter != XSectionMap->end(); ++iter) {
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

  std::cout << "Warning cross section outside range of 80-300GeV!!!!" << std::endl;
  //exit(1);
  return -1;

}

double Normalization_8TeV::GetVBFCorrection(double mass) {
  return GetXsection(mass,"vbf")/GetXsection(mass,"vbfold");
}

// Simple accessors
TString Normalization_8TeV::GetProcess(int ty){
  if (ty < -7999){  // We dont go below 80 GeV and Spin samples in the 100 range 
    int process = -ty % 1000;
    if (process == 0 ) return "ggh";
    else if (process == 10 ) return "ggh_minlo";
    else if (process == 100) return "vbf";
    else if (process == 200) return "wh";
    else if (process == 300) return "zh";
    else if (process == 400) return "tth";
    else if (process == 500) return "wzh";
    else if (process == 500) return "wzh";
    else if (process == 600) return "gg_grav";
    else if (process == 610) return "gg_spin0";
    else if (process == 650) return "qq_grav";
    else {
	std::cout << "Error -- No signal process known " << process << std::endl;
	assert(0);
    }

  } else {
    return SignalTypeMap[ty].first;
  }
}

double Normalization_8TeV::GetMass(int ty){
  if (ty < -7999){  // We dont go below 80 GeV and Spin samples in the 100 range
    return double(-ty/1000);
  }	 
  else return SignalTypeMap[ty].second;
}
//// double Normalization_8TeV::GetXsection(int ty){
////   std::pair<TString,double> proc_mass = SignalTypeMap[ty];
////   // if "grav" in name then return all processes xs*br
////   if (proc_mass.first.Contains("grav")) {
////     return GetXsection(proc_mass.second);
////   }
////   else {
////     return GetXsection(proc_mass.second,proc_mass.first);
////   }
//// }
//// double Normalization_8TeV::GetBR(int ty){
////   std::pair<TString,double> proc_mass = SignalTypeMap[ty];
////   return GetBR(proc_mass.second);
//// }

double Normalization_8TeV::GetXsection(double mass) {
  return GetXsection(mass,"ggh") + GetXsection(mass,"vbf") + GetXsection(mass,"wzh") + GetXsection(mass,"tth");
}

double Normalization_8TeV::GetNorm(double mass1, TH1F* hist1, double mass2, TH1F* hist2, double mass) {

  double br = GetBR(mass);
  double br1 = GetBR(mass1);
  double br2 = GetBR(mass2);
  
  double xsec = GetXsection(mass, hist1->GetName());
  double xsec1 = GetXsection(mass1, hist1->GetName());
  double xsec2 = GetXsection(mass2, hist2->GetName());
  
  double alpha = 1.0*(mass-mass1)/(mass2-mass1);
  double effAcc1 = hist1->Integral()/(xsec1*br1);
  double effAcc2 = hist2->Integral()/(xsec2*br2);

  double Normalization = (xsec*br)*(effAcc1 + alpha * (effAcc2 - effAcc1));

  /// std::cout << mass1 << " " << hist1->GetName() << " " << mass2 << " " << hist2->GetName() << " " << mass 
  /// 	    << br << " " << br1 << " " << br2 << " " << xsec << " " << xsec1 << " " << xsec2 << " " << alpha << " " 
  /// 	    << effAcc1 << " " << effAcc2 << " " << Normalization << std::endl;
	  

  return Normalization;
  
}

void Normalization_8TeV::CheckNorm(double Min, double Max, double Step, TString histname="") {

  vector <double> Mass;
  vector <double> BranchingRatio;
  vector <double> XSection;
  for (double i=Min; i<Max; i+=Step) {
    Mass.push_back(i);
    BranchingRatio.push_back(GetBR(i));
    if (histname=="") XSection.push_back(GetXsection(i));
    else XSection.push_back(GetXsection(i,histname));
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
  TString outfile = "XSections.png";
  if (histname!="") outfile.ReplaceAll(".png",histname.Append(".png"));
  c1->SaveAs(outfile.Data());

  delete BranchGraph;
  delete XSectionGraph;
  delete c1;

}
void Normalization_8TeV::PlotXS(double Min, double Max){

	gROOT->SetBatch(1);
	TLegend *leg = new TLegend(0.65,0.7,0.89,0.89);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);

	TCanvas *can = new TCanvas("c","",800,800);
	TGraph *ggh = new TGraph();
	TGraph *vbf = new TGraph();
	TGraph *wh = new TGraph();
	TGraph *zh = new TGraph();
	TGraph *tth = new TGraph();

	ggh->SetLineColor(kBlue);
	vbf->SetLineColor(kRed);
	wh->SetLineColor(kGreen+3);
	zh->SetLineColor(kBlack);
	tth->SetLineColor(kViolet);

	leg->AddEntry(ggh,"gg fusion","L");
	leg->AddEntry(vbf,"Vector boson fusion","L");
	leg->AddEntry(wh,"W assoc","L");
	leg->AddEntry(zh,"Z assoc","L");
	leg->AddEntry(tth,"t#bar{t} assoc","L");

	ggh->SetLineWidth(3);vbf->SetLineWidth(3);wh->SetLineWidth(3);zh->SetLineWidth(3);tth->SetLineWidth(3);
	int i=0;
	for(double mH = Min;mH<=Max;mH+=0.5){
		ggh->SetPoint(i,mH,GetXsection(mH,"ggh"));
		vbf->SetPoint(i,mH,GetXsection(mH,"vbf"));
		wh->SetPoint(i,mH,GetXsection(mH,"wh"));
		zh->SetPoint(i,mH,GetXsection(mH,"zh"));
		tth->SetPoint(i,mH,GetXsection(mH,"tth"));	
		i++;
	}
	can->SetLogy();
	ggh->Draw("AL");vbf->Draw("L");wh->Draw("L");zh->Draw("L");tth->Draw("L");
	ggh->SetTitle(""); ggh->GetYaxis()->SetRangeUser(10E-4,100);
	ggh->GetXaxis()->SetTitle("m_{H} (GeV)");
	ggh->GetYaxis()->SetTitle("#sigma(pp#rightarrow H) ");
	leg->Draw();
	if (is2011_) can->SaveAs("xsections_7TeV.pdf");
	else	     can->SaveAs("xsections_8TeV.pdf");
}
void Normalization_8TeV::PlotBR(double Min, double Max){

	gROOT->SetBatch(1);
	TLegend *leg = new TLegend(0.65,0.7,0.89,0.89);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);

	TCanvas *can = new TCanvas("c","",800,800);
	TGraph *ggh = new TGraph();

	ggh->SetLineColor(kBlue);

	leg->AddEntry(ggh,"SM H# rightarrow #gamma#gamma","L");

	ggh->SetLineWidth(3);
	int i=0;
	for(double mH = Min;mH<=Max;mH+=0.5){
		ggh->SetPoint(i,mH,GetBR(mH));	
		i++;
	}
	can->SetLogy();
	ggh->Draw("AL");
	ggh->SetTitle(""); ggh->GetYaxis()->SetRangeUser(10E-6,0.1);
	ggh->GetXaxis()->SetTitle("m_{H} (GeV)");
	ggh->GetYaxis()->SetTitle(" BR(H#rightarrow#gamma#gamma) ");
	leg->Draw();
	can->SaveAs("branchingratio.pdf");
}

void Normalization_8TeV::PlotExpected(double Min, double Max){

	gROOT->SetBatch(1);
	TLegend *leg = new TLegend(0.65,0.7,0.89,0.89);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);

	TCanvas *can = new TCanvas("c","",800,800);
	TGraph *ggh = new TGraph();
	TGraph *vbf = new TGraph();
	TGraph *wzh = new TGraph();
	TGraph *tth = new TGraph();

	ggh->SetLineColor(kBlue);
	vbf->SetLineColor(kRed);
	wzh->SetLineColor(kGreen+3);
	tth->SetLineColor(kViolet);

	leg->AddEntry(ggh,"gg fusion","L");
	leg->AddEntry(vbf,"Vector boson fusion","L");
	leg->AddEntry(wzh,"W/Z assoc","L");
	leg->AddEntry(tth,"t#bar{t} assoc","L");

	ggh->SetLineWidth(3);vbf->SetLineWidth(3);wzh->SetLineWidth(3);tth->SetLineWidth(3);
	int i=0;
	for(double mH = Min;mH<=Max;mH+=0.5){
		ggh->SetPoint(i,mH,GetBR(mH)*GetXsection(mH,"ggh"));
		vbf->SetPoint(i,mH,GetBR(mH)*GetXsection(mH,"vbf"));
		wzh->SetPoint(i,mH,GetBR(mH)*GetXsection(mH,"wzh"));
		tth->SetPoint(i,mH,GetBR(mH)*GetXsection(mH,"tth"));
		
		i++;
	}
	can->SetLogy();
	ggh->Draw("AL");vbf->Draw("L");wzh->Draw("L");tth->Draw("L");
	ggh->SetTitle(""); ggh->GetYaxis()->SetRangeUser(10E-10,1);
	ggh->GetXaxis()->SetTitle("m_{H} (GeV)");
	ggh->GetYaxis()->SetTitle("#sigma(pp#rightarrow H) #times BR(#rightarrow#gamma#gamma) ");
	leg->Draw();
	if (is2011_)	can->SaveAs("signalnormalization_7TeV.pdf");
	else		can->SaveAs("signalnormalization_8TeV.pdf");
}


