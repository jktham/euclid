#version 460 core

struct Plane {
	vec4 normal;
	vec4 color;
	vec4 material;
};

struct Sphere {
	vec4 position;
	vec4 color;
	vec4 material;
	vec4 bounds[2];
};

struct Quad {
	vec4 position;
	vec4 edges[2];
	vec4 color;
	vec4 material;
	vec4 normal;
	vec4 bounds[2];
};

struct Cube {
	vec4 position;
	vec4 edges[3];
	vec4 color;
	vec4 material;
	vec4 normals[3];
	vec4 bounds[2];
};

struct Volume {
	vec4 position;
	vec4 edges[3];
	vec4 color;
	vec4 material;
	vec4 normals[3];
	vec4 bounds[2];
};

struct Light {
	vec4 position;
	vec4 color;
	vec4 material;
};

struct Ray {
	vec3 origin;
	vec3 direction;
	vec3 inverseDirection;
};

struct RayHit {
	vec3 position;
	float distance;
	vec3 normal;
	vec4 color;
	vec4 material;
	vec4 tint;
	bool final;
};

float far = 10000.0;
float near = 0.001;
const float PI = 3.1415926;
const int MAX_OBJECTS = 60;

layout (location = 0) in vec2 uvPos;

layout (location = 0) out vec4 fragColor;

layout (location = 0) uniform mat4 view;
layout (location = 1) uniform mat4 inverseView;
layout (location = 2) uniform float fov;
layout (location = 3) uniform ivec2 windowSize;
layout (location = 4) uniform float time;
layout (location = 5) uniform int bounces;
layout (location = 6) uniform bool reflections;
layout (location = 7) uniform bool lighting;
layout (location = 8) uniform bool shadows;
layout (location = 9) uniform vec4 skyColor;
layout (location = 10) uniform int numPlanes;
layout (location = 11) uniform int numSpheres;
layout (location = 12) uniform int numQuads;
layout (location = 13) uniform int numCubes;
layout (location = 14) uniform int numVolumes;
layout (location = 15) uniform int numLights;

layout (binding = 0, std140) uniform Objects {
	Plane planes[MAX_OBJECTS];
	Sphere spheres[MAX_OBJECTS];
	Quad quads[MAX_OBJECTS];
	Cube cubes[MAX_OBJECTS];
	Volume volumes[MAX_OBJECTS];
	Light lights[MAX_OBJECTS];
};

bool intersectAABB(Ray ray, vec4 bounds[2]) {
	float tx0 = (bounds[0].x - ray.origin.x)*ray.inverseDirection.x;
	float tx1 = (bounds[1].x - ray.origin.x)*ray.inverseDirection.x;
	float tmin = min(tx0, tx1);
	float tmax = max(tx0, tx1);

	float ty0 = (bounds[0].y - ray.origin.y)*ray.inverseDirection.y;
	float ty1 = (bounds[1].y - ray.origin.y)*ray.inverseDirection.y;
	tmin = max(tmin, min(ty0, ty1));
	tmax = min(tmax, max(ty0, ty1));
	
	float tz0 = (bounds[0].z - ray.origin.z)*ray.inverseDirection.z;
	float tz1 = (bounds[1].z - ray.origin.z)*ray.inverseDirection.z;
	tmin = max(tmin, min(tz0, tz1));
	tmax = min(tmax, max(tz0, tz1));

	return tmax >= tmin;
}

float intersectPlane(Ray ray, vec4 normal) {
	float a = dot(ray.direction, normal.xyz);
	if (abs(a) < 0.001) {
		return -1.0;
	}
	vec3 n = normal.xyz;
	vec3 p0 = normal.xyz * normal.w;
	vec3 l = ray.direction;
	vec3 l0 = ray.origin;
	return dot((p0-l0), n) / dot(l, n);
}

float intersectSphere(Ray ray, vec4 position) {
	float a = dot(ray.direction, ray.direction);
	vec3 offset = ray.origin - position.xyz;
	float b = 2.0 * dot(ray.direction, offset);
	float c = dot(offset, offset) - (position.w*position.w);
	if (b*b - 4.0*a*c < 0.0) {
		return -1.0;
	}
	return (-b - sqrt((b*b) - 4.0*a*c))/(2.0*a);
}

float intersectQuad(Ray ray, vec4 position, vec4 edges[2], vec4 normal) {
	float t = intersectPlane(ray, normal);
	vec3 pos = ray.origin + ray.direction * t;
	vec3 offset = pos - position.xyz; 
	vec3 e1 = edges[0].xyz;
	vec3 e2 = edges[1].xyz;
	vec3 n = normal.xyz;

	float v1 = dot(cross(e1, offset), n);
	float v2 = dot(cross(offset, e2), n);
	float v3 = dot(cross(e1, e2 - offset), n);
	float v4 = dot(cross(e1 - offset, e2), n);

	if (v1 > 0.0 && v2 > 0.0 && v3 > 0.0 && v4 > 0.0) {
		return t;
	}
	return -1.0;
}

