#version 410 core
layout (location = 0) in vec3 position;

uniform mat4 VP;
uniform mat4 M;

void main()
{
    gl_Position = VP * M * vec4(position.xyz, 1.0f);
}
