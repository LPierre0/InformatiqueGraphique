#include "../include.h"


glm::vec3 compute_normal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
    glm::vec3 v1 = p2 - p1;  
    glm::vec3 v2 = p3 - p1;  
    glm::vec3 normal = glm::cross(v1, v2);  

    return normal;
}

std::string get_executable_path() {
    return std::filesystem::current_path().string();
}

std::string get_texture_path(const std::string& filename) {
    return get_executable_path() + "/textures/" + filename;
}

void printMatrix(const glm::mat4& mat) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}