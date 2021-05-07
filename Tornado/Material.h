#pragma once
#include "Texture.h"

/** Contains properties of this objects appearance.
*/
struct Material
{
	Material();
	Material(Texture* texture);

	Texture* texture = nullptr;
	bool noShading = false;
};
