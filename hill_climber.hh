#pragma once

#include <optional>
#include <vector>

#include "tour.hh"
#include "primitives.hh"
#include "point_set.hh"
#include "kmove.hh"
#include "kmargin.hh"
#include "cycle_check.hh"
#include "feasibility/multi_segment.hh"

class HillClimber
{
 public:
    HillClimber(const PointSet& point_set) : m_point_set(point_set) {}

    std::optional<KMove> find_best(const Tour &tour, size_t kmax);

    int nonfeasible_checks_{0};

private:
    size_t kmax_{3};

    KMove kmove_; // keeps track of current move.
    // the final point of the current move (known when move construction starts).
    primitives::point_id_t swap_end_{constants::INVALID_POINT};
    bool stop_{false}; // when an improving move is found, this is set to true.

    // Keeps track of gain margin (e.g. search radius).
    KMargin m_kmargin;
    // Tells us if a deletion will result in a disconnecting sequential move.
    feasibility::MultiSegment multi_segment_;


    // Starts move from one point.
    void start_search(primitives::point_id_t i);
    void delete_both_edges();
    void try_nearby_points();

    void reset_search();

    primitives::length_t length(primitives::point_id_t edge_start) const;
    primitives::length_t length(primitives::point_id_t a, primitives::point_id_t b) const;

    void final_move_check();
    bool final_new_edge() const;

    std::vector<primitives::point_id_t> search_neighborhood(primitives::point_id_t p);

    const Tour *tour_{nullptr};
    const PointSet &m_point_set;

    primitives::sequence_t size() const {
        return tour_->size();
    }
    primitives::point_id_t next(primitives::point_id_t i) const {
        return tour_->next(i);
    }
    primitives::point_id_t prev(primitives::point_id_t i) const {
        return tour_->prev(i);
    }
};

