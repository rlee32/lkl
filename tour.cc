#include "tour.hh"

Tour::Tour(const point_quadtree::Domain* domain
    , const std::vector<primitives::point_id_t>& initial_tour)
: domain_(domain)
, adjacents_(initial_tour.size(), {constants::INVALID_POINT, constants::INVALID_POINT})
, next_(initial_tour.size(), constants::INVALID_POINT)
, sequence_(initial_tour.size(), constants::INVALID_POINT)
, box_maker_(domain->x(), domain->y())
, length_calculator_(domain->x(), domain->y()) {
    reset_adjacencies(initial_tour);
    update_next();
}

void Tour::swap(const KMove& kmove) {
    apply_kmove(kmove);
    update_next();
}

void Tour::apply_kmove(const KMove &kmove) {
    for (auto p : kmove.removes) {
        break_adjacency(p);
    }
    for (size_t i{0}; i < kmove.current_k(); ++i) {
        create_adjacency(kmove.starts[i], kmove.ends[i]);
    }
}

template <typename T>
void print_short_vec(const std::vector<T>& vec) {
    for(auto v : vec)
    {
        std::cout << "\t" << v;
    }
    std::cout << std::endl;
}

primitives::sequence_t Tour::sequence(primitives::point_id_t i, primitives::point_id_t start) const {
    auto start_sequence {sequence_[start]};
    auto raw_sequence {sequence_[i]};
    if (raw_sequence < start_sequence) {
        raw_sequence += sequence_.size();
    }
    return raw_sequence - start_sequence;
}

Box Tour::search_box(primitives::point_id_t i, primitives::length_t radius) const {
    return box_maker_(i, radius);
}

void Tour::reset_adjacencies(const std::vector<primitives::point_id_t>& initial_tour) {
    auto prev = initial_tour.back();
    for (auto p : initial_tour) {
        create_adjacency(p, prev);
        prev = p;
    }
}

primitives::point_id_t Tour::prev(primitives::point_id_t i) const {
    const auto next {next_[i]};
    if (adjacents_[i][0] == next) {
        return adjacents_[i][1];
    }
    else if (adjacents_[i][1] == next) {
        return adjacents_[i][0];
    }
    else {
        print_first_cycle();
        std::cout << "next, i, adjacents: "<< next
            << ", " << i
            << ", " << adjacents_[i][0]
            << ", " << adjacents_[i][1]
            << std::endl;
        std::cout << __func__ << ": error: could not determine previous point." << std::endl;
        std::abort();
    }
}

primitives::length_t Tour::length() const {
    primitives::length_t sum {0};
    for (primitives::point_id_t i {0}; i < next_.size(); ++i) {
        sum += length(i);
    }
    return sum;
}

primitives::length_t Tour::prev_length(primitives::point_id_t i) const {
    return length_calculator_(i, prev(i));
}

primitives::length_t Tour::length(primitives::point_id_t i) const {
    return length_calculator_(i, next_[i]);
}

primitives::length_t Tour::length(primitives::point_id_t i, primitives::point_id_t j) const {
    return length_calculator_(i, j);
}

void Tour::update_next(const primitives::point_id_t start) {
    primitives::point_id_t current {start};
    next_[current] = adjacents_[current].front();
    primitives::point_id_t sequence {0};
    order_.clear();
    order_.reserve(next_.size());
    do {
        auto prev = current;
        sequence_[current] = sequence++;
        order_.push_back(current);
        current = next_[current];
        next_[current] = get_other(current, prev);
    } while (current != start); // tour cycle condition.
    if (order_.size() != next_.size()) {
        throw std::logic_error("order_ was not build up properly.");
    }
}

primitives::point_id_t Tour::get_other(primitives::point_id_t point, primitives::point_id_t adjacent) const {
    const auto& a = adjacents_[point];
    if (a.front() == adjacent) {
        return a.back();
    } else {
        return a.front();
    }
}

void Tour::create_adjacency(primitives::point_id_t point1, primitives::point_id_t point2) {
    fill_adjacent(point1, point2);
    fill_adjacent(point2, point1);
}

void Tour::fill_adjacent(primitives::point_id_t point, primitives::point_id_t new_adjacent) {
    if (adjacents_[point].front() == constants::INVALID_POINT) {
        adjacents_[point].front() = new_adjacent;
    }
    else if (adjacents_[point].back() == constants::INVALID_POINT) {
        adjacents_[point].back() = new_adjacent;
    } else {
        std::cout << __func__ << ": error: no available slot for new adjacent." << std::endl;
        std::cout << point << " -> " << new_adjacent << std::endl;
        std::abort();
    }
}

void Tour::break_adjacency(primitives::point_id_t i) {
    break_adjacency(i, next_[i]);
}

void Tour::break_adjacency(primitives::point_id_t point1, primitives::point_id_t point2) {
    vacate_adjacent_slot(point1, point2);
    vacate_adjacent_slot(point2, point1);
}

void Tour::vacate_adjacent_slot(primitives::point_id_t point, primitives::point_id_t adjacent) {
    if (adjacents_[point][0] == adjacent) {
        adjacents_[point][0] = constants::INVALID_POINT;
    }
    else if (adjacents_[point][1] == adjacent) {
        adjacents_[point][1] = constants::INVALID_POINT;
    }
}

void Tour::validate() const {
    constexpr primitives::point_id_t start {0};
    primitives::point_id_t current {start};
    size_t visited {0};
    do {
        ++visited;
        if (visited > next_.size()) {
            std::cout << __func__ << ": error: invalid tour." << std::endl;
            std::abort();
        }
        current = next_[current];
    } while(current != start);
    if (visited != next_.size()) {
        throw std::logic_error("invalid tour.");
    }
}

