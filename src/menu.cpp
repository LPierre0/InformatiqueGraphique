#include "menu.h"
#include <cstdio>
#include <iostream>

bool menuVisible = true;
bool afficherParam = false;
bool afficherNormale = false;
bool surfaceChanged = false;
bool afficherPolygone = true;
ImVec4 color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f); // Gris clair

std::vector<std::vector<glm::vec3>> controlP = {
    // Début de la première courbe
    {
        // Points de la première courbe
        { -1.0f, -1.0f, -1.0f },
        { -0.5f,  0.0f, -1.0f },
        {  0.5f,  0.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f }
    },
    // Début de la deuxième courbe
    {
        // Points de la deuxième courbe
        { -1.0f, -1.0f, -0.0f },
        { -0.5f,  0.0f, -0.0f },
        {  0.5f,  0.0f, -0.0f },
        {  1.0f, -1.0f, -0.0f }
    },
    // Début de la troisième courbe
    {
        // Points de la troisième courbe
        { -1.0f, -1.0f, 1.0f },
        { -0.5f,  0.0f, 1.0f },
        {  0.5f,  0.0f, 1.0f },
        {  1.0f, -1.0f, 1.0f }
    }
    // Vous pouvez ajouter plus de courbes si nécessaire en continuant ce modèle
};

int nbCurve = 3;
int nbPointParCurve = 4;

std::vector<glm::vec3> controlPointsBezier = {
    // Début de la première courbe
    {
        // Points de la première courbe
        { -1.0f, -1.0f, 0.0f },
        { -0.5f,  0.0f, 0.0f },
        {  0.5f,  0.0f, 0.0f },
        {  1.0f, -1.0f, 0.0f }
    }
};


int typeAff = 0;
int typeCourbe = 0;
int nbPointToDra = 10;
int nbPointToDrawBezier = 10;
int nbPointControlBezier = 4;
bool updatedBezier = false;
float distanceBezier = 0.5f;



float radiusMenu = 1.0f;
int sectorCountMenu = 36;
int stackCountMenu = 18;
bool updateSphere = false;
glm::vec3 cen = {1.0f, 1.0f, 1.0f}; 
 

bool rayon = false;

void choixMode(){
    ImGui::Text("Choissisez le type d'affichage");
    ImGui::Combo("##Type d'affichage", reinterpret_cast<int*>(&typeAff), "BezierCurve\0BezierSurface\0Sphere\0Scene\0");
    ImGui::NewLine();
}




void addPointBezier(){
    controlPointsBezier.push_back(glm::vec3(0.0f));
    nbPointControlBezier++;
    updatedBezier = true;
}

void delPointBezier(){
    if(controlPointsBezier.size() > 2){
        controlPointsBezier.pop_back();
        nbPointControlBezier--;
        updatedBezier = true;
    }
}

void modeSphere(){

    ImGui::NewLine();
    ImGui::ColorEdit3("Couleur", (float*) &color);
    ImGui::NewLine();

    float prev = radiusMenu;
    ImGui::SliderFloat("Radius", &radiusMenu, 0.2f, 2.0f);
    if (prev != radiusMenu){
        updateSphere = true;
    }
    ImGui::NewLine();
    int prevSec = sectorCountMenu;
    ImGui::SliderInt("Sector count", &sectorCountMenu, 4, 200);
    if (prevSec != sectorCountMenu){
        updateSphere = true;
    }
    ImGui::NewLine();
    int prevStack = stackCountMenu;
    ImGui::SliderInt("Stack count", &stackCountMenu, 4, 200);
    if (prevStack != stackCountMenu){
        updateSphere = true;
    }


    glm::vec3 previousCenter = cen;
    ImGui::SliderFloat3("Centre de la sphère", glm::value_ptr(cen), -1.0f, 1.0f);
    if (previousCenter != cen){
        updateSphere = true;
    }
}

