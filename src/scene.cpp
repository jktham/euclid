#include "scene.hpp"

#include <random>

float Scene::rnd(float min, float max) {
	return min + (float)std::rand() / ((float)RAND_MAX/(max-min));
}

void Scene::init() {
	planes.reserve(100);
	spheres.reserve(100);
	quads.reserve(100);
	cubes.reserve(100);
	volumes.reserve(100);
	lights.reserve(100);
	load(1);
}

void Scene::load(int id) {
	skyColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	planes.clear();
	spheres.clear();
	quads.clear();
	cubes.clear();
	volumes.clear();
	lights.clear();
	updaters.clear();

	if (id == 1) {
		planes.push_back(Plane(glm::vec3(0.0f, 1.0f, 0.0f), -30.0f, glm::vec4(0.5f, 0.5f, 0.5f, 0.2f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		quads.push_back(Quad(glm::vec3(0.0f, -30.0f, 0.0f), glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(0.0f, 20.0f, -50.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.1f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		glm::vec3 colors1[] = {glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.3f, 0.9f, 0.9f), glm::vec3(0.9f, 0.3f, 0.9f), glm::vec3(0.9f, 0.9f, 0.3f)};
		glm::vec3 colors2[] = {glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.9f, 0.3f, 0.3f), glm::vec3(0.3f, 0.9f, 0.3f), glm::vec3(0.3f, 0.3f, 0.9f)};
		int n = 16;
		float pi = 3.1415926f;
		for (int i=0;i<n;i++) {
			spheres.push_back(Sphere(glm::vec3( 10.0f*(i%4) + 10.0f, 0.0f, 10.0f*(i/4) - 40.0f), 3.0f, glm::vec4(colors1[i%4], 1.0f - i/4 * 0.333f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
			updaters.push_back(new BobUpdater(&spheres.back().position, glm::vec3(0.0f, 1.0f, 0.0f), -6.0f, 6.0f, 1.0f, i*2.0f*pi/(float)n));
			spheres.push_back(Sphere(glm::vec3(-10.0f*(i%4) - 10.0f, 0.0f, 10.0f*(i/4) - 40.0f), 3.0f, glm::vec4(colors2[i%4], 1.0f - i/4 * 0.333f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
			updaters.push_back(new BobUpdater(&spheres.back().position, glm::vec3(0.0f, 1.0f, 0.0f), -6.0f, 6.0f, 1.0f, i*2.0f*pi/(float)n));
		}

	} else if (id == 2) {
		planes.push_back(Plane(glm::vec3(0.0f, 1.0f, 0.0f), -10.0f, glm::vec4(0.5f, 0.5f, 0.5f, 0.2f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		int n = 30;
		float pi = 3.1415926f;
		for (int i=0;i<n;i++) {
			spheres.push_back(Sphere(glm::vec3(rnd(-50.0f, 50.0f), rnd(0.0f, 10.0f), rnd(-10.0f, -110.0f)), rnd(2.0f, 6.0f), glm::vec4(rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), 0.0f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
			updaters.push_back(new BobUpdater(&spheres.back().position, glm::vec3(0.0f, 1.0f, 0.0f), rnd(-8.0f, -2.0f), rnd(2.0f, 8.0f), rnd(0.5f, 2.0f), rnd(0.0f, 2.0f*pi)));
		}

	} else if (id == 3) {
		planes.push_back(Plane(glm::vec3(0.0f, 1.0f, 0.0f), -30.0f, glm::vec4(0.5f, 0.5f, 0.5f, 0.2f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		spheres.push_back(Sphere(glm::vec3(0.0f, 0.0f, -30.0f), 20.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		int n = 30;
		float pi = 3.1415926f;
		for (int i=0;i<30;i++) {
			float phi = i * 2.0f*pi/(float)n;
			spheres.push_back(Sphere(glm::vec3(cos(phi)*n, 0.0f, -30.0f + sin(phi)*n), 1.0f, glm::vec4(rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), 0.0f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
			updaters.push_back(new BobUpdater(&spheres.back().position, glm::vec3(0.0f, 1.0f, 0.0f), -10.0f, 10.0f, 1.0f, phi*3.0f));
		}

	} else if (id == 4) {
		quads.push_back(Quad(glm::vec3(-35.0f, -20.0f, 0.0f), glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(0.0f, 0.0f, -40.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.1f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		quads.push_back(Quad(glm::vec3( 35.0f, -20.0f, 0.0f), glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(0.0f, 0.0f, -40.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.1f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		int n = 36;
		for (int i=0;i<n;i++) {
			float pi = 3.1415926f;
			float phi = i * 2.0f*pi/(float)n;
			spheres.push_back(Sphere(glm::vec3(((n-1)/2.0f-i)*2.0f, 0.0f, -20.0f), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
			updaters.push_back(new BobUpdater(&spheres.back().position, glm::vec3(0.0f, cos(phi), sin(phi)), -10.0f, 10.0f, 1.0f, 24.0f*phi));
		}

	} else if (id == 5) {
		planes.push_back(Plane(glm::vec3(0.0f, 1.0f, 0.0f), -30.0f, glm::vec4(0.5f, 0.5f, 0.5f, 0.2f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		int n = 4;
		float w = 10.0f;
		for (int i=0;i<n;i++) {
			for (int j=0;j<n-i;j++) {
				cubes.push_back(Cube(glm::vec3(i*w, (i+j)*w - 15.0f, -j*w - 10.0f), glm::vec3(w, 0.0f, 0.0f), glm::vec3(0.0f, w, 0.0f), glm::vec3(0.0f, 0.0f, w), glm::vec4(rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), 0.1f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
			}
		}
		for (int i=0;i<n-1;i++) {
			for (int j=0;j<n-i-1;j++) {
				spheres.push_back(Sphere(glm::vec3(i*w + 5.0f, (i+j)*w, -j*w - 5.0f), 2.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
			}
		}

	} else if (id == 6) {
		int n = 6;
		float pi = 3.1415926f;
		float r = 20.0f;
		float s = 2.0f*r*sin(pi/(float)n);
		float h = s;
		for (int i=0;i<n;i++) {
			float phi = i * 2.0f*pi/(float)n;
			quads.push_back(Quad(glm::vec3(cos(phi)*r, -h/2.0f, sin(phi)*r), glm::vec3(0.0f, h, 0.0f), glm::vec3(cos(phi+pi/2.0f+pi/(float)n)*s, 0.0f, sin(phi+pi/2.0f+pi/(float)n)*s), glm::vec4(0.8f, 0.8f, 0.8f, 0.0f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		}
		quads.push_back(Quad(glm::vec3(-r, -h/2.0f, r), glm::vec3(2.0f*r, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -2.0f*r), glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		quads.push_back(Quad(glm::vec3(-r,  h/2.0f, r), glm::vec3(2.0f*r, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -2.0f*r), glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		spheres.push_back(Sphere(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));

	} else if (id == 7) {
		skyColor = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
		lights.push_back(Light(glm::vec3(0.0f, 100.0f, 50.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		updaters.push_back(new BobUpdater(&lights.back().position, glm::vec3(1.0f, 0.0f, 0.0f), -100.0f, 100.0f, 1.0f, 0.0f));
		planes.push_back(Plane(glm::vec3(0.0f, 1.0f, 0.0f), -30.0f, glm::vec4(0.4f, 0.4f, 0.4f, 0.6f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		quads.push_back(Quad(glm::vec3(0.0f, -30.0f, 0.0f), glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(0.0f, 20.0f, -50.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.1f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		glm::vec3 colors1[] = {glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.3f, 0.9f, 0.9f), glm::vec3(0.9f, 0.3f, 0.9f), glm::vec3(0.9f, 0.9f, 0.3f)};
		glm::vec3 colors2[] = {glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.9f, 0.3f, 0.3f), glm::vec3(0.3f, 0.9f, 0.3f), glm::vec3(0.3f, 0.3f, 0.9f)};
		int n = 16;
		float pi = 3.1415926f;
		for (int i=0;i<n;i++) {
			spheres.push_back(Sphere(glm::vec3( 10.0f*(i%4) + 10.0f, 0.0f, 10.0f*(i/4) - 40.0f), 3.0f, glm::vec4(colors1[i%4], 1.0f - i/4 * 0.333f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
			updaters.push_back(new BobUpdater(&spheres.back().position, glm::vec3(0.0f, 1.0f, 0.0f), -6.0f, 6.0f, 1.0f, i*2.0f*pi/(float)n));
			spheres.push_back(Sphere(glm::vec3(-10.0f*(i%4) - 10.0f, 0.0f, 10.0f*(i/4) - 40.0f), 3.0f, glm::vec4(colors2[i%4], 1.0f - i/4 * 0.333f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
			updaters.push_back(new BobUpdater(&spheres.back().position, glm::vec3(0.0f, 1.0f, 0.0f), -6.0f, 6.0f, 1.0f, i*2.0f*pi/(float)n));
		}

	} else if (id == 8) {
		skyColor = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
		lights.push_back(Light(glm::vec3(0.0f, 80.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		updaters.push_back(new CircleUpdater(&lights.back().position, glm::vec3(0.0f, 1.0f, 0.0f), 100.0f, 1.0f, 0.0f));
		spheres.push_back(Sphere(glm::vec3(0.0f, 80.0f, 0.0f), 10.0f, glm::vec4(rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f))));
		quads.push_back(Quad(glm::vec3(-200.0f, -40.0f, -200.0f), glm::vec3(400.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 400.0f), glm::vec4(0.3f, 0.3f, 0.3f, 0.6f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
		int n = 5;
		for (int i=0;i<n;i++) {
			for (int j=0;j<n;j++) {
				cubes.push_back(Cube(glm::vec3(20.0f * i - 40.0f - 5.0f, 0.0f, -20.0f * j + 40.0f - 5.0f), glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 10.0f), glm::vec4(rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f)), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
			}
		}
	} else if (id == 9) {
		lights.push_back(Light(glm::vec3(0.0f, 80.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		updaters.push_back(new CircleUpdater(&lights.back().position, glm::vec3(0.0f, 1.0f, 0.0f), 100.0f, 1.0f, 0.0f));

		planes.push_back(Plane(glm::vec3(0.0f, 1.0f, 0.0f), -30.0f, glm::vec4(0.5f, 0.5f, 0.5f, 0.2f)));
		int n = 30;
		float pi = 3.1415926f;
		float r = 30.0f;
		for (int i=0;i<n;i++) {
			float phi = i * 2.0f*pi/(float)n;
			spheres.push_back(Sphere(glm::vec3(cos(phi)*r, 0.0f, sin(phi)*r), 1.0f, glm::vec4(rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), 0.0f)));
			updaters.push_back(new BobUpdater(&spheres.back().position, glm::vec3(0.0f, 1.0f, 0.0f), -5.0f, 5.0f, 1.0f, phi*4.0f));
		}
		float w = 20.0f;
		volumes.push_back(Volume(glm::vec3(0.0f - w/2.0f, 0.0f - w, 0.0f - w/2.0f), glm::vec3(w, 0.0f, 0.0f), glm::vec3(0.0f, w*2.0f, 0.0f), glm::vec3(0.0f, 0.0f, w), glm::vec4(rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), rnd(0.0f, 1.0f), 0.03f), glm::vec4(0.1f, 0.5f, 0.5f, 32.0f)));
	}
}

void Scene::update(float time) {
	for (int i=0;i<updaters.size();i++) {
		updaters[i]->update(time);
	}
	for (int i=0;i<spheres.size();i++) {
		spheres[i].generate();
	}
	for (int i=0;i<quads.size();i++) {
		quads[i].generate();
	}
	for (int i=0;i<cubes.size();i++) {
		cubes[i].generate();
	}
}
