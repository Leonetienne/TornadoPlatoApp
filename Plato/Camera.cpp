#include "Camera.h"
#include "WorldObjectManager.h"

using namespace Plato;
using namespace Plato::Components;
using namespace TorGL;

Camera::Camera(WorldObject* worldObject, double fov, double nearclip, double farclip) :
	Component(worldObject),
    projectionProperties(Vector2i(0,0), fov, nearclip, farclip)
{
	// If there is no main camera, set myself as the new main camera
	if (mainCamera == nullptr)
		SetAsMainCamera();

	return;
}

Camera::~Camera()
{
	if (mainCamera == this)
		mainCamera = nullptr;

	return;
}

double Camera::GetFarclip() const
{
	return projectionProperties.GetFarclip();
}

double Camera::GetSqrFarclip() const
{
	return projectionProperties.GetSqrFarclip();
}

const ProjectionProperties& Camera::GetProjectionProperties() const
{
	return projectionProperties;
}

void Camera::SetAsMainCamera()
{
	// Clear the id on the existing main camera
	WorldObject* currentIdHolder = WorldObjectManager::FindObjectById("main_camera");
	if (currentIdHolder != nullptr)
		currentIdHolder->ClearId();

	// Set own id to main_camera
	worldObject->SetId("main_camera");

	// Update record in classifier
	mainCamera = this;

	return;
}

Vector3d Camera::WorldSpaceToCameraSpace(const Vector3d& worldSpacePoint) const
{
	return transform->GetGlobalRotation().Inverse() * (worldSpacePoint - transform->GetGlobalPosition());
}

Camera* Camera::GetMainCamera()
{
	return mainCamera;
}

void Camera::SetFov(double fov)
{
	projectionProperties.SetFov(fov);
	return;
}

double Camera::GetFov() const
{
	return projectionProperties.GetFov();
}

void Camera::SetNearclip(double nearclip)
{
	projectionProperties.SetNearclip(nearclip);
	return;
}

double Camera::GetNearclip() const
{
	return projectionProperties.GetNearclip();
}

void Camera::SetFarclip(double farclip)
{
	projectionProperties.SetFarclip(farclip);
	return;
}

Camera* Camera::mainCamera = nullptr;
