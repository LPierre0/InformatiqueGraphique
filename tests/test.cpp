#include "../src/include.h"
#include "../src/objects/Quad.h"
#include "../src/manager/TextureManager.h"



// Fonction pour initialiser OpenGL
bool initOpenGL(GLFWwindow*& window, int width, int height) {
    if (!glfwInit()) {
        std::cerr << "Erreur lors de l'initialisation de GLFW" << std::endl;
        return false;
    }

    window = glfwCreateWindow(width, height, "Triangle OpenGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erreur lors de la création de la fenêtre" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erreur lors du chargement de GLAD" << std::endl;
        return false;
    }
    return true;
}


int main() {
    // Initialisation de GLFW et OpenGL
    GLFWwindow* window;
    if (!initOpenGL(window, 800, 600)) {
        return -1;
    }


    int width = 800;
    int height = 600;


    TextureManager texture_manager;
    // Charger la texture
    GLuint texture = texture_manager.load_texture("earth.jpg");

    // Créer un quad
    Shader shader("/home/pierre/Projects/InformatiqueGraphique/shaders/quad.vs", "/home/pierre/Projects/InformatiqueGraphique/shaders/quad.fs");
    Quad quad(texture, shader);
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);  // Projection orthographique
    shader.use();
    shader.setMat4("projection", projection);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        quad.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}