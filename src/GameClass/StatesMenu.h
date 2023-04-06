#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <rapidjson/document.h>
#include "../UI/UIElement.h"
#include "../Renderer/Sprite.h"
#include "StateController.h"
#include "../Renderer/PrintText.h"
#include "../Audio/SoundEffectsPlayer.h"
#include "../Renderer/RenderEngine.h"
#include "../Managers/ConfigManager.h"
#include "../UI/Translater.h"

class StatesMenu {
public:
	struct staticSprite{
		std::shared_ptr<Renderer::Sprite> ms_sprite;
		glm::vec3 ms_position;
		glm::vec2 ms_size;
		GLfloat ms_rotation;
		glm::vec2 ms_origin;

		staticSprite(std::shared_ptr<Renderer::Sprite> sprite, const glm::vec3& position, const glm::vec2& size,const GLfloat& rotation, const glm::vec2& origin)
			:ms_sprite(sprite), ms_position(position), ms_size(size),ms_rotation(rotation), ms_origin(origin) {}

		void render() {
			ms_sprite->renderUI(ms_position, ms_size, ms_rotation, ms_origin);
		}
	};
	struct AnimatedSprite {
		std::shared_ptr<StateController> ms_state;
		glm::vec3 ms_position;
		glm::vec2 ms_size;
		GLfloat ms_rotation;
		glm::vec2 ms_origin;

		AnimatedSprite(const std::string& state, const glm::vec3& position, const glm::vec2& size, const GLfloat& rotation, const glm::vec2& origin)
			:ms_position(position), ms_size(size), ms_rotation(rotation), ms_origin(origin) {
			ms_state = std::make_shared<StateController>(state);
		}

		void render() {
			ms_state->renderUI(ms_position, ms_size, ms_rotation, ms_origin);
		}

		void update(const double& duration) {
			ms_state->update(duration);
		}
	};
	struct TextForMenu {
		PRINT_TEXT::Text ms_text;
		float ms_sizeText;
		bool ms_centrText;
		std::shared_ptr<Renderer::Sprite> ms_spriteBackLine;
		float ms_shift;

		TextForMenu(const PRINT_TEXT::Text& text, float size, bool center, std::shared_ptr<Renderer::Sprite> sprite = nullptr, float shift = 0)
			:ms_text(text), ms_sizeText(size), ms_centrText(center), ms_spriteBackLine(sprite), ms_shift(shift) {}

		void render() {
			if (ms_spriteBackLine) {
				ms_spriteBackLine->renderUI(glm::vec3(ms_text.ms_position.x-ms_shift, ms_text.ms_position.y, ms_text.ms_position.z-0.1f), glm::vec2(ms_sizeText + ms_shift, ms_text.ms_scale), 0, glm::vec2(0, 0));
			}
			PRINT_TEXT::printTextReduction(ms_text, ms_sizeText, ms_centrText);
		}
	};

	void update(const double& duration);
	void updateButton();
	void render();  
	void events();   

	void init(const std::string& gameState);
	void clear();
private:
	void loadMusicBackground(const rapidjson::Document& JSONDoc);

	void loadStaticSpritesFromJSON(const rapidjson::Document& JSONDoc);

	void loadAnimatedSpriteFromJSON(const rapidjson::Document& JSONDoc);

	void loadTextForMenu(const rapidjson::Document& JSONDoc);

	void loadButtonFromJSON(const rapidjson::Document& JSONDoc);
	std::function<void()> getFunctionForButton(const std::string& nameFunction);

	void loadSliderFromJSON(const rapidjson::Document& JSONDoc);
	float getStartValue(const std::string& statrtValue);
	std::function<void(float value)> getFunctionForSlider(const std::string& nameFunction);

	void loadSwitchBoolFromJSON(const rapidjson::Document& JSONDoc);
	bool getStartBool(const std::string& statrtValue);
	std::function<void(bool flag)> getFunctionForSwitchBool(const std::string& nameFunction);

	void loadListParametrsFromJSON(const rapidjson::Document& JSONDoc);
	int getStartIndexElementInVector(const std::string& nameFunction);

