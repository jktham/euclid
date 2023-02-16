#pragma once

#include "objects.hpp"

#include <vector>

class Scene {
public:
	std::vector<Plane> planes;
	std::vector<Sphere> spheres;
	std::vector<Quad> quads;
	std::vector<Cube> cubes;
	std::vector<Volume> volumes;
	std::vector<Light> lights;
	std::vector<Updater*> updaters;

	glm::vec4 skyColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f); // r, g, b, gradient bottom

	float rnd(float min, float max);

	void init();
	void load(int id);
	void update(float time);
};
