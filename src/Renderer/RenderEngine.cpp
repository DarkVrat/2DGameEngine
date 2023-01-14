#include "RenderEngine.h"
#include <rapidjson/error/en.h>
#include <rapidjson/document.h>
#include <fstream>
#include <sstream>
#include <iostream>

glm::vec2 Renderer::RenderEngine::g_windowSize;
int Renderer::RenderEngine::g_displayNumber;
bool Renderer::RenderEngine::g_fullScreen;
double Renderer::RenderEngine::g_volumeSound;

namespace Renderer {
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

	void RenderEngine::init(const std::string& executablePath) {
		size_t found = executablePath.find_last_of("/\\");
		std::string m_path = executablePath.substr(0, found);
		std::ifstream f;
		f.open(m_path + "/" + "res/config.json", std::ios::in | std::ios::binary);
		if (!f.is_open()) {
			std::cerr << "(!) Failed to open: res / config.json" << std::endl;
			return;
		}
		std::stringstream buffer;
		buffer << f.rdbuf();
		const std::string JSONString = buffer.str();
		if (JSONString.empty()) {
			std::cerr << "(!) No JSON resources file" << std::endl;
			return;
		}
		rapidjson::Document JSONDoc;
		rapidjson::ParseResult parseResult = JSONDoc.Parse(JSONString.c_str());
		if (!parseResult) {
			std::cerr << "(!) JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << std::endl;
			std::cerr << "(!) in JSON resources file: " << std::endl;
			return;
		}

		const auto& windowSize = JSONDoc.FindMember("window size")->value;
		g_windowSize = glm::vec2(windowSize["width"].GetInt(), windowSize["height"].GetInt());
		
		g_fullScreen = JSONDoc.FindMember("full screen")->value.GetBool();

		g_displayNumber = JSONDoc.FindMember("display")->value.GetInt();

		g_volumeSound = JSONDoc.FindMember("volume")->value.GetDouble();
	}
}
