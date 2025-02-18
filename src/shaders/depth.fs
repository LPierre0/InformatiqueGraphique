#version 330 core

out float FragDepth;  // Profondeur du fragment

void main()
{
    FragDepth = gl_FragCoord.z;  // Profondeur dans l'espace lumière
}
