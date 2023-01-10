#pragma once

#include <glm/vec2.hpp>
#include <glad/glad.h>
#include <map>
#include <memory>
#include <vector>
#include "ShaderProgram.h"

namespace Renderer {
	class PrintText {

		struct Character {
			GLuint TextureID;   
			glm::ivec2 Size;    
			glm::ivec2 Bearing;  
			GLuint Advance;
		};

		struct Text {
			std::string text;
			glm::vec3 position;
			GLfloat scale;
			glm::vec3 color;
		};

	public:
		static void init(std::shared_ptr<ShaderProgram> shader);
		static void RenderText(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color);
		static void AddTextInTimeBuffer(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color, double Time);
		static void AddTextInCountBuffer(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color, int Count=1);
		static void renderBuffer();
		static void updateBuffer(double duration);
	private:
		static std::shared_ptr<ShaderProgram> m_shader;
		static std::map<GLchar, Character> m_Characters;
		static std::vector<std::pair<Text,double>> m_timeBufferText;
		static std::vector<std::pair<Text,int>> m_countBufferText;
		static GLuint m_VAO, m_VBO;
	};
}