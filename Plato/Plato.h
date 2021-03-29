#pragma once
#include "../Tornado/Tornado.h"

class Plato
{
public:
	Plato();
	~Plato();

	void Update();
	PixelBuffer<3>* GetPixelBuffer() const;

private:
	Tornado* tornado;
	RenderTriangle3D rd;

};

