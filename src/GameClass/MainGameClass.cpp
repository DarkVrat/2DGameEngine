#include "MainGameClass.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "../Resources/ResourceManager.h"
#include "../Renderer/AllRender.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <rapidjson/document.h>
#include "../Audio/AllAudio.h"
#include "../Control/MouseControl.h"

MainGameClass::MainGameClass(const glm::ivec2& window) :m_GState(E_GAME_STATE::Active), m_window(window) {
    m_keys.fill(false);
}

MainGameClass::~MainGameClass() {
}

void MainGameClass::update(double duration){
    for (auto current : m_GObject) {
        current.update(duration);
    }
    Renderer::PrintText::updateBuffer(duration);
    MOUSE->Update();
}

void MainGameClass::render() {
    for (auto current : m_GObject) {
        current.render();
    }
    Renderer::PrintText::RenderText("Hello world! -> Привет мир!", glm::vec3(10, 400, 1), 0.5, glm::vec3(1, 1, 1));
    Renderer::PrintText::renderBuffer();
    Renderer::PrintText::RenderText("x: " + std::to_string(MOUSE->GetPosition().x) + " y: " + std::to_string(MOUSE->GetPosition().y), glm::vec3(10, 500, 0), 0.5, glm::vec3(1, 1, 1));
}

void MainGameClass::setKey(const int key, const int action) {
    m_keys[key] = action;
}

bool MainGameClass::init() {
    ResourceManager::loadJSONResurces("res/resJSON/resources.json");

    GlobalSoundDevice->SetAttunation(AL_INVERSE_DISTANCE_CLAMPED);
    GlobalSoundDevice->SetPosition(glm::vec3(420.f, 128.f, 0.f));
    GlobalSoundDevice->SetOrientation(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));

    m_GObject.reserve(sizeof(GameObject));

    m_GObject.emplace_back("Attack1", glm::vec2(100, 128), glm::vec2(256, 256), 0.f, -4.f);
    m_GObject.emplace_back("Attack1", glm::vec2(180, 128), glm::vec2(256, 256), 0.f, -3.f);
    m_GObject.emplace_back("Attack1", glm::vec2(260, 128), glm::vec2(256, 256), 0.f, -2.f);
    m_GObject.emplace_back("Attack1", glm::vec2(340, 128), glm::vec2(256, 256), 0.f, -1.f);
    m_GObject.emplace_back("Attack1", glm::vec2(420, 128), glm::vec2(256, 256), 0.f, 1.f);
    m_GObject.emplace_back("Attack1", glm::vec2(500, 128), glm::vec2(-256, 256), 0.f, -0.1f);
    m_GObject.emplace_back("Attack1", glm::vec2(580, 128), glm::vec2(-256, 256), 0.f, -0.2f);
    m_GObject.emplace_back("Attack1", glm::vec2(660, 128), glm::vec2(-256, 256), 0.f, -0.3f);
    m_GObject.emplace_back("Attack1", glm::vec2(740, 128), glm::vec2(-256, 256), 0.f, -0.4f);

    SetProjectionMat(m_window);

    Renderer::PrintText::init(ResourceManager::getShader("textShader"));
    
    return true;
}

void MainGameClass::Events(){
    if (m_keys[GLFW_KEY_1] == GLFW_PRESS) m_GObject[0].attack();
    if (m_keys[GLFW_KEY_2] == GLFW_PRESS) m_GObject[1].attack();
    if (m_keys[GLFW_KEY_3] == GLFW_PRESS) m_GObject[2].attack();
    if (m_keys[GLFW_KEY_4] == GLFW_PRESS) m_GObject[3].attack();
    if (m_keys[GLFW_KEY_5] == GLFW_PRESS) m_GObject[4].attack();
    if (m_keys[GLFW_KEY_6] == GLFW_PRESS) m_GObject[5].attack();
    if (m_keys[GLFW_KEY_7] == GLFW_PRESS) m_GObject[6].attack();
    if (m_keys[GLFW_KEY_8] == GLFW_PRESS) m_GObject[7].attack();
    if (m_keys[GLFW_KEY_9] == GLFW_PRESS) m_GObject[8].attack();
    

    if (m_keys[GLFW_KEY_1] == GLFW_RELEASE) m_GObject[0].idle();
    if (m_keys[GLFW_KEY_2] == GLFW_RELEASE) m_GObject[1].idle();
    if (m_keys[GLFW_KEY_3] == GLFW_RELEASE) m_GObject[2].idle();
    if (m_keys[GLFW_KEY_4] == GLFW_RELEASE) m_GObject[3].idle();
    if (m_keys[GLFW_KEY_5] == GLFW_RELEASE) m_GObject[4].idle();
    if (m_keys[GLFW_KEY_6] == GLFW_RELEASE) m_GObject[5].idle();
    if (m_keys[GLFW_KEY_7] == GLFW_RELEASE) m_GObject[6].idle();
    if (m_keys[GLFW_KEY_8] == GLFW_RELEASE) m_GObject[7].idle();
    if (m_keys[GLFW_KEY_9] == GLFW_RELEASE) m_GObject[8].idle();
    
    if (m_keys[GLFW_KEY_SPACE] == GLFW_PRESS) {
        Renderer::PrintText::AddTextInTimeBuffer("space it ok", glm::vec3(200, 600, 100), 0.5, glm::vec3(1, 1, 1), 5000.0);
    }

    if (MOUSE->IfPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        Renderer::PrintText::AddTextInTimeBuffer("Mouse press", glm::vec3(200, 450, 100), 0.5, glm::vec3(1, 1, 1), 1000.0);
    }

    Renderer::PrintText::AddTextInCountBuffer("scroll x: " + std::to_string(MOUSE->GetScroll().x) + " scroll y: " + std::to_string(MOUSE->GetScroll().y), glm::vec3(10, 550, 0), 0.5, glm::vec3(1, 1, 1));
}

void MainGameClass::SetProjectionMat(glm::ivec2 window){
    m_window = window;

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_window.x), 0.f, static_cast<float>(m_window.y), -100.f, 100.f);

    auto pSpriteShaderProgram = ResourceManager::getShader("spriteShader");
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    auto pTextShaderProgram = ResourceManager::getShader("textShader");
    pTextShaderProgram->use();
    pTextShaderProgram->setMatrix4("projection", projectionMatrix);
}
