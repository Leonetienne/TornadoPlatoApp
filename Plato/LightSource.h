#pragma once
#include "Component.h"
#include "Collider.h"
#include "Color.h"
#include "../Tornado/RenderLightSource.h"

namespace Plato
{
	namespace Components
	{
		/** This component is the abstract implementation of all LightSources.
		* You can't instanciate it on its own, and even if you did, bad things would happen. Don't do it.
		*/
		class LightSource : public Component
		{
		public:
			//! Will return a reference to the color
			virtual Color& GetColor();

			//! Will return a read-only reference to the color
			virtual const Color& GetColor() const;

			//! Will set the color
			virtual void SetColor(const Color& col);

			//! Will return the light intensity
			virtual const double GetIntensity() const;

			//! Will set the light intensity
			virtual void SetIntensity(double intensity);

			//! Will return the light softness
			virtual const double GetSoftness() const;

			//! Will set this lights softness. Valid range: \f$0 <= soft <= 1\f$
			virtual void SetSoftness(const double softness) const;

			//! Will set wether or not this lightsource uses domains.  
			//! Domains are Collider objects that will restrict where the light will be rendered.
			void SetUseDomains(bool useDomains);

			//! Will return wether or not this lightsource uses domains.  
			//! Domains are Collider objects that will restrict where the light will be rendered.
			bool GetUseDomains() const;

			//! Will return a vector of the lighting domains used.  
			//! Domains are Collider objects that will restrict where the light will be rendered.
			std::vector<PCollider*>& GetDomains();
			//! Will return a vector of the lighting domains used.  
			//! Domains are Collider objects that will restrict where the light will be rendered.
			const std::vector<PCollider*>& GetDomains() const;

			//! Will return the tornado render light source. This does NOT include transformation!
			virtual TorGL::RenderLightSource* GetRawTornadoRenderLightSource() const = 0;

		protected:
			mutable TorGL::RenderLightSource* tornadoLightSource; // <-- Muteable so that the Renderer can access the position member

			friend class WorldObject;
			LightSource(WorldObject* worldObject, double intensity, const Color& color = Color::white);

		private:
			void LateUpdate(double frameTime) override;
			void Render(Renderer* renderer) override;

			std::vector<PCollider*> lightDomains;

		};
	}
}
