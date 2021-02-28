#pragma once

#include "box.hh"
#include "primitives.hh"

#include <vector>

class BoxMaker
{
public:
    BoxMaker() = default;
    BoxMaker(const std::vector<primitives::space_t>& x
        , const std::vector<primitives::space_t>& y)
        : m_x(&x), m_y(&y) {}

    auto operator()(primitives::point_id_t i, primitives::length_t radius) const
    {
        Box box;
        const auto& x = *m_x;
        box.xmin = x[i] - radius;
        box.xmax = x[i] + radius;
        const auto& y = *m_y;
        box.ymin = y[i] - radius;
        box.ymax = y[i] + radius;
        return box;
    }

private:
    const std::vector<primitives::space_t>* m_x {nullptr};
    const std::vector<primitives::space_t>* m_y {nullptr};
};

