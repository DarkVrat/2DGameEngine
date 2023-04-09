#include "StateAnimationController.h"

 //(RUS) Создание, и инициализация первого состояния
//(ENG) Creation and initialization of the first state
StateAnimationController::StateAnimationController(const std::string& initState){
	m_nameState=initState;
	m_state = RESOURCE_MANAGER::getStateAnimation(m_nameState);
	m_bufferState = m_state->getNextStateName();
	m_sprites = m_state->getFrames();
	m_indexFrame = 0;
	m_durationFrame = m_sprites[m_indexFrame].second;
	m_time=0;
}

       //(RUS) Установка состояния анимации. 
      //       Если вызвана из - за  конца кадров в анимации, то либо начинается заново, либо переключится на ту что в буфере.
     //        Если вызвана до окончания анимации, проверяется можно ли перейти в nameState, 
    //         и если текущую нельзя прервать, то перехлдит в буфер, иначе переключается
   //(ENG) Set animation state.
  //       If called because of the end of frames in the animation, it will either start over or switch to the one in the buffer.
 //        If called before the end of the animation, it is checked whether it is possible to go to nameState, 
//         and if the current one cannot be interrupted, then it will go to the buffer, otherwise it switches
void StateAnimationController::setState(const std::string& nextState){
	if (m_indexFrame >= m_sprites.size()) {
		if (nextState == m_nameState) {
			m_indexFrame = 0;
			m_durationFrame = m_sprites[m_indexFrame].second;
		}
		else {
			std::shared_ptr<Renderer::StateAnimation> newState = RESOURCE_MANAGER::getStateAnimation(nextState);
			m_nameState = nextState;
			m_state = newState;
			m_bufferState = newState->getNextStateName();
			m_sprites = newState->getFrames();
			m_indexFrame = 0;
			m_durationFrame = m_sprites[m_indexFrame].second;
		}
	}
	else {
		if (nextState == m_nameState) return;
		std::shared_ptr<Renderer::StateAnimation> newState = RESOURCE_MANAGER::getStateAnimation(nextState);
		if (!newState->canChange(m_nameState)) { return; }
		if (m_state->getUninterrupted()) {
			m_bufferState = nextState;
			return;
		}
		m_nameState = nextState;
		m_state = newState;
		m_bufferState = newState->getNextStateName();
		m_sprites = newState->getFrames();
		m_indexFrame = 0;
		m_durationFrame = m_sprites[m_indexFrame].second;
	}
}

 //(RUS) Обновление спрайта, смена кадра
//(ENG) Sprite update, frame change
void StateAnimationController::update(const double& duration){
	m_time += duration;
	while (m_time > m_durationFrame) {
		m_time -= m_durationFrame;
		m_indexFrame++;
	
		if (m_indexFrame < m_sprites.size())m_durationFrame = m_sprites[m_indexFrame].second;
		else setState(m_bufferState);
	}
}

void StateAnimationController::renderUI(const glm::vec3& position, const glm::vec2& size, const float& rotation, const glm::vec2& origin){
	m_sprites[m_indexFrame].first->renderUI(position, size, rotation, origin);
}

 //(RUS) Рендеринг спрайта
//(ENG) Sprite rendering
void StateAnimationController::render(const glm::vec3& position, const glm::vec2& size, const float& rotation, const glm::vec2& origin){
	m_sprites[m_indexFrame].first->render(position, size, rotation, origin);
}
