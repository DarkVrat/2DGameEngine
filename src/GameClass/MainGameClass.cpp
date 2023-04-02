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


std::vector<std::shared_ptr<GameObject>> MainGameClass::m_vectorGameObject;
MainGameClass::E_GAME_STATE MainGameClass::m_GameState;
glm::ivec2 MainGameClass::m_window;
double MainGameClass::m_time;
int MainGameClass::m_fps;
float MainGameClass::size=0.5;
std::array<UserInterface::Button, 2> MainGameClass::m_testButton;
UserInterface::SwitchBool MainGameClass::m_testSwich;
UserInterface::Slider MainGameClass::m_testSliderVertical;
UserInterface::Slider MainGameClass::m_testSliderHorizontal;
glm::vec2 MainGameClass::m_vecToTestSliders=glm::vec2(0.1f , 0.1f);
UserInterface::ListParameter<glm::ivec2> MainGameClass::m_testList;
UserInterface::InputField MainGameClass::m_testInputField;
//
//std::array<UserInterface::Slider, 8> MainGameClass::m_SliderForSpriteSetting;
//glm::vec3 MainGameClass::m_vecForSpritePosition=glm::vec3(0.5f, 0.5f, 0.f);
//glm::vec2 MainGameClass::m_vecForSpriteSize=glm::vec2(0.1f, 0.1f);
//float MainGameClass::m_floatForSpriteRotation=0.f;
//glm::vec2 MainGameClass::m_vecForSpriteOrigin=glm::vec2(0.5f, 0.5f);
//std::shared_ptr<Renderer::Sprite> MainGameClass::m_sprite;

 //(RUS) первоначальная инициализация переменных
//(ENG) initial initialization of variables
MainGameClass::MainGameClass() {
    m_window=glm::ivec2(0, 0);
    m_GameState=E_GAME_STATE::MAIN_MENU; 
    m_fps=0;
    m_time = 0.0;
}

 //(RUS) обновление объектов, буферов текста, позиции мыши
//(ENG) updating objects, text buffers, mouse positions
void MainGameClass::update(const double& duration){ 
    for (int i = 0; i < m_vectorGameObject.size(); i++) {
        m_vectorGameObject[i]->update(duration);
    }

    m_fps++;
    m_time += duration;
    if (m_time > 1000) {
        PRINT_TEXT::printText(PRINT_TEXT::Text(std::to_string(m_fps), glm::vec3(0, 0, 10), 0.03, glm::vec3(1, 1, 1)), 1000);
        m_fps = 0;
        m_time -= 1000;
    }

    PRINT_TEXT::updateBuffer(duration);
    MOUSE::updatePositionAndScroll();
}

 //(RUS) отрисовка объектов и текста
//(ENG) drawing objects and text
void MainGameClass::render() {

    for (int i = 0; i < m_vectorGameObject.size(); i++) {
        m_vectorGameObject[i]->render();
    }
    PRINT_TEXT::printText(PRINT_TEXT::Text("Изменить размер можно колёсиком мыши, текущий размер "+std::to_string(size), glm::vec3(0, 0.97, 10), 0.03, glm::vec3(0.5, 1, 1)));
    PRINT_TEXT::printTextWrapping(PRINT_TEXT::Text("Этот текст нужен для проверки работоспособности переноса, выровняного по левому краю,", glm::vec3(0,0.93, 12), 0.04, glm::vec3(1, 0.5, 1)), size, LEFT);
    PRINT_TEXT::printTextWrapping(PRINT_TEXT::Text("Этот текст нужен для проверки работоспособности переноса, выровняного по центру", glm::vec3(0, 0.85, 13), 0.04, glm::vec3(1, 1, 0.5)), size, CENTR);
    PRINT_TEXT::printText(PRINT_TEXT::Text("Позиция мыши:" + std::to_string(MOUSE::getPosition().x)+" "+ std::to_string(MOUSE::getPosition().y), glm::vec3(0, 0.03, 10), 0.04, glm::vec3(1, 1, 1)));
    //m_testButton[0].render();
    //m_testButton[1].render();
   // m_testSwich.render(); 
   // PRINT_TEXT::printText(PRINT_TEXT::Text("FullScreen: " + std::to_string(CONFIG_MANAGER::getFullScreen()), glm::vec3(0, 0.07, 10), 0.03));
    m_testSliderVertical.render();
    m_testSliderHorizontal.render();
    m_testInputField.render();
   // m_testList.render(); 
    
    PRINT_TEXT::printText(PRINT_TEXT::Text(std::to_string(m_vecToTestSliders.x) + " " + std::to_string(m_vecToTestSliders.y), glm::vec3(0, 0.1, 10), 0.03));

    

    /*
    for (auto& currentSlider : m_SliderForSpriteSetting) {
        currentSlider.render();
    }
    PRINT_TEXT::printText(PRINT_TEXT::Text("x: " + std::to_string(m_vecForSpritePosition.x), glm::vec3(300, 624, 10)));
    PRINT_TEXT::printText(PRINT_TEXT::Text("y: " + std::to_string(m_vecForSpritePosition.y), glm::vec3(300, 584, 10)));
    PRINT_TEXT::printText(PRINT_TEXT::Text("z: " + std::to_string(m_vecForSpritePosition.z), glm::vec3(300, 544, 10)));
    PRINT_TEXT::printText(PRINT_TEXT::Text("height: " + std::to_string(m_vecForSpriteSize.y), glm::vec3(300, 504, 10)));
    PRINT_TEXT::printText(PRINT_TEXT::Text("width: " + std::to_string(m_vecForSpriteSize.x), glm::vec3(300, 464, 10)));
    PRINT_TEXT::printText(PRINT_TEXT::Text("rotation: " + std::to_string(m_floatForSpriteRotation), glm::vec3(300, 424, 10)));
    PRINT_TEXT::printText(PRINT_TEXT::Text("origin x: " + std::to_string(m_vecForSpriteOrigin.x), glm::vec3(300, 384, 10)));
    PRINT_TEXT::printText(PRINT_TEXT::Text("origin y: " + std::to_string(m_vecForSpriteOrigin.y), glm::vec3(300, 344, 10)));*/

    Translater::setLanguage("EN");
    PRINT_TEXT::printText(PRINT_TEXT::Text(TRANSLETE("menu", "newGame"), glm::vec3(0.85, 0.95, 10), 0.05));
    Translater::setLanguage("RU");
    PRINT_TEXT::printText(PRINT_TEXT::Text(TRANSLETE("menu", "continue"), glm::vec3(0.85, 0.9, 10), 0.05));
    Translater::setLanguage("NO");
    PRINT_TEXT::printText(PRINT_TEXT::Text(TRANSLETE("menu", "loadGame"), glm::vec3(0.85, 0.85, 10), 0.05));
    
   // m_sprite->render(m_vecForSpritePosition, m_vecForSpriteSize.y, m_vecForSpriteSize.x, m_floatForSpriteRotation, m_vecForSpriteOrigin);
     
    RENDER_ENGINE::render();
    PRINT_TEXT::renderBuffer();

}

 //(RUS) сортировка объектов по их layer, для отрисовки
