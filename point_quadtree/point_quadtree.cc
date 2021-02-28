#include "point_quadtree.hh"

namespace point_quadtree {

Node make_quadtree(const std::vector<primitives::space_t>& x
    , const std::vector<primitives::space_t>& y
    , const Domain& domain)
{
    GridPosition grid_position(domain);
    Node root(grid_position.make_box());
    insert_points(x, y, domain, root);
    validate(root);
    std::cout << "max node occupancy: " << max_leaf_points(root) << std::endl;
    std::cout << "max tree depth: " << max_depth(root) << std::endl;
    if (count_points(root) != x.size())
    {
        throw std::logic_error("quadtree root did not count points accurately.");
    }
    return root;
}

void insert_points(const std::vector<primitives::space_t>& x
    , const std::vector<primitives::space_t>& y
    , const Domain& domain
    , Node& root)
{
    const auto morton_keys
    {
        morton_keys::compute_point_morton_keys(x, y, domain)
    };
    GridPosition grid_position(domain);
    for (primitives::point_id_t i {0}; i < morton_keys.size(); ++i)
    {
        PointInserter inserter(grid_position
            , morton_keys
            , i
            , &root
            , 0);
    }
}

size_t count_points(const Node& node)
{
    if (node.leaf())
    {
        return node.size();
    }
    size_t counted {0};
    for (const auto& unique_ptr : node.children())
    {
        if (unique_ptr)
        {
            counted += count_points(*unique_ptr);
        }
    }
    return counted;
}

size_t count_nodes(const Node& node)
{
    size_t counted {1};
    for (const auto& unique_ptr : node.children())
    {
        if (unique_ptr)
        {
            counted += count_nodes(*unique_ptr);
        }
    }
    return counted;
}

void validate(const Node& node, primitives::depth_t depth)
{
    if (node.leaf() and node.empty())
    {
        throw std::logic_error("leaf node is empty!");
    }
    if (not node.leaf() and not node.empty())
    {
        throw std::logic_error("non-leaf node is not empty!");
    }
    if (depth != constants::max_tree_depth and node.size() > 1)
    {
        throw std::logic_error("found non-max-depth node with more than 1 point!");
    }
    if (depth > constants::max_tree_depth)
    {
        throw std::logic_error("max tree depth exceeded!");
    }
    for (const auto& unique_ptr : node.children())
    {
        if (unique_ptr)
        {
            validate(*unique_ptr, depth + 1);
        }
    }
}

size_t max_leaf_points(const Node& node)
{
    size_t max = node.size();
    for (const auto& unique_ptr : node.children())
    {
        if (unique_ptr)
        {
            max = std::max(max, max_leaf_points(*unique_ptr));
        }
    }
    return max;
}

primitives::depth_t max_depth(const Node& node, primitives::depth_t depth)
{
    auto max {depth};
    for (const auto& unique_ptr : node.children())
    {
        if (unique_ptr)
        {
            max = std::max(max, max_depth(*unique_ptr, depth + 1));
        }
    }
    return max;
}

} // namespace point_quadtree
