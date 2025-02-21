#ifndef RENDERER_H
#define RENDERER_H

#include "../include.h"
#include "Object.h"

class Renderer {
private:

public:
    Renderer();


    void draw(const std::vector<std::unique_ptr<Object>>& object);

};

#endif
