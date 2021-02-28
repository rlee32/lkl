#pragma once

#include "tour.hh"
#include "primitives.hh"

#include <exception>
#include <iostream>
#include <vector>

namespace check {

void check_next(const std::vector<primitives::point_id_t> &next) {
    constexpr primitives::point_id_t START{0};
    primitives::point_id_t current {START};
    std::vector<primitives::point_id_t> seen(next.size(), false);
    size_t visited{0};
    do {
        ++visited;
        if (seen[current]) {
            throw std::logic_error("invalid tour: point visited more than once.");
        }
        seen[current] = true;
        current = next[current];
    } while(current != START);
    if (visited != next.size()) {
        throw std::logic_error("invalid tour: invalid traversal.");
    }
}

void check_order(const std::vector<primitives::point_id_t> &order) {
    const primitives::sequence_t n = order.size();
    std::vector<primitives::point_id_t> seen(n, false);
    for (const auto &point : order) {
        if (seen[point]) {
            throw std::logic_error("invalid tour: point visited more than once.");
        }
        seen[point] = true;
    }
}

void check_tour(const Tour &tour) {
    check_next(tour.next());
    check_order(tour.order());
    std::cout << "tour validated.\n";
}

} // namespace check

