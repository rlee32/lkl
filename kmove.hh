#pragma once

#include "primitives.hh"

#include <algorithm> // find, count
#include <stdexcept>
#include <vector>

struct KMove
{
    std::vector<primitives::point_id_t> starts;
    std::vector<primitives::point_id_t> ends;
    std::vector<primitives::point_id_t> removes; // removes edge i, next(i)

    size_t current_k() const;

    bool removable(primitives::point_id_t i) const;
    bool startable(primitives::point_id_t i) const;
    bool endable(primitives::point_id_t i) const;
    void clear();
    void validate() const;
    KMove operator+(const KMove& other) const;
    KMove& operator+=(const KMove& other);

    KMove make_reverse() const;
    bool valid() const;

private:
    static bool contains(const std::vector<primitives::point_id_t>& points
        , primitives::point_id_t point);
};


