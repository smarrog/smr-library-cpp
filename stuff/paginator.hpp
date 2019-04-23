#include <vector>

template <typename Iterator>
class Page {
public:
    Page(Iterator begin, Iterator end)
        : _begin(begin)
        , _end (end) { }

    Iterator begin() const {
        return _begin;
    }

    Iterator end() const {
        return _end;
    }

    size_t size() const {
        return end() - begin();
    }
private:
    Iterator _begin;
    Iterator _end;
};

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, size_t pageSize)
        : _begin(begin)
        , _end(end)
        , _pageSize(pageSize) {
        auto current = _begin;
        while (current != _end) {
            auto pageEnd = current + std::min(pageSize, static_cast<size_t>(_end - current));
            _pages.emplace_back(current, pageEnd);
            current = pageEnd;
        }
    }

    auto begin() const {
        return _pages.begin();
    }

    auto end() const {
        return _pages.end();
    }

    size_t size() const {
        return end() - begin();
    }
private:
    Iterator _begin;
    Iterator _end;
    size_t _pageSize;
    std::vector<Page<Iterator>> _pages;
};

template <typename Container>
auto Paginate(Container& container, size_t page_size) {
    return Paginator(container.begin(), container.end(), page_size);
}