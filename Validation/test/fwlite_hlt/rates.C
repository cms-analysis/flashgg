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

    //  TFile f("/afs/cern.ch/user/c/carrillo/flashgg/CMSSW_7_0_7_patch1/src/flashgg/MicroAODProducers/test/myOutputFile.root");
    //  TFile f("/afs/cern.ch/user/c/carrillo/my_production_microAOD_hlt/output.root");
    //TFile f("/afs/cern.ch/user/c/carrillo/eoscarrillo/low_mass_hlt/output125.root");
    TFile f( "/afs/cern.ch/user/c/carrillo/eoscarrillo/low_mass_hlt/V8/outputfilelist2030.root" );
    //TFile f("/afs/cern.ch/user/c/carrillo/eoscarrillo/low_mass_hlt/V8/outputfilelist80170.root");

    //TFile f("/tmp/carrillo/myOutputFileBig.root");
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
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include <iostream>
#include <fstream>
#endif

    gStyle->SetOptStat( 0 );

    const int N = 13;

    TH1F *studied_bit_histo[N];
    TH1F *allpaths;
    int cut_index;
    theFileOut = new TFile( "rates.root", "RECREATE" );
    TString name;

    allpaths = new TH1F( "allpaths", "allpaths", N, 0, N );
    allpaths2D = new TH2F( "allpaths2D", "allpaths2D", N, 0, N, N, 0, N );

    fwlite::Event ev( &f );

    int iEvent = 0;

    stringstream label;

    for( ev.toBegin(); ! ev.atEnd(); ++ev )
    {
        iEvent++;
        cout << iEvent << endl;
        fwlite::Handle<edm::TriggerResults> hTriggerResults;
        hTriggerResults.getByLabel( ev, "TriggerResults", "", "HLTV8" );
        edm::TriggerNames const  &triggerNames = ev.triggerNames( *hTriggerResults );
        vector<std::string> const &names = triggerNames.triggerNames();
        if( iEvent == 1 ) {
            for( unsigned index = 0; index < triggerNames.size(); ++index ) {
                cout << index << " " << triggerNames.triggerName( index ) << " " << hTriggerResults->accept( index ) << endl;
                label.str( "" );
                label << "bit" << index;
                cout << label.str() << endl;
                studied_bit_histo[index] = new TH1F( ( label.str() ).c_str(), triggerNames.triggerName( index ).c_str(), 5  , -0.5, 4.5 );

                if( index == 0 ) {
                    allpaths->GetXaxis()->SetBinLabel( index + 1, "all_events" );
                    allpaths2D->GetXaxis()->SetBinLabel( index + 1, "all_events" );
                    allpaths2D->GetYaxis()->SetBinLabel( index + 1, "all_events" );
                } else {
                    allpaths->GetXaxis()->SetBinLabel( index + 1, triggerNames.triggerName( index ).c_str() );
                    allpaths2D->GetXaxis()->SetBinLabel( index + 1, triggerNames.triggerName( index ).c_str() );
                    allpaths2D->GetYaxis()->SetBinLabel( index + 1, triggerNames.triggerName( index ).c_str() );
                }
            }
            cout << "size = " << triggerNames.size() << endl;
        }

        allpaths->Fill( 0 );
        unsigned l, n;
        for( l = 1; l < triggerNames.size(); l++ ) {
            if( hTriggerResults->accept( l ) ) {
                allpaths->Fill( l );
                for( n = 1; n < triggerNames.size(); n++ ) {
                    if( hTriggerResults->accept( n ) ) { allpaths2D->Fill( l, n ); }
                }
            }
        }

        unsigned studied_bit = 8;
        bool othersOR = false;
        int pho_bits[2] = {1, 2};

        for( l = 0; l < 2; l++ ) {
            othersOR = othersOR || hTriggerResults->accept( pho_bits[l] );
        }

        for( studied_bit = 0; studied_bit < N; studied_bit++ ) {
            studied_bit_histo[studied_bit]->Fill( 0 );
            if( hTriggerResults->accept( studied_bit ) ) {
                studied_bit_histo[studied_bit]->Fill( 1 );
            }

            if( othersOR ) {
                studied_bit_histo[studied_bit]->Fill( 2 );
            }

            if( othersOR || hTriggerResults->accept( studied_bit ) ) {
                studied_bit_histo[studied_bit]->Fill( 3 );
            }

            if( othersOR && hTriggerResults->accept( studied_bit ) ) {
                studied_bit_histo[studied_bit]->Fill( 4 );
            }
        }
    }

    //endjob
    cout << "beginning endJob" << endl;
    theFileOut->cd();

    for( index = 0; index < triggerNames.size(); ++index )
    {
        studied_bit_histo[index]->SetTitle( triggerNames.triggerName( index ).c_str() );
        studied_bit_histo[index]->GetXaxis()->SetBinLabel( 1, "events" );
        studied_bit_histo[index]->GetXaxis()->SetBinLabel( 2, "studied bit" );
        studied_bit_histo[index]->GetXaxis()->SetBinLabel( 3, "rediscover" );
        studied_bit_histo[index]->GetXaxis()->SetBinLabel( 4, "rediscover || studied bit" );
        studied_bit_histo[index]->GetXaxis()->SetBinLabel( 5, "rediscover && studied bit" );
        studied_bit_histo[index]->SetFillColor( kYellow );
        studied_bit_histo[index]->Write();
    }

    allpaths->SetFillColor( kRed );
    allpaths->Write();
    allpaths2D->Write();

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

