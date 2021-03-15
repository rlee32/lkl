#include "hill_climber.hh"
#include "multi_box.hh"
#include "cycle.hh"

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

        Cycle cycle(tour_->sequence(), tour_->prev(), tour_->next());
        multi_segments_.emplace_back(cycle, swap_end, i);
        assert(i == multi_segments_.back().head());
        try_nearby_points();
        if (stop_) {
            return;
        }
        multi_segments_.pop_back();
        kmove_.removes.pop_back();
        m_kmargin.pop_increase();
    }
    kmove_.starts.pop_back();
}

void HillClimber::try_nearby_points() {
    const auto start = kmove_.starts.back();
    assert(start == multi_segments_.back().head());
    for (auto p : search_neighborhood(start)) {
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
                if (p == swap_end_) {
                    // found improving move.
                    stop_ = true;
                    return;
                }
                delete_next_edge();
                if (stop_) {
                    return;
                }
                kmove_.ends.pop_back();
            }
            m_kmargin.pop_decrease();
        }
    }
}

void HillClimber::delete_next_edge() {
    // get point in current tour from which an edge will be deleted.
    const auto i = kmove_.ends.back(); // split point.

    // get other end point of deleted edge. only 1 of the 2 adjacent edges can be deleted.
    multi_segments_.push_back(multi_segments_.back());
    Cycle cycle(tour_->sequence(), tour_->prev(), tour_->next());
    auto start = multi_segments_.back().split(cycle, i).value();
    std::cout << i << ", " << start << "; " << prev(i) << ", " << next(i) << std::endl;
    std::cout << prev(prev(i)) << std::endl;
    std::cout << next(next(i)) << std::endl;
    std::cout << prev(prev(prev(i))) << std::endl;
    std::cout << next(next(next(i))) << std::endl;

    assert(prev(i) == start or next(i) == start);
    if (kmove_.startable(start)) {
        const auto &edge = prev(i) == start ? start : i;
        if (kmove_.removable(edge)) {
            kmove_.starts.push_back(start);
            kmove_.removes.push_back(edge);
            m_kmargin.increase(length(edge));
            try_nearby_points();
            if (stop_) {
                return;
            }
            kmove_.starts.pop_back();
            kmove_.removes.pop_back();
            m_kmargin.pop_increase();
        }
    }

    assert(not stop_);

    multi_segments_.pop_back();
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

