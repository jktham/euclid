#version 460 core
out vec4 FragColor;

in vec2 uvPos;

uniform mat4 view;
uniform mat4 projection;

struct Sphere {
    vec4 position; // x, y, z, radius
    vec4 color; // r, g, b, shininess
};

layout (binding = 0, std140) uniform Spheres
{
    Sphere spheres[4];
};

struct Plane {
    vec4 normal; // x, y, z, 0
    vec4 point; // x, y, z, 0
    vec4 color; // r, g, b, shininess
};

layout (binding = 1, std140) uniform Planes
{
    Plane planes[1];
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
    return (dot(plane.normal.xyz, plane.point.xyz) - dot(plane.normal.xyz, ray.origin+ray.direction)) / dot(plane.normal.xyz, normalize(ray.direction));
}

RayHit trace(Ray ray) {
    RayHit hit;
    hit.distance = 10001.0;
    for (int i=0;i<spheres.length();i++) {
        Sphere sphere = spheres[i];
        sphere.position.xyz = vec3(view * vec4(sphere.position.xyz, 1.0));
        float t = intersectSphere(ray, sphere);
        if (t < hit.distance && t > 0.001) {
            hit.distance = t;
            hit.position = ray.origin + ray.direction * hit.distance;
            hit.normal = normalize(hit.position - sphere.position.xyz);
            hit.color = sphere.color;
        }
    }
    for (int i=0;i<planes.length();i++) {
        Plane plane = planes[i];
        plane.point.xyz = vec3(view * vec4(plane.point.xyz, 1.0));
        plane.normal.xyz = vec3(transpose(inverse(view)) * vec4(plane.normal.xyz, 1.0));
        float t = intersectPlane(ray, plane);
        if (t < hit.distance && t > 0.001) {
            hit.distance = t;
            hit.position = ray.origin + ray.direction * hit.distance;
            hit.normal = plane.normal.xyz;
            hit.color = plane.color;
        }
    }
    return hit;
}

void main() {
    vec2 uv = uvPos;
    uv.x *= 1920.0/1080.0;

    Ray ray = Ray(vec3(0.0, 0.0, 0.0), normalize(vec3(0.0, 0.0, -1.0) + vec3(uv, 0.0)));
    RayHit hit = trace(ray);

    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    if (hit.distance < 10000.0) {
        color = vec4(hit.color.rgb, 1.0);
    }
    
    FragColor = color;
}
