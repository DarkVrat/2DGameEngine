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

namespace Renderer {
	class PrintText {

		struct Character {
			GLuint ms_textureID;   
			glm::ivec2 ms_size;    
			glm::ivec2 ms_bearing;  
			GLuint ms_advance;

			Character(GLuint T, glm::ivec2 S, glm::ivec2 B, GLuint A) :ms_textureID(T), ms_size(S), ms_bearing(B), ms_advance(A) {}
			~Character() {glDeleteTextures(1, &ms_textureID);}
		};

		struct Text {
			std::string ms_text;
			glm::vec3 ms_position;
			GLfloat ms_scale;
			glm::vec3 ms_color;
		};

	public:
		static void createSymbols(std::shared_ptr<ShaderProgram> shader, int fontSize, std::string fontPath);

		static void renderText(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color);

		static void addTextInTimeBuffer(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color, double Time);
		static void addTextInCountBuffer(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color, int Count=1);
		static void renderBuffer();
		static void updateBuffer(double duration);

	private:
		PrintText();
		~PrintText();

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