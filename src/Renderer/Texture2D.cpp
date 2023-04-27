#include "Texture2D.h"

namespace Renderer {
	 //(RUS) —оздание текстуры, заполнение данных о текстуре
	//(ENG) Creating a texture, filling in texture data
	Texture2D::Texture2D(const GLuint& width, const GLuint& height, const unsigned char* data, std::shared_ptr<ShaderProgram> shader, const unsigned& channels, const GLenum& filter, const GLenum& wrapMode)
		:m_widht(width), m_height(height), m_shader(shader) {
		switch (channels){
		case 3:
			m_mode = GL_RGB;
			break;
		default:
			m_mode = GL_RGBA;
			break;
		}

		glGenTextures(1, &m_ID); 
		glActiveTexture(GL_TEXTURE0); 
		glBindTexture(GL_TEXTURE_2D, m_ID); 
		
		glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_widht, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data); 

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	 //(RUS)  онструктор переноса текстуры
	//(ENG) Texture transfer constructor
	Texture2D& Texture2D::operator=(Texture2D&& texture2d) noexcept {
		glDeleteTextures(1, &m_ID);
		m_ID = texture2d.m_ID;
		texture2d.m_ID = 0;
		m_mode = texture2d.m_mode;
		m_height = texture2d.m_height;
		m_widht = texture2d.m_widht;
		m_shader = texture2d.m_shader;
		return *this;
	}
	Texture2D::Texture2D(Texture2D&& texture2d) noexcept {
		m_ID = texture2d.m_ID;
		texture2d.m_ID = 0;
		m_mode = texture2d.m_mode;
		m_height = texture2d.m_height;
		m_widht = texture2d.m_widht;
		m_shader = texture2d.m_shader;
	}
	
	 //(RUS) ”даление текстуры
	//(ENG) Removing texture
	Texture2D::~Texture2D() { glDeleteTextures(1, &m_ID); }

	 //(RUS) bind текстуры дл€ использовани€
	//(ENG) bind texture to use
	void Texture2D::bind() const { 
		m_shader->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ID); 
	}
}