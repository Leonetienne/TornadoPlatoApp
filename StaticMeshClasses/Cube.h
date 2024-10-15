#pragma once
#include "../Plato/Mesh.h"

using namespace Plato;

class Cube : public Mesh
{
public:
	Cube()
	{
		v_vertices = {
			Vector3d(-1, -1,  1),
			Vector3d(-1,  1,  1),
			Vector3d(-1, -1, -1),
			Vector3d(-1,  1, -1),
			Vector3d(1, -1,  1),
			Vector3d(1,  1,  1),
			Vector3d(1, -1, -1),
			Vector3d(1,  1, -1)
		};

		uv_vertices = {
			Vector2d(0.625, 0.00),
			Vector2d(0.375, 0.25),
			Vector2d(0.375, 0.00),
			Vector2d(0.625, 0.25),
			Vector2d(0.375, 0.50),
			Vector2d(0.625, 0.50),
			Vector2d(0.375, 0.75),
			Vector2d(0.625, 0.75),
			Vector2d(0.375, 1.00),
			Vector2d(0.125, 0.75),
			Vector2d(0.125, 0.50),
			Vector2d(0.875, 0.50),
			Vector2d(0.625, 1.00),
			Vector2d(0.875, 0.75)
		};

		normals = {
			Vector3d(-1,  0,  0),
			Vector3d(0,  0, -1),
			Vector3d(1,  0,  0),
			Vector3d(0,  0,  1),
			Vector3d(0, -1,  0),
			Vector3d(0,  1,  0)
		};

		tris = {
			{1,  0, 0}, { 2,  1, 0}, {0,  2, 0},
			{3,  3, 1}, { 6,  4, 1}, {2,  1, 1},
			{7,  5, 2}, { 4,  6, 2}, {6,  4, 2},
			{5,  7, 3}, { 0,  8, 3}, {4,  6, 3},
			{6,  4, 4}, { 0,  9, 4}, {2,  10, 4},
			{3, 11, 5}, { 5,  7, 5}, {7,  5, 5},
			{1,  0, 0}, { 3,  3, 0}, {2,  1, 0},
			{3,  3, 1}, { 7,  5, 1}, {6,  4, 1},
			{7,  5, 2}, { 5,  7, 2}, {4,  6, 2},
			{5,  7, 3}, { 1, 12, 3}, {0,  8, 3},
			{6,  4, 4}, { 4,  6, 4}, {0,  9, 4},
			{3, 11, 5}, { 1, 13, 5}, {5,  7, 5}
		};

		return;
	}
};
