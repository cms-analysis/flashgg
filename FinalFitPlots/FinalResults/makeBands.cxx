TString prefix = "";

TGraph* bestFit(TTree *t, TString x, TString y) {
    t->Draw(y+":"+x, "deltaNLL == 0");
    TGraph *gr0 = (TGraph*) gROOT->FindObject("Graph")->Clone();
    gr0->SetMarkerStyle(34); gr0->SetMarkerSize(2.0);
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
    t->Draw(Form("2*deltaNLL:%s:%s>>%s(%d,%10g,%10g,%d,%10g,%10g)", y.Data(), x.Data(), name.Data(), xbins, xmin, xmax, ybins, ymin, ymax), "deltaNLL > 0", "PROF");
    TH2 *ret = (TH2*) gROOT->FindObject(name);
    ret->SetDirectory(0);
    return ret;
}

TList* contourFromTH2(TH2 *h2, double threshold) {
    std::cout << "Getting contour at threshold " << threshold << " from " << h2->GetName() << std::endl;
    //http://root.cern.ch/root/html/tutorials/hist/ContourList.C.html
    Double_t contours[1];
    contours[0] = threshold;

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

void readParamScan1D(TFile *bands, TString name, TString rootfile, TString paramName) {
    if (/*error=*/gSystem->AccessPathName(rootfile)) return;
    TFile *f = TFile::Open(rootfile);
    TTree *t = (TTree*) f->Get("limit");
    t->Draw("2*deltaNLL:"+paramName);
    TGraph *gr = (TGraph*) gROOT->FindObject("Graph")->Clone(name+"_obs");
    std::cout << " Found scan " << name <<  " with  " << gr->GetN() << " points." << std::endl;
    gr->SetName(name+"_obs");
    gr->Sort();
    bands->WriteTObject(gr);
    printLine(bands, name+"_obs", "results/"+name+".txt");
    printValueFromScan1D(bands, name+"_obs", "results/"+name+"_crossings.txt"); 
    f->Close();
}
void readMassScan1D(TFile *bands, TString name, TString rootfile) {
    readParamScan1D(bands,name,rootfile,"MH");
}


bool readParamScan2DContour(TFile *bands, TString name, TString rootfile, double cl, TString postfix, TString x, TString y) {
    if (/*error=*/gSystem->AccessPathName(rootfile)) return false;
    TFile *f = TFile::Open(rootfile);
    TTree *t = (TTree*) f->Get("limit");
    TGraph *gr0  = bestFit(t, x, y);  
    gr0->SetName(name+"_best");
    TList *grCL = contourPlot(t, x, y, (1-cl)*0.9, 0.9999, gr0);
    grCL->SetName(name+postfix);
    bands->WriteTObject(gr0, name+"_best", "Overwrite");
    bands->WriteTObject(grCL, grCL->GetName(), "SingleKey");
    f->Close();
    return true;
}

void readParamScan2D(TFile *bands, TString name, TString rootfile, TString x, TString y) {
    TString c68file = rootfile; c68file(".root") = "_C68.root";
    TString c95file = rootfile; c95file(".root") = "_C95.root";
    bool has68 = readParamScan2DContour(bands, name, c68file, 0.68, "_c68", x, y);
    bool has95 = readParamScan2DContour(bands, name, c95file, 0.95, "_c95", x, y);

    if (/*error=*/gSystem->AccessPathName(rootfile)) return;
    TFile *f = TFile::Open(rootfile);
    TTree *t = (TTree*) f->Get("limit");
    TGraph *gr0  = bestFit(t, x, y);  
    gr0->SetName(name+"_best");
    bands->WriteTObject(gr0, name+"_best", "Overwrite");

    TH2 *h2d = treeToHist2D(t, x, y, name+"_th2");
    bands->WriteTObject(h2d);
    if (!has68) {
        TList *c68 = contourFromTH2(h2d, 2.30);
        if (c68) { c68->SetName(name+"_c68"); bands->WriteTObject(c68, name+"_c68", "SingleKey"); }
    }
    if (!has95) {
        TList *c95 = contourFromTH2(h2d, 5.99);
        if (c95) { c95->SetName(name+"_c95"); bands->WriteTObject(c95, name+"_c95", "SingleKey"); }
    }
    f->Close();
}
void readMassScan2D(TFile *bands, TString name, TString rootfile) {
    readParamScan2D(bands,name,rootfile,"MH","r");
}

void makeBands() {
    gROOT->LoadMacro("$CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/plotting/bandUtils.cxx+");
    
    TFile *bands = new TFile("results/test.root","RECREATE");
    readParamScan2D(bands, "cvcf_cutbased", "cutbased_data/cVcF125_x6/higgsCombinecVcF125_grid.MultiDimFit.mH125.root", "CV", "CF");
    readParamScan2D(bands, "cvcf_mva2cat", "trainingJul23_124_Mjj100_data_2cat/cVcF125_x6/higgsCombinecVcF125_grid.MultiDimFit.mH125.root", "CV", "CF");

    readParamScan2D(bands, "rvrf_cutbased", "cutbased_data/rVrF125_x6/higgsCombinerVrF125_grid.MultiDimFit.mH125.root", "RV", "RF"); 
    readParamScan2D(bands, "rvrf_mva2cat", "trainingJul23_124_Mjj100_data_2cat/rVrF125_x6/higgsCombinerVrF125_grid.MultiDimFit.mH125.root", "RV", "RF");

    bands->Close();
}
