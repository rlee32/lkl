#pragma once

#include <primitives.hh>
#include <constants.hh>

#include <array>
#include <algorithm> // min_element, max_element
#include <vector>
#include <ostream>

namespace point_quadtree {

class Domain
{
public:
    Domain(const std::vector<primitives::space_t>& x, const std::vector<primitives::space_t>& y)
        : m_x(x), m_y(y)
    {
        // Get domain bounds
        m_xmin = *std::min_element(std::cbegin(x), std::cend(x));
        auto xmax = *std::max_element(std::cbegin(x), std::cend(x));
        m_ymin = *std::min_element(std::cbegin(y), std::cend(y));
        auto ymax = *std::max_element(std::cbegin(y), std::cend(y));
        auto xrange = xmax - m_xmin;
        auto yrange = ymax - m_ymin;
        // Points within each node have the same Morton Key prefix.
        // So when a point is on the exact boundary,
        // it gets put into the quadrant it would go in if it was a small increment
        // larger (in either x or y). This is fine, EXCEPT at the root. There is no
        // next quadrant. So, we have to apply fudge factor to ranges to completely
        // capture boundary points within the root node.
        constexpr primitives::space_t RootNodeMargin {0.00001};
        m_xmin -= RootNodeMargin * xrange;
        m_ymin -= RootNodeMargin * yrange;
        xrange *= 1 + 2 * RootNodeMargin;
        yrange *= 1 + 2 * RootNodeMargin;
        primitives::grid_t grid_dim {1};
        for (primitives::depth_t depth {0}; depth < constants::max_tree_depth; ++depth)
        {
            m_xdim[depth] = xrange / static_cast<primitives::space_t>(grid_dim);
            m_ydim[depth] = yrange / static_cast<primitives::space_t>(grid_dim);
            grid_dim <<= 1;
        }
    }
    auto xmin() const { return m_xmin; }
    auto ymin() const { return m_ymin; }
    auto xdim(int depth) const { return m_xdim[depth]; }
    auto ydim(int depth) const { return m_ydim[depth]; }

    const auto& x() const { return m_x; }
    const auto& y() const { return m_y; }

private:
    const std::vector<primitives::space_t>& m_x;
    const std::vector<primitives::space_t>& m_y;

    primitives::space_t m_xmin {0};
    primitives::space_t m_ymin {0};
    std::array<primitives::space_t, constants::max_tree_depth> m_xdim; // x-dimension of boxes.
    std::array<primitives::space_t, constants::max_tree_depth> m_ydim; // y-dimension of boxes.
};

} // namespace point_quadtree
