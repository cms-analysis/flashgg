TGraph* bestFit(TTree *t, TString x, TString y) {
    t->Draw(y+":"+x, "deltaNLL == 0");
    TGraph *gr0 = (TGraph*) gROOT->FindObject("Graph")->Clone();
    gr0->SetMarkerStyle(34); gr0->SetMarkerSize(2.0);
    if (gr0->GetN() > 1) gr0->Set(1);
    return gr0;
}

TList* contourPlot(TTree *t, TString x, TString y, double pmin, double pmax, TGraph *bestFit) {
    int n = t->Draw(y+":"+x, Form("%f <= quantileExpected && quantileExpected <= %f && quantileExpected != 1",pmin,pmax));
    TGraph *gr = (TGraph*) gROOT->FindObject("Graph")->Clone();
    Double_t x0 = bestFit->GetX()[0], y0 = bestFit->GetY()[0];
    Double_t *xi = gr->GetX(), *yi = gr->GetY();
    int n = gr->GetN();
    for (int i = 0; i < n; ++i) { xi[i] -= x0; yi[i] -= y0; }
    gr->Sort(&TGraph::CompareArg);
    for (int i = 0; i < n; ++i) { xi[i] += x0; yi[i] += y0; }
    TList *ret = new TList();
    ret->Add(gr);
    return ret;
}

int countGridPointsFromTree(TTree *t, TString x) {
    double xmin = t->GetMinimum(x);
    double xmax = t->GetMaximum(x);
    t->Draw(Form("%s>>h1000(1000,%10g,%10g)", x.Data(),xmin-1e-4,xmax+1e-4), "deltaNLL > 0");
    TH1 *h1000 = (TH1*) gROOT->FindObject("h1000");
    int bins = 0;
    for (int i = 1, n = h1000->GetNbinsX(); i <= n; ++i) {
        if (h1000->GetBinContent(i) != 0) bins++;
    }
    h1000->Delete();
    return bins;
}
TH2 *treeToHist2D(TTree *t, TString x, TString y, TString name) {
    double xmin = t->GetMinimum(x);
    double xmax = t->GetMaximum(x);
    double ymin = t->GetMinimum(y);
    double ymax = t->GetMaximum(y);
    int xbins = countGridPointsFromTree(t,x);
    int ybins = countGridPointsFromTree(t,y);
    double dx = (xmax-xmin)/(xbins-1);
    double dy = (ymax-ymin)/(ybins-1);
    xmin -= 0.5*dx; xmax += 0.5*dx;
    ymin -= 0.5*dy; ymax += 0.5*dy;
    if (fabs(xmin) < 1e-5) xmin = 0;
    if (fabs(xmax) < 1e-5) xmax = 0;
    //std::cout << "In making " << name << ", guessed " << xbins << " bins for " << x << " from " << xmin << " to " << xmax << std::endl;
    //std::cout << "In making " << name << ", guessed " << ybins << " bins for " << y << " from " << ymin << " to " << ymax << std::endl;
    //    t->Draw(Form("2*deltaNLL:%s:%s>>%s_prof(%d,%10g,%10g,%d,%10g,%10g)", y.Data(), x.Data(), name.Data(), xbins, xmin, xmax, ybins, ymin, ymax), "deltaNLL != 0", "PROF");
    t->Draw(Form("2*deltaNLL:%s:%s>>%s_prof(%d,%10g,%10g,%d,%10g,%10g)", y.Data(), x.Data(), name.Data(), xbins, xmin, xmax, ybins, ymin, ymax), "", "PROF");
    TH2 *prof = (TH2*) gROOT->FindObject(name+"_prof");
    TH2D *h2d = new TH2D(name, name, xbins, xmin, xmax, ybins, ymin, ymax);
    for (int ix = 1; ix <= xbins; ++ix) {
        for (int iy = 1; iy <= ybins; ++iy) {
             h2d->SetBinContent(ix, iy, prof->GetBinContent(ix,iy));
	     //	     if(h2d->GetBinContent(ix,iy)==0) 
        }
    }
    h2d->SetDirectory(0);
    return h2d;
}

TList* contourFromTH2(TH2 *h2in, double threshold) {
    std::cout << "Getting contour at threshold " << threshold << " from " << h2in->GetName() << std::endl;
    //http://root.cern.ch/root/html/tutorials/hist/ContourList.C.html
    Double_t contours[1];
    contours[0] = threshold;

    TH2D *h2 = frameTH2D((TH2D*)h2in);

    h2->SetContour(1, contours);

    // Draw contours as filled regions, and Save points
    h2->Draw("CONT Z LIST");
    gPad->Update(); // Needed to force the plotting and retrieve the contours in TGraphs

    // Get Contours
    TObjArray *conts = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
    TList* contLevel = NULL;

    if (conts == NULL || conts->GetSize() == 0){
        printf("*** No Contours Were Extracted!\n");
        return 0;
    }

    TList *ret = new TList();
    for(int i = 0; i < conts->GetSize(); i++){
        contLevel = (TList*)conts->At(i);
        printf("Contour %d has %d Graphs\n", i, contLevel->GetSize());
        for (int j = 0, n = contLevel->GetSize(); j < n; ++j) {
            TGraph *gr1 = (TGraph*) contLevel->At(j)->Clone();
            ret->Add(gr1);
        }
    }
    return ret;
}

TH2D* frameTH2D(TH2D *in){

	Double_t xw = in->GetXaxis()->GetBinWidth(0);
	Double_t yw = in->GetYaxis()->GetBinWidth(0);

	Int_t nx = in->GetNbinsX();
	Int_t ny = in->GetNbinsY();

	Double_t x0 = in->GetXaxis()->GetXmin();
	Double_t x1 = in->GetXaxis()->GetXmax();

	Double_t y0 = in->GetYaxis()->GetXmin();
	Double_t y1 = in->GetYaxis()->GetXmax();

	TH2D *framed = new TH2D(
			Form("%s framed",in->GetName()),
			Form("%s framed",in->GetTitle()),
			nx + 2, x0-xw, x1+xw,
			ny + 2, y0-yw, y1+yw
			);

	//Copy over the contents
	for(int ix = 1; ix <= nx ; ix++){
		for(int iy = 1; iy <= ny ; iy++){
			framed->SetBinContent(1+ix, 1+iy, in->GetBinContent(ix,iy));
		}
	}
	//Frame with huge values
	nx = framed->GetNbinsX();
	ny = framed->GetNbinsY();
	for(int ix = 1; ix <= nx ; ix++){
		framed->SetBinContent(ix,  1, 1000.);
		framed->SetBinContent(ix, ny, 1000.);
	}
	for(int iy = 2; iy <= ny-1 ; iy++){
		framed->SetBinContent( 1, iy, 1000.);
		framed->SetBinContent(nx, iy, 1000.);
	}

	return framed;
}

