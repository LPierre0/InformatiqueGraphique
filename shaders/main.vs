#version 330 core
layout (location = 0) in vec3 aPos;        // Position du vertex
layout (location = 1) in vec3 aNormal;     // Normale du vertex
layout (location = 2) in vec2 aTexCoord;   // Coordonnées de texture ajoutées

out vec3 FragPos;    // Position du fragment
out vec3 Normal;     // Normale du fragment
out vec2 TexCoord;   // Coordonnées de texture à passer au fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); // Calcul de la position dans l'espace monde
    Normal = aNormal;  
    TexCoord = aTexCoord;  // Passer les coordonnées de texture

    gl_Position = projection * view * vec4(FragPos, 1.0); // Transformation du vertex
}
