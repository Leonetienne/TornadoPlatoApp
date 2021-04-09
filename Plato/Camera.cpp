#include "Camera.h"

Camera::Camera(WorldObject* worldObject, const Vector2i& renderResolution, double fov, double nearclip, double farclip) :
	Component(worldObject),
	projectionProperties(renderResolution, fov, nearclip, farclip)
{
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
