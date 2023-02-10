#version 460 core

struct Plane {
	vec4 normal; // x, y, z, offset
	vec4 color; // r, g, b, shininess
};

struct Sphere {
	vec4 position; // x, y, z, radius
	vec4 color; // r, g, b, shininess
};

struct Quad {
	Plane plane;
	vec4 position; // x, y, z, 0
	vec4 edge1; // x1, y1, z1, 0
	vec4 edge2; // x2, y2, z2, 0
};

struct BobbingSphere {
	Sphere sphere;
	vec4 direction; // x, y, z, 0
	vec4 parameters; // min, max, speed, offset
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
Plane updatedPlanes[MAX_OBJECTS];
Sphere updatedSpheres[MAX_OBJECTS];
Quad updatedQuads[MAX_OBJECTS];
BobbingSphere updatedBobbingSpheres[MAX_OBJECTS];

in vec2 uvPos;

out vec4 fragColor;

layout (location = 0) uniform mat4 view;
layout (location = 1) uniform mat4 inverseView;
layout (location = 2) uniform float fov;
layout (location = 3) uniform ivec2 windowSize;
layout (location = 4) uniform int planeCount;
layout (location = 5) uniform int sphereCount;
layout (location = 6) uniform int quadCount;
layout (location = 7) uniform int bobbingSphereCount;
layout (location = 8) uniform int bounces;
layout (location = 9) uniform float time;
layout (location = 10) uniform bool paused;

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

float intersectPlane(Ray ray, Plane plane) {
	float a = dot(ray.direction, plane.normal.xyz);
	if (abs(a) < 0.001) {
		return -1.0;
	}
	vec3 n = plane.normal.xyz;
	vec3 p0 = plane.normal.xyz * plane.normal.w;
	vec3 l = ray.direction;
	vec3 l0 = ray.origin;
	return dot((p0-l0), n) / dot(l, n);
}

float intersectSphere(Ray ray, Sphere sphere) {
	float a = dot(ray.direction, ray.direction);
	vec3 offset = ray.origin - sphere.position.xyz;
	float b = 2.0 * dot(ray.direction, offset);
	float c = dot(offset, offset) - (sphere.position.w*sphere.position.w);
	if (b*b - 4.0*a*c < 0.0) {
		return -1.0;
	}
	return (-b - sqrt((b*b) - 4.0*a*c))/(2.0*a);
}

RayHit trace(Ray ray) {
	RayHit hit;
	hit.distance = far + 1.0;
	for (int i=0;i<planeCount;i++) {
		float t = intersectPlane(ray, updatedPlanes[i]);
		if (t < hit.distance && t > near) {
			hit.distance = t;
			hit.position = ray.origin + ray.direction * hit.distance;
			hit.normal = updatedPlanes[i].normal.xyz;
			hit.color = updatedPlanes[i].color;
		}
	}
	for (int i=0;i<sphereCount;i++) {
		float t = intersectSphere(ray, updatedSpheres[i]);
		if (t < hit.distance && t > near) {
			hit.distance = t;
			hit.position = ray.origin + ray.direction * hit.distance;
			hit.normal = normalize(hit.position - updatedSpheres[i].position.xyz);
			hit.color = updatedSpheres[i].color;
		}
	}
	for (int i=0;i<quadCount;i++) {
		float t = intersectPlane(ray, updatedQuads[i].plane);
		if (t < hit.distance && t > near) {
			vec3 pos = ray.origin + ray.direction * t;
			vec3 offset = pos - updatedQuads[i].position.xyz; 
			vec3 e1 = updatedQuads[i].edge1.xyz;
			vec3 e2 = updatedQuads[i].edge2.xyz;
			vec3 n = updatedQuads[i].plane.normal.xyz;
			float v1 = dot(cross(e1, offset), n);
			float v2 = dot(cross(offset, e2), n);
			float v3 = dot(cross(e1, e2 - offset), n);
			float v4 = dot(cross(e1 - offset, e2), n);
			if (v1 > 0.0 && v2 > 0.0 && v3 > 0.0 && v4 > 0.0) {
				hit.distance = t;
				hit.position = ray.origin + ray.direction * hit.distance;
				hit.normal = updatedQuads[i].plane.normal.xyz;
				hit.color = updatedQuads[i].plane.color;
			}
		}
	}
	for (int i=0;i<bobbingSphereCount;i++) {
		float t = intersectSphere(ray, updatedBobbingSpheres[i].sphere);
		if (t < hit.distance && t > near) {
			hit.distance = t;
			hit.position = ray.origin + ray.direction * hit.distance;
			hit.normal = normalize(hit.position - updatedBobbingSpheres[i].sphere.position.xyz);
			hit.color = updatedBobbingSpheres[i].sphere.color;
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

void updateObjects() {
	for (int i=0;i<planeCount;i++) {
		updatedPlanes[i] = planes[i];
	}
	for (int i=0;i<sphereCount;i++) {
		updatedSpheres[i] = spheres[i];
	}
	for (int i=0;i<quadCount;i++) {
		updatedQuads[i] = quads[i];
		updatedQuads[i].plane.normal.xyz = normalize(cross(updatedQuads[i].edge1.xyz, updatedQuads[i].edge2.xyz));
		updatedQuads[i].plane.normal.w = dot(updatedQuads[i].position.xyz, updatedQuads[i].plane.normal.xyz);
	}
	for (int i=0;i<bobbingSphereCount;i++) {
		updatedBobbingSpheres[i] = bobbingSpheres[i];
		if (!paused) {
			float factor = updatedBobbingSpheres[i].parameters.x + 0.5*(updatedBobbingSpheres[i].parameters.y - updatedBobbingSpheres[i].parameters.x)*(1.0 + sin(updatedBobbingSpheres[i].parameters.z*time + updatedBobbingSpheres[i].parameters.w));
			updatedBobbingSpheres[i].sphere.position.xyz += updatedBobbingSpheres[i].direction.xyz * factor;
		}
	}
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
	updateObjects();
	vec4 color = render();
	fragColor = color;
}
