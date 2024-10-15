#pragma once
#include "../Plato/Scene.h"
#include "../Plato/Mesh.h"
#include "../Plato/Material.h"
#include "../Plato/WorldObject.h"
#include "../Plato/Texture.h"

class Test__TextureProjectionWorks : public Plato::Scene
{
public:
	Test__TextureProjectionWorks();

	void Update(double frametime);

private:
    Plato::Mesh* mesh_coob;
	Plato::WorldObject* wo_coob;
	Plato::Texture* txt_coob;
	Plato::Material* mat_coob;
};
