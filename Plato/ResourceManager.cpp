#include "ResourceManager.h"
#include "Color.h"

using namespace BMPlib;
using namespace Plato;

Material* ResourceManager::NewMaterial(const std::string& name)
{
	// Name already taken!
	if (materials.find(name) != materials.end())
		throw std::runtime_error("Name already taken!");

	Material* mat = new Material();

	materials.insert(
		std::pair<std::string, Material*>(name, mat)
	);

	return mat;
}

Texture* ResourceManager::NewTexture(const std::string& name, const Vector2i& size)
{
	// Name already taken!
	if (textures.find(name) != textures.end())
		throw std::runtime_error("Name already taken!");


	Texture* text = new Texture(Color::black, size);

	textures.insert(
		std::pair<std::string, Texture*>(name, text)
	);

	return text;
}

Mesh* ResourceManager::NewMesh(const std::string& name)
{
	// Name already taken!
	if (meshes.find(name) != meshes.end())
		throw std::runtime_error("Name already taken!");


	Mesh* mesh = new Mesh();

	meshes.insert(
		std::pair<std::string, Mesh*>(name, mesh)
	);

	return mesh;
}

Texture* ResourceManager::LoadTextureFromBmp(const std::string& name, const std::string& filename)
{
	// Name already taken!
	if (textures.find(name) != textures.end())
		throw std::runtime_error("Name already taken!");

	BMP bmp;

	if (!bmp.Read(filename))
		throw std::runtime_error("Unable to open file " + filename + " for reading");

	bmp.ConvertTo(BMP::COLOR_MODE::RGBA);

	Texture* text = new Texture(Color::green);
	text->GetPixelBuffer().Refit(bmp.GetPixelBuffer(),
		Vector2i(
			(int)bmp.GetWidth(),
			(int)bmp.GetHeight()
		)
	);

	textures.insert(
		std::pair<std::string, Texture*>(name, text)
	);

	return text;
}

Mesh* ResourceManager::LoadMeshFromObj(const std::string& name, const std::string& filename, bool loadMtlFile)
{
	// Name already taken!
	if (meshes.find(name) != meshes.end())
		throw std::runtime_error("Name already taken!");

	Mesh* mesh = new Mesh(OBJParser().ParseObj(filename, loadMtlFile));

	meshes.insert(
		std::pair<std::string, Mesh*>(name, mesh)
	);

	return mesh;
}

Material* ResourceManager::FindMaterial(const std::string& name)
{
	std::unordered_map<std::string, Material*>::iterator found =
		materials.find(name);

	// Nothing found :(
	if (found == materials.end())
		return nullptr;

	return found->second;
}

Texture* ResourceManager::FindTexture(const std::string& name)
{
	std::unordered_map<std::string, Texture*>::iterator found =
		textures.find(name);

	// Nothing found :(
	if (found == textures.end())
		return nullptr;

	return found->second;
}

Mesh* ResourceManager::FindMesh(const std::string& name)
{
	std::unordered_map<std::string, Mesh*>::iterator found =
		meshes.find(name);

	// Nothing found :(
	if (found == meshes.end())
		return nullptr;

	return found->second;
}

Texture* ResourceManager::FindTextureOrLoadFromBmp(const std::string &name, const std::string &filename)
{
    Texture* texture = FindTexture(name);

    if (!texture) {
        texture = LoadTextureFromBmp(name, filename);
    }

    return texture;
}

Mesh* ResourceManager::FindMeshOrLoadFromObj(const std::string &name, const std::string &filename, bool loadMtlFile)
{
    Mesh* mesh = FindMesh(name);

    if (!mesh) {
        mesh = LoadMeshFromObj(name, filename, loadMtlFile);
    }

    return mesh;
}

void ResourceManager::Free()
{
	for (std::pair<std::string, Material*> m : materials)
		delete m.second;

	for (std::pair<std::string, Texture*> t : textures)
		delete t.second;

	for (std::pair<std::string, Mesh*> m : meshes)
		delete m.second;

	materials.clear();
	textures.clear();
	meshes.clear();

	return;
}

std::unordered_map<std::string, Material*> ResourceManager::materials;
std::unordered_map<std::string, Texture*> ResourceManager::textures;
std::unordered_map<std::string, Mesh*> ResourceManager::meshes;
 
