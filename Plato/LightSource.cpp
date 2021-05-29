#include "LightSource.h"
#include "Renderer.h"

LightSource::LightSource(WorldObject* worldObject, double intensity, const Color& color) :
	Component(worldObject),
	tornadoLightSource { nullptr }
{

	return;
}

Color& LightSource::GetColor()
{
	return tornadoLightSource->GetColor();
}

const Color& LightSource::GetColor() const
{
	return tornadoLightSource->GetColor();
}

void LightSource::SetColor(const Color& col)
{
	tornadoLightSource->SetColor(col);
	return;
}

const double LightSource::GetIntensity() const
{
	return tornadoLightSource->GetIntensity();
}

void LightSource::SetIntensity(double intensity)
{
	tornadoLightSource->SetIntensity(intensity);
	return;
}

const double LightSource::GetSoftness() const
{
	return tornadoLightSource->GetSoftness();
}

void LightSource::SetSoftness(const double softness) const
{
	// Softness gets clamped to 0 < soft < 1 in the tornado SetSoftness setter
	tornadoLightSource->SetSoftness(softness);
	return;
}

void LightSource::SetUseDomains(bool useDomains)
{
	tornadoLightSource->SetUseDomains(useDomains);
	return;
}

bool LightSource::GetUseDomains() const
{
	return tornadoLightSource->GetUseDomains();
}

std::vector<PCollider*>& LightSource::GetDomains()
{
	return lightDomains;
}

const std::vector<PCollider*>& LightSource::GetDomains() const
{
	return lightDomains;
}

void LightSource::LateUpdate(double frameTime)
{
	// Update light source camera space position
	tornadoLightSource->SetPosition(
		Camera::GetMainCamera()->WorldSpaceToCameraSpace(transform->GetGlobalPosition())
	);

	// Update light source domains, if in use
	if (tornadoLightSource->GetUseDomains())
	{
		tornadoLightSource->GetDomains().clear();
		for (const PCollider* domain : lightDomains)
			tornadoLightSource->GetDomains().push_back(domain->CameraSpaceColldier());
	}

	return;
}

void LightSource::Render(Renderer* renderer)
{
	renderer->RegisterLightSource(this);
	return;
}
