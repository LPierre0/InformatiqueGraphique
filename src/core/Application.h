#ifndef APPLICATION_H
#define APPLICATION_H

#include "../include.h"
#include "Object.h"
#include "Renderer.h"
#include "../objects/Parallelepiped.h"
#include "../manager/TextureManager.h"



class Application {
private:
    GLFWwindow* window;

    int width;
    int height;

    Camera camera;
    // Camera direction

    float lastX;
    float lastY;
    bool firstMouse;
    bool mousec;
    // timing
    float deltaTime;
    float lastFrame;
    bool espaceParam;
    bool p = true;
    // lighting
    glm::vec3 lightInitialPos;
    glm::vec3 lightPos;
    float lightSpeed;

    double mouseX, mouseY;
    int nbRayon;



public:
    Application(int width, int height, const char* title);

    void run();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    
    // ~Application();
    void processInput();
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    // void processInput(GLFWwindow *window);

};

#endif
