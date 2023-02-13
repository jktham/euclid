#include "app.hpp"

#include "camera.hpp"
#include "scene.hpp"
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
	if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9 && action == GLFW_PRESS) {
		app.scene.load(key - GLFW_KEY_0);
		app.renderer.updateBuffers();
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		app.renderer.animation = !app.renderer.animation;
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		app.renderer.reflections = !app.renderer.reflections;
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		app.renderer.lighting = !app.renderer.lighting;
	}
	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		app.renderer.shadows = !app.renderer.shadows;
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

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam) {
	
	if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " <<  message << std::endl;

	switch (source) {
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type) {
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;
	
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
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

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	glfwSwapInterval(0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	camera.init();
	scene.init();
	renderer.init();
}

void App::loop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		processInput(window);

		deltaTime = glfwGetTime() - time;
		time = glfwGetTime();
		
		std::cout << std::fixed << std::setprecision(4);
		std::cout << "time: " << time << ", delta: " << deltaTime << ", fps: " << 1.0f / deltaTime;
		std::cout << ", pos: (" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << ")";
		std::cout << ", bounces: " << renderer.bounces;
		std::cout << ", animation: " << renderer.animation;
		std::cout << ", reflections: " << renderer.reflections;
		std::cout << ", lighting: " << renderer.lighting;
		std::cout << ", shadows: " << renderer.shadows;
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
