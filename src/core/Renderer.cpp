#include "Renderer.h"



Renderer::Renderer(Shader &existing_shader)
    : shader(existing_shader){
        glEnable(GL_DEPTH_TEST);
}

