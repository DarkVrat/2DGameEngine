#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <rapidjson/document.h>
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/StateAnimation.h"
#include "../Audio/FileOfSound.h"

#define RESOURCE_MANAGER ResourceManager::Get()

namespace Renderer {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class StateAnimation;
}
namespace Audio {
	class FileOfSound;
}

class ResourceManager {
public:
	static ResourceManager* Get();

	//метод получающий argv[0] в main дл€ получени€ пути к игре
	void setExecutablePath(const std::string& executablePath);
	void unloadAllRes();

	//функции создани€ и получени€ указателей на ресурсы
	std::shared_ptr<Renderer::ShaderProgram> loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	std::shared_ptr<Renderer::ShaderProgram> getShader(const std::string& shaderName);

	std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePatn);
	std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName);

	std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const std::string& subTextureName = "Default");
	std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);

	std::shared_ptr<Renderer::StateAnimation> loadStateAnimation(const std::string& spriteName, std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> frames, std::vector<std::string> sources, std::string nextState, bool uninterrupted);
	std::shared_ptr<Renderer::StateAnimation> getStateAnimation(const std::string& spriteName);

	Audio::FileOfSound loadSound(const std::string& soundName, const std::string& soundPath);
	Audio::FileOfSound getSound(const std::string& soundName);

	//создание текстурного атласа
	std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(std::string textureName,std::string texturePatn,std::vector<std::string> subTextures,const unsigned subWidth,const unsigned subHeigth);

	rapidjson::Document loadJSONDoc(const std::string& JSONPath);
	bool loadJSONResurces(const std::string& JSONPath);

private:
	//функци€ получени€ данных из файла
	std::string getFileString(const std::string& relativeFilePath) ;

	//ќбъ€вление типов Map дл€ хранени€ Shared_ptr указателей наших типов.
	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpriteMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::StateAnimation>> StateAnimationMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	typedef std::map < const std::string, Audio::FileOfSound> SoundMap;

	//’ранение всех данных загруженных в программу
	SpriteMap m_sprite;
	StateAnimationMap m_stateAnimation;
	TexturesMap m_textures;
	ShaderProgramsMap m_shaderPrograms;
	SoundMap m_soundMap;

	//путь к папке с игрой
	std::string m_path;
};