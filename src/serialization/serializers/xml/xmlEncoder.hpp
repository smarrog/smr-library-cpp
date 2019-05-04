#pragma once

#include "serialization/encoder.hpp"

namespace smr {
	class XmlEncoder final : public Encoder {
	public:
		explicit XmlEncoder(std::ostream& os, const SerializerConfig& config)
			: Encoder(os, config) {}

		XmlEncoder(const XmlEncoder&) = delete;
		XmlEncoder& operator = (XmlEncoder const&) = delete;
		XmlEncoder(XmlEncoder&&) noexcept = delete;
		XmlEncoder& operator = (XmlEncoder&&) noexcept = delete;

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