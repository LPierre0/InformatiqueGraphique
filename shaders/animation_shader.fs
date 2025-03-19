#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;  
in vec2 weight;

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
    
    vec3 ambient = 1.0f * lightColor;
    
    vec4 textureColor = texture(ourTexture, TexCoord);
    
    vec3 color = (weight.x == 0.0 && weight.y == 0.0) ? vec3(1.0, 1.0, 1.0) : vec3(weight.x, weight.y, 0.0);

    vec3 result = (ambient + diffuse) * color;


    if(useTexture){
        result *= textureColor.rgb;

    }


    FragColor = vec4(result, 1.0);
}