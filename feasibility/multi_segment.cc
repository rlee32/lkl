#include "multi_segment.hh"

namespace feasibility {


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
    return start_point_;
}

} // namespace feasibility


