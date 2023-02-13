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

std::vector<std::shared_ptr<GameObject>> MainGameClass::m_vectorGameObject;
MainGameClass::E_GAME_STATE MainGameClass::m_GameState;
glm::ivec2 MainGameClass::m_window;
double MainGameClass::m_time;
int MainGameClass::m_fps;
int MainGameClass::size;

 //(RUS) �������������� ������������� ����������
//(ENG) initial initialization of variables
MainGameClass::MainGameClass() {
    m_window=glm::ivec2(0, 0);
    m_GameState=E_GAME_STATE::MAIN_MENU;
    m_fps=0;
    m_time = 0.0;
}

 //(RUS) ���������� ��������, ������� ������, ������� ����
//(ENG) updating objects, text buffers, mouse positions
void MainGameClass::update(const double& duration){ 
    for (int i = 0; i < m_vectorGameObject.size(); i++) {
        m_vectorGameObject[i]->update(duration);
    }

    m_fps++;
    m_time += duration;
    if (m_time > 1000) {
        PRINT_TEXT::addTextInTimeBuffer(std::to_string(m_fps), glm::vec3(0, 0, 100), 0.25, glm::vec3(1, 1, 1), 1000);
        m_fps = 0;
        m_time -= 1000;
    }

    PRINT_TEXT::updateBuffer(duration);
    MOUSE::updatePositionAndScroll();
}

 //(RUS) ��������� �������� � ������
//(ENG) drawing objects and text
void MainGameClass::render() {
    for (int i = 0; i < m_vectorGameObject.size(); i++) {
        m_vectorGameObject[i]->render();
    }
    PRINT_TEXT::renderBuffer();
    PRINT_TEXT::renderText("�������� ������ ����� �������� ����, ������� ������ "+std::to_string(size), glm::vec3(20, 850, 10), 0.25, glm::vec3(1, 1, 1));
    PRINT_TEXT::renderTextWrapping("���� ����� ����� ��� �������� ����������������� ��������, ����������� �� ������ ����", glm::vec3(100,300,10),0.5, glm::vec3(1, 1, 1), size, LEFT);
    PRINT_TEXT::renderTextWrapping("���� ����� ����� ��� �������� ����������������� ��������, ����������� �� ������", glm::vec3(100, 700, 10), 0.5, glm::vec3(1, 1, 1), size, CENTR);
}

 //(RUS) ���������� �������� �� �� layer, ��� ���������
//(ENG) sorting objects by their layer, for rendering
void MainGameClass::sortGameObject(){
    auto comp = [](std::shared_ptr<GameObject> a, std::shared_ptr<GameObject> b) {return a->getLayer() < b->getLayer();};
    std::sort(m_vectorGameObject.begin(), m_vectorGameObject.end(), comp);
}

 //(RUS) �������������, �������� ��������, ��������� ���������� ��� �����::SoundDevice, �������� ��������
//(ENG) Initialization, loading resources, setting parameters for Audio::SoundDevice, creating objects
bool MainGameClass::init() {
    RESOURCE_MANAGER::loadJSONResurces("res/resJSON/resources.json");

    SOUND_DEVICE::setGain(CONFIG_MANAGER::getVolumeSounde());
    SOUND_DEVICE::setAttunation(AL_INVERSE_DISTANCE_CLAMPED);
    SOUND_DEVICE::setPosition(glm::vec3(420.f, 128.f, 0.f));
    SOUND_DEVICE::setOrientation(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));

    m_vectorGameObject.reserve(sizeof(GameObject));

    setProjectionMat(m_window);
    
    return true;
}

 //(RUS) ��������� �������
//(ENG) Handling clicks
void MainGameClass::events(){
    size += MOUSE::getScroll().y * 10;
    if (size < 0)size = 0;
}

 //(RUS) ��������� ������� �������� ��� ���������
//(ENG) Setting the projection matrix for rendering
void MainGameClass::setProjectionMat(const glm::ivec2& window){
    m_window = window;

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_window.x), 0.f, static_cast<float>(m_window.y), -100.f, 100.f);

    auto pSpriteShaderProgram = RESOURCE_MANAGER::getShader("spriteShader");
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    auto pTextShaderProgram = RESOURCE_MANAGER::getShader("textShader");
    pTextShaderProgram->use();
    pTextShaderProgram->setMatrix4("projection", projectionMatrix);
}

 //(RUS) ����������� ������� ��������
//(ENG) Destruction of game objects
void MainGameClass::terminate(){
    m_vectorGameObject.clear();
}
