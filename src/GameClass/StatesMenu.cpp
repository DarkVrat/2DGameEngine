#include "StatesMenu.h"

#include <rapidjson/error/en.h>
#include "../Managers/ResourceManager.h"
#include "../UI/Translater.h"
#include "../UI/Button.h"
#include "../UI/Slider.h"
#include "../UI/SwitchBool.h"
#include "../UI/ListParameter.h"
#include "../Control/MouseControl.h"
#include "../Managers/SoundManager.h"

void StatesMenu::update(const double& duration){
	for (auto& currentAnimation : m_animationVector) {
		currentAnimation->update(duration);
	}
	if (m_backgroundMusic->isStopped()) {
		m_backgroundMusic->play(); 
	}
}

void StatesMenu::updateButton(){
	for (auto& currentUIElement : m_UIElementVector) {
		currentUIElement->update();
	}
} 

void StatesMenu::render(){
	for (auto& currentSprite : m_staticSpriteVector) {
		currentSprite->render();
	}
	for (auto& currentUIElement : m_UIElementVector) {
		currentUIElement->render();
	}
	for (auto& currentAnimation : m_animationVector) {
		currentAnimation->render();
	}
	for (auto& currentText : m_textVector) {
		currentText.render();
	}
}

void StatesMenu::events(){
	for (auto& currentUIElement : m_UIElementVector) {
		if (currentUIElement->checkClick()) {
			break;
		} 
	}
}

void StatesMenu::init(const std::string& gameState){ 
	clear();

	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc("res/UI/"+gameState+".json");
	if (!JSONDoc.IsNull()) {
		if (m_backgroundMusic == nullptr&&gameState=="MainMenu") {
			loadMusicBackground(JSONDoc);
			m_backgroundMusic->play();
			loadAnimatedSpriteFromJSON(JSONDoc);
		}
		loadStaticSpritesFromJSON(JSONDoc);
		loadButtonFromJSON(JSONDoc);
		loadTextForMenu(JSONDoc);
		loadSliderFromJSON(JSONDoc);
		loadSwitchBoolFromJSON(JSONDoc);
		loadListParametrsFromJSON(JSONDoc); 
	}
}

void StatesMenu::clear(){
	m_staticSpriteVector.clear();
	m_UIElementVector.clear();
	m_textVector.clear();
}

void StatesMenu::loadMusicBackground(const rapidjson::Document& JSONDoc){
	auto musicIT = JSONDoc.FindMember("musicBackGround");
	if (musicIT != JSONDoc.MemberEnd()) {
		m_backgroundMusic = MAKE_SOUND_PLAYER(musicIT->value["nameMusic"].GetString(), musicIT->value["nameSample"].GetString());
	}
}

void StatesMenu::loadStaticSpritesFromJSON(const rapidjson::Document& JSONDoc){
	auto staticSpriteIT = JSONDoc.FindMember("staticSprites");
	if (staticSpriteIT != JSONDoc.MemberEnd()) {
		for (const auto& currentStaticSprites : staticSpriteIT->value.GetArray()) {
			const std::string name = currentStaticSprites["spriteName"].GetString();
			 
			glm::vec3 position = glm::vec3(0, 0, 0);
			position.x = currentStaticSprites.FindMember("position")->value["x"].GetFloat();
			position.y = currentStaticSprites.FindMember("position")->value["y"].GetFloat();
			position.z = currentStaticSprites.FindMember("position")->value["z"].GetFloat();

			glm::vec2 size = glm::vec2(0, 0);
			size.x = currentStaticSprites.FindMember("size")->value["x"].GetFloat();
			size.y = currentStaticSprites.FindMember("size")->value["y"].GetFloat();

			const GLfloat rotation = currentStaticSprites["rotation"].GetFloat();

			glm::vec2 origin = glm::vec2(0, 0);
			origin.x = currentStaticSprites.FindMember("origin")->value["x"].GetFloat();
			origin.y = currentStaticSprites.FindMember("origin")->value["y"].GetFloat();

			m_staticSpriteVector.push_back(std::make_shared<staticSprite>(RESOURCE_MANAGER::getSprite(name), position, size, rotation, origin));
		}
	}
}

