#pragma once

#include <memory>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "VertexBuffer.h"
#include "VertexArray.h"

 //(RUS) ����� �������, ������ ������, ������ �� ������, � ������ �� ��������, ����������� ��� ���������
//(ENG) Sprite class, stores the buffers, shader reference, and texture reference required for rendering

namespace Renderer {
	class Texture2D;
	class ShaderProgram;

	class Sprite {
	public:
		Sprite(const std::shared_ptr<Texture2D>& pTexture, const glm::vec4& coordTexture, const glm::ivec2& aspectRatio);
		~Sprite() {};
		
		void render(const glm::vec3& position, const float& height, const float& width=0.f, const float& rotation=0.f, const glm::vec2& origin = glm::vec2(0.5f, 0.5f));
		void render(const glm::vec3& position, const glm::vec2& size, const float& rotation, const glm::vec2& origin = glm::vec2(0.5f, 0.5f));

		glm::ivec2 getAspectRatio() { return m_aspectRatio; } 

		static void setWindow(const glm::vec2& window) { m_window = window; }

	protected:
		std::shared_ptr<Texture2D> m_texture; 
		glm::vec4 m_coordTexture;
		glm::ivec2 m_aspectRatio;

		static glm::vec2 m_window;
	};
}