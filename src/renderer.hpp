#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Plane {
	glm::vec4 normal = glm::vec4(0.0f);
	glm::vec4 color = glm::vec4(0.0f);

	Plane(glm::vec4 normal, glm::vec4 color) {
		this->normal = normal;
		this->color = color;
	}
};

struct Sphere {
	glm::vec4 position = glm::vec4(0.0f);
	glm::vec4 color = glm::vec4(0.0f);
	
	Sphere(glm::vec4 position, glm::vec4 color) {
		this->position = position;
		this->color = color;
	}
};

class Renderer {
public:
	unsigned int shader;
	unsigned int vao;
	unsigned int vbo;
	unsigned int uboPlanes;
	unsigned int uboSpheres;

	std::vector<float> vertices;
	std::vector<Plane> planes;
	std::vector<Sphere> spheres;

	int bounces = 10;

	void init();
	void update();
	void draw();

	float rnd(float min, float max);
	void loadScene(int id);

	void generateBuffers();
	void updateBuffers();
	unsigned int compileShader(std::string name);
};