void modeCourbe(){
    ImGui::Text("Quel type de courbe ?");
    ImGui::NewLine();
    ImGui::Combo("##Type de courbe", reinterpret_cast<int*>(&typeCourbe), "Uniforme nombre segment\0Uniforme par longueur\0");
    ImGui::NewLine();
    ImGui::ColorEdit3("Couleur", (float*) &color);


    if (typeCourbe == 0){
        ImGui::NewLine();
        int previousP = nbPointToDrawBezier;
        ImGui::SliderInt("NbPoint", &nbPointToDrawBezier, 2, 100);
        if (previousP != nbPointToDrawBezier){
            updatedBezier = true;
        }
        ImGui::NewLine();
    }else{
        float previousDist = distanceBezier;
        ImGui::SliderFloat("Distance", &distanceBezier, 0.01f, 1.0f);
        if (previousDist != distanceBezier){
            updatedBezier = true;
        }
        ImGui::NewLine();
    }



    if (ImGui::CollapsingHeader("Points de la courbe")) {
        for(unsigned int i = 0; i < controlPointsBezier.size(); i++){
            char label[32];
            snprintf(label, sizeof(label), "Point %d", i + 1);
            glm::vec3 prev = controlPointsBezier[i];
            ImGui::InputFloat3(label, glm::value_ptr(controlPointsBezier[i]));
            if (prev != controlPointsBezier[i]) updatedBezier = true;
        }
    }

    ImGui::NewLine();
    if (ImGui::Button("Ajouter un point"))
        addPointBezier();
    if(ImGui::Button("Supprimer un point"))
        delPointBezier();


}


void addCurve() {
    // Ajouter une nouvelle courbe avec des points de contrôle par défaut
    std::vector<glm::vec3> defaultControlPoints(nbPointParCurve, glm::vec3(0.0f)); // Par exemple, des points de contrôle nuls
    controlP.push_back(defaultControlPoints);
    nbCurve++;
    surfaceChanged = true; // Indiquer que la surface de Bézier doit être recalculée
}

void delCurve() {
    // Ajouter une nouvelle courbe avec des points de contrôle par défaut
    if(nbCurve > 2){
        std::vector<glm::vec3> defaultControlPoints(nbPointParCurve, glm::vec3(0.0f)); // Par exemple, des points de contrôle nuls
        controlP.pop_back();
        nbCurve--;
        surfaceChanged = true; // Indiquer que la surface de Bézier doit être recalculée
    } 
}

void addPoint(){
    for(unsigned int i = 0; i < controlP.size(); i++){
        controlP[i].push_back(glm::vec3(0.0f));
    }
    nbPointParCurve++;
    surfaceChanged = true;
}

void delPoint(){
    if(nbPointParCurve > 2){
        for(unsigned int i = 0; i < controlP.size(); i++){
            controlP[i].pop_back();
        }
        nbPointParCurve--;
    }
    surfaceChanged = true;
}


void modeSurface(){
    ImGui::Text("Combien de point de controle");
    ImGui::NewLine();
    int previous = nbPointToDra;
    int nbMax = (nbCurve < nbPointParCurve) ? nbPointParCurve : nbCurve;
    ImGui::SliderInt("NBPoint", &nbPointToDra, nbMax, 100);
    if (previous != nbPointToDra){
        surfaceChanged = true;
    }
    ImGui::NewLine();
    ImGui::ColorEdit3("Couleur", (float*) &color);
    if (ImGui::CollapsingHeader("Options d'affichage")) {
        ImGui::Checkbox("Afficher les normales", &afficherNormale);
        ImGui::Checkbox("Afficher le polygone de contrôle", &afficherPolygone);
        ImGui::Checkbox("Afficher l'espace paramétrique", &afficherParam);
    }
    std::vector<std::vector<glm::vec3>> previousControlPoints = controlP;
    for (int i = 0; i < nbCurve; i++){
        char labelC[32];
        snprintf(labelC, sizeof(labelC), "Courbe %d", i + 1);
        if (ImGui::CollapsingHeader(labelC)) {
            for (int j = 0; j < nbPointParCurve; j++){
                char label[32];
                snprintf(label, sizeof(label), "Point %d", j + 1);
                ImGui::InputFloat3(label, glm::value_ptr(controlP[i][j]));
                if (previousControlPoints[i][j] != controlP[i][j]){
                    surfaceChanged = true;
                    previousControlPoints[i][j] = controlP[i][j];
                }
            }
        }
    }
    ImGui::NewLine();
    if (ImGui::Button("Ajouter une courbe")) 
        addCurve(); 
    ImGui::NewLine();
    if (ImGui::Button("Supprimer derniere courbe"))
        delCurve();
    ImGui::NewLine();
    if (ImGui::Button("Ajouter un point par courbe"))
        addPoint(); 
    ImGui::NewLine();
    if (ImGui::Button("Supprimer un point par courbe"))
        delPoint();

}


void menu(){
    ImGui::Begin("Menu");
    ImGui::Text("Pour désactiver le suivi de la souris\n appuyez sur O et pour le réactiver\n appuyez sur C");
    if(ImGui::Button("Tirer un rayon")){
        rayon = true;
    }
    choixMode();
    if (typeAff == 0){
        modeCourbe();
    }
    if (typeAff == 1){
        modeSurface();
    }
    if (typeAff == 2){
        modeSphere();
    }
    ImGui::End();
}