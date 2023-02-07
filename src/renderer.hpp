#pragma once

#include <glm/glm.hpp>
#include <string>

class App;

class Renderer {
public:
	unsigned int baseShader;
	unsigned int vao;
	unsigned int vbo;
	glm::mat4 projection;

	App* app;

	void init(App* app);
	void draw();
	unsigned int compileShader(std::string name);
};
