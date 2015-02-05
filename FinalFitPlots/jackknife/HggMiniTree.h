//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Nov  8 15:00:18 2012 by ROOT version 5.32/00
// from TTree hgg_mini_tree/hgg_mini_tree
// found on file: eventList_ICHEP53x.root
//////////////////////////////////////////////////////////

#ifndef HggMiniTree_h
#define HggMiniTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class HggMiniTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           run;
   Int_t           lumis;
   Int_t           event;
   Int_t           category;
   Float_t         diphotonMVA;
   Float_t         CMS_hgg_mass;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_lumis;   //!
   TBranch        *b_event;   //!
   TBranch        *b_category;   //!
   TBranch        *b_diphotonMVA;   //!
   TBranch        *b_CMS_hgg_mass;   //!

   HggMiniTree(TTree *tree=0);
   virtual ~HggMiniTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef HggMiniTree_cxx
HggMiniTree::HggMiniTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("eventList_ICHEP53x.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("eventList_ICHEP53x.root");
      }
      f->GetObject("hgg_mini_tree",tree);

   }
   Init(tree);
}

HggMiniTree::~HggMiniTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t HggMiniTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t HggMiniTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void HggMiniTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumis", &lumis, &b_lumis);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("category", &category, &b_category);
   /// fChain->SetBranchAddress("diphotonMVA", &diphotonMVA, &b_diphotonMVA);
   fChain->SetBranchAddress("CMS_hgg_mass", &CMS_hgg_mass, &b_CMS_hgg_mass);
   Notify();
}

Bool_t HggMiniTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void HggMiniTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t HggMiniTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef HggMiniTree_cxx
