#pragma once

#include <string>
#include <map>

namespace smr {
    class Serializable;

    class Object : public std::map<std::string, Serializable> {

    };
}