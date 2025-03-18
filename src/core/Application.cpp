#include "Application.h"
void drawAxes(Shader &shader, glm::mat4 view, glm::mat4 projection);

Application::Application(int width, int height, const char* title)
    : camera(glm::vec3(3.0f, 5.0f, -5.0f)), lightInitialPos(1.0f, 1.0f, 0.0f){

    // Camera direction

    camera.Front = {-1.0f, -1.0f, 1.0f};


    this->lastX = width / 2.0f;
    this->lastY = height / 2.0f;
    this->firstMouse = true;
    this->mousec = false;
    // timing
    this->deltaTime = 0.0f;
    this->lastFrame = 0.0f;
    this->espaceParam = false;
    this->p = true;
    // lighting
    this->lightPos = lightInitialPos;
    this->lightSpeed = 1.0f;

    this->mouseX = 0.0, mouseY = 0.0;
    this->nbRayon = 0;

    this->width = width;
    this->height = height;
    
    if (!glfwInit()) {
        std::cerr << "Erreur lors de l'initialisation de GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(this->width, this->height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Erreur lors de la création de la fenêtre" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erreur lors du chargement de GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwInit();
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    glEnable(GL_DEPTH_TEST); // Active le test de profondeur

}


void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void Application::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        mousec = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mousec = false;
    }
}

void Application::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {

    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app) return; // Si app est nullptr, on quitte la fonction

    if (app->mousec) { // Accéder aux membres de l'instance `Application`
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (app->firstMouse) {
            app->lastX = xpos;
            app->lastY = ypos;
            app->firstMouse = false;
        }

        float xoffset = xpos - app->lastX;
        float yoffset = app->lastY - ypos; // inversé car les coordonnées Y vont de bas en haut

        app->lastX = xpos;
        app->lastY = ypos;
        app->camera.ProcessMouseMovement(xoffset, yoffset); // Accès à `camera`
    }
}

void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app) return;

    app->camera.ProcessMouseScroll(static_cast<float>(yoffset));
}



void Application::render_shadow_map(){
    Menu menu = Menu(this->window);
    std::cout << "Launching application\n" << std::endl;
    TextureManager texture_manager;
    GLuint texture_brick = texture_manager.load_texture("brick.jpg");
    GLuint texture_earth = texture_manager.load_texture("earth.jpg");

    
    glm::mat4 model = glm::mat4(1.0f);
    float near_plane = 0.1f, far_plane = 5.0f;
    
    ShadowMap shadow_map = ShadowMap("/home/pierre/Projects/InformatiqueGraphique/shaders/test_depth.vs",
         "/home/pierre/Projects/InformatiqueGraphique/shaders/test_depth.fs", this->width, this->height, 
         near_plane, far_plane, model, menu.cutoff, menu.light_pos, menu.light_look_at);
    
    
    
    
    std::vector<std::unique_ptr<Object>> objects;
    
    objects.push_back(std::make_unique<Parallelepiped>(0, glm::vec3(0.0f, -1.45f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 10.0f, 0.3f, 10.0f));
    objects.push_back(std::make_unique<Parallelepiped>(0, glm::vec3(-1.45f, 0.3f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.3f, 0.3f, 0.3f));
    objects.push_back(std::make_unique<Sphere>(0, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.3f, 32, 32));
    objects.push_back(std::make_unique<Cylinder>(0, glm::vec3(-1.0f, 0.0f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.3f, 1.0f, 64, 512));

    
    
    
    Shader main_shader("/home/pierre/Projects/InformatiqueGraphique/shaders/main.vs", "/home/pierre/Projects/InformatiqueGraphique/shaders/main.fs");
    
    Shader depth_shader("/home/pierre/Projects/InformatiqueGraphique/shaders/test_depth.vs", "/home/pierre/Projects/InformatiqueGraphique/shaders/test_depth.fs");
    Renderer rend(main_shader);


    glEnable(GL_CULL_FACE);
    

    
    
    
    while (!glfwWindowShouldClose(window)) {      
        
        float currentFrame = static_cast<float>(glfwGetTime());
        
        this->deltaTime = currentFrame - this->lastFrame;
        this->lastFrame = currentFrame;
        processInput();
        
        glCullFace(GL_FRONT);
        shadow_map.update(menu.cutoff, menu.light_pos, menu.light_look_at);
        shadow_map.set_shader();
        rend.draw(objects);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // 2. then render scene as normal with shadow mapping (using depth map)
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        
        
        
        
        main_shader.use();
        glCullFace(GL_BACK); 
        main_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        main_shader.setVec3("lightPos", menu.light_pos);
        main_shader.setVec3("spot_direction", menu.light_look_at);
        main_shader.setFloat("cutoff",  glm::cos(glm::radians(menu.cutoff)));
        main_shader.setFloat("ambientStrength", 0.0f);
        main_shader.setMat4("model", model);
        main_shader.setMat4("projection", projection);
        main_shader.setMat4("view", view);
        main_shader.setMat4("depthMvp", shadow_map.depthMvp);
        main_shader.setMat4("depthBiasMvp", shadow_map.depthBiaisMvp);
        main_shader.setVec3("camera_pos", camera.Position);
        main_shader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        glActiveTexture(GL_TEXTURE1); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, shadow_map.depthMap);
        main_shader.setInt("depthMap", 1);
        rend.draw(objects);
        
        menu.render();
        if (menu.camera_on_light){
            camera.Position = menu.light_pos;
            camera.Front = menu.light_look_at;
            camera.Zoom = menu.cutoff;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


void Application::render_animation(){
    Menu menu(this->window);


    std::vector<std::unique_ptr<Object>> objects;
    
    objects.push_back(std::make_unique<Cylinder>(0, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.1f, 1.0f, 64, 512));
    objects.push_back(std::make_unique<Sphere>(0, glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.3f, 64, 512));
    objects.push_back(std::make_unique<Sphere>(0, glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.3f, 64, 512));


    
    Shader shader = Shader("/home/pierre/Projects/InformatiqueGraphique/shaders/animation_shader.vs", "/home/pierre/Projects/InformatiqueGraphique/shaders/animation_shader.fs");
    Renderer rend(shader);

    glm::mat4 model = glm::mat4(1.0f);

    while (!glfwWindowShouldClose(window)) { 

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentFrame = static_cast<float>(glfwGetTime());
        
        this->deltaTime = currentFrame - this->lastFrame;
        this->lastFrame = currentFrame;
        processInput();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        drawAxes(shader, view, projection);

        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setBool("useTexture", false);
        shader.setVec3("lightPos", glm::vec3(1.0f, 1.0f, 0.0f));

        rend.draw(objects);

        menu.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


void drawAxes(Shader &shader, glm::mat4 view, glm::mat4 projection) {
    // Coordonnées des axes X, Y, Z
    std::vector<glm::vec3> axes = {
        glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), // X (Rouge)
        glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), // Y (Vert)
        glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)  // Z (Bleu)
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, axes.size() * sizeof(glm::vec3), axes.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);


    // Dessiner l'axe X (Rouge)
    shader.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));  // Rouge
    glDrawArrays(GL_LINES, 0, 2);

    // Dessiner l'axe Y (Vert)
    shader.setVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f));  // Vert
    glDrawArrays(GL_LINES, 2, 2);

    // Dessiner l'axe Z (Bleu)
    shader.setVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));  // Bleu
    glDrawArrays(GL_LINES, 4, 2);

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}