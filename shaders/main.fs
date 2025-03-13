#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;  // Coordonnées de texture reçues du vertex shader
in vec4 ShadowCoord; // Coordonnées de la shadow map

uniform vec3 lightPos; 
uniform vec3 spot_direction;
uniform float cutoff;


uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;


uniform sampler2D ourTexture; // Texture à appliquer
uniform sampler2D depthMap; // Shadow map
void main()
{
    // Calcul de l'éclairage ambiant
    vec3 ambient = ambientStrength * lightColor;
    
    // Calcul de l'éclairage diffus
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  


    // Utilisation de la normale correcte en fonction de gl_FrontFacing
    float diff;
    if (gl_FrontFacing) {
        diff = max(dot(norm, lightDir), 0.0);
    } else {
        diff = max(dot(-norm, lightDir), 0.0);
    }
    
    vec3 diffuse = diff * lightColor;

    // Calcul de la visibilité pour l'ombrage
    float visibility = 1.0;
    vec4 shadowCoord = ShadowCoord;

    // Ajuste le bias en fonction de l'angle entre la normale et la direction de la lumière
    float cosTheta = clamp(dot(norm, lightDir), 0.0, 1.0);
    float bias = 0.005 * tan(acos(cosTheta)); 
    bias = clamp(bias, 0.0, 0.01);

    shadowCoord.z -= bias; // Pour éviter l'auto-occultation

    // Récupérer la profondeur à partir de la shadow map
    float depth = texture(depthMap, shadowCoord.xy).z;

    // Comparaison avec la profondeur de la shadow map
    if (shadowCoord.z > depth)
        visibility = 0.5; // Objet dans l'ombre
        


    // Récupère la couleur de la texture
    vec4 textureColor = texture(ourTexture, TexCoord);
    
    // Applique l'éclairage avant d'ajouter la texture
    float theta = dot(lightDir, normalize(-spot_direction));


    
    vec3 result = (ambient + diffuse) * objectColor;


    // Mélange avec la couleur de la texture
    result *= textureColor.rgb;

    // Applique la visibilité des ombres
    if (theta > cutoff){
        FragColor = vec4(result, 1.0) * visibility;
    }else{
        FragColor = vec4(0.0);
    }
}