#version 460 core

out vec4 FragColor;

in vec2 uvPos;

uniform mat4 view;
uniform float fov;
uniform ivec2 windowSize;
uniform int sphereCount;
uniform int planeCount;
uniform int bounces;

float far = 10000.0;
float near = 0.001;

struct Sphere {
    vec4 position; // x, y, z, radius
    vec4 color; // r, g, b, shininess
};

layout (binding = 0, std140) uniform Spheres {
    Sphere spheres[100];
};

struct Plane {
    vec4 normal; // x, y, z, 0
    vec4 point; // x, y, z, 0
    vec4 color; // r, g, b, shininess
};

layout (binding = 1, std140) uniform Planes {
    Plane planes[100];
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

float intersectPlane(Ray ray, Plane plane) {
    float a = dot(ray.direction, plane.normal.xyz);
    if (abs(a) < 0.001) {
        return -1.0;
    }
    vec3 n = plane.normal.xyz;
    vec3 p0 = plane.point.xyz;
    vec3 l = ray.direction;
    vec3 l0 = ray.origin;
    return dot((p0-l0), n) / dot(l, n);
}

RayHit trace(Ray ray) {
    RayHit hit;
    hit.distance = far + 1.0;
    for (int i=0;i<sphereCount;i++) {
        float t = intersectSphere(ray, spheres[i]);
        if (t < hit.distance && t > near) {
            hit.distance = t;
            hit.position = ray.origin + ray.direction * hit.distance;
            hit.normal = normalize(hit.position - spheres[i].position.xyz);
            hit.color = spheres[i].color;
        }
    }
    for (int i=0;i<planeCount;i++) {
        float t = intersectPlane(ray, planes[i]);
        if (t < hit.distance && t > near) {
            hit.distance = t;
            hit.position = ray.origin + ray.direction * hit.distance;
            hit.normal = planes[i].normal.xyz;
            hit.color = planes[i].color;
        }
    }
    if (hit.distance > far || hit.distance < near) {
        hit.distance = far + 1.0;
        hit.position = ray.origin + ray.direction * hit.distance;
        hit.normal = -ray.direction;
        hit.color = vec4(0.2, 0.6, 0.9, 1.0) * (-hit.normal.y + 1.0) / 2.0;
    }
    return hit;
}

void main() {
    vec2 uv = uvPos;
    uv.y *= float(windowSize.y)/float(windowSize.x);

    vec3 cameraPos = vec3(inverse(view) * vec4(0.0, 0.0, 0.0, 1.0));
    vec3 cameraDir = vec3(inverse(view) * vec4(0.0, 0.0, -1.0, 0.0));
    vec3 rayOffset = vec3(inverse(view) * vec4(uv, 0.0, 0.0));

    int lastHit;
    Ray rays[100];
    RayHit hits[100];
    for (int i=0;i<bounces;i++) {
        lastHit = i;
        if (i == 0) {
            rays[0] = Ray(cameraPos, normalize(cameraDir + rayOffset * fov / 180.0 * 3.1415));
            hits[0] = trace(rays[0]);
        } else {
            rays[i] = Ray(hits[i-1].position, normalize(rays[i-1].direction - 2 * dot(rays[i-1].direction, hits[i-1].normal) * hits[i-1].normal));
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
    
    FragColor = color;
}
