#include "ResourceManager.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <rapidjson/error/en.h>
#define  STB_IMAGE_IMPLEMENTATION
#define  STBI_ONLY_PNG
#include "stb_image.h"
#include "../Renderer/RenderEngine.h"
#include "../Renderer/PrintText.h"

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

 //(RUS) Получение строки всех данных из файла
//(ENG) Getting a string of all data from a file
std::string ResourceManager::getFileString(const std::string& relativeFilePath)  {
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
void ResourceManager::loadTexture(std::string textureName, std::string texturePatn, std::vector<std::string> subTextures, const unsigned subWidth, const unsigned subHeigth) {
	int channels, widht, height;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels=stbi_load(std::string(m_path + "/" + texturePatn).c_str(), &widht, &height, &channels, 0);
	if (!pixels) { 
		std::cerr << "(!) ERROR TEXTUR LOAD" << textureName <<std::endl;
		return ;
	}

	std::shared_ptr<Renderer::Texture2D> newTexture = std::make_shared<Renderer::Texture2D>(widht, height,pixels, channels,GL_NEAREST,GL_CLAMP_TO_EDGE);
	stbi_image_free(pixels);

	if (newTexture) {
		const unsigned int textureWidth = newTexture->getWidth();	
		const unsigned int textureHeight = newTexture->getHeight(); 
		unsigned int currentTexOffsetX = 0;
		unsigned int currentTexOffsetY = textureHeight;

		for (auto& currentSubTexName : subTextures) {
			glm::vec2 leftBottomUV(static_cast<float>(currentTexOffsetX + 0.01f) / textureWidth, static_cast<float>(currentTexOffsetY - subHeigth + 0.01f) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTexOffsetX + subHeigth - 0.01f) / textureWidth, static_cast<float>(currentTexOffsetY - 0.01f) / textureHeight);

			newTexture->addSubTexture(std::move(currentSubTexName), leftBottomUV, rightTopUV);

			currentTexOffsetX += subWidth; 

			if (currentTexOffsetX >= textureWidth) {
				currentTexOffsetX = 0;
				currentTexOffsetY -= subHeigth;
			}
		}
	}

	m_textures.emplace(textureName, newTexture);
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
void ResourceManager::loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName,const std::string& subTextureName){
	auto pTexture = getTexture(textureName);
	if (!pTexture) std::cerr << "(!) Cant find the textureName " << textureName << " for sprite " << spriteName << std::endl;
	
	auto pShader = getShader(shaderName);
	if (!pShader) std::cerr << "(!) Cant find the shaderName " << shaderName << " for sprite " << spriteName << std::endl;

	m_sprite.emplace(spriteName,std::make_shared<Renderer::Sprite>(pTexture,subTextureName,pShader));
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
void ResourceManager::loadStateAnimation(const std::string& stateName, std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> frames, std::vector<std::string> sources, std::string nextState, bool uninterrupted) {
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

 //(RUS) Загрузка JSON документа
//(ENG) Loading a JSON document
rapidjson::Document ResourceManager::loadJSONDoc(const std::string& JSONPath){
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

 //(RUS) Загрузка ресурсов из JSON файла
//(ENG) Loading resources from a JSON file
bool ResourceManager::loadJSONResurces(const std::string& JSONPath) {
	const rapidjson::Document JSONDoc = loadJSONDoc(JSONPath);

	auto shadersIt = JSONDoc.FindMember("shader");
	if (shadersIt != JSONDoc.MemberEnd()) {
		for (const auto& currentShader : shadersIt->value.GetArray()) {
			const std::string name = currentShader["name"].GetString();
			const std::string filePath_v = currentShader["filePath_v"].GetString();
			const std::string filePath_f = currentShader["filePath_f"].GetString();

			loadShader(name, filePath_v, filePath_f);
		}
	}

	auto textureAtlasesIt = JSONDoc.FindMember("textureAtlases");
	if (textureAtlasesIt != JSONDoc.MemberEnd()) {
		for (const auto& currentTextureAtlases : textureAtlasesIt->value.GetArray()) {
			const std::string name = currentTextureAtlases["name"].GetString();
			const std::string filePath = currentTextureAtlases["filePath"].GetString();
			const unsigned int subTextureWidth = currentTextureAtlases["subTextureWidth"].GetUint();
			const unsigned int subTextureHeight = currentTextureAtlases["subTextureHeight"].GetUint();

			const auto subTexturesArray = currentTextureAtlases["subTextures"].GetArray();
			std::vector<std::string> subTexturesVector;
			subTexturesVector.reserve(subTexturesArray.Size());
			for (const auto& currentSubTexture : subTexturesArray) {
				subTexturesVector.emplace_back(currentSubTexture.GetString());
			}

			loadTexture(name, filePath, std::move(subTexturesVector), subTextureWidth, subTextureHeight);
		}
	}

	auto spriteIt = JSONDoc.FindMember("sprites");
	if (spriteIt != JSONDoc.MemberEnd()) {
		for (const auto& currentSprite : spriteIt->value.GetArray()) {
			const std::string textureAtlas = currentSprite["textureAtlas"].GetString();
			const std::string shader = currentSprite["shader"].GetString();

			for (const auto& current : currentSprite["createSprites"].GetArray()) {
				const std::string nameSprite = current["nameSprite"].GetString();
				const std::string subTexture = current["subTexture"].GetString();

				loadSprite(nameSprite, textureAtlas, shader, subTexture);
			}
		}
	}

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

	auto& textSettings = JSONDoc.FindMember("textSettings")->value;
	PRINT_TEXT::createSymbols(getShader(textSettings["shader"].GetString()), textSettings["fontSize"].GetInt(), textSettings["fontPath"].GetString());

	return true;
}
