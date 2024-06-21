#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aNormalInverse; // Nouvel attribut pour la normale vers l'intérieur

out vec3 FragPos;
out vec3 Normal;
out vec3 NormalInverse; // Passer la normale vers l'intérieur au fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;  
    NormalInverse = aNormalInverse; // Passage de la normale vers l'intérieur
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