void StatesMenu::loadAnimatedSpriteFromJSON(const rapidjson::Document& JSONDoc){
	auto animatedSpriteIT = JSONDoc.FindMember("animatedSprite");
	if (animatedSpriteIT != JSONDoc.MemberEnd()) {
		for (const auto& currentAnimetedSprites : animatedSpriteIT->value.GetArray()) {
			const std::string name = currentAnimetedSprites["StateName"].GetString();

			glm::vec3 position = glm::vec3(0, 0, 0);
			position.x = currentAnimetedSprites.FindMember("position")->value["x"].GetFloat();
			position.y = currentAnimetedSprites.FindMember("position")->value["y"].GetFloat();
			position.z = currentAnimetedSprites.FindMember("position")->value["z"].GetFloat();

			glm::vec2 size = glm::vec2(0, 0);
			size.x = currentAnimetedSprites.FindMember("size")->value["x"].GetFloat();
			size.y = currentAnimetedSprites.FindMember("size")->value["y"].GetFloat();

			const GLfloat rotation = currentAnimetedSprites["rotation"].GetFloat();

			glm::vec2 origin = glm::vec2(0, 0);
			origin.x = currentAnimetedSprites.FindMember("origin")->value["x"].GetFloat();
			origin.y = currentAnimetedSprites.FindMember("origin")->value["y"].GetFloat();
			 
			m_animationVector.push_back(std::make_shared<AnimatedSprite>(name, position, size, rotation, origin));
		}
	}
}

void StatesMenu::loadTextForMenu(const rapidjson::Document& JSONDoc){ 
	auto TextIT = JSONDoc.FindMember("TextForMenu");
	if (TextIT != JSONDoc.MemberEnd()) {
		for (const auto& currentText : TextIT->value.GetArray()) {
			const std::string subjectTranslete = currentText["subjectTranslete"].GetString();
			const std::string textForTranslete = currentText["textForTranslete"].GetString();

			glm::vec3 position = glm::vec3(0, 0, 0);
			position.x = currentText.FindMember("position")->value["x"].GetFloat();
			position.y = currentText.FindMember("position")->value["y"].GetFloat();
			position.z = currentText.FindMember("position")->value["z"].GetFloat();

			const float scaleText= currentText["scaleText"].GetFloat();

			glm::vec3 color = glm::vec3(0, 0, 0);
			color.r = currentText.FindMember("color")->value["r"].GetFloat();
			color.g = currentText.FindMember("color")->value["g"].GetFloat();
			color.b = currentText.FindMember("color")->value["b"].GetFloat();

			const float sizeByXScreen = currentText["sizeByXScreen"].GetFloat();

			const bool center = currentText["center"].GetBool();

			std::shared_ptr<Renderer::Sprite> backSprite=nullptr;
			float shift = 0;

			auto backSpriteIT = currentText.FindMember("sprite");
			if (backSpriteIT != currentText.MemberEnd()) {
				backSprite = RESOURCE_MANAGER::getSprite(currentText["sprite"].GetString());  
				shift = currentText["shift"].GetFloat();
			}
		
			TextForMenu text(PRINT_TEXT::Text(TRANSLETE(subjectTranslete, textForTranslete), position, scaleText, color), sizeByXScreen, center, backSprite, shift);

			m_textVector.push_back(text);
		}
	}
}

