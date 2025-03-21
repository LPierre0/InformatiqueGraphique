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

    this->light_pos = glm::vec3(0.5f, 2.0f, 0.0f);
    this->light_look_at = glm::vec3(0.0f, -1.0f, 0.0f);
    this->inner_cutoff = 45.0f;
    this->outer_cutoff = 55.0f;
    this->camera_on_light = false;
    this->show_bone = false;
    this->set_initial_pos = true;
    this->use_gpu = false;
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

    float v_look_at[3]=  {light_look_at[0], light_look_at[1], light_look_at[2]};
    ImGui::SliderFloat3("LIGHT LOOK AT", v_look_at, -5.0f, 5.0f);
    light_look_at[0] = v_look_at[0];
    light_look_at[1] = v_look_at[1];
    light_look_at[2] = v_look_at[2];


    ImGui::SliderFloat("inner cutoff", &this->inner_cutoff, 0.0f, 90.0f);

    ImGui::SliderFloat("outer cutoff", &this->outer_cutoff, 0.0f, 90.0f);



    this->camera_on_light = ImGui::Button("Put camera on light", {100, 100});

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void Menu::render_animation(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Demo window");
    

    ImGui::Checkbox("Voir les os", &show_bone);
    ImGui::Checkbox("Pose initiale", &set_initial_pos);
    ImGui::Checkbox("Use gpu", &use_gpu);



    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}