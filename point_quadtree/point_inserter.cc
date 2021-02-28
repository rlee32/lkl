#include "point_inserter.hh"

namespace point_quadtree {

void PointInserter::place()
{
    while (not m_current_node->leaf())
    {
        descend();
    }
    if (m_current_node->empty() or m_current_depth == constants::max_tree_depth)
    {
        m_current_node->insert(m_point);
        return;
    }

    auto previous_node = m_current_node;
    auto previous_grid_position = m_grid_position;
    descend();
    m_current_node->insert(m_point);
    PointInserter other(previous_grid_position
        , m_morton_keys
        , previous_node->pop()
        , previous_node
        , m_current_depth - 1);
}

void PointInserter::descend()
{
    auto q = m_path[m_current_depth];
    auto child = m_current_node->child(q);
    m_grid_position.descend(q);
    if (not child)
    {
        child = m_current_node->create_child(q, m_grid_position.make_box());
    }
    m_current_node = child;
    ++m_current_depth;
}

PointInserter::PointInserter(const GridPosition& grid_position
    , const std::vector<primitives::morton_key_t>& morton_keys
    , primitives::point_id_t point
    , point_quadtree::Node* current_node
    , primitives::depth_t current_depth)
    : m_morton_keys(morton_keys)
    , m_point(point)
    , m_path(morton_keys::point_insertion_path(morton_keys[point]))
    , m_current_node(current_node)
    , m_current_depth(current_depth)
    , m_grid_position(grid_position)
{
    place();
}

} // namespace point_quadtree

