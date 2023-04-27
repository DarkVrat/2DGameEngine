#include "ResourceManager.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <rapidjson/error/en.h>
#define  STB_IMAGE_IMPLEMENTATION
#define  STBI_ONLY_PNG
#include "stb_image.h"
#include "../Renderer/RenderEngine.h"
#include "../Renderer/PrintText.h"
#include "../UI/Translater.h"
#include "../Renderer/MapRender.h"

ResourceManager::StateAnimationMap ResourceManager::m_stateAnimation;
ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::SpriteMap ResourceManager::m_sprite;
ResourceManager::SoundMap ResourceManager::m_soundMap;
ResourceManager::SampleSourseMap ResourceManager::m_sampleSourseMap;
std::string ResourceManager::m_path;

 //(RUS) Установка полного пути до папки с игрой
//(ENG) Setting the full path to the game folder
void ResourceManager::setExecutablePath(const std::string& executablePath){
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

 //(RUS) Получение пути
//(ENG) Getting the path
std::string ResourceManager::getExecutablePath(){
	return m_path;
}

//(RUS) Получение строки всех данных из файла
//(ENG) Getting a string of all data from a file
std::string ResourceManager::getFileString(const std::string& relativeFilePath) {
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str());
	if (!f.is_open()) {
		std::cerr << "(!) Failed to open: " << relativeFilePath << std::endl;
		return std::string{};
	}
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

//(RUS) Загрузка JSON документа
//(ENG) Loading a JSON document
rapidjson::Document ResourceManager::loadJSONDoc(const std::string& JSONPath) {
	const std::string JSONString = getFileString(JSONPath);
	if (JSONString.empty()) {
		std::cerr << "(!) No JSON resources file" << std::endl;
		return nullptr;
	}
	rapidjson::Document JSONDoc;
	rapidjson::ParseResult parseResult = JSONDoc.Parse(JSONString.c_str());
	if (!parseResult) {
		std::cerr << "(!) JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << std::endl;
		std::cerr << "(!) in JSON resources file: " << JSONPath << std::endl;
		return nullptr;
	}
	return JSONDoc;
}

void ResourceManager::setProjection(const glm::mat4& projectionMatrix, const bool& camera) {
	for (auto& currentShader : m_shaderPrograms) {
		if (static_cast<bool>(currentShader.second->getSettings()>>1)!=camera) {
			currentShader.second->use();
			currentShader.second->setMatrix4("projection", projectionMatrix);
		}
	}
}

void ResourceManager::setView(const glm::vec3& view){
	for (auto& currentShader : m_shaderPrograms) {
		if (!(currentShader.second->getSettings()&0x02)) {
			currentShader.second->use();
			currentShader.second->setVec3("view", view);
		}
	}
}

 //(RUS) удаление всех ресурсов
//(ENG) deleting all resources
void ResourceManager::unloadAllRes(){
	m_shaderPrograms.clear();
	m_sprite.clear();
	m_textures.clear();
	m_soundMap.clear();
	m_stateAnimation.clear();
	m_sampleSourseMap.clear();
}

  //-------------------------------Shader------------------------------------//
 //(RUS) Загрузка шейдера
//(ENG) Loading a shader
void ResourceManager::loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexString = getFileString(vertexPath);
	std::string fragmentString = getFileString(fragmentPath);
	if (vertexString.empty()|| fragmentString.empty()) {
		std::cerr << "(!) Vertex or fragment is empty" << std::endl;  
		return;
	}

	std::shared_ptr<Renderer::ShaderProgram>&newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (!newShader->isCompiled()) { 
		std::cerr << "(!) Can't load shader" << std::endl;
	}
}

//(RUS) Получение shared_ptr на шейдер
//(ENG) Getting shared_ptr per shader
std::shared_ptr<Renderer::ShaderProgram>  ResourceManager::getShader(const std::string& shaderName) {
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end()) return it->second;
	std::cerr << "(!) Cant find the shader " << shaderName << std::endl;
	return nullptr;
}
//-------------------------------Shader------------------------------------//

  //-------------------------------Texture-----------------------------------//
 //(RUS) Загрузка текстуры
