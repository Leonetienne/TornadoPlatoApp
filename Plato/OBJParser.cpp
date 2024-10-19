#include "OBJParser.h"
#include "MTLParser.h"
#include "Util.h"
#include "ResourceManager.h"
#include <filesystem>
#include <iostream>
#include <sstream>

using namespace Plato;

Mesh OBJParser::ParseObj(
    const std::string& filepath,
    bool loadMtlFile,
    const std::string& mtlResourceNamePrefix
)
{
    this->loadMtl = loadMtlFile;
    this->mtlResourceNamePrefix = mtlResourceNamePrefix;

	std::stringstream ss(Util::ReadFile(filepath));
    curObjFilePath = filepath;

	std::string line;
	while (std::getline(ss, line))
		InterpretLine(line);

	Mesh mesh = AssembleSubmeshes();
	Reset();
	return mesh;
}

void OBJParser::InterpretLine(const std::string& line)
{
	// Ignore blank lines
	if (line.length() == 0)
		return;

	// Ignore commments
	else if (line[0] == '#')
		return;

	// Quick-forward to o (object name)
	else if (line[0] == 'o')
		return Interpret_o(line);

	// Quick-forward to f (face)
	else if (line[0] == 'f')
		return Interpret_f(line);

	if (line.length() < 2)
		return;


	const std::string linetype = line.substr(0, 2);

	// Forward to vn (vertex-normal)
	if (linetype == "v ") // <-- Whitespace intended
		return Interpret_v(line);

	// Forward to vt (vertex-texture)
	else if (linetype == "vt")
		return Interpret_vt(line);

	// Forward to vn (vertex-normal)
	else if (linetype == "vn")
		return Interpret_vn(line);

    // MTL-Specifica
    if (loadMtl) {
        // Line could be usemtl line
        const std::string usemtl = "usemtl";
        if (line.length() > usemtl.length()) {
            const std::string linetype = line.substr(0, usemtl.length());
            if (linetype == usemtl) {
                return Interpret_usemtl(line);
            }
        }

        // Line could be mtllib line
        const std::string mtllib = "mtllib";
        if (line.length() > mtllib.length()) {
            const std::string linetype = line.substr(0, mtllib.length());
            if (linetype == mtllib) {
                return Interpret_mtllib(line);
            }
        }
    }

	return;
}

void OBJParser::Interpret_o(const std::string& line)
{
	// Initiates a new object

	// Minimal feasible line length is 3
	if (line.length() < 3)
		return;
	
	// unused
	// const std::string objectName = line.substr(2, line.length() - 2);

	NextSubmesh();

	return;
}

void OBJParser::Interpret_v(const std::string& line)
{
	// Declares a 3d vertex

	// Minimal feasible line length is 7
	if (line.length() < 7)
		return;

	// Collect arguments here
	std::vector<std::string> params = CollectSpaceSeperatedParameters(line, 2, 3);

	// Check argument count
	if (params.size() != 3)
		throw std::runtime_error("Wavefront file syntax error! v argument-count mismatch!");

	// Convert arguments to double
	Vector3d v;
	try {
		v = Vector3d(
			std::stod(params[0]),
			std::stod(params[1]),
			std::stod(params[2])
		);
	}
	catch (std::runtime_error)
	{
		throw std::runtime_error("Wavefront file syntax error! std::stod failure in v");
	}

	// Save arguments
	curSubmesh.v_vertices.push_back(v);
	//std::cout << "v: " << v << std::endl;

	return;
}

void OBJParser::Interpret_vt(const std::string& line)
{
	// Declares a texture vertex

	// Minimal feasible line length is 6
	if (line.length() < 6)
		return;

	// Collect arguments here
	std::vector<std::string> params = CollectSpaceSeperatedParameters(line, 3, 2);

	// Check argument count
	if (params.size() != 2)
		throw std::runtime_error("Wavefront file syntax error! vt argument-count mismatch!");

	// Convert arguments to double
	Vector2d v;
	try {
		v = Vector2d(
			std::stod(params[0]),
			std::stod(params[1])
		);
	}
	catch (std::logic_error)
	{
		throw std::runtime_error("Wavefront file syntax error! std::stod failure in vt");
	}

	// Save arguments
	curSubmesh.uv_vertices.push_back(v);
	//std::cout << "vt: " << v << std::endl;

	return;
}

