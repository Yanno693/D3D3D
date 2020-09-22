#include "Mesh.h"

#include <iostream>

Mesh::Mesh() {
	position[0] = 0.0f;
	position[1] = 0.0f;
	position[2] = 0.0f;

	rotation[0] = 0.0f;
	rotation[1] = 0.0f;
	rotation[2] = 0.0f;

	bind_ptr = nullptr;
}

Mesh::Mesh(float* _position, float* rotation) {
	position[0] = 0.0f;
	position[1] = 0.0f;
	position[2] = 0.0f;

	rotation[0] = 0.0f;
	rotation[1] = 0.0f;
	rotation[2] = 0.0f;

	bind_ptr = nullptr;
}

void Mesh::load_mesh_obj(const char* path) {
	std::ifstream file;
	file.open(path, std::fstream::in);

	if (!file.is_open()) {
		OutputDebugStringA(path);
		OutputDebugStringA(" could not be open.\n");
		exit(1);
	}

	std::vector<float> points;
	std::vector<unsigned int> triangles;

	std::string buffer;

	while (!file.eof()) {
		std::getline(file, buffer);
		if (file.eof())
			break;

		size_t c1, c2, c3;
		double x, y, z;
		int p1, p2, p3;

		if (buffer[0] == 'v' && buffer[1] == ' ') { // vertice
			x = std::stod(buffer.substr(2), &c1);
			y = std::stod(buffer.substr(2 + c1), &c2);
			z = std::stod(buffer.substr(2 + c1 + c2));

			points.push_back(x);
			points.push_back(y);
			points.push_back(z);
		} else if (buffer[0] == 'f') {
			p1 = std::stod(buffer.substr(2), &c1);
			p2 = std::stod(buffer.substr(2 + c1), &c2);
			p3 = std::stod(buffer.substr(2 + c1 + c2));

			triangles.push_back(p1 - 1);
			triangles.push_back(p2 - 1);
			triangles.push_back(p3 - 1);
		}
	}
	
	file.close();

	size_t i = 0;
	float r1, r2, r3;
	
	while (i < triangles.size()) {

		if (i % 3 == 0) {
			r1 = points[3 * triangles[i]];
			r2 = points[3 * triangles[i] + 1];
			r3 = points[3 * triangles[i] + 2];

			triangles_points.push_back(points[3 * triangles[i]]);
			triangles_points.push_back(points[3 * triangles[i] + 1]);
			triangles_points.push_back(points[3 * triangles[i] + 2]);
		} else {
			triangles_points.push_back(points[3 * triangles[i]] - r1);
			triangles_points.push_back(points[3 * triangles[i] + 1] - r2);
			triangles_points.push_back(points[3 * triangles[i] + 2] - r3);
		}
		i++;
	}

	std::cout << std::endl;
}

void Mesh::bind(Graphics& gfx) {
	unsigned int i = 0;
	bind_ptr = gfx.triangles.triangles_points.data() + gfx.triangles.triangles_points.size();

	while (i < triangles_points.size()) {
		gfx.triangles.triangles_points.push_back(triangles_points[i]);
		gfx.triangles.triangles_points.push_back(triangles_points[i + 1]);
		gfx.triangles.triangles_points.push_back(triangles_points[i + 2]);
		gfx.triangles.triangles_points.push_back(triangles_points[i + 3]);
		gfx.triangles.triangles_points.push_back(triangles_points[i + 4]);
		gfx.triangles.triangles_points.push_back(triangles_points[i + 5]);
		gfx.triangles.triangles_points.push_back(triangles_points[i + 6]);
		gfx.triangles.triangles_points.push_back(triangles_points[i + 7]);
		gfx.triangles.triangles_points.push_back(triangles_points[i + 8]);
		gfx.triangles.color[gfx.triangle_count] = DirectX::XMFLOAT4(0.5f, 1, 0, 1);
		i += 9;
		gfx.triangle_count++;
	}
}