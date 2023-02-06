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

#define PRINT_TEXT Renderer::PrintText::Get()

namespace Renderer {
	class PrintText {

		struct Character {
			GLuint TextureID;   
			glm::ivec2 Size;    
			glm::ivec2 Bearing;  
			GLuint Advance;

			Character(GLuint T, glm::ivec2 S, glm::ivec2 B, GLuint A) :TextureID(T), Size(S), Bearing(B), Advance(A) {}
			~Character() {glDeleteTextures(1, &TextureID);}
		};

		struct Text {
			std::string text;
			glm::vec3 position;
			GLfloat scale;
			glm::vec3 color;
		};

	public:
		static PrintText* Get();
		static void Terminate();

		void SetShader(std::shared_ptr<ShaderProgram> shader);

		void RenderText(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color);

		void AddTextInTimeBuffer(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color, double Time);
		void AddTextInCountBuffer(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color, int Count=1);
		void renderBuffer();
		void updateBuffer(double duration);

	private:
		~PrintText();

		std::shared_ptr<ShaderProgram> m_shader;
		std::map<GLchar, std::shared_ptr<Character>> m_Characters;
		std::vector<std::pair<Text,double>> m_timeBufferText;
		std::vector<std::pair<Text,int>> m_countBufferText;
		VertexArray m_VertexArray;
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
	};
}