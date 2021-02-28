#pragma once

#include "primitives.hh"

#include <cmath>
#include <vector>

class LengthCalculator
{
public:
    LengthCalculator() = default;
    LengthCalculator(const std::vector<primitives::space_t>& x
        , const std::vector<primitives::space_t>& y)
        : m_x(&x), m_y(&y) {}

    primitives::length_t operator()(primitives::point_id_t a, primitives::point_id_t b) const;

    const auto& x() const { return *m_x; }
    const auto& y() const { return *m_y; }

    auto x(primitives::point_id_t i) const { return m_x->operator[](i); }
    auto y(primitives::point_id_t i) const { return m_y->operator[](i); }

private:
    const std::vector<primitives::space_t>* m_x {nullptr};
    const std::vector<primitives::space_t>* m_y {nullptr};

};

inline primitives::length_t LengthCalculator::operator()(
    primitives::point_id_t a, primitives::point_id_t b) const
{
    const auto& x = *m_x;
    auto dx = x[a] - x[b];
    const auto& y = *m_y;
    auto dy = y[a] - y[b];
    auto exact = std::sqrt(dx * dx + dy * dy);
    return exact + 0.5; // return type cast.
}
