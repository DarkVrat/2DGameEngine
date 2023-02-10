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

 //(RUS) �������� ��������, �������� ������, � �������� �� � �������
//(ENG) Resource manager, loading data, and passing it to objects

class ResourceManager {
public:
	static void setExecutablePath(const std::string& executablePath);
	static std::string getExecutablePath();

	static std::string getFileString(const std::string& relativeFilePath);

	static void unloadAllRes();

	static void loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	static void loadTexture(const std::string& textureName, const std::string& texturePatn, const std::map<std::string, Renderer::Texture2D::SubTexture2D>& subTextures);
	static void loadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const std::string& subTextureName = "Default");
	static void loadStateAnimation(const std::string& stateName, const  std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>>& frames, const std::vector<std::string>& sources, const std::string& nextState, const bool& uninterrupted);
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
	typedef std::map<const std::string, std::shared_ptr<Renderer::StateAnimation>> StateAnimationMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpriteMap;
	typedef std::map<const std::string, std::shared_ptr<Audio::FileOfSound>> SoundMap;
	typedef std::map<const std::string, std::shared_ptr<Audio::SampleSourse>> SampleSourseMap;

	static StateAnimationMap m_stateAnimation;
	static ShaderProgramsMap m_shaderPrograms;
	static TexturesMap m_textures;
	static SpriteMap m_sprite;
	static SoundMap m_soundMap;
	static SampleSourseMap m_sampleSourseMap;

	static std::string m_path;
};