#pragma once

#include <glm/glm.hpp>

class App;

class Camera {
public:
	float fov = 90.0f;
	float speed = 0.1f;
	float sensitivity = 0.1f;
	double cursorX = 0.0f;
	double cursorY = 0.0f;
	double lastCursorX = 0.0f;
	double lastCursorY = 0.0f;
	float pitch = 0.0f;
	float yaw = -90.0f;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	App* app;

	void init(App* app);
	void update();
};
