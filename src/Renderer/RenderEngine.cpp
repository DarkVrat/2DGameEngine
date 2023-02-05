#include "RenderEngine.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Audio/SoundDevice.h"
#include "../Audio/SoundManager.h"

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

	void RenderEngine::loadConfig() {
		const std::string JSONString = RESOURCE_MANAGER->getFileString("res/config.json");

		if (JSONString.empty()) {
			std::cerr << "(!) No JSON resources file" << std::endl;
			return;
		}

		rapidjson::ParseResult parseResult = JSONDoc.Parse(JSONString.c_str());

		if (!parseResult) {
			std::cerr << "(!) JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << std::endl;
			std::cerr << "(!) in JSON resources file: " << std::endl;
			return;
		}
	}
	void RenderEngine::saveConfig() {
		std::ofstream f;
		f.open(RESOURCE_MANAGER->getExecutablePath() + "/" + "res/config.json");
		if (!f.is_open()) {
			std::cerr << "(!) Failed to open: res / config.json" << std::endl;
			return;
		}

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		JSONDoc.Accept(writer);
		
		f << buffer.GetString();
	}

	void RenderEngine::setWindowSize(glm::vec2 windowSize) {
		JSONDoc.FindMember("window size")->value["width"].SetInt(windowSize.x);
		JSONDoc.FindMember("window size")->value["height"].SetInt(windowSize.y);
	}
	void RenderEngine::setFullScreen(bool fullScreen) {JSONDoc.FindMember("full screen")->value.SetBool(fullScreen);}
	void RenderEngine::setDisplayNumber(int monitorNumber) {JSONDoc.FindMember("display")->value.SetInt(monitorNumber);}
	void RenderEngine::setVolumeSounde(double volume) {
		JSONDoc.FindMember("volume")->value.SetDouble(volume); 
		SOUND_MANAGER->UpdateGain();
	}
	void RenderEngine::setVolumeSample(std::string name, double volume) {
		if (name == "default") {
			JSONDoc.FindMember("volumeSample")->value["default"].SetDouble(volume);
		}
		SOUND_MANAGER->UpdateGain();
	}

	glm::vec2 RenderEngine::getWindowSize() {
		int x=JSONDoc.FindMember("window size")->value["width"].GetInt();
		int y=JSONDoc.FindMember("window size")->value["height"].GetInt();
		return glm::vec2(x, y);
	}
	int RenderEngine::getDisplayNumber(){ return JSONDoc.FindMember("display")->value.GetInt(); }
	bool RenderEngine::getFullScreen(){return JSONDoc.FindMember("full screen")->value.GetBool(); }
	double RenderEngine::getVolumeSounde(){return JSONDoc.FindMember("volume")->value.GetDouble();}
	double RenderEngine::getVolumeSample(std::string name) {
		if (name == "default") {
			return JSONDoc.FindMember("volumeSample")->value["default"].GetDouble();  
		}
		return 1.0;
	}

	GLFWmonitor* RenderEngine::getMonitor() {
		if (!getFullScreen()) { return NULL; }
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		if (count <= getDisplayNumber()) { setDisplayNumber(); }
		return monitors[getDisplayNumber()];
	}
}
