#include "RenderEngine.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Audio/SoundDevice.h"
#include "../Managers/SoundManager.h"
#include "../Managers/ConfigManager.h"

namespace Renderer {
	 //(RUS) ��������� �����������
	//(ENG) image rendering
	void RenderEngine::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer){
		vertexArray.bind();
		indexBuffer.bind();
		
		glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
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
}
