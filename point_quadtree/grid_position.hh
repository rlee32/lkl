#pragma once

#include "domain.hh"
#include "primitives.hh"
#include <box.hh>

namespace point_quadtree {

class GridPosition
{
public:
    GridPosition(const Domain& domain) : m_domain(&domain) {}

    void descend(primitives::quadrant_t q)
    {
        ++m_depth;
        m_x <<= 1;
        m_x += quadrant_x(q);
        m_y <<= 1;
        m_y += quadrant_y(q);
    }

    auto make_box() const
    {
        Box box;
        box.xmin = m_domain->xmin() + m_x * m_domain->xdim(m_depth);
        box.ymin = m_domain->ymin() + m_y * m_domain->ydim(m_depth);
        box.xmax = box.xmin + m_domain->xdim(m_depth);
        box.ymax = box.ymin + m_domain->ydim(m_depth);
        return box;
    }

    auto x() const { return m_x; }
    auto y() const { return m_y; }

private:
    const Domain* m_domain {nullptr};
    primitives::depth_t m_depth {0};
    primitives::grid_t m_x {0};
    primitives::grid_t m_y {0};

    primitives::grid_t quadrant_x(primitives::quadrant_t q)
    {
        // assuming "N" curve; see morton_keys::interleave_coordinates for an explanation.
        switch(q)
        {
            case 0:
            case 1:
            {
                return 0;
            }
            case 2:
            case 3:
            {
                return 1;
            }
            default: return 0;
        }
    }

    primitives::grid_t quadrant_y(primitives::quadrant_t q)
    {
        // assuming "N" curve; see morton_keys::interleave_coordinates for an explanation.
        switch(q)
        {
            case 0:
            case 2:
            {
                return 0;
            }
            case 1:
            case 3:
            {
                return 1;
            }
            default: return 0;
        }
    }
};

} // namespace point_quadtree

