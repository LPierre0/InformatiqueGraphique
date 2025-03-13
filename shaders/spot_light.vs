#version 330 core
layout (location = 0) in vec3 aPos;

vec3 light_pos;
vec3 light_direction;
float cutoff;

out float theta;
out float cutoff;

void main()
{
    vec3 lightDir = normalize(-light_direction);
    float theta = dot(lightDir, normalize(-light_direction));
}