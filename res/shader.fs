#version 460 core
out vec4 FragColor;

in vec2 pos;

void main() {
    FragColor = vec4(pos.x/1920.0, 0.0f, pos.y/1080.0, 1.0f);
}
