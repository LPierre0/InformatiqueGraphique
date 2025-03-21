#ifndef APPLICATION_H
#define APPLICATION_H

#include "../include.h"
#include "Object.h"
#include "Renderer.h"
#include "ShadowMapping.h"
#include "../utils/utils.h"
#include "../objects/Parallelepiped.h"
#include "../animation/Animation.h"
#include "../objects/Sphere.h"
#include "../objects/Cylinder.h"
#include "../manager/TextureManager.h"
#include "../objects/Quad.h"


#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "imgui/imgui.h"

#include "menu.h"


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

    void render_shadow_map();

    void render_animation();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    
    // ~Application();
    void processInput();
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    // void processInput(GLFWwindow *window);

};

#endif
