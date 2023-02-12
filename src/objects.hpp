#pragma once

#include <glm/glm.hpp>

struct Plane {
	glm::vec4 normal; // x, y, z, offset
	glm::vec4 color; // r, g, b, a
	glm::vec4 material; // ambient, diffuse, specular, exponent

	Plane() {
		this->normal = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		this->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		this->material = glm::vec4(0.1f, 0.5f, 0.5f, 32.0f);
	}

	Plane(glm::vec3 normal, float offset, glm::vec4 color, glm::vec4 material) {
		this->normal = glm::vec4(normal, offset);
		this->color = color;
		this->material = material;
	}
};

struct Sphere {
	glm::vec4 position; // x, y, z, radius
	glm::vec4 color; // r, g, b, a
	glm::vec4 bounds[2]; // x, y, z, 0 (generated)
	glm::vec4 material; // ambient, diffuse, specular, exponent
	
	Sphere() {
		this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		this->bounds[0] = glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f);
		this->bounds[1] = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		this->material = glm::vec4(0.1f, 0.5f, 0.5f, 32.0f);
	}

	Sphere(glm::vec3 position, float radius, glm::vec4 color, glm::vec4 material) {
		this->position = glm::vec4(position, radius);
		this->color = color;
		this->bounds[0] = glm::vec4(position - glm::vec3(radius), 0.0f);
		this->bounds[1] = glm::vec4(position + glm::vec3(radius), 0.0f);
		this->material = material;
	}
};

struct BobbingSphere {
	glm::vec4 origin; // x, y, z, radius
	glm::vec4 direction; // x, y, z, 0
	glm::vec4 parameters; // min, max, speed, offset
	glm::vec4 color; // r, g, b, a
	glm::vec4 position; // x, y, z, radius (generated)
	glm::vec4 bounds[2]; // x, y, z, 0 (generated)
	glm::vec4 material; // ambient, diffuse, specular, exponent
	
	BobbingSphere() {
		this->origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->direction = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		this->parameters = glm::vec4(-1.0f, 1.0f, 1.0f, 0.0f);
		this->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->bounds[0] = glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f);
		this->bounds[1] = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		this->material = glm::vec4(0.1f, 0.5f, 0.5f, 32.0f);
	}

	BobbingSphere(glm::vec3 position, float radius, glm::vec3 direction, float min, float max, float speed, float offset, glm::vec4 color, glm::vec4 material) {
		this->origin = glm::vec4(position, radius);
		this->direction = glm::vec4(direction, 0.0f);
		this->parameters = glm::vec4(min, max, speed, offset);
		this->color = color;
		this->position = origin;
		this->bounds[0] = glm::vec4(position - glm::vec3(radius), 0.0f);
		this->bounds[1] = glm::vec4(position + glm::vec3(radius), 0.0f);
		this->material = material;
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
	glm::vec4 color; // r, g, b, a
	glm::vec4 normal; // x, y, z, offset (generated)
	glm::vec4 bounds[2]; // x, y, z, 0 (generated)
	glm::vec4 material; // ambient, diffuse, specular, exponent
	
	Quad() {
		this->position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		this->edges[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		this->edges[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		this->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		this->normal = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		this->bounds[0] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		this->bounds[1] = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);
		this->material = glm::vec4(0.1f, 0.5f, 0.5f, 32.0f);
	}

	Quad(glm::vec3 position, glm::vec3 edge1, glm::vec3 edge2, glm::vec4 color, glm::vec4 material) {
		this->position = glm::vec4(position, 0.0f);
		this->edges[0] = glm::vec4(edge1, 0.0f);
		this->edges[1] = glm::vec4(edge2, 0.0f);
		this->color = color;
		this->normal = glm::vec4(glm::normalize(glm::cross(edge1, edge2)), glm::dot(position, glm::normalize(glm::cross(edge1, edge2))));
		this->bounds[0] = glm::vec4(position, 0.0f);
		this->bounds[1] = glm::vec4(position + edge1 + edge2, 0.0f);
		this->material = material;
	}
};

struct Cube {
	glm::vec4 position; // x, y, z, 0
	glm::vec4 edges[3]; // x, y, z, 0
	glm::vec4 color; // r, g, b, a
	glm::vec4 normals[3]; // x, y, z, offset (generated)
	glm::vec4 bounds[2]; // x, y, z, 0 (generated)
	glm::vec4 material; // ambient, diffuse, specular, exponent
	
	Cube() {
		this->position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		this->edges[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		this->edges[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		this->edges[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		this->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		this->normals[0] = -glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		this->normals[1] = -glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		this->normals[2] = -glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		this->bounds[0] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		this->bounds[1] = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		this->material = glm::vec4(0.1f, 0.5f, 0.5f, 32.0f);
	}

	Cube(glm::vec3 position, glm::vec3 edge1, glm::vec3 edge2, glm::vec3 edge3, glm::vec4 color, glm::vec4 material) {
		this->position = glm::vec4(position, 0.0f);
		this->edges[0] = glm::vec4(edge1, 0.0f);
		this->edges[1] = glm::vec4(edge2, 0.0f);
		this->edges[2] = glm::vec4(edge3, 0.0f);
		this->color = color;
		this->normals[0] = -glm::vec4(glm::normalize(glm::cross(edge1, edge2)), glm::dot(position, glm::normalize(glm::cross(edge1, edge2))));
		this->normals[1] = -glm::vec4(glm::normalize(glm::cross(edge2, edge3)), glm::dot(position, glm::normalize(glm::cross(edge2, edge3))));
		this->normals[2] = -glm::vec4(glm::normalize(glm::cross(edge3, edge1)), glm::dot(position, glm::normalize(glm::cross(edge3, edge1))));
		this->bounds[0] = glm::vec4(position, 0.0f);
		this->bounds[1] = glm::vec4(position + edge1 + edge2 + edge3, 0.0f);
		this->material = material;
	}
};

struct Light {
	glm::vec4 position; // x, y, z, 0
	glm::vec4 color; // r, g, b, a
	glm::vec4 material; // ambient, diffuse, specular, exponent

	Light() {
		this->position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		this->material = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	Light(glm::vec3 position, glm::vec4 color, glm::vec4 material) {
		this->position = glm::vec4(position, 0.0f);
		this->color = color;
		this->material = material;
	}
};
