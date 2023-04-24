#pragma once

class JSONtoType {
public:
	template<class Type>
	static std::vector<std::shared_ptr<Type>> loadFromJSONDoc(const std::string& JSONpath);

	template<class Type>
	static std::shared_ptr<Type> loadOneFromJSON(const std::string& JSONpath);

	static glm::vec3 extractPosition(const rapidjson::Value& From);
	static glm::vec3 extractColor(const rapidjson::Value& From);
	static glm::vec2 extractSize(const rapidjson::Value& From);
	static glm::vec2 extractOrigin(const rapidjson::Value& From);

private:
};

//---------------------------------------------------------------------------------------------//
glm::vec3 JSONtoType::extractPosition(const rapidjson::Value& From) {
	glm::vec3 position(0, 0, 0);
	position.x = From.FindMember("position")->value["x"].GetFloat();
	position.y = From.FindMember("position")->value["y"].GetFloat();
	position.z = From.FindMember("position")->value["z"].GetFloat();
	return position;
}

glm::vec3 JSONtoType::extractColor(const rapidjson::Value& From) {
	glm::vec3 color(0, 0, 0);
	color.r = From.FindMember("color")->value["r"].GetFloat();
	color.g = From.FindMember("color")->value["g"].GetFloat();
	color.b = From.FindMember("color")->value["b"].GetFloat();
	return color;
}

glm::vec2 JSONtoType::extractSize(const rapidjson::Value& From) {
	glm::vec2 size(0, 0);
	size.x = From.FindMember("size")->value["x"].GetFloat();
	size.y = From.FindMember("size")->value["y"].GetFloat();
	return size;
}

glm::vec2 JSONtoType::extractOrigin(const rapidjson::Value& From) {
	glm::vec2 origin(0, 0);
	origin.x = From.FindMember("origin")->value["x"].GetFloat();
	origin.y = From.FindMember("origin")->value["y"].GetFloat();
	return origin;
}
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
			const std::string subjectTranslete = currentButton["subjectTranslete"].GetString();
			const std::string textForTranslete = currentButton["textForTranslete"].GetString();
			const GLfloat scaleTextForButton = currentButton["scaleTextForButton"].GetFloat();
			const glm::vec3 position = extractPosition(currentButton);
			const glm::vec3 color = extractColor(currentButton);
			const glm::vec2 size = extractSize(currentButton);
			const glm::vec2 origin = extractOrigin(currentButton);

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
			const GLfloat rotation = currentStaticSprites["rotation"].GetFloat();
			const glm::vec3 position = extractPosition(currentStaticSprites);
			const glm::vec2 size = extractSize(currentStaticSprites);
			const glm::vec2 origin = extractOrigin(currentStaticSprites);

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
			const GLfloat rotation = currentAnimetedSprites["rotation"].GetFloat();
			const glm::vec3 position = extractPosition(currentAnimetedSprites);
			const glm::vec2 size = extractSize(currentAnimetedSprites);
			const glm::vec2 origin = extractOrigin(currentAnimetedSprites);

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
			const float scaleText = currentText["scaleText"].GetFloat();
			const float sizeByXScreen = currentText["sizeByXScreen"].GetFloat();
			const bool center = currentText["center"].GetBool();
			const glm::vec3 position = extractPosition(currentText);
			const glm::vec3 color = extractColor(currentText);

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
			const std::string startValue = currentList["startValue"].GetString();
			const glm::vec3 position = extractPosition(currentList);
			const glm::vec2 size = extractSize(currentList);
			const glm::vec2 origin = extractOrigin(currentList);
			const GLfloat scaleText = currentList["scaleText"].GetFloat();

			auto vec = valuesControl::getList<int>(startValue);
			auto list = std::make_shared<UserInterface::ListParameter<int>>(position,
																			size,
																			scaleText,
																			vec,
																			valuesControl::getStartValue<int>(startValue),
																			origin);
			list->setCallBack(Functions::getCallBack<void, int>(functionName));
			list->setTypeToString(valuesControl::getTypeToString<int>(startValue));
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
			const std::string startValue = currentList["startValue"].GetString();
			const glm::vec3 position = extractPosition(currentList);
			const glm::vec2 size = extractSize(currentList);
			const glm::vec2 origin = extractOrigin(currentList);
			const GLfloat scaleText = currentList["scaleText"].GetFloat();

			auto vec = valuesControl::getList<glm::ivec2>(startValue);
			auto list = std::make_shared<UserInterface::ListParameter<glm::ivec2>>(	position,
																					size,
																					scaleText,
																					vec,
																					valuesControl::getStartValue<glm::ivec2>(startValue),
																					origin);
			list->setCallBack(Functions::getCallBack<void, glm::ivec2>(functionName));
			list->setTypeToString(valuesControl::getTypeToString<glm::ivec2>(startValue));
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
			const std::string startValue = currentList["startValue"].GetString();
			const glm::vec3 position = extractPosition(currentList);
			const glm::vec2 size = extractSize(currentList);
			const glm::vec2 origin = extractOrigin(currentList);
			const GLfloat scaleText = currentList["scaleText"].GetFloat();

			auto vec = valuesControl::getList<std::string>(startValue);
			auto list = std::make_shared<UserInterface::ListParameter<std::string>>(position,
																					size,
																					scaleText,
																					vec,
																					valuesControl::getStartValue<std::string>(startValue),
																					origin);
			list->setCallBack(Functions::getCallBack<void, std::string>(functionName));
			list->setTypeToString(valuesControl::getTypeToString<std::string>(startValue));
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
			const std::string startValue = currentSwichBool["startValue"].GetString();
			const glm::vec3 position = extractPosition(currentSwichBool);
			const glm::vec2 size = extractSize(currentSwichBool);
			const glm::vec2 origin = extractOrigin(currentSwichBool);

			auto switchBool = std::make_shared<UserInterface::SwitchBool>(	position,
																			size,
																			valuesControl::getStartValue<bool>(startValue),
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
			const std::string startValue = currentSlider["startValue"].GetString();
			const glm::vec3 position = extractPosition(currentSlider);
			const glm::vec2 size = extractSize(currentSlider);
			const glm::vec2 origin = extractOrigin(currentSlider);
			const bool view = currentSlider["view"].GetBool();

			glm::vec2 minMax = glm::vec2(0, 0);
			minMax.x = currentSlider.FindMember("minMax")->value["x"].GetFloat();
			minMax.y = currentSlider.FindMember("minMax")->value["y"].GetFloat();

			auto slider = std::make_shared<UserInterface::Slider>(	position,
																	size,
																	view,
																	minMax,
																	valuesControl::getStartValue<float>(startValue),
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
 