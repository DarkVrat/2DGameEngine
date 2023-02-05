#pragma once

#include"VertexArray.h"
#include"IndexBuffer.h"
#include"ShaderProgram.h"
#include <rapidjson/error/en.h>
#include <rapidjson/document.h>
#include<string>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>
#include"../Resources/ResourceManager.h"

#define RENDER_ENGINE Renderer::RenderEngine::Get()

namespace Renderer {
	class RenderEngine {
	public:
		static RenderEngine* Get();

		void draw(const VertexArray& vertexArray, const IndexBuffer& indesBuffer, const ShaderProgram& shader);
		void setClearColor(float r, float g, float b, float a);
		void setDetphTest(bool flag);
		
		void clear();
		void setViewport(unsigned int width, unsigned int height, unsigned intleftOffset=0, unsigned int bottomOffset=0);
		void enableBlend(GLenum sfactor, GLenum dfactor);

		std::string getRender() { return (char*)glGetString(GL_RENDERER); };
		std::string getVersion(){ return (char*)glGetString(GL_VERSION);};

		void loadConfig();
		void saveConfig();

		void setWindowSize(glm::vec2 windowSize=glm::vec2(800,600));
		void setFullScreen(bool fullScreen = true);
		void setVolumeSounde(double volume = 1.0);
		void setDisplayNumber(int monitorNumber = 0);
		void setVolumeSample(std::string name, double volume = 1.0);

		glm::vec2 getWindowSize();
		int getDisplayNumber();
		bool getFullScreen();
		double getVolumeSounde();
		double getVolumeSample(std::string name);
		
		GLFWmonitor* getMonitor();
	private:
		rapidjson::Document JSONDoc;
	};
}