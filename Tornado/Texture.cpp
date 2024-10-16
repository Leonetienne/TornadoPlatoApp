#include "Texture.h"

using namespace TorGL;

Texture::Texture(const PixelBuffer<4>& pixelBuffer)
{
	this->pixelBuffer = new PixelBuffer<4>(pixelBuffer);

	return;
}

Texture::Texture(const Color& color)
{
	pixelBuffer = new PixelBuffer<4>({ 1,1 });

	*pixelBuffer->GetPixel({ 0, 0 }, 0) = (uint8_t)color.r;
	*pixelBuffer->GetPixel({ 0, 0 }, 1) = (uint8_t)color.g;
	*pixelBuffer->GetPixel({ 0, 0 }, 2) = (uint8_t)color.b;
	*pixelBuffer->GetPixel({ 0, 0 }, 3) = (uint8_t)color.a;

	return;
}

Texture::Texture(const Color& color, const Vector2i& size)
{
	pixelBuffer = new PixelBuffer<4>(size);

	std::size_t cachedSize = pixelBuffer->GetSizeofBuffer();
	for (std::size_t i = 0; i < cachedSize; i += 4)
	{
		pixelBuffer->GetRawData()[i+0] = (uint8_t)color.r;
		pixelBuffer->GetRawData()[i+1] = (uint8_t)color.g;
		pixelBuffer->GetRawData()[i+2] = (uint8_t)color.b;
		pixelBuffer->GetRawData()[i+3] = (uint8_t)color.a;
	}
	return;
}

Texture::Texture(const Texture& other)
{
	SetPixelBuffer(*other.pixelBuffer);

	return;
}

Texture::~Texture()
{
	delete pixelBuffer;

	pixelBuffer = nullptr;

	return;
}

void Texture::SetPixelBuffer(const PixelBuffer<4>& newPxb)
{
	if (pixelBuffer != nullptr)
		delete pixelBuffer;

	pixelBuffer = new PixelBuffer<4>(newPxb);

	return;
}

PixelBuffer<4>& Texture::GetPixelBuffer()
{
	return *pixelBuffer;
}

const PixelBuffer<4>& Texture::GetPixelBuffer() const
{
	return *pixelBuffer;
}
