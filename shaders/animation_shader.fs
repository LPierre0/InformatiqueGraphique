#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;  

uniform vec3 objectColor;

uniform vec3 lightPos;

uniform sampler2D ourTexture;
uniform bool useTexture;
void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); 
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

    float diff;
    if (gl_FrontFacing) {
        diff = max(dot(norm, lightDir), 0.0);
    } else {
        diff = max(dot(-norm, lightDir), 0.0);
    }
    
    vec3 diffuse = diff * lightColor;
    
    vec3 ambient = 0.3f * lightColor;
    
    vec4 textureColor = texture(ourTexture, TexCoord);
    
    vec3 result = (ambient + diffuse) * objectColor;


    if(useTexture){
        result *= textureColor.rgb;

    }


    FragColor = vec4(result, 1.0);
}