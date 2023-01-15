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

//Проверить позже на ноутбуке
//extern "C" {
//    _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
//    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
//}


//Размер окна
glm::ivec2 g_window;

//При измении окна
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_window.x = width;
    g_window.y = height;
    RENDER_ENGINE->setViewport(g_window.x, g_window.y);
    Control::MouseControl::Get()->SetHeight(g_window.y);
    MAIN_GAME_CLASS->SetProjectionMat(g_window);
}

//проверка нажатия кнопок
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    KEYBOARD->SetKey(key, action);
}

void glfwMouseCallback(GLFWwindow* pWindow, int button, int action, int mods) {
    MOUSE->SetKey(button, action); 
}

void glfwMouseScrollCallback(GLFWwindow* окно, double x, double y){
    MOUSE->SetScroll(glm::vec2(x, y));
}

int main(int argc, char** argv){

    if (!glfwInit()) { //Инициализация библиотеки GLFW
        std::cerr << "(!) glfwInit failed" << std::endl;
        return -1;
    }

    RENDER_ENGINE->loadConfig(argv[0]);
    SOUND_DEVICE->SetGain(RENDER_ENGINE->getVolumeSounde());

    g_window = RENDER_ENGINE->getWindowSize();


    //Установка используемой версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* PWindow = nullptr;

    if (RENDER_ENGINE->getFullScreen()) {
        int count;
        GLFWmonitor** monitors = glfwGetMonitors(&count);
        if (count > RENDER_ENGINE->getDisplayNumber()) {
            PWindow = glfwCreateWindow(g_window.x, g_window.y, "Game", monitors[RENDER_ENGINE->getDisplayNumber()], NULL);
        }
    }

    if(!PWindow) {
        PWindow = glfwCreateWindow(g_window.x, g_window.y, "Game", NULL, NULL);
    }

    if (!PWindow){//Проверка создания
        glfwTerminate();//Уничтожение GLFW и невозможность дальнейшего использования
        std::cerr << "(!) PWindow failed" << std::endl;
        return -1;
    }

    glfwSetWindowSizeCallback(PWindow, glfwWindowSizeCallback);//Передача в GLFW функции изменения размера окна приложения
    glfwSetKeyCallback(PWindow, glfwKeyCallback);//Передача в GLFW функции отслеживающей нажатия
    glfwSetMouseButtonCallback(PWindow, glfwMouseCallback);
    glfwSetScrollCallback(PWindow, glfwMouseScrollCallback);
    glfwMakeContextCurrent(PWindow);//Указывает контекст окна активным
	
	if(!gladLoadGL()){//загрузка OpenGL
        glfwTerminate();//Уничтожение GLFW и невозможность дальнейшего использования
		std::cerr<<"(!) gladLoadGL failed"<<std::endl;
		return -1;
	}

    //Вывод версии
    std::cout << "Renderer: " << RENDER_ENGINE->getRender() << std::endl;
    std::cout << "OpenGL version: " << RENDER_ENGINE->getVersion() << std::endl;
    //

    MOUSE->SetWindow(PWindow);
    MOUSE->SetHeight(g_window.y);

    //
    RENDER_ENGINE->enableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    RENDER_ENGINE->setClearColor(0.2f, 0.2f, 0.2f, 1.f);//цвет заполнения
    RENDER_ENGINE->setDetphTest(true);
    {
        RESOURCE_MANAGER->setExecutablePath(argv[0]);//Передача пути к программе
       
        MAIN_GAME_CLASS->init();
        MAIN_GAME_CLASS->SetProjectionMat(g_window);

        //Таймер
        double lastTime = glfwGetTime();
        while (!glfwWindowShouldClose(PWindow)) {
            double currTime = glfwGetTime();//Обновление таймера
            double dura = (currTime - lastTime)*1000;//Просчёт изменившегося времени
            lastTime = currTime;//сдвиг таймера

            MAIN_GAME_CLASS->update(dura);

            //Заполнение цветом указаном в glClearColor
            RENDER_ENGINE->clear();

            MAIN_GAME_CLASS->render();

            glfwSwapBuffers(PWindow);//Меняет буферы отрисовки
            glfwPollEvents();//Обработка событий
            MAIN_GAME_CLASS->Events();
        }
        RESOURCE_MANAGER->unloadAllRes();
    }
    glfwTerminate();//Уничтожение GLFW и невозможность дальнейшего использования
    return 0;
}