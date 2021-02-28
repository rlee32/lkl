#pragma once

#include "box.hh"

#include <optional>
#include <vector>

struct MultiBox : public Box {

    bool touches(const Box& other) const {
        if (not Box::touches(other)) {
            return false;
        }
        for (size_t i{0}; i < x_.size(); ++i) {
            if (other.touches(x_[i], y_[i])) {
                return true;
            }
        }
        return false;
    }

    // if x, y is not in this box, the box is enlarged to contain it.
    void include(double x, double y) {
        Box::include(x, y);
        x_.push_back(x);
        y_.push_back(y);
    }

 private:
    // coordinates of constituent points.
    std::vector<primitives::space_t> x_;
    std::vector<primitives::space_t> y_;
};

