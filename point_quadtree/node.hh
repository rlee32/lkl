#pragma once

// Only leaf nodes have points.

#include <box.hh>
#include <primitives.hh>

#include <algorithm> // all_of
#include <array>
#include <iostream>
#include <memory> // unique_ptr
#include <vector>

namespace point_quadtree {

class Node
{
public:
    Node(const Box&);

    Node* create_child(primitives::quadrant_t, const Box& box);
    const std::array<std::unique_ptr<Node>, 4>& children() const { return m_children; }
    Node* child(primitives::quadrant_t q) { return m_children[q].get(); }

    void insert(primitives::point_id_t i);

    size_t empty() const { return m_points.empty(); }
    size_t size() const { return m_points.size(); }

    // TODO: consider making this non-member.
    std::vector<primitives::point_id_t>
        get_points
        (primitives::point_id_t i, const Box& search_box) const;

    const auto& box() const { return m_box; }
    bool leaf() const;
    primitives::point_id_t pop();

private:
    // Children are indexed by Morton key quadrant.
    std::array<std::unique_ptr<Node>, 4> m_children;

    std::vector<primitives::point_id_t> m_points;
    const Box m_box;

    bool touches(const Box&) const;
    void get_points(primitives::point_id_t i
        , const Box& search_box
        , std::vector<primitives::point_id_t>& points) const;

};

} // namespace point_quadtree
