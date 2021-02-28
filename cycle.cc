#include "cycle.hh"


primitives::sequence_t Cycle::normalized_sequence(primitives::point_id_t point, primitives::sequence_t start) const {
    return sequence_[point] - start + sequence_.size();
}