//(ENG) sorting objects by their layer, for rendering
void MainGameClass::sortGameObject(){
    auto comp = [](std::shared_ptr<GameObject> a, std::shared_ptr<GameObject> b) {return a->getPosition().z < b->getPosition().z; };
    std::sort(m_vectorGameObject.begin(), m_vectorGameObject.end(), comp);
}

 //(RUS) Инициализация, загрузка ресурсов, установка параметров для Аудио::SoundDevice, создание объектов
//(ENG) Initialization, loading resources, setting parameters for Audio::SoundDevice, creating objects
bool MainGameClass::init(glm::vec2 window) {
    RESOURCE_MANAGER::loadJSONResurces();

    SOUND_DEVICE::setGain(CONFIG_MANAGER::getVolumeSounde());
    SOUND_DEVICE::setAttunation(AL_INVERSE_DISTANCE_CLAMPED);
    SOUND_DEVICE::setPosition(glm::vec3(420.f, 128.f, 0.f));
    SOUND_DEVICE::setOrientation(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));

    setProjectionMat(window);
     
    //------------------------//
    m_testButton[0].create(glm::vec3(0.25, 0.5, 10), glm::vec2(0.5, 0),E_STANDART, "size-0.1", 0.8, glm::vec3(0, 0, 0));
    m_testButton[0].setCallBack([]() {size -= 0.1;});
     
    m_testButton[1].create(glm::vec3(0.75, 0.5, 10), glm::vec2(0.5, 0),E_STANDART, "size+0.1", 0.8, glm::vec3(0, 0, 0));
    m_testButton[1].setCallBack([]() {size += 0.1;});

    m_testSwich.create(glm::vec3(0.05, 0.1, 10), glm::vec2(0, 0.05), CONFIG_MANAGER::getFullScreen(), glm::vec2(0,0));
    m_testSwich.setCallBack([](bool flag) {CONFIG_MANAGER::setFullScreen(flag);});
     
    m_testSliderVertical.create(glm::vec3(0.5, 0.05, 10), glm::vec2(0, 0.1), UI_VERTICAL_SLIDER, glm::vec2(0, 1), m_vecToTestSliders.x);
    m_testSliderVertical.setCallBack([](float value) {
            m_vecToTestSliders.x = value;
        });

    m_testSliderHorizontal.create(glm::vec3(0.015, 0.5, 10), glm::vec2(0.03, 0), UI_HORIZONTAL_SLIDER, glm::vec2(0, 1), m_vecToTestSliders.y);
    m_testSliderHorizontal.setCallBack([](float value) {
        m_vecToTestSliders.y = value;
        });

    std::vector<glm::ivec2> testList; 
    testList.push_back(glm::vec2(800, 600));
    testList.push_back(glm::vec2(1280, 720));
    testList.push_back(glm::vec2(1680, 1050));
    m_testList.create(glm::vec3(0.5, 0.2, 10), glm::vec2(0.2, 0), 0.8, testList);
    m_testList.setTypeToString([](glm::ivec2 value) {
            return std::to_string(value.x)+"x"+std::to_string(value.y);
        });

    m_testInputField.create(glm::vec3(0.5, 0.5, 10), glm::vec2(0.6, 0.3));
    m_testInputField.setCallBack([](std::string value) {
        PRINT_TEXT::printText(PRINT_TEXT::Text(value, glm::vec3(0.2, 0.65, 10), 0.05), 1000);
        });
    ////-----------------------------------------------//

    //m_SliderForSpriteSetting.at(0).create(glm::vec3(150, 640, 10), glm::vec2(300, 40), UI_VERTICAL_SLIDER, glm::vec2(0.f, 1.f), m_vecForSpritePosition.x);
    //m_SliderForSpriteSetting.at(0).setCallBack([](float value) {
    //    m_vecForSpritePosition.x = value;
    //    });

    //m_SliderForSpriteSetting.at(1).create(glm::vec3(150, 600, 10), glm::vec2(300, 40), UI_VERTICAL_SLIDER, glm::vec2(0.f, 1.f), m_vecForSpritePosition.y);
    //m_SliderForSpriteSetting.at(1).setCallBack([](float value) {
    //    m_vecForSpritePosition.y = value;
    //    });

    //m_SliderForSpriteSetting.at(2).create(glm::vec3(150, 560, 10), glm::vec2(300, 40), UI_VERTICAL_SLIDER, glm::vec2(-101.f, 101.f), m_vecForSpritePosition.z);
    //m_SliderForSpriteSetting.at(2).setCallBack([](float value) {
    //    m_vecForSpritePosition.z = value;
    //    });

    //m_SliderForSpriteSetting.at(3).create(glm::vec3(150, 520, 10), glm::vec2(300, 40), UI_VERTICAL_SLIDER, glm::vec2(-1.f, 1.f), m_vecForSpriteSize.y);
    //m_SliderForSpriteSetting.at(3).setCallBack([](float value) {
    //    m_vecForSpriteSize.y = value;
    //    });

    //m_SliderForSpriteSetting.at(4).create(glm::vec3(150, 480, 10), glm::vec2(300, 40), UI_VERTICAL_SLIDER, glm::vec2(-1.f, 1.f), m_vecForSpriteSize.x);
    //m_SliderForSpriteSetting.at(4).setCallBack([](float value) {
    //    m_vecForSpriteSize.x = value;
    //    });

    //m_SliderForSpriteSetting.at(5).create(glm::vec3(150, 440, 10), glm::vec2(300, 40), UI_VERTICAL_SLIDER, glm::vec2(0.f, 360.f), m_floatForSpriteRotation);
    //m_SliderForSpriteSetting.at(5).setCallBack([](float value) {
    //    m_floatForSpriteRotation = value;
    //    });

    //m_SliderForSpriteSetting.at(6).create(glm::vec3(150, 400, 10), glm::vec2(300, 40), UI_VERTICAL_SLIDER, glm::vec2(0.f, 1.f), m_vecForSpriteOrigin.x);
    //m_SliderForSpriteSetting.at(6).setCallBack([](float value) {
    //    m_vecForSpriteOrigin.x = value;
    //    });

    //m_SliderForSpriteSetting.at(7).create(glm::vec3(150, 360, 10), glm::vec2(300, 40), UI_VERTICAL_SLIDER, glm::vec2(0.f, 1.f), m_vecForSpriteOrigin.y);
    //m_SliderForSpriteSetting.at(7).setCallBack([](float value) {
    //    m_vecForSpriteOrigin.y = value;
    //    });

    //m_sprite = RESOURCE_MANAGER::getSprite("Attack1_1");

    //==================================================//

    

    return true;
}

 //(RUS) Обработка нажатий