//(ENG) Loading a textures
void ResourceManager::loadOneTexture(const std::string& textureName, const std::string& texturePatn, const std::string& shader) {
	int channels, widht, height;
	stbi_set_flip_vertically_on_load(true);
	uint8_t* pixels = stbi_load(std::string(m_path + "/" + texturePatn).c_str(), &widht, &height, &channels, 0);
	if (!pixels) {
		std::cerr << "(!) ERROR TEXTUR LOAD" << textureName << std::endl;
		return;
	}

	m_textures.emplace(textureName, std::make_shared<Renderer::Texture2D>(widht, height, pixels, getShader(shader), channels, GL_NEAREST, GL_CLAMP_TO_EDGE));

	stbi_image_free(pixels);

	loadSprite(textureName, textureName, glm::ivec2(widht, height));
}

void ResourceManager::loadTextureCut(const std::string& textureName, const std::string& texturePatn, const std::string& shader, const int& heightSprite, const int& widthSprite, const std::vector<std::string>& sprites){
	loadOneTexture(textureName, texturePatn, shader);

	auto pTexture = getTexture(textureName);

	const unsigned int textureWidth = pTexture->getWidth();	
	const unsigned int textureHeight = pTexture->getHeight();
	unsigned int currentTexOffsetX = 0;
	unsigned int currentTexOffsetY = textureHeight;

	for (auto& currentSprite : sprites) {
		float left = (currentTexOffsetX + 0.01f) / textureWidth;
		float right = (currentTexOffsetX + widthSprite - 0.01f) / textureWidth;
		float bottom = (currentTexOffsetY - heightSprite + 0.01f) / textureHeight;
		float top = (currentTexOffsetY - 0.01f) / textureHeight;

		loadSprite(currentSprite, textureName, glm::ivec2(widthSprite, heightSprite), glm::vec4(left, bottom, right, top));

		currentTexOffsetX += widthSprite;
		if (currentTexOffsetX >= textureWidth) {
			currentTexOffsetX = 0;
			currentTexOffsetY -= heightSprite;
		}
	}
}

void ResourceManager::loadTextureAtlas(const std::string& textureName, const std::string& texturePatn, const std::string& shader, const std::map<std::string, glm::ivec4>& sprites){
	loadOneTexture(textureName, texturePatn, shader);

	auto pTexture = getTexture(textureName);

	const unsigned int textureWidth = pTexture->getWidth();
	const unsigned int textureHeight = pTexture->getHeight();

	for (auto& currentSprite : sprites) {
		glm::ivec2 aspectRatio = glm::ivec2(currentSprite.second.z, currentSprite.second.w);
		glm::vec4 coords = glm::vec4(	(currentSprite.second.x+0.01f)/textureWidth,
										(currentSprite.second.y+0.01f)/textureHeight,
										(currentSprite.second.x+aspectRatio.x-0.01f)/textureWidth,
										(currentSprite.second.y+aspectRatio.y-0.01f)/textureHeight);
		loadSprite(currentSprite.first, textureName, aspectRatio, coords);
	}
}

 //(RUS) Получение shared_ptr на текстуру
//(ENG) Getting shared_ptr per textures
std::shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(const std::string& textureName) {
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end()) return it->second;
	std::cerr << "(!) Cant find the textureName " << textureName << std::endl;
	return nullptr;
}
//-------------------------------Texture-----------------------------------//

  //-------------------------------Sprite------------------------------------//
 //(RUS) Загрузка спрайта
//(ENG) Loading a sprite
void ResourceManager::loadSprite(const std::string& spriteName, const std::string& textureName, const glm::ivec2& aspectRatio, const glm::vec4& coords){
	auto pTexture = getTexture(textureName);
	if (!pTexture) std::cerr << "(!) Cant find the textureName " << textureName << " for sprite " << spriteName << std::endl;
	
	m_sprite.emplace(spriteName,std::make_shared<Renderer::Sprite>(pTexture, coords, aspectRatio));
}

 //(RUS) Получение shared_ptr на спрайт
