#pragma once

#include <glm/glm.hpp>
#include <iostream>

struct Plane {
	glm::vec4 normal; // x, y, z, offset
	glm::vec4 color; // r, g, b, a
	glm::vec4 material; // ambient, diffuse, specular, exponent

	Plane(
		glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f), 
		float offset = 0.0f, 
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), 
		glm::vec4 material = glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)) {
			this->normal = glm::vec4(glm::normalize(normal), offset);
			this->color = color;
			this->material = material;
	}
};

struct Sphere {
	glm::vec4 position; // x, y, z, radius
	glm::vec4 color; // r, g, b, a
	glm::vec4 material; // ambient, diffuse, specular, exponent
	
	glm::vec4 bounds[2]; // x, y, z, 0 (generated)

	Sphere(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		float radius = 1.0f, 
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), 
		glm::vec4 material = glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)) {
			this->position = glm::vec4(position, radius);
			this->color = color;
			this->material = material;
			this->bounds[0] = glm::vec4(position - glm::vec3(radius), 0.0f);
			this->bounds[1] = glm::vec4(position + glm::vec3(radius), 0.0f);
	}

	void generate() {
		bounds[0] = glm::vec4(glm::vec3(position) - glm::vec3(position.w), 0.0f);
		bounds[1] = glm::vec4(glm::vec3(position) + glm::vec3(position.w), 0.0f);
	}
};

struct Quad {
	glm::vec4 position; // x, y, z, 0
	glm::vec4 edges[2]; // x, y, z, 0
	glm::vec4 color; // r, g, b, a
	glm::vec4 material; // ambient, diffuse, specular, exponent
	
	glm::vec4 normal; // x, y, z, offset (generated)
	glm::vec4 bounds[2]; // x, y, z, 0 (generated)

	Quad(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 edge1 = glm::vec3(1.0f, 0.0f, 0.0f), 
		glm::vec3 edge2 = glm::vec3(0.0f, 1.0f, 0.0f), 
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), 
		glm::vec4 material = glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)) {
			this->position = glm::vec4(position, 0.0f);
			this->edges[0] = glm::vec4(edge1, 0.0f);
			this->edges[1] = glm::vec4(edge2, 0.0f);
			this->color = color;
			this->material = material;
			this->normal = glm::vec4(glm::normalize(glm::cross(edge1, edge2)), glm::dot(position, glm::normalize(glm::cross(edge1, edge2))));
			this->bounds[0] = glm::vec4(position, 0.0f);
			this->bounds[1] = glm::vec4(position + edge1 + edge2, 0.0f);
	}

	void generate() {
		normal = glm::vec4(glm::normalize(glm::cross(glm::vec3(edges[0]), glm::vec3(edges[1]))), glm::dot(glm::vec3(position), glm::normalize(glm::cross(glm::vec3(edges[0]), glm::vec3(edges[1])))));
		bounds[0] = position;
		bounds[1] = position + edges[0] + edges[1];
	}
};

struct Cube {
	glm::vec4 position; // x, y, z, 0
	glm::vec4 edges[3]; // x, y, z, 0
	glm::vec4 color; // r, g, b, a
	glm::vec4 material; // ambient, diffuse, specular, exponent

	glm::vec4 normals[3]; // x, y, z, offset (generated)
	glm::vec4 bounds[2]; // x, y, z, 0 (generated)

	Cube(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 edge1 = glm::vec3(1.0f, 0.0f, 0.0f), 
		glm::vec3 edge2 = glm::vec3(0.0f, 1.0f, 0.0f), 
		glm::vec3 edge3 = glm::vec3(0.0f, 0.0f, 1.0f), 
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), 
		glm::vec4 material = glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)) {
			this->position = glm::vec4(position, 0.0f);
			this->edges[0] = glm::vec4(edge1, 0.0f);
			this->edges[1] = glm::vec4(edge2, 0.0f);
			this->edges[2] = glm::vec4(edge3, 0.0f);
			this->color = color;
			this->material = material;
			this->normals[0] = glm::vec4(glm::normalize(glm::cross(edge1, edge2)), glm::dot(position, glm::normalize(glm::cross(edge1, edge2))));
			this->normals[1] = glm::vec4(glm::normalize(glm::cross(edge2, edge3)), glm::dot(position, glm::normalize(glm::cross(edge2, edge3))));
			this->normals[2] = glm::vec4(glm::normalize(glm::cross(edge3, edge1)), glm::dot(position, glm::normalize(glm::cross(edge3, edge1))));
			this->bounds[0] = glm::vec4(position, 0.0f);
			this->bounds[1] = glm::vec4(position + edge1 + edge2 + edge3, 0.0f);
	}

	void generate() {
		normals[0] = glm::vec4(glm::normalize(glm::cross(glm::vec3(edges[0]), glm::vec3(edges[1]))), glm::dot(glm::vec3(position), glm::normalize(glm::cross(glm::vec3(edges[0]), glm::vec3(edges[1])))));
		normals[1] = glm::vec4(glm::normalize(glm::cross(glm::vec3(edges[1]), glm::vec3(edges[2]))), glm::dot(glm::vec3(position), glm::normalize(glm::cross(glm::vec3(edges[1]), glm::vec3(edges[2])))));
		normals[2] = glm::vec4(glm::normalize(glm::cross(glm::vec3(edges[2]), glm::vec3(edges[0]))), glm::dot(glm::vec3(position), glm::normalize(glm::cross(glm::vec3(edges[2]), glm::vec3(edges[0])))));
		bounds[0] = position;
		bounds[1] = position + edges[0] + edges[1] + edges[2];
	}
};

