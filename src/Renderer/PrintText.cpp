#include "PrintText.h"

#include "ft2build.h"
#include <GLFW/glfw3.h>
#include "../Managers/ResourceManager.h"
#include FT_FREETYPE_H

std::map<GLchar, std::shared_ptr<Renderer::PrintText::Character>> Renderer::PrintText::m_characters;
std::vector<std::pair<Renderer::PrintText::Text, double>> Renderer::PrintText::m_timeBufferText;
std::vector<std::pair<Renderer::PrintText::Text, int>> Renderer::PrintText::m_countBufferText;
std::shared_ptr<Renderer::ShaderProgram> Renderer::PrintText::m_shader;
Renderer::VertexArray* Renderer::PrintText::m_vertexArray;
Renderer::VertexBuffer Renderer::PrintText::m_vertexBuffer;
Renderer::IndexBuffer Renderer::PrintText::m_indexBuffer;
int Renderer::PrintText::m_fontSize;

namespace Renderer {
    PrintText::PrintText() {
        m_shader = nullptr;
        m_characters.clear();
        m_timeBufferText.clear();
        m_countBufferText.clear();
        m_vertexArray=nullptr;
        m_fontSize = 1;
    }

    void PrintText::createSymbols(std::shared_ptr<ShaderProgram> shader, int fontSize, std::string fontPath) {
        m_fontSize = fontSize;
        m_shader=std::move(shader);

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
            std::cerr << "(!) ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

        FT_Face face;
        if (FT_New_Face(ft, (RESOURCE_MANAGER::getExecutablePath() + fontPath).c_str(), 0, &face))
            std::cerr << "(!) ERROR::FREETYPE: Failed to load font" << std::endl;

        FT_Set_Pixel_Sizes(face, 0, m_fontSize);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


        for (unsigned int c=32; c<256; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cerr << "(!) ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }

            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            std::shared_ptr<Character> character = std::make_shared<Character>( texture,
                                                                                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                                                                                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                                                                                static_cast<GLuint>(face->glyph->advance.x));

            m_characters.insert(std::pair<GLchar, std::shared_ptr<Character>>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        m_vertexArray = new VertexArray();

        VertexBufferLayout VBL;
        VBL.addElementLayoutFloat(4, false);
        m_vertexBuffer.init(NULL, sizeof(GLfloat) * 6 * 4);
        m_vertexArray->bind();
        m_vertexArray->addBuffer(m_vertexBuffer, VBL);
        m_vertexBuffer.unbind();
        m_vertexArray->unbind();

        const GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_indexBuffer.init(indices, 6);
    }

    void PrintText::renderText(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color) {
        m_shader->use();
        m_shader->setVec3("textColor", color);
        m_shader->setFloat("layer", position.z);

        glActiveTexture(GL_TEXTURE0);
        m_vertexArray->bind();
        m_indexBuffer.bind();

        for (char c: text)
        {
            std::shared_ptr<Character> ch = m_characters[c];

            GLfloat xpos = position.x + ch->ms_bearing.x * scale;
            GLfloat ypos = position.y - (ch->ms_size.y - ch->ms_bearing.y) * scale;

            GLfloat w = ch->ms_size.x * scale;
            GLfloat h = ch->ms_size.y * scale;
            GLfloat vertices[4][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
            };

            glBindTexture(GL_TEXTURE_2D, ch->ms_textureID);

            m_vertexBuffer.update(vertices, sizeof(vertices));
            m_vertexBuffer.unbind();

            glDrawElements(GL_TRIANGLES, m_indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
            position.x += (ch->ms_advance >> 6) * scale;
        }
        m_vertexArray->unbind();

        glBindTexture(GL_TEXTURE_2D, 0);
	}

    void PrintText::addTextInTimeBuffer(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color, double Time){
        std::vector<std::pair<Text, double>>::iterator It;
        for (It = m_timeBufferText.begin(); It != m_timeBufferText.end(); It++) {
            if (It->first.ms_text == text && It->first.ms_position == position) {
                It->second = Time;
                return;
            }
        }
        Text textInBuffer = {
            text,
            position,
            scale,
            color,
        };
        m_timeBufferText.emplace_back(std::make_pair<>(textInBuffer,Time));
    }

    void PrintText::addTextInCountBuffer(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color, int Count){
        std::vector<std::pair<Text, int>>::iterator It;
        for (It = m_countBufferText.begin(); It != m_countBufferText.end(); It++) {
            if (It->first.ms_text == text && It->first.ms_position == position) {
                It->second = Count;
                return;
            }
        }
        Text textInBuffer = {
            text,
            position,
            scale,
            color,
        };
        m_countBufferText.emplace_back(std::make_pair<>(textInBuffer, Count));
    }

    void PrintText::renderBuffer(){
        for (auto& It:m_timeBufferText) {
            renderText(It.first.ms_text, It.first.ms_position, It.first.ms_scale, It.first.ms_color);
        }
        for (int i = m_countBufferText.size()-1; i >= 0; i--) {
            Text t = m_countBufferText.at(i).first;
            renderText(t.ms_text, t.ms_position, t.ms_scale, t.ms_color);
            m_countBufferText.at(i).second--;
            if (m_countBufferText.at(i).second < 1)
                m_countBufferText.erase(m_countBufferText.begin() + i);
        }
    }

    void PrintText::updateBuffer(double duration){
        for (int i = m_timeBufferText.size() - 1; i >= 0; i--) {
            m_timeBufferText.at(i).second -= duration;
            if (m_timeBufferText.at(i).second < 0.0)
                m_timeBufferText.erase(m_timeBufferText.begin()+i);
        }
    }

    PrintText::~PrintText(){
        m_characters.clear();
    }
}
