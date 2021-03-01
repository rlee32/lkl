#pragma once

#include <primitives.hh>
#include <vector>

// Holds connectivity information about a tour.
struct Cycle {
    using SequenceVector = std::vector<primitives::sequence_t>;
    using PointVector = std::vector<primitives::point_id_t>;

    Cycle(const SequenceVector &sequence) : sequence_(sequence) {}

    // The length of these vectors must be the total number of points.
    const SequenceVector &sequence_; // indexed by point ID, gives the sequence number in the tour.

    // Returns a normalized sequence number, where 'start' is 0.
    primitives::sequence_t normalized_sequence(primitives::point_id_t point, primitives::sequence_t start) const;
};

