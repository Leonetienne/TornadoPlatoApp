#pragma once
#include "Component.h"
#include "Vector.h"
#include "../Tornado/ProjectionProperties.h"

namespace Plato
{
	namespace Components
	{
		/** Component to represent a 3d camera that can be rendered from
		*/
		class Camera : public Component
		{
		public:
			//! Will set the field of view, in degrees
			void SetFov(double fov);

			//! Will return the field of view, in degrees
			double GetFov() const;

			//! Will set the nearclip
			void SetNearclip(double nearclip);

			//! Will return the nearclip
			double GetNearclip() const;

			//! Will set the farclip
			void SetFarclip(double farclip);

			//! Will return the farclip
			double GetFarclip() const;

			//! Will return the square of the farclip
			double GetSqrFarclip() const;

			//! Will return the projection matrix. NO RESOLUTION IS SET!!
            //! They HAVE to be set before being passed to tornado!!!
			const TorGL::ProjectionProperties& GetProjectionProperties() const;

			//! Will set the current camera as the new main camera
			void SetAsMainCamera();

			//! Will transform a world-space point to a camera-space point.
			Vector3d WorldSpaceToCameraSpace(const Vector3d& worldSpacePoint) const;

			//! Will return the current main camera
			static Camera* GetMainCamera();

		protected:
			~Camera();

		private:
			Camera(WorldObject* worldObject, double fov, double nearclip, double farclip);
			TorGL::ProjectionProperties projectionProperties;

			friend WorldObject;

			static Camera* mainCamera;
		};
	}
}
