#include "app.hpp"

#include "camera.hpp"
#include "renderer.hpp"

#include <glm/gtx/string_cast.hpp>
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

	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	camera.init(this);
	renderer.init(this);
}

void App::loop() {
	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, true);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		std::cout << glm::to_string(camera.position) << std::endl;

		camera.update();
		renderer.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void App::exit() {
	glfwTerminate();
}
