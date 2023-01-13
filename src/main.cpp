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




//Размер окна
glm::ivec2 g_window;
MainGameClass g_Game;

//При измении окна
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_window.x = width;
    g_window.y = height;
    Renderer::RenderEngine::setViewport(g_window.x, g_window.y);
    Control::MouseControl::Get()->SetHeight(g_window.y);
    g_Game.SetProjectionMat(g_window);
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

    Renderer::RenderEngine::init(argv[0]);

    g_window = Renderer::RenderEngine::getWindowSize();
    g_Game = MainGameClass(g_window);

    //Установка используемой версии OpenGL
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
    std::cout << "Renderer: " << Renderer::RenderEngine::getRender() << std::endl;
    std::cout << "OpenGL version: " << Renderer::RenderEngine::getVersion() << std::endl;
    //

    MOUSE->SetWindow(PWindow);
    MOUSE->SetHeight(g_window.y);

    //
    Renderer::RenderEngine::enableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Renderer::RenderEngine::setClearColor(0.2f, 0.2f, 0.2f, 1.f);//цвет заполнения
    Renderer::RenderEngine::setDetphTest(true);
    {
        ResourceManager::setExecutablePath(argv[0]);//Передача пути к программе
       
        g_Game.init();

        //Таймер
        double lastTime = glfwGetTime();
        while (!glfwWindowShouldClose(PWindow)) {
            double currTime = glfwGetTime();//Обновление таймера
            double dura = (currTime - lastTime)*1000;//Просчёт изменившегося времени
            lastTime = currTime;//сдвиг таймера

            g_Game.update(dura);

            //Заполнение цветом указаном в glClearColor
            Renderer::RenderEngine::clear();

            g_Game.render();

            glfwSwapBuffers(PWindow);//Меняет буферы отрисовки
            glfwPollEvents();//Обработка событий
            g_Game.Events();
        }
        ResourceManager::unloadAllRes();
    }
    glfwTerminate();//Уничтожение GLFW и невозможность дальнейшего использования
    return 0;
}