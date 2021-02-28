#pragma once

// Represents a TSP instance (not any particular tour, though).

#include <vector>

#include "length_calculator.hh"
#include "box_maker.hh"
#include "primitives.hh"
#include "point_quadtree/node.hh"

class PointSet {
 public:
    PointSet(const point_quadtree::Node& root,
        const std::vector<primitives::space_t> &x,
        const std::vector<primitives::space_t> &y)
        : m_root(root), m_box_maker(x, y), size_(x.size()), m_length_calculator(x, y) {}

    primitives::length_t length(primitives::point_id_t a, primitives::point_id_t b) const {
        return m_length_calculator(a, b);
    }

    // Returns points within square (of size 2 * radius) centered at point i.
    inline std::vector<primitives::point_id_t> get_points(primitives::point_id_t i,
        primitives::length_t radius) const {
        return m_root.get_points(i, m_box_maker(i, radius));
    }
    // Returns points within square (of size 2 * radius) centered at point i.
    inline std::vector<primitives::point_id_t> get_points(primitives::point_id_t i, const Box &box) const {
        return m_root.get_points(i, box);
    }

    inline Box get_box(primitives::point_id_t i, primitives::length_t radius) const {
        return m_box_maker(i, radius);
    }

    inline primitives::point_id_t size() const {
        return size_;
    }

 private:
    const point_quadtree::Node& m_root;
    const BoxMaker m_box_maker;
    const primitives::point_id_t size_{0};
    LengthCalculator m_length_calculator;

};

