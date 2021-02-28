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
    MultiSegment(const Segment &segment, primitives::point_id_t start_point)
        : segments_({segment})
        , start_point_(start_point)
        , end_point_(segment.other(start_point)) {}

    // Splits this segment given a point and edge direction.
    // In order to maintain a single segment, the point can only split one way.
    point_id_t split(const Cycle &cycle, point_id_t split_point);

private:
    vector<Segment> segments_;
    const point_id_t start_point_{constants::INVALID_POINT};
    point_id_t end_point_{constants::INVALID_POINT};
};

} // namespace feasibility

