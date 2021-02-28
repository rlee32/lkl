#pragma once

#include "domain.hh"
#include "grid_position.hh"
#include "node.hh"
#include "morton_keys.hh"
#include <constants.hh>
#include <primitives.hh>

#include <stdexcept>
#include <vector>

namespace point_quadtree {

class PointInserter
{
public:
    PointInserter(const GridPosition& grid_position
        , const std::vector<primitives::morton_key_t>& morton_keys
        , primitives::point_id_t point
        , point_quadtree::Node* current_node
        , primitives::depth_t current_depth);

    void place();

private:
    const std::vector<primitives::morton_key_t>& m_morton_keys;

    const primitives::point_id_t        m_point {constants::invalid_point};
    const morton_keys::InsertionPath    m_path;

    point_quadtree::Node*   m_current_node  {nullptr};
    primitives::depth_t     m_current_depth {0}; // 0 is root.
    GridPosition            m_grid_position;

    void descend();
};

} // namespace point_quadtree

