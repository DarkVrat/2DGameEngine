#include "SwitchBool.h"

namespace UserInterface {
	SwitchBool::SwitchBool(glm::vec3 position, glm::vec2 size, bool startFlag, glm::vec2 origin){
		create(position, size, startFlag);
	}

	SwitchBool::SwitchBool(){
		m_flag = false;
		m_button.create(glm::vec3(0,0,0), glm::vec2(0,0), E_NONE);
	}

	SwitchBool::~SwitchBool(){
		m_button.~Button();
	}

	void SwitchBool::create(glm::vec3 position, glm::vec2 size, bool startFlag, glm::vec2 origin){
		m_flag = startFlag;
		if (startFlag) {
			m_button.create(position, size, 0,E_TRUE, origin);
		}
		else {
			m_button.create(position, size, 0, E_FALSE, origin);
		}

		m_button.setCallBack([&]() {
			m_flag = !m_flag;
			if (m_flag) {
				m_button.setType(E_TRUE);
			}
			else {
				m_button.setType(E_FALSE);
			}
			});
	}

	void SwitchBool::render(){
		m_button.render();
	}

	void SwitchBool::update(){
		m_button.update();
	}

	void SwitchBool::checkClick() {
		if (m_button.checkClick())
			m_callback(m_flag);
	}

	void SwitchBool::setCallBack(std::function<void(bool flag)> callBack){
		m_callback = callBack;
	}


}