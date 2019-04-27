#pragma once

#include <stdexcept>
#include <iostream>
#include <sstream>

namespace smr {
	class DecodeException : public std::runtime_error {
	public:
		DecodeException(const std::string& decoderType, std::istream& is, const std::exception& exception, size_t nextBufSize = 50)
			: std::runtime_error([&]() {
			std::ostringstream oss;
			auto pos = is.tellg();
			while (is.peek() != -1 && nextBufSize--) {
				oss << static_cast<char>(is.get());
			}
			return std::runtime_error(
				std::string(decoderType + " decode error ->\n") +
				exception.what() + '\n' +
				"position " + std::to_string(pos) + '\n' +
				"next: " + oss.str());
		}()) {}
	};

	class EncodeException : public std::runtime_error {
	public:
		EncodeException(const std::string& decoderType, const std::exception& exception)
			: std::runtime_error([&]() {
			return std::runtime_error(
				std::string(decoderType + " encode error ->\n") +
				exception.what());
		}()) {}
	};

	class UnexpectedEofException : public std::runtime_error {
	public:
		explicit UnexpectedEofException(const std::string& description = std::string())
			: std::runtime_error(std::string("Unexpected eof ") + description) {};
	};

	class NotImplementException : public std::runtime_error {
	public:
		explicit NotImplementException(const std::string& description = std::string())
			: std::runtime_error(std::string("Not implemented ") + description) {};
	};

	template <typename T>
	class UnexpectedTokenException : public std::runtime_error {
	public:
		explicit UnexpectedTokenException(T token, const std::string& description = std::string())
			: std::runtime_error(std::string("Unexpected token ") + std::to_string(token) + description) {}
	};
}