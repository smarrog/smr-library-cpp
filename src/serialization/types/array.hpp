#pragma once

#include <vector>

namespace smr {
    class Serializable;

    class Array : public std::vector<Serializable> {

    };
}