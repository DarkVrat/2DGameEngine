#include "Texture2D.h"

namespace Renderer {
	 //(RUS) �������� ��������, ���������� ������ � ��������
	//(ENG) Creating a texture, filling in texture data
	Texture2D::Texture2D(const GLuint& width, const GLuint& height, const unsigned char* data, const unsigned& channels, const GLenum& filter, const GLenum& wrapMode)  
		:m_widht(width), m_height(height) {
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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	 //(RUS) ����������� �������� ��������
	//(ENG) Texture transfer constructor
	Texture2D& Texture2D::operator=(Texture2D&& texture2d) noexcept {
		glDeleteTextures(1, &m_ID);
		m_ID = texture2d.m_ID;
		texture2d.m_ID = 0;
		m_mode = texture2d.m_mode;
		m_height = texture2d.m_height;
		m_widht = texture2d.m_widht;
		return *this;
	}
	Texture2D::Texture2D(Texture2D&& texture2d) noexcept {
		m_ID = texture2d.m_ID;
		texture2d.m_ID = 0;
		m_mode = texture2d.m_mode;
		m_height = texture2d.m_height;
		m_widht = texture2d.m_widht;
	}
	
	 //(RUS) �������� ��������
	//(ENG) Removing texture
	Texture2D::~Texture2D() { glDeleteTextures(1, &m_ID); }

	 //(RUS) bind �������� ��� �������������
	//(ENG) bind texture to use
	void Texture2D::bind() const { glBindTexture(GL_TEXTURE_2D, m_ID); }

	 //(RUS) ���������� �����������
	//(ENG) Adding Subtexture
	void Texture2D::setSubTextureMap(const std::map<std::string, Renderer::Texture2D::SubTexture2D>& subTextures){
		m_subTexture=subTextures;
		for (auto& It : m_subTexture) {
			It.second.m_leftTopUV.x /= m_widht;
			It.second.m_leftTopUV.y /= m_height;
			It.second.m_rightBottomUV.x /= m_widht;
			It.second.m_rightBottomUV.y /= m_height;
		}
	}

	 //(RUS) ��������� �����������
	//(ENG) Getting a subtexture
	const Texture2D::SubTexture2D& Texture2D::getSubTexture2D(const std::string& name) const {
		auto it = m_subTexture.find(name); 
		if (it != m_subTexture.end()) return it->second; 
		const static SubTexture2D defaultSubTex;
		return defaultSubTex; 
	}
}