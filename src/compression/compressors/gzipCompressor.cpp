#include "gzipCompressor.hpp"

//#include <iomanip>
//#include <zlib.h>

using namespace smr;

REGISTER_COMPRESSOR(gzip, GzipCompressor)

void GzipCompressor::deflate(std::istream& is, std::ostream& os) {

}

void GzipCompressor::inflate(std::istream& is, std::ostream& os) {

}

/*void GzipCompressor::deflate(const std::vector<char>& in, std::vector<char>& out)
{
	// Меньше килобайта не сжимаем
	if (in.size() <= 1024)
	{
		out.resize(sizeof(char) + in.size());
		out[0] = 0;
		std::copy(in.begin(), in.end(), out.begin() + sizeof(char));
	}
	else
	{
		size_t compLen = in.size();

		out.resize(sizeof(char) + sizeof(uint32_t) + in.size());

		// Сжимаем
		auto status = compress((Bytef *)(out.data() + sizeof(char) + sizeof(uint32_t)), &compLen, (const Bytef *)in.data(), in.size());
		if (Z_OK == status)
		{
			out.resize(sizeof(char) + sizeof(uint32_t) + compLen);
			out[0] = 1;
			*(uint32_t*)(out.data() + sizeof(char)) = (uint32_t)in.size();
		}
		else
		{
			out.resize(sizeof(char) + in.size());
			out[0] = 0;
			std::copy(in.begin(), in.end(), out.begin() + sizeof(char));
		}
	}
}

void GzipCompressor::inflate(const std::vector<char>& in, std::vector<char>& out)
{
	if (in.size() < sizeof(char))
	{
		throw("Not anough data for trying decompress");
	}

	auto flags = in[0];

	if (flags == 0)
	{
		out.resize(in.size() - sizeof(char));
		std::copy(in.begin() + sizeof(char), in.end(), out.begin());
		return;
	}

	if (flags == 1)
	{
		if (in.size() < sizeof(char) + sizeof(uint32_t))
		{
			throw("Not anough data for decompress");
		}

		size_t origLen = *(uint32_t*)(in.data() + 1);

		out.resize(origLen);

		auto status = uncompress((Bytef *)out.data(), &origLen, (const Bytef *)(in.data() + sizeof(char) + sizeof(uint32_t)), in.size() - sizeof(char) - sizeof(uint32_t));
		if (Z_OK != status)
		{
			throw("Decompression failed");
		}

		return;
	}

	out.reserve(in.size());
	std::copy(in.begin(), in.end(), out.begin());
	return;
}*/
