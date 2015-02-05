TString prefix = "";

#include "scripts/channels.h"
#include "scripts/bayesPosterior_MuMH.cxx"
#include "scripts/contours.cxx"

// TGraph* smValue(double x0 = 1.0, double y0 = 1.0) {
//   TGraph* ret = new TGraph(1);
//   ret->SetPoint(0, x0, y0);
//   ret->SetMarkerStyle(29); ret->SetMarkerSize(4.0);
//   ret->SetMarkerColor(kYellow);
//   return ret;
// }

void cleanCLs(TGraphAsymmErrors *graph) {
    bool foundBad = false;
    do {
        foundBad = false;
        for (int i = 0, n = graph->GetN(); i < n; ++i) {
            if (graph->GetX()[i] == 0) { 
                graph->RemovePoint(i); 
                foundBad = true; 
                break; 
            } 
        }
    } while (foundBad);
    
}

double inverseLinearInterp(double x1, double y1, double x2, double y2, double y0) {
    double w1= fabs(y2-y0), w2 = fabs(y1-y0);
    return (w1*x1+w2*x2)/(w1+w2);
}

void readParamScan1D(TFile *bands, TString name, TString rootfile, TString paramName, TString cut="") {
    if (/*error=*/gSystem->AccessPathName(rootfile)) return;
    TFile *f = TFile::Open(rootfile);
    TTree *t = (TTree*) f->Get("limit");
    int n = t->Draw("2*deltaNLL:"+paramName, cut);
    if (n == 0) return;
    TGraph *gr = (TGraph*) gROOT->FindObject("Graph")->Clone(name+"_obs");
    std::cout << " Found scan " << name <<  " with  " << gr->GetN() << " points." << std::endl;
    gr->SetName(name+"_obs");
    gr->Sort();
    double *yi = gr->GetY(); 
    int imin = 0;
    for (int i = 1; i < n; ++i) {
        if (yi[i] < yi[imin]) imin = i;
    }
    if (yi[imin] < -1e-4) { 
        double ymin = yi[imin];
        for (int i = 0; i < n; ++i) yi[i] -= ymin;
    }
    bands->WriteTObject(gr);
    //    printLine(bands, name+"_obs", "results/"+name+".txt");
    //    printValueFromScan1D(bands, name+"_obs", "results/"+name+"_crossings.txt"); 
    f->Close();
}
void readMassScan1D(TFile *bands, TString name, TString rootfile) {
    readParamScan1D(bands,name,rootfile,"MH");
}
void readMuScan1D(TFile *bands, TString name, TString rootfile) {
    readParamScan1D(bands, name, rootfile, "r", "mh == 125");
    readParamScan1D(bands, name+"_110", rootfile, "r", "mh == 110");
    readParamScan1D(bands, name+"_115", rootfile, "r", "mh == 115");
    readParamScan1D(bands, name+"_120", rootfile, "r", "mh == 120");
    readParamScan1D(bands, name+"_130", rootfile, "r", "mh == 130");
    readParamScan1D(bands, name+"_135", rootfile, "r", "mh == 135");
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

void readParamScan2D(TFile *bands, TString name, TString rootfile, TString x, TString y, bool renormalizeFullPlot=true) {
  TString dir=rootfile; dir.Remove(dir.Last('/'));
    TString c68file = rootfile; c68file(".root") = "_C68.root";
    TString c95file = rootfile; c95file(".root") = "_C95.root";
    bool has68 = false; //readParamScan2DContour(bands, name, c68file, 0.68, "_c68", x, y);
    bool has95 = false; //readParamScan2DContour(bands, name, c95file, 0.95, "_c95", x, y);

    if (/*error=*/gSystem->AccessPathName(rootfile)) return;
    TFile *f = TFile::Open(rootfile);
    TTree *t = (TTree*) f->Get("limit");
    TGraph *gr0  = bestFit(t, x, y);  

    TH2 *h2d = treeToHist2D(t, x, y, name+"_th2");
    if (name.Contains("cvcf",kFALSE) || name.Contains("rvrf",kFALSE)) {
        int nx = h2d->GetNbinsX(), ny = h2d->GetNbinsY();
        double min = 0;
        for (int ix = 1; ix <= nx; ++ix) {
            for (int iy = 1; iy <= ny; ++iy) {
                double hx = h2d->GetXaxis()->GetBinCenter(ix);
                double hy = h2d->GetYaxis()->GetBinCenter(iy);
                if (renormalizeFullPlot || (hx > 0 && hy > 0)) {
                    double z = h2d->GetBinContent(ix,iy);
                    if (z < min) { 
                        min = z;
                        if (renormalizeFullPlot) gr0->SetPoint(0, hx, hy);
                    }
                }
            }
        }
        if (min < 0) std::cout << "Subtracting minimum " << min << " from " << name << std::endl;
        else min = 0;
        for (int ix = 1; ix <= nx; ++ix) {
            for (int iy = 1; iy <= ny; ++iy) {
                double z = h2d->GetBinContent(ix,iy) - min;
                if (z > -1e-2 && z < 1e-4) z = 1e-4;
                h2d->SetBinContent(ix, iy, z);
            }
        }

    }
    gr0->SetName(name+"_best");
    bands->WriteTObject(gr0, name+"_best", "Overwrite");

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

    int ix = h2d->GetXaxis()->FindBin(gr0->GetX()[0]);
    int iy = h2d->GetYaxis()->FindBin(gr0->GetY()[0]);
    double xhi = h2d->GetXaxis()->GetXmax(), xlo = h2d->GetXaxis()->GetXmin(), yhi = h2d->GetYaxis()->GetXmax(), ylo = h2d->GetYaxis()->GetXmin();
    for (int i = ix; i <= h2d->GetNbinsX(); ++i) {
        if (h2d->GetBinContent(i, iy) >= 1) {
            xhi = inverseLinearInterp(h2d->GetXaxis()->GetBinCenter(i-1), h2d->GetBinContent(i-1, iy),
                                      h2d->GetXaxis()->GetBinCenter(i  ), h2d->GetBinContent(i  , iy), 1.0); 
            break;
        }
    }
    for (int i = ix; i >= 1; --i) {
        if (h2d->GetBinContent(i, iy) >= 1) {
            xlo = inverseLinearInterp(h2d->GetXaxis()->GetBinCenter(i+1), h2d->GetBinContent(i+1, iy),
                                      h2d->GetXaxis()->GetBinCenter(i  ), h2d->GetBinContent(i  , iy), 1.0); 
            break;
        }
    }
    for (int i = iy; i <= h2d->GetNbinsY(); ++i) {
        if (h2d->GetBinContent(ix, i) >= 1) {
            yhi = inverseLinearInterp(h2d->GetYaxis()->GetBinCenter(i-1), h2d->GetBinContent(ix, i-1),
                                      h2d->GetYaxis()->GetBinCenter(i  ), h2d->GetBinContent(ix, i  ), 1.0); 
            break;
        }
    }
    for (int i = iy; i >= 1; --i) {
        if (h2d->GetBinContent(ix, i) >= 1) {
            ylo = inverseLinearInterp(h2d->GetYaxis()->GetBinCenter(i+1), h2d->GetBinContent(ix, i+1),
                                      h2d->GetYaxis()->GetBinCenter(i  ), h2d->GetBinContent(ix, i  ), 1.0); 
            break;
        }
    }
    FILE *f1 = fopen((dir+name+"_cross.txt").Data(), "w");
    fprintf(f1, "Best fit point %s = % 7.3f    %s = % 7.3f\n", x.Data(), gr0->GetX()[0], y.Data(), gr0->GetY()[0]);
    fprintf(f1, "One dimentional error bars: \n");
    fprintf(f1, "      %s   = % 7.3f   - % 7.3f / + % 7.3f\n", x.Data(), gr0->GetX()[0], gr0->GetX()[0]-xlo, xhi-gr0->GetX()[0]);
    fprintf(f1, "      %s   = % 7.3f   - % 7.3f / + % 7.3f\n", y.Data(), gr0->GetY()[0], gr0->GetY()[0]-ylo, yhi-gr0->GetY()[0]);
    fclose(f1);
}
void readMassScan2D(TFile *bands, TString name, TString rootfile) {
    readParamScan2D(bands,name,rootfile,"MH","r");
}

void readBayesParamScan2D(TFile *bands, TString name, TString rootfile, TString x, TString y, double mhMin, double mhMax, double muMin, double muMax, int binsX=100, int binsY=100) {
    return;
    if (/*error=*/gSystem->AccessPathName(rootfile)) return;
    TFile *f = TFile::Open(rootfile);
    TH2F *frame = new TH2F(name+"_th2", Form("%s;;;Posterior density (1/GeV)",name.Data()),
                                        binsX, mhMin, mhMax, binsY, muMin, muMax); 
    frame->SetContour(100);
    bayesPosterior2D(frame,x,y);
    bands->WriteTObject(frame);
    TGraph *gr0 = bestFit(frame);
    TList *gr68 = bayesContour(frame, 0.68);
    TList *gr95 = bayesContour(frame, 0.95);
    gr0->SetName(name+"_best"); bands->WriteTObject(gr0);
    gr68->SetName(name+"_c68"); bands->WriteTObject(gr68, gr68->GetName(), "SingleKey");
    gr95->SetName(name+"_c95"); bands->WriteTObject(gr95, gr95->GetName(), "SingleKey");
    f->Close();

}
void readBayesMassScan(TFile *bands, TString name, TString rootfile, double mhMin0=-1, double mhMax0=-1, double muMax=-1, int binsX=100, int binsY=100) {
    double mhMin=120., mhMax=130, muMax=5;
    if (mhMin0 != -1 && mhMax0 != -1 && muMax != -1) { 
        mhMin = mhMin; mhMax = mhMax; muMax = muMax0;
    } else {
        if (name.Contains("hzz") || name.Contains("qqH_hgg")) {
            mhMin = 115; mhMax = 135;
            if (name.Contains("qqH_hgg")) muMax = 10;
        }
    }
    readBayesParamScan2D(bands,name,rootfile,"MH","r",mhMin,mhMax,0,muMax,binsX,binsY);
}


void doIt(TFile *bands, TString name, TString rootfile) {
    if (name.Contains("mass_scan_1d")) { readMassScan1D(bands, name, rootfile); return; }
    if (name.Contains("mass_scan_2d")) { readMassScan2D(bands, name, rootfile); return; }
    if (name.Contains("mu_scan")) { readMuScan1D(bands, name, rootfile); return; }
    if (name.Contains("mass_bayes")) { readBayesMassScan(bands, name, rootfile); return; }
    if (/*error=*/gSystem->AccessPathName(rootfile)) return;
    zero_is_valid = (name.Contains("pval") || name.Contains("sig") || name.Contains("smcls") || name.Contains("ml_") || name.Contains("mlz_") || name.Contains("mlez_"));
    do_bands_asimov = name.Contains("pval");
    use_precomputed_quantiles = name.Contains("cls") || name.Contains("ml_") || name.Contains("mlz_") || name.Contains("mlez_") || name.Contains("pval_") || name.Contains("sigts_");
    do_bands_95 = !(name.Contains("ml_") || name.Contains("mlz_") || name.Contains("mlez_"));
    //if (name.Contains("pval") && name.Contains("comb")) do_bands_cputime = true;
    precomputed_median_only = name.Contains("pval_") || name.Contains("sigts_");
    makeBands(bands, name, rootfile, 0);
    std::cout << "Tryring to do " << name << std::endl;
    if (name.Index("ml") == 0) {
        if (bands->Get(name+"_median") == 0) {  std::cerr << "Missing median??" << std::endl; return; } 
        TGraphAsymmErrors *obs = (TGraphAsymmErrors *) bands->Get(name+"_median")->Clone();
        if (obs == 0) return;
        bands->Delete(name+"_obs;*");
        bands->Delete(name+"_median;*");
        obs->SetName(name+"_obs");
        bands->WriteTObject(obs, name+"_obs");
        printLineAErr(bands, name+"_obs", "results/"+prefix+name+".txt");
    } else {
        printBand(bands, name, "results/"+prefix+name+".txt", /*mean=*/false);
        if (rootfile.Contains("PLP")) {
            printLine(bands, name+"_obs", "results/"+prefix+name+"_obs.txt");
        }
    }
    if (name.Contains("sigts_")) {
        TString name2 = name; 
        name2.ReplaceAll("sigts_", "pvala_");
        testStatToPVals(bands,name,name2);
    } else if (name.Contains("pvala_")) {
        TString name2 = name; 
        name2.ReplaceAll("pvala_", "sig_");
        pvalToSignificance(bands,name+"_obs",name2+"_obs");
        printLine(bands, name2+"_obs", "results/"+prefix+name2+"_obs.txt");
        pvalToSignificance(bands,name+"_asimov",name2+"_asimov");
        printLine(bands, name2+"_asimov", "results/"+prefix+name2+"_asimov.txt");
    } 
    zero_is_valid = false;
    use_precomputed_quantiles = false; precomputed_median_only = false;
    do_bands_95 = true;
    do_bands_asimov = false;
    do_bands_cputime = false; do_bands_realtime = false;
}

#if 0
const int    nfcbands = 4;
const char * fcbandnam[nfcbands] = { "68", "95", "99", "9973" };
const double fcbandwid[nfcbands] = { 0.68, 0.95, 0.99, 0.9973 };
void doFc(TFile *bands, TString name, TString rootfile) {
    if (/*error=*/gSystem->AccessPathName(rootfile)) return;
    TFile *rfile = TFile::Open(rootfile);
    for (int i = 0; i < nfcbands; ++i) {
        std::cout << "Make " << name << std::endl;
        TGraphAsymmErrors *fcb = theFcBelt(rfile, 1, 0, Observed, fcbandwid[i]);
        if (fcb == 0) continue;
        fcb->SetName(name+"_"+fcbandnam[i]);
        bands->WriteTObject(fcb);
    }
    printFcBand(bands, name, name+".txt", nfcbands, fcbandnam);
}
#endif
void importCloned(TFile *bands, TString oldfile, TString oldname, int nwhat, const char **what, int nwho, const char **who) {
    TFile *oldBands = TFile::Open(oldfile);
    for (int i = -2; i < nwho; ++i) {
        TString whoI, who0;
        switch (i) {
            case -1: whoI = "comb"; who0 = "comb"; break;
            case -2: whoI = "hzz2l2q"; who0 = "hzz2l2q"; break;
            default: whoI = who[i]; who0 = who[i]; break;
        }
        if (oldname == "lp11" && whoI == "hww") whoI = "hwwc";
        for (int j = 0; j < nwhat; ++j) {
            TString name  = TString::Format("%s_%s", what[j], who0.Data());
            TString nameI = TString::Format("%s_%s", what[j], whoI.Data());
            if (!nameI.Contains("acls_comb")) continue;
            TGraphAsymmErrors *oldg_obs    = (TGraphAsymmErrors *) oldBands->Get(nameI+"_obs");
            TGraphAsymmErrors *oldg_median = (TGraphAsymmErrors *) oldBands->Get(nameI+"_median");
            TGraphAsymmErrors *oldg_medi95 = (TGraphAsymmErrors *) oldBands->Get(nameI+"_median_95");
            if (oldg_obs) {
                cleanCLs(oldg_obs);
                bands->WriteTObject(oldg_obs->Clone(name+"_"+oldname+"_obs"), name+"_"+oldname+"_obs");
                std::cout  << "Imported " << oldname << " result " << name << "_obs" << std::endl;
            }
            if (oldg_median) {
                cleanCLs(oldg_median);
                bands->WriteTObject(oldg_median->Clone(name+"_"+oldname+"_median"), name+"_"+oldname+"_median");
                std::cout  << "Imported " << oldname << " result " << name << "_median" << std::endl;
            }
            if (oldg_medi95) {
                cleanCLs(oldg_medi95);
                bands->WriteTObject(oldg_medi95->Clone(name+"_"+oldname+"_median_95"), name+"_"+oldname+"_median_95");
                std::cout  << "Imported " << oldname << " result " << name << "_median_95" << std::endl;
            }
        }
    }
    oldBands->Close();
}

void importLandSScan1D(TDirectory *bands, TString name, TString rootfile, double xmin=-9e9, double xmax=9e9) {
    if (/*error=*/gSystem->AccessPathName(rootfile)) return;
    TFile *f = TFile::Open(rootfile);
    TH1 *hist = (TH1*) f->Get("dummy");
    TGraph *ret = new TGraph(); int k = 0;
    int n = hist->GetNbinsX(); double ymin = +9e9;
    for (int i = 1; i <= n; ++i) {
        double x = hist->GetXaxis()->GetBinCenter(i);
        double y = hist->GetBinContent(i);
        if (y == 0 || x < xmin || x > xmax) continue;
        ret->Set(k+1);
        ret->SetPoint(k, x, y);
        k++;
        if (y < ymin) ymin = y;
    }
    double *yi = ret->GetY();
    for (int i = 0; i < k; ++i) {
        yi[i] -= ymin;
    }
    ret->SetName(name+"_obs");
    bands->WriteTObject(ret);
    printLine(bands, name+"_obs", "results/"+name+".txt");
    printValueFromScan1D(bands, name+"_obs", "results/"+name+"_crossings.txt"); 
    f->Close();
}
void importLandSContour2D(TDirectory *bands, TString name, TString landsname, TString rootfile, double xmin=-9e9, double xmax=9e9) {
    if (/*error=*/gSystem->AccessPathName(rootfile)) return;
    TFile *f = TFile::Open(rootfile);
    TGraph *ret = new TGraph(); int k = 0;
    double xc = 0, yc = 0;
    for (int i = 0; i < 999; ++i) {
        TGraph *gi = (TGraph*) f->Get(Form("%s_%d", landsname.Data(), i));
        if (gi == 0) break;
        for (int j = 0, n = gi->GetN(); j < n; ++j) {
            double x = gi->GetX()[j], y = gi->GetY()[j];
            if (x < xmin || x > xmax) continue;
            xc += x; yc += y;
            ret->Set(k+1);
            ret->SetPoint(k, gi->GetX()[j], gi->GetY()[j]);
            k++;
        }
    }
    xc /= k; yc /= k;
    Double_t *xi = ret->GetX(), *yi = ret->GetY();
    for (int i = 0; i < k; ++i) { xi[i] -= xc; yi[i] -= yc; }
    ret->Sort(&TGraph::CompareArg);
    for (int i = 0; i < k; ++i) {
         xi[i] += xc; yi[i] += yc; 
    }
    TList *tmg = new TList();
    tmg->Add(ret);
    tmg->SetName(name);
    bands->WriteTObject(tmg, name, "SingleKey");
    f->Close();
}
void importLandSScan2D(TDirectory *bands, TString name, TString rootfile) {
    if (/*error=*/gSystem->AccessPathName(rootfile)) return;
    TFile *f = TFile::Open(rootfile);
    TH2 *hist = (TH2*) f->Get("dummy");
    int nx = hist->GetNbinsX(), ny = hist->GetNbinsY(); double nllmin = 9e9;
    int ixmin, iymin;
    for (int ix = 1; ix <= nx; ++ix) { for (int iy = 1; iy <= ny; ++iy) { 
        double nll = hist->GetBinContent(ix,iy);
        if (nll == 0) continue;
        if (nll < nllmin) { nllmin = nll; ixmin = ix; iymin = iy; }
    } }
    for (int ix = 1; ix <= nx; ++ix) { for (int iy = 1; iy <= ny; ++iy) { 
        double nll = hist->GetBinContent(ix,iy);
        if (nll == 0) continue;
        hist->SetBinContent(ix, iy, nll - nllmin);
    } }
    hist->SetDirectory(0);
    hist->SetName(name+"_th2");
    bands->WriteTObject(hist);
    TList *c68 = contourFromTH2(hist, 2.30); c68->SetName(name+"_c68");
    TList *c95 = contourFromTH2(hist, 5.99); c68->SetName(name+"_c68");
    bands->WriteTObject(c68, name+"_c68", "SingleKey");
    bands->WriteTObject(c95, name+"_c95", "SingleKey");
    TGraph *best = new TGraph(1);
    best->SetPoint(0, hist->GetXaxis()->GetBinCenter(ixmin), hist->GetYaxis()->GetBinCenter(iymin));
    best->SetName(name+"_best");
    bands->WriteTObject(best);
    f->Close();
}
void importLandsMuHats(TDirectory *bands, TString file, double mass) {
    FILE *f = fopen(file.Data(), "r");
    char buff[1024];
    TString pst = "";
    while (fgets(buff,1023,f) != NULL) {
        TString line(buff);
        if (line.Contains("MUHAT 7 TeV @ 125")) { pst = "_7"; continue; }
        if (line.Contains("MUHAT 8 TeV @ 125")) { pst = "_8"; continue; }
        if (line.Contains("MUHAT 7+8 TeV @ 125")) { pst = ""; continue; }
        if (!line.Contains("-")) continue;
        std::cout << "LINE " << line << std::endl;  
        int itok = 0;
        TString graphName = ""; double mu, lo, hi;
        for (char *tok = strtok(buff, " "); tok != NULL; tok = strtok(NULL," "), ++itok) {
            TString name(tok);
            if (name == "combination") graphName = "comb";
            if (name == "ggH->ZZ") graphName = "hzz";
            if (name == "VH->WW") graphName = "VH_hww";
            if (name == "qqH->WW") graphName = "qqH_hww";
            if (name == "ggH->WW") graphName = "ggH_hww";
            if (name == "qqH->GG") graphName = "qqH_hgg";
            if (name == "ggH->GG") graphName = "ggH_hgg";
            if (name == "VH->TT") graphName = "VH_htt";
            if (name == "qqH->TT") graphName = "qqH_htt";
            if (name == "ggH->TT") graphName = "ggH_htt";
            if (name == "ttH->bb") graphName = "ttH_hbb";
            if (name == "VH->bb") graphName = "VH_hbb";
            if (name == "H->ZZ") graphName = "hzz";
            if (name == "H->WW") graphName = "hww";
            if (name == "H->GG") graphName = "hgg";
            if (name == "H->TT") graphName = "htt";
            if (name == "H->bb") graphName = "hbb";
            if (name == "qqH") graphName = "qqH";
            if (name == "ggH") graphName = "ggH";
            if (name == "ttH") graphName = "ttH";
            if (name == "VH") graphName = "VH";
            if (itok == 3) mu = atof(tok);
            if (itok == 5) hi = atof(tok);
            if (itok == 7) lo = atof(tok);
        }
        TGraphAsymmErrors *g = new TGraphAsymmErrors(1);
        g->SetName("mlz_"+graphName+pst+"_lands_obs");
        g->SetPoint(0, mass, mu);
        g->SetPointError(0, 0, 0, lo, hi);
        bands->WriteTObject(g);
        std::cout << "   created " << g->GetName() << " with mu = " << mu << " -" << lo << " +" << hi << std::endl;
    }
}


void importLandS(TDirectory *bands) {
    /*
    importLandS(bands, "acls_comb_7_lands", "lands/comb7tev_aclsobs.root", 1, 0);
    importLandS(bands, "acls_comb_7_lands", "lands/comb7tev_aclsexp.root", 0, 1);
    importLandS(bands, "pvala_comb_7_lands", "lands/comb7tev_pval.root", 1, 0);
    importLandS(bands, "acls_comb_8_lands", "lands/comb8tev_aclsobs.root", 1, 0);
    importLandS(bands, "acls_comb_8_lands", "lands/comb8tev_aclsexp.root", 0, 1);
    importLandS(bands, "acls_comb_lands", "lands/comb0tev_aclsobs.root", 1, 0);
    importLandS(bands, "acls_comb_lands", "lands/comb0tev_aclsexp.root", 0, 1);
    */
    importLandS(bands, "pvala_comb_8_lands", "lands/comb8tev_pval.root", 1, 0);
    importLandS(bands, "pvala_comb_lands", "lands/comb0tev_pval.root", 1, 0);
    importLandS(bands, "pvala_hires_lands", "lands/comb0tev_ggzz_pval.root", 1, 0);
    importLandS(bands, "pvala_bosons_lands", "lands/comb0tev_ggwwzz_pval.root", 1, 0);
     /*
    importLandSScan1D(bands, "mass_scan_1d_hgg_lands", "lands/massscan_1D_hgg.root");
    importLandSScan1D(bands, "mass_scan_1d_qqH_hgg_lands", "lands/massscan_1D_hgg_vbf.root");
    importLandSScan1D(bands, "mass_scan_1d_ggH_hgg_lands", "lands/massscan_1D_hgg_inc.root");
    importLandSScan1D(bands, "mass_scan_1d_hires_lands", "lands/massscan_1D_hggzz_3mu.root");
    importLandSScan1D(bands, "mass_scan_1d_hzz_lands", "lands/massscan_1D_hzz4l.root");
    importLandSScan2D(bands, "mass_scan_2d_hgg_lands", "lands/massscan_2D_hgg.root");
    importLandSScan2D(bands, "mass_scan_2d_qqH_hgg_lands", "lands/massscan_2D_hgg_vbf.root");
    importLandSScan2D(bands, "mass_scan_2d_ggH_hgg_lands", "lands/massscan_2D_hgg_inc.root");
    importLandSScan2D(bands, "mass_scan_2d_hires_lands", "lands/massscan_2D_hggzz.root");
    importLandSScan2D(bands, "mass_scan_2d_hzz_lands", "lands/massscan_2D_hzz4l.root");
    importLandSContour2D(bands, "mass_scan_2d_hzz_lands_c68", "hzz", "lands/mH_muhat_contours.root", 118, 135);
    importLandSContour2D(bands, "mass_scan_2d_ggH_hgg_lands_c68", "hgginc", "lands/mH_muhat_contours.root", 118, 135);
    importLandSContour2D(bands, "mass_scan_2d_qqH_hgg_lands_c68", "hggvbf", "lands/mH_muhat_contours.root", 118, 135);
    importLandSContour2D(bands, "mass_scan_2d_hires_lands_c68", "comb", "lands/mH_muhat_contours.root", 118, 135);
    */
    importLandsMuHats(bands, "lands/muhat.txt", 125.0);
}
void makeBands() {
    gROOT->LoadMacro("$CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/plotting/bandUtils.cxx+");

    TFile *bands = new TFile(Form("results/bands.root",prefix.Data()),"RECREATE");

    std::cout << "Will save bands to " << bands->GetName() << std::endl;
    
    obs_avg_mode = MedianObs;
    do_bands_nosyst = false;
    do_bands_mean   = false;
    do_bands_ntoys  = false;
    do_bands_asimov = false;
    halfint_masses = true;

    const int nwhat = 22;
    const char *what[nwhat] = { "pla", "sigts", "pvala", "pvala", "pval", "acls" , "ml", "mlz",  "cls",  "bayes", "smcls", "smacls", "acls90", "acls99",   "mass_scan_1d", "mass_scan_1d_fast", "mass_scan_2d", "mass_scan_2d_fast", "mass_bayes_2d", "mu_scan", "mu_scan_fast"};
    const char *WHAT[nwhat] = { "PLC", "SIGTS", "PLP",   "PLPE",  "PVAL", "ASCLS", "ML", "MLZ", "FREQ", "BAYES", "SMCLS", "SMASCLS", "ASCLS90", "ASCLS99", "MASS_SCAN_1D", "MASS_SCAN_1D_FAST", "MASS_SCAN_2D", "MASS_SCAN_2D_FAST", "MASS_BAYES", "MU_SCAN", "MU_SCAN_FAST"};

#if 0
    for (int e = 0; e < ne; ++e) {
    for (int i = 0; i < nchann; ++i) {
        //if (!TString(chann[i]).Contains("comb")) continue;
        //if (!TString(chann[i]).Contains("hzz") && !TString(chann[i]).Contains("hires")) continue;
        //doFc(bands, TString::Format("fc_%s",chann[i]), TString::Format("higgsCombine%s_FC.root", CHANN[i]));
        //continue;
        for (int j = 0; j < nwhat; ++j) {
            //if (!TString(what[j]).Contains("1d")) continue;
            TString THECHANN = TString::Format("%s%s", epre[e], CHANN[i]); THECHANN.ReplaceAll("COMB_",""); 
            if (i == 0) THECHANN = (e == 0 ? "" : Form("%d",e+6));
            doIt(bands, TString::Format("%s_%s%s", what[j], chann[i], epst[e]),
                        TString::Format("results/higgsCombine%s_%s.root", THECHANN.Data(), WHAT[j]));
            //if(j>4) break;
        }
    } // chann
    } // energy
#endif
    mergeBands(bands, "mlz_comb", "mlz_comb_clean", "mlz_comb");
    mergeBands(bands, "mlz_comb_7", "mlz_comb_7_clean", "mlz_comb_7");
    mergeBands(bands, "mlz_comb_8", "mlz_comb_8_clean", "mlz_comb_8");


    doIt(bands, "acls_comb2_7", "results/higgsCombine7_ASCLS_2ndSetDC.root");
    doIt(bands, "acls_comb2_8", "results/higgsCombine8_ASCLS_2ndSetDC.root");
    doIt(bands, "acls_comb2",   "results/higgsCombine_ASCLS_2ndSetDC.root");
    doIt(bands, "smacls_comb2", "results/higgsCombine_SMASCLS_2ndSetDC.root");

    stripBands(bands, "acls_comb",  112, 113.5);
    stripBands(bands, "acls_comb2", 112, 113.5);

#if 1
    //readParamScan1D(bands, "mass_scan_1d_onemu_hires", "results/higgsCombineHIRES_MASS_SCAN_1D_ONEMU.root", "MH");
    readParamScan1D(bands, "rwz_scan_1d_comb", "results/higgsCombineRWZ_WWZZ.root", "Rwz");
    readParamScan1D(bands, "rwz_scan_1d_ggH",  "results/higgsCombineRWZ_WWZZ_gg.root", "Rwz");
    readParamScan1D(bands, "cwz_scan_1d_comb", "results/higgsCombineCWZ_WWZZ.root",    "Cwz");
    readParamScan1D(bands, "cwz_scan_1d_ggH",  "results/higgsCombineCWZ_WWZZ_gg.root", "Cwz");

    readParamScan2D(bands, "cvcf_scan_2d_comb", "results/higgsCombineCVCF_CLEAN_SCAN.root", "CV", "CF");
    readParamScan2D(bands, "cvcf_cut_scan_2d_comb", "results/higgsCombineCVCF_CLEAN_SCAN.root", "CV", "CF", false);
    //readParamScan2D(bands, "cvcfmh_scan_2d_comb", "results/higgsCombineCVCF_CLEAN_NUISMH_SCAN.root", "CV", "CF");
    //readParamScan2D(bands, "cvcfmh_cut_scan_2d_comb", "results/higgsCombineCVCF_CLEAN_NUISMH_SCAN.root", "CV", "CF", false);
    readParamScan2D(bands, "cvcf_scan_2d_bloggers_comb", "results/higgsCombineBLOGGERS_CVCF_SCAN.root", "CV", "CF");
    readParamScan2D(bands, "cvcf_cut_scan_2d_bloggers_comb", "results/higgsCombineBLOGGERS_CVCF_SCAN.root", "CV", "CF", false);
    //readParamScan2D(bands, "cvcf_scan_2d_bloggers_hires", "results/higgsCombineBLOGGERS_HIRES_CVCF_SCAN.root", "CV", "CF");
    //readParamScan2D(bands, "cvcf_cut_scan_2d_bloggers_hires", "results/higgsCombineBLOGGERS_HIRES_CVCF_SCAN.root", "CV", "CF", false);
    readBayesParamScan2D(bands, "cvcf_bayes_2d_comb", "results/higgsCombineCVCF_CLEAN_BAYES.root", "CV", "CF", 0, 2.5, -2.5, 3.5 );
    readBayesParamScan2D(bands, "cvcf_cut_bayes_2d_comb", "results/higgsCombineCVCF_CLEAN_BAYES.root", "CV", "CF", 0, 2.5, 0, 3.5) ;

    //readParamScan2D(bands, Form("cvcf_scan_fast_2d_%s", chann[0]), Form("results/higgsCombineCVCF_%s_SCAN_FAST.root",     "CLEAN"), "CV",    "CF");
    //readParamScan2D(bands, Form("cvcf_cut_scan_fast_2d_%s", chann[0]), Form("results/higgsCombineCVCF_%s_SCAN_FAST.root", "CLEAN"), "CV",    "CF", false);
    for (int i = 0; i < nchann; ++i) {
        readParamScan2D(bands, Form("cvcf_scan_2d_%s", chann[i]), Form("results/higgsCombine%s_CVCF_SCAN.root", CHANN[i]), "CV",    "CF");
        readParamScan2D(bands, Form("cvcf_cut_scan_2d_%s", chann[i]), Form("results/higgsCombine%s_CVCF_SCAN.root", CHANN[i]), "CV",    "CF");
        //readParamScan2D(bands, Form("cvcf_scan_fast_2d_%s", chann[i]), Form("results/higgsCombineCVCF_%s_SCAN_FAST.root", CHANN[i]), "CV",    "CF");
        //readParamScan2D(bands, Form("cvcf_cut_scan_fast_2d_%s", chann[i]), Form("results/higgsCombineCVCF_%s_SCAN_FAST.root", CHANN[i]), "CV",    "CF", false);
        readParamScan2D(bands, Form("xs_scan_2d_%s",   chann[i]), Form("results/higgsCombine%s_XS2D_SCAN.root", CHANN[i]), "r_qqH", "r_ggH");
    }

    importLandS(bands);
    importCloned(bands, "results/bands.moriond.root", "moriond", nwhat, what, nchann, chann);
#endif


}





// function to measure the area in one contour
float contourArea(TList *contour, TH2F* map){
  float area=0;
  // brute force, for each bin in the TH2F 
  // if the bin is in the contour then add it to the area measurement
  int nBinX=map->GetNbinsX();
  int nBinY=map->GetNbinsY();

  for(int ibinX=0; ibinX< nBinX; ibinX++){
    for(int ibinY=0; ibinY< nBinY; ibinY++){
      std::cout << ibinX << "\t" << ibinY << "\t" << map->GetBinContent(ibinX,ibinY);
      area+=map->GetXaxis()->GetBinWidth(ibinX)*map->GetYaxis()->GetBinWidth(ibinY);
    }
  }
  return area;
}

TH2D* contourArea(TGraph *contour,TH2D* map){

  //TProfile2D *hist = new TProfile2D(*map);
  TH2D *hist = new TH2D(*map);
  if(hist==NULL) return NULL;
  //  hist->Print();
  hist->Reset();
  
  TGraph *g = (TGraph *) contour;
  Double_t x=0,y=0;

  Int_t nPoints=g->GetN();
  //  std::cout << "nPoints = " << nPoints << std::endl;
//   for(int iPoint=0; iPoint < nPoints; iPoint++){
//     g->GetPoint(iPoint,x,y);
//     hist->Fill(x,y,1);
//   }

  int nBinX=hist->GetNbinsX();
  int nBinY=hist->GetNbinsY();

   for(int ibinX=0; ibinX< nBinX; ibinX++){
     for(int ibinY=0; ibinY< nBinY; ibinY++){
      //      std::cout << ibinX << "\t" << ibinY << "\t" << map->GetXaxis()->GetBinWidth(ibinX) << "\t" << map->GetYaxis()->GetBinWidth(ibinY) << "\t" << map->GetBinContent(ibinX,ibinY) << std::endl;
      //      if(
      //      area+=map->GetXaxis()->GetBinWidth(ibinX)*map->GetYaxis()->GetBinWidth(ibinY);
       Double_t binCenterX = hist->GetXaxis()->GetBinCenter(ibinX);
       Double_t binCenterY = hist->GetYaxis()->GetBinCenter(ibinY);

       int isInside = g->IsInside(binCenterX, binCenterY);
       //if(fabs(hist->GetXaxis()->GetBinCenter(ibinX) - 1) <0.1)
       //      std::cout << ibinX << "\t" << ibinY 
       //		<< "\t" << hist->GetXaxis()->GetBinCenter(ibinX) << "\t" << hist->GetYaxis()->GetBinCenter(ibinY) 
       //		<< "\t" << isInside << std::endl;
       if(isInside==1) hist->Fill(binCenterX, binCenterY,1);
     }
   }
  return hist;

}


float contourArea(TH2D* map, TList *contourList, int contourIndex=0){

  float integral=-1;
  for(int i=0; i < contourList->GetSize(); i++){
    TGraph *contour = (TGraph *) contourList->At(i);
    TH2D *hist = contourArea(contour, map);
    if(hist==NULL) return -1;
    if(i==contourIndex) integral = hist->Integral();
    std::cout << "Area:boardLength contour " << i << " = " << hist->Integral() << ":" <<contour->GetN() << std::endl;
    delete hist;
  }
  return integral;
  
}
