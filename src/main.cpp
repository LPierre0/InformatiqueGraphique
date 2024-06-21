#include "include.h"
#include "sphere.h"
#include "bezier.h"
#include "utils.h"
#include "menu.h"
#include "ray.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void updateLightPosition(float deltaTime);
void updateFinal(std::vector<int> &IndicesSurface, std::vector<std::vector<glm::vec3>> &surfaceBezier, std::vector<std::vector<glm::vec3>> &normales, std::vector<std::vector<glm::vec3>> &normalesInv, std::vector<std::vector<glm::vec3>> pointToSegment, std::vector<glm::vec3> finalData);
void drawLightCube(Shader lightCubeShader, glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, unsigned int lightCubeVAO);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
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

    Shader lightingShader("/home/pierre/Documents/Fac/Master1/S2/Projet-basecode/src/2.1.basic_lighting.vs", "/home/pierre/Documents/Fac/Master1/S2/Projet-basecode/src/2.1.basic_lighting.fs");
    Shader lightCubeShader("/home/pierre/Documents/Fac/Master1/S2/Projet-basecode/src/2.1.light_cube.vs", "/home/pierre/Documents/Fac/Master1/S2/Projet-basecode/src/2.1.light_cube.fs");

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    std::vector<glm::vec3> point;
    std::vector<glm::vec3> bezierCurve = getBezierCurve(controlPointsBezier, nbPointToDrawBezier, false, point);
    std::vector<glm::vec3> bezierCurveUni = getBezierCurveLenghtSeg(controlPointsBezier, distanceBezier);
    unsigned int curveVAO, curveVBO;
    glGenVertexArrays(1, &curveVAO);
    glGenBuffers(1, &curveVBO);

    glBindVertexArray(curveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, curveVBO);
    glBufferData(GL_ARRAY_BUFFER, bezierCurve.size()  * sizeof(glm::vec3), bezierCurve.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    unsigned int curveUniVAO, curveUniVBO;
    glGenVertexArrays(1, &curveUniVAO);
    glGenBuffers(1, &curveUniVBO);
    glBindVertexArray(curveUniVAO);
    glBindBuffer(GL_ARRAY_BUFFER, curveUniVBO);
    glBufferData(GL_ARRAY_BUFFER, bezierCurveUni.size()  * sizeof(glm::vec3), bezierCurveUni.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    unsigned int curveControlVAO, curveControlVBO;
    glGenVertexArrays(1, &curveControlVAO);
    glGenBuffers(1, &curveControlVBO);
    glBindVertexArray(curveControlVAO);
    glBindBuffer(GL_ARRAY_BUFFER, curveControlVBO);
    glBufferData(GL_ARRAY_BUFFER, controlPointsBezier.size()  * sizeof(glm::vec3), controlPointsBezier.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
    Bezier surface;

    surface.setControlPoint(controlP, nbCurve, nbPointParCurve, 10);
    surface.calculateBezierSurface();
    surface.calculateNormales();
    surface.calculateNormalesInv();
    surface.calculateSegment();
    surface.calculateFinalData();

    // Passage en 1D car pas affichable en 2D
    std::vector<glm::vec3> control = convertVec1D(surface.controlPoints);

    glGenVertexArrays(1, &surface.controlVAO);
    glGenBuffers(1, &surface.controlVBO);
    glGenBuffers(1, &surface.controlEBO);
    glBindVertexArray(surface.controlVAO);
    glBindBuffer(GL_ARRAY_BUFFER, surface.controlVBO);
    glBufferData(GL_ARRAY_BUFFER, control.size()  * sizeof(glm::vec3), control.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surface.controlEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, surface.IndicesControl.size() * sizeof(int), surface.IndicesControl.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
    
    glGenVertexArrays(1, &surface.surfVAO);
    glGenBuffers(1, &surface.surfVBO);
    glGenBuffers(1, &surface.surfEBO);
    glBindVertexArray(surface.surfVAO);
    glBindBuffer(GL_ARRAY_BUFFER, surface.surfVBO);
    glBufferData(GL_ARRAY_BUFFER, surface.finalData.size() * sizeof(glm::vec3), surface.finalData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surface.surfEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, surface.IndicesSurface.size() * sizeof(int), surface.IndicesSurface.data(), GL_STATIC_DRAW);
    // 0 = Position, 1 = Normale, 2 = NormaleInv
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(2 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &surface.segVAO);
    glGenBuffers(1, &surface.segVBO);
    glGenBuffers(1, &surface.segEBO);
    glBindVertexArray(surface.segVAO);
    glBindBuffer(GL_ARRAY_BUFFER, surface.segVBO);
    glBufferData(GL_ARRAY_BUFFER, surface.segments.size() * sizeof(glm::vec3), surface.segments.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surface.segEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, surface.IndicesSegments.size() * sizeof(int), surface.IndicesSegments.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);






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
    Sphere sphere1(cen, radiusMenu, sectorCountMenu, stackCountMenu);
    sphere1.calculatePointsAndNormals();
    sphere1.calculateIndices();
    sphere1.finalData();

    Ray rayon1; 
    float t = 0;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        updateLightPosition(deltaTime);
        // input
        // -----
        processInput(window);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_Always);
        menu();
        ImGui::Render();
        // Render ImGui draw data
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


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
        lightingShader.setMat4("model", model);




        lightingShader.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
        glfwGetCursorPos(window, &mouseX, &mouseY);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && rayon) {
            rayon1.update(SCR_WIDTH, SCR_HEIGHT, projection, view, camera.Position, mouseX, mouseY);
            rayon = false;
            nbRayon++;
        }

        lightingShader.setVec3("objectColor", col);
        if (typeAff == 0){
            if (updatedBezier){
                bezierCurve.clear();
                bezierCurve = getBezierCurve(controlPointsBezier, nbPointToDrawBezier, false, point);
                bezierCurveUni.clear();
                bezierCurveUni = getBezierCurveLenghtSeg(controlPointsBezier, distanceBezier);
                glBindVertexArray(curveVAO);
                glBindBuffer(GL_ARRAY_BUFFER, curveVBO);
                glBufferData(GL_ARRAY_BUFFER, bezierCurve.size()  * sizeof(glm::vec3), bezierCurve.data(), GL_STATIC_DRAW);
                glBindVertexArray(curveControlVAO);
                glBindBuffer(GL_ARRAY_BUFFER, curveControlVBO);
                glBufferData(GL_ARRAY_BUFFER, controlPointsBezier.size()  * sizeof(glm::vec3), controlPointsBezier.data(), GL_STATIC_DRAW);
                glBindVertexArray(curveUniVAO);
                glBindBuffer(GL_ARRAY_BUFFER, curveUniVBO);
                glBufferData(GL_ARRAY_BUFFER, bezierCurveUni.size()  * sizeof(glm::vec3), bezierCurveUni.data(), GL_STATIC_DRAW);
                updatedBezier = false;
            }
            if (typeCourbe == 0){
                glBindVertexArray(curveVAO);
                glDrawArrays(GL_LINE_STRIP, 0, bezierCurve.size());
                glBindVertexArray(curveControlVAO);
                glDrawArrays(GL_LINE_STRIP, 0, controlPointsBezier.size());
            }else{
                
                glBindVertexArray(curveUniVAO);
                glDrawArrays(GL_LINE_STRIP, 0, bezierCurveUni.size());
                glBindVertexArray(curveControlVAO);
                glDrawArrays(GL_LINE_STRIP, 0, controlPointsBezier.size());
            }
        }

        if (typeAff == 1){
            lightingShader.setFloat("ambientStrength", 0.1f);
            surface.draw(control);
    
            if (nbRayon > 0){
                if(intersectSurface(rayon1, surface, t)){
                    surface.t = t;
                    lightingShader.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
                    surface.drawIntersection(rayon1);
                    lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
                }
            }

            drawLightCube(lightCubeShader, model, view, projection, lightCubeVAO); 

        }

        if (typeAff == 2){
            sphere1.draw();
            if (sphere1.intersect(rayon1)){
                lightingShader.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
                sphere1.drawIntersection(rayon1);
                lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            }
            drawLightCube(lightCubeShader, model, view, projection, lightCubeVAO);     
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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