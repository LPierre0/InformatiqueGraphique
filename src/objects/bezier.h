#ifndef BEZIER_H
#define BEZIER_H


#include "../include.h"
#include "ray.h"
#include "../menu.h"
#include "sphere.h"

glm::vec3 getValPoint(float t, glm::vec3 Point1, glm::vec3 Point2);
glm::vec3 getDeCastelJauPoint(float t, std::vector<glm::vec3> controlPoints, bool normal, std::vector<glm::vec3>& pointToSegment);
std::vector<glm::vec3>  getBezierCurve(std::vector<glm::vec3> controlPoints, float nbPointToDraw, bool normal, std::vector<glm::vec3>& pointToSegment);
std::vector<std::vector<glm::vec3>> getBezierSurface(std::vector<std::vector<glm::vec3>> controlPoints, std::vector<std::vector<glm::vec3>>& pointToSegment, int nbPointToDraw);
std::vector<std::vector<glm::vec3>> getNormales(std::vector<std::vector<glm::vec3>> surface, std::vector<std::vector<glm::vec3>> pointToSegment);
std::vector<glm::vec3> getSegments(std::vector<std::vector<glm::vec3>> surface, std::vector<std::vector<glm::vec3>> normales, float distance);
std::vector<std::vector<glm::vec3>> getNormalesInv(std::vector<std::vector<glm::vec3>> surface, std::vector<std::vector<glm::vec3>> pointToSegment);
std::vector<glm::vec3> getBezierCurveLenghtSeg(std::vector<glm::vec3> controlPoints,float distance);

class Bezier{
    public :
        unsigned int surfVAO;
        unsigned int surfVBO;
        unsigned int surfEBO;
        unsigned int controlVAO;
        unsigned int controlVBO;
        unsigned int controlEBO;
        unsigned int segVAO;
        unsigned int segVBO;
        unsigned int segEBO;
        int nbPointToDraw;
        std::vector<std::vector<glm::vec3>> controlPoints;

        std::vector<int> IndicesControl;
        std::vector<int> IndicesSurface;
        std::vector<int> IndicesSegments;

        std::vector<std::vector<glm::vec3>> surface;
        std::vector<std::vector<glm::vec3>> normales;
        std::vector<std::vector<glm::vec3>> normalesInv;
        std::vector<glm::vec3> finalData;
        std::vector<std::vector<glm::vec3>> pointToSegment;
        std::vector<glm::vec3> segments;

        void setControlPoint(std::vector<std::vector<glm::vec3>> control, int nbCurve, int nbPointParCurve, int nbPointToDraw);
        void afficherMatPoint(std::vector<std::vector<glm::vec3>> mat);
        void calculateBezierSurface();
        void calculateNormales();
        void calculateNormalesInv();
        void calculateSegment();
        void calculateFinalData();
        bool update(std::vector<std::vector<glm::vec3>> control, int nbCurve, int nbPointParCurve, int nbPointToDraw);
        void clear();
        void draw(std::vector<glm::vec3> &control);
        float t;
        void drawIntersection(Ray rayon);
};

#endif