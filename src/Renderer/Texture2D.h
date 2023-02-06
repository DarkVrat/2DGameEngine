#pragma once

#include <glad/glad.h>
#include <string>
#include <map>
#include <glm/mat2x2.hpp>

namespace Renderer {
	class Texture2D {
	public:

		//Структура сабтекстур с информацией о левой нижней координате, и правой верхней.
		struct SubTexture2D {
			glm::vec2 leftBottomUV;
			glm::vec2 rightTopUV;

			//конструктор получает либо часть текстуры, либо устанавливает всю текстуру
			SubTexture2D(const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV):leftBottomUV(_leftBottomUV), rightTopUV(_rightTopUV) {}
			SubTexture2D():leftBottomUV(0.f), rightTopUV(1.f){}
		};

		//Конструкторы и деструктор
		Texture2D(const GLuint width, const GLuint height, const unsigned char* data, const unsigned int channels = 4, const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);
		Texture2D& operator=(Texture2D&& texture2d) noexcept;
		Texture2D(Texture2D&& texture2d)noexcept;
		~Texture2D();

		unsigned int getWidth()const{return m_widht;}
		unsigned int getHeight()const{return m_height;}

		void bind()const; //установка таргета на текстуру
		void addSubTexture(std::string name, const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV); //добавление подтекстуры
		const SubTexture2D& getSubTexture2D(const std::string& name) const; //получение подтекстуры

	private:
		GLuint m_ID; //ID текущей текстуры
		GLenum m_mode; //Мод отображения, RGB или RGBA
		unsigned int m_widht; //ширина
		unsigned int m_height; //высота
		std::map<std::string, SubTexture2D> m_subTexture; //map подтекстур
	};
}