#include "camera.hpp"

#include "app.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

void Camera::init(App *app) {
	this->app = app;
	view = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(fov), (float)app->width/(float)app->height, 0.0f, 99999.0f);
	glfwGetCursorPos(app->window, &cursorX, &cursorY);
	glfwGetCursorPos(app->window, &lastCursorX, &lastCursorY);
}

void Camera::update() {
	glfwGetCursorPos(app->window, &cursorX, &cursorY);
	pitch -= (cursorY - lastCursorY) * sensitivity;
	yaw += (cursorX - lastCursorX) * sensitivity;
	glfwGetCursorPos(app->window, &lastCursorX, &lastCursorY);

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}
	if (yaw < 0.0f) {
		yaw += 360.0f;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(direction);
	right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
	up = glm::normalize(glm::cross(right, front));

	float s = speed;
	if (glfwGetKey(app->window, GLFW_KEY_LEFT_SHIFT)) {
		s *= 10.0f;
	}
	if (glfwGetKey(app->window, GLFW_KEY_W)) {
		position += s * front;
	}
	if (glfwGetKey(app->window, GLFW_KEY_S)) {
		position -= s * front;
	}
	if (glfwGetKey(app->window, GLFW_KEY_D)) {
		position += s * right;
	}
	if (glfwGetKey(app->window, GLFW_KEY_A)) {
		position -= s * right;
	}
	if (glfwGetKey(app->window, GLFW_KEY_R)) {
		position += s * up;
	}
	if (glfwGetKey(app->window, GLFW_KEY_F)) {
		position -= s * up;
	}

	view = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}
