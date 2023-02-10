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

struct Quad {
	Plane plane = Plane(glm::vec4(0.0f), glm::vec4(0.0f));
	glm::vec4 position = glm::vec4(0.0f);
	glm::vec4 edge1 = glm::vec4(0.0f);
	glm::vec4 edge2 = glm::vec4(0.0f);
	
	Quad(Plane plane, glm::vec4 position, glm::vec4 edge1, glm::vec4 edge2) {
		this->plane = plane;
		this->position = position;
		this->edge1 = edge1;
		this->edge2 = edge2;
	}
};

struct BobbingSphere {
	Sphere sphere = Sphere(glm::vec4(0.0f), glm::vec4(0.0f));
	glm::vec4 position = glm::vec4(0.0f);
	glm::vec4 direction = glm::vec4(0.0f);
	glm::vec4 parameters = glm::vec4(0.0f);
	
	BobbingSphere(Sphere sphere, glm::vec4 position, glm::vec4 direction, glm::vec4 parameters) {
		this->sphere = sphere;
		this->position = position;
		this->direction = direction;
		this->parameters = parameters;
	}
};

class Renderer {
public:
	unsigned int shader;
	unsigned int vao;
	unsigned int vbo;
	unsigned int uboPlanes;
	unsigned int uboSpheres;
	unsigned int uboQuads;
	unsigned int uboBobbingSpheres;

	std::vector<float> vertices;
	std::vector<Plane> planes;
	std::vector<Sphere> spheres;
	std::vector<Quad> quads;
	std::vector<BobbingSphere> bobbingSpheres;

	int bounces = 20;
	float time;

	void init();
	void update();
	void draw();

	float rnd(float min, float max);
	void loadScene(int id);

	void generateBuffers();
	void updateBuffers();
	unsigned int compileShader(std::string name);
};
