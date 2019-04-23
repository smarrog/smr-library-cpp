#pragma once

struct Dummy final {
    Dummy() noexcept = default;
    Dummy(const Dummy&) = delete;
    Dummy& operator=(Dummy const&) = delete;
    Dummy(Dummy&&) noexcept = default;
    Dummy& operator=(Dummy&&) noexcept = delete;
};