#include "bezier.h"
#include "utils.h"
#include "menu.h"
#include "sphere.h"

void Bezier::setControlPoint(std::vector<std::vector<glm::vec3>> control, int nbCurve, int nbPointParCurve, int nbPointToDraw){
    Bezier::controlPoints = control;
    
    Bezier::IndicesControl = getIndiceTab(nbCurve, nbPointParCurve);
    Bezier::nbPointToDraw = nbPointToDraw;
    Bezier::pointToSegment.assign(nbPointToDraw, std::vector<glm::vec3>());
}


void Bezier::afficherMatPoint(std::vector<std::vector<glm::vec3>> mat){
    for(unsigned int i = 0; i < mat.size(); i++){
        for(unsigned int j = 0; j < mat[0].size(); j++){
            std::cout << mat[i][j][0] << "," << mat[i][j][1] << "," << mat[i][j][2] << std::endl;
        }
        std::cout << std::endl;
    }
}

void Bezier::calculateBezierSurface(){
    float t = 1.0 / (Bezier::nbPointToDraw - 1);
    int nbCurve = Bezier::controlPoints.size();
    if (nbCurve < 2){
        return;
    }
    for(int i = 0; i < Bezier::nbPointToDraw; i++){
        // Pour tous les points a dessiné 
        std::vector<glm::vec3> pointsForCurve;
        for(int j = 0; j < nbCurve; j++){
            // Pour toutes les courbes on recupere le point résultant de bezier au moment t.
            pointsForCurve.push_back(getDeCastelJauPoint((t * i), Bezier::controlPoints[j], false, Bezier::pointToSegment[i]));
        }
        Bezier::surface.push_back(getBezierCurve(pointsForCurve, Bezier::nbPointToDraw, true, Bezier::pointToSegment[i]));
    }
    Bezier::IndicesSurface = getIndiceTab(Bezier::surface.size(), Bezier::surface[0].size());
}

void Bezier::calculateNormales(){
    
    for(unsigned int i = 0; i < Bezier::surface.size(); i++){
        std::vector<glm::vec3> temp;
        for(unsigned int j = 0; j < Bezier::surface[0].size(); j++){

            glm::vec3 v1, v2, normal;
            v1 = Bezier::surface[i][j] - Bezier::pointToSegment[i][j];

            if (i == surface.size() - 1){
                v2 = Bezier::surface[i - 1][j] - Bezier::surface[i][j];
                if (j == surface.size() - 1){
                    normal = glm::cross(v2, v1);
                }else{
                    normal = glm::cross(v1, v2);
                }
            }

            else{
                v2 = Bezier::surface[i + 1][j] - Bezier::surface[i][j];
                if (j == Bezier::surface.size() - 1){
                    normal = glm::cross(v1, v2);
                }else{
                    normal = glm::cross(v2, v1);
                }
            
            }
            temp.push_back(glm::normalize(normal));
        }
        Bezier::normales.push_back(temp);
        temp.clear();
    }
}



void Bezier::calculateNormalesInv(){
    for(unsigned int i = 0; i < Bezier::surface.size(); i++){
        std::vector<glm::vec3> temp;
        for(unsigned int j = 0; j < Bezier::surface[0].size(); j++){
            glm::vec3 v1, v2, normal;
            v1 = Bezier::surface[i][j] - Bezier::pointToSegment[i][j];

            if (i == surface.size() - 1){
                v2 = Bezier::surface[i - 1][j] - Bezier::surface[i][j];
                if (j == surface.size() - 1){
                    normal = glm::cross(v1, v2);
                }else{
                    normal = glm::cross(v2, v1);
                }
            }

            else{
                v2 = Bezier::surface[i + 1][j] - Bezier::surface[i][j];
                if (j == Bezier::surface.size() - 1){
                    normal = glm::cross(v2, v1);
                }else{
                    normal = glm::cross(v1, v2);
                }
            
            }
            temp.push_back(glm::normalize(normal));
        }
        Bezier::normalesInv.push_back(temp);
        temp.clear();
    }
}


