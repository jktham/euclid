#version 460 core
out vec4 FragColor;

in vec2 uvPos;

uniform mat4 view;
uniform mat4 projection;

layout (std140) uniform Spheres
{
    vec3 position;
    float radius;
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

struct Sphere {
    vec4 position;
    vec4 color;
};

float sphereIntersect(Ray ray, Sphere sphere) {
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
    RayHit hit = RayHit(vec3(0.0, 0.0, 0.0), 99999.0, vec3(0.0, 0.0, 0.0), vec4(0.0, 0.0, 0.0, 0.0));
    Sphere sphere = Sphere(vec4(vec3(view * vec4(0.0, 0.0, -2.0, 1.0)), 1.0), vec4(1.0, 0.0, 0.0, 1.0));

    float result = sphereIntersect(ray, sphere);
    if (result < hit.distance && result > 0.001) {
        hit.distance = result;
        vec3 hitPos = ray.origin + ray.direction * result;
        hit.normal = normalize(hitPos - sphere.position.xyz);
        hit.color = sphere.color;
    }
    return hit;
}

void main() {
    vec2 uv = uvPos;
    uv.x *= 1920.0/1080.0;

    Ray ray = Ray(vec3(0.0, 0.0, 0.0), normalize(vec3(0.0, 0.0, -1.0) + vec3(uv, 0.0)));
    RayHit hit = trace(ray);

    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    if (hit.distance < 9999.0) {
        color = hit.color;
    }
    
    FragColor = color;
}
