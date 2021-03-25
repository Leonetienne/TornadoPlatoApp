#pragma once
#include "Vector2.h"
#include "Matrix4x4.h"
#include "Constants.h"

class ProjectionProperties
{
public:
	ProjectionProperties(const Vector2i& resolution, double fov, double nearclip, double farclip);

	double GetFov() const;
	void SetFov(double fov);

	double GetNearclip() const;
	void SetNearclip(double nearclip);

	double GetFarclip() const;
	long double GetSqrFarclip() const;
	void SetFarclip(double farclip);

	double GetAspectRatio() const;

	const Vector2i& GetResolution() const;
	void SetResolution(const Vector2i& resolution);

	const Matrix4x4& GetProjectionMatrix() const;

private:
	void UpdateMatrix();

	double fov;
	double nearclip;
	double farclip;
	long double sqrFarclip;
	double aspectRatio;
	Vector2i resolution;
	Matrix4x4 projectionMatrix;
};