void Bezier::calculateSegment(){
    float distance = 0.2f;
    
    Bezier::segments = getSegments(Bezier::surface, Bezier::normales, distance);

    std::vector<glm::vec3> segmentsinv = getSegments(Bezier::surface, Bezier::normalesInv, distance);
    // Ajouter les segments de la première série
    Bezier::segments.insert(Bezier::segments.end(), segmentsinv.begin(), segmentsinv.end());
    for(unsigned int i = 0; i < segments.size(); i++){
        Bezier::IndicesSegments.push_back(i);
    }
}



void Bezier::calculateFinalData(){
    for(unsigned int i = 0; i < Bezier::surface.size(); i++){
        for(unsigned int j = 0; j < Bezier::surface[0].size(); j++){
            Bezier::finalData.push_back(Bezier::surface[i][j]);
            Bezier::finalData.push_back(Bezier::normales[i][j]);
            Bezier::finalData.push_back(Bezier::normalesInv[i][j]);
        }
    }
    for (unsigned int i = 0; i < Bezier::IndicesSurface.size(); i++){
        Bezier::IndicesSurface[i] = Bezier::IndicesSurface[i] * 3;
    }
}

void Bezier::draw(std::vector<glm::vec3> &control){
    if (this->update(controlP, nbCurve, nbPointParCurve, nbPointToDra)){
        control.clear();
        control = convertVec1D(controlP);
        glBindBuffer(GL_ARRAY_BUFFER, controlVBO);
        glBufferData(GL_ARRAY_BUFFER, control.size()  * sizeof(glm::vec3), control.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, controlEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesControl.size() * sizeof(int), IndicesControl.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, surfVBO);
        glBufferData(GL_ARRAY_BUFFER, finalData.size() * sizeof(glm::vec3), finalData.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surfEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesSurface.size() * sizeof(int), IndicesSurface.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, segVBO);
        glBufferData(GL_ARRAY_BUFFER, segments.size() * sizeof(glm::vec3), segments.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, segEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesControl.size() * sizeof(int), IndicesControl.data(), GL_STATIC_DRAW);
    }

    if (afficherParam)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(surfVAO);
    glDrawElements(GL_TRIANGLES, IndicesSurface.size(), GL_UNSIGNED_INT, 0);

    if (afficherPolygone){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(controlVAO);
        glBindBuffer(GL_ARRAY_BUFFER, controlVBO);
        glBufferData(GL_ARRAY_BUFFER, control.size()  * sizeof(glm::vec3), control.data(), GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, IndicesControl.size(), GL_UNSIGNED_INT, 0);
    }

    if (afficherNormale){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(segVAO);
        glDrawArrays(GL_LINES, 0, segments.size());
    }
}



glm::vec3 getValPoint(float t, glm::vec3 Point1, glm::vec3 Point2){
    return (1 - t) * Point1 + t * Point2;
}


glm::vec3 getDeCastelJauPoint(float t, std::vector<glm::vec3> controlPoints, bool normal, std::vector<glm::vec3>& pointToSegment){
    /*
    In  : t = % influence  des points de controle
          controlPoints points du polygone de controle
          normal parametre pour spécifier si on veut recuperer le dernier segment ou non.
          pointToSegment Vecteur vide pour recuperer un point du segment a l'avant derniere etape de deCastel Jau pour pouvoir calculer la normale
    Out : Le point en retour.
    */
    
    if (controlPoints.size() == 1){
        // Si il y a un seul point de control on le renvoit
        return controlPoints[0];
    }

    // Initialisation 
    int nbPointsRet = controlPoints.size();

    std::vector<glm::vec3> matricePoints;

    for(unsigned int i = 0; i < controlPoints.size(); i++){
        matricePoints.push_back(controlPoints[i]);
    }

    while (nbPointsRet != 1){

        // Tant qu'on a plus de 1 points c'est a dire au moins un segment
        if (normal && (nbPointsRet == 2)){
            if (t == 1){
                pointToSegment.push_back(matricePoints[0]);
            }else{
                pointToSegment.push_back(matricePoints[1]);
            }
        }

        for (int i = 0; i < nbPointsRet - 1; i++){
            // On recupere les points sur les segments
            matricePoints[i] = getValPoint(t, matricePoints[i], matricePoints[(i+1)]);
        }

        nbPointsRet--;
    }

    // On renvoit le seul point final.
    return matricePoints[0];
}

