#pragma once

#include "objects.hpp"

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Renderer {
public:
	unsigned int shader;
	unsigned int vao;
	unsigned int vbo;
	unsigned int uboObjects;
	const int MAX_OBJECTS = 100;

	std::vector<float> vertices;

	int bounces = 20;
	float time;
	bool animation = true;
	bool reflections = true;
	bool lighting = true;

	void init();
	void update();
	void draw();

	void generateBuffers();
	void updateBuffers();
	unsigned int compileShader(std::string name);
};