	template<class T>
	std::vector<T> getList(const std::string& nameFunction);
	template<class T>
	std::function<void(T value)> getCallBack(const std::string& nameFunction);
	template<class T>
	std::function<std::string(T value)> getTypeToString(const std::string& nameFunction);

	std::shared_ptr<Audio::SoundEffectsPlayer> m_backgroundMusic;
	std::vector<TextForMenu> m_textVector;
	std::vector<std::shared_ptr<AnimatedSprite>> m_animationVector;
	std::vector<std::shared_ptr<staticSprite>> m_staticSpriteVector;
	std::vector<std::shared_ptr<UserInterface::UIElement>> m_UIElementVector;
};

template<class T>
inline std::vector<T> StatesMenu::getList(const std::string& nameFunction){return std::vector<T>();}
template<>
inline std::vector<int> StatesMenu::getList<int>(const std::string& nameFunction) {
	if (nameFunction == "samples") {
		return std::vector<int>({ 1,2,4,8,16 });
	}
	if (nameFunction == "display") {
		std::vector<int> vec;
		for (int i = 0; i < RENDER_ENGINE::getCountMonitor(); i++) {
			vec.push_back(i);
		}
		return vec;
	}
	return std::vector<int>();
}
template<>
inline std::vector<glm::ivec2> StatesMenu::getList<glm::ivec2>(const std::string& nameFunction) {
	if (nameFunction == "windowSize") {
		return RENDER_ENGINE::getScreenResolutions();
	}
	return std::vector<glm::ivec2>();
}
template<>
inline std::vector<std::string> StatesMenu::getList<std::string>(const std::string& nameFunction) {
	if (nameFunction == "language") {
		return std::vector<std::string>({"EN","RU"});
	}
	return std::vector<std::string>();
}

template<class T>
inline std::function<void(T value)> StatesMenu::getCallBack(const std::string& nameFunction){return std::function<void(T value)>();}
template<>
inline std::function<void(int value)> StatesMenu::getCallBack<int>(const std::string& nameFunction) {
	if (nameFunction == "samples") {
		return [](int value) {
			CONFIG_MANAGER::setSamples(value);
		};
	}
	if (nameFunction == "display") {
		return [](int value) {
			CONFIG_MANAGER::setDisplayNumber(value);
		};
	}
	return std::function<void(int value)>(); 
}
template<>
inline std::function<void(glm::ivec2 value)> StatesMenu::getCallBack<glm::ivec2>(const std::string& nameFunction) {
	if (nameFunction == "windowSize") {
		return [](glm::ivec2 value) {
			CONFIG_MANAGER::setWindowSize(value.x, value.y);
		};
	}
	return std::function<void(glm::ivec2 value)>();
}
template<>
inline std::function<void(std::string value)> StatesMenu::getCallBack<std::string>(const std::string& nameFunction) {
	if (nameFunction == "language") {
		return [](std::string value) {
			CONFIG_MANAGER::setLanguage(value);
		};
	}
	return std::function<void(std::string value)>();
}

template<class T>
inline std::function<std::string(T value)> StatesMenu::getTypeToString(const std::string& nameFunction){return std::function<std::string(T value)>();}
template<>
inline std::function<std::string(int value)> StatesMenu::getTypeToString<int>(const std::string& nameFunction) { 
	if (nameFunction == "samples") {
		return [](int value) {
			if (value == 1) {
				return TRANSLETE("menu", "no");
			}
			return "x" + std::to_string(value);
		};
	}
	if (nameFunction == "display") {
		return [](int value) {
			return std::to_string(value);
		};
	}
	return std::function<std::string(int value)>(); 
}
template<>
inline std::function<std::string(glm::ivec2 value)> StatesMenu::getTypeToString<glm::ivec2>(const std::string& nameFunction) {
	if (nameFunction == "windowSize") {
		return [](glm::ivec2 value) {
			return std::to_string(value.x) + "x" + std::to_string(value.y);
		};
	}
	return std::function<std::string(glm::ivec2 value)>();
}
template<>
inline std::function<std::string(std::string value)> StatesMenu::getTypeToString<std::string>(const std::string& nameFunction) { 
	if (nameFunction == "language") {
		return [](std::string value) {
			return value;
		};
	}
	return std::function<std::string(std::string value)>();
}
