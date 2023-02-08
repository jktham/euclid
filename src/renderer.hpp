#pragma once

#include <string>
#include <vector>

class App;
class Camera;

class Renderer {
public:
	unsigned int shader;
	unsigned int vao;
	unsigned int vbo;
	unsigned int uboSpheres;
	unsigned int uboPlanes;

	std::vector<float> vertices;
	std::vector<float> spheres;
	std::vector<float> planes;

	int bounces = 10;

	void init();
	void update();
	void draw();
	void generateBuffers();
	void updateBuffers();
	unsigned int compileShader(std::string name);
};
