#include "RenderEngine.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Audio/SoundDevice.h"
#include "../Managers/SoundManager.h"
#include "../Managers/ConfigManager.h"

static Renderer::RenderEngine* renderEngine = nullptr;

namespace Renderer {
	RenderEngine* RenderEngine::Get() {
		if (renderEngine == nullptr)
			renderEngine = new RenderEngine();
		return renderEngine;
	}

	void RenderEngine::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader){
		shader.use();
		vertexArray.bind();
		indexBuffer.bind();
		
		glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void RenderEngine::setClearColor(float r, float g, float b, float a){
		glClearColor(r, g, b, a);
	}

	void RenderEngine::setDetphTest(bool flag){
		if (flag) {
			glEnable(GL_DEPTH_TEST);
		}
		else{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void RenderEngine::clear() {
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}

	void RenderEngine::setViewport(unsigned int width, unsigned int height, unsigned intleftOffset, unsigned int bottomOffset){
		glViewport(intleftOffset, bottomOffset, width, height);
	}

	void RenderEngine::enableBlend(GLenum sfactor, GLenum dfactor){
		glEnable(GL_BLEND);
		glBlendFunc(sfactor, dfactor);
	}

	GLFWmonitor* RenderEngine::getMonitor() {
		if (!CONFIG_MANAGER->getFullScreen()) { 
			return NULL; 
		}
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		if (count <= CONFIG_MANAGER->getDisplayNumber()) {
			CONFIG_MANAGER->setDisplayNumber();
		}
		return monitors[CONFIG_MANAGER->getDisplayNumber()];
	}
}
