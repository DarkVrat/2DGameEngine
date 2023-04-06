#pragma once

#include "VertexArray.h"
#include "ShaderProgram.h"
#include <rapidjson/error/en.h>
#include <algorithm>
#include <rapidjson/document.h>
#include <string>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>
#include "../Managers/ResourceManager.h"

#define RENDER_ENGINE Renderer::RenderEngine

 //(RUS) Класс для установки параметров OpenGL 
//(ENG) Class for setting OpenGL options

namespace Renderer {
	class RenderEngine {
	public:
		
		struct SpritesForRender{
			std::vector<std::pair<glm::vec4, glm::mat4>> ms_sprites;

			SpritesForRender() { 
				Clear();
			}

			void Clear() {ms_sprites.clear();}
			void AddSprite(const glm::vec4& texture, const glm::mat4& model) {ms_sprites.push_back(std::make_pair(texture, model));}
			int Size() { return ms_sprites.size(); }

			void Load(const VertexBuffer& textureCoordsBuffer, const VertexBuffer* modelMatBuffer) {
				int size = Size();

				std::vector<glm::vec4> textures;
				std::vector<glm::vec4> model0;
				std::vector<glm::vec4> model1;
				std::vector<glm::vec4> model2;
				std::vector<glm::vec4> model3;
				textures.reserve(size);
				model0.reserve(size);
				model0.reserve(size);
				model0.reserve(size);
				model0.reserve(size);

				for (auto& current : ms_sprites) {
					textures.push_back(current.first);
					model0.push_back(current.second[0]);
					model1.push_back(current.second[1]);
					model2.push_back(current.second[2]);
					model3.push_back(current.second[3]);
				}

				textureCoordsBuffer.update(&textures[0], size * sizeof(glm::vec4));
				modelMatBuffer[0].update(&model0[0], size * sizeof(glm::vec4));
				modelMatBuffer[1].update(&model1[0], size * sizeof(glm::vec4));
				modelMatBuffer[2].update(&model2[0], size * sizeof(glm::vec4));
				modelMatBuffer[3].update(&model3[0], size * sizeof(glm::vec4));
			}

			void Sort() {
				auto comp = [](std::pair<glm::vec4, glm::mat4> a, std::pair<glm::vec4, glm::mat4> b) {
					return a.second[2][2] < b.second[2][2];
				};
				std::sort(ms_sprites.begin(), ms_sprites.end(), comp);
			}
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

		static std::shared_ptr<VertexArray> m_VAO;
		static VertexBuffer  m_textureCoordsBuffer;
		static VertexBuffer  m_modelMatBuffer[4];
		static GLFWwindow* m_pWindow;

		static std::map<std::shared_ptr<Texture2D>, SpritesForRender> m_Sprites;
		static std::map<std::shared_ptr<Texture2D>, SpritesForRender> m_SpritesWithBlend;
	};
}