//(ENG) Handling clicks
void MainGameClass::events(){ 
    size += MOUSE::getScroll().y/250;
    if (size < 0)size = 0;
    if (size > 1)size = 1;
   
    //m_testButton[0].checkClick();
    //m_testButton[1].checkClick();
    //m_testSwich.checkClick();
    m_testSliderVertical.checkClick();
    m_testSliderHorizontal.checkClick();
    m_testInputField.checkClick();
   // m_testList.checkClick();

   /*for (auto& currentSlider : m_SliderForSpriteSetting) {
        currentSlider.checkClick();
    }*/
}

 //(RUS) Установка матрицы проекции для отрисовки
//(ENG) Setting the projection matrix for rendering
void MainGameClass::setProjectionMat(const glm::ivec2& window){
    m_window = window;

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_window.x), 0.f, static_cast<float>(m_window.y), -100.f, 100.f);

    RESOURCE_MANAGER::setProjection(projectionMatrix);
    PRINT_TEXT::setWindow(m_window);
    Renderer::Sprite::setWindow(m_window);
    MOUSE::setWindowSize(m_window);
    m_testSliderVertical.update();
    m_testSliderHorizontal.update();
    m_testInputField.update();
    //m_testButton[0].update();
   // m_testButton[1].update();
   // m_testSwich.update();
   // m_testList.update();
}

 //(RUS) Уничтожение игровых объектов
//(ENG) Destruction of game objects
void MainGameClass::terminate(){
    m_vectorGameObject.clear();
}
