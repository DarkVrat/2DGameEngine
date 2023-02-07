#include "Texture2D.h"

namespace Renderer {
	//передача в конструктор ширины и высоты (В пикселях), данных из PNG в виде массива символов, количества каналов в изображении, фильтр, и wrapMode
	Texture2D::Texture2D(const GLuint width, const GLuint height, const unsigned char* data, const unsigned int channels, const GLenum filter, const GLenum wrapMode)  :m_widht(width), m_height(height) {
		//Установка мода исходя из числа каналов
		switch (channels)
		{
		case 3:
			m_mode = GL_RGB;
			break;
		default:
			m_mode = GL_RGBA;
			break;
		}

		glGenTextures(1, &m_ID); //создание добавление 1 текстуры в массив, и передача её номера в m_ID 
		glActiveTexture(GL_TEXTURE0); //связнка на текстуры
		glBindTexture(GL_TEXTURE_2D, m_ID); //Привязка 2D текстур к данной по ID

		//текстурированя, для возможности чтения изображения шейдером, указан тип текстуры, уровень на MIP карте, формат, шир/выс, границы, формат, Тип пикселя, и изображение
		glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_widht, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data); 

		//Установка фильтров и wrap модов
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		//Генерация MIP карты
		glGenerateMipmap(GL_TEXTURE_2D);

		//Очистка привязки
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//предача текстуры с удалением
	Texture2D& Texture2D::operator=(Texture2D&& texture2d) noexcept {
		glDeleteTextures(1, &m_ID);
		m_ID = texture2d.m_ID;
		texture2d.m_ID = 0;
		m_mode = texture2d.m_mode;
		m_height = texture2d.m_height;
		m_widht = texture2d.m_widht;
		return *this;
	}

	//предача текстуры с удалением
	Texture2D::Texture2D(Texture2D&& texture2d) noexcept {
		m_ID = texture2d.m_ID;
		texture2d.m_ID = 0;
		m_mode = texture2d.m_mode;
		m_height = texture2d.m_height;
		m_widht = texture2d.m_widht;
	}
	
	//удаление текструы по её ID
	Texture2D::~Texture2D() {
		glDeleteTextures(1, &m_ID);
	}

	//установка таргета на текстуру
	void Texture2D::bind() const {
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	//добавление подтекстуры
	void Texture2D::addSubTexture(std::string name, const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV) {
		m_subTexture.emplace(std::move(name), SubTexture2D(_leftBottomUV, _rightTopUV));
	}

	//получение подтекстуры
	const Texture2D::SubTexture2D& Texture2D::getSubTexture2D(const std::string& name) const {
		auto it = m_subTexture.find(name); //поиск текстуры
		if (it != m_subTexture.end()) return it->second; //Получение подтекстуры если она нашлась
		const static SubTexture2D defaultSubTex;
		return defaultSubTex; //получение всей текстуры если не нашлась
	}
}