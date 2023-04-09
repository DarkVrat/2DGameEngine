#pragma once

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include "../GameClass/UIGame/functions.h"
#include "../UI/Button.h"
#include "../GameClass/UIGame/staticSprite.h"
#include "../Managers/ResourceManager.h"
#include "../UI/Translater.h"
#include "../GameClass/UIGame/staticAnimatedSprite.h"
#include "../UI/ListParameter.h"

class JSONtoType {
public:
	template<class Type>
	static std::vector<std::shared_ptr<Type>> loadFromJSONDoc(const std::string& JSONpath);

	template<class Type>
	static std::shared_ptr<Type> loadOneFromJSON(const std::string& JSONpath);
};

//---------------------------------------------vector------------------------------------//
template<class Type>
inline std::vector<std::shared_ptr<Type>> JSONtoType::loadFromJSONDoc(const std::string& JSONpath){
	return std::vector<std::shared_ptr<Type>>();
}

//Type=UserInterface::Button
template<>
inline std::vector<std::shared_ptr<UserInterface::Button>> JSONtoType::loadFromJSONDoc(const std::string& JSONpath) {
	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(JSONpath);

	std::vector<std::shared_ptr<UserInterface::Button>> Buttons;

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

			auto button = std::make_shared<UserInterface::Button>(	position,
																	size,
																	TRANSLETE(subjectTranslete, textForTranslete),
																	scaleTextForButton,
																	color,
																	origin);
			button->setCallBack(Functions::getCallBack<void>(functionName));

			Buttons.push_back(button);
		}
	}
	return Buttons;
}

//Type=staticSprite
template<>
inline std::vector<std::shared_ptr<staticSprite>> JSONtoType::loadFromJSONDoc(const std::string& JSONpath) {
	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(JSONpath);

	std::vector<std::shared_ptr<staticSprite>> sprites;

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

			sprites.push_back(std::make_shared<staticSprite>(RESOURCE_MANAGER::getSprite(name), position, size, rotation, origin));
		}
	}
	return sprites;
}

//Type=staticAnimatedSprite
template<>
inline std::vector<std::shared_ptr<staticAnimatedSprite>> JSONtoType::loadFromJSONDoc(const std::string& JSONpath) {
	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(JSONpath);

	std::vector<std::shared_ptr<staticAnimatedSprite>> animatedSprite;

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

			animatedSprite.push_back(std::make_shared<staticAnimatedSprite>(name, position, size, rotation, origin));
		}
	}

	return animatedSprite;
}

//Type=staticTextForUI
template<>
inline std::vector<std::shared_ptr<staticTextForUI>> JSONtoType::loadFromJSONDoc(const std::string& JSONpath) {
	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(JSONpath);

	std::vector<std::shared_ptr<staticTextForUI>> textForUi;

	auto TextIT = JSONDoc.FindMember("TextForMenu");
	if (TextIT != JSONDoc.MemberEnd()) {
		for (const auto& currentText : TextIT->value.GetArray()) {
			const std::string subjectTranslete = currentText["subjectTranslete"].GetString();
			const std::string textForTranslete = currentText["textForTranslete"].GetString();

			glm::vec3 position = glm::vec3(0, 0, 0);
			position.x = currentText.FindMember("position")->value["x"].GetFloat();
			position.y = currentText.FindMember("position")->value["y"].GetFloat();
			position.z = currentText.FindMember("position")->value["z"].GetFloat();

			const float scaleText = currentText["scaleText"].GetFloat();

			glm::vec3 color = glm::vec3(0, 0, 0);
			color.r = currentText.FindMember("color")->value["r"].GetFloat();
			color.g = currentText.FindMember("color")->value["g"].GetFloat();
			color.b = currentText.FindMember("color")->value["b"].GetFloat();

			const float sizeByXScreen = currentText["sizeByXScreen"].GetFloat();

			const bool center = currentText["center"].GetBool();

			std::shared_ptr<Renderer::Sprite> backSprite = nullptr;
			float shift = 0;

			auto backSpriteIT = currentText.FindMember("sprite");
			if (backSpriteIT != currentText.MemberEnd()) {
				backSprite = RESOURCE_MANAGER::getSprite(currentText["sprite"].GetString());
				shift = currentText["shift"].GetFloat();
			}

			textForUi.push_back(std::make_shared<staticTextForUI>(TEXT(TRANSLETE(subjectTranslete, textForTranslete), position, scaleText, color), sizeByXScreen, center, backSprite, shift));
		}
	}

	return textForUi;
}

//Type=UserInterface::ListParameter<int>
template<>
inline std::vector<std::shared_ptr<UserInterface::ListParameter<int>>> JSONtoType::loadFromJSONDoc(const std::string& JSONpath) {
	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(JSONpath);

	std::vector<std::shared_ptr<UserInterface::ListParameter<int>>> ListInt;

	auto staticListIT = JSONDoc.FindMember("listParametrs");
	if (staticListIT != JSONDoc.MemberEnd()) {
		for (const auto& currentList : staticListIT->value.GetArray()) {
			const std::string type = currentList["type"].GetString();
			if (type != "int") {
				continue;
			}

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

			auto vec = valuesControl::getList<int>(functionName);
			auto list = std::make_shared<UserInterface::ListParameter<int>>(position,
																			size,
																			scaleText,
																			vec,
																			valuesControl::getStartValue<int>(functionName),
																			origin);
			list->setCallBack(Functions::getCallBack<void, int>(functionName));
			list->setTypeToString(valuesControl::getTypeToString<int>(functionName));
			ListInt.push_back(list);
			
		}
	}

	return ListInt;
}

