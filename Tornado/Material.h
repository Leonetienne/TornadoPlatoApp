#pragma once
#include "Texture.h"

namespace TorGL
{
	/** Contains properties of this objects appearance.
	*/
	struct Material
	{
		Material();
		Material(Texture* texture);

		Texture* texture = nullptr;
		bool noShading = false;
	};
}
