#pragma once

#include <glad/glad.h>
#include <string>
#include <map>
#include <glm/mat2x2.hpp>

 //(RUS) Класс текстуры, хранение ID текстуры и списка подтекстур
//(ENG) Texture class, storage of texture ID and list of subtextures

namespace Renderer {
	class Texture2D {
	public:

		 //(RUS) Структура саб текстур, имеет координаты подтекстуры
		//(ENG) Sub texture structure, has sub texture coordinates

		struct SubTexture2D {
			glm::vec2 leftBottomUV;
			glm::vec2 rightTopUV;

			SubTexture2D(const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV):leftBottomUV(_leftBottomUV), rightTopUV(_rightTopUV) {}
			SubTexture2D():leftBottomUV(0.f), rightTopUV(1.f){}
		};

		Texture2D(const GLuint width, const GLuint height, const unsigned char* data, const unsigned int channels = 4, const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);
		Texture2D& operator=(Texture2D&& texture2d) noexcept;
		Texture2D(Texture2D&& texture2d)noexcept;
		~Texture2D();

		unsigned int getWidth()const{return m_widht;}
		unsigned int getHeight()const{return m_height;}

		void bind()const;
		void addSubTexture(std::string name, const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV);
		const SubTexture2D& getSubTexture2D(const std::string& name) const;

	private:
		GLuint m_ID; 
		GLenum m_mode; 
		unsigned int m_widht; 
		unsigned int m_height; 
		std::map<std::string, SubTexture2D> m_subTexture; 
	};
}