//(ENG) Getting shared_ptr per sprite
std::shared_ptr<Renderer::Sprite> ResourceManager::getSprite(const std::string& spriteName){
	SpriteMap::const_iterator it = m_sprite.find(spriteName);//поиск
	if (it != m_sprite.end()) return it->second;//получение
	std::cerr << "(!) Cant find the spriteName " << spriteName << std::endl;
	return nullptr;
}
//-------------------------------Sprite------------------------------------//


  //-------------------------------StateAnimation------------------------------------//
 //(RUS) Загрузка StateAnimation
//(ENG) Loading a StateAnimation
void ResourceManager::loadStateAnimation(const std::string& stateName, const  std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>>& frames, const std::vector<std::string>& sources, const std::string& nextState, const bool& uninterrupted) {
	m_stateAnimation.emplace(stateName, std::make_shared<Renderer::StateAnimation>(frames, sources,nextState, uninterrupted));
}

 //(RUS) Получение shared_ptr на StateAnimation
//(ENG) Getting shared_ptr per StateAnimation
std::shared_ptr<Renderer::StateAnimation> ResourceManager::getStateAnimation(const std::string& stateName) {
	StateAnimationMap::const_iterator it = m_stateAnimation.find(stateName);
	if (it != m_stateAnimation.end()) return it->second;
	std::cerr << "(!) Cant find the spriteName " << stateName << std::endl;
	return nullptr;
}
//-------------------------------StateAnimation------------------------------------//

  //-------------------------------Sound------------------------------------//
 //(RUS) Загрузка звука
//(ENG) Loading a sound
void ResourceManager::loadSound(const std::string& soundName, const std::string& soundPath){
	auto it = m_soundMap.find(soundName);
	if (it != m_soundMap.end()) {
		m_soundMap.erase(it);
	}
	m_soundMap.emplace(soundName, std::make_shared<Audio::FileOfSound>(soundPath));
}

 //(RUS) Получение shared_ptr на звук
//(ENG) Getting shared_ptr per sound
std::shared_ptr<Audio::FileOfSound> ResourceManager::getSound(const std::string& soundName){
	SoundMap::const_iterator it = m_soundMap.find(soundName);
	if (it != m_soundMap.end()) return it->second;
	std::cerr << "(!) Cant find the spriteName " << soundName << std::endl;
	return nullptr;
}

 //(RUS) Получение shared_ptr на образец источника звука
//(ENG) Getting shared_ptr per sound source sample
std::shared_ptr<Audio::SampleSourse> ResourceManager::getSampleSourse(const std::string& sampleName){
	SampleSourseMap::const_iterator it = m_sampleSourseMap.find(sampleName);
	if (it != m_sampleSourseMap.end()) return it->second;
	std::cerr << "(!) Cant find the spriteName " << sampleName << std::endl;
	return nullptr;
}
//-------------------------------Sound------------------------------------//

 //(RUS) Загрузка ресурсов из JSON файла
//(ENG) Loading resources from a JSON file
bool ResourceManager::loadJSONResurces() {

	std::thread audio([&]() {LoadAudioResurces("res/Audio/Audio.json"); });

	LoadTextureResurces("res/Textures/Textures.json");
	
	std::thread Animation([&]() {LoadAnimationResurces("res/Animation/Animation.json"); });
	
	LoadTextResurces("res/Text/Text.json");
	LoadMapResurces("res/Map/TextureMapConfig.json");

	audio.join();
	Animation.join();

	return true;
}

