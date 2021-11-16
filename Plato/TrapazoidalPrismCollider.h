#pragma once
#include "Collider.h"
#include "Eule/TrapazoidalPrismCollider.h"

namespace Plato
{
	namespace Components
	{
		class TrapazoidalPrismCollider : public Collider, private Eule::TrapazoidalPrismCollider
		{
		public:
			// Make these public again
			using Eule::TrapazoidalPrismCollider::BACK;
			using Eule::TrapazoidalPrismCollider::FRONT;
			using Eule::TrapazoidalPrismCollider::LEFT;
			using Eule::TrapazoidalPrismCollider::RIGHT;
			using Eule::TrapazoidalPrismCollider::TOP;
			using Eule::TrapazoidalPrismCollider::BOTTOM;

			using Eule::TrapazoidalPrismCollider::GetVertex;

			//! Will set the value of a specific vertex
			void SetVertex(std::size_t index, const Vector3d value);

			//! Will return a pointer to this collider in world space.
			virtual const Eule::Collider* WorldSpaceColldier() const override;

			//! Will return a pointer to this collider in camera space.
			virtual const Eule::Collider* CameraSpaceColldier() const override;

		protected:
			friend class WorldObject;
			TrapazoidalPrismCollider(WorldObject* worldObject);

		private:
			void LateUpdate(double frameTime) override;

			//! collider in world space
			Eule::TrapazoidalPrismCollider worldspaceCollider;

			//! collider in camera space
			Eule::TrapazoidalPrismCollider cameraspaceCollider;
		};
	}
}
