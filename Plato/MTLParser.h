#pragma once
#include "Material.h"
#include <string>
#include <vector>

/*
*   VERY EXPERIMENTAL!!
*/

namespace Plato
{
	/** Wavefront material parser
     * It really just loads textures, lol
	*/
	class MTLParser
	{
	public:
        //! Will parse a wavefront (.mtl) file to a Mesh
        //! The textures and materials will be available in the ResourceManager afterwards,
        //! but it will still return a vector of them
        std::vector<Material*> ParseMtl(const std::string& filepath, const std::string& textureBasePath, const std::string& resourceNamePrefix);

        static std::string DeriveMaterialName(const std::string& resourceNamePrefix, const std::string& materialName);

	private:
		//! Will interpret any line in a wavefront file
		void InterpretLine(const std::string& line);

		//! Will interpret newmtl-lines in a wavefront file
		void Interpret_newmtl(const std::string& line);

		//! Will interpret map_Kd-lines in a wavefront file
		void Interpret_map_Kd(const std::string& line);

		//! Will create a new material from the aquired cache
		void PushMaterial();

		//! Will reset this object, so it can parse another file
		void Reset();

		std::vector<Material*> materials;
        Material* currentMaterial = nullptr;
        std::string currentMaterialName = "";
        std::string textureBasePath = "";
        std::string curMatTexturePath = "";
        std::string resourceNamePrefix = "";
	};

}
