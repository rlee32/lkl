#include "segment.hh"

#include <cassert>
#include <utility>
#include <algorithm>

namespace feasibility {

Segment Segment::split(const Cycle &cycle, point_id_t split_point_1, point_id_t split_point_2) {
    assert(cycle.normalized_sequence(split_point_1, split_point_2) == 1 or cycle.normalized_sequence(split_point_2, split_point_1) == 1);
    assert(has_point(cycle, split_point_1) and has_point(cycle, split_point_2));

    // Get sequence numbers. first_ has sequence number 0.
    const auto second_seq = cycle.normalized_sequence(second_, first_);
    assert(second_seq != 0); // degnerate segments are allowed, but not to split.
    const auto split_seq_1 = cycle.normalized_sequence(split_point_1, first_);
    const auto split_seq_2 = cycle.normalized_sequence(split_point_2, first_);

    // Update second point on this segment, perform split, and return new segment.
    auto original_second = second_;
    if (split_seq_1 < split_seq_2) {
        second_ = split_seq_1;
        return Segment(split_seq_2, original_second);
    } else {
        second_ = split_seq_2;
        return Segment(split_seq_1, original_second);
    }
}

void Segment::reverse() {
    std::swap(first_, second_);
}

bool Segment::has_point(const Cycle &cycle, point_id_t point) const {
    return cycle.normalized_sequence(point, first_) <= cycle.normalized_sequence(second_, first_);
}

bool Segment::has_point(point_id_t point) const {
    return point == first_ or point == second_;
}

primitives::point_id_t Segment::other(point_id_t point) const {
    assert(has_point(point));
    return first_ == point ? second_ : first_;
}

} // namespace feasibility

