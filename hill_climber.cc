#include "hill_climber.hh"
#include "multi_box.hh"
#include "cycle.hh"

void HillClimber::final_move_check() {
    if (cycle_check::feasible(*tour_, kmove_)) {
        stop_ = true;
    } else {
        ++nonfeasible_checks_;
    }
}

bool HillClimber::final_new_edge() const {
    return kmove_.current_k() == kmax_;
}

std::vector<primitives::point_id_t> HillClimber::search_neighborhood(primitives::point_id_t p) {
    const auto search_radius = m_kmargin.total_margin + 1;
    const auto &box = m_point_set.get_box(p, search_radius);
    return m_point_set.get_points(p, box);
}

std::optional<KMove> HillClimber::find_best(const Tour &tour, size_t kmax) {
    tour_ = &tour;
    kmax_ = kmax;
    reset_search();
    for (primitives::point_id_t i{0}; i < size(); ++i) {
        start_search(i);
        if (stop_) {
            return kmove_;
        }
    }
    return std::nullopt;
}

void HillClimber::start_search(primitives::point_id_t i) {
    kmove_.starts.push_back(i);
    const std::array<primitives::point_id_t, 2> back_pair {prev(i), prev(i)};
    const std::array<primitives::point_id_t, 2> front_pair {i, next(i)};
    // Try removing edges on both sides of this point.
    for(auto [edge, swap_end] : {back_pair, front_pair}) {
        kmove_.removes.push_back(edge);
        m_kmargin.increase(length(edge));
        swap_end_ = swap_end;

        Cycle cycle(tour_->sequence());
        multi_segment_ = feasibility::MultiSegment(cycle, swap_end, i);
        try_nearby_points();
        if (stop_) {
            return;
        }
        kmove_.removes.pop_back();
        m_kmargin.pop_increase();
    }
    kmove_.starts.pop_back();
}

void HillClimber::try_nearby_points() {
    const auto start = kmove_.starts.back();
    for (auto p : search_neighborhood(start))
    {
        // check easy exclusion cases.
        const bool old_edge {p == next(start) or p == prev(start)};
        const bool self {p == start};
        const bool backtrack {(not kmove_.ends.empty()) and p == kmove_.ends.back()};
        if (backtrack or self or old_edge) {
            continue;
        }

        // check if worth considering.
        if (m_kmargin.decrease(length(start, p))) {
            if (kmove_.endable(p)) {
                kmove_.ends.push_back(p);
                // check if closing swap.
                if (p == swap_end_) {
                    final_move_check();
                    if (stop_) {
                        return;
                    }
                }
                delete_both_edges();
                if (stop_) {
                    return;
                }
                kmove_.ends.pop_back();
            }
            m_kmargin.pop_decrease();
        }
    }
}

void HillClimber::delete_both_edges() {
    const auto i = kmove_.ends.back();
    const std::array<primitives::point_id_t, 2> back_pair {prev(i), prev(i)};
    const std::array<primitives::point_id_t, 2> front_pair {i, next(i)};
    for(auto [edge, start] : {back_pair, front_pair}) {
        if (not kmove_.removable(edge) or not kmove_.startable(start)) {
            continue;
        }
        kmove_.starts.push_back(start);
        kmove_.removes.push_back(edge);
        m_kmargin.increase(length(edge));
        if (final_new_edge()) {
            if (m_kmargin.decrease(length(start, swap_end_))) {
                kmove_.ends.push_back(swap_end_);
                final_move_check();
                if (stop_) {
                    return;
                }
                kmove_.ends.pop_back();
                m_kmargin.pop_decrease();
            }
        } else {
            try_nearby_points();
            if (stop_) {
                return;
            }
        }
        kmove_.starts.pop_back();
        kmove_.removes.pop_back();
        m_kmargin.pop_increase();
    }
}

void HillClimber::reset_search() {
    kmove_.clear();
    m_kmargin.clear();
    swap_end_ = constants::invalid_point;
    stop_ = false;
}

primitives::length_t HillClimber::length(primitives::point_id_t a, primitives::point_id_t b) const {
    return m_point_set.length(a, b);
}

primitives::length_t HillClimber::length(primitives::point_id_t edge_start) const {
    return m_point_set.length(edge_start, next(edge_start));
}

