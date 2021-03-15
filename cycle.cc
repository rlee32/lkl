#include "cycle.hh"


primitives::sequence_t Cycle::normalized_sequence(primitives::point_id_t point, primitives::sequence_t start) const {
    if (point == start) {
        return 0;
    }
    return sequence_[point] > sequence_[start]
        ? sequence_[point] - sequence_[start]
        : (sequence_[point] + sequence_.size()) - sequence_[start];
}

primitives::point_id_t Cycle::prev(primitives::point_id_t point) const {
    return prev_[point];
}

primitives::point_id_t Cycle::next(primitives::point_id_t point) const {
    return next_[point];
}
