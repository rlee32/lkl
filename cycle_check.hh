#pragma once

#include "broken_edge.hh"
#include "kmove.hh"
#include "tour.hh"
#include "primitives.hh"

#include <algorithm> // sort
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace cycle_check {

// returns true if current swap does not break tour into multiple cycles.
// new edge i: (starts[i], ends[i])
// for p in removes: (p, next(p))
bool feasible(const Tour&
    , const std::vector<primitives::point_id_t>& starts
    , const std::vector<primitives::point_id_t>& ends
    , const std::vector<primitives::point_id_t>& removes);

inline bool feasible(const Tour& tour, const KMove& kmove)
{
    return feasible(tour, kmove.starts, kmove.ends, kmove.removes);
}

bool breaks_cycle(const Tour&, const KMove&);
size_t count_cycles(const Tour&, const KMove&);
size_t count_cycles(const std::vector<BrokenEdge>& deleted_edges
    , const std::unordered_map<primitives::point_id_t, std::vector<primitives::point_id_t>>& new_edges);

void visit_cycle(const std::unordered_map<primitives::point_id_t, size_t>& sequence
    , const std::vector<BrokenEdge>& deleted_edges
    , const std::unordered_map<primitives::point_id_t, std::vector<primitives::point_id_t>>& new_edges
    , std::unordered_set<primitives::point_id_t>& visited);

} // namespace cycle_check

