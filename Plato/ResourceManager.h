#pragma once
#include <unordered_map>
#include "../Tornado/Texture.h"
#include "../Tornado/Material.h"
#include "Mesh.h"
#include "bmplib.h"
#include "OBJParser.h"

class ResourceManager
{
public:
	static Material* NewMaterial(const std::string& name);
	static Texture* NewTexture(const std::string& name, const Vector2i& size);
	static Mesh* NewMesh(const std::string& name);

	static Texture* LoadTextureFromBmp(const std::string& name, const std::string& filename);
	static Mesh* LoadMeshFromObj(const std::string& name, const std::string& filename);

	static Material* FindMaterial(const std::string& name);
	static Texture* FindTexture(const std::string& name);
	static Mesh* FindMesh(const std::string& name);

	static void Free();

private:
	static std::unordered_map<std::string, Material*> materials;
	static std::unordered_map<std::string, Texture*> textures;
	static std::unordered_map<std::string, Mesh*> meshes;

	//  No instanciation! >:(
	ResourceManager();
};
