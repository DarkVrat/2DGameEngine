#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <chrono>
#include <iostream>
#include "Resources/ResourceManager.h"
#include "GameClass/MainGameClass.h"
#include "Renderer/RenderEngine.h"
#include "Control/MouseControl.h"
#include "Control/KeyboardControl.h"
#include "Audio/SoundDevice.h"
#include "Audio/SoundEffectsLibrary.h"

//��������� ����� �� ��������
//extern "C" {
//    _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
//    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
//}


//������ ����
glm::ivec2 g_window;

GLFWwindow* PWindow = nullptr;

//��� ������� ����
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_window.x = width;
    g_window.y = height;
    RENDER_ENGINE->setViewport(g_window.x, g_window.y);
    Control::MouseControl::Get()->SetHeight(g_window.y);
    MAIN_GAME_CLASS->SetProjectionMat(g_window);
}

//�������� ������� ������
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    KEYBOARD->SetKey(key, action);
}

void glfwMouseCallback(GLFWwindow* pWindow, int button, int action, int mods) {
    MOUSE->SetKey(button, action); 
}

void glfwMouseScrollCallback(GLFWwindow* ����, double x, double y){
    MOUSE->SetScroll(glm::vec2(x, y));
}

void glfwMonitorCallBack(GLFWmonitor* monitor, int action){
    if (action==GLFW_DISCONNECTED && RENDER_ENGINE->getFullScreen()) {
        GLFWmonitor* new_monitor = RENDER_ENGINE->getMonitor();
        const  GLFWvidmode* mode = glfwGetVideoMode(new_monitor);
        RENDER_ENGINE->setWindowSize(glm::vec2(mode->width, mode->height));
        glfwSetWindowMonitor(PWindow, new_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
}

void glfwCharCallback(GLFWwindow* ����, unsigned int codepoint) {
    KEYBOARD->addCharInBuffer(codepoint);
}

int main(int argc, char** argv){

    if (!glfwInit()) { //������������� ���������� GLFW
        std::cerr << "(!) glfwInit failed" << std::endl;
        return -1;
    }

    RENDER_ENGINE->loadConfig(argv[0]);
    SOUND_DEVICE->SetGain(RENDER_ENGINE->getVolumeSounde());

    g_window = RENDER_ENGINE->getWindowSize();


    //��������� ������������ ������ OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    PWindow = glfwCreateWindow(g_window.x, g_window.y, "Game", RENDER_ENGINE->getMonitor(), NULL); 

    if (!PWindow){//�������� ��������
        glfwTerminate();//����������� GLFW � ������������� ����������� �������������
        std::cerr << "(!) PWindow failed" << std::endl;
        return -1;
    }

    glfwSetWindowSizeCallback(PWindow, glfwWindowSizeCallback);//�������� � GLFW ������� ��������� ������� ���� ����������
    glfwSetKeyCallback(PWindow, glfwKeyCallback);//�������� � GLFW ������� ������������� �������
    glfwSetMouseButtonCallback(PWindow, glfwMouseCallback);
    glfwSetScrollCallback(PWindow, glfwMouseScrollCallback);
    glfwSetCharCallback(PWindow, glfwCharCallback);
    glfwSetMonitorCallback(glfwMonitorCallBack);
    glfwMakeContextCurrent(PWindow);//��������� �������� ���� ��������
	
	if(!gladLoadGL()){//�������� OpenGL
        glfwTerminate();//����������� GLFW � ������������� ����������� �������������
		std::cerr<<"(!) gladLoadGL failed"<<std::endl;
		return -1;
	}

    //����� ������
    std::cout << "Renderer: " << RENDER_ENGINE->getRender() << std::endl;
    std::cout << "OpenGL version: " << RENDER_ENGINE->getVersion() << std::endl;
    //

    MOUSE->SetWindow(PWindow);
    MOUSE->SetHeight(g_window.y);

    //
    RENDER_ENGINE->enableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    RENDER_ENGINE->setClearColor(0.2f, 0.2f, 0.2f, 1.f);//���� ����������
    RENDER_ENGINE->setDetphTest(true);
    {
        RESOURCE_MANAGER->setExecutablePath(argv[0]);//�������� ���� � ���������
       
        MAIN_GAME_CLASS->init();
        MAIN_GAME_CLASS->SetProjectionMat(g_window);

        //������
        double lastTime = glfwGetTime();
        while (!glfwWindowShouldClose(PWindow)) {
            double currTime = glfwGetTime();//���������� �������
            double dura = (currTime - lastTime)*1000;//������� ������������� �������
            lastTime = currTime;//����� �������

            MAIN_GAME_CLASS->update(dura);

            //���������� ������ �������� � glClearColor
            RENDER_ENGINE->clear();

            MAIN_GAME_CLASS->render();

            glfwSwapBuffers(PWindow);//������ ������ ���������
            glfwPollEvents();//��������� �������
            MAIN_GAME_CLASS->Events();
        }
        RESOURCE_MANAGER->unloadAllRes();
    }
    RENDER_ENGINE->saveConfig(argv[0]);
    MainGameClass::Terminate();
    Renderer::PrintText::Terminate();
    Audio::SoundDevice::Terminate();
    Audio::SoundEffectsLibrary::Terminate();
    glfwTerminate();//����������� GLFW � ������������� ����������� �������������
    return 0;
}