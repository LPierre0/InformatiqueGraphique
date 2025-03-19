#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;  // Coordonnées de texture reçues du vertex shader
in vec4 ShadowCoord; // Coordonnées de la shadow map

uniform vec3 lightPos; 
uniform vec3 spot_direction;
uniform float inner_cutoff;
uniform float outer_cutoff;



uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;


uniform sampler2D ourTexture; // Texture à appliquer
uniform sampler2D depthMap; // Shadow map
uniform bool useTexture;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main()
{


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
    shadowCoord /= shadowCoord.w; // Correction de la perspective


    // Ajuste le bias en fonction de l'angle entre la normale et la direction de la lumière
    float cosTheta = clamp(dot(norm, lightDir), 0.0, 1.0);
    float bias = max(0.005 * (1.0 - cosTheta), 0.001);
    bias = clamp(bias, 0.0, 0.01);

    shadowCoord.z -= bias; // Pour éviter l'auto-occultation

    // Récupérer la profondeur à partir de la shadow map
    float depth = texture(depthMap, shadowCoord.xy).r; // Utiliser le canal rouge


    // Comparaison avec la profondeur de la shadow map
    for (int i=0;i<4;i++){
        int index = int(16.0*random(floor(FragPos.xyz*1000.0), i))%16;
        float shadowValue = texture(depthMap, shadowCoord.xy + poissonDisk[index] / 700.0).r;
        visibility -= 0.2 * (shadowCoord.z > shadowValue ? 1.0 : 0.0);
    }
        


    // Récupère la couleur de la texture
    vec4 textureColor = texture(ourTexture, TexCoord);
    
    // Applique l'éclairage avant d'ajouter la texture
    float theta = dot(lightDir, normalize(-spot_direction));


    
    vec3 result = (ambient + diffuse) * objectColor;


    // Mélange avec la couleur de la texture
    if (useTexture) result *= textureColor.rgb;

    // Applique la visibilité des ombres

    if (theta > inner_cutoff){
        FragColor = vec4(result, 1.0) * visibility;
    }else{
        float epsilon = inner_cutoff - outer_cutoff;
        float intensity = clamp((theta - outer_cutoff) / epsilon, 0.0, 1.0);
        FragColor = vec4(result, 1.0) * intensity * visibility;
    }
}
