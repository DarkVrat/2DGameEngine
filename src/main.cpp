#include <glad/glad.h>

#include "GameClass/MainGameClass.h"
#include "Renderer/RenderEngine.h"
#include "Resources/ResourceManager.h"
#include "Control/MouseControl.h"
#include "Control/KeyboardControl.h"
#include "Audio/SoundDevice.h"
#include "Audio/SoundEffectsLibrary.h"

//Проверить позже на ноутбуке
//extern "C" {
//    _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
//    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
//}

GLFWwindow* PWindow = nullptr;

void GLFWWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    RENDER_ENGINE->setWindowSize(glm::vec2(width, height));
    RENDER_ENGINE->setViewport(width, height);
    Control::MouseControl::Get()->SetHeight(height);
    MAIN_GAME_CLASS->SetProjectionMat(glm::vec2(width, height));
}
void GLFWMonitorCallBack(GLFWmonitor* monitor, int action) {
    if (action == GLFW_DISCONNECTED && RENDER_ENGINE->getFullScreen()) {
        GLFWmonitor* new_monitor = RENDER_ENGINE->getMonitor();
        const  GLFWvidmode* mode = glfwGetVideoMode(new_monitor);
        RENDER_ENGINE->setWindowSize(glm::vec2(mode->width, mode->height));
        glfwSetWindowMonitor(PWindow, new_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
}

void GLFWKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    KEYBOARD->SetKey(key, action);
}
void GLFWCharCallback(GLFWwindow* окно, unsigned int codepoint) {
    KEYBOARD->addCharInBuffer(codepoint);
}

void GLFWMouseCallback(GLFWwindow* pWindow, int button, int action, int mods) {
    MOUSE->SetKey(button, action); 
}
void GLFWMouseScrollCallback(GLFWwindow* окно, double x, double y){
    MOUSE->SetScroll(glm::vec2(x, y));
}

int main(int argc, char** argv){
    if (!glfwInit()) { 
        std::cerr << "(!) glfwInit failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    RESOURCE_MANAGER->setExecutablePath(argv[0]);

    RENDER_ENGINE->loadConfig();

    glm::vec2 window = RENDER_ENGINE->getWindowSize();
    PWindow = glfwCreateWindow(window.x, window.y, "Game", RENDER_ENGINE->getMonitor(), NULL);

    if (!PWindow){
        glfwTerminate();
        std::cerr << "(!) PWindow failed" << std::endl;
        return -1;
    }

    glfwSetWindowSizeCallback(PWindow, GLFWWindowSizeCallback);
    glfwSetMonitorCallback(GLFWMonitorCallBack);

    glfwSetKeyCallback(PWindow, GLFWKeyCallback);
    glfwSetCharCallback(PWindow, GLFWCharCallback);

    glfwSetMouseButtonCallback(PWindow, GLFWMouseCallback);
    glfwSetScrollCallback(PWindow, GLFWMouseScrollCallback);

    glfwMakeContextCurrent(PWindow);

    glfwSwapInterval(1);
	
	if(!gladLoadGL()){
        glfwTerminate();
		std::cerr<<"(!) gladLoadGL failed"<<std::endl;
		return -1;
	}

    std::cout << "Renderer: " << RENDER_ENGINE->getRender() << std::endl;
    std::cout << "OpenGL version: " << RENDER_ENGINE->getVersion() << std::endl;

    MOUSE->SetWindow(PWindow);
    MOUSE->SetHeight(window.y);

    RENDER_ENGINE->enableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    RENDER_ENGINE->setClearColor(0.2f, 0.2f, 0.2f, 1.f);
    RENDER_ENGINE->setDetphTest(true);
       
    MAIN_GAME_CLASS->init();
    MAIN_GAME_CLASS->SetProjectionMat(window);

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(PWindow)) {
        double currTime = glfwGetTime();
        double dura = (currTime - lastTime)*1000;
        lastTime = currTime;

        MAIN_GAME_CLASS->update(dura);

        RENDER_ENGINE->clear();

        MAIN_GAME_CLASS->render();

        glfwSwapBuffers(PWindow);

        glfwPollEvents();
        MAIN_GAME_CLASS->Events();
    }
    RESOURCE_MANAGER->unloadAllRes();

    RENDER_ENGINE->saveConfig();

    MainGameClass::Terminate();
    Renderer::PrintText::Terminate();
    Audio::SoundDevice::Terminate();
    Audio::SoundEffectsLibrary::Terminate();
    glfwTerminate();

    return 0;
}