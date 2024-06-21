#ifndef MENU_H
#define MENU_H

#include "include.h"

extern bool menuVisible;
extern bool afficherParam;
extern bool afficherNormale;
extern bool afficherPolygone;
extern bool surfaceChanged; 
extern int typeAff;
extern int typeCourbe;
extern int nbPointToDra;
extern ImVec4 color;
extern std::vector<std::vector<glm::vec3>> controlP;
extern int nbCurve;
extern int nbPointParCurve;
extern std::vector<glm::vec3> controlPointsBezier;
extern int nbPointToDrawBezier;
extern int nbPointControlBezier;
extern bool updatedBezier;
extern float distanceBezier;

extern float radiusMenu;
extern int sectorCountMenu;
extern int stackCountMenu;
extern bool updateSphere;
extern bool rayon;
extern glm::vec3 cen;

void menu();
#endif