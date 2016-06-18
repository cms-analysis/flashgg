{
    // Set up FW Lite for automatic loading of CMS libraries
    // and data formats.   As you may have other user-defined setup
    // in your rootlogon.C, the CMS setup is executed only if the CMS
    // environment is set up.
    //

    using namespace std;

    TString cmsswbase = getenv( "CMSSW_BASE" );
    if( cmsswbase.Length() > 0 )
    {
        //
        // The CMSSW environment is defined (this is true even for FW Lite)
        // so set up the rest.
        //
        cout << "Loading FW Lite setup." << endl;
        gSystem->Load( "libFWCoreFWLite.so" );
        AutoLibraryLoader::enable();
        gSystem->Load( "libDataFormatsFWLite.so" );
        gSystem->Load( "libDataFormatsPatCandidates.so" );
        gSystem->Load( "libflashggMicroAODFormats.so" );

    }

    //TFile f("/afs/cern.ch/user/c/carrillo/eoscarrillo/low_mass_hlt/V8/outputfilelist125GeVgenparticles.root");
    //TFile f( "/afs/cern.ch/user/c/carrillo/eoscarrillo/low_mass_hlt/V6/outputfilelist125GeV.root" );
    TFile f( "/afs/cern.ch/user/c/carrillo/flashgg/CMSSW_7_4_0_pre9/src/flashgg/MicroAOD/test/myMicroAODOutputFile.root" );
    TTree *Events = f.Get( "Events" );
    //Events->Print();
    Events->SetScanField( 0 );
    //Events->Scan("flashggDiPhotonCandidates_flashggDiPhotons__FLASHggTEST.obj.pt_:flashggPhotons_flashggPhotons__FLASHggTEST.obj.zernike42:flashggPhotons_flashggPhotons__FLASHggTEST.obj.hcalTowersBehindClusters[0]");


#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "flashgg/DataFormats/interface/DiphotonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#endif

    gStyle->SetOptStat( 0 );

    TH1F *cutflow;
    TH1F *ptLead[9  ];
    TH1F *ptTrail[9  ];
    TH1F *ptLeadNorm[9  ];
    TH1F *ptTrailNorm[9  ];
    TH1F *massDiphotonhisto[9  ];
    TH1F *massHiggshisto[9  ];
    TH1F *higgsEta[9  ];
    TH1F *higgsPhi[9  ];
    TH1F *higgsP[9  ];
    TH1F *higgsPt[9  ];
    TH2F *phi1phi2[9  ];
    TH2F *eta1eta2[9  ];
    TH2F *pt1pt2[9  ];
    TH2F *pt1pt2Norm[9  ];
    TH2F *pt1pt2Zoom[9  ];
    TH2F *wide_pt1pt2[9  ];

    TH1F *diphoton_LeadR9[9  ];
    TH1F *diphoton_SubLeadR9[9  ];

    TH1F *diphoton_LeadHoE[9  ];
    TH1F *diphoton_SubLeadHoE[9  ];

    TH1F *diphoton_LeadTrkIso03[9  ];
    TH1F *diphoton_SubLeadTrkIso03[9  ];

    TH1F *diphoton_LeadHcalIso03[9  ];
    TH1F *diphoton_SubLeadHcalIso03[9  ];

    TH1F *diphoton_Leadpfiso02[9  ];
    TH1F *diphoton_SubLeadpfiso02[9  ];

    const Double_t bins[19] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 90, 100, 110};
    bool cut[9  ];
    //string arrayHLTpathsNames[442  ];
    int cut_index;
    //beginJobs
    theFileOut = new TFile( "output_hlt.root", "RECREATE" );
    TString name;

    cutflow = new TH1F( "cutflow", "cutflow", 9  , -0.5, 8.5 );

    for( cut_index = 0; cut_index < 9  ; cut_index++ ) //Loop over the different histograms
    {
        name = Form( "ptLead_%d", cut_index );
        ptLead[cut_index]            = new TH1F( name, name, 100, 0., 120. );
        name = Form( "ptTrail_%d", cut_index );
        ptTrail[cut_index]           = new TH1F( name, name, 100, 0., 120. );
        name = Form( "ptLeadNorm_%d", cut_index );
        ptLeadNorm[cut_index]        = new TH1F( name, name, 100, 0., 1. );
        name = Form( "ptTrailNorm_%d", cut_index );
        ptTrailNorm[cut_index]       = new TH1F( name, name, 100, 0., 1. );
        name = Form( "massDiphoton_%d", cut_index );
        massDiphotonhisto[cut_index] = new TH1F( name, name, 100, 60., 180. );
        name = Form( "massHiggs_%d", cut_index );
        massHiggshisto[cut_index]    = new TH1F( name, name, 100, 60., 180. );
        name = Form( "higgsEta_%d", cut_index );
        higgsEta[cut_index]          = new TH1F( name, name, 100, -5, 5 );
        name = Form( "higgsPhi_%d", cut_index );
        higgsPhi[cut_index]          = new TH1F( name, name, 180, -3.15, 3.15 );
        name = Form( "higgsP_%d", cut_index );
        higgsP[cut_index]            = new TH1F( name, name, 500, 0, 1200 );
        name = Form( "higgsPt_%d", cut_index );
        higgsPt[cut_index]           = new TH1F( name, name, 500, 0, 1200 );
        name = Form( "phi1phi2_%d", cut_index );
        phi1phi2[cut_index]          = new TH2F( name, name, 180, -3.15, 3.15, 180, -3.15, 3.15 );
        name = Form( "eta1eta2_%d", cut_index );
        eta1eta2[cut_index]          = new TH2F( name, name, 100, -5, 5, 100, -5, 5 );
        name = Form( "pt1pt2_%d", cut_index );
        pt1pt2[cut_index]            = new TH2F( name, name, 100, 0., 110., 100, 0., 110. );
        name = Form( "pt1pt2Norm_%d", cut_index );
        pt1pt2Norm[cut_index]        = new TH2F( name, name, 100, 0., 1., 100, 0., 1. );
        name = Form( "pt1pt2Zoom_%d", cut_index );
        pt1pt2Zoom[cut_index]        = new TH2F( name, name, 20, 0.3, 0.5, 20, 0.2, 0.4 );
        name = Form( "wide_pt1pt2_%d", cut_index );
        wide_pt1pt2[cut_index]       = new TH2F( name, name, 19 - 1, bins, 19 - 1, bins );

        name = Form( "diphoton_LeadR9_%d", cut_index );
        diphoton_LeadR9[cut_index]       = new TH1F( name, name, 100, 0., 1. );
        name = Form( "diphoton_SubLeadR9_%d", cut_index );
        diphoton_SubLeadR9[cut_index]       = new TH1F( name, name, 100, 0., 1. );

        name = Form( "diphoton_LeadHoE_%d", cut_index );
        diphoton_LeadHoE[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );
        name = Form( "diphoton_SubLeadHoE_%d", cut_index );
        diphoton_SubLeadHoE[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );

        name = Form( "diphoton_LeadTrkIso03_%d", cut_index );
        diphoton_LeadTrkIso03[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );
        name = Form( "diphoton_SubLeadTrkIso03_%d", cut_index );
        diphoton_SubLeadTrkIso03[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );

        name = Form( "diphoton_LeadHcalIso03_%d", cut_index );
        diphoton_LeadHcalIso03[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );
        name = Form( "diphoton_SubLeadHcalIso03_%d", cut_index );
        diphoton_SubLeadHcalIso03[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );

        name = Form( "diphoton_Leadpfiso02_%d", cut_index );
        diphoton_Leadpfiso02[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );
        name = Form( "diphoton_SubLeadpfiso02_%d", cut_index );
        diphoton_SubLeadpfiso02[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );
    }

    fwlite::Event ev( &f );

    unsigned int count = 0;

    for( ev.toBegin(); ! ev.atEnd() && count <= 100000; ++ev )
    {
        count++;
        cout << count << endl;

        fwlite::Handle<vector<flashgg::Photon> > objs_pho;
        fwlite::Handle<vector<flashgg::DiPhotonCandidate> > objs_dipho;
        fwlite::Handle<vector<reco::Vertex> > objs_vertex;
        fwlite::Handle<vector<reco::GenParticle> > objs_genpart;
        fwlite::Handle<edm::TriggerResults> hTriggerResults;

        hTriggerResults.getByLabel( ev, "TriggerResults", "", "HLTV9" );
        edm::TriggerNames const  &triggerNames = ev.triggerNames( *hTriggerResults );
        vector<std::string> const &names = triggerNames.triggerNames();
        if( count == 1 ) {
            for( unsigned index = 0; index < triggerNames.size(); ++index ) {
                cout << index << " " << triggerNames.triggerName( index ) << " " << hTriggerResults->accept( index ) << endl;
            }
            std::cout << "size = " << triggerNames.size() << std::endl;
        }
        objs_pho.getByLabel( ev, "flashggPhotons" );
        objs_dipho.getByLabel( ev, "flashggDiPhotons" );
        objs_vertex.getByLabel( ev, "offlineSlimmedPrimaryVertices" );
        //objs_genpart.getByLabel(ev,"flashggPrunedGenParticles");
        objs_genpart.getByLabel( ev, "genParticles" );

        vector<reco::GenParticle> const &genpart = *objs_genpart;

        float z_higgs = 0;
        for( int i = 0; i < objs_genpart.ptr()->size(); i++ ) {
            if( genpart[i].pdgId() == 25 ) { z_higgs = genpart[i].vz(); }
        }

        int sel_i = 1000;
        vector<flashgg::DiPhotonCandidate> const &dipho = *objs_dipho;
        float min_z = 999;
        for( int k = 0; k < objs_dipho.ptr()->size(); k++ ) {
            if( fabs( dipho[k].getVertex()->position().z() - z_higgs ) < min_z ) { sel_i = k; }
        }

        if( sel_i == 1000 ) {
            cout << "No Diphoton reconstructed in this event" << endl;
            continue;
        }

        bool preselection =
            !( ( fabs( dipho[sel_i].leadingPhoton()->eta() ) > 1.4442 && fabs( dipho[sel_i].leadingPhoton()->eta() ) < 1.5660 )
               || ( fabs( dipho[sel_i].subLeadingPhoton()->eta() ) > 1.4442 && fabs( dipho[sel_i].subLeadingPhoton()->eta() ) < 1.5660 ) )
            && dipho[sel_i].leadingPhoton()->pt() > dipho[sel_i].mass() / 3
            && dipho[sel_i].subLeadingPhoton()->pt() > dipho[sel_i].mass() / 4;

        bool hlt = ( hTriggerResults->accept( 210 ) || hTriggerResults->accept( 210 ) );

        cut[0] = true; //all minitree level plots for reference, no cuts
        cut[1] = preselection && ( dipho[sel_i].leadingPhoton()->isEB() && dipho[sel_i].leadingPhoton()->isEB()
                                   && dipho[sel_i].leadingPhoton()->r9() > 0.94 && dipho[sel_i].subLeadingPhoton()->r9() > 0.94 );
        cut[2] = cut[1] && hlt;
        cut[3] = preselection && ( dipho[sel_i].leadingPhoton()->isEB() && dipho[sel_i].leadingPhoton()->isEB() &&
                                   ( dipho[sel_i].leadingPhoton()->r9() < 0.94 || dipho[sel_i].subLeadingPhoton()->r9() < 0.94 ) );
        cut[4] = cut[3] && hlt;
        cut[5] = preselection && ( dipho[sel_i].leadingPhoton()->isEE() || dipho[sel_i].leadingPhoton()->isEE() &&
                                   ( dipho[sel_i].leadingPhoton()->r9() > 0.94 && dipho[sel_i].subLeadingPhoton()->r9() > 0.94 ) );
        cut[6] = cut[5] && hlt;
        cut[7] = preselection && ( dipho[sel_i].leadingPhoton()->isEE() || dipho[sel_i].leadingPhoton()->isEE() &&
                                   ( dipho[sel_i].leadingPhoton()->r9() < 0.94 || dipho[sel_i].subLeadingPhoton()->r9() < .94 ) );
        cut[8] = cut[7] && hlt;

        for( cut_index = 0; cut_index < 9  ; cut_index++ ) { //Loop over the different histograms
            if( cut[cut_index] ) { //all hitograms below will be filled up if the boolean is true.
                cutflow->Fill( cut_index );
                phi1phi2[cut_index]->Fill( dipho[sel_i].leadingPhoton()->phi(), dipho[sel_i].subLeadingPhoton()->phi() );
                eta1eta2[cut_index]->Fill( dipho[sel_i].leadingPhoton()->eta(), dipho[sel_i].subLeadingPhoton()->eta() );
                pt1pt2[cut_index]->Fill( dipho[sel_i].leadingPhoton()->pt(), dipho[sel_i].subLeadingPhoton()->pt() );
                pt1pt2Norm[cut_index]->Fill( dipho[sel_i].leadingPhoton()->pt() / dipho[sel_i].mass(), dipho[sel_i].subLeadingPhoton()->pt() / dipho[sel_i].mass() );
                pt1pt2Zoom[cut_index]->Fill( dipho[sel_i].leadingPhoton()->pt() / dipho[sel_i].mass(), dipho[sel_i].subLeadingPhoton()->pt() / dipho[sel_i].mass() );
                wide_pt1pt2[cut_index]->Fill( dipho[sel_i].leadingPhoton()->pt(), dipho[sel_i].subLeadingPhoton()->pt() );
                higgsEta[cut_index]->Fill( dipho[sel_i].eta() );
                higgsPhi[cut_index]->Fill( dipho[sel_i].phi() );
                higgsP[cut_index]->Fill( dipho[sel_i].p() );
                higgsPt[cut_index]->Fill( dipho[sel_i].pt() );
                ptLead[cut_index]->Fill( dipho[sel_i].leadingPhoton()->pt() );
                ptTrail[cut_index]->Fill( dipho[sel_i].subLeadingPhoton()->pt() );
                ptLeadNorm[cut_index]->Fill( dipho[sel_i].leadingPhoton()->pt() / dipho[sel_i].mass() );
                ptTrailNorm[cut_index]->Fill( dipho[sel_i].subLeadingPhoton()->pt() / dipho[sel_i].mass() );
                massHiggshisto[cut_index]->Fill( dipho[sel_i].mass() );
                massDiphotonhisto[cut_index]->Fill( dipho[sel_i].mass() );

                diphoton_LeadR9[cut_index]->Fill( dipho[sel_i].leadingPhoton()->r9() );
                diphoton_SubLeadR9[cut_index]->Fill( dipho[sel_i].subLeadingPhoton()->r9() );

                //diphoton_LeadTrkIso03[cut_index]->Fill(dipho[sel_i].leadingPhoton()->TrkIso03);
                //diphotonsubLeadTrkIso03[cut_index]->Fill(dipho[sel_i].subLeadingPhoton()->SubLeadTrkIso03);

                //diphoton_LeadHcalIso03[cut_index]->Fill(dipho[sel_i].leadingPhoton()->HcalIso03);
                //diphotonsubLeadHcalIso03[cut_index]->Fill(dipho[sel_i].subLeadingPhoton()->HcalIso03);

                //diphoton_Leadpfiso02[cut_index]->Fill(dipho[sel_i].leadingPhoton()->pfiso02);
                //diphotonsubLeadpfiso02[cut_index]->Fill(dipho[sel_i].subLeadingPhoton()->pfiso02);
            }
        }
    }

    //endjob
    cout << "beginning endJob" << endl;
    theFileOut->cd();
    cutflow->Write();
    for( cut_index = 0; cut_index < 9  ; cut_index++ ) //Loop over the different histograms
    {
        cout << "index=" << cut_index << endl;
        ptLead[cut_index]->Write();
        ptTrail[cut_index]->Write();
        ptLeadNorm[cut_index]->Write();
        ptTrailNorm[cut_index]->Write();
        massDiphotonhisto[cut_index]->Write();
        massHiggshisto[cut_index]->Write();
        higgsEta[cut_index]->Write();
        higgsPhi[cut_index]->Write();
        higgsP[cut_index]->Write();
        higgsPt[cut_index]->Write();
        phi1phi2[cut_index]->Write();
        eta1eta2[cut_index]->Write();
        pt1pt2[cut_index]->Write();
        pt1pt2Norm[cut_index]->Write();
        pt1pt2Zoom[cut_index]->Write();
        wide_pt1pt2[cut_index]->Write();
        diphoton_LeadR9[cut_index]->Write();
        diphoton_SubLeadR9[cut_index]->Write();
        diphoton_LeadHoE[cut_index]->Write();
        diphoton_SubLeadHoE[cut_index]->Write();
        diphoton_LeadTrkIso03[cut_index]->Write();
        diphoton_SubLeadTrkIso03[cut_index]->Write();
        diphoton_LeadHcalIso03[cut_index]->Write();
        diphoton_SubLeadHcalIso03[cut_index]->Write();
        diphoton_Leadpfiso02[cut_index]->Write();
        diphoton_SubLeadpfiso02[cut_index]->Write();
    }

    theFileOut->Close();
    exit( 0 );
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

