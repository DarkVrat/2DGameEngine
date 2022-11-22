#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>


#include <chrono>
#include <iostream>
#include "Resources/ResourceManager.h"
#include "GameClass/MainGameClass.h"
#include "Renderer/RenderEngine.h"

//Размер окна
glm::ivec2 g_window(1280, 720);
MainGameClass g_Game(g_window);

//При измении окна
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_window.x = width;
    g_window.y = height;
    Renderer::RenderEngine::setViewport(g_window.x, g_window.y);
}

//проверка нажатия кнопок
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    g_Game.setKey(key, action);
}

int main(int argc, char** argv){

    if (!glfwInit()) { //Инициализация библиотеки GLFW
        std::cerr << "(!) glfwInit failed" << std::endl;
        return -1;
    }

    //Установка используемой версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Создание окна, размером g_window.x на g_window.y, с именем Game, в оконном режиме, и контекст использования общих ресурсов
    GLFWwindow* PWindow = glfwCreateWindow(g_window.x, g_window.y, "Game", NULL, NULL);

    if (!PWindow){//Проверка создания
        glfwTerminate();//Уничтожение GLFW и невозможность дальнейшего использования
        std::cerr << "(!) PWindow failed" << std::endl;
        return -1;
    }

    glfwSetWindowSizeCallback(PWindow, glfwWindowSizeCallback);//Передача в GLFW функции изменения размера окна приложения
    glfwSetKeyCallback(PWindow, glfwKeyCallback);//Передача в GLFW функции отслеживающей нажатия
    glfwMakeContextCurrent(PWindow);//Указывает контекст окна активным
	
	if(!gladLoadGL()){//загрузка OpenGL
        glfwTerminate();//Уничтожение GLFW и невозможность дальнейшего использования
		std::cerr<<"(!) gladLoadGL failed"<<std::endl;
		return -1;
	}

    //Вывод версии
    std::cout << "Renderer: " << Renderer::RenderEngine::getRender() << std::endl;
    std::cout << "OpenGL version: " << Renderer::RenderEngine::getVersion() << std::endl;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Renderer::RenderEngine::setClearColor(0.f, 0.f, 0.f, 1.f);//цвет заполнения
    Renderer::RenderEngine::setDetphTest(true);
    {
        ResourceManager::setExecutablePath(argv[0]);//Передача пути к программе
       
        g_Game.init();

        //Таймер
        auto lastTime = std::chrono::high_resolution_clock::now();
        int i = 0;
        double t= 0;
        while (!glfwWindowShouldClose(PWindow)) {
            auto currTime = std::chrono::high_resolution_clock::now();//Обновление таймера
            double dura = std::chrono::duration<double, std::milli>(currTime - lastTime).count();//Просчёт изменившегося времени
            lastTime = currTime;//сдвиг таймера
            
            t += dura;
            i++;
            g_Game.update(dura);

            if (t > 1000.0) {
                std::cerr << "fps: " << i << std::endl;
                t -= 1000.0;
                i = 0;
            }
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