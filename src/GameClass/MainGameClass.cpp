#include "MainGameClass.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <rapidjson/document.h>

MainGameClass::MainGameClass(const glm::ivec2& window) :m_GState(E_GAME_STATE::Active), m_window(window) {
    m_keys.fill(false);
}

MainGameClass::~MainGameClass() {
}

void MainGameClass::update(double duration){
    for (auto current : m_GObject) {
        current.update(duration);
    }
}

void MainGameClass::render() {
    for (auto current : m_GObject) {
        current.render();
    }
}

void MainGameClass::setKey(const int key, const int action) {
    m_keys[key] = action;
}

bool MainGameClass::init() {
    ResourceManager::loadJSONResurces("res/resJSON/resources.json");

    auto pSpriteShaderProgram = ResourceManager::getShader("spriteShader");

    m_GObject.emplace_back("Attack1", glm::vec2(0, 100), glm::vec2(256, 256), 0.f, -4.f);
    m_GObject.emplace_back("Attack1", glm::vec2(60, 100), glm::vec2(256, 256), 0.f, -3.f);
    m_GObject.emplace_back("Attack1", glm::vec2(120, 100), glm::vec2(256, 256), 0.f, -2.f);
    m_GObject.emplace_back("Attack1", glm::vec2(180, 100), glm::vec2(256, 256), 0.f, -1.f);
    m_GObject.emplace_back("Attack1", glm::vec2(240, 100), glm::vec2(256, 256), 0.f, 0.f);
    m_GObject.emplace_back("Attack1", glm::vec2(300, 100), glm::vec2(256, 256), 0.f, -0.1f);
    m_GObject.emplace_back("Attack1", glm::vec2(360, 100), glm::vec2(256, 256), 0.f, -0.2f);
    m_GObject.emplace_back("Attack1", glm::vec2(420, 100), glm::vec2(256, 256), 0.f, -0.3f);
    m_GObject.emplace_back("Attack1", glm::vec2(480, 100), glm::vec2(256, 256), 0.f, -0.4f);

    //Матрица для шейдера
    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_window.x), 0.f, static_cast<float>(m_window.y), -100.f, 100.f);

    //Активация шейдера
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

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
}
