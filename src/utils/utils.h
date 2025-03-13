#ifndef UTILS_H
#define UTILS_H

glm::vec3 compute_normal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
std::string get_texture_path(const std::string& filename);
void printMatrix(const glm::mat4& mat);

#endif