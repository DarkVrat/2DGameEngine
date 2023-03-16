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
    Sprite::Sprite(const std::shared_ptr<Texture2D>& pTexture, const glm::vec4& coordTexture, const glm::ivec2& aspectRatio):m_texture(pTexture), m_coordTexture(coordTexture), m_aspectRatio(aspectRatio) {}     

    void Sprite::render(const glm::vec3& position, const float& height, const float& width, const float& rotation, const glm::vec2& origin){
        glm::vec3 PositionSprite = glm::vec3(position.x * m_window.x, position.y * m_window.y, position.z);
        glm::vec2 SizeSprite;
        if (width < 0.000001f && width>-0.000001f) {
            SizeSprite = glm::vec2(height * m_aspectRatio.x * m_window.y / m_aspectRatio.y, height * m_window.y);
        }
        else {
            if (height < 0.000001f && height>-0.000001f) {
                SizeSprite = glm::vec2(width * m_window.x, width * m_aspectRatio.y * m_window.x / m_aspectRatio.x);
            }
            else {
                SizeSprite = glm::vec2(width * m_window.x, height * m_window.y);
            }
        }
        render(PositionSprite, SizeSprite, rotation, origin);
    }

    //(RUS) Отрисовка спрайта
    //(ENG) Drawing a sprite
    void Sprite::render(const glm::vec3& position, const glm::vec2& size, const float& rotation, const glm::vec2& origin){
        glm::mat4 model(1.f);
         
        model = glm::translate(model, glm::vec3(position.x, position.y, 0.f));
        //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
        model = glm::translate(model, glm::vec3(-origin.x * size.x, -origin.y * size.y, 0.f));
        model = glm::scale(model, glm::vec3(size, position.z));

       

        RENDER_ENGINE::draw(m_texture, m_coordTexture, model);
    }
}