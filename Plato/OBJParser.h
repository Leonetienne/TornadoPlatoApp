#pragma once
#include "Mesh.h"
#include "Material.h"
#include <string>

namespace Plato
{
	/** Wavefront mesh parser
	*/
	class OBJParser
	{
	public:
		//! Will parse a wavefront (.obj) file to a Mesh
        //! If loadMtlFile is true, it will extract the mtl filename from the
        //! obj file, attempt to load it (emit a warning if it doesnt exist),
        //! which creates textures and materials from this mtl, and will assign these materials
        //! to individual faces of the loaded mesh, as defined in the obj file.
		Mesh ParseObj(const std::string& filepath, bool loadMtlFile = false);

	private:
		//! Will interpret any line in a wavefront file
		void InterpretLine(const std::string& line);

		//! Will interpret o-lines in a wavefront file
		void Interpret_o(const std::string& line);

		//! Will interpret v-lines in a wavefront file
		void Interpret_v(const std::string& line);

		//! Will interpret vt-lines in a wavefront file
		void Interpret_vt(const std::string& line);

		//! Will interpret vn-lines in a wavefront file
		void Interpret_vn(const std::string& line);

		//! Will interpret f-lines in a wavefront file
		void Interpret_f(const std::string& line);

		//! Will interpret usemtl-lines in a wavefront file
		void Interpret_usemtl(const std::string& line);

		//! Will interpret mtllib-lines in a wavefront file
		void Interpret_mtllib(const std::string& line);

		//! Will clean a submesh, f.e. add placeholder values for unsupplied values
		void CleanSubmesh();

		//! Will stash the current submesh, and provice a new one in its place
		void NextSubmesh();

		//! Will combine all submeshes to a single submesh
		Mesh AssembleSubmeshes();

		//! Will reset this object, so it can parse another file
		void Reset();

		//! Will traverse string 'line', and splits it by space characters.
		std::vector<std::string> CollectSpaceSeperatedParameters(const std::string& line, std::size_t begin, std::size_t reseve_size = 0);

		std::vector<Mesh> submeshes;
		Mesh curSubmesh;
        Material* currentMaterial = nullptr;
        bool loadMtl = false;
        std::string curObjFilePath;
	};

}
