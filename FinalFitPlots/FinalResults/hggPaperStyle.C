{

  // Hgg Paper style for plots
  TStyle *hggStyle = new TStyle("hggPaperStyle","Hgg Paper Style");

  //hggStyle->SetCanvasColor(0);
  //hggStyle->SetPadColor(0);
  hggStyle->SetFrameFillColor(0);
  hggStyle->SetStatColor(0);
  hggStyle->SetOptStat(0);
  hggStyle->SetTitleFillColor(0);
  hggStyle->SetCanvasBorderMode(0);
  hggStyle->SetPadBorderMode(0);
  hggStyle->SetFrameBorderMode(0);
  //hggStyle->SetFrameBorderSize(3);
  //hggStyle->SetPadBorderSize(3);
  //hggStyle->SetCanvasBorderSize(3);
  hggStyle->SetPadColor(kWhite);
  hggStyle->SetCanvasColor(kWhite);

  hggStyle->SetCanvasDefH(600); //Height of canvas
  hggStyle->SetCanvasDefW(600); //Width of canvas
  hggStyle->SetCanvasDefX(0);   //POsition on screen
  hggStyle->SetCanvasDefY(0);

  hggStyle->SetPadLeftMargin(0.13);//0.16);
  hggStyle->SetPadRightMargin(0.1);//0.02);
  hggStyle->SetPadTopMargin(0.085);//0.02);
  hggStyle->SetPadBottomMargin(0.12);//0.02);

    // For hgg axis titles:
  hggStyle->SetTitleColor(1, "XYZ");
  hggStyle->SetTitleFont(42, "XYZ");
  hggStyle->SetTitleSize(0.05, "XYZ");
  hggStyle->SetTitleXOffset(0.95);//0.9);
  hggStyle->SetTitleYOffset(1.15); // => 1.15 if exponents

  // For hgg axis labels:
  hggStyle->SetLabelColor(1, "XYZ");
  hggStyle->SetLabelFont(42, "XYZ");
  hggStyle->SetLabelOffset(0.007, "XYZ");
  hggStyle->SetLabelSize(0.04, "XYZ");

  // Legends
  hggStyle->SetLegendBorderSize(0);
  hggStyle->SetLegendFillColor(kWhite);
  hggStyle->SetLegendFont(42);

  hggStyle->SetFillColor(10);
  // Nothing for now
  hggStyle->SetTextFont(42);
  hggStyle->SetTextSize(0.03);
  hggStyle->cd();
}
