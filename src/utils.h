#ifndef UTILS_H
#define UTILS_H

#include "include.h"
#include "ray.h"
#include "bezier.h"

typedef struct Vertex_s {
    glm::vec3 position;
    glm::vec3 normal;
} Vertex_t;



float ** initMatrice(int nbrow, int nbcol);
void freeMatrice(float ** matrice, int nbrow, int nbcol);
void afficherPoint(float * point);
void afficherMat(float ** matrice, int nbrow, int nbcol);
float * getValPoint(float t, float * Point1, float * Point2);
void copyVecToMat(std::vector<glm::vec3> src, float ** dest, int nbrow, int nbcol);
float * convert1D(float ** mat, int nbrow, int nbcol);
float ** convert2D(float * tab, int nbrow, int nbcol);
std::vector<int> getIndiceTab(int nbCurve, int nbPointsParCurve);
void matToVector(float **curve, std::vector<glm::vec3> &vec, int nbrow, int nbcol);
void afficherSurface(std::vector<std::vector<glm::vec3>> surface);
std::vector<glm::vec3> convertVec1D(std::vector<std::vector<glm::vec3>> vec);
void afficherVecVec3(std::vector<glm::vec3> vec);
void afficherPoint(glm::vec3 point);
std::vector<glm::vec3> getFinalData(std::vector<std::vector<glm::vec3>> surfacePos, std::vector<std::vector<glm::vec3>> normales, std::vector<std::vector<glm::vec3>> normalesinv);
bool intersectSurface(Ray rayon, Bezier surface, float &t);

#endif 
