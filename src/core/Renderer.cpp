#include "Renderer.h"



Renderer::Renderer(){
    glEnable(GL_DEPTH_TEST);
}


void Renderer::draw(const std::vector<std::unique_ptr<Object>>& objects){
    for (auto& object : objects){
        object->bind();
        glDrawElements(GL_TRIANGLES, object->getIndexCount(), GL_UNSIGNED_INT, 0);
    }
}