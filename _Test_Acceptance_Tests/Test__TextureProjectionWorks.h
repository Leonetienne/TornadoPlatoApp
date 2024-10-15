#pragma once
#include "TestFixture.h"
#include "../Plato/MeshRenderer.h"
#include "../Plato/Material.h"
#include "../Plato/Texture.h"

class Test__TextureProjectionWorks : public TestFixture
{
public:
	Test__TextureProjectionWorks();

	void Update(double frametime);

private:
	Mesh mesh_coob;
	WorldObject* wo_coob;
	Texture txt_coob;
	Material mat_coob;
};