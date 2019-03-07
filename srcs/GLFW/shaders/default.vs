#version 410 core
layout (location = 0) in vec2 position;

uniform mat4 VP;
uniform mat4 M;

void main()
{
    gl_Position = VP * M * vec4(position.xy, 0.f, 1.0f);
}
