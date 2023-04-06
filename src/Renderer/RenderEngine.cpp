#include "RenderEngine.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Audio/SoundDevice.h"
#include "../Managers/SoundManager.h"
#include "../Managers/ConfigManager.h"

std::shared_ptr<Renderer::VertexArray> Renderer::RenderEngine::m_VAO;
Renderer::VertexBuffer  Renderer::RenderEngine::m_textureCoordsBuffer;
Renderer::VertexBuffer  Renderer::RenderEngine::m_modelMatBuffer[4];
GLFWwindow* Renderer::RenderEngine::m_pWindow;

std::map<std::shared_ptr<Renderer::Texture2D>, Renderer::RenderEngine::SpritesForRender> Renderer::RenderEngine::m_Sprites;
std::map<std::shared_ptr<Renderer::Texture2D>, Renderer::RenderEngine::SpritesForRender> Renderer::RenderEngine::m_SpritesWithBlend;

namespace Renderer {
	void RenderEngine::init(const std::map<const std::string, std::shared_ptr<Renderer::Texture2D>>& Texture){
		SpritesForRender forMap;
		for (auto& current : Texture) {
			if (current.second->getBleng()) {
				m_SpritesWithBlend.emplace(current.second, forMap);
			}
			else {
				m_Sprites.emplace(current.second, forMap);
			}
		}

		m_VAO = std::make_shared<VertexArray>();
		m_VAO->bind();

		const GLfloat vertexCoords[] = {
			// X  Y     //      1---2
			0.f, 0.f,   //0     |  /|
			0.f, 1.f,   //1     | / |
			1.f, 1.f,   //2     |/  |
			1.f, 0.f,   //3     0---3      
		};
		VertexBuffer vertexCoordsBuffer;
		vertexCoordsBuffer.init(vertexCoords, sizeof(vertexCoords));
		m_VAO->addBuffer(vertexCoordsBuffer, 0, 2);

		m_textureCoordsBuffer.init(NULL, sizeof(GLfloat)*4);
		m_VAO->addBuffer(m_textureCoordsBuffer, 1, 4, true);

		m_modelMatBuffer[0].init(NULL, sizeof(GLfloat) * 4);
		m_modelMatBuffer[1].init(NULL, sizeof(GLfloat) * 4);
		m_modelMatBuffer[2].init(NULL, sizeof(GLfloat) * 4);
		m_modelMatBuffer[3].init(NULL, sizeof(GLfloat) * 4);
		m_VAO->addBuffer(m_modelMatBuffer[0], 2, 4, true);
		m_VAO->addBuffer(m_modelMatBuffer[1], 3, 4, true);
		m_VAO->addBuffer(m_modelMatBuffer[2], 4, 4, true);
		m_VAO->addBuffer(m_modelMatBuffer[3], 5, 4, true);

		m_VAO->unbind();
	}

	//(RUS) ��������� �����������
	//(ENG) image rendering
	void RenderEngine::draw(const std::shared_ptr<Texture2D>& PTRtexture2D, const glm::vec4& textureSprite, const glm::mat4& modelSprite){
		if (PTRtexture2D->getBleng()) {
			m_SpritesWithBlend.at(PTRtexture2D).AddSprite(textureSprite, modelSprite);
		}
		else {
			m_Sprites.at(PTRtexture2D).AddSprite(textureSprite, modelSprite);
		}
	}

	void RenderEngine::drawInstanced(const VertexArray& vertexArray, const GLuint& count){
		vertexArray.bind();

		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, count);
	}

	void RenderEngine::render(){
		for (auto& currentSrites : m_Sprites) {
			if (currentSrites.second.Size() < 1) {
				continue;
			}

			currentSrites.first->bind();

			currentSrites.second.Load(m_textureCoordsBuffer, m_modelMatBuffer);

			drawInstanced(*m_VAO, currentSrites.second.Size());

			currentSrites.second.Clear();
		}
		for (auto& currentSrites : m_SpritesWithBlend) {
			if (currentSrites.second.Size() < 1) {
				continue;
			}

			currentSrites.first->bind();

			currentSrites.second.Sort();

			currentSrites.second.Load(m_textureCoordsBuffer, m_modelMatBuffer);

			drawInstanced(*m_VAO, currentSrites.second.Size());

			currentSrites.second.Clear();
		}
	}

	 //(RUS) ��������� ����� ����
	//(ENG) Setting the background color
	void RenderEngine::setClearColor(const int& r, const int& g, const int& b, const int& a){
		glClearColor(r/255.0, g/255.0, b/255.0, a/255.0);
	}

	 //(RUS) ���������/���������� ������� ��� ���������
	//(ENG) Enable/disable depth for rendering
	void RenderEngine::setDetphTest(const bool& flag){
		if (flag) { glEnable(GL_DEPTH_TEST); }
		else { glDisable(GL_DEPTH_TEST); }
	}

	 //(RUS) ������� �������
	//(ENG) clearing buffers
	void RenderEngine::clear() {
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}

	 //(RUS) ��������� ������ � ������ ������� ���������
	//(ENG) Setting Viewport Width and Height
	void RenderEngine::setViewport(const unsigned& width, const unsigned& height, const unsigned& intleftOffset, const unsigned& bottomOffset){
		glViewport(intleftOffset, bottomOffset, width, height);
	}

	 //(RUS) ��������� ���������� ����������
	//(ENG) Setting blending options
	void RenderEngine::enableBlend(const GLenum& sfactor, const GLenum& dfactor){
		glEnable(GL_BLEND);
		glBlendFunc(sfactor, dfactor);
	}

	 //(RUS) ��������� ���������� ����������, � ������ ��������
	//(ENG) Getting the render device and driver version
	std::string RenderEngine::getRender() { return (char*)glGetString(GL_RENDERER); }
	std::string RenderEngine::getVersion() { return (char*)glGetString(GL_VERSION); }

	 //(RUS) ��������� ��������
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
}
