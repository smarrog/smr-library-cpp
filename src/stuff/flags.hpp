#pragma once

namespace smr {
    class Flags {
    public:
        explicit Flags(uint32_t flags = 0)
            : _flags(flags) {};

        Flags& setFlags(uint32_t flags) {
            _flags = flags;
            return *this;
        }

        Flags& reset() {
            return setFlags(0);
        }

        Flags& setFlag(uint32_t flag) {
            _flags |= flag;
            return *this;
        }

        Flags& unsetFlag(uint32_t flag) {
            _flags &= ~flag & _flags;
            return *this;
        }

        bool isFlagSet(uint32_t flag) const {
            return (_flags & flag) > 0;
        }

    private:
        uint32_t _flags;
    };
}