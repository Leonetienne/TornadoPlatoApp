#pragma once
#include "PixelBuffer.h"
#include "Color.h"

class Texture
{
public:
	Texture(const PixelBuffer<4>& pixelBuffer);
	Texture(const Color& color);
	Texture(const Color& color, const Vector2i& size);
	Texture(const Texture& other);
	~Texture();

	void SetPixelBuffer(const PixelBuffer<4>& newPxb);
	PixelBuffer<4>& GetPixelBuffer();
	const PixelBuffer<4>& GetPixelBuffer() const;

private:
	PixelBuffer<4>* pixelBuffer = nullptr;
};