void ResourceManager::LoadAnimationResurces(const std::string& JSONPath) {
	rapidjson::Document JSONDoc = loadJSONDoc(JSONPath);

	auto stateAnimationIt = JSONDoc.FindMember("StateAnimation");
	if (stateAnimationIt != JSONDoc.MemberEnd()) {
		for (const auto& currentStateAnimation : stateAnimationIt->value.GetArray()) {
			const std::string nameState = currentStateAnimation["nameState"].GetString();
			const bool uninterrupted = currentStateAnimation["uninterrupted"].GetBool();
			const std::string nextState = currentStateAnimation["nextState"].GetString();

			const auto sourcesArray = currentStateAnimation["sources"].GetArray();
			std::vector<std::string> sourcesVector;
			sourcesVector.reserve(sourcesArray.Size());
			for (const auto& currentSource : sourcesArray) {
				sourcesVector.emplace_back(currentSource.GetString());
			}

			const auto framesArray = currentStateAnimation["frames"].GetArray();
			std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> framesVector;
			framesVector.reserve(framesArray.Size());
			for (const auto& currentFrame : framesArray) {
				framesVector.emplace_back(std::make_pair<std::shared_ptr<Renderer::Sprite>, double>(getSprite(currentFrame["sprite"].GetString()), currentFrame["duration"].GetDouble()));
			}

			loadStateAnimation(nameState, framesVector, sourcesVector, nextState, uninterrupted);
		}
	}
}

void ResourceManager::LoadAudioResurces(const std::string& JSONPath) {
	rapidjson::Document JSONDoc = loadJSONDoc(JSONPath);

	auto soundIt = JSONDoc.FindMember("sound");
	if (soundIt != JSONDoc.MemberEnd()) {
		for (const auto& currentSound : soundIt->value.GetArray()) {
			const std::string name = currentSound["name"].GetString();
			const std::string path = currentSound["path"].GetString();

			loadSound(name, path);
		}
	}

	auto sampleSourseIt = JSONDoc.FindMember("sampleSourse");
	if (sampleSourseIt != JSONDoc.MemberEnd()) {
		for (const auto& currentSample : sampleSourseIt->value.GetArray()) {

			Audio::SampleSourse sample = Audio::SampleSourse();
			sample.AlPitch = currentSample["AlPitch"].GetDouble();
			sample.AlRolloffFactor = currentSample["AlRolloffFactor"].GetDouble();
			sample.AlReferenceDistance = currentSample["AlReferenceDistance"].GetDouble();
			sample.AlMinGain = currentSample["AlMinGain"].GetDouble();
			sample.AlMaxGain = currentSample["AlMaxGain"].GetDouble();
			sample.AlGainOutCone = currentSample["AlGainOutCone"].GetDouble();
			sample.AlAngleInCone = currentSample["AlAngleInCone"].GetDouble();
			sample.AlAngleOutCone = currentSample["AlAngleOutCone"].GetDouble();

			if (currentSample["AlMaxDistance"].GetDouble() < 0.0) {
				sample.AlMaxDistance = std::numeric_limits<float>::max();
			}
			else {
				sample.AlMaxDistance = currentSample["AlMaxDistance"].GetDouble();
			}

			sample.GainString = currentSample["GainString"].GetString();

			const std::string name = currentSample["nameSample"].GetString();

			m_sampleSourseMap.emplace(name, std::make_shared<Audio::SampleSourse>(sample));
		}
	}
}

void ResourceManager::LoadTextResurces(const std::string& JSONPath) {
	rapidjson::Document JSONDoc = loadJSONDoc(JSONPath);

	std::string vertexString = JSONDoc.FindMember("textVertexShaderPath")->value.GetString();
	std::string fragmentString = JSONDoc.FindMember("textFragmentShaderPath")->value.GetString();
	std::string textureFontPath = JSONDoc.FindMember("textureFontPath")->value.GetString();
	std::string TTFPath = JSONDoc.FindMember("TTFPath")->value.GetString();
	std::string languagePath = JSONDoc.FindMember("languagePath")->value.GetString();

	loadShader("TextShader", vertexString, fragmentString);
	loadOneTexture("FontTexture", textureFontPath, "TextShader");
	Renderer::PrintText::init(TTFPath, getTexture("FontTexture"));
	Translater::init(loadJSONDoc(languagePath));
}

