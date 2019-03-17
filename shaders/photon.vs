#version 410 core
layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform float aPointSize;

void main()
{
    gl_Position = model * vec4(aPos, 0.0, 1.0f);
    gl_PointSize = aPointSize;
}
