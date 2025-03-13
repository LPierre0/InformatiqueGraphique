#version 330 core
out vec4 FragColor;

in float cutoff;
in float theta;

void main()
{
    if (theta > cutoff) FragColor = vec4(1.0);
    else FragColor = vec4(0.0):
}

