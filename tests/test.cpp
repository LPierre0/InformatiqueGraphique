#include "../src/core/Application.h"



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

    std::vector<std::unique_ptr<Object>> objects;
    
    objects.push_back(std::make_unique<Parallelepiped>(GL_NONE, glm::vec3(0.0f, -1.45f, 0.0f), 10.0f, 0.3f, 10.0f));
    objects.push_back(std::make_unique<Parallelepiped>(GL_NONE, glm::vec3(-1.45f, 0.3f, 0.0f), 0.3f, 0.3f, 0.3f));
    objects.push_back(std::make_unique<Sphere>(GL_NONE, glm::vec3(0.0f, 0.0f, 0.0f), 0.3f, 32, 32));
    
    
    Renderer rend;
    
    Shader main_shader("/home/pierre/Projects/InformatiqueGraphique/shaders/main.vs", "/home/pierre/Projects/InformatiqueGraphique/shaders/main.fs");
    
    Shader depth_shader("/home/pierre/Projects/InformatiqueGraphique/shaders/test_depth.vs", "/home/pierre/Projects/InformatiqueGraphique/shaders/test_depth.fs");
    
    Shader ligth_shader("/home/pierre/Projects/InformatiqueGraphique/shaders/light.vs", "/home/pierre/Projects/InformatiqueGraphique/shaders/light.fs");
    

    glfwTerminate();
    return 0;
}