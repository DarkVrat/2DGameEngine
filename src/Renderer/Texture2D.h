#pragma once

#include <glad/glad.h>
#include <string>
#include <map>
#include <memory>
#include <glm/mat2x2.hpp>
#include "ShaderProgram.h"

 //(RUS) Класс текстуры, хранение ID текстуры и списка подтекстур
//(ENG) Texture class, storage of texture ID and list of subtextures

namespace Renderer {
	class Texture2D {
	public:

		Texture2D(const GLuint& width, const GLuint& height, const unsigned char* data, std::shared_ptr<ShaderProgram> shader, const unsigned& channels = 4, const GLenum& filter = GL_LINEAR, const GLenum& wrapMode = GL_CLAMP_TO_EDGE);
		Texture2D& operator=(Texture2D&& texture2d) noexcept;
		Texture2D(Texture2D&& texture2d)noexcept;
		~Texture2D();

		unsigned int getWidth()const{return m_widht;}
		unsigned int getHeight()const{return m_height;}
		uint8_t getShaderSettings() { return m_shader->getSettings(); }
		std::shared_ptr<ShaderProgram> getShader() { return m_shader; }

		void bind()const;

	private:
		GLuint m_ID; 
		GLenum m_mode; 
		unsigned int m_widht; 
		unsigned int m_height; 
		std::shared_ptr<ShaderProgram> m_shader;
	};
}