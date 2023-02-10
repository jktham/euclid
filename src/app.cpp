#include "app.hpp"

#include "camera.hpp"
#include "renderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>

App app;

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	app.cursorOffset = glm::ivec2(xpos, ypos) - app.cursorPos;
	app.cursorPos = glm::ivec2(xpos, ypos);
	if (app.firstInput) {
		app.firstInput = false;
		app.cursorOffset = glm::ivec2(0);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		app.renderer.bounces += 1;
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		app.renderer.bounces -= 1;
		if (app.renderer.bounces < 1) {
			app.renderer.bounces = 1;
		}
	}
	if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		app.renderer.loadScene(0);
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		app.renderer.loadScene(1);
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		app.renderer.loadScene(2);
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		app.renderer.loadScene(3);
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		app.renderer.loadScene(4);
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		app.paused = !app.paused;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset > 0.0) {
		app.camera.fov *= 0.8f;
		app.camera.sensitivity *= 0.8f;
	}
	if (yoffset < 0.0) {
		app.camera.fov *= 1.25f;
		app.camera.sensitivity *= 1.25f;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	app.width = width;
	app.height = height;
	glViewport(0, 0, width, height);
}

void App::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(width, height, "euclid", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSwapInterval(0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	camera.init();
	renderer.init();
}

void App::loop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		processInput(window);

		deltaTime = glfwGetTime() - time;
		time = glfwGetTime();
		
		std::cout << std::fixed << std::setprecision(4);
		std::cout << time << ", " << deltaTime << ", " << 1.0f / deltaTime;
		std::cout << ", " << glm::to_string(camera.position);
		std::cout << std::endl;

		camera.update();
		renderer.update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.draw();
		glfwSwapBuffers(window);
	}
}

void App::exit() {
	glfwTerminate();
}
