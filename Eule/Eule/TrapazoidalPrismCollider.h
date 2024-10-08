#pragma once
#include "Vector3.h"
#include "Collider.h"
#include <array>

namespace Eule
{
	/** A collider describing a trapazoidal prism.
	* A trapazoidal prism is basically a box, but each vertex can be manipulated individually, altering
	* the angles between faces.
	* Distorting a 2d face into 3d space will result in undefined behaviour. Each face should stay flat, relative to itself. This shape is based on QUADS!
	*/
	class TrapazoidalPrismCollider : public Collider
	{
	public:
		TrapazoidalPrismCollider();
		TrapazoidalPrismCollider(const TrapazoidalPrismCollider& other) = default;
		TrapazoidalPrismCollider(TrapazoidalPrismCollider&& other) noexcept = default;
		void operator=(const TrapazoidalPrismCollider& other);
		void operator=(TrapazoidalPrismCollider&& other) noexcept;

		//! Will return a specific vertex
		const Vector3d& GetVertex(std::size_t index) const;

		//! Will set the value of a specific vertex
		void SetVertex(std::size_t index, const Vector3d value);

		//! Tests, if this Collider contains a point
		bool Contains(const Vector3d& point) const override;

		/* Vertex identifiers */
		static constexpr std::size_t BACK = 0;
		static constexpr std::size_t FRONT = 4;
		static constexpr std::size_t LEFT = 0;
		static constexpr std::size_t RIGHT = 2;
		static constexpr std::size_t BOTTOM = 0;
		static constexpr std::size_t TOP = 1;

	private:
		enum class FACE_NORMALS : std::size_t;

		//! Will calculate the vertex normals from vertices
		void GenerateNormalsFromVertices();

		//! Returns the dot product of a given point against a specific plane of the bounding box
		double FaceDot(FACE_NORMALS face, const Vector3d& point) const;

		std::array<Vector3d, 8> vertices;


		// Face normals
		enum class FACE_NORMALS : std::size_t
		{
			LEFT = 0,
			RIGHT = 1,
			FRONT = 2,
			BACK = 3,
			TOP = 4,
			BOTTOM = 5
		};
		std::array<Vector3d, 6> faceNormals;
	};
}
