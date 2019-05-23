#pragma once

#include <vector>
#include <string>
#include <set>

namespace smr {
    class Amf3Traits {
    public:
        std::string className;
        bool dynamic = false;
        bool externalizable = false;

        void addAttribute(std::string name) {
            _attributes.insert(std::move(name));
        }

        bool hasAttribute(const std::string& name) const {
            return _attributes.find(name) != _attributes.end();
        }

        const std::set<std::string>& getAttributes() const {
            return _attributes;
        }

        bool operator == (const Amf3Traits& other) const {
            return dynamic == other.dynamic && externalizable == other.externalizable && className == other.className && _attributes == other._attributes;
        }

        bool operator!=(const Amf3Traits& other) const
        {
            return !(*this == other);
        }
    private:
        std::set<std::string> _attributes;
    };
}