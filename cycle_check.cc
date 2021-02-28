#include "cycle_check.hh"

namespace cycle_check {

namespace {

// sort deleted edges by sequence number.
std::vector<BrokenEdge> sorted_removes(const Tour &tour
    , const std::vector<primitives::point_id_t> &removes) {
    std::vector<BrokenEdge> deleted_edges;
    for (auto p : removes)
    {
        const auto sequence {tour.sequence(p, removes[0])};
        const auto first {p};
        const auto second {tour.next(p)};
        deleted_edges.push_back({first, second, sequence});
    }
    std::sort(std::begin(deleted_edges), std::end(deleted_edges)
        , [](const auto& lhs, const auto& rhs) { return lhs.sequence < rhs.sequence; });
    return deleted_edges;
}

// map points to up to 2 points making up new edges.
std::unordered_map<primitives::point_id_t, std::vector<primitives::point_id_t>>
compute_new_edge_connectivity(const std::vector<primitives::point_id_t> &starts
    , const std::vector<primitives::point_id_t> &ends) {
    std::unordered_map<primitives::point_id_t, std::vector<primitives::point_id_t>> new_edges;
    for (size_t i {0}; i < starts.size(); ++i)
    {
        new_edges[starts[i]].push_back(ends[i]);
        new_edges[ends[i]].push_back(starts[i]);
    }
    for (const auto& pair : new_edges)
    {
        // TODO: remove this check after verified.
        // There can be 1 or 2 new edges incident to any point.
        if (pair.second.size() > 2)
        {
            throw std::logic_error("too many adjacent points");
        }
    }
    return new_edges;
}

// map constituent points of deleted edges to sequence.
std::unordered_map<primitives::point_id_t, size_t>
compute_sequence(const std::vector<BrokenEdge> &deleted_edges)
{
    std::unordered_map<primitives::point_id_t, size_t> deleted_edge_order;
    for (size_t i {0}; i < deleted_edges.size(); ++i)
    {
        deleted_edge_order[deleted_edges[i].first] = i;
        deleted_edge_order[deleted_edges[i].second] = i;
    }
    return deleted_edge_order;
}

bool breaks_cycle_(const std::vector<BrokenEdge>& deleted_edges
    , const std::unordered_map<primitives::point_id_t, std::vector<primitives::point_id_t>>& new_edges) {
    const auto sequence = compute_sequence(deleted_edges);
    std::unordered_set<primitives::point_id_t> visited;
    visit_cycle(sequence, deleted_edges, new_edges, visited);
    return visited.size() != sequence.size();
}

}  // namespace

bool breaks_cycle(const Tour &tour, const KMove &kmove) {
    const auto deleted_edges = sorted_removes(tour, kmove.removes);
    if (kmove.starts.size() != deleted_edges.size()) {
        throw std::logic_error("number of deleted edges does not equal number of new edges.");
    }
    const auto new_edges = compute_new_edge_connectivity(kmove.starts, kmove.ends);
    return breaks_cycle_(deleted_edges, new_edges);
}

size_t count_cycles(const Tour &tour, const KMove &kmove)
{
    const auto deleted_edges = sorted_removes(tour, kmove.removes);
    if (kmove.starts.size() != deleted_edges.size()) {
        throw std::logic_error("number of deleted edges does not equal number of new edges.");
    }
    const auto new_edges = compute_new_edge_connectivity(kmove.starts, kmove.ends);
    return count_cycles(deleted_edges, new_edges);
}

void visit_cycle(const std::unordered_map<primitives::point_id_t, size_t>& sequence
    , const std::vector<BrokenEdge>& deleted_edges
    , const std::unordered_map<primitives::point_id_t, std::vector<primitives::point_id_t>>& new_edges
    , std::unordered_set<primitives::point_id_t>& visited) {
    primitives::point_id_t current {constants::invalid_point};
    for (const auto& pair : sequence) {
        if (visited.find(pair.first) == std::cend(visited)) {
            current = pair.first;
            visited.insert(current);
            break;
        }
    }
    while (current != constants::invalid_point) {
        // go to next unvisited point in new edge.
        const auto& new_ends = new_edges.find(current)->second;
        current = new_ends[0];
        if (visited.find(current) != std::cend(visited)) {
            if (new_ends.size() == 1) {
                return;
            }
            current = new_ends[1];
            if (visited.find(current) != std::cend(visited)) {
                return;
            }
        }
        visited.insert(current);
        // find next new start, connected by old segments.
        auto index {sequence.find(current)->second};
        const auto& edge {deleted_edges[index]};
        if (edge.first == current) {
            if (index == 0) {
                index = deleted_edges.size() - 1;
            }
            else {
                --index;
            }
            current = deleted_edges[index].second;
        } else {
            ++index;
            if (index == deleted_edges.size()) {
                index = 0;
            }
            current = deleted_edges[index].first;
        }
        visited.insert(current);
    }
}

size_t count_cycles(const std::vector<BrokenEdge>& deleted_edges
    , const std::unordered_map<primitives::point_id_t, std::vector<primitives::point_id_t>>& new_edges) {
    const auto sequence = compute_sequence(deleted_edges);
    std::unordered_set<primitives::point_id_t> visited;
    size_t cycles {0};
    while (visited.size() != sequence.size()) {
        visit_cycle(sequence, deleted_edges, new_edges, visited);
        ++cycles;
    }
    return cycles;
}

bool feasible(const Tour& tour
    , const std::vector<primitives::point_id_t>& starts
    , const std::vector<primitives::point_id_t>& ends
    , const std::vector<primitives::point_id_t>& removes) {
    const auto deleted_edges = sorted_removes(tour, removes);
    if (starts.size() != deleted_edges.size()) {
        throw std::logic_error("number of deleted edges does not equal number of new edges.");
    }
    const auto sequence = compute_sequence(deleted_edges);
    const auto new_edges = compute_new_edge_connectivity(starts, ends);

    // traversal
    const auto start {deleted_edges[0].first};
    auto current {start};
    size_t visited {0};
    size_t max_visited {starts.size() + ends.size()};
    std::unordered_map<primitives::point_id_t, bool> visit_flag;
    visit_flag[current] = true;
    std::unordered_set<primitives::point_id_t> checklist;
    do {
        if (sequence.find(current) == std::cend(sequence)) {
            throw std::logic_error("point not recognized");
        }
        // go to next in new edge.
        auto next = new_edges.find(current)->second.back();
        if (new_edges.find(next)->second.size() > 2) {
            throw std::logic_error("too many adjacent points");
        }
        if (visit_flag[next]) {
            next = new_edges.find(current)->second.front();
        }
        current = next;
        visit_flag[current] = true;
        ++visited;
        if (current == start or checklist.find(current) != std::cend(checklist)) {
            ++visited;
            break;
        }
        checklist.insert(current);
        // find adjacent new edge start point.
        auto index {sequence.find(current)->second};
        const auto& edge {deleted_edges[index]};
        if (edge.first == current) {
            if (index == 0) {
                index = deleted_edges.size() - 1;
            } else {
                --index;
            }
            current = deleted_edges[index].second;
        } else {
            ++index;
            if (index == deleted_edges.size()) {
                index = 0;
            }
            current = deleted_edges[index].first;
        }
        checklist.insert(current);
        visit_flag[current] = true;
        ++visited;
    } while (current != start and visited < max_visited);
    return current == start and visited == max_visited;
}

} // namespace cycle_check
