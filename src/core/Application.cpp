#include "Application.h"

glm::vec3 updateLightPosition(float deltaTime, glm::vec3 lightInitialPos);

Application::Application(int width, int height, const char* title)
    : camera(glm::vec3(3.0f, 5.0f, -5.0f)), lightInitialPos(-1.0f, 2.0f, -0.5f){

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



void Application::run(){


    std::cout << "Launching application\n" << std::endl;


    TextureManager texture_manager;

    GLuint texture_or = texture_manager.load_texture("brick.jpg");
    if (texture_or == -1){
        return;
    }

    GLuint texture_brick = texture_manager.load_texture("earth.jpg");
    if (texture_brick == -1){
        return;
    }
    
    std::vector<std::unique_ptr<Object>> light_cubes;
    light_cubes.push_back(std::make_unique<Parallelepiped>(GL_NONE, lightInitialPos, 0.3f, 0.3f, 0.3f));

    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);  

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
    SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  


    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    

    std::vector<std::unique_ptr<Object>> objects;

    objects.push_back(std::make_unique<Parallelepiped>(texture_or, glm::vec3(0.0f, -1.45f, 0.0f), 20.0f, 0.3f, 20.0f));
    objects.push_back(std::make_unique<Parallelepiped>(texture_or, glm::vec3(-1.45f, 0.0f, 0.0f), 0.3f, 3.0f, 1.0f));
    objects.push_back(std::make_unique<Sphere>(texture_or, glm::vec3(0.0f, 0.0f, 0.0f), 0.3f, 256, 256));


    
    Renderer rend;
    
    Shader main_shader("/home/pierre/Projects/InformatiqueGraphique/shaders/main.vs", "/home/pierre/Projects/InformatiqueGraphique/shaders/main.fs");

    Shader depth_shader("/home/pierre/Projects/InformatiqueGraphique/shaders/test_depth.vs", "/home/pierre/Projects/InformatiqueGraphique/shaders/test_depth.fs");
        
    Shader ligth_shader("/home/pierre/Projects/InformatiqueGraphique/shaders/light.vs", "/home/pierre/Projects/InformatiqueGraphique/shaders/light.fs");
    Quad my_quad(depthMap, main_shader);




    
    while (!glfwWindowShouldClose(window)) {        
        float currentFrame = static_cast<float>(glfwGetTime());
        
        this->deltaTime = currentFrame - this->lastFrame;
        this->lastFrame = currentFrame;
        processInput();
        

        glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
            );
        glm::mat4 model = glm::mat4(1.0f);
        float near_plane = 1.0f, far_plane = 7.5f;
        glm::mat4 light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        
        
        glm::mat4 light_view = glm::lookAt(lightPos, 
                                        glm::vec3( 0.0f, 0.0f,  0.0f), 
                                        glm::vec3( 0.0f, 1.0f,  0.0f));  
    
        
        glm::mat4 depthMvp = light_projection * light_view * model; 
        glm::mat4 depthBiaisMvp = biasMatrix * depthMvp;
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        depth_shader.use();
        depth_shader.setMat4("depthMvp", depthMvp);
        rend.draw(objects);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // 2. then render scene as normal with shadow mapping (using depth map)
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        ligth_shader.use();
        ligth_shader.setMat4("projection", projection);
        ligth_shader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        
        ligth_shader.setMat4("model", model);
        rend.draw(light_cubes);



        main_shader.use();
        main_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        main_shader.setVec3("lightPos", lightPos);
        main_shader.setFloat("ambientStrength", 1.0f);
        model = glm::mat4(1.0f);
        main_shader.setMat4("model", model);
        main_shader.setMat4("projection", projection);
        main_shader.setMat4("view", view);
        main_shader.setMat4("depthMvp", depthBiaisMvp);
        main_shader.setMat4("depthBiasMvp", depthBiaisMvp);
        main_shader.setVec3("camera_pos", camera.Position);
        main_shader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        glActiveTexture(GL_TEXTURE1); // Utilisation de l'unité 1
        glBindTexture(GL_TEXTURE_2D, depthMap);
        main_shader.setInt("depthMap", 1);

        for (auto& object : objects){
            object->set_texture(texture_brick);
        }
        rend.draw(objects);



        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
}




glm::vec3 updateLightPosition(float deltaTime, glm::vec3 lightInitialPos) {
    // Rayon du cercle dans le plan XY
    float radiusXY = 4.0f;

    // Rayon du cercle dans le plan XZ
    float radiusXZ = 4.0f;

    // Vitesse de rotation
    float angularSpeed = 0.5f;

    // Temps écoulé
    float time = glfwGetTime();

    // Calcul des nouvelles positions en fonction du temps
    float newX = lightInitialPos.x + radiusXY * cos(angularSpeed * time);
    float newY = lightInitialPos.y + radiusXY * sin(angularSpeed * time);
    float newZ = lightInitialPos.z + radiusXZ * cos(angularSpeed * time);

    // Mise à jour de la position de la lumière
    glm::vec3 lightPos = glm::vec3(newX, newY, newZ);
    return lightPos;
}