#pragma once

#include <constants.hh>
#include <primitives.hh>
#include <vector>
#include <cycle.hh>

#include "segment.hh"

namespace feasibility {

// Represents a tour undergoing sequential k-move as fragments of original tour.
class MultiSegment {
    using point_id_t = primitives::point_id_t;
    using sequence_t = primitives::sequence_t;
    using Direction = primitives::Direction;
    template<typename T>
    using vector = std::vector<T>;

public:
    MultiSegment() = default;
    MultiSegment(const Cycle &cycle, primitives::point_id_t fixed_tail, primitives::point_id_t head);

    // Splits this segment given a point and edge direction.
    // In order to maintain a single segment, the point can only split one way.
    point_id_t split(const Cycle &cycle, point_id_t split_point);

private:
    // First segment contains start, which is fixed and does not change.
    // Last segment contains current end, which is dynamic.
    // Segments are in order, though note that the segment endpoints are not necessarily in multi-segment order.
    vector<Segment> segments_;
    // Fixed endpoint.
    point_id_t fixed_tail_{constants::INVALID_POINT};
    // Dynamic point that changes with each split.
    point_id_t head_{constants::INVALID_POINT};
};

} // namespace feasibility

