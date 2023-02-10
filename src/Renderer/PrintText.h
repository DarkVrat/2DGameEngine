#pragma once

#include <glm/vec3.hpp>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#define PRINT_TEXT Renderer::PrintText

 //(RUS)  ласс дл€ рендеринга текста
//(ENG) Text rendering class

namespace Renderer {
	class PrintText {
		 //(RUS) структура описывающа€ символы, хранит id текстуры, и параметры
		//(ENG) structure describing symbols, stores texture id, and parameters
		struct Character {
			GLuint ms_textureID;   
			glm::ivec2 ms_size;    
			glm::ivec2 ms_bearing;  
			GLuint ms_advance;

			Character(const GLuint& texture, const glm::ivec2& size, const glm::ivec2& bearing, const GLuint& advance) 
				:ms_textureID(texture), ms_size(size), ms_bearing(bearing), ms_advance(advance) {}
			~Character() {glDeleteTextures(1, &ms_textureID);}
		};

		 //(RUS) —труктура описывающа€ текст дл€ рендеринга из буферов
		//(ENG) Structure describing text to render from buffers
		struct Text {
			std::string ms_text;
			glm::vec3 ms_position;
			GLfloat ms_scale;
			glm::vec3 ms_color;
		};
	public:
		static void createSymbols(const std::shared_ptr<ShaderProgram>& shader, const int& fontSize, const std::string& fontPath);

		static void renderText(const std::string& text, glm::vec3 position, const GLfloat& scale, const glm::vec3& color);

		static void addTextInTimeBuffer(const std::string& text, const glm::vec3& position, const GLfloat& scale, const glm::vec3& color, const double& Time=1000);
		static void addTextInCountBuffer(const std::string& text, const glm::vec3& position, const GLfloat& scale, const glm::vec3& color, const int& Count=1);
		static void renderBuffer();
		static void updateBuffer(const double& duration);

		static void terminate();
	private:
		PrintText();
		~PrintText() {};

		static std::shared_ptr<ShaderProgram> m_shader;
		static std::map<GLchar, std::shared_ptr<Character>> m_characters;
		static std::vector<std::pair<Text,double>> m_timeBufferText;
		static std::vector<std::pair<Text,int>> m_countBufferText;
		static VertexArray* m_vertexArray;
		static VertexBuffer m_vertexBuffer;
		static IndexBuffer m_indexBuffer;
		static int m_fontSize;
	};
}