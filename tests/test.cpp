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

    return 0;
}