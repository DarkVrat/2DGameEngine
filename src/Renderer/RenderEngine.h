#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include <rapidjson/error/en.h>
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
		static void draw(const VertexArray& vertexArray, const IndexBuffer& indesBuffer);
		static void setClearColor(float r, float g, float b, float a);
		static void setDetphTest(bool flag);
		
		static void clear();
		static void setViewport(unsigned int width, unsigned int height, unsigned intleftOffset=0, unsigned int bottomOffset=0);
		static void enableBlend(GLenum sfactor, GLenum dfactor);

		static std::string getRender();
		static std::string getVersion();
		
		static GLFWmonitor* getMonitor();
	private:
	};
}