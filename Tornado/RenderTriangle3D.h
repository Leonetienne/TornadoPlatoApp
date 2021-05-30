#pragma once
#include "Vertex.h"
#include "Material.h"

namespace TorGL
{
	/** Data structure describing a face do be rendered
	*/
	struct RenderTriangle3D
	{
		Vertex a;
		Vertex b;
		Vertex c;

		const Material* material = nullptr; //! Material to render with
	};
}