void OBJParser::Interpret_vn(const std::string& line)
{
	// Declares a normal vertex

	// Minimal feasible line length is 8
	if (line.length() < 8)
		return;

	// Collect arguments here
	std::vector<std::string> params = CollectSpaceSeperatedParameters(line, 3, 3);

	// Check argument count
	if (params.size() != 3)
		throw std::runtime_error("Wavefront file syntax error! vn argument-count mismatch!");

	// Convert arguments to double
	Vector3d v;
	try
	{
		v = Vector3d(
			std::stod(params[0]),
			std::stod(params[1]),
			std::stod(params[2])
		);
	}
	catch (std::logic_error)
	{
		throw std::runtime_error("Wavefront file syntax error! std::stod failure in vn");
	}

	// Save arguments
	curSubmesh.normals.push_back(v);
	//std::cout << "vn: " << v << std::endl;

	return;
}

void OBJParser::Interpret_f(const std::string& line)
{
	// Declares vertex-indices for a face

	// Minimal feasible line length is 7
	if (line.length() < 7)
		return;

	// Collect arguments here
	std::vector<std::string> params = CollectSpaceSeperatedParameters(line, 2, 3);

	// Prepent an '/' to the end of each arg. Helps parsing later.
	for (std::string& fArg : params)
		fArg += "/";

	// Check argument count
	if (params.size() != 3)
		throw std::runtime_error("Wavefront file syntax error! f argument-count mismatch!");

	// Parse arguments
	for (const std::string& fParam : params)
	{
		// if npos, value is not given
		MeshVertexIndices newVertexIndices;

		newVertexIndices.v  = 0;
		newVertexIndices.uv = 0;
		newVertexIndices.vn = 0;

		std::stringstream ss;
		// How often we've seen a '/'
		std::size_t slash_count = 0;
		for (std::size_t i = 0; i < fParam.length(); i++)
		{
			const char& c = fParam[i];

			if (c != '/')
			{
				ss << c;
			}
			else
			{
				if (ss.str().length() > 0)
					try
					{
						switch (slash_count)
						{
						case 0: // first position is v
							newVertexIndices.v = std::stoul(ss.str()) - 1; // wavefront indices start at 1
							break;
						case 1: // second position is vt/uv
							newVertexIndices.uv = std::stoul(ss.str()) - 1;
							break;
						case 2: // third position is vn
							newVertexIndices.vn = std::stoul(ss.str()) - 1;
							break;
						default:
							throw std::runtime_error("Wavefront file syntax error! f-segment argument-count mismatch!");
						}
						ss.str("");
					}
					catch (std::logic_error)
					{
						//std::cerr << "Exception details: " << ss.str() << std::endl;
						throw std::runtime_error("Wavefront file syntax error! std::stoul failure in f");
					}

				slash_count++;
			}
		}

		curSubmesh.tris.push_back(newVertexIndices);
        // If we are interpreting tris materials, and have a material, push it back
        if (loadMtl && currentMaterial) {
            curSubmesh.trisMaterialIndices[curSubmesh.tris.size()-1] = currentMaterial;
        }
		//std::cout << "f: " << Vector3i(newVertexIndices.v, newVertexIndices.uv, newVertexIndices.vn) << std::endl;
	}

	return;
}

void OBJParser::Interpret_usemtl(const std::string& line)
{
    // Extract material name
    const std::string materialName = MTLParser::DeriveMaterialName(mtlResourceNamePrefix, line.substr(std::string("newmtl ").length()));

    // Fetch the material to use for coming faces
    currentMaterial = ResourceManager::FindMaterial(materialName);
}

void OBJParser::Interpret_mtllib(const std::string& line)
{
    // Extract mtl filename (it is the relative file path to the directory of the obj file)
    const std::string mtlFilePathRelativeToObj = line.substr(std::string("mtllib ").length());

    // Derive MTL file path
    const std::string mtlFilePath = Util::FilePathToDirPath(curObjFilePath) + "/" + mtlFilePathRelativeToObj;

    // Does it exist?
    if (!std::filesystem::exists(mtlFilePath)) {
        std::cerr << "[WARNING] [OBJParser]: Attempted to load mtl file \""
            << mtlFilePath
            << "\" for obj file \""
            << curObjFilePath
            << "\" but it does not exist!"
            << std::endl;
        return;
    }

    // Load the mtl file
    // Derive the directory path of the mtl file
    std::string textureBasePath = Util::FilePathToDirPath(mtlFilePath) + "/";

    try {
        (MTLParser()).ParseMtl(mtlFilePath, textureBasePath, mtlResourceNamePrefix);
    }
    catch (std::runtime_error &e) {
        std::cerr << "[WARNING] [OBJParser]: An exception occured while reading mtl file \""
            << mtlFilePath
            << "\" for obj file \""
            << curObjFilePath
            << "\": "
            << e.what()
            << "... Ignoring the entire MTL for this mesh!!!"
            << std::endl;

        // Don't assign mtl-defined materials for this mesh anymore!
        this->loadMtl = false;
        return;
    }
}

