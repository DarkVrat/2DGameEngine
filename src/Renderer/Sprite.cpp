#include "Sprite.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "RenderEngine.h"

glm::vec2 Renderer::Sprite::m_window;

namespace Renderer {
     //(RUS) создание спрайта, заполнение буферов
    //(ENG) creating a sprite, filling buffers
    Sprite::Sprite(const std::shared_ptr<Texture2D>& pTexture, const glm::vec4& coordTexture, const glm::ivec2& aspectRatio):m_texture(pTexture), m_coordTexture(coordTexture), m_aspectRatio(aspectRatio) {
        m_ratio = static_cast<float>(m_aspectRatio.x) / static_cast<float>(m_aspectRatio.y);
    }     

    //(RUS) Отрисовка спрайта
    //(ENG) Drawing a sprite
    void Sprite::render(const glm::vec3& position, const glm::vec2& size, const float& rotation, const glm::vec2& origin){
        glm::vec2 SizeSprite= glm::vec2(size.x * m_window.x, size.y * m_window.y);
        if (size.x < 0.000001f && size.x>-0.000001f) {
            SizeSprite.x = SizeSprite.y * m_ratio;
        }
        else if (size.y < 0.000001f && size.y>-0.000001f) {
            SizeSprite.y = SizeSprite.x / m_ratio;
        }

        glm::mat4 model(1.f);

        model = glm::translate(model, glm::vec3(position.x * m_window.x, position.y * m_window.y, 0.f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
        model = glm::translate(model, glm::vec3(-origin.x * SizeSprite.x, -origin.y * SizeSprite.y, 0.f));
        model = glm::scale(model, glm::vec3(SizeSprite, position.z));

        RENDER_ENGINE::draw(m_texture, m_coordTexture, model);
    }
}