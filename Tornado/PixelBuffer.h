#pragma once
#include <cstdint>
#include "Vector2.h"

// T = num channels
template <std::size_t T>
class PixelBuffer
{
public:
	PixelBuffer(const uint8_t* data, const Vector2i& size);
	PixelBuffer(const Vector2i& size);
	PixelBuffer(const PixelBuffer& other);

	~PixelBuffer();

	// Will return the raw pixel buffer
	uint8_t* GetRawData();
	const uint8_t* GetRawData() const;

	// Will copy an existing raw pixel buffer, and adjust size, if needed.
	// This assumes that the length of 'data' is equal to size.x*size.y*T.
	void Refit(const uint8_t* data, const Vector2i& size);

	// Will just return T, aka the amount of channels
	std::size_t GetChannelWidth();

	// Will return the width and height
	const Vector2i& GetSize();

	// Will return a pointer to a pixel at position pos
	uint8_t* GetPixel(const Vector2i& pos);
	// Will return a pointer to a pixel at position pos
	const uint8_t* GetPixel(const Vector2i& pos) const;

	// Will return a pointer to a pixel at position pos, with a channel offset
	uint8_t* GetPixel(const Vector2i& pos, uint8_t channelOffset);
	// Will return a pointer to a pixel at position pos, with a channel offset
	const uint8_t* GetPixel(const Vector2i& pos, uint8_t channelOffset) const;

	// Will return the size in bytes of the raw pixel buffer
	std::size_t GetSizeofBuffer() const;

private:
	uint8_t* pixelBuffer;
	Vector2i size;
	bool isInitialized = false;
};
