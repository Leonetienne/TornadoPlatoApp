#include "ProjectionProperties.h"

ProjectionProperties::ProjectionProperties(const Vector2i& resolution, double fov, double nearclip, double farclip)
	:
	fov { fov },
	nearclip { nearclip },
	farclip { farclip },
	sqrFarclip { farclip * farclip },
	resolution{ resolution }
{
	
	aspectRatio = (double)resolution.x / (double)resolution.y;
	UpdateMatrix();

	return;
}

double ProjectionProperties::GetFov() const
{
	return fov;
}

void ProjectionProperties::SetFov(double fov)
{
	this->fov = fov;
	UpdateMatrix();

	return;
}

double ProjectionProperties::GetNearclip() const
{
	return nearclip;
}

void ProjectionProperties::SetNearclip(double nearclip)
{
	this->nearclip = nearclip;
	UpdateMatrix();

	return;
}

double ProjectionProperties::GetFarclip() const
{
	return farclip;
}

long double ProjectionProperties::GetSqrFarclip() const
{
	return sqrFarclip;
}

void ProjectionProperties::SetFarclip(double farclip)
{
	this->farclip = farclip;
	this->sqrFarclip = farclip * farclip;
	UpdateMatrix();

	return;
}

double ProjectionProperties::GetAspectRatio() const
{
	return aspectRatio;
}

const Vector2i& ProjectionProperties::GetResolution() const
{
	return resolution;
}

void ProjectionProperties::SetResolution(const Vector2i& resolution)
{
	this->resolution = resolution;
	this->aspectRatio = (double)resolution.x / (double)resolution.y;
	UpdateMatrix();

	return;
}

const Matrix4x4& ProjectionProperties::GetProjectionMatrix() const
{
	return projectionMatrix;
}

void ProjectionProperties::UpdateMatrix()
{
	const double range = nearclip - farclip;
	const double tanHalfFov = tan((fov / 2.0) * PI / 180.0);

	projectionMatrix.a = 1.0 / (tanHalfFov * aspectRatio);
	projectionMatrix.f = 1.0 / tanHalfFov;
	projectionMatrix.k = (nearclip + farclip) / range;
	projectionMatrix.l = (2.0 * farclip * nearclip) / range;
	projectionMatrix.o = -1;

	return;
}
