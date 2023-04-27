#pragma once

#include "VertexArray.h"
#include <rapidjson/error/en.h>
#include <algorithm>
#include <rapidjson/document.h>
#include <string>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>
#include <functional>
#include <glm/gtx/hash.hpp>
#include "../Managers/ResourceManager.h"

#define RENDER_ENGINE Renderer::RenderEngine

 //(RUS) Класс для установки параметров OpenGL 
//(ENG) Class for setting OpenGL options

namespace Renderer {
	class RenderEngine {
	public:
		
		struct SpritesForRender{
		public:
			SpritesForRender();

			void Clear() {ms_sprites.clear();}
			void AddSprite(const glm::vec4& texture, const glm::mat4& model) {
				ms_sprites.push_back(std::make_pair(texture, model));
			}
			int Size() { return ms_sprites.size(); }

			void Load();
			std::shared_ptr<VertexArray> getVAO() { return ms_VAO; }

			void Sort() {
				auto comp = [](std::pair<glm::vec4, glm::mat4> a, std::pair<glm::vec4, glm::mat4> b) {
					return a.second[2][2] < b.second[2][2];
				};
				std::sort(ms_sprites.begin(), ms_sprites.end(), comp);
			}
		private:
			size_t ms_hashBuffer;
			template <class T>
			void hash_combine(size_t& seed, const T& value);
			std::size_t getHash();

			std::vector<std::pair<glm::vec4, glm::mat4>> ms_sprites;
			std::shared_ptr<VertexArray> ms_VAO;
			VertexBuffer  ms_textureCoordsBuffer;
			VertexBuffer  ms_modelMatBuffer[4];
		};

		static void init(const std::map<const std::string, std::shared_ptr<Renderer::Texture2D>>& Texture);

		static void draw(const std::shared_ptr<Texture2D>& PTRtexture2D, const glm::vec4& textureSprite, const glm::mat4& modelSprite);
		static void drawInstanced(const VertexArray& vertexArray, const GLuint& count);
		static void render();
		
		static void setClearColor(const int& r, const int& g, const int& b, const int& a);
		static void setDetphTest(const bool& flag);
		
		static void clear();
		static void setViewport(const unsigned& width, const unsigned& height, const unsigned& intleftOffset=0, const unsigned& bottomOffset=0);
		static void enableBlend(const GLenum& sfactor, const GLenum& dfactor);

		static std::string getRender();
		static std::string getVersion();
		
		static GLFWmonitor* getMonitor();
		static int getCountMonitor();
		static std::vector<glm::ivec2> getScreenResolutions();

		static void setWindow(GLFWwindow* window);
		static void applySettings();
		static void closeWindow();
	private:
		static GLFWwindow* m_pWindow;

		static std::map<uint8_t, std::map<std::shared_ptr<Texture2D>, std::shared_ptr<SpritesForRender>>> m_Sprites;
	};


	template<class T>
	inline void RenderEngine::SpritesForRender::hash_combine(size_t& seed, const T& value){
		std::hash<T> hasher;
		seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
}