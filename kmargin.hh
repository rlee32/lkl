#pragma once

#include "primitives.hh"

#include <vector>

struct KMargin
{
    std::vector<primitives::length_t> increases;
    std::vector<primitives::length_t> decreases;
    primitives::length_t total_margin {0};

    void increase(primitives::length_t increment)
    {
        total_margin += increment;
        increases.push_back(increment);
    }
    void pop_increase()
    {
        total_margin -= increases.back();
        increases.pop_back();
    }

    bool decrease(primitives::length_t increment)
    {
        // TODO: threshold
        if (increment >= total_margin)
        {
            return false;
        }
        total_margin -= increment;
        decreases.push_back(increment);
        return true;
    }

    void pop_decrease()
    {
        total_margin += decreases.back();
        decreases.pop_back();
    }

    void clear()
    {
        increases.clear();
        decreases.clear();
        total_margin = 0;
    }
};



