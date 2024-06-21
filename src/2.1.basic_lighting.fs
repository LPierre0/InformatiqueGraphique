#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 NormalInverse;
in vec3 FragPos;  

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;

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
    vec3 result = (ambient + diffuse) * objectColor;
    
    FragColor = vec4(result, 1.0);
}
