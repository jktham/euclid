#version 460 core

struct Plane {
	vec4 normal;
	vec4 color;
};

struct Sphere {
	vec4 position;
	vec4 color;
};

struct Quad {
	vec4 position;
	vec4 edges[2];
	vec4 color;
	vec4 normal;
};

struct BobbingSphere {
	vec4 origin;
	vec4 direction;
	vec4 parameters;
	vec4 color;
	vec4 position;
};


struct Cube {
	vec4 position;
	vec4 edges[3];
	vec4 color;
	vec4 normals[3];
};

struct Ray {
	vec3 origin;
	vec3 direction;
};

struct RayHit {
	vec3 position;
	float distance;
	vec3 normal;
	vec4 color;
};

float far = 10000.0;
float near = 0.001;
const int MAX_OBJECTS = 100;

in vec2 uvPos;

out vec4 fragColor;

layout (location = 0) uniform mat4 view;
layout (location = 1) uniform mat4 inverseView;
layout (location = 2) uniform float fov;
layout (location = 3) uniform ivec2 windowSize;
layout (location = 4) uniform float time;
layout (location = 5) uniform int bounces;
layout (location = 6) uniform int planeCount;
layout (location = 7) uniform int sphereCount;
layout (location = 8) uniform int quadCount;
layout (location = 9) uniform int bobbingSphereCount;
layout (location = 10) uniform int cubeCount;

layout (binding = 0, std140) uniform Planes {
	Plane planes[MAX_OBJECTS];
};

layout (binding = 1, std140) uniform Spheres {
	Sphere spheres[MAX_OBJECTS];
};

layout (binding = 2, std140) uniform Quads {
	Quad quads[MAX_OBJECTS];
};

layout (binding = 3, std140) uniform BobbingSpheres {
	BobbingSphere bobbingSpheres[MAX_OBJECTS];
};

layout (binding = 4, std140) uniform Cubes {
	Cube cubes[MAX_OBJECTS];
};

float intersectPlane(Ray ray, vec4 planeNormal) {
	float a = dot(ray.direction, planeNormal.xyz);
	if (abs(a) < 0.001) {
		return -1.0;
	}
	vec3 n = planeNormal.xyz;
	vec3 p0 = planeNormal.xyz * planeNormal.w;
	vec3 l = ray.direction;
	vec3 l0 = ray.origin;
	return dot((p0-l0), n) / dot(l, n);
}

float intersectSphere(Ray ray, vec4 spherePosition) {
	float a = dot(ray.direction, ray.direction);
	vec3 offset = ray.origin - spherePosition.xyz;
	float b = 2.0 * dot(ray.direction, offset);
	float c = dot(offset, offset) - (spherePosition.w*spherePosition.w);
	if (b*b - 4.0*a*c < 0.0) {
		return -1.0;
	}
	return (-b - sqrt((b*b) - 4.0*a*c))/(2.0*a);
}

