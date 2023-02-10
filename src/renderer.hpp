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
	glm::vec4 edges[2] = {glm::vec4(0.0f), glm::vec4(0.0f)}; // x1, y1, z1, 0
	glm::vec4 color = glm::vec4(0.0f); // r, g, b, shininess
	glm::vec4 normal = glm::vec4(0.0f); // x, y, z, offset (generated)
	
	Quad(glm::vec3 position, glm::vec3 edge1, glm::vec3 edge2, glm::vec3 color, float shininess) {
		this->position = glm::vec4(position, 0.0f);
		this->edges[0] = glm::vec4(edge1, 0.0f);
		this->edges[1] = glm::vec4(edge2, 0.0f);
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

struct Cube {
	glm::vec4 position = glm::vec4(0.0f); // x, y, z, 0
	glm::vec4 edges[3] = {glm::vec4(0.0f), glm::vec4(0.0f), glm::vec4(0.0f)}; // x, y, z, 0
	glm::vec4 color = glm::vec4(0.0f); // r, g, b, shininess
	glm::vec4 normals[3] = {glm::vec4(0.0f), glm::vec4(0.0f), glm::vec4(0.0f)}; // x, y, z, offset (generated)
	
	Cube(glm::vec3 position, glm::vec3 edge1, glm::vec3 edge2, glm::vec3 edge3, glm::vec3 color, float shininess) {
		this->position = glm::vec4(position, 0.0f);
		this->edges[0] = glm::vec4(edge1, 0.0f);
		this->edges[1] = glm::vec4(edge2, 0.0f);
		this->edges[2] = glm::vec4(edge3, 0.0f);
		this->color = glm::vec4(color, shininess);
		this->normals[0] = glm::vec4(glm::normalize(glm::cross(edge1, edge2)), 0.0f);
		this->normals[0].w = glm::dot(position, glm::vec3(normals[0]));
		this->normals[1] = glm::vec4(glm::normalize(glm::cross(edge2, edge3)), 0.0f);
		this->normals[1].w = glm::dot(position, glm::vec3(normals[1]));
		this->normals[2] = glm::vec4(glm::normalize(glm::cross(edge3, edge1)), 0.0f);
		this->normals[2].w = glm::dot(position, glm::vec3(normals[2]));
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
	unsigned int uboCubes;

	std::vector<float> vertices;
	std::vector<Plane> planes;
	std::vector<Sphere> spheres;
	std::vector<Quad> quads;
	std::vector<BobbingSphere> bobbingSpheres;
	std::vector<Cube> cubes;

	int bounces = 40;
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
