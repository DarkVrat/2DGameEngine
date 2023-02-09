#pragma once

#include <memory>
#include <string>
#include <glm/vec2.hpp>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

 //(RUS) Класс спрайта, хранит буферы, ссылку на шейдер, и ссылку на текстуру, необходимые для отрисовки
//(ENG) Sprite class, stores the buffers, shader reference, and texture reference required for rendering

namespace Renderer {
	class Texture2D;
	class ShaderProgram;

	class Sprite {
	public:
		Sprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram);
		~Sprite() {};
		
		void render(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)const;

	protected:
		std::shared_ptr<Texture2D> m_texture; 
		std::shared_ptr<ShaderProgram> m_shaderProgram; 

		VertexArray m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_textureCoordsBuffer;
		IndexBuffer m_indexBuffer;
	};
}