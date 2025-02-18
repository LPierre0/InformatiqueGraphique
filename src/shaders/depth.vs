#version 330 core

layout (location = 0) in vec3 aPos;  // Position du vertex

uniform mat4 lightSpaceMatrix;  // Matrice projetée de la lumière

void main()
{
    gl_Position = lightSpaceMatrix * vec4(aPos, 1.0);
}
