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

//��������� ����� �� ��������
//extern "C" {
//    _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
//    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
//}


//������ ����
glm::ivec2 g_window;
MainGameClass g_Game;

//��� ������� ����
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_window.x = width;
    g_window.y = height;
    Renderer::RenderEngine::setViewport(g_window.x, g_window.y);
    Control::MouseControl::Get()->SetHeight(g_window.y);
    g_Game.SetProjectionMat(g_window);
}

//�������� ������� ������
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    KEYBOARD->SetKey(key, action);
}

void glfwMouseCallback(GLFWwindow* pWindow, int button, int action, int mods) {
    MOUSE->SetKey(button, action); 
}

void glfwMouseScrollCallback(GLFWwindow* ����, double x, double y){
    MOUSE->SetScroll(glm::vec2(x, y));
}

int main(int argc, char** argv){

    if (!glfwInit()) { //������������� ���������� GLFW
        std::cerr << "(!) glfwInit failed" << std::endl;
        return -1;
    }

    Renderer::RenderEngine::init(argv[0]);
    SOUND_DEVICE->SetGain(Renderer::RenderEngine::getVolumeSounde());

    g_window = Renderer::RenderEngine::getWindowSize();
    g_Game = MainGameClass(g_window);

    //��������� ������������ ������ OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* PWindow = nullptr;

    if (Renderer::RenderEngine::getFullScreen()) {
        int count;
        GLFWmonitor** monitors = glfwGetMonitors(&count);
        if (count > Renderer::RenderEngine::getDisplayNumber()) {
            PWindow = glfwCreateWindow(g_window.x, g_window.y, "Game", monitors[Renderer::RenderEngine::getDisplayNumber()], NULL);
        }
    }

    if(!PWindow) {
        PWindow = glfwCreateWindow(g_window.x, g_window.y, "Game", NULL, NULL);
    }

    if (!PWindow){//�������� ��������
        glfwTerminate();//����������� GLFW � ������������� ����������� �������������
        std::cerr << "(!) PWindow failed" << std::endl;
        return -1;
    }

    glfwSetWindowSizeCallback(PWindow, glfwWindowSizeCallback);//�������� � GLFW ������� ��������� ������� ���� ����������
    glfwSetKeyCallback(PWindow, glfwKeyCallback);//�������� � GLFW ������� ������������� �������
    glfwSetMouseButtonCallback(PWindow, glfwMouseCallback);
    glfwSetScrollCallback(PWindow, glfwMouseScrollCallback);
    glfwMakeContextCurrent(PWindow);//��������� �������� ���� ��������
	
	if(!gladLoadGL()){//�������� OpenGL
        glfwTerminate();//����������� GLFW � ������������� ����������� �������������
		std::cerr<<"(!) gladLoadGL failed"<<std::endl;
		return -1;
	}

    //����� ������
    std::cout << "Renderer: " << Renderer::RenderEngine::getRender() << std::endl;
    std::cout << "OpenGL version: " << Renderer::RenderEngine::getVersion() << std::endl;
    //

    MOUSE->SetWindow(PWindow);
    MOUSE->SetHeight(g_window.y);

    //
    Renderer::RenderEngine::enableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Renderer::RenderEngine::setClearColor(0.2f, 0.2f, 0.2f, 1.f);//���� ����������
    Renderer::RenderEngine::setDetphTest(true);
    {
        ResourceManager::setExecutablePath(argv[0]);//�������� ���� � ���������
       
        g_Game.init();

        //������
        double lastTime = glfwGetTime();
        while (!glfwWindowShouldClose(PWindow)) {
            double currTime = glfwGetTime();//���������� �������
            double dura = (currTime - lastTime)*1000;//������� ������������� �������
            lastTime = currTime;//����� �������

            g_Game.update(dura);

            //���������� ������ �������� � glClearColor
            Renderer::RenderEngine::clear();

            g_Game.render();

            glfwSwapBuffers(PWindow);//������ ������ ���������
            glfwPollEvents();//��������� �������
            g_Game.Events();
        }
        ResourceManager::unloadAllRes();
    }
    glfwTerminate();//����������� GLFW � ������������� ����������� �������������
    return 0;
}