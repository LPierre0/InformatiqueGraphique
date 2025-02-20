#include "Application.h"


Application::Application(int width, int height, const char* title)
    : camera(glm::vec3(0.0f, 0.0f, -10.0f)), lightInitialPos(0.0f, 0.0f, 0.0f){

    // Camera direction

    camera.ProcessMouseMovement(width * 2 - 250, 0);


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



void Application::run(){


    std::cout << "Launching application\n" << std::endl;


    TextureManager texture_manager;

    unsigned int texture_or = texture_manager.load_texture("or.jpg");
    if (texture_or == -1){
        return;
    }

    unsigned int texture_brick = texture_manager.load_texture("brick.jpg");
    if (texture_brick == -1){
        return;
    }



    Parallelepiped para(texture_or, glm::vec3({0.0f, 0.0f, 0.0f}), 0.1f, 1.0f, 1.0f);
    Parallelepiped para_2(texture_brick, glm::vec3({0.45f, -0.45f, 0.0f}), 1.0f, 0.1f, 1.0f);
    Parallelepiped para_3(texture_or, glm::vec3({1.0f, 0.0f, 0.0f}), 0.1f, 1.0f, 1.0f);
    Parallelepiped para_4(texture_or, glm::vec3({0.45f, 0.0f, +0.45f}), 1.0f, 1.0f, 0.1f);


    Renderer rend;
    
    Shader main_shader("/home/pierre/Projects/InformatiqueGraphique/shaders/main.vs", "/home/pierre/Projects/InformatiqueGraphique/shaders/main.fs");


    while (!glfwWindowShouldClose(window)) {        
        float currentFrame = static_cast<float>(glfwGetTime());

        this->deltaTime = currentFrame - this->lastFrame;
        this->lastFrame = currentFrame;
        processInput();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        main_shader.use();
        main_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        main_shader.setVec3("lightPos", lightPos);
        main_shader.setFloat("ambientStrength", 1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        main_shader.setMat4("projection", projection);
        main_shader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(2.0f));  // Augmenter la taille de l'objet

        main_shader.setMat4("model", model);

        main_shader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));

        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rend.draw(para);
        rend.draw(para_2);
        rend.draw(para_3);
        rend.draw(para_4);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
}


