#pragma once

#include <memory>
#include <string>
#include <glm/vec2.hpp>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

namespace Renderer {
	class Texture2D;
	class ShaderProgram;

	class Sprite {
	public:
		//Конструкторы и деструкторы
		Sprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram);
		~Sprite() {};
		
		//отрисовка
		void render(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)const;

	protected:
		std::shared_ptr<Texture2D> m_texture; //текстура
		std::shared_ptr<ShaderProgram> m_shaderProgram; //Шейдерная программа

		//буферы
		VertexArray m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_textureCoordsBuffer;
		IndexBuffer m_indexBuffer;
	};
}