#version 460 core

layout (location = 0) in vec2 aPos;

layout (location = 0) out vec2 uvPos;

void main() {
	vec4 pos = vec4(aPos.x, aPos.y, 0.0, 1.0);
	uvPos = vec2(pos);
	gl_Position = pos;
}
