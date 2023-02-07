#include "app.hpp"

#include "renderer.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

void App::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	window = glfwCreateWindow(width, height, "euclid", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);

	glfwSwapInterval(0);
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	renderer.init(this);
}

void App::loop() {
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		renderer.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void App::exit() {
	glfwTerminate();
}