//Type=UserInterface::ListParameter<glm::ivec2>
template<>
inline std::vector<std::shared_ptr<UserInterface::ListParameter<glm::ivec2>>> JSONtoType::loadFromJSONDoc(const std::string& JSONpath) {
	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(JSONpath);

	std::vector<std::shared_ptr<UserInterface::ListParameter<glm::ivec2>>> ListInt;

	auto staticListIT = JSONDoc.FindMember("listParametrs");
	if (staticListIT != JSONDoc.MemberEnd()) {
		for (const auto& currentList : staticListIT->value.GetArray()) {
			const std::string type = currentList["type"].GetString();
			if (type != "glm::ivec2") {
				continue;
			}

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

			auto vec = valuesControl::getList<glm::ivec2>(functionName);
			auto list = std::make_shared<UserInterface::ListParameter<glm::ivec2>>(	position,
																					size,
																					scaleText,
																					vec,
																					valuesControl::getStartValue<glm::ivec2>(functionName),
																					origin);
			list->setCallBack(Functions::getCallBack<void, glm::ivec2>(functionName));
			list->setTypeToString(valuesControl::getTypeToString<glm::ivec2>(functionName));
			ListInt.push_back(list);

		}
	}

	return ListInt;
}

//Type=UserInterface::ListParameter<std::string>
template<>
inline std::vector<std::shared_ptr<UserInterface::ListParameter<std::string>>> JSONtoType::loadFromJSONDoc(const std::string& JSONpath) {
	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(JSONpath);

	std::vector<std::shared_ptr<UserInterface::ListParameter<std::string>>> ListInt;

	auto staticListIT = JSONDoc.FindMember("listParametrs");
	if (staticListIT != JSONDoc.MemberEnd()) {
		for (const auto& currentList : staticListIT->value.GetArray()) {
			const std::string type = currentList["type"].GetString();
			if (type != "std::string") {
				continue;
			}

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

			auto vec = valuesControl::getList<std::string>(functionName);
			auto list = std::make_shared<UserInterface::ListParameter<std::string>>(position,
																					size,
																					scaleText,
																					vec,
																					valuesControl::getStartValue<std::string>(functionName),
																					origin);
			list->setCallBack(Functions::getCallBack<void, std::string>(functionName));
			list->setTypeToString(valuesControl::getTypeToString<std::string>(functionName));
			ListInt.push_back(list);
		}
	}

	return ListInt;
}

//Type=UserInterface::SwitchBool
template<>
inline std::vector<std::shared_ptr<UserInterface::SwitchBool>> JSONtoType::loadFromJSONDoc(const std::string& JSONpath) {
	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(JSONpath);

	std::vector<std::shared_ptr<UserInterface::SwitchBool>> switchBoolVec;

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

			auto switchBool = std::make_shared<UserInterface::SwitchBool>(	position,
																			size,
																			valuesControl::getStartValue<bool>(functionName),
																			origin);
			switchBool->setCallBack(Functions::getCallBack<void, bool>(functionName));

			switchBoolVec.push_back(switchBool);
		}
	}

	return switchBoolVec;
}

//Type=UserInterface::UserInterface::Slider
template<>
inline std::vector<std::shared_ptr<UserInterface::Slider>> JSONtoType::loadFromJSONDoc(const std::string& JSONpath) {
	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(JSONpath);

	std::vector<std::shared_ptr<UserInterface::Slider>> sliders;

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

			auto slider = std::make_shared<UserInterface::Slider>(position,
				size,
				view,
				minMax,
				valuesControl::getStartValue<float>(functionName),
				origin);
			slider->setCallBack(Functions::getCallBack<void, float>(functionName));

			sliders.push_back(slider);
		}
	}
	return sliders;
}
//---------------------------------------------vector------------------------------------//

//---------------------------------------------one---------------------------------------//
template<class Type>
inline std::shared_ptr<Type> JSONtoType::loadOneFromJSON(const std::string& JSONpath){
	return std::shared_ptr<Type>();
}

//Type=Audio::SoundEffectsPlayer
template<>
inline std::shared_ptr<Audio::SoundEffectsPlayer> JSONtoType::loadOneFromJSON(const std::string& JSONpath) {
	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(JSONpath);

	std::shared_ptr<Audio::SoundEffectsPlayer> backgroundMusic = nullptr;

	auto musicIT = JSONDoc.FindMember("soundEffect");
	if (musicIT != JSONDoc.MemberEnd()) {
		backgroundMusic = MAKE_SOUND_PLAYER(musicIT->value["nameSound"].GetString(), musicIT->value["nameSample"].GetString());
	}

	return backgroundMusic;
}
//---------------------------------------------one---------------------------------------// 
