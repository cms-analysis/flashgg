{
TFile *oldFILE = new TFile("./vertex_reweighing_mva_Moriond.root");

oldFILE->Print();
oldFILE->ls();

TFile *newFILE = TFile::Open("../AnalysisScripts/aux/sig_reweighing_moriond2013_run2012C_baseline_v1.root","recreate");
newFILE->cd();

TGraphAsymmErrors * toCopy;

// Single Valued TF1 MC->Data corrections
int ncats = 8;
// Fill in the values and their errors - one entry per category
// HIGHPT - EBhighr9, EBlowR9, EEhighR9, EElowR9
// Since this is a Diphoton Smear, keep the 8 categories for back compatibility
// L1HLT ------------------------
// Numbers from Xiaohang: https://hypernews.cern.ch/HyperNews/CMS/get/higgs2g/995.html

// baseline:
Double_t effL1HLT_[ncats] 	       = {0.9978,0.9978,0.9978,0.9978,0.9978,0.9978,0.9978,0.9978};

// mva:
//Double_t effL1HLT_[ncats] 	       = {0.9968,0.9968,0.9968,0.9968,0.9968,0.9968,0.9968,0.9968};

Double_t effL1HLT_low_err_[ncats]  = {0.0003,0.0003,0.0003,0.0003,0.0003,0.0003,0.0003,0.0003};
Double_t effL1HLT_high_err_[ncats] = {0.0003,0.0003,0.0003,0.0003,0.0003,0.0003,0.0003,0.0003};

for (int cat=0;cat<ncats;cat++){
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
   Int_t n=2;  
   Double_t effL1HLT_EBHighR9_x[n]   = {0, 10000};
   Double_t effL1HLT_EBHighR9_y[n]   = {effL1HLT_[cat],effL1HLT_[cat]};
   Double_t effL1HLT_EBHighR9_exl[n] = {0., 0.};
   Double_t effL1HLT_EBHighR9_exh[n] = {0., 0.};
   Double_t effL1HLT_EBHighR9_eyl[n] = {effL1HLT_low_err_[cat],effL1HLT_low_err_[cat]};
   Double_t effL1HLT_EBHighR9_eyh[n] = {effL1HLT_high_err_[cat],effL1HLT_high_err_[cat]};
   TGraphAsymmErrors* gr = new TGraphAsymmErrors(n,effL1HLT_EBHighR9_x,effL1HLT_EBHighR9_y,effL1HLT_EBHighR9_exl,effL1HLT_EBHighR9_exh,effL1HLT_EBHighR9_eyl,effL1HLT_EBHighR9_eyh);
   gr->SetTitle(Form("effL1HLT_cat%d",cat));
   gr->SetName(Form("effL1HLT_cat%d",cat));
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->Draw("ALP");
   gr->Write();
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ID efficieny corrections ------------------------------------------------------------------------------------------------------------------------------------------------------
// presel and id numbers from matteo: https://hypernews.cern.ch/HyperNews/CMS/get/AUX/2012/10/07/08:20:52-62393-photonID_scaleFactors_HCP_Freezing.dat
// eleVeto numbers from Nancy: https://hypernews.cern.ch/HyperNews/CMS/get/higgs2g/819/1/1.html
// Errors all listed in eff_errs.dat
// can combine efficiencies and errors with effError.py 
int nphocats=4;

//baseline Run2012ABCD - Moriond2013
 Double_t ratioTP_[nphocats]            = {0.987,0.960,0.988,0.959};
 Double_t ratioTP_low_err_[nphocats]    = {0.004,0.011,0.012,0.022};
 Double_t ratioTP_high_err_[nphocats]   = {0.004,0.011,0.012,0.022};
 
//mva Run2012ABCD - Moriond2013
// Double_t ratioTP_[nphocats]            = {0.998,0.969,0.994,0.957};
// Double_t ratioTP_low_err_[nphocats]    = {0.003,0.007,0.010,0.019};
// Double_t ratioTP_high_err_[nphocats]   = {0.003,0.007,0.010,0.019};

//baseline Run2012ABC - Moriond2013
// Double_t ratioTP_[nphocats]            = {0.990,0.966,0.994,0.969};
// Double_t ratioTP_low_err_[nphocats]    = {0.004,0.010,0.011,0.023};
// Double_t ratioTP_high_err_[nphocats]   = {0.004,0.010,0.011,0.023};
 
//mva Run2012ABC - Moriond2013
// Double_t ratioTP_[nphocats]            = {0.997,0.969,0.994,0.957};
// Double_t ratioTP_low_err_[nphocats]    = {0.003,0.006,0.009,0.019};
// Double_t ratioTP_high_err_[nphocats]   = {0.003,0.006,0.009,0.019};

//baseline Run2012D - Moriond2013
// Double_t ratioTP_[nphocats]            = {0.983,0.945,0.976,0.946};
// Double_t ratioTP_low_err_[nphocats]    = {0.004,0.011,0.013,0.023};
// Double_t ratioTP_high_err_[nphocats]   = {0.004,0.011,0.013,0.023};
 
//mva Run2012D - Moriond2013
// Double_t ratioTP_[nphocats]            = {1.002,0.967,0.993,0.960};
// Double_t ratioTP_low_err_[nphocats]    = {0.003,0.008,0.011,0.020};
// Double_t ratioTP_high_err_[nphocats]   = {0.003,0.008,0.011,0.020};

//baseline Run2012AB - Moriond2013
//Double_t ratioTP_[nphocats]            = {0.993,0.980,0.990,0.974};
//Double_t ratioTP_low_err_[nphocats]    = {0.004,0.010,0.011,0.022};
//Double_t ratioTP_high_err_[nphocats]   = {0.004,0.010,0.011,0.022};

//mva Run2012AB - Moriond2013
//Double_t ratioTP_[nphocats]            = {0.995,0.974,0.995,0.966};
//Double_t ratioTP_low_err_[nphocats]    = {0.003,0.006,0.009,0.018};
//Double_t ratioTP_high_err_[nphocats]   = {0.003,0.006,0.009,0.018};

//baseline Run2012C - Moriond2013
//Double_t ratioTP_[nphocats]            = {0.988,0.958,0.997,0.964};
//Double_t ratioTP_low_err_[nphocats]    = {0.004,0.010,0.011,0.023};
//Double_t ratioTP_high_err_[nphocats]   = {0.004,0.010,0.011,0.023};

//mva Run2012C - Moriond2013
//Double_t ratioTP_[nphocats]            = {0.998,0.966,0.994,0.949};
//Double_t ratioTP_low_err_[nphocats]    = {0.003,0.006,0.009,0.019};
//Double_t ratioTP_high_err_[nphocats]   = {0.003,0.006,0.009,0.019};



std::string iDLabels_[nphocats] 	    = {"EBHighR9","EBLowR9","EEHighR9","EELowR9"};

for (int cat=0;cat<nphocats;cat++){
   Int_t n=2;  
   Double_t ratioTP_EBHighR9_x[n]   = {0, 10000};
   Double_t ratioTP_EBHighR9_y[n]   = {ratioTP_[cat],ratioTP_[cat]};
   Double_t ratioTP_EBHighR9_exl[n] = {0., 0.};
   Double_t ratioTP_EBHighR9_exh[n] = {0., 0.};
   Double_t ratioTP_EBHighR9_eyl[n] = {ratioTP_low_err_[cat],ratioTP_low_err_[cat]};
   Double_t ratioTP_EBHighR9_eyh[n] = {ratioTP_high_err_[cat],ratioTP_high_err_[cat]};
   TGraphAsymmErrors* gr = new TGraphAsymmErrors(n,ratioTP_EBHighR9_x,ratioTP_EBHighR9_y,ratioTP_EBHighR9_exl,ratioTP_EBHighR9_exh,ratioTP_EBHighR9_eyl,ratioTP_EBHighR9_eyh);
   gr->SetTitle(Form("ratioTP_%s",iDLabels_[cat].c_str()));
   gr->SetName(Form("ratioTP_%s",iDLabels_[cat].c_str()));
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->Draw("ALP");
   gr->Write();
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// R9 Part -------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//float effEBHighR9 = 0.67412; 	// Should be checked if this is the same in the S6 Fall11 MC samples
//float effEEHighR9 = 0.68308; 
float effEBHighR9 = 0.6916; 	// Should be checked if this is the same in the S6 Fall11 MC samples
float effEEHighR9 = 0.6916; 

float effEBLowR9 = 1.-effEBHighR9; 
float effEELowR9 = 1.-effEEHighR9; 

float errEBHighR9  = 0.04;
float errEEHighR9  = 0.065;
float errEBLowR9   = 1. - ( 1. - effEBHighR9*(1.+errEBHighR9) ) / effEBLowR9;
float errEELowR9   = 1. - ( 1. - effEEHighR9*(1.+errEEHighR9) ) / effEELowR9;

   Int_t n=2;  
   Double_t ratioR9_EBHighR9[n]   = {0, 10000};
   Double_t ratioR9_EBHighR9_y[n]   = {1, 1};
   Double_t ratioR9_EBHighR9_exl[n] = {0., 0.};
   Double_t ratioR9_EBHighR9_exh[n] = {0., 0.};
   Double_t ratioR9_EBHighR9_eyl[n] = {errEBHighR9, errEBHighR9};
   Double_t ratioR9_EBHighR9_eyh[n] = {errEBHighR9, errEBHighR9};
   TGraphAsymmErrors* gr = new TGraphAsymmErrors(n,ratioR9_EBHighR9,ratioR9_EBHighR9_y,ratioR9_EBHighR9_exl,ratioR9_EBHighR9_exh,ratioR9_EBHighR9_eyl,ratioR9_EBHighR9_eyh);
   gr->SetTitle("ratioR9_EBHighR9");
   gr->SetName("ratioR9_EBHighR9");
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->Draw("ALP");
   gr->Write();


   Int_t n=2;  
   Double_t ratioR9_EBLowR9[n]   = {0, 10000};
   Double_t ratioR9_EBLowR9_y[n]   = {1, 1};
   Double_t ratioR9_EBLowR9_exl[n] = {0., 0.};
   Double_t ratioR9_EBLowR9_exh[n] = {0., 0.};
   Double_t ratioR9_EBLowR9_eyl[n] = {errEBLowR9, errEBLowR9};
   Double_t ratioR9_EBLowR9_eyh[n] = {errEBLowR9, errEBLowR9};
   TGraphAsymmErrors* gr = new TGraphAsymmErrors(n,ratioR9_EBLowR9,ratioR9_EBLowR9_y,ratioR9_EBLowR9_exl,ratioR9_EBLowR9_exh,ratioR9_EBLowR9_eyl,ratioR9_EBLowR9_eyh);
   gr->SetTitle("ratioR9_EBLowR9");
   gr->SetName("ratioR9_EBLowR9");
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->Draw("ALP");
   gr->Write();

   Int_t n=2;  
   Double_t ratioR9_EEHighR9[n]   = {0, 10000};
   Double_t ratioR9_EEHighR9_y[n]   = {1, 1};
   Double_t ratioR9_EEHighR9_exl[n] = {0., 0.};
   Double_t ratioR9_EEHighR9_exh[n] = {0., 0.};
   Double_t ratioR9_EEHighR9_eyl[n] = {errEEHighR9, errEEHighR9};
   Double_t ratioR9_EEHighR9_eyh[n] = {errEEHighR9, errEEHighR9};
   TGraphAsymmErrors* gr = new TGraphAsymmErrors(n,ratioR9_EEHighR9,ratioR9_EEHighR9_y,ratioR9_EEHighR9_exl,ratioR9_EEHighR9_exh,ratioR9_EEHighR9_eyl,ratioR9_EEHighR9_eyh);
   gr->SetTitle("ratioR9_EEHighR9");
   gr->SetName("ratioR9_EEHighR9");
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->Draw("ALP");
   gr->Write();


   Int_t n=2;  
   Double_t ratioR9_EELowR9[n]   = {0, 10000};
   Double_t ratioR9_EELowR9_y[n]   = {1, 1};
   Double_t ratioR9_EELowR9_exl[n] = {0., 0.};
   Double_t ratioR9_EELowR9_exh[n] = {0., 0.};
   Double_t ratioR9_EELowR9_eyl[n] = {errEELowR9, errEELowR9};
   Double_t ratioR9_EELowR9_eyh[n] = {errEELowR9, errEELowR9};
   TGraphAsymmErrors* gr = new TGraphAsymmErrors(n,ratioR9_EELowR9,ratioR9_EELowR9_y,ratioR9_EELowR9_exl,ratioR9_EELowR9_exh,ratioR9_EELowR9_eyl,ratioR9_EELowR9_eyh);
   gr->SetTitle("ratioR9_EELowR9");
   gr->SetName("ratioR9_EELowR9");
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->Draw("ALP");
   gr->Write();

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Assumes that we only have the Vertex parts in the file already, they get copied across:
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat0_pass"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat0_fail"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat1_pass"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat1_fail"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat2_pass"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat2_fail"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat3_pass"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat3_fail"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat4_pass"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat4_fail"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat5_pass"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat5_fail"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat6_pass"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat6_fail"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat7_pass"); toCopy->Write(); 
toCopy = (TGraphAsymmErrors *) oldFILE->Get("ratioVertex_cat7_fail"); toCopy->Write(); 

newFILE->Close();


}