RayHit trace(Ray ray) {
	RayHit hit;
	hit.distance = far + 1.0;
	hit.tint = vec4(0.0, 0.0, 0.0, 0.0);
	
	for (int i=0;i<numPlanes;i++) {
		float t = intersectPlane(ray, planes[i].normal);
		if (t < hit.distance && t > near) {
			hit.distance = t;
			hit.position = ray.origin + ray.direction * hit.distance;
			hit.normal = planes[i].normal.xyz;
			if (dot(ray.direction, hit.normal) > 0.0) {
				hit.normal = -hit.normal;
			}
			hit.color = planes[i].color;
			hit.material = planes[i].material;
			hit.final = false;
		}
	}

	for (int i=0;i<numSpheres;i++) {
		float t = intersectSphere(ray, spheres[i].position);
		if (t < hit.distance && t > near) {
			hit.distance = t;
			hit.position = ray.origin + ray.direction * hit.distance;
			hit.normal = normalize(hit.position - spheres[i].position.xyz);
			hit.color = spheres[i].color;
			hit.material = spheres[i].material;
			hit.final = false;
		}
	}

	for (int i=0;i<numQuads;i++) {
		if (!intersectAABB(ray, quads[i].bounds)) {
			continue;
		}
		float t = intersectQuad(ray, quads[i].position, quads[i].edges, quads[i].normal);
		if (t < hit.distance && t > near) {
			hit.distance = t;
			hit.position = ray.origin + ray.direction * hit.distance;
			hit.normal = quads[i].normal.xyz;
			if (dot(ray.direction, hit.normal) > 0.0) {
				hit.normal = -hit.normal;
			}
			hit.color = quads[i].color;
			hit.material = quads[i].material;
			hit.final = false;
		}
	}

	for (int i=0;i<numCubes;i++) {
		if (!intersectAABB(ray, cubes[i].bounds)) {
			continue;
		}
		for (int j=0;j<3;j++) {
			float t = intersectQuad(ray, cubes[i].position, vec4[](cubes[i].edges[j], cubes[i].edges[(j+1)%3]), cubes[i].normals[j]);
			if (t < hit.distance && t > near) {
				if (dot(ray.direction, -cubes[i].normals[j].xyz) > 0.0) {
					continue;
				}
				hit.distance = t;
				hit.position = ray.origin + ray.direction * hit.distance;
				hit.normal = -cubes[i].normals[j].xyz;
				hit.color = cubes[i].color;
				hit.material = cubes[i].material;
				hit.final = false;
			}
		}
		for (int j=0;j<3;j++) {
			float t = intersectQuad(ray, cubes[i].position + cubes[i].edges[(j+2)%3], vec4[](cubes[i].edges[j], cubes[i].edges[(j+1)%3]), vec4(cubes[i].normals[j].xyz, cubes[i].normals[j].w + dot(cubes[i].normals[j].xyz, cubes[i].edges[(j+2)%3].xyz)));
			if (t < hit.distance && t > near) {
				if (dot(ray.direction, cubes[i].normals[j].xyz) > 0.0) {
					continue;
				}
				hit.distance = t;
				hit.position = ray.origin + ray.direction * hit.distance;
				hit.normal = cubes[i].normals[j].xyz;
				hit.color = cubes[i].color;
				hit.material = cubes[i].material;
				hit.final = false;
			}
		}
	}

	for (int i=0;i<numLights;i++) {
		vec3 pos = lights[i].position.xyz - ray.origin;
		if (hit.distance > length(pos) && dot(ray.direction, normalize(pos)) > 0.9999) {
			hit.distance = length(pos);
			hit.position = ray.origin + ray.direction * length(pos);
			hit.normal = -normalize(pos);
			hit.color = vec4(lights[i].color.rgb, 1.0f);
			hit.material = vec4(0.0, 0.0, 0.0, 0.0);
			hit.final = true;
		}
	}

	for (int i=0;i<numVolumes;i++) {
		if (!intersectAABB(ray, volumes[i].bounds)) {
			continue;
		}
		float t[6];
		for (int j=0;j<3;j++) {
			t[j] = intersectQuad(ray, volumes[i].position, vec4[](volumes[i].edges[j], volumes[i].edges[(j+1)%3]), volumes[i].normals[j]);
		}
		for (int j=0;j<3;j++) {
			t[j+3] = intersectQuad(ray, volumes[i].position + volumes[i].edges[(j+2)%3], vec4[](volumes[i].edges[j], volumes[i].edges[(j+1)%3]), vec4(volumes[i].normals[j].xyz, volumes[i].normals[j].w + dot(volumes[i].normals[j].xyz, volumes[i].edges[(j+2)%3].xyz)));
		}
		float s[2];
		int k = 0;
		for (int j=0;j<6;j++) {
			if (t[j] < hit.distance && t[j] > near && k < 2) {
				s[k] = t[j];
				k++;
			}
		}
		if (k == 1) {
			s[1] = 0.0;
			k++;
		}
		if (k == 2) {
			float d = abs(s[0] - s[1]);
			hit.tint = vec4(volumes[i].color.rgb, min(d * volumes[i].color.a, 1.0));
		}
	}

	if (hit.distance > far || hit.distance < near) {
		hit.distance = far + 1.0;
		hit.position = ray.origin + ray.direction * hit.distance;
		hit.normal = -ray.direction;
		float skyAngle = (-hit.normal.y + 1.0) / 2.0;
		hit.color = vec4(mix(skyColor.rgb*skyColor.a, skyColor.rgb, skyAngle), 1.0);
		hit.material = vec4(0.0, 0.0, 0.0, 0.0);
		hit.final = true;
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

	vec3 rayDir = normalize(cameraDir + rayOffset * fov / 180.0 * PI);
	rays[0] = Ray(cameraPos, rayDir, vec3(1.0/rayDir.x, 1.0/rayDir.y, 1.0/rayDir.z));
	hits[0] = trace(rays[0]);

	if (reflections && !hits[0].final) {
		for (int i=1;i<bounces;i++) {
			lastHit = i;
			rayDir = reflect(rays[i-1].direction, hits[i-1].normal);
			rays[i] = Ray(hits[i-1].position, rayDir, vec3(1.0/rayDir.x, 1.0/rayDir.y, 1.0/rayDir.z));
			hits[i] = trace(rays[i]);
			if (hits[i].final) {
				break;
			}
		}
	}
	
	if (lighting && numLights > 0) {
		vec3 prevPos = cameraPos;
		for (int i=0;i<=lastHit;i++) {
			if (hits[i].final) {
				continue;
			}
			vec3 sum = vec3(0.0, 0.0, 0.0);
			for (int j=0;j<numLights;j++) {
				vec3 lightDir = normalize(lights[j].position.xyz - hits[i].position);
				vec3 viewDir = normalize(prevPos - hits[i].position);
				vec3 reflectDir = reflect(-lightDir, hits[i].normal);
				vec3 halfwayDir = normalize(lightDir + viewDir);

				float diffuseFactor = max(dot(hits[i].normal, lightDir), 0.0);
				// float specularFactor = max(dot(viewDir, reflectDir), 0.0) * max(sign(diffuseFactor), 0.0);
				float specularFactor = max(dot(hits[i].normal, halfwayDir), 0.0) * max(sign(diffuseFactor), 0.0);

				if (shadows && diffuseFactor + specularFactor > 0.0) {
					Ray shadowRay = Ray(hits[i].position, lightDir, vec3(1.0/lightDir.x, 1.0/lightDir.y, 1.0/lightDir.z));
					RayHit shadowHit = trace(shadowRay);
					if (shadowHit.distance < length(lights[j].position.xyz - hits[i].position)) {
						diffuseFactor = 0.0;
						specularFactor = 0.0;
					}
				}

				vec3 ambient = lights[j].color.rgb * hits[i].material.x * lights[j].material.x;
				vec3 diffuse = lights[j].color.rgb * diffuseFactor * hits[i].material.y * lights[j].material.y;
				vec3 specular = lights[j].color.rgb * pow(specularFactor, hits[i].material.w * lights[j].material.w * 2.0) * hits[i].material.z * lights[j].material.z;
				vec3 phong = (ambient + diffuse + specular) * hits[i].color.rgb;

				sum += phong;
			}
			hits[i].color = vec4(mix(hits[i].color.rgb, sum, hits[i].color.a), hits[i].color.a);
			prevPos = hits[i].position;
		}
	}

	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

	for (int i=lastHit;i>=0;i--) {
		color = color * hits[i].color;
		color = vec4(mix(color.rgb, hits[i].color.rgb, hits[i].color.a), 1.0);
		color = vec4(mix(color.rgb, hits[i].tint.rgb, hits[i].tint.a), 1.0);
	}

	return color;
}

void main() {
	vec4 color = render();
	fragColor = color;
}
