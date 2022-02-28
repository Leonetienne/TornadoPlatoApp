#pragma once
#include "Vector2.h"
#include "Matrix4x4.h"
#include <Eule/Eule/Constants.h>

namespace TorGL
{
	/** This is a generator for a projection matrix, if you will.
	* Offers a nice, and easy to use interface. Just pass this object as-is to the rendering engine.
	*/
	class ProjectionProperties
	{
	public:
		ProjectionProperties(const Vector2i& resolution, double fov, double nearclip, double farclip);

		//! Will return the current field of view in degrees
		double GetFov() const;
		//! Will set the field of view in degrees
		void SetFov(double fov);

		//! Will return the current nearclip
		double GetNearclip() const;
		//! Will set the nearclip
		void SetNearclip(double nearclip);

		//! Will return the current farclip
		double GetFarclip() const;
		//! Will return the square of the current farclip.
		//! Compare this value to the magnitude of a vector for a cheap distance check.
		//! This value is cached, and thus faster than just squaring it yourself.
		long double GetSqrFarclip() const;
		//! Will set the current farclip
		void SetFarclip(double farclip);

		//! Will return the current aspect ratio \f$x/y\f$
		double GetAspectRatio() const;

		//! Will return the current rendering resolution
		const Vector2i& GetResolution() const;
		//! Will return half of the current rendering resolution.
		//! This value is cached, and thus faster than just halving it yourself.
		const Vector2d& GetHalfResolution() const;
		//! This will set the rendering resolution
		void SetResolution(const Vector2i& resolution);

		//! Returns the current projection matrix.
		//! This value is cached and cheap to call.
		const Matrix4x4& GetProjectionMatrix() const;

	private:
		//! Will recalculate the projection matrix
		void UpdateMatrix();

		double fov;
		double nearclip;
		double farclip;
		long double sqrFarclip;
		double aspectRatio;
		Vector2i resolution;
		Vector2d halfResolution;
		Matrix4x4 projectionMatrix;
	};
}
