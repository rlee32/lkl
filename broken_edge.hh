#pragma once

#include <constants.hh>
#include <primitives.hh>
#include <iostream>

struct BrokenEdge {
    // Direction of traversal is first to second.
    primitives::point_id_t first {constants::invalid_point};
    primitives::point_id_t second {constants::invalid_point};
    primitives::point_id_t sequence {0}; // relative order of this edge to other edges.
};

inline std::ostream& operator<<(std::ostream& o, const BrokenEdge& edge) {
    return o << "sequence, first, second: "
        << edge.sequence
        << "\t" << edge.first
        << "\t" << edge.second;
}
