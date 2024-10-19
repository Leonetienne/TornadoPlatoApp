#include "MTLParser.h"
#include "ResourceManager.h"
#include "Util.h"
#include <sstream>

/*
*   VERY EXPERIMENTAL!!
*/

using namespace Plato;

 std::vector<Material*> MTLParser::ParseMtl(
    const std::string& filepath,
    const std::string& textureBasePath,
    const std::string& resourceNamePrefix
)
{
    this->textureBasePath = textureBasePath;
    this->resourceNamePrefix = resourceNamePrefix;

	std::stringstream ss(Util::ReadFile(filepath));

	std::string line;
	while (std::getline(ss, line))
		InterpretLine(line);

    std::vector<Material*> toRet = materials;
	Reset();
    return toRet;
}

void MTLParser::InterpretLine(const std::string& line)
{
	// Ignore blank lines
	if (line.length() == 0)
		return;

	// Ignore commments
	else if (line[0] == '#')
		return;

    // Line could be newmtl line
    const std::string newmtl = "newmtl";
    if (line.length() > newmtl.length()) {
	    const std::string linetype = line.substr(0, newmtl.length());
        if (linetype == newmtl) {
            return Interpret_newmtl(line);
        }
    }

    // Line could be map_Kd line
    const std::string mapkd = "map_Kd";
    if (line.length() > mapkd.length()) {
	    const std::string linetype = line.substr(0, mapkd.length());
        if (linetype == mapkd) {
            return Interpret_map_Kd(line);
        }
    }

	return;
}

void MTLParser::Interpret_newmtl(const std::string& line)
{
    // If we currently have a material, finish it up
    if (currentMaterial != nullptr) {
        PushMaterial();
    }

    // Extract material name
    const std::string materialName = DeriveMaterialName(resourceNamePrefix, line.substr(std::string("newmtl ").length()));

    // Create material
    currentMaterial = ResourceManager::NewMaterial(materialName);
    currentMaterialName = materialName;
}

void MTLParser::Interpret_map_Kd(const std::string& line)
{
    // Extract texture file path
    const std::string texturePath = line.substr(std::string("map_Kd ").length());
    const std::string combinedPath = textureBasePath + '/' + texturePath;

    // Derive texture resource name
    // The materialName already contains the resourceNamePrefix, and the currentMaterialName,
    // so it is most likely unique (given that the mesh name is unique). 
    const std::string textureName = currentMaterialName + "--" + "colormap";

    // Load it
    Texture* texture = ResourceManager::FindTextureOrLoadFromBmp(
        textureName,
        combinedPath
    );

    // Bind it to the material
    currentMaterial->texture = texture;
}

void MTLParser::PushMaterial()
{
    materials.push_back(currentMaterial)	;
}

void MTLParser::Reset()
{
    materials.clear();
    textureBasePath = "";
    currentMaterial = nullptr;
    currentMaterialName = "";
}

std::string MTLParser::DeriveMaterialName(
    const std::string& resourceNamePrefix,
    const std::string& materialName
) {
    return std::string("mtl-generated--") +
          resourceNamePrefix + "--" +
          materialName;
}

