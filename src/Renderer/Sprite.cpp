#include "Sprite.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "RenderEngine.h"

namespace Renderer {
     //(RUS) создание спрайта, заполнение буферов
    //(ENG) creating a sprite, filling buffers
    Sprite::Sprite(const std::shared_ptr<Texture2D>& pTexture, const glm::vec4& coordTexture):m_texture(pTexture), m_coordTexture(coordTexture) {}

     //(RUS) Отрисовка спрайта
    //(ENG) Drawing a sprite
    void Sprite::render(const glm::vec3& position, const glm::vec2& size, const float& rotation)
    {
        glm::mat4 model(1.f);
         
        model = glm::translate(model, glm::vec3(position.x, position.y, 0.f));
        //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f));
        model = glm::scale(model, glm::vec3(size, position.z));

       

        RENDER_ENGINE::draw(m_texture, m_coordTexture, model);
    }
}