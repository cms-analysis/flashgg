// Open the Categorisation training output file in CINT and run macro
{
  // Assume file is open with _file0
  gROOT->SetBatch(1);
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  int nBins = Bin_Edges->GetNbinsX(); 
  gStyle->SetPalette(51);
  //gROOT->ProcessLine(".x ~/tdrstyle.cc"); 

  TLatex *lat = new TLatex();
  lat->SetTextFont(42);
  lat->SetTextSize(0.035);

  // 1. Categorisation map is required 
  TCanvas *c0 = new TCanvas("c0","",600,600);
  Category_Map->SetTitle("");
  Category_Map->GetXaxis()->SetTitle("Diphoton BDT Output");
  Category_Map->GetYaxis()->SetTitle("#Deltam/m_{H}");
  Category_Map->GetXaxis()->SetLabelSize(0.025);
  Category_Map->GetYaxis()->SetLabelSize(0.025);
  Category_Map->GetXaxis()->SetTitleSize(0.035);
  Category_Map->GetYaxis()->SetTitleSize(0.035);
  Category_Map->GetZaxis()->SetTitleSize(0.035);
  Category_Map->GetYaxis()->SetTitleOffset(1.4);
  Category_Map->GetZaxis()->SetTitleOffset(0.5);
  Category_Map->SetContour(nBins);
  Category_Map->GetZaxis()->SetLabelSize(0);
  Category_Map->GetZaxis()->SetTitle("Increasing S/B #rightarrow");
  Category_Map->Draw("colz");
  lat->DrawLatex(0.1,0.92,"Categorisation Map");
  c0->SaveAs("Category_Map.pdf");
  c0->SaveAs("Category_Map.png");
  c0->SaveAs("Category_Map.C");

  // 2. The thing which gets optimized 
  TCanvas *c1 = new TCanvas("c1","",600,600);
  TLegend *leg = new TLegend(0.6,0.7,0.89,0.89);
  leg->SetFillColor(0);
  leg->SetTextFont(42);
  leg->AddEntry(targetB,"Background","L");
  leg->AddEntry(targetS,"#splitline{Signal}{m_{H} = 124 GeV}","L");
  targetB->GetXaxis()->SetTitleFont(42);
  targetB->GetXaxis()->SetTitleFont(42);
  targetB->GetXaxis()->SetLabelSize(0.025);
  targetB->GetYaxis()->SetLabelSize(0.025);
  targetB->GetYaxis()->SetTitle("Arbitrary Units");
  targetB->GetYaxis()->SetTitleOffset(1.2);
  targetB->GetXaxis()->SetTitle("Log_{2}(1+S/B)");
  targetB->Draw(); targetS->Draw("same");
  leg->Draw();
  c1->SaveAs("target.pdf");
  c1->SaveAs("target.png");
  c1->SaveAs("target.C");
 
  // 3. The ROC Curve
  TCanvas *c2 = new TCanvas("c2","",600,600);
  ROC->GetXaxis()->SetTitleFont(42);
  ROC->GetXaxis()->SetTitleFont(42);
  ROC->GetYaxis()->SetTitleOffset(1.2);
  ROC->Draw("alp");
  c2->SaveAs("roc.pdf");
  c2->SaveAs("roc.png");
  c2->SaveAs("roc.C");

  // 4. Signal and background dists
  TCanvas *csig = new TCanvas("csig","",600,600); 
  hsig->GetYaxis()->SetTitleOffset(2.0);
  hsig->GetXaxis()->SetTitleOffset(1.4);
  hsig->GetXaxis()->SetTitle("Diphoton BDT Output");
  hsig->GetYaxis()->SetTitle("#Deltam/m_{H}");
  hsig->SetTitle("");
  lat->DrawLatex(0.1,0.92,"Signal Distribution");
  hsig->Draw("surf");
  hsig_raw->Draw("legosame");
  csig->SaveAs("sig.pdf");
  csig->SaveAs("sig.png");
  csig->SaveAs("sig.C");
  TCanvas *cbkg = new TCanvas("cbkg","",600,600); 
  hbkg->GetYaxis()->SetTitleOffset(2.0);
  hbkg->GetXaxis()->SetTitleOffset(1.4);
  hbkg->GetXaxis()->SetTitle("Diphoton BDT Output");
  hbkg->GetYaxis()->SetTitle("#Deltam/m_{H}");
  hbkg->SetTitle("");
  lat->DrawLatex(0.1,0.92,"Background Distribution");
  hbkg->Draw("surf");
  hbkg_raw->Draw("legosame");
  cbkg->SaveAs("bkg.pdf");
  cbkg->SaveAs("bkg.png");
  cbkg->SaveAs("bkg.C");

  // Projections 
  TCanvas *cX = new TCanvas("cX","",600,600); 
  TH1D * projX_sig_raw = (TH1D*)hsig_raw->ProjectionX();
  projX_sig_raw->SetMarkerSize(0.5);
  projX_sig_raw->SetMarkerColor(2);
  projX_sig_raw->SetLineColor(2);
  projX_sig_raw->SetMarkerStyle(21);
  TH1D * projX_bkg_raw = (TH1D*)hbkg_raw->ProjectionX();
  projX_bkg_raw->SetMarkerSize(0.5);
  projX_bkg_raw->SetMarkerColor(4);
  projX_bkg_raw->SetLineColor(4);
  projX_bkg_raw->SetMarkerStyle(21);
  projX_bkg_raw->GetXaxis()->SetTitle("Diphoton BDT Output");
  projX_bkg_raw->GetYaxis()->SetTitle("Arbitrary Units");

  TH1D * projX_sig = (TH1D*)hsig->ProjectionX();
  TH1D * projX_bkg = (TH1D*)hbkg->ProjectionX();
  projX_sig->SetLineColor(2);
  projX_bkg->SetLineColor(4);
  projX_bkg_raw->SetTitle("");
  projX_bkg_raw->GetYaxis()->SetTitleFont(42);
  projX_bkg_raw->GetYaxis()->SetLabelFont(42);

  projX_sig->Scale(1./projX_sig->Integral());
  projX_bkg->Scale(1./projX_bkg->Integral());
  projX_sig_raw->Scale(1./projX_sig_raw->Integral());
  projX_bkg_raw->Scale(1./projX_bkg_raw->Integral());
  //projX_bkg_raw->SetMaximum(0.02);
  projX_bkg_raw->Draw("p");
  projX_bkg->Draw("samehist");
  projX_sig_raw->Draw("samep");
  projX_sig->Draw("samehist");
  TLegend *legPX = new TLegend(0.54,0.7,0.89,0.89);
  legPX->SetFillColor(0);
  legPX->SetTextFont(42);
  legPX->AddEntry(projX_sig_raw,"Signal RAW","P");
  legPX->AddEntry(projX_sig,"Signal Regressed","L");
  legPX->AddEntry(projX_bkg_raw,"Background RAW","P");
  legPX->AddEntry(projX_bkg,"Background Regressed","L");
  legPX->Draw(); 
  cX->SaveAs("sigbkg_proj_diphoBDT.pdf");
  cX->SaveAs("sigbkg_proj_diphoBDT.png");
  cX->SaveAs("sigbkg_proj_diphoBDT.C");

  TCanvas *cY = new TCanvas("cY","",600,600); 
  TH1D * projY_sig_raw = (TH1D*)hsig_raw->ProjectionY();
  projY_sig_raw->SetMarkerSize(0.5);
  projY_sig_raw->SetMarkerColor(2);
  projY_sig_raw->SetLineColor(2);
  projY_sig_raw->SetMarkerStyle(21);
  TH1D * projY_bkg_raw = (TH1D*)hbkg_raw->ProjectionY();
  projY_bkg_raw->SetMarkerSize(0.5);
  projY_bkg_raw->SetMarkerColor(4);
  projY_bkg_raw->SetLineColor(4);
  projY_bkg_raw->SetMarkerStyle(21);
  projY_sig_raw->GetXaxis()->SetTitle("#Deltam/m_{H}");
  projY_sig_raw->GetYaxis()->SetTitle("Arbitrary Units");

  TH1D * projY_sig = (TH1D*)hsig->ProjectionY();
  TH1D * projY_bkg = (TH1D*)hbkg->ProjectionY();
  projY_sig->SetLineColor(2);
  projY_bkg->SetLineColor(4);

  projY_sig->Scale(1./projY_sig->Integral());
  projY_bkg->Scale(1./projY_bkg->Integral());
  projY_sig_raw->Scale(1./projY_sig_raw->Integral());
  projY_bkg_raw->Scale(1./projY_bkg_raw->Integral());
  projY_sig_raw->SetTitle("");
  projY_sig_raw->GetYaxis()->SetTitleFont(42);
  projY_sig_raw->GetYaxis()->SetLabelFont(42);
  projY_sig_raw->Draw("p");
  projY_sig->Draw("samehist");
  projY_bkg_raw->Draw("samep");
  projY_bkg->Draw("samehist");
  TLegend *legPY = new TLegend(0.54,0.7,0.89,0.89);
  legPY->SetFillColor(0);
  legPY->SetTextFont(42);
  legPY->AddEntry(projY_sig_raw,"Signal RAW","P");
  legPY->AddEntry(projY_sig,"Signal Regressed","L");
  legPY->AddEntry(projY_bkg_raw,"Background RAW","P");
  legPY->AddEntry(projY_bkg,"Background Regressed","L");
  legPY->Draw(); 
  cY->SaveAs("sigbkg_proj_dmom.pdf");
  cY->SaveAs("sigbkg_proj_dmom.png");
  cY->SaveAs("sigbkg_proj_dmom.C");

  // 5. Optimization graph
  TCanvas *c5 = new TCanvas("csig","",600,600); 
  Optimization->GetXaxis()->SetTitleFont(42);
  Optimization->GetYaxis()->SetTitleFont(42);
  Optimization->GetYaxis()->SetRangeUser(3.5,5.0);
  Optimization->Draw("alp");
  c5->SaveAs("optimization.pdf") ;
  c5->SaveAs("optimization.png") ;
  c5->SaveAs("optimization.C") ;

  // S/B graph
  TCanvas *c6 = new TCanvas("csb","",600,600); 
  TPad *pad1 = new TPad("upper","upper",0,0.3,1,1);
  TPad *pad2 = new TPad("lower","lower",0,0,1,0.28);
  pad1->Draw(); pad2->Draw(); 
  pad1->cd(); pad1->SetLogy();
  TLegend *legi = new TLegend(0.65,0.64,0.89,0.89);
  legi->SetFillColor(0);
  legi->SetTextFont(42);
  legi->AddEntry(b_raw_final,"Background","L");
  legi->AddEntry(s_raw_final,"#splitline{Signal}{m_{H} = 124 GeV}","L");
  b_raw_final->GetYaxis()->SetTitle("# Events");
  b_raw_final->GetXaxis()->SetLabelSize(0.06);
	
  b_raw_final->SetMinimum(0.1);
  b_raw_final->Draw("p");
  s_raw_final->Draw("psame");
  legi->Draw();

  pad2->cd();
  TH1F *s_raw_finalrat = (TH1F*)s_raw_final->Clone();
  s_raw_finalrat->Divide(b_raw_final);
  s_raw_finalrat->GetYaxis()->SetTitle("S/B");
  s_raw_finalrat->GetYaxis()->SetTitleOffset(0.45);
  s_raw_finalrat->GetYaxis()->SetNdivisions(4);
  s_raw_finalrat->GetYaxis()->SetLabelSize(0.09);
  s_raw_finalrat->GetYaxis()->SetTitleSize(0.1);
  s_raw_finalrat->GetXaxis()->SetLabelSize(0.15);
  s_raw_finalrat->Draw();
  c6->SaveAs("soverb.pdf") ;
  c6->SaveAs("soverb.png"); 
  c6->SaveAs("soverb.C") ;
  

}
