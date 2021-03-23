#include "PixelBuffer.h"

template <std::size_t T>
PixelBuffer<T>::PixelBuffer(const uint8_t* data, const Vector2i& size)
{
	Refit(data, size);
	return;
}

template<std::size_t T>
PixelBuffer<T>::PixelBuffer(const Vector2i& size)
{
	this->size = size;
	pixelBuffer = new uint8_t[GetSizeofBuffer()];
	memset(pixelBuffer, 0, GetSizeofBuffer());
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
	// Resize buffer size, if needed
	if ((isInitialized) && (size != this->size))
	{
		delete[] pixelBuffer;
		pixelBuffer = new uint8_t[GetSizeofBuffer()];
	}

	// Copy old buffer
	memcpy_s(pixelBuffer, GetSizeofBuffer(), data, GetSizeofBuffer());

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
const Vector2i& PixelBuffer<T>::GetSize()
{
	return size;
}

template<std::size_t T>
inline uint8_t* PixelBuffer<T>::GetPixel(const Vector2i& pos)
{
	return pixelBuffer + (T * (pos.y*size.x + pos.x));
}

template<std::size_t T>
inline const uint8_t* PixelBuffer<T>::GetPixel(const Vector2i& pos) const
{
	return pixelBuffer + (T * (pos.y * size.x + pos.x));
}

template<std::size_t T>
inline uint8_t* PixelBuffer<T>::GetPixel(const Vector2i& pos, uint8_t channelOffset)
{
	return pixelBuffer + (T * (pos.y * size.x + pos.x)) + channelOffset;
}

template<std::size_t T>
inline const uint8_t* PixelBuffer<T>::GetPixel(const Vector2i& pos, uint8_t channelOffset) const
{
	return pixelBuffer + (T * (pos.y * size.x + pos.x)) + channelOffset;
}

template<std::size_t T>
std::size_t PixelBuffer<T>::GetSizeofBuffer() const
{
	return sizeof(uint8_t) * size.x * size.y * T;
}
