#include "multi_segment.hh"

#include <cassert>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <optional>

namespace feasibility {


MultiSegment::MultiSegment(const Cycle &cycle, primitives::point_id_t fixed_tail, primitives::point_id_t head)
    : fixed_tail_(fixed_tail)
    , head_(head) {
    std::cout << "head: " << head << std::endl;
    std::cout << "fixed_tail: " << fixed_tail_ << std::endl;
    auto head_seq = cycle.normalized_sequence(head_, fixed_tail_);
    if (head_seq > 1) {
        assert(head_seq + 1 == cycle.sequence_.size());
        segments_.emplace_back(fixed_tail_, head_);
    } else {
        assert(head_seq == 1);
        segments_.emplace_back(head_, fixed_tail_);
    }
    assert(segments_.back().seqmag(cycle) + 1 == cycle.sequence_.size());
}

primitives::point_id_t MultiSegment::get_end(size_t segment_index) {
    std::cout << "segment index: " << segment_index << std::endl;
    const auto &s = segments_[segment_index];

    if (s.is_degenerate()) {
        return s.first();
    }

    // first segment.
    if (segment_index == 0) {
        return s.other(fixed_tail_);
    }

    // last segment.
    if (segment_index == segments_.size() - 1) {
        std::cout << "LAST" << std::endl;
        std::cout << s.first() << ", " << s.second() << std::endl;
        // this function may be used to update head_, so do not rely on it.
        if (new_edges_.find(s.first()) == std::cend(new_edges_)) {
            const auto &prev = segments_[segment_index - 1];
            std::cout << head_ << ", " << fixed_tail_ << std::endl;
            std::cout << prev.first() << ", " << prev.second() << std::endl;
            std::cout << new_edges_.size() << std::endl;
            for (const auto &p : new_edges_) {
                std::cout << "\t" << p.first << ", " << p.second << std::endl;
            }
            std::cout << s.first() << ", " << s.second() << std::endl;
            assert(new_edges_.find(s.second()) != std::cend(new_edges_));
            return s.first();
        } else {
            assert(new_edges_.find(s.second()) == std::cend(new_edges_));
            assert(new_edges_.find(s.first()) != std::cend(new_edges_));
            return s.second();
        }
    }

    const auto &next = segments_[segment_index + 1];
    assert(new_edges_.find(s.first()) != std::cend(new_edges_));
    assert(new_edges_.find(s.second()) != std::cend(new_edges_));
    const auto &first = new_edges_[s.first()];
    const auto &second = new_edges_[s.second()];
    if (next.has_point(first)) {
        return s.first();
    } else {
        assert(next.has_point(second));
        return s.second();
    }
}

void MultiSegment::reverse(size_t start_index) {
    std::reverse(std::begin(segments_) + start_index, std::end(segments_));
}

std::optional<primitives::point_id_t> MultiSegment::split(const Cycle &cycle, point_id_t split_point) {
    // Find segment to split.
    size_t i{0};
    std::cout << "start" << std::endl;
    for (; i < segments_.size(); ++i) {
        if (segments_[i].has_point(cycle, split_point)) {
            std::cout << "broken" << std::endl;
            break;
        }
    }
    std::cout << "split point: " << split_point << std::endl;
    assert(i < segments_.size());

    // split out segment, update segment order.
    std::cout << "splitting " << segments_[i].first() << ", " << segments_[i].second() << std::endl;
    const auto &old_seqmag = segments_[i].seqmag(cycle);
    const auto &split_toward = get_end(i);
    if (split_toward == split_point) {
        return std::nullopt;
    }
    auto new_segment = segments_[i].split(cycle, split_point, split_toward);
    reverse(i + 1);
    std::cout << "split into " << segments_[i].first() << ", " << segments_[i].second() << std::endl;
    std::cout << "split into " << new_segment.first() << ", " << new_segment.second() << std::endl;
    assert(segments_[i].seqmag(cycle) + new_segment.seqmag(cycle) + 1 == old_seqmag);
    segments_.push_back(new_segment);

    // update new edges.
    std::cout << "new edge " << split_point << ", " << head_ << std::endl;
    new_edges_[split_point] = head_;
    new_edges_[head_] = split_point;

    // update head.
    head_ = get_end(segments_.size() - 1);

    assert(head_ != fixed_tail_);
    return head_;
}

} // namespace feasibility


