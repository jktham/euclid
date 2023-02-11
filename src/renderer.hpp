#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Plane {
	glm::vec4 normal; // x, y, z, offset
	glm::vec4 color; // r, g, b, shininess

	Plane() {
		this->normal = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		this->color = glm::vec4(1.0f);
	}

	Plane(glm::vec3 normal, float offset, glm::vec3 color, float shininess) {
		this->normal = glm::vec4(normal, offset);
		this->color = glm::vec4(color, shininess);
	}
};

struct Sphere {
	glm::vec4 position; // x, y, z, radius
	glm::vec4 color; // r, g, b, shininess
	glm::vec4 bounds[2]; // x, y, z, 0 (generated)
	
	Sphere() {
		this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->color = glm::vec4(1.0f);
		this->bounds[0] = glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f);
		this->bounds[1] = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	}

	Sphere(glm::vec3 position, float radius, glm::vec3 color, float shininess) {
		this->position = glm::vec4(position, radius);
		this->color = glm::vec4(color, shininess);
		this->bounds[0] = glm::vec4(position - glm::vec3(radius), 0.0f);
		this->bounds[1] = glm::vec4(position + glm::vec3(radius), 0.0f);
	}
};

struct BobbingSphere {
	glm::vec4 origin; // x, y, z, radius
	glm::vec4 direction; // x, y, z, 0
	glm::vec4 parameters; // min, max, speed, offset
	glm::vec4 color; // r, g, b, shininess
	glm::vec4 position; // x, y, z, radius (generated)
	glm::vec4 bounds[2]; // x, y, z, 0 (generated)
	
	BobbingSphere() {
		this->origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->direction = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		this->parameters = glm::vec4(-1.0f, 1.0f, 1.0f, 0.0f);
		this->color = glm::vec4(1.0f);
		this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->bounds[0] = glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f);
		this->bounds[1] = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	}

	BobbingSphere(glm::vec3 position, float radius, glm::vec3 direction, float min, float max, float speed, float offset, glm::vec3 color, float shininess) {
		this->origin = glm::vec4(position, radius);
		this->direction = glm::vec4(direction, 0.0f);
		this->parameters = glm::vec4(min, max, speed, offset);
		this->color = glm::vec4(color, shininess);
		this->position = origin;
		this->bounds[0] = glm::vec4(position - glm::vec3(radius), 0.0f);
		this->bounds[1] = glm::vec4(position + glm::vec3(radius), 0.0f);
	}

	void update(float time) {
		float factor = parameters.x + 0.5f*(parameters.y - parameters.x)*(1.0f + sin(parameters.z*time + parameters.w));
		position = origin + glm::vec4(glm::vec3(direction * factor), 0.0f);
		bounds[0] = glm::vec4(glm::vec3(position) - glm::vec3(position.w), 0.0f);
		bounds[1] = glm::vec4(glm::vec3(position) + glm::vec3(position.w), 0.0f);
	}
};

struct Quad {
	glm::vec4 position; // x, y, z, 0
	glm::vec4 edges[2]; // x, y, z, 0
	glm::vec4 color; // r, g, b, shininess
	glm::vec4 normal; // x, y, z, offset (generated)
	glm::vec4 bounds[2]; // x, y, z, 0 (generated)
	
	Quad() {
		this->position = glm::vec4(0.0f);
		this->edges[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		this->edges[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		this->color = glm::vec4(1.0f);
		this->normal = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		this->bounds[0] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		this->bounds[1] = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);
	}

	Quad(glm::vec3 position, glm::vec3 edge1, glm::vec3 edge2, glm::vec3 color, float shininess) {
		this->position = glm::vec4(position, 0.0f);
		this->edges[0] = glm::vec4(edge1, 0.0f);
		this->edges[1] = glm::vec4(edge2, 0.0f);
		this->color = glm::vec4(color, shininess);
		this->normal = glm::vec4(glm::normalize(glm::cross(edge1, edge2)), glm::dot(position, glm::normalize(glm::cross(edge1, edge2))));
		this->bounds[0] = glm::vec4(position, 0.0f);
		this->bounds[1] = glm::vec4(position + edge1 + edge2, 0.0f);
	}
};

struct Cube {
	glm::vec4 position; // x, y, z, 0
	glm::vec4 edges[3]; // x, y, z, 0
	glm::vec4 color; // r, g, b, shininess
	glm::vec4 normals[3]; // x, y, z, offset (generated)
	glm::vec4 bounds[2]; // x, y, z, 0 (generated)
	
	Cube() {
		this->position = glm::vec4(0.0f);
		this->edges[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		this->edges[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		this->edges[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		this->color = glm::vec4(1.0f);
		this->normals[0] = -glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		this->normals[1] = -glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		this->normals[2] = -glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		this->bounds[0] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		this->bounds[1] = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	}

	Cube(glm::vec3 position, glm::vec3 edge1, glm::vec3 edge2, glm::vec3 edge3, glm::vec3 color, float shininess) {
		this->position = glm::vec4(position, 0.0f);
		this->edges[0] = glm::vec4(edge1, 0.0f);
		this->edges[1] = glm::vec4(edge2, 0.0f);
		this->edges[2] = glm::vec4(edge3, 0.0f);
		this->color = glm::vec4(color, shininess);
		this->normals[0] = -glm::vec4(glm::normalize(glm::cross(edge1, edge2)), glm::dot(position, glm::normalize(glm::cross(edge1, edge2))));
		this->normals[1] = -glm::vec4(glm::normalize(glm::cross(edge2, edge3)), glm::dot(position, glm::normalize(glm::cross(edge2, edge3))));
		this->normals[2] = -glm::vec4(glm::normalize(glm::cross(edge3, edge1)), glm::dot(position, glm::normalize(glm::cross(edge3, edge1))));
		this->bounds[0] = glm::vec4(position, 0.0f);
		this->bounds[1] = glm::vec4(position + edge1 + edge2 + edge3, 0.0f);
	}
};

class Renderer {
public:
	unsigned int shader;
	unsigned int vao;
	unsigned int vbo;
	unsigned int uboObjects;
	const int MAX_OBJECTS = 100;

	std::vector<float> vertices;
	std::vector<Plane> planes;
	std::vector<Sphere> spheres;
	std::vector<BobbingSphere> bobbingSpheres;
	std::vector<Quad> quads;
	std::vector<Cube> cubes;

	int bounces = 20;
	float time;
	bool animation = true;
	bool raytracing = true;

	void init();
	void update();
	void draw();

	float rnd(float min, float max);
	void loadScene(int id);

	void generateBuffers();
	void updateBuffers();
	unsigned int compileShader(std::string name);
};
