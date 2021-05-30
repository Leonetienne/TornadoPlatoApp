#include "ProjectionProperties.h"

using namespace TorGL;

ProjectionProperties::ProjectionProperties(const Vector2i& resolution, double fov, double nearclip, double farclip)
	:
	fov { fov },
	nearclip { nearclip },
	farclip { farclip },
	sqrFarclip { farclip * farclip },
	resolution{ resolution }
{
	this->halfResolution = Vector2d(resolution.x / 2.0, resolution.y / 2.0);
	this->aspectRatio = (double)resolution.x / (double)resolution.y;

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

const Vector2d& ProjectionProperties::GetHalfResolution() const
{
	return halfResolution;
}

void ProjectionProperties::SetResolution(const Vector2i& resolution)
{
	this->resolution = resolution;
	this->aspectRatio = (double)resolution.x / (double)resolution.y;
	this->halfResolution = Vector2d(resolution.x / 2.0, resolution.y / 2.0);
	UpdateMatrix();

	return;
}

const Matrix4x4& ProjectionProperties::GetProjectionMatrix() const
{
	return projectionMatrix;
}

// Source: http://www.songho.ca/opengl/gl_projectionmatrix.html
void ProjectionProperties::UpdateMatrix()
{
	const double n = nearclip;
	const double f = farclip;
	const double range = f - n;
	const double tanHalfFov = tan(fov * 0.5 * Deg2Rad);
	const double r = tanHalfFov * aspectRatio;
	const double t = tanHalfFov;

	projectionMatrix.a = 1.0 / r;
	projectionMatrix.f = 1.0 / t;
	projectionMatrix.k = -(f + n) / range;
	projectionMatrix.l = (-2.0 * f * n) / range;
	projectionMatrix.o = -1;

	return;
}