struct Volume {
	glm::vec4 position; // x, y, z, 0
	glm::vec4 edges[3]; // x, y, z, 0
	glm::vec4 color; // r, g, b, a
	glm::vec4 material; // ambient, diffuse, specular, exponent

	glm::vec4 normals[3]; // x, y, z, offset (generated)
	glm::vec4 bounds[2]; // x, y, z, 0 (generated)

	Volume(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 edge1 = glm::vec3(1.0f, 0.0f, 0.0f), 
		glm::vec3 edge2 = glm::vec3(0.0f, 1.0f, 0.0f), 
		glm::vec3 edge3 = glm::vec3(0.0f, 0.0f, 1.0f), 
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), 
		glm::vec4 material = glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)) {
			this->position = glm::vec4(position, 0.0f);
			this->edges[0] = glm::vec4(edge1, 0.0f);
			this->edges[1] = glm::vec4(edge2, 0.0f);
			this->edges[2] = glm::vec4(edge3, 0.0f);
			this->color = color;
			this->material = material;
			this->normals[0] = glm::vec4(glm::normalize(glm::cross(edge1, edge2)), glm::dot(position, glm::normalize(glm::cross(edge1, edge2))));
			this->normals[1] = glm::vec4(glm::normalize(glm::cross(edge2, edge3)), glm::dot(position, glm::normalize(glm::cross(edge2, edge3))));
			this->normals[2] = glm::vec4(glm::normalize(glm::cross(edge3, edge1)), glm::dot(position, glm::normalize(glm::cross(edge3, edge1))));
			this->bounds[0] = glm::vec4(position, 0.0f);
			this->bounds[1] = glm::vec4(position + edge1 + edge2 + edge3, 0.0f);
	}

	void generate() {
		normals[0] = glm::vec4(glm::normalize(glm::cross(glm::vec3(edges[0]), glm::vec3(edges[1]))), glm::dot(glm::vec3(position), glm::normalize(glm::cross(glm::vec3(edges[0]), glm::vec3(edges[1])))));
		normals[1] = glm::vec4(glm::normalize(glm::cross(glm::vec3(edges[1]), glm::vec3(edges[2]))), glm::dot(glm::vec3(position), glm::normalize(glm::cross(glm::vec3(edges[1]), glm::vec3(edges[2])))));
		normals[2] = glm::vec4(glm::normalize(glm::cross(glm::vec3(edges[2]), glm::vec3(edges[0]))), glm::dot(glm::vec3(position), glm::normalize(glm::cross(glm::vec3(edges[2]), glm::vec3(edges[0])))));
		bounds[0] = position;
		bounds[1] = position + edges[0] + edges[1] + edges[2];
	}
};

struct Light {
	glm::vec4 position; // x, y, z, 0
	glm::vec4 color; // r, g, b, a
	glm::vec4 material; // ambient, diffuse, specular, exponent

	Light(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
		glm::vec4 material = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) {
			this->position = glm::vec4(position, 0.0f);
			this->color = color;
			this->material = material;
	}
};

class Updater {
public:
	virtual void update(float time) {

	}
};

class BobUpdater : public Updater {
public:
	glm::vec4* position;
	glm::vec3 origin;
	glm::vec3 axis;
	float min;
	float max;
	float speed;
	float offset;

	BobUpdater(
		glm::vec4* position, 
		glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f), 
		float min = -1.0f, 
		float max = 1.0f, 
		float speed = 1.0f, 
		float offset = 0.0f) {
			this->position = position;
			this->origin = glm::vec3(*position);
			this->axis = glm::normalize(axis);
			this->min = min;
			this->max = max;
			this->speed = speed;
			this->offset = offset;
	}

	virtual void update(float time) {
		float factor = min + 0.5f*(max - min)*(1.0f + sin(speed*time + offset));
		*position = glm::vec4(origin + glm::vec3(axis * factor), position->w);
	}
};

class CircleUpdater : public Updater {
public:
	glm::vec4* position;
	glm::vec3 origin;
	glm::vec3 axis;
	float radius;
	float speed;
	float offset;

	CircleUpdater(
		glm::vec4* position, 
		glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f), 
		float radius = 1.0f, 
		float speed = 1.0f, 
		float offset = 0.0f) {
			this->position = position;
			this->origin = glm::vec3(*position);
			this->axis = glm::normalize(axis);
			this->radius = radius;
			this->speed = speed;
			this->offset = offset;
	}

	virtual void update(float time) {
		glm::vec3 axisX = glm::cross(axis, glm::vec3(1.0f, 0.0f, 0.0f));
		if (glm::length(axisX) < 0.001f) {
			axisX = glm::cross(axis, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		glm::vec3 axisY = glm::cross(axis, axisX);
		*position = glm::vec4(origin + glm::normalize(axisX) * (float)sin(speed * time + offset) * radius + glm::normalize(axisY) * (float)cos(speed * time + offset) * radius, position->w);
	}
};
