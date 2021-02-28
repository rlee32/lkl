#pragma once

#include <optional>
#include <vector>

#include "tour.hh"
#include "primitives.hh"
#include "point_set.hh"
#include "kmove.hh"
#include "kmargin.hh"
#include "cycle_check.hh"

class HillClimber
{
 public:
    HillClimber(const PointSet& point_set) : m_point_set(point_set) {}

    std::optional<KMove> find_best(const Tour &tour, size_t kmax);

    void changed(const KMove &kmove);

    int nonfeasible_checks_{0};

private:
    size_t m_kmax {3};

    KMove m_kmove;
    primitives::point_id_t m_swap_end {constants::invalid_point};
    bool m_stop {false};

    KMargin m_kmargin;

    void search(primitives::point_id_t i);
    void delete_both_edges();
    void try_nearby_points();

    void reset_search();

    primitives::length_t length(primitives::point_id_t edge_start) const;
    primitives::length_t length(primitives::point_id_t a, primitives::point_id_t b) const;

    void final_move_check();
    bool final_new_edge() const;

    std::vector<primitives::point_id_t> search_neighborhood(primitives::point_id_t p);

    const Tour *m_tour{nullptr};
    const PointSet &m_point_set;

    primitives::sequence_t size() const {
        return m_tour->size();
    }
    primitives::point_id_t next(primitives::point_id_t i) const {
        return m_tour->next(i);
    }
    primitives::point_id_t prev(primitives::point_id_t i) const {
        return m_tour->prev(i);
    }

    std::vector<std::optional<Box>> search_extents_;
};

