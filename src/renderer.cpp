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
#include <random>

void Renderer::init() {
	shader = compileShader("shader");
	generateBuffers();

	vertices = {
		1.0f, 1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f, 1.0f,
	};

	loadScene(1);
}

void Renderer::update() {
	
}

void Renderer::draw() {
	glUseProgram(shader);
	glBindVertexArray(vao);

	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(app.camera.view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "inverseView"), 1, GL_FALSE, glm::value_ptr(glm::inverse(app.camera.view)));
	glUniform1f(glGetUniformLocation(shader, "fov"), app.camera.fov);
	glUniform2i(glGetUniformLocation(shader, "windowSize"), app.width, app.height);
	glUniform1i(glGetUniformLocation(shader, "planeCount"), planes.size());
	glUniform1i(glGetUniformLocation(shader, "sphereCount"), spheres.size());
	glUniform1i(glGetUniformLocation(shader, "bounces"), bounces);
	glUniform1f(glGetUniformLocation(shader, "time"), app.time);
	glUniform1i(glGetUniformLocation(shader, "paused"), app.paused);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);

	glBindVertexArray(0);
	glUseProgram(0);
}

float Renderer::rnd(float min, float max) {
	return min + (float)std::rand() / ((float)RAND_MAX/(max-min));
}

void Renderer::loadScene(int id) {
	planes.clear();
	spheres.clear();

	if (id == 1) {
		planes.push_back(Plane(glm::vec4(0.0f, 1.0f, 0.0f, -10.0f), glm::vec4(0.5f, 0.5f, 0.5f, 0.8f)));
		spheres.push_back(Sphere(glm::vec4(-9.0f, 0.0f, -10.0f, 2.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		spheres.push_back(Sphere(glm::vec4(-3.0f, 0.0f, -10.0f, 2.0f), glm::vec4(0.9f, 0.3f, 0.9f, 1.0f)));
		spheres.push_back(Sphere(glm::vec4(3.0f, 0.0f, -10.0f, 2.0f), glm::vec4(0.9f, 0.9f, 0.3f, 1.0f)));
		spheres.push_back(Sphere(glm::vec4(9.0f, 0.0f, -10.0f, 2.0f), glm::vec4(0.3f, 0.3f, 0.9f, 1.0f)));
		spheres.push_back(Sphere(glm::vec4(-9.0f, 0.0f, -16.0f, 2.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.66f)));
		spheres.push_back(Sphere(glm::vec4(-3.0f, 0.0f, -16.0f, 2.0f), glm::vec4(0.9f, 0.3f, 0.9f, 0.66f)));
		spheres.push_back(Sphere(glm::vec4(3.0f, 0.0f, -16.0f, 2.0f), glm::vec4(0.9f, 0.9f, 0.3f, 0.66f)));
		spheres.push_back(Sphere(glm::vec4(9.0f, 0.0f, -16.0f, 2.0f), glm::vec4(0.3f, 0.3f, 0.9f, 0.66f)));
		spheres.push_back(Sphere(glm::vec4(-9.0f, 0.0f, -22.0f, 2.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.33f)));
		spheres.push_back(Sphere(glm::vec4(-3.0f, 0.0f, -22.0f, 2.0f), glm::vec4(0.9f, 0.3f, 0.9f, 0.33f)));
		spheres.push_back(Sphere(glm::vec4(3.0f, 0.0f, -22.0f, 2.0f), glm::vec4(0.9f, 0.9f, 0.3f, 0.33f)));
		spheres.push_back(Sphere(glm::vec4(9.0f, 0.0f, -22.0f, 2.0f), glm::vec4(0.3f, 0.3f, 0.9f, 0.33f)));
		spheres.push_back(Sphere(glm::vec4(-9.0f, 0.0f, -28.0f, 2.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)));
		spheres.push_back(Sphere(glm::vec4(-3.0f, 0.0f, -28.0f, 2.0f), glm::vec4(0.9f, 0.3f, 0.9f, 0.0f)));
		spheres.push_back(Sphere(glm::vec4(3.0f, 0.0f, -28.0f, 2.0f), glm::vec4(0.9f, 0.9f, 0.3f, 0.0f)));
		spheres.push_back(Sphere(glm::vec4(9.0f, 0.0f, -28.0f, 2.0f), glm::vec4(0.3f, 0.3f, 0.9f, 0.0f)));
	} else if (id == 2) {
		planes.push_back(Plane(glm::vec4(0.0f, 1.0f, 0.0f, -10.0f), glm::vec4(0.5f, 0.5f, 0.5f, 0.8f)));
		for (int i=0;i<30;i++) {
			spheres.push_back(Sphere(glm::vec4(rnd(-50.0f, 50.0f), rnd(0.0f, 10.0f), rnd(-10.0f, -110.0f), rnd(2.0f, 6.0f)), glm::vec4(rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), 1.0f)));
		}
	} else if (id == 3) {
		planes.push_back(Plane(glm::vec4(0.0f, 1.0f, 0.0f, -30.0f), glm::vec4(0.5f, 0.5f, 0.5f, 0.8f)));
		spheres.push_back(Sphere(glm::vec4(0.0f, 0.0f, -30.0f, 20.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		for (int i=0;i<30;i++) {
			float phi = (float)i * 2.0f*3.1415f/30.0f;
			spheres.push_back(Sphere(glm::vec4(cos(phi)*30.0f, 0.0f, -30.0f + sin(phi)*30.0f, 1.0f), glm::vec4(rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), 1.0f)));
		}
	}

	updateBuffers();
}

void Renderer::generateBuffers() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glGenBuffers(1, &uboPlanes);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboPlanes);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	glGenBuffers(1, &uboSpheres);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboSpheres);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::updateBuffers() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
	glBindVertexArray(0);

	glBindBuffer(GL_UNIFORM_BUFFER, uboPlanes);
	glBufferData(GL_UNIFORM_BUFFER, planes.size() * sizeof(Plane), &planes.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, uboSpheres);
	glBufferData(GL_UNIFORM_BUFFER, spheres.size() * sizeof(Sphere), &spheres.front(), GL_STATIC_DRAW);
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
