#pragma once

#include "hill_climber.hh"
#include "point_set.hh"
#include "primitives.hh"
#include "tour.hh"

#include <iostream>

namespace hill_climb {

inline primitives::length_t hill_climb(const PointSet &point_set, Tour &tour, size_t kmax) {
    HillClimber hill_climber(point_set);
    auto kmove = hill_climber.find_best(tour, kmax);
    int iterations{0};
    while (kmove) {
        tour.swap(*kmove);
        hill_climber.changed(*kmove);
        kmove = hill_climber.find_best(tour, kmax);
        ++iterations;
    }
    const auto length = tour.length();
    std::cout << "tour length after " << iterations << " iterations: " << length << std::endl;
    return length;
}

inline primitives::length_t hill_climb(HillClimber &hill_climber, Tour &tour, size_t kmax) {
    int iterations{0};
    auto kmove = hill_climber.find_best(tour, kmax);
    while (kmove) {
        tour.swap(*kmove);
        hill_climber.changed(*kmove);
        kmove = hill_climber.find_best(tour, kmax);
        ++iterations;
    }
    const auto length = tour.length();
    std::cout << "tour length after " << iterations << " iterations: " << length << std::endl;
    return length;
}

} // namespace hill_climb

