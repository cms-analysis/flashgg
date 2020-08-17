#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include <iostream>

namespace flashgg {

    void print_track_count( const VertexCandidateMap &theMap, std::string s )
    {
        int nvtx = -1;
        int ntrk = 0;
        edm::Ptr<reco::Vertex> current_vtx;
        for( auto current_pair = theMap.begin() ; current_pair != theMap.end() ; current_pair++ ) {
            if( current_vtx != current_pair->first ) {
                if( nvtx >= 0 ) {
                    std::cout << " " << s << ": Vertex " << nvtx << " had " << ntrk << " candidates" << std::endl;
                }
                nvtx++;
                ntrk = 0;
                current_vtx = current_pair->first;
            }
            std::cout << "   " << s << ": Vertex " << nvtx << " trk " << ntrk << " pt eta charge dz " << current_pair->second->pt() << " " << current_pair->second->eta() <<
                      " " << current_pair->second->charge()
                      << " " << current_pair->second->dz( current_pair->first->position() ) << std::endl;
            ntrk++;
        }
    }
}

