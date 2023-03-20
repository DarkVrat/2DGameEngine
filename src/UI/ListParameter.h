#pragma once

#include <vector>
#include "Button.h"
#include "../Renderer/PrintText.h"

namespace UserInterface {
	template<class T>
	class ListParameter {
	public:
		ListParameter(glm::vec3 position, glm::vec2 size, GLfloat scaleText, std::vector<T> vectorParam, glm::vec2 origin=glm::vec2(0.5,0.5));
		ListParameter();
		~ListParameter();

		void create(glm::vec3 position, glm::vec2 size, GLfloat scaleText, std::vector<T> vectorParam, glm::vec2 origin = glm::vec2(0.5, 0.5));

		void render();
		void update();
		void checkClick();

		void setTypeToString(std::function<std::string(T value)> typeToStringFunction);

		void updateText();

		T getParametr();

	private:
		Button m_buttonLeft;
		Button m_buttonRight;

		std::shared_ptr<Renderer::Sprite> m_spriteBackGroung;
		glm::vec3 m_position;
		glm::vec3 m_positionSprite;
		glm::vec2 m_size;
		glm::vec2 m_sizeSprite;
		glm::vec2 m_sizeStandart;
		glm::vec2 m_origin;

		PRINT_TEXT::Text m_text;
		int m_index;
		float m_scaleText;
		std::vector<T> m_vectorParametrs;
		std::function<std::string(T value)> m_typeToString;
	};

	template<class T>
	inline ListParameter<T>::ListParameter(glm::vec3 position, glm::vec2 size, GLfloat scaleText, std::vector<T> vectorParam, glm::vec2 origin) {
		create(position, size, scaleText, vectorParam, origin);
	}

	template<class T>
	inline ListParameter<T>::ListParameter(){
		m_spriteBackGroung=nullptr;
		m_position = glm::vec3(0, 0, 0);
		m_size = glm::vec2(0, 0);
		m_text;
		m_index=0;
	}

	template<class T>
	inline ListParameter<T>::~ListParameter() {
		m_spriteBackGroung.~shared_ptr();
	}

	template<class T>
	inline void ListParameter<T>::create(glm::vec3 position, glm::vec2 size, GLfloat scaleText, std::vector<T> vectorParam, glm::vec2 origin) {
		m_index = 0;
		m_position = position;
		m_spriteBackGroung = RESOURCE_MANAGER::getSprite("Button_Off");
		m_text.ms_scale = scaleText * size.y;
		m_text.ms_color = glm::vec3(0, 0, 0);
		m_sizeStandart = size;
		m_origin = origin;

		m_scaleText = scaleText;

		m_buttonLeft.create(glm::vec3(0,0,0), glm::vec2(1, 1), 90, E_ARROW);
		m_buttonRight.create(glm::vec3(0,0,0), glm::vec2(1, 1), -90, E_ARROW);
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
		m_size = m_sizeStandart;

		if (m_size.x < 0.000001f && m_size.x>-0.000001f) {
			m_size.x = m_size.y * (ResourceManager::getSprite("Button_Off")->getRatio() + ResourceManager::getSprite("Arrow_Off")->getRatio() * 2);
		}
		if (m_size.y < 0.000001f && m_size.y>-0.000001f) {
			m_size.y = m_size.x / (ResourceManager::getSprite("Button_Off")->getRatio() + ResourceManager::getSprite("Arrow_Off")->getRatio() * 2);
		}

		m_buttonLeft.setSize(glm::vec2(0, m_size.y));
		m_buttonRight.setSize(glm::vec2(0, m_size.y));

		m_buttonLeft.setPosition(glm::vec3(m_position.x - m_size.x * m_origin.x + m_buttonLeft.getSize().x / 2, m_position.y + (0.5 - m_origin.y) * m_size.y, m_position.z));
		m_buttonRight.setPosition(glm::vec3(m_position.x + m_size.x * (1 - m_origin.x) - m_buttonRight.getSize().x / 2, m_position.y + (0.5 - m_origin.y) * m_size.y, m_position.z));
		
		m_sizeSprite = glm::vec2(m_size.x - m_buttonLeft.getSize().x * 2, m_size.y);
		m_positionSprite = glm::vec3(m_position.x + (0.5 - m_origin.x) * m_buttonLeft.getSize().x * 2, m_position.y, m_position.z);

		if (!m_vectorParametrs.empty()) {
			updateText();
		}
	}

	template<class T>
	inline void ListParameter<T>::checkClick() {
		m_buttonLeft.checkClick();
		m_buttonRight.checkClick();
	}

	template<class T>
	inline void ListParameter<T>::setTypeToString(std::function<std::string(T value)> typeToStringFunction) {
		m_typeToString = typeToStringFunction;
		updateText();
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
