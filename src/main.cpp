#include <glad/glad.h>

#include "GameClass/MainGameClass.h"
#include "Renderer/RenderEngine.h"
#include "Managers/ResourceManager.h"
#include "Control/MouseControl.h"
#include "Control/KeyboardControl.h"
#include "Audio/SoundDevice.h"
#include "Audio/SoundEffectsLibrary.h"
#include "Managers/ConfigManager.h"
#include "Renderer/PrintText.h"
#include "Managers/SoundManager.h"
#include "UI/Translater.h"
#include "GameEngine/PhysicsAndLogic/DebugRender.h"

 //(RUS) Для отрисовки с видеокарт NVidia и AMD, а не встроеных
//(ENG) For rendering from NVidia and AMD video cards, not built-in ones
extern "C" {
    _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
} 

GLFWwindow* PWindow = nullptr;

void GLFWWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    CONFIG_MANAGER::setWindowSize(width, height);
    RENDER_ENGINE::setViewport(width, height); 
    MAIN_GAME_CLASS::setProjectionMat(glm::vec2(width, height));
}
void GLFWMonitorCallBack(GLFWmonitor* monitor, int action) {
    if (action == GLFW_DISCONNECTED && CONFIG_MANAGER::getFullScreen()) {
        GLFWmonitor* new_monitor = RENDER_ENGINE::getMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(new_monitor);
        CONFIG_MANAGER::setWindowSize(mode->width, mode->height);
        glfwSetWindowMonitor(PWindow, new_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
}

int main(int argc, char** argv){
    if (!glfwInit()) { 
        std::cerr << "(!) glfwInit failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    RESOURCE_MANAGER::setExecutablePath(argv[0]);
     
    CONFIG_MANAGER::loadConfig();

    glfwWindowHint(GLFW_SAMPLES, CONFIG_MANAGER::getSamples());

    glm::ivec2 window = CONFIG_MANAGER::getWindowSize();
    //реализовать создание окна с размером монитора при полноэкранном режиме, и его передачу в мышь
    PWindow = glfwCreateWindow(window.x, window.y, "Game", RENDER_ENGINE::getMonitor(), NULL);

    if (!PWindow){
        glfwTerminate();
        std::cerr << "(!) PWindow failed" << std::endl;
        return -1;
    }

    glfwSetWindowSizeCallback(PWindow, GLFWWindowSizeCallback);
    glfwSetMonitorCallback(GLFWMonitorCallBack);

    glfwSetKeyCallback(PWindow, KEYBOARD::setKey);
    glfwSetCharCallback(PWindow, KEYBOARD::addCharInBuffer);

    glfwSetMouseButtonCallback(PWindow, MOUSE::setButton);
    glfwSetScrollCallback(PWindow, MOUSE::setScroll);

    glfwMakeContextCurrent(PWindow);

    glfwSwapInterval(CONFIG_MANAGER::getVSync());
	
	if(!gladLoadGL()){
        glfwTerminate();
		std::cerr<<"(!) gladLoadGL failed"<<std::endl;
		return -1;
	}

    if (CONFIG_MANAGER::getFullScreen()) {
        int screenWidth, screenHeight;
        glfwGetFramebufferSize(PWindow, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);
    }
    else {
        int windowWidth, windowHeight;
        glfwGetWindowSize(PWindow, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);
    }

    std::cout << "Renderer: " << RENDER_ENGINE::getRender() << std::endl;
    std::cout << "OpenGL version: " << RENDER_ENGINE::getVersion() << std::endl;
    glEnable(GL_PROGRAM_POINT_SIZE);
    MOUSE::setWindow(PWindow);
    RENDER_ENGINE::setWindow(PWindow);

    RENDER_ENGINE::enableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    RENDER_ENGINE::setClearColor(0, 0, 0, 255);
    RENDER_ENGINE::setDetphTest(true);
       
    SOUND_DEVICE::init();
    SOUND_LIBRARY::init();

    DebugRender::init();

    MAIN_GAME_CLASS::init(window);

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(PWindow)) { 
        double currTime = glfwGetTime();
        double dura = (currTime - lastTime)*1000;
        lastTime = currTime;

        MAIN_GAME_CLASS::update(dura);

        RENDER_ENGINE::clear();

        MAIN_GAME_CLASS::render();

        glfwSwapBuffers(PWindow);

        glfwPollEvents();
        MAIN_GAME_CLASS::events();
    }
    RESOURCE_MANAGER::unloadAllRes();
     
    PRINT_TEXT::terminate();
    SOUND_MANAGER::terminate();
    MAIN_GAME_CLASS::terminate();

    glfwDestroyWindow(PWindow);

    glfwTerminate();

    return 0;
}