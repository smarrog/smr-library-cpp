#pragma once

#include <stdint.h>

namespace smr::morton {
	bool static getBitAt(uint32_t value, uint32_t bitIndex) {
		return (value & (1 << bitIndex)) > 0;
	}

	uint32_t static setBitAt(uint32_t value, uint32_t bitIndex, bool bit) {
		if (getBitAt(value, bitIndex) != bit)
			value |= (1 << bitIndex);
		return value;
	}

	static uint32_t getIndex(uint32_t x, uint32_t y) {
		x = (x | (x << 8)) & 0x00FF00FF;
		x = (x | (x << 4)) & 0x0F0F0F0F;
		x = (x | (x << 2)) & 0x33333333;
		x = (x | (x << 1)) & 0x55555555;

		y = (y | (y << 8)) & 0x00FF00FF;
		y = (y | (y << 4)) & 0x0F0F0F0F;
		y = (y | (y << 2)) & 0x33333333;
		y = (y | (y << 1)) & 0x55555555;

		return x | (y << 1);
	}

	uint32_t static getEven(uint32_t index) { // extract even bits
		index = index & 0x55555555;
		index = (index | (index >> 1)) & 0x33333333;
		index = (index | (index >> 2)) & 0x0F0F0F0F;
		index = (index | (index >> 4)) & 0x00FF00FF;
		index = (index | (index >> 8)) & 0x0000FFFF;
		return index;
	}
}