RayHit trace(Ray ray) {
	RayHit hit;
	hit.distance = far + 1.0;
	for (int i=0;i<planeCount;i++) {
		float t = intersectPlane(ray, planes[i].normal);
		if (t < hit.distance && t > near) {
			hit.distance = t;
			hit.position = ray.origin + ray.direction * hit.distance;
			hit.normal = planes[i].normal.xyz;
			hit.color = planes[i].color;
		}
	}
	for (int i=0;i<sphereCount;i++) {
		float t = intersectSphere(ray, spheres[i].position);
		if (t < hit.distance && t > near) {
			hit.distance = t;
			hit.position = ray.origin + ray.direction * hit.distance;
			hit.normal = normalize(hit.position - spheres[i].position.xyz);
			hit.color = spheres[i].color;
		}
	}
	for (int i=0;i<quadCount;i++) {
		float t = intersectPlane(ray, quads[i].normal);
		if (t < hit.distance && t > near) {
			vec3 pos = ray.origin + ray.direction * t;
			vec3 offset = pos - quads[i].position.xyz; 
			vec3 e1 = quads[i].edges[0].xyz;
			vec3 e2 = quads[i].edges[1].xyz;
			vec3 n = quads[i].normal.xyz;
			float v1 = dot(cross(e1, offset), n);
			float v2 = dot(cross(offset, e2), n);
			float v3 = dot(cross(e1, e2 - offset), n);
			float v4 = dot(cross(e1 - offset, e2), n);
			if (v1 > 0.0 && v2 > 0.0 && v3 > 0.0 && v4 > 0.0) {
				hit.distance = t;
				hit.position = ray.origin + ray.direction * hit.distance;
				hit.normal = quads[i].normal.xyz;
				hit.color = quads[i].color;
			}
		}
	}
	for (int i=0;i<bobbingSphereCount;i++) {
		float t = intersectSphere(ray, bobbingSpheres[i].position);
		if (t < hit.distance && t > near) {
			hit.distance = t;
			hit.position = ray.origin + ray.direction * hit.distance;
			hit.normal = normalize(hit.position - bobbingSpheres[i].position.xyz);
			hit.color = bobbingSpheres[i].color;
		}
	}
	for (int i=0;i<cubeCount;i++) {
		for (int j=0;j<3;j++) {
			float t = intersectPlane(ray, cubes[i].normals[j]);
			if (t < hit.distance && t > near) {
				vec3 pos = ray.origin + ray.direction * t;
				vec3 offset = pos - cubes[i].position.xyz; 
				vec3 e1 = cubes[i].edges[j].xyz;
				vec3 e2 = cubes[i].edges[(j+1)%3].xyz;
				vec3 n = cubes[i].normals[j].xyz;
				float v1 = dot(cross(e1, offset), n);
				float v2 = dot(cross(offset, e2), n);
				float v3 = dot(cross(e1, e2 - offset), n);
				float v4 = dot(cross(e1 - offset, e2), n);
				if (v1 > 0.0 && v2 > 0.0 && v3 > 0.0 && v4 > 0.0 && dot(ray.direction, n) < 0.0) {
					hit.distance = t;
					hit.position = ray.origin + ray.direction * hit.distance;
					hit.normal = cubes[i].normals[j].xyz;
					hit.color = cubes[i].color;
				}
			}
		}
		for (int j=0;j<3;j++) {
			float t = intersectPlane(ray, vec4(cubes[i].normals[j].xyz, cubes[i].normals[j].w + dot(cubes[i].normals[j], cubes[i].edges[(j+2)%3])));
			if (t < hit.distance && t > near) {
				vec3 pos = ray.origin + ray.direction * t;
				vec3 offset = pos - cubes[i].position.xyz; 
				vec3 e1 = cubes[i].edges[j].xyz;
				vec3 e2 = cubes[i].edges[(j+1)%3].xyz;
				vec3 n = cubes[i].normals[j].xyz;
				float v1 = dot(cross(e1, offset), n);
				float v2 = dot(cross(offset, e2), n);
				float v3 = dot(cross(e1, e2 - offset), n);
				float v4 = dot(cross(e1 - offset, e2), n);
				if (v1 > 0.0 && v2 > 0.0 && v3 > 0.0 && v4 > 0.0 && dot(ray.direction, n) > 0.0) {
					hit.distance = t;
					hit.position = ray.origin + ray.direction * hit.distance;
					hit.normal = cubes[i].normals[j].xyz;
					hit.color = cubes[i].color;
				}
			}
		}
	}
	if (hit.distance > far || hit.distance < near) {
		hit.distance = far + 1.0;
		hit.position = ray.origin + ray.direction * hit.distance;
		hit.normal = -ray.direction;
		hit.color = vec4(1.0, 1.0, 1.0, 1.0) * (-hit.normal.y + 1.0) / 2.0;
	}
	return hit;
}

vec4 render() {
	vec2 uv = uvPos;
	uv.y *= float(windowSize.y)/float(windowSize.x);

	vec3 cameraPos = vec3(inverseView * vec4(0.0, 0.0, 0.0, 1.0));
	vec3 cameraDir = vec3(inverseView * vec4(0.0, 0.0, -1.0, 0.0));
	vec3 rayOffset = vec3(inverseView * vec4(uv, 0.0, 0.0));

	int lastHit;
	Ray rays[100];
	RayHit hits[100];
	for (int i=0;i<bounces;i++) {
		lastHit = i;
		if (i == 0) {
			rays[0] = Ray(cameraPos, normalize(cameraDir + rayOffset * fov / 180.0 * 3.1415));
			hits[0] = trace(rays[0]);
		} else {
			rays[i] = Ray(hits[i-1].position, reflect(rays[i-1].direction, hits[i-1].normal));
			hits[i] = trace(rays[i]);
		}
		if (hits[i].distance > far || hits[i].distance < near) {
			break;
		}
	}

	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
	for (int i=lastHit;i>=0;i--) {
		color = mix(vec4(hits[i].color.xyz, 1.0), color * vec4(hits[i].color.xyz, 1.0), hits[i].color.w);
	}

	return color;
}

void main() {
	vec4 color = render();
	fragColor = color;
}
