#include "multi_segment.hh"

#include <cassert>

namespace feasibility {


MultiSegment::MultiSegment(const Cycle &cycle, primitives::point_id_t fixed_tail, primitives::point_id_t head)
    : fixed_tail_(fixed_tail)
    , head_(head) {
    auto head_seq = cycle.normalized_sequence(head_, fixed_tail_);
    if (head_seq > 1) {
        assert(head_seq + 1 == cycle.sequence_.size());
        segments_.emplace_back(fixed_tail_, head_);
    } else {
        assert(head_seq == 1);
        segments_.emplace_back(head_, fixed_tail_);
    }
}

primitives::point_id_t MultiSegment::split(const Cycle &cycle, point_id_t split_point) {
    // Find segment to split.
    size_t i{0};
    for (; i < segments_.size(); ++i) {
        if (segments_[i].has_point(cycle, split_point)) {
            break;
        }
    }
    assert(i < segments_.size());
    // TODO: implement
    return head_;
}

} // namespace feasibility