void OBJParser::CleanSubmesh()
{
	// This is just to prevent vector-subscript-out-of-ranges

	return;
	if (curSubmesh.normals.size() == 0)
		curSubmesh.normals.push_back(Vector3d::one);

	if (curSubmesh.uv_vertices.size() == 0)
		curSubmesh.uv_vertices.push_back(Vector2d::zero);

	// No 3d-vertices?! lolz
	if (curSubmesh.v_vertices.size() == 0)
		curSubmesh.v_vertices.push_back(Vector2d::zero);

	return;
}

void OBJParser::NextSubmesh()
{
	CleanSubmesh();
	submeshes.push_back(curSubmesh);
	curSubmesh = Mesh();

	return;
}

Mesh OBJParser::AssembleSubmeshes()
{
	// Insert the last worked on submesh into the submesh vector
	NextSubmesh();

	// Common-case shortcut: File contains only one mesh
	//if (submeshes.size() == 2)
	//	return submeshes[1];

	// Else: Merge all submeshes into one single mesh

	Mesh toRet;

	// Reserve memory in new mesh
	{
		// Initialize counters
		std::size_t numTotal_v = 0;
		std::size_t numTotal_uv = 0;
		std::size_t numTotal_vn = 0;
		std::size_t numTotal_tris = 0;

		// Count how much we need
		for (const Mesh& submesh : submeshes)
		{
			numTotal_v += submesh.v_vertices.size();
			numTotal_uv += submesh.uv_vertices.size();
			numTotal_vn += submesh.normals.size();
			numTotal_tris += submesh.tris.size();
		}

		// Reserve it
		toRet.v_vertices.reserve(numTotal_v);
		toRet.uv_vertices.reserve(numTotal_uv);
		toRet.normals.reserve(numTotal_vn);
		toRet.tris.reserve(numTotal_tris);

        if (loadMtl) {
		    toRet.trisMaterialIndices.reserve(numTotal_tris);
        }
	}

    std::size_t currentTrisOffset = 0;
	for (const Mesh& submesh : submeshes)
	{
		if (submesh.v_vertices.size() == 0)
			continue;

		// Merge submesh vertex vectors into main mesh vectors
		toRet.v_vertices.insert(
			toRet.v_vertices.end(),
			submesh.v_vertices.begin(),
			submesh.v_vertices.end()
		);

		toRet.uv_vertices.insert(
			toRet.uv_vertices.end(),
			submesh.uv_vertices.begin(),
			submesh.uv_vertices.end()
		);

		toRet.normals.insert(
			toRet.normals.end(),
			submesh.normals.begin(),
			submesh.normals.end()
		);

        // Have to add tris material indices manually
        if (loadMtl) {
            for (auto it : submesh.trisMaterialIndices) {
                toRet.trisMaterialIndices[it.first + currentTrisOffset] = it.second;
            }
            currentTrisOffset += submesh.tris.size();
        }


		// Merge offset-adjusted tris indices into main mesh tris vector
		for (const MeshVertexIndices& mvi : submesh.tris)
			toRet.tris.push_back(
				{
					mvi.v,
					mvi.uv,
					mvi.vn,
				}
			);
	}
	
	return toRet;
}

void OBJParser::Reset()
{
	submeshes.clear();
	curSubmesh = Mesh();

	return;
}

std::vector<std::string> OBJParser::CollectSpaceSeperatedParameters(const std::string& line, std::size_t begin, std::size_t reserve_size)
{
	std::vector<std::string> params;
	if (reserve_size > 0)
		params.reserve(reserve_size);

	// Seperate arguments
	std::stringstream ss;
	for (std::size_t i = begin; i < line.length(); i++)
	{
		const char& c = line[i];

		if (c != ' ')
			ss << c;
		else
		{
			params.push_back(ss.str());
			ss.str("");
		}
	}
	params.push_back(ss.str());

	return params;
}
