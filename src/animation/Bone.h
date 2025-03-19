#ifndef BONE_H
#define BONE_H


#include "../include.h"


class Bone{
    public:
        Bone* parent;
        glm::mat4 transform;
        glm::vec3 position;

        
        Bone(glm::vec3 position, Bone* p = nullptr) : parent(p), transform(1.0f), position(position) {}
        
        glm::mat4 getGlobalTransform() const {
            if(parent) return parent->getGlobalTransform() * transform;
            return transform;
        }
};

#endif