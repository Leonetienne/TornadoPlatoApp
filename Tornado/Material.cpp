#include "Material.h"

using namespace TorGL;

Material::Material()
{
}

Material::Material(Texture* texture)
	:
	texture { texture }
{
	return;
}
