#include "utils.h"
#include "bezier.h"
#include "ray.h"

float ** initMatrice(int nbrow, int nbcol){
    float ** matrice = new float*[nbrow];
    for (int i = 0; i < nbrow; i++){
        matrice[i] = new float[nbcol];
    }
    return matrice;
}

void freeMatrice(float ** matrice, int nbrow, int nbcol){
    for (int i = 0; i < nbrow; i++){
        delete[] matrice[i];
    }
    delete[] matrice;
}


void afficherPoint(float * point){
    for (int i = 0; i < 3; i++){
        std::cout << point[i] << ",";
    }
    std::cout <<std::endl;
}

void afficherMat(float ** matrice, int nbrow, int nbcol){
    for (int i = 0; i < nbrow; i++){
        afficherPoint(matrice[i]);
    }
}

float * getValPoint(float t, float * Point1, float * Point2){
    float * pointret = new float[3];
    for (int i = 0; i < 3; i++){
        pointret[i] = (1 - t) * Point1[i] + t * Point2[i];
    }
    return pointret;
}


void copyVecToMat(std::vector<glm::vec3> src, float ** dest, int nbrow, int nbcol){
    for(int i = 0; i < nbrow; i++){
        for(int j = 0; j < nbcol; j++){
            dest[i][j] = src[i][j];
        }
    }
}

float * convert1D(float ** mat, int nbrow, int nbcol){
    float * tab1D = new float[nbcol * nbrow];
    for (int i = 0; i < nbrow; i++) {
        for (int j = 0; j < nbcol; j++) {
            tab1D[i * nbcol + j] = mat[i][j];
        }
    }
    return tab1D;
}

float ** convert2D(float * tab, int nbrow, int nbcol){
    float ** mat = initMatrice(nbrow, nbcol);
    for (int i = 0; i < nbrow; i++) {
        for (int j = 0; j < nbcol; j++) {
            mat[i][j] = tab[i * nbcol + j];
        }
    }
    return mat;
}

std::vector<int> getIndiceTab(int nbCurve, int nbPointsParCurve){
    // Fonction qui a partir d'un nombre de courbe et du nombre de points par courbe, renvoit un tableau correspondant aux indices des différents triangles.
    bool inv = false;
    std::vector<int> dynamicIndices;
    for(int j = 0; j < nbCurve - 1; j++){
        int i = 0;
        while(i < nbPointsParCurve){
            int num = (j * nbPointsParCurve) + i;
            // If et else permet de passer une fois sur deux sur le points pour calculer tous les triangles.
            if (!inv){
                dynamicIndices.push_back(num);
                dynamicIndices.push_back(num + 1);
                dynamicIndices.push_back(num + nbPointsParCurve);
                inv = true;
                i++;
            }
            else{
                dynamicIndices.push_back(num);
                dynamicIndices.push_back(num + nbPointsParCurve);
                dynamicIndices.push_back(num + nbPointsParCurve - 1);
                inv = false;
                if(i == nbPointsParCurve - 1) i++;
            }

        }
    }
    return dynamicIndices;
}


void matToVector(float **curve, std::vector<glm::vec3> &vec, int nbrow, int nbcol) {
    for (int i = 0; i < nbrow; i++) {
        glm::vec3 temp(curve[i][0], curve[i][1], curve[i][2]);
        vec.push_back(temp);
    }
}


void afficherSurface(std::vector<std::vector<glm::vec3>> surface){
    for(unsigned int i = 0; i < surface.size(); i++){
        for(unsigned int j = 0; j < surface[0].size(); j++){
            for(int k = 0; k < 3; k++){
                std::cout << surface[i][j][k] << ", " ;
            }
            std::cout << std::endl;
        }
    }
}

std::vector<glm::vec3> convertVec1D(std::vector<std::vector<glm::vec3>> vec){
    std::vector<glm::vec3> res;
    for (unsigned int i = 0; i < vec.size(); i++){
        for(unsigned int j = 0; j < vec[i].size(); j++){
            res.push_back(vec[i][j]);
        }
    }
    return res;
}

