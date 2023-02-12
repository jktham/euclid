#pragma once

#include "camera.hpp"
#include "scene.hpp"
#include "renderer.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class App {
public:
	int width = 1920;
	int height = 1080;
	GLFWwindow* window;

	float time;
	float deltaTime;

	bool firstInput = true;
	glm::ivec2 cursorPos = glm::ivec2(0);
	glm::ivec2 cursorOffset = glm::ivec2(0);

	Camera camera;
	Scene scene;
	Renderer renderer;

	void init();
	void loop();
	void exit();
};

extern App app;
