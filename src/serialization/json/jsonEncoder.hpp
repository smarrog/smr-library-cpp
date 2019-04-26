#pragma once

#include "jsonSerializer.hpp"
#include "../encoder.hpp"

namespace smr {
    class JsonEncoder final : public Encoder {
    public:
	    explicit JsonEncoder(std::ostream& os, const SerializerConfig& config)
	    : Encoder(os, config) {}

	    JsonEncoder(const JsonEncoder&) = delete;
	    JsonEncoder& operator = (JsonEncoder const&) = delete;
	    JsonEncoder(JsonEncoder&&) noexcept = delete;
	    JsonEncoder& operator = (JsonEncoder&&) noexcept = delete;

	    void encode(const Serializable& serializable);

    private:
        size_t _offset = 0;

	    void encode(std::nullptr_t);
	    void encode(bool);
	    void encode(int);
	    void encode(double);
	    void encode(const std::string&);
	    void encode(const Array&);
	    void encode(const Object&);

        template <typename Container, typename ElemEncoder>
        void encodeContainer(const Container& container, char open, char close, ElemEncoder elemEncoder);

	    inline void nextLine();
	    inline void addSpacing();
    };
}