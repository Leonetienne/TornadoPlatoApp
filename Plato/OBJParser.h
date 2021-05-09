#pragma once
#include <string>
#include "Mesh.h"

/** Wavefront mesh parser
*/
class OBJParser
{
public:
	//! Will parse a wavefront (.obj) file to a Mesh
	Mesh ParseObj(const std::string& filepath);

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
};

