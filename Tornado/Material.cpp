#include "Material.h"

Material::Material()
{
}

Material::Material(Texture* texture)
	:
	texture { texture }
{
	return;
}