std::vector<glm::vec3>  getBezierCurve(std::vector<glm::vec3> controlPoints, float nbPointToDraw, bool normal, std::vector<glm::vec3>& pointToSegment){
    /*
    In  : controlPoints points du polygone de controle
          nbPointToDraw nombre de point a calculer par courbe
    Out : Courbe de bezier
    */
    float t = 1.0 /( nbPointToDraw - 1);
    std::vector<glm::vec3> curve;
    for (int i = 0; i < nbPointToDraw; i++){
        curve.push_back(getDeCastelJauPoint((i * t), controlPoints, true, pointToSegment));
    }
    return curve;
}

float getDistance(const glm::vec3& point1, const glm::vec3& point2) {
    float dx = point2.x - point1.x;
    float dy = point2.y - point1.y;
    float dz = point2.z - point1.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

std::vector<glm::vec3> getBezierCurveLenghtSeg(std::vector<glm::vec3> controlPoints,float distance){
    std::vector<glm::vec3> vide;
    std::vector<glm::vec3> bezierCurve = getBezierCurve(controlPoints, 10000, false, vide);
    std::vector<glm::vec3> curveUni;
    curveUni.push_back(bezierCurve[0]);
    for(unsigned int i = 0; i < bezierCurve.size() - 1; i++){
        if (getDistance(curveUni[curveUni.size() - 1], bezierCurve[i]) >= distance){
            curveUni.push_back(bezierCurve[i]);
        }
    }
    curveUni.push_back(bezierCurve[bezierCurve.size() - 1]);
    return curveUni;
}


std::vector<glm::vec3> getSegments(std::vector<std::vector<glm::vec3>> surface, std::vector<std::vector<glm::vec3>> normales, float distance){
    std::vector<glm::vec3> segments;
    for(unsigned int i = 0; i < surface.size(); i++){
        for(unsigned int j = 0; j < surface[0].size(); j++){
            segments.push_back(surface[i][j]);
            segments.push_back(((normales[i][j] * distance) + surface[i][j]));
        }
    }
    return segments;
}


void Bezier::clear(){
    controlPoints.clear();
    IndicesControl.clear();
    surface.clear();
    normales.clear();
    normalesInv.clear();
    finalData.clear();
    pointToSegment.clear();
    segments.clear();
}

bool Bezier::update(std::vector<std::vector<glm::vec3>> controlPoints, int nbCurve, int nbPointParCurve, int nbPointToDraw){
    if (surfaceChanged){
        this->nbPointToDraw = nbPointToDra;
        Bezier::clear();
        Bezier::pointToSegment.assign(nbPointToDraw, std::vector<glm::vec3>());
        Bezier::setControlPoint(controlPoints, nbCurve, nbPointParCurve, nbPointToDraw);
        Bezier::calculateBezierSurface();
        Bezier::calculateNormales();
        Bezier::calculateNormalesInv();
        Bezier::calculateSegment();
        Bezier::calculateFinalData();
        surfaceChanged = false;
        return true;
    }
    return false;
}

void Bezier::drawIntersection(Ray rayon){
    Sphere sphereIntersection((this->t * rayon.direction + rayon.origin), 0.03f, 36, 18);
    sphereIntersection.calculatePointsAndNormals();
    sphereIntersection.calculateIndices();
    sphereIntersection.finalData();
    sphereIntersection.draw();
}