#pragma once
#include "Vector3.h"

namespace Eule
{
	/** Abstract class of a collider domain.
	* Specializations describe a shape in 3d space, and provide implementations of the methods below,
	* for their specific shape. Examples could be a SphereCollider, a BoxCollider, etc...
	*/
	class Collider
	{
	public:
		//! Tests, if this Collider contains a point
		virtual bool Contains(const Vector3d& point) const = 0;
	};
}
