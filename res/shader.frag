#version 460 core

struct Plane {
	vec4 normal;
	vec4 color;
};

struct Sphere {
	vec4 position;
	vec4 color;
	vec4 bounds[2];
};

struct BobbingSphere {
	vec4 origin;
	vec4 direction;
	vec4 parameters;
	vec4 color;
	vec4 position;
	vec4 bounds[2];
};

struct Quad {
	vec4 position;
	vec4 edges[2];
	vec4 color;
	vec4 normal;
	vec4 bounds[2];
};


struct Cube {
	vec4 position;
	vec4 edges[3];
	vec4 color;
	vec4 normals[3];
	vec4 bounds[2];
};

struct Ray {
	vec3 origin;
	vec3 direction;
	vec3 inv;
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

layout (location = 0) in vec2 uvPos;

layout (location = 0) out vec4 fragColor;

layout (location = 0) uniform mat4 view;
layout (location = 1) uniform mat4 inverseView;
layout (location = 2) uniform float fov;
layout (location = 3) uniform ivec2 windowSize;
layout (location = 4) uniform float time;
layout (location = 5) uniform int bounces;
layout (location = 6) uniform int numPlanes;
layout (location = 7) uniform int numSpheres;
layout (location = 8) uniform int numBobbingSpheres;
layout (location = 9) uniform int numQuads;
layout (location = 10) uniform int numCubes;

layout (binding = 0, std140) uniform Objects {
	Plane planes[MAX_OBJECTS];
	Sphere spheres[MAX_OBJECTS];
	BobbingSphere bobbingSpheres[MAX_OBJECTS];
	Quad quads[MAX_OBJECTS];
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

bool intersectAABB(Ray ray, vec4 bounds[2]) {
	float tx0 = (bounds[0].x - ray.origin.x)*ray.inv.x;
	float tx1 = (bounds[1].x - ray.origin.x)*ray.inv.x;
	float tmin = min(tx0, tx1);
	float tmax = max(tx0, tx1);

	float ty0 = (bounds[0].y - ray.origin.y)*ray.inv.y;
	float ty1 = (bounds[1].y - ray.origin.y)*ray.inv.y;
	tmin = max(tmin, min(ty0, ty1));
	tmax = min(tmax, max(ty0, ty1));
	
	float tz0 = (bounds[0].z - ray.origin.z)*ray.inv.z;
	float tz1 = (bounds[1].z - ray.origin.z)*ray.inv.z;
	tmin = max(tmin, min(tz0, tz1));
	tmax = min(tmax, max(tz0, tz1));

	return tmax >= tmin;
}

RayHit trace(Ray ray) {
	RayHit hit;
	hit.distance = far + 1.0;
	
	for (int i=0;i<numPlanes;i++) {
		float t = intersectPlane(ray, planes[i].normal);
		if (t < hit.distance && t > near) {
			hit.distance = t;
			hit.position = ray.origin + ray.direction * hit.distance;
			hit.normal = planes[i].normal.xyz;
			hit.color = planes[i].color;
		}
	}
	for (int i=0;i<numSpheres;i++) {
		if (!intersectAABB(ray, spheres[i].bounds)) {
			continue;
		}
		float t = intersectSphere(ray, spheres[i].position);
		if (t < hit.distance && t > near) {
			hit.distance = t;
			hit.position = ray.origin + ray.direction * hit.distance;
			hit.normal = normalize(hit.position - spheres[i].position.xyz);
			hit.color = spheres[i].color;
		}
	}
	for (int i=0;i<numBobbingSpheres;i++) {
		if (!intersectAABB(ray, bobbingSpheres[i].bounds)) {
			continue;
		}
		float t = intersectSphere(ray, bobbingSpheres[i].position);
		if (t < hit.distance && t > near) {
			hit.distance = t;
			hit.position = ray.origin + ray.direction * hit.distance;
			hit.normal = normalize(hit.position - bobbingSpheres[i].position.xyz);
			hit.color = bobbingSpheres[i].color;
		}
	}
	for (int i=0;i<numQuads;i++) {
		if (!intersectAABB(ray, quads[i].bounds)) {
			continue;
		}
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
	for (int i=0;i<numCubes;i++) {
		if (!intersectAABB(ray, cubes[i].bounds)) {
			continue;
		}
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
				if (v1 < 0.0 && v2 < 0.0 && v3 < 0.0 && v4 < 0.0 && dot(ray.direction, n) < 0.0) {
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
				if (v1 < 0.0 && v2 < 0.0 && v3 < 0.0 && v4 < 0.0 && dot(ray.direction, -n) < 0.0) {
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
			vec3 rayDir = normalize(cameraDir + rayOffset * fov / 180.0 * 3.1415);
			rays[0] = Ray(cameraPos, rayDir, vec3(1.0/rayDir.x, 1.0/rayDir.y, 1.0/rayDir.z));
			hits[0] = trace(rays[0]);
		} else {
			vec3 rayDir = reflect(rays[i-1].direction, hits[i-1].normal);
			rays[i] = Ray(hits[i-1].position, rayDir, vec3(1.0/rayDir.x, 1.0/rayDir.y, 1.0/rayDir.z));
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
