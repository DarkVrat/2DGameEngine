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

MainGameClass::MainGameClass() {
    m_window=glm::ivec2(0, 0);
    m_GameState=E_GAME_STATE::Pause;
    m_fps=0;
    m_time = 0.0;
}

void MainGameClass::update(double duration){ 
    for (auto current = m_vectorGameObject.begin(); current != m_vectorGameObject.end(); current++) {
        current->get()->update(duration);
    }

    m_fps++;
    m_time += duration;
    if (m_time > 1000) {
        PRINT_TEXT::addTextInTimeBuffer(std::to_string(m_fps), glm::vec3(0, 0, 100), 0.25, glm::vec3(1, 1, 1), 1000);
        m_fps = 0;
        m_time -= 1000;
    }

    PRINT_TEXT::updateBuffer(duration);
    MOUSE::updatePosition();
}

void MainGameClass::render() {
    for (auto current = m_vectorGameObject.begin(); current != m_vectorGameObject.end(); current++) {
        current->get()->render();
    }
    PRINT_TEXT::renderText("Hello world! -> Привет мир!", glm::vec3(10, 400, 1), 0.5, glm::vec3(1, 1, 1));
    PRINT_TEXT::renderBuffer();
    PRINT_TEXT::renderText(KEYBOARD::getBuffer(), glm::vec3(500, 600, 100), 0.5, glm::vec3(0.5, 1, 0.5));
    PRINT_TEXT::renderText("x: " + std::to_string(MOUSE::getPosition().x) + " y: " + std::to_string(MOUSE::getPosition().y), glm::vec3(10, 500, 0), 0.5, glm::vec3(1, 1, 1));
}

void MainGameClass::sortGameObject(){
    auto comp = [](std::shared_ptr<GameObject> a, std::shared_ptr<GameObject> b) {return a->getLayer() < b->getLayer();};
    std::sort(m_vectorGameObject.begin(), m_vectorGameObject.end(), comp);
}

bool MainGameClass::init() {
    RESOURCE_MANAGER::loadJSONResurces("res/resJSON/resources.json");

    SOUND_DEVICE::setGain(CONFIG_MANAGER::getVolumeSounde());
    SOUND_DEVICE::setAttunation(AL_INVERSE_DISTANCE_CLAMPED);
    SOUND_DEVICE::setPosition(glm::vec3(420.f, 128.f, 0.f));
    SOUND_DEVICE::setOrientation(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));

    m_vectorGameObject.reserve(sizeof(GameObject));

    m_vectorGameObject.emplace_back(std::make_shared<GameObject>("Attack1", glm::vec2(100, 128), glm::vec2(256, 256), 0.f, -4.f));
    m_vectorGameObject.emplace_back(std::make_shared<GameObject>("Attack1", glm::vec2(180, 128), glm::vec2(256, 256), 0.f, -3.f));
    m_vectorGameObject.emplace_back(std::make_shared<GameObject>("Attack1", glm::vec2(260, 128), glm::vec2(256, 256), 0.f, -2.f));
    m_vectorGameObject.emplace_back(std::make_shared<GameObject>("Attack1", glm::vec2(340, 128), glm::vec2(256, 256), 0.f, -1.f));
    m_vectorGameObject.emplace_back(std::make_shared<GameObject>("Attack1", glm::vec2(420, 128), glm::vec2(256, 256), 0.f, 1.f));
    m_vectorGameObject.emplace_back(std::make_shared<GameObject>("Attack1", glm::vec2(500, 128), glm::vec2(-256, 256), 0.f, -0.1f));
    m_vectorGameObject.emplace_back(std::make_shared<GameObject>("Attack1", glm::vec2(580, 128), glm::vec2(-256, 256), 0.f, -0.2f));
    m_vectorGameObject.emplace_back(std::make_shared<GameObject>("Attack1", glm::vec2(660, 128), glm::vec2(-256, 256), 0.f, -0.3f));
    m_vectorGameObject.emplace_back(std::make_shared<GameObject>("Attack1", glm::vec2(740, 128), glm::vec2(-256, 256), 0.f, -0.4f));

    sortGameObject();

    setProjectionMat(m_window);
    
    return true;
}

