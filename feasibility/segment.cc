#include "segment.hh"

#include <cassert>
#include <utility>
#include <algorithm>
#include <iostream>

namespace feasibility {

namespace {

primitives::sequence_t seq_diff(primitives::sequence_t a, primitives::sequence_t b) {
    return (a > b) ? a - b : b - a;
}

} // namespace

Segment Segment::split(const Cycle &cycle, point_id_t split_point, point_id_t split_toward) {
    assert(split_point != split_toward); // check that we are not deleting a new edge.
    assert(has_point(cycle, split_point)); // check that split_point is in this segment.
    assert(first_ == split_toward or second_ == split_toward);

    // prev or next to split_point will be new head.
    auto prev = cycle.prev(split_point);
    auto next = cycle.next(split_point);
    const auto prev_seq = cycle.normalized_sequence(prev, first_);
    const auto next_seq = cycle.normalized_sequence(next, first_);

    // check sequence numbers.
    if (seq_diff(prev_seq, next_seq) != 2) {
        // This should only happen if split_point is on first_.
        assert(prev_seq == 1 or next_seq == 1);
        assert(prev_seq == cycle.size() - 1 or next_seq == cycle.size() - 1);
        assert(split_point == first_);
        assert(split_toward != first_);
    } else {
        assert(prev_seq < next_seq);
    }

    // split_toward must be in the new segment (which gets appended to segment list).
    auto original_first = first_;
    auto original_second = second_;
    if (prev_seq < next_seq) {
        if (split_toward == first_) {
            first_ = split_point;
            return Segment(original_first, prev);
        } else {
            second_ = split_point;
            return Segment(next, original_second);
        }
    } else {
        // This should only happen if split_point is on first_.
        second_ = split_point;
        return Segment(next, original_second);
    }
}

void Segment::reverse() {
    std::swap(first_, second_);
}

bool Segment::has_point(const Cycle &cycle, point_id_t point) const {
    auto seq = cycle.normalized_sequence(point, first_);
    auto mag = seqmag(cycle);
    return seq <= mag;
}

bool Segment::has_point(point_id_t point) const {
    return point == first_ or point == second_;
}

bool Segment::is_degenerate() const {
    return first_ == second_;
}

primitives::point_id_t Segment::other(point_id_t point) const {
    assert(has_point(point));
    return first_ == point ? second_ : first_;
}

primitives::sequence_t Segment::seqmag(const Cycle &cycle) const {
    return cycle.normalized_sequence(second_, first_);
}

void Segment::print(const Cycle &cycle) const {
    std::cout << "first, second, size: "
        << first_ << ", "
        << second_ << ", "
        << seqmag(cycle) << std::endl;
}


} // namespace feasibility

