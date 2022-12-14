#include "PrintText.h"
#include "ft2build.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include FT_FREETYPE_H

std::shared_ptr<Renderer::ShaderProgram> Renderer::PrintText::m_shader;
std::map<GLchar, Renderer::PrintText::Character> Renderer::PrintText::m_Characters;
std::vector<std::pair<Renderer::PrintText::Text, double>> Renderer::PrintText::m_timeBufferText;
std::vector<std::pair<Renderer::PrintText::Text, int>> Renderer::PrintText::m_countBufferText;
GLuint Renderer::PrintText::m_VAO, Renderer::PrintText::m_VBO;

namespace Renderer {
    void PrintText::init(std::shared_ptr<ShaderProgram> shader) {
        m_shader=std::move(shader);

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
            std::cerr << "(!) ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

        FT_Face face;
        if (FT_New_Face(ft, "../res/Tkachevica.ttf", 0, &face))
            std::cerr << "(!) ERROR::FREETYPE: Failed to load font" << std::endl;

        FT_Set_Pixel_Sizes(face, 0, 96);

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

            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<GLuint>(face->glyph->advance.x)
            };

            m_Characters.insert(std::pair<GLchar, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        FT_Done_Face(face);
        FT_Done_FreeType(ft);


        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void PrintText::RenderText(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color) {
        m_shader->use();
        m_shader->setVec3("textColor", color);
        m_shader->setFloat("layer", position.z);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(m_VAO);

        for (char c: text)
        {
            Character ch = m_Characters[c];

            GLfloat xpos = position.x + ch.Bearing.x * scale;
            GLfloat ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
            };
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            position.x += (ch.Advance >> 6) * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
	}

    void PrintText::AddTextInTimeBuffer(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color, double Time){
        std::vector<std::pair<Text, double>>::iterator It;
        for (It = m_timeBufferText.begin(); It != m_timeBufferText.end(); It++) {
            if (It->first.text == text && It->first.position == position) {
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

    void PrintText::AddTextInCountBuffer(std::string text, glm::vec3 position, GLfloat scale, glm::vec3 color, int Count){
        std::vector<std::pair<Text, int>>::iterator It;
        for (It = m_countBufferText.begin(); It != m_countBufferText.end(); It++) {
            if (It->first.text == text && It->first.position == position) {
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
        for (auto It:m_timeBufferText) {
            RenderText(It.first.text, It.first.position, It.first.scale, It.first.color);
        }
        for (int i = m_countBufferText.size()-1; i >= 0; i--) {
            Text t = m_countBufferText.at(i).first;
            RenderText(t.text, t.position, t.scale, t.color);
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
}
