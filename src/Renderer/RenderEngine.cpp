#include "RenderEngine.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Audio/SoundDevice.h"
#include "../Managers/SoundManager.h"
#include "../Managers/ConfigManager.h"

GLFWwindow* Renderer::RenderEngine::m_pWindow;

std::map<uint8_t, std::map<std::shared_ptr<Renderer::Texture2D>, std::shared_ptr<Renderer::RenderEngine::SpritesForRender>>> Renderer::RenderEngine::m_Sprites;

namespace Renderer { 
	void RenderEngine::init(const std::map<const std::string, std::shared_ptr<Renderer::Texture2D>>& Texture){
		for (int i = 0; i < 4; i++) {
			m_Sprites.emplace(i, std::map<std::shared_ptr<Texture2D>, std::shared_ptr<SpritesForRender>>());
		}
		for (auto& current : Texture) {
			m_Sprites.at(current.second->getShaderSettings()).emplace(current.second, std::make_shared<SpritesForRender>());
		}
	}

	//(RUS) отрисовка изображения
	//(ENG) image rendering
	void RenderEngine::draw(const std::shared_ptr<Texture2D>& PTRtexture2D, const glm::vec4& textureSprite, const glm::mat4& modelSprite){
		m_Sprites.at(PTRtexture2D->getShaderSettings()).at(PTRtexture2D)->AddSprite(textureSprite, modelSprite);
	}

