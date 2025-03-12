#version 330 core
layout (location = 0) in vec3 aPos;        // Position du vertex
layout (location = 1) in vec3 aNormal;     // Normale du vertex
layout (location = 2) in vec2 aTexCoord;   // Coordonnées de texture ajoutées

out vec3 FragPos;    // Position du fragment
out vec3 Normal;     // Normale du fragment
out vec2 TexCoord;   // Coordonnées de texture à passer au fragment shader
out vec4 ShadowCoord;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 depthMvp;
uniform mat4 depthBiasMvp;
uniform vec3 camera_pos;



void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    TexCoord = aTexCoord;

    gl_Position = projection * view * vec4(FragPos, 1.0);
    ShadowCoord = depthBiasMvp * vec4(aPos, 1.0);
}
