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
			Text() :ms_text(""), ms_position(glm::vec3(0, 0, 0)), ms_scale(32), ms_color(glm::vec3(1, 1, 1)) {}
		};

		static void init(const std::shared_ptr<ShaderProgram>& shader, const std::string& fontPath, const std::shared_ptr<Texture2D>& texture);

		static void printTextWrapping(Text text, const int& size, const bool& centr = LEFT, const double& Time = -1.0);
		static void printText(const Text& text, const double& Time = -1.0);

		static void updateBuffer(const double& duration);
		static void renderBuffer();

		static unsigned sizeText(std::string text, GLint scale);

		static void terminate();
	private:
		PrintText();
		~PrintText() {};

		static std::vector<uint8_t> m_advanceChar;
		static std::shared_ptr<ShaderProgram> m_shader;
		static std::shared_ptr<Texture2D> m_texture;
		static std::shared_ptr<VertexArray> m_VAO;
		static VertexBuffer m_PositionVBO;
		static VertexBuffer m_ColorVBO;
		static VertexBuffer m_TextureVBO;
		static float m_fontSize;

		static std::vector<std::pair<Text, double>> m_timeBufferText;
		static std::vector<Text> m_bufferText;
	};
}