{
	TFile *_file0 = TFile::Open("VertexValidationTree.root");//Open File
	TTree *eventTree = (TTree*)flashggVertexValidationTreeMaker->Get("eventTree"); //access tree

	struct eventInfo { //define struct to house the values held in the branch
		float genVertexZ;
		float zerothVertexZ;
		float higgsPt;
	};

	eventInfo eInfo; 

	TBranch eventBranch;
	// trick branch into filling all three values of the struct by asking it to fill the first. The other two get filled automatically. 
	eventTree->SetBranchAddress("eventBranch", &eInfo.genVertexZ); 

	TH1F *ptnum = new TH1F("ptnum","ptnum",100,0,250); //define histos for numerator and denominator

	TH1F *ptden = new TH1F("ptden","ptden",100,0,250);

	int counter =0;

	for( Long64_t loop = 0 ; loop< eventTree->GetEntries(); loop++)
	{
	std::cout << eInfo.zerothVertexZ << std::endl;
		if(fabs(eInfo.zerothVertexZ) >9998)
	
	{counter++;
	continue;}


		eventTree->GetEntry(loop);
		ptden->Fill(eInfo.higgsPt);

		if(fabs(eInfo.genVertexZ-eInfo.zerothVertexZ)< 1.) // only fill if reco vertex is within 1cm of true
		{
			ptnum->Fill(eInfo.higgsPt);
		}
	}

	TGraphAsymmErrors *graph= new TGraphAsymmErrors(ptnum,ptden);
	graph->Draw("AP"); 
	c1->SaveAs("vertexEfficiency0.png");

	std:: cout << "Statistics: " << eventTree->GetEntries() << " events." << std::endl;
	std:: cout << "skipped " << counter << " events." << std::endl;
}
