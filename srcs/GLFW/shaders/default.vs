#version 410 core
layout (location = 0) in vec4 position;

// uniform mat4 M;
// uniform mat4 VP;

// out vec3 wPos;

void main()
{
    // gl_Position = VP * /* M * */ position;
    gl_Position = vec4(position.xyz, 1.0f);
    // wPos = position.xyz;
}
