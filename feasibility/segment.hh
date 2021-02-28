#pragma once

#include <constants.hh>
#include <primitives.hh>
#include <vector>
#include <cycle.hh>

namespace feasibility {

// Represents one contiguous portion of the tour, storing only the endpoints.
// Segment always flows in direction of tour, from first to second point.
// Given two endpoints, first and second points should be ordered properly to obtain 1 of the 2 possiblities.
// Useful in keeping track of how a tour is split from successive edge removals.
class Segment {
    using point_id_t = primitives::point_id_t;
    using sequence_t = primitives::sequence_t;
    using Direction = primitives::Direction;
    template<typename T>
    using vector = std::vector<T>;

public:
    Segment(point_id_t first, point_id_t second) : first_(first), second_(second) {}

    // Splits this segment given a 2 split points, which should be an existing edge in the tour (e.g. sequence 1 apart).
    // The split points can be in any order.
    Segment split(const Cycle &cycle, point_id_t split_point_1, point_id_t split_point_2);

    // Returns true if this segment contains the point.
    bool has_point(const Cycle &cycle, point_id_t point) const;

    // Returns true if one of the endpoints match the input point.
    bool has_point(point_id_t point) const;

    // Returns the point not equal to input point.
    primitives::point_id_t other(point_id_t point) const;

    void reverse();

private:
    point_id_t first_{constants::INVALID_POINT};
    point_id_t second_{constants::INVALID_POINT};
};

} // namespace feasibility

