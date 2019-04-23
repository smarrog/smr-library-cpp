#pragma once

#include <algorithm>

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end)
        : _begin(begin)
        , _end(end) { }

    Iterator begin() const {
        return _begin;
    }

    Iterator end() const {
        return _end;
    }

private:
    Iterator _begin;
    Iterator _end;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
    return IteratorRange{v.begin(), std::next(v.begin(), std::min(top, v.size()))};
}
