#ifndef RENDERER_H
#define RENDERER_H

#include "../include.h"
#include "Object.h"

class Renderer {
private:

public:
    Renderer();

    template <typename T>
    void draw(const std::vector<std::unique_ptr<T>>& objects) {
        static_assert(std::is_base_of<Object, T>::value, "T must derive from Object");

        for (const auto& obj : objects) {
            obj->bind();
            glDrawElements(GL_TRIANGLES, obj->getIndexCount(), GL_UNSIGNED_INT, 0);
            // glBindVertexArray(obj->normalsVAO);
            // glDrawArrays(GL_LINES, 0, obj->normals_lines.size() / 3);
        }
    }

};

#endif
