#pragma once
#include "Vector2.h"
#include "PixelBuffer.h"
#include "Color.h"

namespace TorGL
{
	/** An RGBA texture
	*/
	class Texture
	{
	public:
		Texture(const PixelBuffer<4>& pixelBuffer);
		Texture(const Color& color);
		Texture(const Color& color, const Vector2i& size);
		Texture(const Texture& other);
		~Texture();

		//! Will set a new pixel buffer as the content
		void SetPixelBuffer(const PixelBuffer<4>& newPxb);

		//! Will return the current pixel buffer
		PixelBuffer<4>& GetPixelBuffer();
		//! Will return the current pixel buffer
		const PixelBuffer<4>& GetPixelBuffer() const;

	private:
		PixelBuffer<4>* pixelBuffer = nullptr;
	};
}
