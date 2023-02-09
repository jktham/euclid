#version 460 core

struct Plane {
	vec4 normal; // x, y, z, offset
	vec4 color; // r, g, b, shininess
};

struct Sphere {
	vec4 position; // x, y, z, radius
	vec4 color; // r, g, b, shininess
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

in vec2 uvPos;

out vec4 fragColor;

uniform mat4 view;
uniform mat4 inverseView;
uniform float fov;
uniform ivec2 windowSize;
uniform int planeCount;
uniform int sphereCount;
uniform int bounces;
uniform float time;
uniform bool paused;

float far = 10000.0;
float near = 0.001;
const int MAX_OBJECTS = 100;

layout (binding = 0, std140) uniform Planes {
	Plane planes[MAX_OBJECTS];
};

layout (binding = 1, std140) uniform Spheres {
	Sphere spheres[MAX_OBJECTS];
};

Plane updatedPlanes[MAX_OBJECTS];
Sphere updatedSpheres[MAX_OBJECTS];

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
	if (hit.distance > far || hit.distance < near) {
		hit.distance = far + 1.0;
		hit.position = ray.origin + ray.direction * hit.distance;
		hit.normal = -ray.direction;
		hit.color = vec4(1.0, 1.0, 1.0, 1.0) * (-hit.normal.y + 1.0) / 2.0;
	}
	return hit;
}

void updateObjects() {
	for (int i=0;i<sphereCount;i++) {
		updatedSpheres[i] = spheres[i];
		if (!paused) {
			updatedSpheres[i].position.y += 4.0 * sin(time + float(i)*2.0*3.1415/float(sphereCount));
		}
	}
	for (int i=0;i<planeCount;i++) {
		updatedPlanes[i] = planes[i];
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
