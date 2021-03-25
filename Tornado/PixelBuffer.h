#pragma once
#include <cstdint>
#include <exception>
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
	const Vector2i& GetDimensions();

	// Will return a pointer to a pixel at position pos
	uint8_t* GetPixel(const Vector2i& pos);
	// Will return a pointer to a pixel at position pos
	const uint8_t* GetPixel(const Vector2i& pos) const;

	// Will return a pointer to a pixel at position pos, with a channel offset
	uint8_t* GetPixel(const Vector2i& pos, std::size_t channelOffset);
	// Will return a pointer to a pixel at position pos, with a channel offset
	const uint8_t* GetPixel(const Vector2i& pos, std::size_t channelOffset) const;

	// Will return the size in elements of the raw pixel buffer
	std::size_t GetSizeofBuffer() const;

private:
	uint8_t* pixelBuffer;
	Vector2i size;
	bool isInitialized = false;
};


template <std::size_t T>
PixelBuffer<T>::PixelBuffer(const uint8_t* data, const Vector2i& size)
{
	Refit(data, size);
	return;
}

template<std::size_t T>
PixelBuffer<T>::PixelBuffer(const Vector2i& size)
{
	if ((size.x < 1) || (size.y < 1) || (T < 1))
		throw std::exception("Can't create a pixel buffer of area <= 0!");

	this->size = size;
	std::size_t cachedBufferSize = GetSizeofBuffer();
	pixelBuffer = new uint8_t[cachedBufferSize];
	memset(pixelBuffer, 0, cachedBufferSize);
	isInitialized = true;

	return;
}

template<std::size_t T>
PixelBuffer<T>::PixelBuffer(const PixelBuffer& other)
{
	Refit(other.data, other.size);
	return;
}

template<std::size_t T>
PixelBuffer<T>::~PixelBuffer()
{
	delete[] pixelBuffer;
	pixelBuffer = nullptr;

	return;
}

template<std::size_t T>
uint8_t* PixelBuffer<T>::GetRawData()
{
	return pixelBuffer;
}

template<std::size_t T>
const uint8_t* PixelBuffer<T>::GetRawData() const
{
	return pixelBuffer;
}

template<std::size_t T>
void PixelBuffer<T>::Refit(const uint8_t* data, const Vector2i& size)
{
	if ((size.x < 1) || (size.y < 1) || (T < 1))
		throw std::exception("Can't create a pixel buffer of area <= 0!");


	// Resize buffer size, if needed
	// If is already initialized, only resize if the size has changed.
	// If it is not initialized, resize either way.
	if (((isInitialized) && (size != this->size)) || (!isInitialized))
	{
		if (isInitialized)
			delete[] pixelBuffer;
		
		this->size = size;
		pixelBuffer = new uint8_t[GetSizeofBuffer()];
	}

	// Copy existing buffer
	std::size_t cachedBufferSize = GetSizeofBuffer();
	memcpy_s(pixelBuffer, cachedBufferSize, data, cachedBufferSize);

	// Set flag
	isInitialized = true;

	return;
}

template<std::size_t T>
std::size_t PixelBuffer<T>::GetChannelWidth()
{
	return T;
}

template<std::size_t T>
const Vector2i& PixelBuffer<T>::GetDimensions()
{
	return size;
}

template<std::size_t T>
inline uint8_t* PixelBuffer<T>::GetPixel(const Vector2i& pos)
{
	const std::size_t posx = pos.x;
	const std::size_t posy = pos.y;
	return pixelBuffer + (T * (posy * size.x + posx));
}

template<std::size_t T>
inline const uint8_t* PixelBuffer<T>::GetPixel(const Vector2i& pos) const
{
	const std::size_t posx = pos.x;
	const std::size_t posy = pos.y;
	return pixelBuffer + (T * (posy * size.x + posx));
}

template<std::size_t T>
inline uint8_t* PixelBuffer<T>::GetPixel(const Vector2i& pos, std::size_t channelOffset)
{
	const std::size_t posx = pos.x;
	const std::size_t posy = pos.y;
	return pixelBuffer + (T * (posy * size.x + posx)) + channelOffset;
}

template<std::size_t T>
inline const uint8_t* PixelBuffer<T>::GetPixel(const Vector2i& pos, std::size_t channelOffset) const
{
	const std::size_t posx = pos.x;
	const std::size_t posy = pos.y;
	return pixelBuffer + (T * (posy * size.x + posx)) + channelOffset;
}

template<std::size_t T>
std::size_t PixelBuffer<T>::GetSizeofBuffer() const
{
	return (std::size_t)size.x * (std::size_t)size.y * T;
}
