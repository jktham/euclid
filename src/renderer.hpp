#pragma once

#include <string>

class App;
class Camera;

class Renderer {
public:
	unsigned int baseShader;
	unsigned int vao;
	unsigned int vbo;
	unsigned int uboSpheres;
	unsigned int uboPlanes;

	App* app;
	Camera* camera;

	void init(App* app);
	void draw();
	unsigned int compileShader(std::string name);
};
