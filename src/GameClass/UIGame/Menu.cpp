#include "Menu.h"

#include <rapidjson/error/en.h>
#include "../../Managers/ResourceManager.h"
#include "../../UI/Translater.h"
#include "../../UI/Button.h"
#include "../../UI/Slider.h"
#include "../../UI/SwitchBool.h"
#include "../../UI/ListParameter.h"
#include "../../Control/MouseControl.h"
#include "../../Managers/SoundManager.h"
#include "functions.h"
#include "../../UI/valuesControl.h"
#include "../../Managers/JSONtoType.h"

void Menu::update(const double& duration){
	for (auto& currentAnimation : m_animationVector) {
		currentAnimation->update(duration);
	}
	if (m_backgroundMusic->isStopped()) {
		m_backgroundMusic->play(); 
	}
}

void Menu::updateButton(){
	for (auto& currentUIElement : m_UIElementVector) {
		currentUIElement->update();
	}
} 

void Menu::render(){
	for (auto& currentSprite : m_staticSpriteVector) {
		currentSprite->renderUI();
	}
	for (auto& currentUIElement : m_UIElementVector) {
		currentUIElement->render();
	}
	for (auto& currentAnimation : m_animationVector) {
		currentAnimation->renderUI();
	}
	for (auto& currentText : m_textVector) {
		currentText->render();
	}
}

void Menu::events(){
	for (auto& currentUIElement : m_UIElementVector) {
		if (currentUIElement->checkClick()) {
			break;
		} 
	}
}

void Menu::init(const std::string& gameState){
	std::string jsonPath = "res/UI/" + gameState + ".json";
	
	clear();

	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(jsonPath);
	if (!JSONDoc.IsNull()) {
		if (m_backgroundMusic == nullptr) {
			m_backgroundMusic = JSONtoType::loadOneFromJSON<Audio::SoundEffectsPlayer>(jsonPath);
			m_backgroundMusic->play();
		}

		m_textVector = JSONtoType::loadFromJSONDoc<staticTextForUI>(jsonPath);
		m_staticSpriteVector = JSONtoType::loadFromJSONDoc<staticSprite>(jsonPath);
		m_animationVector = JSONtoType::loadFromJSONDoc<staticAnimatedSprite>(jsonPath);

		
		auto swichesBool = JSONtoType::loadFromJSONDoc<UserInterface::SwitchBool>(jsonPath);
		auto sliders = JSONtoType::loadFromJSONDoc<UserInterface::Slider>(jsonPath);
		auto listInt = JSONtoType::loadFromJSONDoc<UserInterface::ListParameter<int>>(jsonPath);
		auto listString = JSONtoType::loadFromJSONDoc<UserInterface::ListParameter<std::string>>(jsonPath);
		auto listIvec2 = JSONtoType::loadFromJSONDoc<UserInterface::ListParameter<glm::ivec2>>(jsonPath);
		auto buttons = JSONtoType::loadFromJSONDoc<UserInterface::Button>(jsonPath);
		m_UIElementVector.insert(m_UIElementVector.end(), swichesBool.begin(), swichesBool.end());
		m_UIElementVector.insert(m_UIElementVector.end(), sliders.begin(), sliders.end());
		m_UIElementVector.insert(m_UIElementVector.end(), listInt.begin(), listInt.end());
		m_UIElementVector.insert(m_UIElementVector.end(), listString.begin(), listString.end());
		m_UIElementVector.insert(m_UIElementVector.end(), listIvec2.begin(), listIvec2.end());
		m_UIElementVector.insert(m_UIElementVector.end(), buttons.begin(), buttons.end());
	}
}

void Menu::clear(){
	m_animationVector.clear();
	m_staticSpriteVector.clear();
	m_UIElementVector.clear();
	m_textVector.clear();
}

