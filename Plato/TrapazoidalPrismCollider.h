#pragma once
#include "Collider.h"
#include "Camera.h"
#include "../Eule/TrapazoidalPrismCollider.h"

class PTrapazoidalPrismCollider : public PCollider, private TrapazoidalPrismCollider
{
public:
	// Make these public again
	using TrapazoidalPrismCollider::BACK;
	using TrapazoidalPrismCollider::FRONT;
	using TrapazoidalPrismCollider::LEFT;
	using TrapazoidalPrismCollider::RIGHT;
	using TrapazoidalPrismCollider::TOP;
	using TrapazoidalPrismCollider::BOTTOM;

	using TrapazoidalPrismCollider::GetVertex;

	//! Will set the value of a specific vertex
	void SetVertex(std::size_t index, const Vector3d value);

	//! Will return a pointer to this collider in world space.
	virtual const Collider* WorldSpaceColldier() const override;

	//! Will return a pointer to this collider in camera space.
	virtual const Collider* CameraSpaceColldier() const override;

protected:
	friend class WorldObject;
	PTrapazoidalPrismCollider(WorldObject* worldObject);

private:
	void LateUpdate(double frameTime) override;

	//! collider in world space
	TrapazoidalPrismCollider worldspaceCollider;

	//! collider in camera space
	TrapazoidalPrismCollider cameraspaceCollider;

	Camera* cam;
};