void MainGameClass::events(){
    if (KEYBOARD::getWriteText()) {
        if (KEYBOARD::ifPressed(GLFW_KEY_ENTER)) {
            PRINT_TEXT::addTextInTimeBuffer(KEYBOARD::getBufferAndRemove(), glm::vec3(400, 300, 100), 0.5, glm::vec3(1, 1, 1), 5000.0);
        }
        if (KEYBOARD::ifPressed(GLFW_KEY_BACKSPACE)) {
            KEYBOARD::deleteLastCharInBuffer();
        }

        MOUSE::updateButton();
        KEYBOARD::updateButton();
        return;
    }

    if (KEYBOARD::ifPressed(GLFW_KEY_1)) m_vectorGameObject[0]->attack();
    if (KEYBOARD::ifPressed(GLFW_KEY_2)) m_vectorGameObject[1]->attack();
    if (KEYBOARD::ifPressed(GLFW_KEY_3)) m_vectorGameObject[2]->attack();
    if (KEYBOARD::ifPressed(GLFW_KEY_4)) m_vectorGameObject[3]->attack();
    if (KEYBOARD::ifPressed(GLFW_KEY_5)) m_vectorGameObject[8]->attack();
    if (KEYBOARD::ifPressed(GLFW_KEY_6)) m_vectorGameObject[7]->attack();
    if (KEYBOARD::ifPressed(GLFW_KEY_7)) m_vectorGameObject[6]->attack();
    if (KEYBOARD::ifPressed(GLFW_KEY_8)) m_vectorGameObject[5]->attack();
    if (KEYBOARD::ifPressed(GLFW_KEY_9)) m_vectorGameObject[4]->attack();
    
    if (KEYBOARD::ifReleased(GLFW_KEY_1)) m_vectorGameObject[0]->idle();
    if (KEYBOARD::ifReleased(GLFW_KEY_2)) m_vectorGameObject[1]->idle();
    if (KEYBOARD::ifReleased(GLFW_KEY_3)) m_vectorGameObject[2]->idle();
    if (KEYBOARD::ifReleased(GLFW_KEY_4)) m_vectorGameObject[3]->idle();
    if (KEYBOARD::ifReleased(GLFW_KEY_5)) m_vectorGameObject[8]->idle();
    if (KEYBOARD::ifReleased(GLFW_KEY_6)) m_vectorGameObject[7]->idle();
    if (KEYBOARD::ifReleased(GLFW_KEY_7)) m_vectorGameObject[6]->idle();
    if (KEYBOARD::ifReleased(GLFW_KEY_8)) m_vectorGameObject[5]->idle();
    if (KEYBOARD::ifReleased(GLFW_KEY_9)) m_vectorGameObject[4]->idle();
    
    if (KEYBOARD::ifPressed(GLFW_KEY_SPACE)) {
        PRINT_TEXT::addTextInTimeBuffer("space it ok", glm::vec3(200, 600, 100), 0.5, glm::vec3(1, 1, 1), 5000.0);
    }

    if (MOUSE::ifPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        PRINT_TEXT::addTextInCountBuffer("Mouse press", glm::vec3(200, 450, 100), 0.5, glm::vec3(1, 1, 1));
    }

    if (KEYBOARD::ifPressed(GLFW_KEY_Y)) {
        KEYBOARD::startWritingText();
    }

    if (KEYBOARD::ifPressed(GLFW_KEY_Q)) {
        SOUND_MANAGER::updateSoundSystem();
    }

    PRINT_TEXT::addTextInCountBuffer("scroll x: " + std::to_string(MOUSE::getScroll().x) + " scroll y: " + std::to_string(MOUSE::getScroll().y), glm::vec3(10, 550, 0), 0.5, glm::vec3(1, 1, 1));

    MOUSE::updateButton();
    KEYBOARD::updateButton();
}

void MainGameClass::setProjectionMat(glm::ivec2 window){
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

MainGameClass::~MainGameClass(){
    m_vectorGameObject.clear();
}