	void RenderEngine::drawInstanced(const VertexArray& vertexArray, const GLuint& count){
		vertexArray.bind();

		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, count);
	}

	void RenderEngine::render(){

		for (uint8_t i = 0; i < 4; i++) {
			for (auto& currentSprites : m_Sprites.at(i)) {
				if (currentSprites.second->Size() < 1) {
					continue;
				}

				currentSprites.first->bind();

				if (i & 0x01) {
					currentSprites.second->Sort();
				}

				currentSprites.second->Load();

				drawInstanced(*currentSprites.second->getVAO(), currentSprites.second->Size());

				currentSprites.second->Clear();
			}
		}
	}

	 //(RUS) Установка цвета фона
	//(ENG) Setting the background color
	void RenderEngine::setClearColor(const int& r, const int& g, const int& b, const int& a){
		glClearColor(r/255.0, g/255.0, b/255.0, a/255.0);
	}

	 //(RUS) Включение/отключение глубины для отрисовки
	//(ENG) Enable/disable depth for rendering
	void RenderEngine::setDetphTest(const bool& flag){
		if (flag) { glEnable(GL_DEPTH_TEST); }
		else { glDisable(GL_DEPTH_TEST); }
	}

	 //(RUS) очистка буферов
	//(ENG) clearing buffers
	void RenderEngine::clear() {
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}

	 //(RUS) Установка ширины и высоты области просмотра
	//(ENG) Setting Viewport Width and Height
	void RenderEngine::setViewport(const unsigned& width, const unsigned& height, const unsigned& intleftOffset, const unsigned& bottomOffset){
		glViewport(intleftOffset, bottomOffset, width, height);
	}

	 //(RUS) Установка параметров смешивания
	//(ENG) Setting blending options
	void RenderEngine::enableBlend(const GLenum& sfactor, const GLenum& dfactor){
		glEnable(GL_BLEND);
		glBlendFunc(sfactor, dfactor);
	}

	 //(RUS) Получение устройства рендеринга, и версии драйвера
	//(ENG) Getting the render device and driver version
	std::string RenderEngine::getRender() { return (char*)glGetString(GL_RENDERER); }
	std::string RenderEngine::getVersion() { return (char*)glGetString(GL_VERSION); }

	 //(RUS) получение монитора
	//(ENG) getting a monitor
	GLFWmonitor* RenderEngine::getMonitor() {
		if (!CONFIG_MANAGER::getFullScreen()) { return NULL; }

		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		if (count <= CONFIG_MANAGER::getDisplayNumber()) { CONFIG_MANAGER::setDisplayNumber(); }
		return monitors[CONFIG_MANAGER::getDisplayNumber()];
	}

	int RenderEngine::getCountMonitor(){
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		return count;
	}

	std::vector<glm::ivec2> RenderEngine::getScreenResolutions(){
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		count;
		const GLFWvidmode* modes = glfwGetVideoModes(monitors[CONFIG_MANAGER::getDisplayNumber()], &count);

		std::vector<glm::ivec2> resolutions;
		glm::ivec2 last=glm::ivec2(0,0);
		for (int i = 0; i < count; i++) {
			int width = modes[i].width; 
			int height = modes[i].height;
			if (last.x == width && last.y == height) {
				continue;
			}
			last = glm::ivec2(width, height);
			resolutions.push_back(glm::ivec2(last));
		}

		return resolutions;
	}
	
	void RenderEngine::setWindow(GLFWwindow* window){
		m_pWindow = window;
	}

	void RenderEngine::applySettings(){
		glm::ivec2 windowSize = CONFIG_MANAGER::getWindowSize();
		 
		glfwSetWindowSize(m_pWindow, windowSize.x, windowSize.y);
		glfwSetWindowMonitor(m_pWindow, getMonitor() , 0, 0, windowSize.x, windowSize.y, 0);
		glfwSwapInterval(CONFIG_MANAGER::getVSync());

		if (CONFIG_MANAGER::getFullScreen()) {
			int screenWidth, screenHeight;
			glfwGetFramebufferSize(m_pWindow, &screenWidth, &screenHeight);
			glViewport(0, 0, screenWidth, screenHeight);
		}
		else {
			int windowWidth, windowHeight;
			glfwGetWindowSize(m_pWindow, &windowWidth, &windowHeight);
			glViewport(0, 0, windowWidth, windowHeight);

			int screenWidth, screenHeight;
			glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), NULL, NULL, &screenWidth, &screenHeight);
			glfwSetWindowPos(m_pWindow, (screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);
		}
	}

	void RenderEngine::closeWindow() {
		glfwSetWindowShouldClose(m_pWindow, GL_TRUE);
	}
	
	
	RenderEngine::SpritesForRender::SpritesForRender(){
		ms_sprites.clear();

		ms_VAO = std::make_shared<VertexArray>();
		ms_VAO->bind();

		const GLfloat vertexCoords[] = {
			// X  Y     //      1---2
			0.f, 0.f,   //0     |  /|
			0.f, 1.f,   //1     | / |
			1.f, 1.f,   //2     |/  |
			1.f, 0.f,   //3     0---3      
		};
		VertexBuffer vertexCoordsBuffer;
		vertexCoordsBuffer.init(vertexCoords, sizeof(vertexCoords));
		ms_VAO->addBuffer(vertexCoordsBuffer, 0, 2);

		ms_textureCoordsBuffer.init(NULL, sizeof(GLfloat) * 4);
		ms_VAO->addBuffer(ms_textureCoordsBuffer, 1, 4, GL_FLOAT, true);

		ms_modelMatBuffer[0].init(NULL, sizeof(GLfloat) * 4);
		ms_modelMatBuffer[1].init(NULL, sizeof(GLfloat) * 4);
		ms_modelMatBuffer[2].init(NULL, sizeof(GLfloat) * 4);
		ms_modelMatBuffer[3].init(NULL, sizeof(GLfloat) * 4);
		ms_VAO->addBuffer(ms_modelMatBuffer[0], 2, 4, GL_FLOAT, true);
		ms_VAO->addBuffer(ms_modelMatBuffer[1], 3, 4, GL_FLOAT, true);
		ms_VAO->addBuffer(ms_modelMatBuffer[2], 4, 4, GL_FLOAT, true);
		ms_VAO->addBuffer(ms_modelMatBuffer[3], 5, 4, GL_FLOAT, true);

		ms_VAO->unbind();
	}

	void RenderEngine::SpritesForRender::Load(){
		size_t newHash = getHash();
		if (ms_hashBuffer == newHash) {
			return;
		}
		else {
			ms_hashBuffer = newHash;
		}

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

		ms_textureCoordsBuffer.update(&textures[0], size * sizeof(glm::vec4));
		ms_modelMatBuffer[0].update(&model0[0], size * sizeof(glm::vec4));
		ms_modelMatBuffer[1].update(&model1[0], size * sizeof(glm::vec4));
		ms_modelMatBuffer[2].update(&model2[0], size * sizeof(glm::vec4));
		ms_modelMatBuffer[3].update(&model3[0], size * sizeof(glm::vec4));
	}

	std::size_t RenderEngine::SpritesForRender::getHash(){
		size_t hash = 0;
		for (const auto& current : ms_sprites) {
			hash_combine(hash, current.first);
			hash_combine(hash, current.second);
		}
		return hash;
	}
}