void afficherVecVec3(std::vector<glm::vec3> vec){
    for(unsigned int i = 0; i < vec.size(); i++){
        for(int j = 0; j < 3; j++){
            std::cout << vec[i][j] << ",";
        }
        std::cout << std::endl;
    }
}

void afficherPoint(glm::vec3 point){
    std::cout << point[0] << ", "<< point[1] << ", " << point[2] << std::endl;
}


std::vector<glm::vec3> getFinalData(std::vector<std::vector<glm::vec3>> surfacePos, std::vector<std::vector<glm::vec3>> normales, std::vector<std::vector<glm::vec3>> normalesinv){
    std::vector<glm::vec3> finaldata;
    for(unsigned int i = 0; i < surfacePos.size(); i++){
        for(unsigned int j = 0; j < surfacePos[0].size(); j++){
            finaldata.push_back(surfacePos[i][j]);
            finaldata.push_back(normales[i][j]);
            finaldata.push_back(normalesinv[i][j]);
        }
    }
    return finaldata;
}



bool rayTriangleIntersect(Ray rayon, std::vector<glm::vec3> triangle, float &t){
    // Compute the plane's normal

    float kEpsilon = 1e-6; 

    glm::vec3 v0v1 = triangle[1] - triangle[0];
    glm::vec3 v0v2 = triangle[2] - triangle[0];
    // No need to normalize
    glm::vec3 N = glm::cross(v0v1, v0v2); // N
 
    // Step 1: Finding P
    
    // Check if the ray and plane are parallel
    float NdotRayDirection = glm::dot(N, rayon.direction);
    if (fabs(NdotRayDirection) < kEpsilon) // Almost 0
        return false; // They are parallel, so they don't intersect!

    // Compute d parameter using equation 2
    float d = - glm::dot(N,triangle[0]);
    
    // Compute t (equation 3)
    t = -((glm::dot(N,rayon.origin)) + d) / NdotRayDirection;
    
    // Check if the triangle is behind the ray
    if (t < 0) return false; // The triangle is behind
 
    // Compute the intersection point using equation 1
    glm::vec3 P = rayon.origin + t * rayon.direction;
 
    // Step 2: Inside-Outside Test
    glm::vec3 C; // Vector perpendicular to triangle's plane
 
    // Edge 0
    glm::vec3 edge0 = triangle[1] - triangle[0]; 
    glm::vec3 vp0 = P - triangle[0];
    C = glm::cross(edge0,vp0);
    if (glm::dot(N,C) < 0) return false; // P is on the right side
 
    // Edge 1
    glm::vec3 edge1 = triangle[2] - triangle[1]; 
    glm::vec3 vp1 = P - triangle[1];
    C = glm::cross(edge1, vp1);
    if (glm::dot(N,C) < 0) return false; // P is on the right side
 
    // Edge 2
    glm::vec3 edge2 = triangle[0] - triangle[2]; 
    glm::vec3 vp2 = P - triangle[2];
    C = glm::cross(edge2,vp2);
    if (glm::dot(N,C) < 0) return false; // P is on the right side

    return true; // This ray hits the triangle
}


bool intersectSurface(Ray rayon, Bezier surface, float &t){
    std::vector<glm::vec3> triangle;
    for (unsigned int i = 0; i < surface.IndicesSurface.size(); i+= 3 ){
        int index0 = surface.IndicesSurface[i];
        int index1 = surface.IndicesSurface[i + 1];
        int index2 = surface.IndicesSurface[i + 2];

        triangle.push_back(surface.finalData[index0]);
        triangle.push_back(surface.finalData[index1]);
        triangle.push_back(surface.finalData[index2]);
        if (rayTriangleIntersect(rayon, triangle, t)) {
            return true;
        }
        triangle.clear();
    }
    return false;
}