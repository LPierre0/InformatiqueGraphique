#ifndef MENU_H
#define MENU_H

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "imgui/imgui.h"
#include "vector"
#include <glm/glm.hpp>


class Menu
{
    public : 
        Menu(GLFWwindow * window);
        glm::vec3 light_pos;
        glm::vec3 light_look_at;
        glm::vec3 light_up;
        float cutoff;
        bool camera_on_light;

        void render();
        

    
};


#endif