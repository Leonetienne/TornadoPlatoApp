#pragma once
#include "Color.h"
#include "Vector3.h"
#include "InterRenderTriangle.h"
#include "BoundingBox.h"
#include <vector>

// zxy
struct Box
{
	Vector3d flt; // front left top
	Vector3d flb; // front left bottom
	Vector3d frt; // front right top
	Vector3d frb; // front right bottom
	Vector3d blt; // back left top
	Vector3d blb; // back left bottom
	Vector3d brt; // back right top
	Vector3d brb; // back right bottom

	Vector3d nm_l; // normal left
	Vector3d nm_r; // normal right
	Vector3d nm_t; // normal top
	Vector3d nm_bo; // normal bottom
	Vector3d nm_f; // normal front
	Vector3d nm_ba; // normal back

	void GenerateNormalsFromVertices()
	{
		nm_l = (blb - flb).CrossProduct(flt - flb);
		nm_r = (frt - frb).CrossProduct(brb - frb);
		nm_f = (flt - flb).CrossProduct(frb - flb);
		nm_ba = (brb - blb).CrossProduct(blt - blb);
		nm_t = (blt - flt).CrossProduct(frt - flt);
		nm_bo = (frb - flb).CrossProduct(blb - flb);
		return;
	}
};


/** Abstract implementation of all light sources within Tornado.
* You can't instanciate it on its own, and even if you could, bad things would happen. Don't do it.
*/
class RenderLightSource
{
public:
	//! Will return color multipliers/factors (r,g,b only) for this light source, given an InterRenderTriangle, a normal, and a worldspace point.  
	//! Multiply the raw color values with these factors to get the shaded color (for this light)
	virtual Color GetColorIntensityFactors(const InterRenderTriangle* ird, const Vector3d& point, const Vector3d& normal) const = 0;
	
	//! Will set this lightsources color
	void SetColor(const Color& color);

	//! Will return this lightsources color
	Color& GetColor();
	//! Will return this lightsources color
	const Color& GetColor() const;

	//! Will set this lightsources intensity
	void SetIntensity(const double intensity);

	//! Will return this lightsources intensity
	double GetIntensity() const;

	//! Will set this lightsources softness. Valid range: \f$0 <= soft <= 1\f$
	void SetSoftness(const double softness);

	//! Will return this lightsources softness
	double GetSoftness() const;

	//! Will set this lightsources 3d position
	void SetPosition(const Vector3d& position);

	//! Will return this lightsources 3d position
	Vector3d& GetPosition();
	//! Will return this lightsources 3d position
	const Vector3d& GetPosition() const;
	
	//! Will set wether or not this lightsource uses bounding boxes
	void SetUseBoundingBox(bool useBoundingBox);

	//! Will return wether or not this lightsource uses bounding boxes
	bool GetUseBoundingBox();

	//! Will return a vector of the bounding boxes used
	std::vector<BoundingBox>& GetBoundingBoxes();
	//! Will return a vector of the bounding boxes used
	const std::vector<BoundingBox>& GetBoundingBoxes() const;

protected:

	bool DoBoundingBoxesContainPoint(const Vector3d& point) const;

	Color color;
	double intensity = 10;
	double intensityTimes255 = 0; //! intensity * 255.0  - gets calculated in SetIntensity()
	double softness = 0;
	bool useBoundingBox = false;
	std::vector<BoundingBox> boundingBoxes;
	Vector3d position;
};
