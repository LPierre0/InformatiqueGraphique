#include "include.h"
#include "objects/sphere.h"
#include "objects/bezier.h"
#include "utils.h"
#include "menu.h"
#include "objects/ray.h"
#include "objects/parallepipede.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void updateLightPosition(float deltaTime);
void updateFinal(std::vector<int> &IndicesSurface, std::vector<std::vector<glm::vec3>> &surfaceBezier, std::vector<std::vector<glm::vec3>> &normales, std::vector<std::vector<glm::vec3>> &normalesInv, std::vector<std::vector<glm::vec3>> pointToSegment, std::vector<glm::vec3> finalData);
void drawLightCube(Shader lightCubeShader, glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, unsigned int lightCubeVAO);
PARALLEPIPEDE createCube(glm::vec3 center, std::vector<glm::vec3> points, GLuint texture);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 6.0f));
// Camera direction



float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool mousec = false;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool espaceParam = false;
bool p = true;
// lighting
glm::vec3 lightInitialPos(0.0f, 0.0f, 0.0f);
glm::vec3 lightPos = lightInitialPos;
float lightSpeed = 1.0f;

double mouseX = 0.0, mouseY = 0.0;
int nbRayon = 0;



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);



    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    Shader lightingShader("/home/pierre/Projects/InformatiqueGraphique/src/shaders/2.1.basic_lighting.vs", "/home/pierre/Projects/InformatiqueGraphique/src/shaders/2.1.basic_lighting.fs");
    Shader lightCubeShader("/home/pierre/Projects/InformatiqueGraphique/src/shaders/2.1.light_cube.vs", "/home/pierre/Projects/InformatiqueGraphique/src/shaders/2.1.light_cube.fs");

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);







    

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };


    unsigned int lightCubeVAO, lightCubeVBO;
    glGenBuffers(1, &lightCubeVBO);
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    
    // TEXTURES
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Définit les options de la texture actuellement liée
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // Éviter la répétition de la texture sur les bords
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  // Éviter la répétition sur l'axe T
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // Filtrage de texture pour les petites images
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // Filtrage pour les images agrandies

    // Charger et générer la texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("/home/pierre/Projects/InformatiqueGraphique/textures/2.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);  // Générer des mipmaps
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    std::vector<glm::vec3> points = {
        // 0
        glm::vec3(-0.5f, -0.5f, -0.5f),

        // 1
        glm::vec3(0.5f, -0.5f, -0.5f),

        // 2
        glm::vec3(-0.5f, -0.45f, -0.5f),

        // 3
        glm::vec3(0.5f, -0.45f, -0.5f),

        // 4
        glm::vec3(-0.5f, -0.5f, 0.5f),

        // 5
        glm::vec3(0.5f, -0.5f, 0.5f),

        // 6
        glm::vec3(-0.5f, -0.45f, 0.5f),

        // 7
        glm::vec3(0.5f, -0.45f, 0.5f),
    };

    std::vector<glm::vec3> points_2 = {
        // 0
        glm::vec3(-0.5f, -0.5f, -0.5f),

        // 1
        glm::vec3(-0.45f, -0.5f, -0.5f),

        // 2
        glm::vec3(-0.5f, 0.5f, -0.5f),

        // 3
        glm::vec3(-0.45f, 0.5f, -0.5f),

        // 4
        glm::vec3(-0.5f, -0.5f, 0.5f),

        // 5
        glm::vec3(-0.45f, -0.5f, 0.5f),

        // 6
        glm::vec3(-0.5f, 0.5f, 0.5f),

        // 7
        glm::vec3(-0.45f, 0.5f, 0.5f),
    };
    PARALLEPIPEDE cube = createCube(glm::vec3(0.0f, 0.0f, 0.0f), points, texture);
    PARALLEPIPEDE cube_2 = createCube(glm::vec3(0.0f, 0.0f, 0.0f), points_2, texture);

    Sphere sphere_scene(glm::vec3(0.0f, 0.0f, 0.0f), 0.2f, 36, 18);
    sphere_scene.calculatePointsAndNormals();
    sphere_scene.calculateIndices();
    sphere_scene.finalData();

    Ray rayon1; 
    float t = 0;


    // ------------------------------------------------------------------
    // Main loop
    // ------------------------------------------------------------------

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        updateLightPosition(deltaTime);
        // input
        // -----
        processInput(window);
        // Render ImGui draw data
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::vec3 col = {color.x, color.y, color.z};
        lightingShader.use();
        lightingShader.setVec3("objectColor", col);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setFloat("ambientStrength", 1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(2.0f));  // Augmenter la taille de l'objet

        lightingShader.setMat4("model", model);

        lightingShader.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
        glfwGetCursorPos(window, &mouseX, &mouseY);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && rayon) {
            rayon1.update(SCR_WIDTH, SCR_HEIGHT, projection, view, camera.Position, mouseX, mouseY);
            rayon = false;
            nbRayon++;
        }

        lightingShader.setVec3("objectColor", col);




        glActiveTexture(GL_TEXTURE0);  // Activer l'unité de texture 0
        glBindTexture(GL_TEXTURE_2D, texture); // Lier la texture à l'unité 0
        lightingShader.setInt("ourTexture", 0);
        cube.draw();
        cube_2.draw();
        sphere_scene.draw();
        drawLightCube(lightCubeShader, model, view, projection, lightCubeVAO);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (mousec){
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// Fonction pour mettre à jour la position de la lumière en fonction du temps
void updateLightPosition(float deltaTime) {
    // Rayon du cercle dans le plan XY
    float radiusXY = 2.0f;

    // Rayon du cercle dans le plan XZ
    float radiusXZ = 1.0f;

    // Vitesse de rotation
    float angularSpeed = 0.5f;

    // Temps écoulé
    float time = glfwGetTime();

    // Calcul des nouvelles positions en fonction du temps
    float newX = lightInitialPos.x + radiusXY * cos(angularSpeed * time);
    float newY = lightInitialPos.y + radiusXY * sin(angularSpeed * time);
    float newZ = lightInitialPos.z + radiusXZ * cos(angularSpeed * time);

    // Mise à jour de la position de la lumière
    lightPos = glm::vec3(newX, newY, newZ);
}



void drawLightCube(Shader lightCubeShader, glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, unsigned int lightCubeVAO){
    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
    lightCubeShader.setMat4("model", model);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


PARALLEPIPEDE createCube(glm::vec3 center, std::vector<glm::vec3> points, GLuint texture){
    printf("Creating cube\n");
    std::vector<std::vector<int>> faces;
    std::vector<int> face1 = {0, 1, 2, 1, 3, 2};
    std::vector<int> face2 = {4, 5, 6, 5, 7, 6};
    std::vector<int> face3 = {0, 1, 4, 1, 5, 4};
    std::vector<int> face4 = {2, 3, 6, 3, 7, 6};
    std::vector<int> face5 = {0, 2, 4, 2, 6, 4};
    std::vector<int> face6 = {1, 3, 5, 3, 7, 5};
    faces.push_back(face1);
    faces.push_back(face2);
    faces.push_back(face3);
    faces.push_back(face4);
    faces.push_back(face5);
    faces.push_back(face6);

    printf("Push faces\n");

    printf("Push points\n");

    PARALLEPIPEDE cube(points, faces, center, texture);
    printf("Cube created\n");
    cube.calculateIndices();
    cube.calculateNormals();
    cube.getFinalData();
    return cube;
}