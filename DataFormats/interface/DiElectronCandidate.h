#ifndef FLASHgg_DiElectronCandidate_h
#define FLASHgg_DiElectronCandidate_h

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"

namespace flashgg {
    class DiElectronCandidate : public reco::CompositeCandidate, public WeightedObject
    {
    public:
        DiElectronCandidate();
        DiElectronCandidate( edm::Ptr<flashgg::Electron>, edm::Ptr<flashgg::Electron> );
        DiElectronCandidate( const flashgg::Electron &, const flashgg::Electron & );
        ~DiElectronCandidate();

        const flashgg::Electron *leadingElectron() const;
        const flashgg::Electron *subleadingElectron() const;

    private:


    };


}


#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