void StatesMenu::loadButtonFromJSON(const rapidjson::Document& JSONDoc){
	auto staticButtonsIT = JSONDoc.FindMember("buttons");
	if (staticButtonsIT != JSONDoc.MemberEnd()) {
		for (const auto& currentButton : staticButtonsIT->value.GetArray()) {
			const std::string functionName = currentButton["function"].GetString();

			glm::vec3 position = glm::vec3(0, 0, 0);
			position.x = currentButton.FindMember("position")->value["x"].GetFloat();
			position.y = currentButton.FindMember("position")->value["y"].GetFloat();
			position.z = currentButton.FindMember("position")->value["z"].GetFloat();

			glm::vec2 size = glm::vec2(0, 0);
			size.x = currentButton.FindMember("size")->value["x"].GetFloat();
			size.y = currentButton.FindMember("size")->value["y"].GetFloat();

			const std::string subjectTranslete = currentButton["subjectTranslete"].GetString();
			const std::string textForTranslete = currentButton["textForTranslete"].GetString();
			const GLfloat scaleTextForButton = currentButton["scaleTextForButton"].GetFloat();

			glm::vec3 color = glm::vec3(0, 0, 0);
			color.r = currentButton.FindMember("color")->value["r"].GetFloat();
			color.g = currentButton.FindMember("color")->value["g"].GetFloat();
			color.b = currentButton.FindMember("color")->value["b"].GetFloat();

			glm::vec2 origin = glm::vec2(0, 0);
			origin.x = currentButton.FindMember("origin")->value["x"].GetFloat();
			origin.y = currentButton.FindMember("origin")->value["y"].GetFloat();

			std::shared_ptr<UserInterface::Button> button = std::make_shared<UserInterface::Button>(position,
																									size,
																									TRANSLETE(subjectTranslete, textForTranslete),
																									scaleTextForButton,
																									color,
																									origin);
			button->setCallBack(getFunctionForButton(functionName));

			m_UIElementVector.push_back(button);
		}
	}
}

std::function<void()> StatesMenu::getFunctionForButton(const std::string& nameFunction){
	if (nameFunction == "SettingFunction") {
		return [&]() {
			init("Settings");
		};
	}
	if (nameFunction == "NewGameFunction") {
		return [&]() {
			init("NewGame");
		};
	}
	if (nameFunction == "ExitFunction") {
		return [&]() {
			init("Exit");
		};
	}
	if (nameFunction == "ExitYes") {
		return [&]() {
			RENDER_ENGINE::closeWindow();
		};
	}
	if (nameFunction == "ExitNo") {
		return [&]() {
			init("MainMenu");
		};
	}
	if (nameFunction == "SaveConfig") {
		return [&]() {
			CONFIG_MANAGER::saveConfig();
			RENDER_ENGINE::applySettings();
			init("MainMenu");
		};
	}
	if (nameFunction == "audioRestart") {
		return [&]() {
			SOUND_MANAGER::updateSoundSystem();
			m_backgroundMusic->play();
		};
	}
	return []() {};
}

void StatesMenu::loadSliderFromJSON(const rapidjson::Document& JSONDoc){
	auto staticSlidersIT = JSONDoc.FindMember("sliders");
	if (staticSlidersIT != JSONDoc.MemberEnd()) {
		for (const auto& currentSlider : staticSlidersIT->value.GetArray()) {
			const std::string functionName = currentSlider["function"].GetString();

			glm::vec3 position = glm::vec3(0, 0, 0);
			position.x = currentSlider.FindMember("position")->value["x"].GetFloat();
			position.y = currentSlider.FindMember("position")->value["y"].GetFloat();
			position.z = currentSlider.FindMember("position")->value["z"].GetFloat();

			glm::vec2 size = glm::vec2(0, 0);
			size.x = currentSlider.FindMember("size")->value["x"].GetFloat();
			size.y = currentSlider.FindMember("size")->value["y"].GetFloat();

			const bool view = currentSlider["view"].GetBool();

			glm::vec2 minMax = glm::vec2(0, 0);
			minMax.x = currentSlider.FindMember("minMax")->value["x"].GetFloat();
			minMax.y = currentSlider.FindMember("minMax")->value["y"].GetFloat();

			glm::vec2 origin = glm::vec2(0, 0);
			origin.x = currentSlider.FindMember("origin")->value["x"].GetFloat();
			origin.y = currentSlider.FindMember("origin")->value["y"].GetFloat();

			std::shared_ptr<UserInterface::Slider> slider = std::make_shared<UserInterface::Slider>(position, 
																									size, 
																									view, 
																									minMax, 
																									getStartValue(functionName), 
																									origin);
			slider->setCallBack(getFunctionForSlider(functionName));

			m_UIElementVector.push_back(slider);
		}
	}
}

