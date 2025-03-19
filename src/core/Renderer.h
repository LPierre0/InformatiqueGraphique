#ifndef RENDERER_H
#define RENDERER_H

#include "../include.h"
#include "Object.h"

class Renderer {
private:
    Shader &shader;

public:
    Renderer(Shader &existing_shader);

    template <typename T>
    void draw(const std::vector<std::shared_ptr<T>>& objects) {
        static_assert(std::is_base_of<Object, T>::value, "T must derive from Object");


        for (const auto& obj : objects) {
            obj->bind();
            if (obj->use_texture){
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, obj->texture);
                shader.setInt("ourTexture", 0);
            }
            shader.setVec3("objectColor", obj->color);
            shader.setBool("useTexture", obj->use_texture);
            glDrawElements(GL_TRIANGLES, obj->getIndexCount(), GL_UNSIGNED_INT, 0);
        }
    }



};

#endif
