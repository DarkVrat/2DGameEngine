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
#include "../Audio/SampleSourse.h"

#define RESOURCE_MANAGER ResourceManager

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
	//метод получающий argv[0] в main дл€ получени€ пути к игре
	static void setExecutablePath(const std::string& executablePath);
	static std::string getExecutablePath();

	static std::string getFileString(const std::string& relativeFilePath);//функци€ получени€ данных из файла

	static void unloadAllRes();

	//функции создани€ и получени€ указателей на ресурсы
	static void loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	static void loadTexture(std::string textureName, std::string texturePatn, std::vector<std::string> subTextures, const unsigned subWidth, const unsigned subHeigth);
	static void loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const std::string& subTextureName = "Default");
	static void loadStateAnimation(const std::string& spriteName, std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> frames, std::vector<std::string> sources, std::string nextState, bool uninterrupted);
	static void loadSound(const std::string& soundName, const std::string& soundPath);
	
	static std::shared_ptr<Renderer::ShaderProgram> getShader(const std::string& shaderName);
	static std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName);
	static std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);
	static std::shared_ptr<Renderer::StateAnimation> getStateAnimation(const std::string& spriteName);
	static std::shared_ptr<Audio::FileOfSound> getSound(const std::string& soundName);
	static std::shared_ptr<Audio::SampleSourse> getSampleSourse(const std::string& sampleName);

	static rapidjson::Document loadJSONDoc(const std::string& JSONPath);
	static bool loadJSONResurces(const std::string& JSONPath);

private:
	//ќбъ€вление типов Map дл€ хранени€ Shared_ptr указателей наших типов.
	typedef std::map<const std::string, std::shared_ptr<Renderer::StateAnimation>> StateAnimationMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpriteMap;
	typedef std::map<const std::string, std::shared_ptr<Audio::FileOfSound>> SoundMap;
	typedef std::map<const std::string, std::shared_ptr<Audio::SampleSourse>> SampleSourseMap;

	//’ранение всех данных загруженных в программу
	static StateAnimationMap m_stateAnimation;
	static ShaderProgramsMap m_shaderPrograms;
	static TexturesMap m_textures;
	static SpriteMap m_sprite;
	static SoundMap m_soundMap;
	static SampleSourseMap m_sampleSourseMap;

	//путь к папке с игрой
	static std::string m_path;
};