{
	//TFile *_file0 = TFile::Open("tree.root");//Open File
	//TTree *eventTree = (TTree*)flashggTreeMakerWithTagSorter->Get("flashggTreeMakerWithTagSorter"); //access tree
	TTree *eventTree = (TTree*)flashggTreeMakerWithTagSorter->Get("flashggTreeWithTagSorter"); //access tree

	/*	struct eventInfo { //define struct to house the values held in the branch
			float genVertexZ;
			float zerothVertexZ;
			float diphotonVertexZ;
			float higgsPt;
			};*/

//	eventInfo eInfo; 


	float dijet_LeadJPt;
	float dijet_SubJPt;
	int leadjet_genmatch;
	int subljet_genmatch;
	int flash_VBFTag_Category;
	float pu_n;

	float dijet_leadEta;
	float dijet_subleadEta;
	float dijet_LeadJPt;
	float dijet_SubJPt;
	float dijet_Zep;
	float dijet_dPhi_trunc; 
	float dijet_Mjj;
	float dipho_PToM;


	//TBranch eventBranch;
	/*	TBranch dijet_LeadJPt;
			TBranch dijet_SubJPt;
			TBranch leadjet_genmatch;
			TBranch subljet_genmatch;
			TBranch flash_VBFTag_Category;*/
	// trick branch into filling all three values of the struct by asking it to fill the first. The other three get filled automatically. 
	//eventTree->SetBranchAddress("eventBranch", &eInfo.genVertexZ); 
	eventTree->SetBranchAddress("dijet_leadEta", &dijet_leadEta); 
	eventTree->SetBranchAddress("dijet_subleadEta", &dijet_subleadEta); 
	eventTree->SetBranchAddress("dijet_Zep", &dijet_Zep); 
	eventTree->SetBranchAddress("dijet_dPhi", &dijet_dPhi_trunc); 
	eventTree->SetBranchAddress("dijet_Mjj", &dijet_Mjj); 
	eventTree->SetBranchAddress("dipho_PToM", &dipho_PToM); 
	eventTree->SetBranchAddress("dijet_LeadJPt", &dijet_LeadJPt); 
	eventTree->SetBranchAddress("dijet_SubJPt", &dijet_SubJPt); 
	eventTree->SetBranchAddress("leadjet_genmatch", &leadjet_genmatch); 
	eventTree->SetBranchAddress("subljet_genmatch", &subljet_genmatch); 
	eventTree->SetBranchAddress("flash_VBFTag_Category", &flash_VBFTag_Category); 
	eventTree->SetBranchAddress("pu_n", &pu_n); 

	TH1F *ptnum0 = new TH1F("ptnum0","ptnum0",50,0,250); //define histos for numerator and denominator
	TH1F *ptden0 = new TH1F("ptden0","ptden0",50,0,250);
	TH1F *pu_denVBF = new TH1F("pu_denVBF","pu_denVBF",50,0,250);
	TH1F *pu_numVBF = new TH1F("pu_numVBF","pu_numVBF",50,0,250);
//	TH1F *ptnumLeg = new TH1F("ptnumLeg","ptnumLeg",50,0,250);
//	TH1F *ptdenLeg = new TH1F("ptdenLeg","ptdenLeg",50,0,250);

	TH1F *h_dijet_leadEta     = new TH1F("leadEta_7","",1000,-5,5);
	TH1F *h_dijet_subleadEta  = new TH1F("sublEta_7","",1000,-7,7);
	TH1F *h_dijet_LeadJPt     = new TH1F("leadJPt_7","",1000,0,200);
	TH1F *h_dijet_SubJPt      = new TH1F("sublJpt_7","",1000,0,200);
	TH1F *h_dijet_Zep         = new TH1F("zep_7","",1000,-50,50);
	TH1F *h_dijet_dPhi_trunc  = new TH1F("dPhi_7","",1000,-5,5); 
	TH1F *h_dijet_Mjj         = new TH1F("Mjj_7","",1000,0,200);
	TH1F *h_dipho_PToM        = new TH1F("diPhopToM_7","",1000,0,10);


	int counter =0;
	int VBFcounter =0;

	for( Long64_t loop = 0 ; loop < eventTree->GetEntries(); loop++ )
	{
		eventTree->GetEntry(loop);
		//std::cout << flash_VBFTag_Category  <<std::endl;
		pu_denVBF->Fill(pu_n);
		if(flash_VBFTag_Category != -1){
		VBFcounter++;	
		pu_numVBF->Fill(pu_n);
	//	ptnumVBF->Fill(dijet_LeadJPt);
	//

h_dijet_leadEta     ->Fill(   dijet_leadEta                           );
h_dijet_subleadEta ->Fill(    dijet_subleadEta                        );
h_dijet_LeadJPt    ->Fill(    dijet_LeadJPt                           );
h_dijet_SubJPt     ->Fill(    dijet_SubJPt                            );
h_dijet_Zep        ->Fill(    dijet_Zep                               );
h_dijet_dPhi_trunc ->Fill(    dijet_dPhi_trunc                        );
h_dijet_Mjj        ->Fill(    dijet_Mjj                               );
h_dipho_PToM       ->Fill(    dipho_PToM                              );
		
		}
	//	std::cout << eInfo.zerothVertexZ << std::endl;
		if((dijet_LeadJPt <0) || (dijet_SubJPt <0)) {counter++; continue;} 




		ptden0->Fill(dijet_LeadJPt);
		ptden0->Fill(dijet_SubJPt);
		//ptdenLeg->Fill(eInfo.higgsPt);
	//	std::cout << leadjet_genmatch << "	" << subljet_genmatch << std::endl;

		if(leadjet_genmatch ==1) 		{
			ptnum0->Fill(dijet_LeadJPt);
		}
		if(subljet_genmatch ==1) 		{
			ptnum0->Fill(dijet_SubJPt);
		}
		

	}

	TGraphAsymmErrors *graph0 = new TGraphAsymmErrors(ptnum0,ptden0);
	TGraphAsymmErrors *graphVBF = new TGraphAsymmErrors(pu_numVBF,pu_denVBF);
	graph0->SetName("graph0_7");
	graphVBF->SetName("graphVBF_7");

if (red){	
h_dijet_leadEta     ->SetLineColor(kRed);
h_dijet_subleadEta ->SetLineColor(kRed);
h_dijet_LeadJPt    ->SetLineColor(kRed);
h_dijet_SubJPt     ->SetLineColor(kRed);
h_dijet_Zep        ->SetLineColor(kRed);
h_dijet_dPhi_trunc ->SetLineColor(kRed);
h_dijet_Mjj        ->SetLineColor(kRed);
h_dipho_PToM       ->SetLineColor(kRed);

h_dijet_leadEta    ->SetName( "leadEta_5"          );
h_dijet_subleadEta ->SetName( "sublEta_5"                );
h_dijet_LeadJPt    ->SetName( "leadJPt_5"                );
h_dijet_SubJPt     ->SetName( "sublJpt_5"                );
h_dijet_Zep        ->SetName( "zep_5"               );
h_dijet_dPhi_trunc ->SetName( "dPhi_5"               );
h_dijet_Mjj        ->SetName( "Mjj_5"           );
h_dipho_PToM       ->SetName( "diPhopToM_5"              );
  graphVBF->SetLineColor(kRed);
  graphVBF ->SetName("graphVBF_5"        );
	graph0->SetLineColor(kRed);
	graph0 ->SetName( "graph0_5"              );
}

TFile *t = new TFile("histo.root","update");


	//	TGraphAsymmErrors *graphLeg = new TGraphAsymmErrors(ptnumLeg,ptdenLeg);
	////	graphLeg->SetMarkerColor(kRed); 
	//	graphLeg->SetLineColor(kRed);
	//graphLeg->Draw("P");

//	TFile *t = new TFile("histo.root","recreate");
	//graph0->Draw("AP"); 
	graph0->Write();
	//c1->SaveAs("matchedJets.pdf");
	//graphVBF->Draw("AP"); 
	graphVBF->Write();
	//c1->SaveAs("VBFTagged.pdf");
h_dijet_leadEta     ->Write( );
h_dijet_subleadEta ->Write( );
h_dijet_LeadJPt    ->Write( );
h_dijet_SubJPt     ->Write( );
h_dijet_Zep        ->Write( );
h_dijet_dPhi_trunc ->Write( );
h_dijet_Mjj        ->Write( );
h_dipho_PToM       ->Write( );



t->Close();

	std:: cout << "Statistics: " << eventTree->GetEntries() << " events." << std::endl;
	std:: cout << "skipped " << counter << " events." << std::endl;
	std:: cout << "VBF " << VBFcounter << " events." << std::endl;
}