float StatesMenu::getStartValue(const std::string& statrtValue){
	if (statrtValue == "masterVolume") {
		return CONFIG_MANAGER::getVolumeSounde();
	} 
	if (statrtValue == "UIVolume") {
		return CONFIG_MANAGER::getVolumeSample("UI");
	}
	if (statrtValue == "MusicVolume") {
		return CONFIG_MANAGER::getVolumeSample("Music");
	}
	return 0.0f;
}

std::function<void(float value)> StatesMenu::getFunctionForSlider(const std::string& nameFunction){
	if (nameFunction == "masterVolume") {
		return [](float value) {
			CONFIG_MANAGER::setVolumeSounde(value); 
		};
	} 
	if (nameFunction == "UIVolume") {
		return [](float value) {
			CONFIG_MANAGER::setVolumeSample("UI", value);
		}; 
	}
	if (nameFunction == "MusicVolume") {
		return [](float value) {
			CONFIG_MANAGER::setVolumeSample("Music", value);
		};
	}
	return std::function<void(float value)>();
}

void StatesMenu::loadSwitchBoolFromJSON(const rapidjson::Document& JSONDoc){
	auto staticSwitchBoolIT = JSONDoc.FindMember("switchBool");
	if (staticSwitchBoolIT != JSONDoc.MemberEnd()) {
		for (const auto& currentSwichBool : staticSwitchBoolIT->value.GetArray()) {
			const std::string functionName = currentSwichBool["function"].GetString();

			glm::vec3 position = glm::vec3(0, 0, 0);
			position.x = currentSwichBool.FindMember("position")->value["x"].GetFloat();
			position.y = currentSwichBool.FindMember("position")->value["y"].GetFloat();
			position.z = currentSwichBool.FindMember("position")->value["z"].GetFloat();

			glm::vec2 size = glm::vec2(0, 0);
			size.x = currentSwichBool.FindMember("size")->value["x"].GetFloat();
			size.y = currentSwichBool.FindMember("size")->value["y"].GetFloat();

			glm::vec2 origin = glm::vec2(0, 0);
			origin.x = currentSwichBool.FindMember("origin")->value["x"].GetFloat();
			origin.y = currentSwichBool.FindMember("origin")->value["y"].GetFloat();

			std::shared_ptr<UserInterface::SwitchBool> switchBool = std::make_shared<UserInterface::SwitchBool>(position,
																												size,
																												getStartBool(functionName),
																												origin);
			switchBool->setCallBack(getFunctionForSwitchBool(functionName));

			m_UIElementVector.push_back(switchBool);
		}
	}
}

bool StatesMenu::getStartBool(const std::string& statrtValue){
	if (statrtValue == "fullScreen") {
		return CONFIG_MANAGER::getFullScreen();
	}
	if (statrtValue == "VSync") {
		return CONFIG_MANAGER::getVSync();
	}
	return false;
}

std::function<void(bool flag)> StatesMenu::getFunctionForSwitchBool(const std::string& nameFunction){
	if (nameFunction == "fullScreen") {
		return [](bool flag) {
			CONFIG_MANAGER::setFullScreen(flag);
		};
	} 
	if (nameFunction == "VSync") {
		return [](bool flag) {
			CONFIG_MANAGER::setVSync(flag);
		};
	}
	return std::function<void(bool flag)>();
}

