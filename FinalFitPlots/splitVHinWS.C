#include <iostream>
#include <string>
#include <vector>

#include "TFile.h"
#include "RooWorkspace.h"
#include "RooAbsArg.h"
#include "RooAbsPdf.h"
#include "RooArgSet.h"
#include "RooRealVar.h"

#include "TIterator.h"

using namespace std;
using namespace RooFit;

void splitVH(string infilename, string outfilename, bool is2011=false){
  
  TFile *inFile = TFile::Open(infilename.c_str());
  TFile *outFile = TFile::Open(outfilename.c_str(),"RECREATE");

  RooWorkspace *inWS;
  if (is2011) inWS = (RooWorkspace*)inFile->Get("wsig");
  else inWS = (RooWorkspace*)inFile->Get("wsig_8TeV");
  RooWorkspace *myWS = (RooWorkspace*)inWS->Clone(Form("%s_clone",inWS->GetName()));
  RooWorkspace *outWS = new RooWorkspace(inWS->GetName());

  RooRealVar *mass = (RooRealVar*)inWS->var("CMS_hgg_mass");
  mass->setBins(320);

  RooArgSet pdfs = inWS->allPdfs();

  RooAbsArg *parg;

  TIterator *iter = pdfs.createIterator();

  string name;
  if (is2011) name ="hggpdfsmrel";
  else name="hggpdfrel";


  while((parg=(RooAbsArg*)iter->Next())) {
    
    RooAbsPdf *pdf = (RooAbsPdf*)inWS->pdf(parg->GetName());
    RooAbsArg *norm;
    if (string(pdf->GetName()).find(name)!=string::npos) {
      norm = (RooAbsArg*)inWS->arg(Form("%s_norm",pdf->GetName()));
      //pdf->Print();
      cout << pdf->GetName() << endl;
      cout << norm->GetName() << endl;
      if (string(pdf->GetName()).find("wzh")==string::npos){
        outWS->import(*pdf,RecycleConflictNodes());
        outWS->import(*norm,RecycleConflictNodes());
      }
      else {
        RooAbsPdf *pdfClone = (RooAbsPdf*)myWS->pdf(pdf->GetName());
        RooAbsArg *normClone = (RooAbsArg*)myWS->arg(norm->GetName());
        string wname = pdf->GetName();
        string zname = pdf->GetName();
        wname.replace(wname.find("wzh"),3,"wh");
        zname.replace(zname.find("wzh"),3,"zh");
        cout << wname << " " << zname << endl;
        // WH
        norm->SetName(Form("%s_norm",wname.c_str()));
        RooAbsPdf *whPdf = (RooAbsPdf*)pdf->Clone(wname.c_str());
        outWS->import(*norm,RecycleConflictNodes());
        outWS->import(*whPdf,RecycleConflictNodes());
        // ZH
        normClone->SetName(Form("%s_norm",zname.c_str()));
        RooAbsPdf *zhPdf = (RooAbsPdf*)pdfClone->Clone(zname.c_str());
        outWS->import(*normClone,RecycleConflictNodes());
        outWS->import(*zhPdf,RecycleConflictNodes());
      }
    }
  }
  
  outWS->Write();

  inFile->Close();
  outFile->Close();
}
