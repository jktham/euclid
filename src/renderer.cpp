#include "renderer.hpp"

#include "app.hpp"
#include "scene.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

void Renderer::init() {
	shader = compileShader("shader");
	generateBuffers();

	vertices = {
		 1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f,  1.0f,
	};

	updateBuffers();
}

void Renderer::update() {
	if (animation) {
		time += app.deltaTime;
	}
	app.scene.update(time);
	updateBuffers();
}

void Renderer::draw() {
	glUseProgram(shader);
	glBindVertexArray(vao);

	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(app.camera.view));
	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(glm::inverse(app.camera.view)));
	glUniform1f(2, app.camera.fov);
	glUniform2i(3, app.width, app.height);
	glUniform1f(4, time);
	glUniform1i(5, bounces);
	glUniform1i(6, reflections);
	glUniform1i(7, lighting);
	glUniform1i(8, shadows);
	glUniform4fv(9, 1, glm::value_ptr(app.scene.skyColor));
	glUniform1i(10, app.scene.planes.size());
	glUniform1i(11, app.scene.spheres.size());
	glUniform1i(12, app.scene.quads.size());
	glUniform1i(13, app.scene.cubes.size());
	glUniform1i(14, app.scene.lights.size());

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Renderer::generateBuffers() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glGenBuffers(1, &uboObjects);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboObjects);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::updateBuffers() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
	glBindVertexArray(0);

	int bufferSize = MAX_OBJECTS*sizeof(Plane)+MAX_OBJECTS*sizeof(Sphere)+MAX_OBJECTS*sizeof(Quad)+MAX_OBJECTS*sizeof(Cube)+MAX_OBJECTS*sizeof(Light);
	int offsetPlanes = 0;
	int offsetSpheres = offsetPlanes + MAX_OBJECTS*sizeof(Plane);
	int offsetQuads = offsetSpheres + MAX_OBJECTS*sizeof(Sphere);
	int offsetCubes = offsetQuads + MAX_OBJECTS*sizeof(Quad);
	int offsetLights = offsetCubes + MAX_OBJECTS*sizeof(Cube);

	glBindBuffer(GL_UNIFORM_BUFFER, uboObjects);
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_UNIFORM_BUFFER, offsetPlanes, app.scene.planes.size()*sizeof(Plane), &app.scene.planes.front());
	glBufferSubData(GL_UNIFORM_BUFFER, offsetSpheres, app.scene.spheres.size()*sizeof(Sphere), &app.scene.spheres.front());
	glBufferSubData(GL_UNIFORM_BUFFER, offsetQuads, app.scene.quads.size()*sizeof(Quad), &app.scene.quads.front());
	glBufferSubData(GL_UNIFORM_BUFFER, offsetCubes, app.scene.cubes.size()*sizeof(Cube), &app.scene.cubes.front());
	glBufferSubData(GL_UNIFORM_BUFFER, offsetLights, app.scene.lights.size()*sizeof(Light), &app.scene.lights.front());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

unsigned int Renderer::compileShader(std::string name) {
	const char *vertSource;
	std::ifstream vertFile("res/" + name + ".vert");
	std::string vertString((std::istreambuf_iterator<char>(vertFile)), std::istreambuf_iterator<char>());
	vertSource = vertString.c_str();
	unsigned int vertShader;
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertSource, NULL);
	glCompileShader(vertShader);

	int success;
	success = 0;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (success == 0) {
		int logSize = 0;
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> errorLog(logSize);
		glGetShaderInfoLog(vertShader, logSize, &logSize, &errorLog.front());
		std::cout << errorLog.data() << std::endl;
	}

	const char *fragSource;
	std::ifstream fragFile("res/" + name + ".frag");
	std::string fragString((std::istreambuf_iterator<char>(fragFile)), std::istreambuf_iterator<char>());
	fragSource = fragString.c_str();
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSource, NULL);
	glCompileShader(fragShader);

	success = 0;
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (success == 0) {
		int logSize = 0;
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> errorLog(logSize);
		glGetShaderInfoLog(fragShader, logSize, &logSize, &errorLog.front());
		std::cout << errorLog.data() << std::endl;
	}

	unsigned int shader = glCreateProgram();
	glAttachShader(shader, vertShader);
	glAttachShader(shader, fragShader);
	glLinkProgram(shader);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return shader;
}
