#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Plane {
	glm::vec4 normal = glm::vec4(0.0f); // x, y, z, offset
	glm::vec4 color = glm::vec4(0.0f); // r, g, b, shininess

	Plane(glm::vec3 normal, float offset, glm::vec3 color, float shininess) {
		this->normal = glm::vec4(normal, offset);
		this->color = glm::vec4(color, shininess);
	}
};

struct Sphere {
	glm::vec4 position = glm::vec4(0.0f); // x, y, z, radius
	glm::vec4 color = glm::vec4(0.0f); // r, g, b, shininess
	
	Sphere(glm::vec3 position, float radius, glm::vec3 color, float shininess) {
		this->position = glm::vec4(position, radius);
		this->color = glm::vec4(color, shininess);
	}
};

struct Quad {
	glm::vec4 position = glm::vec4(0.0f); // x, y, z, 0
	glm::vec4 edge1 = glm::vec4(0.0f); // x1, y1, z1, 0
	glm::vec4 edge2 = glm::vec4(0.0f); // x2, y2, z2, 0
	glm::vec4 color = glm::vec4(0.0f); // r, g, b, shininess
	glm::vec4 normal = glm::vec4(0.0f); // x, y, z, offset (generated)
	
	Quad(glm::vec3 position, glm::vec3 edge1, glm::vec3 edge2, glm::vec3 color, float shininess) {
		this->position = glm::vec4(position, 0.0f);
		this->edge1 = glm::vec4(edge1, 0.0f);
		this->edge2 = glm::vec4(edge2, 0.0f);
		this->color = glm::vec4(color, shininess);
		this->normal = glm::vec4(glm::normalize(glm::cross(edge1, edge2)), 0.0f);
		this->normal.w = glm::dot(position, glm::vec3(normal));
	}
};

struct BobbingSphere {
	glm::vec4 origin = glm::vec4(0.0f); // x, y, z, radius
	glm::vec4 direction = glm::vec4(0.0f); // x, y, z, 0
	glm::vec4 parameters = glm::vec4(0.0f); // min, max, speed, offset
	glm::vec4 color = glm::vec4(0.0f); // r, g, b, shininess
	glm::vec4 position = glm::vec4(0.0f); // x, y, z, radius (generated)
	
	BobbingSphere(glm::vec3 position, float radius, glm::vec3 direction, float min, float max, float speed, float offset, glm::vec3 color, float shininess) {
		this->origin = glm::vec4(position, radius);
		this->direction = glm::vec4(direction, 0.0f);
		this->parameters = glm::vec4(min, max, speed, offset);
		this->color = glm::vec4(color, shininess);
		this->position = origin;
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
