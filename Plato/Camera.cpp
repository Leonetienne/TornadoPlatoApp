#include "Camera.h"
#include "WorldObjectManager.h"

Camera::Camera(WorldObject* worldObject, const Vector2i& renderResolution, double fov, double nearclip, double farclip) :
	Component(worldObject),
	projectionProperties(renderResolution, fov, nearclip, farclip)
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

Camera* Camera::GetMainCamera()
{
	return mainCamera;
}

void Camera::SetRenderResolution(const Vector2i& resolution)
{
	projectionProperties.SetResolution(resolution);
	return;
}

const Vector2i& Camera::GetRenderResolution() const
{
	return projectionProperties.GetResolution();
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
