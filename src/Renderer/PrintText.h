#pragma once

#include <glm/vec3.hpp>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#define PRINT_TEXT Renderer::PrintText
#define CENTR true
#define LEFT false

 //(RUS) Класс для рендеринга текста
//(ENG) Text rendering class

namespace Renderer {
	class PrintText {

	public:
		 //(RUS) Структура описывающая текст для рендеринга из буферов
		//(ENG) Structure describing text to render from buffers
		struct Text {
			std::string ms_text;
			glm::vec3 ms_position;
			GLint ms_scale;
			glm::vec3 ms_color;

			Text(const std::string& text, const glm::vec3& position, const GLint& scale, const glm::vec3& color) 
				:ms_text(text), ms_position(position), ms_scale(scale), ms_color(color) {}
		};

		static void init(const std::shared_ptr<ShaderProgram>& shader, const std::string& fontPath, const std::shared_ptr<Texture2D>& texture);

		//static void renderText(const std::string& text, glm::ivec3 position, const GLint& scale, const glm::vec3& color);

		static void printTextWrappingTime( Text text, const int& size, const bool& centr = LEFT, const double& Time = 1000);
		static void printTextWrapping( Text text, const int& size, const bool& centr = LEFT);
		static void printTextTime(const Text& text, const double& Time=1000);
		static void printText(const Text& text);

		static void renderBuffer();

		static void updateBuffer(const double& duration);

		static void terminate();
	private:
		PrintText();
		~PrintText() {};

		static std::shared_ptr<ShaderProgram> m_shader;
		static std::vector<uint8_t> m_advanceChar;
		static std::shared_ptr<Texture2D> m_texture;
		static std::vector<std::pair<Text,double>> m_timeBufferText;
		static std::vector<Text> m_bufferText;
		static VertexArray* m_vertexArray;
		static VertexBuffer m_vertexBuffer;
		static IndexBuffer m_indexBuffer;
		static int m_fontSize;
	};
}