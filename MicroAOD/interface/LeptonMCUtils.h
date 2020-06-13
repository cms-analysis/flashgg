#ifndef _LeptonMCUtils_h_
#define _LeptonMCUtils_h_

#include "DataFormats/Math/interface/deltaR.h"
#include "flashgg/DataFormats/interface/Photon.h" //FIXME


namespace flashgg {

    class LeptonMCUtils
    {

    public:
        template<class GenT, class GenCollT> static float isoSum( const GenT &genp, const GenCollT &coll, float dRMax )
        {
            float sum = 0.;
            for( auto &part : coll ) {
                if( reco::deltaR( genp, part ) >= dRMax || part.p4() == genp.p4() ) { continue; }
                sum += part.et();
            }
            return sum;
        }

        template<class GenT, class GenCollT> static float isoSumAfterDressing( const GenT &genp, const GenCollT &coll, float dRMax, float dRMaxDressing )
        {
            float sum = 0.;
            for( auto &part : coll ) {
                if( reco::deltaR( genp, part ) >= dRMax || part.p4() == genp.p4() || ( reco::deltaR( genp, part ) <= dRMaxDressing && part.p4() != genp.p4() && part.pdgId() == 22 && part.isPromptFinalState() )  ) { continue; }
                sum += part.et();
            }
            return sum;
        }

        template<class GenT, class GenCollT> static reco::Particle::LorentzVector dressedP4( const GenT &genp, const GenCollT &coll, float dRMax )
        {
            reco::Particle::LorentzVector dressedP4 = genp.p4();
            for( auto &part : coll ) {
                if( reco::deltaR( genp, part ) <= dRMax && part.p4() != genp.p4() && part.pdgId() == 22 && part.isPromptFinalState() ) {
                    dressedP4 = dressedP4 + part.p4();
                }
            }
            return dressedP4;
        }



//        static void determineMatchType( flashgg::Lepton &pho, std::vector<int> promptMothers = std::vector<int>(),
//                                        flashgg::Lepton::mcMatch_t defaultType = flashgg::Lepton::kUnkown ); //FIXME
//
//        template<class GenT> static flashgg::Lepton::mcMatch_t determineMatchType(
//            const GenT &gen,
//            std::vector<int> promptMothers = std::vector<int>(),
//            flashgg::Lepton::mcMatch_t defaultType = flashgg::Lepton::kUnkown )
//        {
//            auto matchType = defaultType;
//            //            std::cout<< " Gen " << gen.pdgId() << std::endl;
//
//
//            //for( size_t imom = 0; imom < gen.numberOfMothers(); ++imom ) {
//            //  int mstat = gen.mother( imom )->status();
//            //int mpdgId = abs( gen.mother( imom )->pdgId() );
//            // std::cout << " mstat " << mstat << " mpdgId " << mpdgId << std::endl;
//
//            //     if( ( ( mpdgId > 0 && mpdgId <= 25 ) && ( mstat == 3 || mstat == 23 || mstat == 22 ) )
//            //       || ( find( promptMothers.begin(), promptMothers.end(), mpdgId ) != promptMothers.end() ) ) {
//            if( gen.pdgId() == 22 &&  gen.isPromptFinalState() ) {
//                matchType = flashgg::Lepton::kPrompt;
//                // std::cout << " type prompt "  << std::endl;
//            } else {
//                //if( mstat == 2 && mpdgId != 22 ) {
//                matchType = flashgg::Lepton::kFake;
//                //std::cout << " type fake "  << std::endl;
//            }
//            // }
//            return matchType;
//        }


    };


}


#endif // _LeptonMCUtils_h_
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

