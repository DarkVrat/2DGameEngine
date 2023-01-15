#pragma once

#include <glm/vec2.hpp>
#include <glad/glad.h>
#include <map>
#include <memory>
#include <vector>
#include "ShaderProgram.h"

#define PRINT_TEXT Renderer::PrintText::Get()

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
		static PrintText* Get();

		void SetShader(std::shared_ptr<ShaderProgram> shader);
		void RenderText(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color);
		void AddTextInTimeBuffer(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color, double Time);
		void AddTextInCountBuffer(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color, int Count=1);
		void renderBuffer();
		void updateBuffer(double duration);

	private:
		std::shared_ptr<ShaderProgram> m_shader;
		std::map<GLchar, Character> m_Characters;
		std::vector<std::pair<Text,double>> m_timeBufferText;
		std::vector<std::pair<Text,int>> m_countBufferText;
		GLuint m_VAO, m_VBO;
	};
}