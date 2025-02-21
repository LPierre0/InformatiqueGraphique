#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 depthMvp;
void main()
{
    gl_Position = depthMvp  * vec4(aPos, 1.0);
}  