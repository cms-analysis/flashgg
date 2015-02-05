{
	TFile *inF = TFile::Open("");
	TFile *outF = new TFile("CorrNuis.root","RECREATE")
	int sqrtS=7;

	RooWorkspace *work = (RooWorkspace*)inF->Get(


}
