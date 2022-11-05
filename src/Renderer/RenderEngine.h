#pragma once

#include"VertexArray.h"
#include"IndexBuffer.h"
#include"ShaderProgram.h"
#include<string>

namespace Renderer {
	class RenderEngine {
	public:
		static void draw(const VertexArray& vertexArray, const IndexBuffer& indesBuffer, const ShaderProgram& shader);
		static void setClearColor(float r, float g, float b, float a);
		static void setDetphTest(bool flag);
		static void clear();
		static void setViewport(unsigned int width, unsigned int height, unsigned intleftOffset=0, unsigned int bottomOffset=0);

		static std::string getRender() { return (char*)glGetString(GL_RENDERER); };
		static std::string getVersion(){ return (char*)glGetString(GL_VERSION);};
	};
}