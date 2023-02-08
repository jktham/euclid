#version 460 core
out vec4 FragColor;

in vec2 uvPos;

uniform mat4 view;
uniform mat4 projection;
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
    float a = dot(normalize(ray.direction), normalize(plane.normal.xyz));
    if (abs(a) < 0.001) {
        return -1.0;
    }
    vec3 n = normalize(plane.normal.xyz);
    vec3 p = plane.point.xyz;
    vec3 d = normalize(ray.direction);
    vec3 o = ray.origin;
    return (dot(n, p) - dot(n, o+d)) / dot(n, d);
}

RayHit trace(Ray ray) {
    RayHit hit;
    hit.distance = far + 1.0;
    for (int i=0;i<sphereCount;i++) {
        Sphere sphere = spheres[i];
        sphere.position.xyz = vec3(view * vec4(sphere.position.xyz, 1.0));
        float t = intersectSphere(ray, sphere);
        if (t < hit.distance && t > near) {
            hit.distance = t;
            hit.position = ray.origin + ray.direction * hit.distance;
            hit.normal = normalize(hit.position - sphere.position.xyz);
            hit.color = sphere.color;
        }
    }
    for (int i=0;i<planeCount;i++) {
        Plane plane = planes[i];
        plane.point.xyz = vec3(view * vec4(plane.point.xyz, 1.0));
        plane.normal.xyz = normalize(vec3(transpose(inverse(view)) * vec4(plane.normal.xyz, 1.0)));
        float t = intersectPlane(ray, plane);
        if (t < hit.distance && t > near) {
            hit.distance = t;
            hit.position = ray.origin + ray.direction * hit.distance;
            hit.normal = normalize(plane.normal.xyz);
            hit.color = plane.color;
        }
    }
    if (hit.distance > far || hit.distance < near) {
        hit.distance = far + 1.0;
        hit.position = ray.origin + ray.direction * hit.distance;
        hit.normal = normalize(-ray.direction);
        hit.color = vec4(0.2, 0.6, 0.9, 1.0) * (abs(vec3(inverse(view) * vec4(hit.normal, 0.0)).y) + 1.0) / 2.0;
    }
    return hit;
}

void main() {
    vec2 uv = uvPos;
    uv.y *= float(windowSize.y)/float(windowSize.x);

    int lastHit;
    Ray rays[100];
    RayHit hits[100];
    for (int i=0;i<bounces;i++) {
        lastHit = i;
        if (i == 0) {
            rays[0] = Ray(vec3(0.0, 0.0, 0.0), normalize(vec3(0.0, 0.0, -1.0) + vec3(uv, 0.0) * fov / 180.0 * 3.1415));
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
