#pragma once
#include <string>
#include "../Plato/bmplib.h"
#include "../Tornado/Texture.h"

inline void LoadTextureFromBmp(Texture* txt, const std::string& file)
{
	BMPlib::BMP bmp;
	bmp.Read("../Plato/Cube_thielen_gitignore_.bmp");
	bmp.ConvertTo(BMPlib::BMP::COLOR_MODE::RGBA);
	
	PixelBuffer<4> txtPxb(bmp.GetPixelBuffer(), Vector2i(bmp.GetWidth(), bmp.GetHeight()));
	txt->SetPixelBuffer(txtPxb);
	
	return;
}