#include "Renderer.h"



Renderer::Renderer(){
    glEnable(GL_DEPTH_TEST);
}


void Renderer::draw(const Object& object){
    object.bind();
    glDrawElements(GL_TRIANGLES, object.getIndexCount(), GL_UNSIGNED_INT, 0);
}