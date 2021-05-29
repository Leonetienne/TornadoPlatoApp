#pragma once
#include "Vector3.h"

class Collider
{
public:
	//! Tests, if this Collider contains a point
	virtual bool Contains(const Vector3d& point) const = 0;
};

