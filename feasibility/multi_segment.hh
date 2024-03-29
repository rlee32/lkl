#pragma once

#include <constants.hh>
#include <primitives.hh>
#include <vector>
#include <cycle.hh>
#include <unordered_map>
#include <optional>

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
    // Sometimes a split cannot be performed due to current progress, e.g. deleting an edge that has just been made,
    // in which case nullopt is returned.
    std::optional<point_id_t> split(const Cycle &cycle, point_id_t split_point);

    auto head() const { return head_; }

private:
    // First segment contains start, which is fixed and does not change.
    // Last segment contains current end, which is dynamic.
    // Segments are in order, though note that the segment endpoints are not necessarily in multi-segment order.
    vector<Segment> segments_;
    // Fixed endpoint.
    point_id_t fixed_tail_{constants::INVALID_POINT};
    // Dynamic point that changes with each split.
    point_id_t head_{constants::INVALID_POINT};


    // Reverses segments in [start_index, end).
    void reverse(size_t start_index);

    // Gets the point that is toward head at the given segment index.
    primitives::point_id_t get_end(size_t segment_index);

    std::unordered_map<primitives::point_id_t, primitives::point_id_t> new_edges_;
};

} // namespace feasibility

