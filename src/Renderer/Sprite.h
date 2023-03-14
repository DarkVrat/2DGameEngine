#pragma once

#include <memory>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "VertexBuffer.h"
#include "VertexArray.h"

 //(RUS) Класс спрайта, хранит буферы, ссылку на шейдер, и ссылку на текстуру, необходимые для отрисовки
//(ENG) Sprite class, stores the buffers, shader reference, and texture reference required for rendering

namespace Renderer {
	class Texture2D;
	class ShaderProgram;

	class Sprite {
	public:
		Sprite(const std::shared_ptr<Texture2D>& pTexture, const glm::vec4& coordTexture);
		~Sprite() {};
		
		void render(const glm::vec3& position, const glm::vec2& size, const float& rotation);

	protected:
		std::shared_ptr<Texture2D> m_texture; 
		glm::vec4 m_coordTexture;
	};
}