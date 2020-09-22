#pragma once
#include <DirectXMath.h>
#include <vector>

#include "Graphics.h"

class Mesh {
public:
	Mesh();
	Mesh(float* _position, float* _rotation);

	void load_mesh_obj(const char* path);
	void bind(Graphics& gfx);
private:
	float position[3];
	float rotation[3];

	std::vector<float> triangles_points;

	float* bind_ptr;
};