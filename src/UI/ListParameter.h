#pragma once

#include <vector>
#include <cmath>
#include "Button.h"
#include "../Renderer/PrintText.h"

namespace UserInterface {
	template<class T>
	class ListParameter : public UIElement{
	public:
		ListParameter(const glm::vec3& position, const glm::vec2& size, const GLfloat& scaleText, const std::vector<T>& vectorParam, const glm::vec2& origin=glm::vec2(0.5,0.5));
		ListParameter();
		~ListParameter();

		void create(const glm::vec3& position, const glm::vec2& size, const GLfloat& scaleText, const std::vector<T>& vectorParam, const glm::vec2& origin = glm::vec2(0.5, 0.5));

		void render() override;
		void update() override;
		bool checkClick() override;

		void setTypeToString(std::function<std::string(T value)> typeToStringFunction);
		void setColorText(const glm::vec3& color);

		void updateText();

		T getParametr();

	private:
		Button m_buttonLeft;
		Button m_buttonRight;

		std::shared_ptr<Renderer::Sprite> m_spriteBackGroung;
		glm::vec3 m_position;
		glm::vec3 m_positionSprite;
		glm::vec2 m_sizeSprite;
		glm::vec2 m_size;
		glm::vec2 m_origin;

		PRINT_TEXT::Text m_text;
		int m_index;
		float m_scaleText;
		std::vector<T> m_vectorParametrs;
		std::function<std::string(T value)> m_typeToString;
	};

	template<class T>
	inline ListParameter<T>::ListParameter(const glm::vec3& position, const glm::vec2& size, const GLfloat& scaleText, const std::vector<T>& vectorParam, const glm::vec2& origin) {
		create(position, size, scaleText, vectorParam, origin);
	}

	template<class T>
	inline ListParameter<T>::ListParameter(){
		m_spriteBackGroung=nullptr;
		m_position=glm::vec3(0,0,0);
		m_positionSprite= glm::vec3(0, 0, 0);
		m_sizeSprite= glm::vec2(0, 0);
		m_size= glm::vec2(0, 0);
		m_origin = glm::vec2(0, 0);
		m_text;
		m_index=0;
		m_scaleText=0.f;
	}

	template<class T>
	inline ListParameter<T>::~ListParameter() {
		m_spriteBackGroung.~shared_ptr();
	}

	template<class T>
	inline void ListParameter<T>::create(const glm::vec3& position, const glm::vec2& size, const GLfloat& scaleText, const std::vector<T>& vectorParam, const glm::vec2& origin) {
		m_index = 0;
		m_position = position;
		m_spriteBackGroung = RESOURCE_MANAGER::getSprite("Button_Off");
		m_text.ms_scale = scaleText * size.y;
		m_text.ms_color = glm::vec3(0, 0, 0);
		m_size = size;
		m_origin = origin;

		m_scaleText = scaleText;

		m_buttonLeft.create(glm::vec3(0,0,0), glm::vec2(0, 0), 90, E_ARROW);
		m_buttonRight.create(glm::vec3(0,0,0), glm::vec2(0, 0), -90, E_ARROW);
		update();
		
		m_vectorParametrs = vectorParam;

		m_buttonLeft.setCallBack([&]() {
			m_index--;
			if (m_index < 0)
				m_index = 0;
			updateText();
			});
		m_buttonRight.setCallBack([&]() {
			m_index++;
			if (m_index >= m_vectorParametrs.size())
				m_index = m_vectorParametrs.size() - 1;
			updateText();
			});
	}

	template<class T>
	inline void ListParameter<T>::render() {
		m_buttonLeft.render();
		m_buttonRight.render();
		m_spriteBackGroung->render(m_positionSprite, m_sizeSprite, 0.f, m_origin);
		PRINT_TEXT::printText(m_text);
	}

	template<class T>
	inline void ListParameter<T>::update() {
		glm::vec2 bufSize = m_size;

		if (std::abs(bufSize.x) < 0.000001f) {
			bufSize.x = bufSize.y * (ResourceManager::getSprite("Button_Off")->getRatio() + ResourceManager::getSprite("Arrow_Off")->getRatio() * 2);
		}
		if (std::abs(bufSize.y) < 0.000001f) {
			bufSize.y = bufSize.x / (ResourceManager::getSprite("Button_Off")->getRatio() + ResourceManager::getSprite("Arrow_Off")->getRatio() * 2);
		}

		m_buttonLeft.setSize(glm::vec2(0, bufSize.y));
		m_buttonRight.setSize(glm::vec2(0, bufSize.y));

		m_buttonLeft.setPosition(glm::vec3(	m_position.x - bufSize.x * m_origin.x + m_buttonLeft.getSize().x / 2,
											m_position.y + (0.5 - m_origin.y) * bufSize.y,
											m_position.z));
		m_buttonRight.setPosition(glm::vec3(m_position.x + bufSize.x * (1 - m_origin.x) - m_buttonRight.getSize().x / 2,
											m_position.y + (0.5 - m_origin.y) * bufSize.y,
											m_position.z));
		
		m_sizeSprite = glm::vec2(bufSize.x - m_buttonLeft.getSize().x * 2, bufSize.y);
		m_positionSprite = glm::vec3(m_position.x + (0.5 - m_origin.x) * m_buttonLeft.getSize().x * 2, 
									 m_position.y, 
									 m_position.z);

		if (!m_vectorParametrs.empty()) {
			updateText();
		}
	}

	template<class T>
	inline bool ListParameter<T>::checkClick() {
		if (m_buttonLeft.checkClick() || m_buttonRight.checkClick()) {
			return true;
		}
		return false;
	}

	template<class T>
	inline void ListParameter<T>::setTypeToString(std::function<std::string(T value)> typeToStringFunction) {
		m_typeToString = typeToStringFunction;
		updateText();
	}

	template<class T>
	inline void ListParameter<T>::setColorText(const glm::vec3& color){
		m_text.ms_color = color;
	}
	 
	template<class T>
	inline void ListParameter<T>::updateText() {
		m_text.ms_text = m_typeToString(m_vectorParametrs[m_index]);

		m_text.ms_scale = m_sizeSprite.y * m_scaleText;

		if (Renderer::PrintText::sizeText(m_text.ms_text, m_text.ms_scale) > m_sizeSprite.x * 0.9) {
			m_text.ms_scale = (m_sizeSprite.x * 0.9 * m_text.ms_scale) / Renderer::PrintText::sizeText(m_text.ms_text, m_text.ms_scale);
		}

		glm::vec3 posText(0, 0, 0);
		posText.z = m_positionSprite.z + 0.1f;
		posText.y = m_positionSprite.y + (0.5 - m_origin.y) * m_sizeSprite.y - m_text.ms_scale / 2.0;
		posText.x = m_positionSprite.x + (0.5 - m_origin.x) * m_sizeSprite.x - Renderer::PrintText::sizeText(m_text.ms_text, m_text.ms_scale) / 2.0;
		m_text.ms_position = posText;
	}

	template<class T>
	inline T ListParameter<T>::getParametr() {
		return m_vectorParametrs[m_index];
	}
}
