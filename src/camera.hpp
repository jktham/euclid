#pragma once

#include <glm/glm.hpp>

class Camera {
public:
	double fov = 90.0f;
	double speed = 10.0f;
	double sensitivity = 0.1f;
	
	double pitch = 0.0f;
	double yaw = -90.0f;

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::mat4 view = glm::mat4(1.0f);

	void init();
	void update();
};
