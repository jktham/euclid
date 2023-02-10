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
		 1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f,  1.0f,
	};

	loadScene(1);
}

void Renderer::update() {
	if (animation) {
		time += app.deltaTime;
	}
	for (int i=0;i<bobbingSpheres.size();i++) {
		float factor = bobbingSpheres[i].parameters.x + 0.5f*(bobbingSpheres[i].parameters.y - bobbingSpheres[i].parameters.x)*(1.0f + sin(bobbingSpheres[i].parameters.z*time + bobbingSpheres[i].parameters.w));
		bobbingSpheres[i].position = bobbingSpheres[i].origin + glm::vec4(glm::vec3(bobbingSpheres[i].direction * factor), 0.0f);
	}
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
	if (raytracing) {
		glUniform1i(5, bounces);
	} else {
		glUniform1i(5, 1);
	}
	glUniform1i(6, planes.size());
	glUniform1i(7, spheres.size());
	glUniform1i(8, quads.size());
	glUniform1i(9, bobbingSpheres.size());
	glUniform1i(10, cubes.size());

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
	quads.clear();
	bobbingSpheres.clear();
	cubes.clear();

	if (id == 1) {
		planes.push_back(Plane(glm::vec3(0.0f, 1.0f, 0.0f), -30.0f, glm::vec3(0.5f, 0.5f, 0.5f), 0.8f));
		quads.push_back(Quad(glm::vec3(0.0f, -30.0f, 0.0f), glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(0.0f, 20.0f, -50.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.9f));
		glm::vec3 colors1[] = {glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.3f, 0.9f, 0.9f), glm::vec3(0.9f, 0.3f, 0.9f), glm::vec3(0.9f, 0.9f, 0.3f)};
		glm::vec3 colors2[] = {glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.9f, 0.3f, 0.3f), glm::vec3(0.3f, 0.9f, 0.3f), glm::vec3(0.3f, 0.3f, 0.9f)};
		int n = 16;
		float pi = 3.1415926f;
		for (int i=0;i<n;i++) {
			bobbingSpheres.push_back(BobbingSphere(glm::vec3( 10.0f*(i%4) + 10.0f, 0.0f, 10.0f*(i/4) - 40.0f), 3.0f, glm::vec3(0.0f, 1.0f, 0.0f), -6.0f, 6.0f, 1.0f, i*2.0f*pi/(float)n, colors1[i%4], i/4 * 0.333f));
			bobbingSpheres.push_back(BobbingSphere(glm::vec3(-10.0f*(i%4) - 10.0f, 0.0f, 10.0f*(i/4) - 40.0f), 3.0f, glm::vec3(0.0f, 1.0f, 0.0f), -6.0f, 6.0f, 1.0f, i*2.0f*pi/(float)n, colors2[i%4], i/4 * 0.333f));
		}
	} else if (id == 2) {
		planes.push_back(Plane(glm::vec3(0.0f, 1.0f, 0.0f), -10.0f, glm::vec3(0.5f, 0.5f, 0.5f), 0.8f));
		int n = 30;
			float pi = 3.1415926f;
		for (int i=0;i<n;i++) {
			bobbingSpheres.push_back(BobbingSphere(glm::vec3(rnd(-50.0f, 50.0f), rnd(0.0f, 10.0f), rnd(-10.0f, -110.0f)), rnd(2.0f, 6.0f), glm::vec3(0.0f, 1.0f, 0.0f), rnd(-8.0f, -2.0f), rnd(2.0f, 8.0f), rnd(0.5f, 2.0f), rnd(0.0f, 2.0f*pi), glm::vec3(rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f)), 1.0f));
		}
	} else if (id == 3) {
		planes.push_back(Plane(glm::vec3(0.0f, 1.0f, 0.0f), -30.0f, glm::vec3(0.5f, 0.5f, 0.5f), 0.8f));
		spheres.push_back(Sphere(glm::vec3(0.0f, 0.0f, -30.0f), 20.0f, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
		int n = 30;
			float pi = 3.1415926f;
		for (int i=0;i<30;i++) {
			float phi = i * 2.0f*pi/(float)n;
			bobbingSpheres.push_back(BobbingSphere(glm::vec3(cos(phi)*n, 0.0f, -30.0f + sin(phi)*n), 1.0f, glm::vec3(0.0f, 1.0f, 0.0f), -10.0f, 10.0f, 1.0f, phi*3.0f, glm::vec3(rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f)), 1.0f));
		}
	} else if (id == 4) {
		quads.push_back(Quad(glm::vec3(-35.0f, -20.0f, 0.0f), glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(0.0f, 0.0f, -40.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.9f));
		quads.push_back(Quad(glm::vec3( 35.0f, -20.0f, 0.0f), glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(0.0f, 0.0f, -40.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.9f));
		int n = 36;
		for (int i=0;i<n;i++) {
			float pi = 3.1415926f;
			float phi = i * 2.0f*pi/(float)n;
			bobbingSpheres.push_back(BobbingSphere(glm::vec3(((n-1)/2.0f-i)*2.0f, 0.0f, -20.0f), 1.0f, glm::vec3(0.0f, cos(phi), sin(phi)), -10.0f, 10.0f, 1.0f, 24.0f*phi, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
		}
	} else if (id == 5) {
		planes.push_back(Plane(glm::vec3(0.0f, 1.0f, 0.0f), -30.0f, glm::vec3(0.5f, 0.5f, 0.5f), 0.8f));
		int n = 3;
		float w = 10.0f;
		for (int i=0;i<n;i++) {
			for (int j=0;j<n-i;j++) {
				cubes.push_back(Cube(glm::vec3(i*w, (i+j)*w - 15.0f, -j*w - 10.0f), glm::vec3(w, 0.0f, 0.0f), glm::vec3(0.0f, w, 0.0f), glm::vec3(0.0f, 0.0f, -w), glm::vec3(rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f)), 0.9f));
			}
		}
		spheres.push_back(Sphere(glm::vec3( 5.0f,  0.0f, -15.0f), 2.0f, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
		spheres.push_back(Sphere(glm::vec3(15.0f, 10.0f, -15.0f), 2.0f, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
		spheres.push_back(Sphere(glm::vec3( 5.0f, 10.0f, -25.0f), 2.0f, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
	} else if (id == 6) {
		int n = 6;
		float pi = 3.1415926f;
		float r = 20.0f;
		float s = 2.0f*r*sin(pi/(float)n);
		float h = s;
		for (int i=0;i<n;i++) {
			float phi = i * 2.0f*pi/(float)n;
			quads.push_back(Quad(glm::vec3(cos(phi)*r, -h/2.0f, sin(phi)*r), glm::vec3(0.0f, h, 0.0f), glm::vec3(cos(phi+pi/2.0f+pi/(float)n)*s, 0.0f, sin(phi+pi/2.0f+pi/(float)n)*s), glm::vec3(0.9f, 0.9f, 0.9f), 1.0f));
		}
		quads.push_back(Quad(glm::vec3(-r, -h/2.0f, r), glm::vec3(2.0f*r, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -2.0f*r), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
		quads.push_back(Quad(glm::vec3(-r, h/2.0f, r), glm::vec3(2.0f*r, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -2.0f*r), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
		spheres.push_back(Sphere(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
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

	glGenBuffers(1, &uboQuads);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboQuads);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glGenBuffers(1, &uboBobbingSpheres);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, uboBobbingSpheres);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glGenBuffers(1, &uboCubes);
	glBindBufferBase(GL_UNIFORM_BUFFER, 4, uboCubes);
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

	glBindBuffer(GL_UNIFORM_BUFFER, uboQuads);
	glBufferData(GL_UNIFORM_BUFFER, quads.size() * sizeof(Quad), &quads.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, uboBobbingSpheres);
	glBufferData(GL_UNIFORM_BUFFER, bobbingSpheres.size() * sizeof(BobbingSphere), &bobbingSpheres.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, uboCubes);
	glBufferData(GL_UNIFORM_BUFFER, cubes.size() * sizeof(Cube), &cubes.front(), GL_STATIC_DRAW);
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
