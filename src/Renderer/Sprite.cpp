#include "Sprite.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "RenderEngine.h"

namespace Renderer {
     //(RUS) �������� �������, ���������� �������
    //(ENG) creating a sprite, filling buffers
    Sprite::Sprite(std::shared_ptr<Texture2D> pTexture,
        std::string initialSubTexture,
        std::shared_ptr<ShaderProgram> pShaderProgram)
        : m_texture(std::move(pTexture))
        , m_shaderProgram(std::move(pShaderProgram))
    {
        const GLfloat vertexCoords[] = {
            // X  Y     //1---2
            0.f, 0.f,   //|  /|
            0.f, 1.f,   //| / |
            1.f, 1.f,   //|/  |
            1.f, 0.f    //0---3
        };

        auto& subTexture = m_texture->getSubTexture2D(std::move(initialSubTexture));

        const GLfloat textureCoords[] = {
            // U                       //V
            subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
            subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
            subTexture.rightTopUV.x,   subTexture.rightTopUV.y,
            subTexture.rightTopUV.x,   subTexture.leftBottomUV.y,
        };

        const GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_vertexCoordsBuffer.init(vertexCoords, 8 * sizeof(GLfloat));
        VertexBufferLayout vertexCoordsLayout;
        vertexCoordsLayout.addElementLayoutFloat(2, false);
        m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

        m_textureCoordsBuffer.init(textureCoords, 8 * sizeof(GLfloat));
        VertexBufferLayout textureCoordsLayout;
        textureCoordsLayout.addElementLayoutFloat(2, false);
        m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

        m_indexBuffer.init(indices, 6);

        m_vertexArray.unbind();
        m_indexBuffer.unbind();
    }

     //(RUS) ��������� �������
    //(ENG) Drawing a sprite
    void Sprite::render(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer) const
    {
        m_shaderProgram->use();

        glm::mat4 model(1.f);

        model = glm::translate(model, glm::vec3(position, 0.f));
        //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f));
        model = glm::scale(model, glm::vec3(size, 1.f));

        m_shaderProgram->setMatrix4("modelMat", model);
        m_shaderProgram->setFloat("layer", layer);

        glActiveTexture(GL_TEXTURE0);
        m_texture->bind();

        RENDER_ENGINE::draw(m_vertexArray, m_indexBuffer);
    }
}