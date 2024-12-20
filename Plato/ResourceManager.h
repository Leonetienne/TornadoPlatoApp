#pragma once
#include <unordered_map>
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "bmplib.h"
#include "OBJParser.h"

namespace Plato
{
	/** Responsible for managing resources such as meshes, textures, materials, etc...
	*/
	class ResourceManager
	{
	public:
		//! Will create a new empty material.  
		//! Exception if name already taken
		static Material* NewMaterial(const std::string& name);

		//! Will create a new empty texture, given a size.  
		//! Exception if name already taken
		static Texture* NewTexture(const std::string& name, const Vector2i& size);

		//! Will create a new empty mesh  
		//! Exception if name already taken
		static Mesh* NewMesh(const std::string& name);



		//! Will attempt to load a texture from a bmp file
		static Texture* LoadTextureFromBmp(const std::string& name, const std::string& filename);

		//! Will attempt to load a mesh from a wavefront (.obj) file
        //! If loadMtlFile is true, it will extract the mtl filename from the
        //! obj file, attempt to load it (emit a warning if it doesnt exist),
        //! which creates textures and materials from this mtl, and will assign these materials
        //! to individual faces of the loaded mesh, as defined in the obj file.
		static Mesh* LoadMeshFromObj(const std::string& name, const std::string& filename, bool loadMtlFile = false);



		//! Will search for a material and return it.  
		//! Nullptr if not found
		static Material* FindMaterial(const std::string& name);

		//! Will search for a texture and return it.  
		//! Nullptr if not found
		static Texture* FindTexture(const std::string& name);

		//! Will search for a mesh and return it.  
		//! Nullptr if not found
		static Mesh* FindMesh(const std::string& name);

		//! Will search for a texture and return it.  
        //! Will load and create it if not found
        static Texture* FindTextureOrLoadFromBmp(const std::string& name, const std::string& filename);

		//! Will search for a mesh and return it.  
        //! Will load and create it if not found
        //! If loadMtlFile is true, it will extract the mtl filename from the
        //! obj file, attempt to load it (emit a warning if it doesnt exist),
        //! which creates textures and materials from this mtl, and will assign these materials
        //! to individual faces of the loaded mesh, as defined in the obj file.
        static Mesh* FindMeshOrLoadFromObj(const std::string& name, const std::string& filename, bool loadMtlFile = false);

		static void Free();

	private:
		static std::unordered_map<std::string, Material*> materials;
		static std::unordered_map<std::string, Texture*> textures;
		static std::unordered_map<std::string, Mesh*> meshes;

		//  No instanciation! >:(
		ResourceManager();
	};
}
