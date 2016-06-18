{
    TFile *_file0 = TFile::Open( "VertexValidationTree.root" ); //Open File
    TTree *eventTree = ( TTree * )flashggVertexValidationTreeMaker->Get( "eventTree" ); //access tree

    struct eventInfo { //define struct to house the values held in the branch
        float genVertexZ;
        float zerothVertexZ;
        float diphotonVertexZ;
        float higgsPt;
    };

    eventInfo eInfo;

    TBranch eventBranch;
    // trick branch into filling all three values of the struct by asking it to fill the first. The other three get filled automatically.
    eventTree->SetBranchAddress( "eventBranch", &eInfo.genVertexZ );

    TH1F *ptnum0 = new TH1F( "ptnum0", "ptnum0", 50, 0, 250 ); //define histos for numerator and denominator
    TH1F *ptden0 = new TH1F( "ptden0", "ptden0", 50, 0, 250 );
    TH1F *ptnumLeg = new TH1F( "ptnumLeg", "ptnumLeg", 50, 0, 250 );
    TH1F *ptdenLeg = new TH1F( "ptdenLeg", "ptdenLeg", 50, 0, 250 );


    int counter = 0;

    for( Long64_t loop = 0 ; loop < eventTree->GetEntries(); loop++ )
    {
        eventTree->GetEntry( loop );

        std::cout << eInfo.zerothVertexZ << std::endl;
        if( fabs( eInfo.diphotonVertexZ ) > 9998 ) {counter++; continue;} // Require diphoton vertex to return a good result, or skip event for BOTH plots!


        ptden0->Fill( eInfo.higgsPt );
        ptdenLeg->Fill( eInfo.higgsPt );

        if( fabs( eInfo.genVertexZ - eInfo.zerothVertexZ ) < 1. ) { // only fill if reco vertex is within 1cm of true
            ptnum0->Fill( eInfo.higgsPt );
        }
        if( fabs( eInfo.genVertexZ - eInfo.diphotonVertexZ ) < 1. ) { // only fill if reco vertex is within 1cm of true
            ptnumLeg->Fill( eInfo.higgsPt );
        }
    }

    TGraphAsymmErrors *graph0 = new TGraphAsymmErrors( ptnum0, ptden0 );
    TGraphAsymmErrors *graphLeg = new TGraphAsymmErrors( ptnumLeg, ptdenLeg );
    graphLeg->SetMarkerColor( kRed );
    graphLeg->SetLineColor( kRed );
    graph0->Draw( "AP" );
    graphLeg->Draw( "P" );

    c1->SaveAs( "vertexEfficiency0.pdf" );

    std:: cout << "Statistics: " << eventTree->GetEntries() << " events." << std::endl;
    std:: cout << "skipped " << counter << " events." << std::endl;
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

