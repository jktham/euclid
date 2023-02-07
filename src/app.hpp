#pragma once

#include "camera.hpp"
#include "renderer.hpp"

#include <GLFW/glfw3.h>

class App {
public:
	int width = 1920;
	int height = 1080;
	GLFWwindow* window;

	Camera camera;
	Renderer renderer;
	
	void init();
	void loop();
	void exit();
};
