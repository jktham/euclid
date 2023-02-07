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

void Renderer::init(App* app) {
	this->app = app;
	camera = &app->camera;

	baseShader = compileShader("shader");

	float vertices[] = {
		 1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f,  1.0f,
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	float spheres[] = {
		0.0f, 0.0f, -3.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 4.0f, -10.0f, 2.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-4.0f, -2.0f, -12.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		5.0f, -2.0f, -8.0f, 4.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	};

	glGenBuffers(1, &uboSpheres);
	glBindBuffer(GL_UNIFORM_BUFFER, uboSpheres);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(spheres), &spheres, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glUniformBlockBinding(baseShader, glGetUniformBlockIndex(baseShader, "Spheres"), 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboSpheres);

	float planes[] = {
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f, 0.0f,
	};

	glGenBuffers(1, &uboPlanes);
	glBindBuffer(GL_UNIFORM_BUFFER, uboPlanes);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(planes), &planes, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glUniformBlockBinding(baseShader, glGetUniformBlockIndex(baseShader, "Planes"), 1);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboPlanes);
	
	glBindVertexArray(0);
}

void Renderer::draw() {
	glUseProgram(baseShader);
	glBindVertexArray(vao);

	glUniformMatrix4fv(glGetUniformLocation(baseShader, "view"), 1, GL_FALSE, glm::value_ptr(camera->view));
	glUniformMatrix4fv(glGetUniformLocation(baseShader, "projection"), 1, GL_FALSE, glm::value_ptr(camera->projection));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glUseProgram(0);
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
