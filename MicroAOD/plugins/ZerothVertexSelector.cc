#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"

namespace flashgg {

    class ZerothVertexSelector : public VertexSelectorBase
    {
    public:
        ZerothVertexSelector( const edm::ParameterSet &conf ) :
            VertexSelectorBase( conf ),
            _whichVertex( conf.getParameter<unsigned int>( "whichVertex" ) ) {} // default it to 0

        edm::Ptr<reco::Vertex> select( const edm::Ptr<flashgg::Photon> &, const edm::Ptr<flashgg::Photon> &,
                                       const std::vector<edm::Ptr<reco::Vertex> > &,
                                       const VertexCandidateMap &,
                                       const std::vector<edm::Ptr<reco::Conversion> > &,
                                       const std::vector<edm::Ptr<reco::Conversion> > &,
                                       const math::XYZPoint &,
                                       bool
                                       //  const Parameters_Selector_Type&,
                                       //  const float&
                                     ) override;

        edm::Ptr<reco::Vertex> select( const edm::Ptr<flashgg::Photon> &, const edm::Ptr<pat::Jet> &,
                                      const std::vector<edm::Ptr<reco::Vertex> > &,
                                      const VertexCandidateMap &,
                                      const std::vector<edm::Ptr<reco::Conversion> > &,
                                      const std::vector<edm::Ptr<reco::Conversion> > &,
                                      const math::XYZPoint &,
                                       bool
                                      //  const Parameters_Selector_Type&,                                                                                                                         
                                      //  const float&                                                                                                                                             
                                      ) override;

        void writeInfoFromLastSelectionTo( flashgg::DiPhotonCandidate & ) override;
        void writeInfoFromLastSelectionTo( flashgg::PhotonJetCandidate & ) override;

    private:
        unsigned int _whichVertex; // set this variable to something non-zero to make this stupid selector both stupider and poorly-named
        // Also it would cause crashes if there was only 1 vertex in the collection
    };

    edm::Ptr<reco::Vertex> ZerothVertexSelector::select( const edm::Ptr<flashgg::Photon> &g1,
            const edm::Ptr<flashgg::Photon> &g2,
            const std::vector<edm::Ptr<reco::Vertex> > &vtxs,
            const VertexCandidateMap &vertexCandidateMap,
            const std::vector<edm::Ptr<reco::Conversion> > &convs,
            const std::vector<edm::Ptr<reco::Conversion> > &convsSingleLeg,
            const math::XYZPoint &beamSpot,
            bool
            //						      const Parameters_Selector_Type& param,
            //                                                      const float& beamsig
                                                       )
    {
        return vtxs[_whichVertex];
    }

    edm::Ptr<reco::Vertex> ZerothVertexSelector::select( const edm::Ptr<flashgg::Photon> &g1,
            const edm::Ptr<pat::Jet> &g2,
            const std::vector<edm::Ptr<reco::Vertex> > &vtxs,
            const VertexCandidateMap &vertexCandidateMap,
            const std::vector<edm::Ptr<reco::Conversion> > &convs,
            const std::vector<edm::Ptr<reco::Conversion> > &convsSingleLeg,
            const math::XYZPoint &beamSpot,
            bool
            //						      const Parameters_Selector_Type& param,
            //                                                      const float& beamsig
                                                       )
    {
        return vtxs[_whichVertex];
    }

    void ZerothVertexSelector::writeInfoFromLastSelectionTo( flashgg::DiPhotonCandidate &dipho )
    {
        // No need to store anything if we're just taking the zeroth vertex
    }

    void ZerothVertexSelector::writeInfoFromLastSelectionTo( flashgg::PhotonJetCandidate &phojet )
    {
        // No need to store anything if we're just taking the zeroth vertex
    }

}

DEFINE_EDM_PLUGIN( FlashggVertexSelectorFactory,
                   flashgg::ZerothVertexSelector,
                   "FlashggZerothVertexSelector" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

