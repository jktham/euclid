#include "renderer.hpp"

#include "app.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void Renderer::init() {
	shader = compileShader("shader");

	vertices = {
		1.0f, 1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f, 1.0f,
	};

	spheres = { // position.xyz, radius, color.rgb, shininess
		0.0f, 0.0f, -3.0f, 1.0f, 0.8f, 0.4f, 0.2f, 1.0f,
		1.0f, 4.0f, -10.0f, 2.0f, 0.1f, 0.8f, 0.2f, 1.0f,
		-4.0f, -2.0f, -12.0f, 3.0f, 0.3f, 0.3f, 0.8f, 1.0f,
		5.0f, -2.0f, -8.0f, 4.0f, 0.8f, 0.2f, 0.8f, 0.0f,
		-9.0f, 5.0f, -4.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-3.0f, 5.0f, -4.0f, 2.0f, 1.0f, 1.0f, 1.0f, 0.6f,
		3.0f, 5.0f, -4.0f, 2.0f, 1.0f, 1.0f, 1.0f, 0.3f,
		9.0f, 5.0f, -4.0f, 2.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-9.0f, 10.0f, -4.0f, 2.0f, 1.0f, 0.1f, 0.1f, 1.0f,
		-3.0f, 10.0f, -4.0f, 2.0f, 1.0f, 0.1f, 0.1f, 0.6f,
		3.0f, 10.0f, -4.0f, 2.0f, 1.0f, 0.1f, 0.1f, 0.3f,
		9.0f, 10.0f, -4.0f, 2.0f, 1.0f, 0.1f, 0.1f, 0.0f,
	};

	planes = { // normal.xyz, 0, point.xyz, 0, color.rgb, shininess
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.6f, 0.6f, 0.6f, 0.8f,
	};

	generateBuffers();
}

void Renderer::update() {

}

void Renderer::draw() {
	glUseProgram(shader);
	glBindVertexArray(vao);

	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(app.camera.view));
	glUniform1f(glGetUniformLocation(shader, "fov"), app.camera.fov);
	glUniform2i(glGetUniformLocation(shader, "windowSize"), app.width, app.height);
	glUniform1i(glGetUniformLocation(shader, "sphereCount"), spheres.size() / 8);
	glUniform1i(glGetUniformLocation(shader, "planeCount"), planes.size() / 12);
	glUniform1i(glGetUniformLocation(shader, "bounces"), bounces);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Renderer::generateBuffers() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	
	glGenBuffers(1, &uboSpheres);
	glBindBuffer(GL_UNIFORM_BUFFER, uboSpheres);
	glBufferData(GL_UNIFORM_BUFFER, spheres.size() * sizeof(float), &spheres.front(), GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboSpheres);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glGenBuffers(1, &uboPlanes);
	glBindBuffer(GL_UNIFORM_BUFFER, uboPlanes);
	glBufferData(GL_UNIFORM_BUFFER, planes.size() * sizeof(float), &planes.front(), GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboPlanes);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::updateBuffers() {
	glBindBuffer(GL_UNIFORM_BUFFER, uboSpheres);
	glBufferData(GL_UNIFORM_BUFFER, spheres.size() * sizeof(float), &spheres.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, uboPlanes);
	glBufferData(GL_UNIFORM_BUFFER, planes.size() * sizeof(float), &planes.front(), GL_STATIC_DRAW);
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

	const char *fragSource;
	std::ifstream fragFile("res/" + name + ".frag");
	std::string fragString((std::istreambuf_iterator<char>(fragFile)), std::istreambuf_iterator<char>());
	fragSource = fragString.c_str();
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSource, NULL);
	glCompileShader(fragShader);

	unsigned int shader = glCreateProgram();
	glAttachShader(shader, vertShader);
	glAttachShader(shader, fragShader);
	glLinkProgram(shader);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return shader;
}
