#pragma once
#include "Component.h"
#include "../Eule/Collider.h"

namespace Plato
{
	namespace Components
	{
		/** Abstract implementation of a Collider Component
		*/
		class PCollider : public Component
		{
		public:
			//! Will return a pointer to this collider in world space.
			virtual const Eule::Collider* WorldSpaceColldier() const = 0;

			//! Will return a pointer to this collider in camera space.
			virtual const Eule::Collider* CameraSpaceColldier() const = 0;

		protected:
			// No WorldObject friend declaration, because this abstract component should not be instanciable
			PCollider(WorldObject* worldObject);

		private:

		};
	}
}
