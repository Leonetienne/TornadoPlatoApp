#pragma once
#include "Component.h"
#include "../Tornado/ProjectionProperties.h"

class Camera : public Component
{
public:
	// Will set the rendering resolution
	void SetRenderResolution(const Vector2i& resolution);

	// Will return the render resolution
	const Vector2i& GetRenderResolution() const;

	// Will set the field of view, in degrees
	void SetFov(double fov);

	// Will return the field of view, in degrees
	double GetFov() const;

	// Will set the nearclip
	void SetNearclip(double nearclip);

	// Will return the nearclip
	double GetNearclip() const;

	// Will set the farclip
	void SetFarclip(double farclip);

	// Will return the farclip
	double GetFarclip() const;

	// Will return the square of the farclip
	double GetSqrFarclip() const;

	// Will return the projection matrix
	const ProjectionProperties& GetProjectionProperties() const;

private:
	ProjectionProperties projectionProperties;

private:
	Camera(WorldObject* worldObject, const Vector2i& renderResolution, double fov, double nearclip, double farclip);

	friend WorldObject;
};
