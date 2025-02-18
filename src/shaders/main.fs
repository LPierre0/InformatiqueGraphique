#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 NormalInverse;
in vec3 FragPos;
in vec4 FragPosLightSpace;  // Position du fragment dans l'espace lumière

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;
uniform sampler2D shadowMap;  // Shadow map
uniform mat4 lightSpaceMatrix;  // Matrice projetée de la lumière

// Fonction de calcul de l'ombre
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // Convertir la position dans l'espace lumière de [-1, 1] à [0, 1]
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // Récupérer la profondeur de la shadow map
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // Profondeur actuelle du fragment
    float currentDepth = projCoords.z;

    // Tester si le fragment est dans l'ombre
    if (currentDepth > closestDepth)
        return 0.5;  // Dans l'ombre
    else
        return 1.0;  // Pas dans l'ombre
}

void main()
{
    // Calcul de l'éclairage ambiant
    vec3 ambient = ambientStrength * lightColor;
  
    // Calcul de l'éclairage diffus
    vec3 norm = normalize(Normal);
    vec3 normInv = normalize(NormalInverse);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Utilisation de la normale correcte en fonction de gl_FrontFacing
    float diff;
    if (gl_FrontFacing) {
        diff = max(dot(norm, lightDir), 0.0);
    } else {
        diff = max(dot(normInv, lightDir), 0.0);
    }
    
    vec3 diffuse = diff * lightColor;
    
    // Calcul de l'ombre
    float shadow = ShadowCalculation(FragPosLightSpace);
    
    // Calcul de la couleur finale
    vec3 result = (ambient + diffuse) * objectColor * shadow;
    
    FragColor = vec4(result, 1.0);
}
