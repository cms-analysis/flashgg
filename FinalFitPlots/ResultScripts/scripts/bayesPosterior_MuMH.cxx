TGraph* smValue(double x0 = 1.0, double y0 = 1.0) {
    TGraph* ret = new TGraph(1);
    ret->SetPoint(0, x0, y0);
    ret->SetMarkerStyle(29); ret->SetMarkerSize(4.0);
    ret->SetMarkerColor(1);
    return ret;
}

void bayesPosterior2D(TH2F *frame, TString x, TString y) { 
    TDirectory *toyDir = gFile->GetDirectory("toys");
    if (toyDir == 0) {
        std::cerr << "Error in file " << gROOT->GetListOfFiles()->At(i)->GetName() << ": directory /toys not found" << std::endl;
        continue;
    }
    TString prefix = "MarkovChain_";
    TIter next(toyDir->GetListOfKeys()); TKey *k;
    while ((k = (TKey *) next()) != 0) {
        if (TString(k->GetName()).Index(prefix) != 0) continue;
        RooStats::MarkovChain *chain = dynamic_cast<RooStats::MarkovChain *>(toyDir->Get(k->GetName()));
        const RooDataSet *rds = chain->GetAsConstDataSet();
        int entries = rds->numEntries();
        std::cout << "Got chain " << chain->GetName() << " with " << entries << " entries." << std::endl;
        int burnIn = TMath::Max(5000,entries/2);
        for (int i = burnIn; i < entries; ++i) {
            const RooArgSet *point = rds->get(i);
            frame->Fill(point->getRealValue(x), point->getRealValue(y), rds->weight());
            //std::cout << i<<"/"<<entries<<": " 
            //    << x << " = " << point->getRealValue(x) << ", " << y << " = " << point->getRealValue(y) <<
            //    " (weight " << rds->weight() << ")" << std::endl;
        }
    }
}

TGraph* bestFit(TH2* hist) {
    TGraph* ret = new TGraph(1);
    int nx = hist->GetNbinsX(), ny = hist->GetNbinsY();
    double max = -1;
    TAxis *ax = hist->GetXaxis(), *ay = hist->GetYaxis();
    for (int i = 1; i <= nx; ++i) {
        for (int j = 1; j <= ny; ++j) {
            double p = hist->GetBinContent(i,j);
            if (p > max) {
                max = p;
                ret->SetPoint(0, ax->GetBinCenter(i), ay->GetBinCenter(j));
            }
        }
    }
    ret->SetMarkerStyle(34); ret->SetMarkerSize(2.0);
    return ret;
}

TList* bayesContour(TH2 *hist, double cl) {
    int nx = hist->GetNbinsX(), ny = hist->GetNbinsY(), n2 = nx * ny;
    Double_t *vals = new Double_t[n2];
    Double_t sum = 0;
    for (int i = 1, k = 0; i <= nx; ++i) { 
        for (int j = 1; j <= ny; ++j, ++k) {
            vals[k] = hist->GetBinContent(i,j);
            sum += vals[k];
        }
    }
    Int_t *index = new Int_t[n2];
    TMath::Sort(n2, vals, index);
    double cut = cl*sum, runsum = 0, threshold = -1;
    for (int i = 0; i < n2; ++i) {
        runsum += vals[index[i]];
        if (runsum > cut) { threshold = vals[index[i]]; break; }
    }
    return contourFromTH2(hist, threshold);
    TGraph *points = new TGraph(); int p = 0;
    TAxis *ax = hist->GetXaxis(), *ay = hist->GetYaxis();
    for (int i = 1, k = 0; i <= nx; ++i) {
        if (hist->GetBinContent(i,ny) > threshold) {
            points->Set(++p); points->SetPoint(p-1, ax->GetBinCenter(i), ay->GetBinUpEdge(ny));
        } else {
            for (int j = ny-1; j > 0; --j) {
                if (hist->GetBinContent(i,j) > threshold) {
                    points->Set(++p); points->SetPoint(p-1, ax->GetBinCenter(i), ay->GetBinCenter(j));
                    break;
                }
            }
        }
    }
    for (int i = nx; i > 0; --i) {
        if (hist->GetBinContent(i,1) > threshold) {
            points->Set(++p); points->SetPoint(p-1, ax->GetBinCenter(i), ay->GetBinLowEdge(1));
        } else {
            for (int j = 2; j <= ny; ++j) {
                if (hist->GetBinContent(i,j) > threshold) {
                    points->Set(++p); points->SetPoint(p-1, ax->GetBinCenter(i), ay->GetBinCenter(j));
                    break;
                }
            }
        }
    }
    points->Set(++p); points->SetPoint(p-1, points->GetX()[0], points->GetY()[0]);
    TList *tmg = new TList();
    tmg->Add(points);
    return tmg;
}


void bayesPosterior_MuMH(const char *name, const char *title, double mhMin=115., double mhMax=130., double muMax=5, int binsX=50, int binsY=50) {
    TH2F *frame = new TH2F(name, Form("%s;Higgs boson mass (GeV);#sigma/#sigma_{SM};Posterior density (1/GeV)",title),
                            binsX, mhMin, mhMax, binsY, 0, muMax); 
    frame->SetContour(100);
    bayesPosterior2D(frame,"MH","r");
    TCanvas *c1 = new TCanvas("c1","c1");
    c1->SetRightMargin(0.22);
    //c1->SetLogz(1);
    frame->Scale(1.0/frame->Integral("Width"));
    frame->GetZaxis()->SetTitleOffset(1.3);
    gStyle->SetOptStat(0);
    frame->Draw("COLZ");
    TGraph *gr0 = bestFit(frame);
    TGraph *gr68 = bayesContour(frame, 0.68);
    gr68->SetLineWidth(3);   
    gr68->Draw("L SAME");
    gr0->Draw("P SAME");
    //TGraph *sm = smValue(); sm->Draw("P SAME");
    c1->Print(Form("%s.png", name));
    TFile *fOut = TFile::Open(Form("%s.root", name), "RECREATE");
    fOut->WriteTObject(frame);
    fOut->Close();
}