void StatesMenu::loadListParametrsFromJSON(const rapidjson::Document& JSONDoc){ 
	auto staticListIT = JSONDoc.FindMember("listParametrs");
	if (staticListIT != JSONDoc.MemberEnd()) {
		for (const auto& currentList : staticListIT->value.GetArray()) {
			const std::string type = currentList["type"].GetString();
			const std::string functionName = currentList["function"].GetString();

			glm::vec3 position = glm::vec3(0, 0, 0);
			position.x = currentList.FindMember("position")->value["x"].GetFloat();
			position.y = currentList.FindMember("position")->value["y"].GetFloat();
			position.z = currentList.FindMember("position")->value["z"].GetFloat();

			glm::vec2 size = glm::vec2(0, 0);
			size.x = currentList.FindMember("size")->value["x"].GetFloat();
			size.y = currentList.FindMember("size")->value["y"].GetFloat();

			const GLfloat scaleText = currentList["scaleText"].GetFloat();

			glm::vec2 origin = glm::vec2(0, 0);
			origin.x = currentList.FindMember("origin")->value["x"].GetFloat();
			origin.y = currentList.FindMember("origin")->value["y"].GetFloat();

			if (type == "glm::ivec2") {
				std::vector<glm::ivec2> vec = getList<glm::ivec2>(functionName);
				std::shared_ptr<UserInterface::ListParameter<glm::ivec2>> list = std::make_shared<UserInterface::ListParameter<glm::ivec2>>(position,
																																			size,
																																			scaleText,
																																			vec,
																																			getStartIndexElementInVector(functionName),
																																			origin);
				list->setCallBack(getCallBack<glm::ivec2>(functionName));
				list->setTypeToString(getTypeToString<glm::ivec2>(functionName));
				m_UIElementVector.push_back(list);
			}
			if (type == "int") {
				std::vector<int> vec = getList<int>(functionName);
				std::shared_ptr<UserInterface::ListParameter<int>> list = std::make_shared<UserInterface::ListParameter<int>>(	position,
																																size,
																																scaleText,
																																vec,
																																getStartIndexElementInVector(functionName),
																																origin);
				list->setCallBack(getCallBack<int>(functionName));
				list->setTypeToString(getTypeToString<int>(functionName));
				m_UIElementVector.push_back(list);
			}  
			if (type == "std::string") {
				std::vector<std::string> vec = getList<std::string>(functionName);
				std::shared_ptr<UserInterface::ListParameter<std::string>> list = std::make_shared<UserInterface::ListParameter<std::string>>(	position,
																																				size,
																																				scaleText,
																																				vec,
																																				getStartIndexElementInVector(functionName),
																																				origin);
				list->setCallBack(getCallBack<std::string>(functionName));
				list->setTypeToString(getTypeToString<std::string>(functionName));
				m_UIElementVector.push_back(list);
			}
		}
	}
}

int StatesMenu::getStartIndexElementInVector(const std::string& nameFunction){
	if (nameFunction == "samples") {
		int value = CONFIG_MANAGER::getSamples();
		std::vector<int> vec = { 1,2,4,8,16 };
		for (int i = 0; i < vec.size(); i++) {
			if (vec[i] == value) {
				return i;
			}
		}
	}
	if (nameFunction == "display") {
		return CONFIG_MANAGER::getDisplayNumber();
	}
	if (nameFunction == "windowSize") {
		glm::ivec2 value = CONFIG_MANAGER::getWindowSize();
		std::vector<glm::ivec2> vec = RENDER_ENGINE::getScreenResolutions();
		for (int i = 0; i < vec.size(); i++) {
			if (vec[i] == value) {
				return i;
			}
		}
	}
	if (nameFunction == "language") {
		std::string value = CONFIG_MANAGER::getLanguage();
		std::vector<std::string> vec = { "EN","RU" };
		for (int i = 0; i < vec.size(); i++) {
			if (vec[i] == value) {
				return i;
			}
		}
	}
	return 0;
}
