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
		static void drawInstanced(const VertexArray& vertexArray, const GLuint& count);
		static void setClearColor(const int& r, const int& g, const int& b, const int& a);
		static void setDetphTest(const bool& flag);
		
		static void clear();
		static void setViewport(const unsigned& width, const unsigned& height, const unsigned& intleftOffset=0, const unsigned& bottomOffset=0);
		static void enableBlend(const GLenum& sfactor, const GLenum& dfactor);

		static std::string getRender();
		static std::string getVersion();
		
		static GLFWmonitor* getMonitor();
	private:
	};
}