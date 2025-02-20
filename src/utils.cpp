#include "include.h"


glm::vec3 calculateNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
    glm::vec3 v1 = p2 - p1;  
    glm::vec3 v2 = p3 - p1;  
    glm::vec3 normal = glm::cross(v1, v2);  

    return glm::normalize(normal);
}

