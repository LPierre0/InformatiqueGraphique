#include "menu.h"


Menu::Menu(GLFWwindow * window){
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    this->light_pos = glm::vec3(1.0f, 1.0f, 0.0f);
    this->light_up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->light_look_at = glm::vec3(0.0f, 0.0f, 0.0f);
}



void Menu::render(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Demo window");
    float v[3] =  {light_pos[0], light_pos[1], light_pos[2]};
    ImGui::SliderFloat3("LIGHT POSITION", v, -5.0f, 5.0f);
    light_pos[0] = v[0];
    light_pos[1] = v[1];
    light_pos[2] = v[2];


    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}