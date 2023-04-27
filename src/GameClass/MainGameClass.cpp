#include "MainGameClass.h"

#include <algorithm>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Audio/SoundDevice.h"
#include "../Control/MouseControl.h"
#include "../Control/KeyboardControl.h"
#include "../Managers/SoundManager.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/ResourceManager.h"
#include "../Renderer/PrintText.h"
#include "../Renderer/RenderEngine.h"
#include "../GameEngine/Objects/Camera.h"

Menu MainGameClass::m_Menu;
glm::ivec2 MainGameClass::m_window;
double MainGameClass::m_time;
int MainGameClass::m_fps;
bool MainGameClass::m_StateGame;
GameScene MainGameClass::m_gameScene;

 //(RUS) первоначальная инициализация переменных
//(ENG) initial initialization of variables
MainGameClass::MainGameClass() {
    m_window=glm::ivec2(0, 0);
    m_fps=0;
    m_time = 0.0;
}

 //(RUS) обновление объектов, буферов текста, позиции мыши
//(ENG) updating objects, text buffers, mouse positions
void MainGameClass::update(const double& duration) {

    m_fps++;
    m_time += duration;
    if (m_time > 1000) {
        PRINT_TEXT::printText(TEXT(std::to_string(m_fps), glm::vec3(0, 0.97, 10), 0.03, glm::vec3(1, 1, 1)), 1000);
        m_fps = 0;
        m_time -= 1000;
    }

    if (m_StateGame) {
        m_gameScene.update(duration);
    }
    else {
        m_Menu.update(duration);
    }

    PRINT_TEXT::updateBuffer(duration);
    MOUSE::updatePositionAndScroll();
}

 //(RUS) отрисовка объектов и текста
//(ENG) drawing objects and text
void MainGameClass::render() {
    if (m_StateGame) {
        m_gameScene.render();
    }
    else {
        m_Menu.render();
    }

    RENDER_ENGINE::render();
    PRINT_TEXT::renderBuffer();

}

 //(RUS) Инициализация, загрузка ресурсов, установка параметров для Аудио::SoundDevice, создание объектов
//(ENG) Initialization, loading resources, setting parameters for Audio::SoundDevice, creating objects
bool MainGameClass::init(glm::vec2 window) {
    RESOURCE_MANAGER::loadJSONResurces();
     
    SOUND_DEVICE::setGain(CONFIG_MANAGER::getVolumeSounde());
    SOUND_DEVICE::setAttunation(AL_INVERSE_DISTANCE_CLAMPED);
    SOUND_DEVICE::setPosition(glm::vec3(0.f, 0.f, 0.f));
    SOUND_DEVICE::setOrientation(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));

    setProjectionMat(window);

    m_StateGame = false;

    m_Menu.init("MainMenu");

    return true;
}

 //(RUS) Обработка нажатий
//(ENG) Handling clicks
void MainGameClass::events(){ 
    if (m_StateGame) {
        m_gameScene.events();
    }
    else {
        m_Menu.events();
    }
}

 //(RUS) Установка матрицы проекции для отрисовки
//(ENG) Setting the projection matrix for rendering
void MainGameClass::setProjectionMat(const glm::ivec2& window){
    m_window = window;

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_window.x), 0.f, static_cast<float>(m_window.y), -100.f, 100.f); 

    RESOURCE_MANAGER::setProjection(projectionMatrix, false);
    PRINT_TEXT::setWindow(m_window);
    Renderer::Sprite::setWindow(m_window);
    MOUSE::setWindowSize(m_window);
    CAMERA::updateSize();
    m_Menu.updateButton();
}

void MainGameClass::setGame(const bool& flag){
    m_StateGame = flag;
    if (flag) {
        m_Menu.stopSound();
    }
}

 //(RUS) Уничтожение игровых объектов
//(ENG) Destruction of game objects
void MainGameClass::terminate(){
    
}

Menu& MainGameClass::getMenu(){
    return m_Menu;
}

GameScene& MainGameClass::getScene(){
    return m_gameScene;
}