void ResourceManager::LoadTextureResurces(const std::string& JSONPath) {
	rapidjson::Document JSONDoc = loadJSONDoc(JSONPath);

	auto shadersIt = JSONDoc.FindMember("shaders");
	if (shadersIt != JSONDoc.MemberEnd()) {
		for (const auto& currentShader : shadersIt->value.GetArray()) {
			const std::string name = currentShader["name"].GetString();
			const std::string filePath_v = currentShader["filePath_v"].GetString();
			const std::string filePath_f = currentShader["filePath_f"].GetString();

			loadShader(name, filePath_v, filePath_f);
		}
	}

	auto OnetextureIt = JSONDoc.FindMember("oneTexture");
	if (OnetextureIt != JSONDoc.MemberEnd()) {
		for (const auto& currentTexture : OnetextureIt->value.GetArray()) {
			const std::string name = currentTexture["name"].GetString();
			const std::string filePath = currentTexture["filePath"].GetString();
			const std::string shader = currentTexture["shader"].GetString();

			loadOneTexture(name, filePath, shader);
		}
	}

	auto textureCutIt = JSONDoc.FindMember("textureAtlasesCut");
	if (textureCutIt != JSONDoc.MemberEnd()) {
		for (const auto& currentTexture : textureCutIt->value.GetArray()) {
			const std::string name = currentTexture["name"].GetString();
			const std::string filePath = currentTexture["filePath"].GetString();
			const std::string shader = currentTexture["shader"].GetString();
			const int heightSprite = currentTexture["heightSprite"].GetInt();
			const int widthSprite = currentTexture["widthSprite"].GetInt();
			std::vector<std::string> sprites;
			for (const auto& currentSprite : currentTexture["sprites"].GetArray()) {
				sprites.push_back(currentSprite.GetString());
			}

			loadTextureCut(name, filePath, shader, heightSprite, widthSprite, sprites);
		}
	}

	auto textureAtlasIt = JSONDoc.FindMember("textureAtlases");
	if (textureAtlasIt != JSONDoc.MemberEnd()) {
		for (const auto& currentTexture : textureAtlasIt->value.GetArray()) {
			const std::string name = currentTexture["name"].GetString();
			const std::string filePath = currentTexture["filePath"].GetString();
			const std::string shader = currentTexture["shader"].GetString();
			std::map<std::string, glm::ivec4> sprites;
			for (const auto& currentSprite : currentTexture["sprites"].GetArray()) {
				std::string name = currentSprite["name"].GetString();
				int left = currentSprite["left"].GetInt();
				int bottom = currentSprite["bottom"].GetInt();
				int width = currentSprite["width"].GetInt();
				int height = currentSprite["height"].GetInt();

				sprites.emplace(name, glm::vec4(left, bottom, width, height));
			}

			loadTextureAtlas(name, filePath, shader, sprites);
		}
	}

	RENDER_ENGINE::init(m_textures);
}

void ResourceManager::LoadMapResurces(const std::string& JSONPath){
	rapidjson::Document JSONDoc = loadJSONDoc(JSONPath);

	std::string mapVertexShaderPath = JSONDoc.FindMember("mapVertexShaderPath")->value.GetString();
	std::string mapFragmentShaderPath = JSONDoc.FindMember("mapFragmentShaderPath")->value.GetString();
	std::string texturePath = JSONDoc.FindMember("texturePath")->value.GetString();
	int mapCellSize = JSONDoc.FindMember("mapCellSize")->value.GetInt();

	loadShader("MapShader", mapVertexShaderPath, mapFragmentShaderPath);
	loadOneTexture("MapTexture", texturePath, "MapShader");

	MAP::init(getTexture("MapTexture"), mapCellSize);
	MAP::setLayer(0);
}
