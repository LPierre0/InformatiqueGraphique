#version 330 core
layout (location = 0) in vec3 aPos;        // Position du vertex
layout (location = 1) in vec3 aNormal;     // Normale du vertex
layout (location = 2) in vec2 aTexCoord;   // Coordonnées de texture ajoutées
layout (location = 3) in vec2 Weights;   // Coordonnées de texture ajoutées



out vec3 FragPos;    // Position du fragment
out vec3 Normal;     // Normale du fragment
out vec2 TexCoord;
out vec2 weight;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform_1;
uniform mat4 transform_2;

uniform bool use_gpu;

void main()
{
    vec3 transformed_point = vec3(0.0);

    if (use_gpu){
        vec4 transformed_1 = transform_1 * vec4(aPos, 1.0);
        vec4 transformed_2 = transform_2 * vec4(aPos, 1.0);

        // Combinaison des transformations avec les poids
        transformed_point = Weights.x * vec3(transformed_1) + Weights.y * vec3(transformed_2);

        FragPos = vec3(model * vec4(transformed_point, 1.0));
    }
    else{
        FragPos = vec3(model * vec4(aPos, 1.0));
    }
    
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    weight = Weights;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
