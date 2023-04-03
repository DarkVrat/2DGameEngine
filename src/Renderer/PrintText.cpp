#include "PrintText.h"

#include "ft2build.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Managers/ResourceManager.h"
#include "../Renderer/RenderEngine.h"
#include FT_FREETYPE_H

std::vector<float> Renderer::PrintText::m_advanceChar;
std::vector<glm::vec4> Renderer::PrintText::m_textureChar;
std::shared_ptr<Renderer::Texture2D> Renderer::PrintText::m_texture;
glm::vec2 Renderer::PrintText::m_window;

std::vector<std::pair<Renderer::PrintText::Text, double>> Renderer::PrintText::m_timeBufferText;
std::vector<Renderer::PrintText::Text> Renderer::PrintText::m_bufferText;

std::shared_ptr<Renderer::VertexArray> Renderer::PrintText::m_VAO;
Renderer::VertexBuffer Renderer::PrintText::m_PositionVBO;
Renderer::VertexBuffer Renderer::PrintText::m_ColorVBO;
Renderer::VertexBuffer Renderer::PrintText::m_TextureVBO;

float Renderer::PrintText::m_fontSize=32.f;

namespace Renderer {
     //(RUS) Создание и заполнение начальными параметрами
    //(ENG) Creation and filling with initial parameters
    PrintText::PrintText() {
        m_advanceChar.clear();
        m_timeBufferText.clear();
        m_texture = nullptr;
    } 

     //(RUS) Создание символов, и установка параметров
    //(ENG) Creating Symbols and Setting Parameters
    void PrintText::init(const std::string& fontPath, const std::shared_ptr<Texture2D>& texture) {
        m_texture = texture;

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
            std::cerr << "(!) ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

        FT_Face face;
        if (FT_New_Face(ft, (RESOURCE_MANAGER::getExecutablePath() +"/" + fontPath).c_str(), 0, &face))
            std::cerr << "(!) ERROR::FREETYPE: Failed to load font" << std::endl;

        FT_Set_Pixel_Sizes(face, 0, m_fontSize);

        m_advanceChar.reserve(255);

        float sizeTexture = 1.0 / 16.0;

        for (unsigned int c=0; c<256; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cerr << "(!) ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            m_advanceChar.push_back((face->glyph->advance.x >> 6)/ m_fontSize);

            m_textureChar.push_back(glm::vec4(  (c % 16) * sizeTexture + 0.0001, 
                                                (c / 16) * sizeTexture + 0.0001,
                                                (c % 16 + 1) * sizeTexture - 0.0001,
                                                (c / 16 + 1) * sizeTexture - 0.0001));
        }
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        float vertex[] = {
            0.f, 0.f,   // 0    1--2
            0.f, 1.f,   // 1    | /|
            1.f, 1.f,   // 2    |/ |
            1.f, 0.f,   // 3    0--3  
        };

        m_VAO = std::make_shared<VertexArray>();
        m_VAO->bind();
        
        VertexBuffer vertexVBO;
        vertexVBO.init(vertex, sizeof(vertex));
        m_VAO->addBuffer(vertexVBO, 0, 2);

        m_PositionVBO.init(NULL, NULL);
        m_VAO->addBuffer(m_PositionVBO, 1, 4, true);

        m_ColorVBO.init(NULL, NULL);
        m_VAO->addBuffer(m_ColorVBO, 2, 3, true);

        m_TextureVBO.init(NULL, NULL);
        m_VAO->addBuffer(m_TextureVBO, 3, 4, true);

        m_TextureVBO.unbind();
        m_VAO->unbind();
    }

     //(RUS) Создание текста с переносом, выравняным по левому краю или центру
    //(ENG) Create text with hyphenation aligned to the left or center
    void PrintText::printTextWrapping( Text text, float size, const bool& centr, const double& Time) {
        std::map<int, float> words;
        float sizeWord = 0;
        float sizeSpace = m_advanceChar[(uint8_t)' '] * text.ms_scale * (m_window.y / m_window.x);

        for (int i = 0; i < text.ms_text.length(); i++) {
            if (text.ms_text[i] != ' ') {
                sizeWord += m_advanceChar[(uint8_t)text.ms_text[i]] * text.ms_scale * (m_window.y / m_window.x);
            }
            else {
                words.emplace(i, sizeWord + sizeSpace);
                sizeWord = 0;
            }
            if (i == text.ms_text.length() - 1) {
                words.emplace(i + 1, sizeWord + sizeSpace);
                sizeWord = 0;
            }
        }

        float sizeText = 0;
        int lastIndex = 0;
        int lastTextRender = 0;

        for (auto& word : words) {
            if (sizeText + word.second > size && sizeText != 0) {
                if (!centr) {
                    printText(Text( text.ms_text.substr(lastTextRender, lastIndex - lastTextRender), 
                                    text.ms_position, 
                                    text.ms_scale, 
                                    text.ms_color), 
                              Time);
                }
                else { 
                    printText(Text( text.ms_text.substr(lastTextRender, lastIndex - lastTextRender), 
                                    glm::vec3(text.ms_position.x + (size - sizeText) / 2, text.ms_position.y, text.ms_position.z), 
                                    text.ms_scale, 
                                    text.ms_color),
                              Time);
                }
                text.ms_position.y -= text.ms_scale;
                lastTextRender = lastIndex + 1;
                sizeText = word.second;
            }
            else {
                sizeText += word.second;
            }
            lastIndex = word.first;
        }

        if (!centr) {
            printText(Text( text.ms_text.substr(lastTextRender, lastIndex - lastTextRender),
                            text.ms_position,
                            text.ms_scale,
                            text.ms_color),
                      Time);
        }
        else {
            printText(Text( text.ms_text.substr(lastTextRender, lastIndex - lastTextRender),
                            glm::vec3(text.ms_position.x + (size - sizeText) / 2, text.ms_position.y, text.ms_position.z),
                            text.ms_scale,
                            text.ms_color),
                     Time);
        }
    }

     //(RUS) Добавление текста в буфер для отрисовки, либо разово, либо на время
    //(ENG) Adding text to the rendering buffer, either one-time or temporarily
    void PrintText::printText(const Text& text, const double& Time) {
        if (Time < 0) {
            m_bufferText.push_back(text);
            return;
        }

        for (auto& It:m_timeBufferText) {
            if (It.first.ms_text == text.ms_text && It.first.ms_position == text.ms_position) {
                It.second = Time;
                return;
            }
        }

        m_timeBufferText.push_back(std::make_pair<>(text,Time));
    }

    //(RUS) обновление Time буфера, возможно удаление
   //(ENG) updating Time buffer, possibly deleting
    void PrintText::updateBuffer(const double& duration) {
        for (int i = m_timeBufferText.size() - 1; i >= 0; i--) {
            m_timeBufferText[i].second -= duration;
            if (m_timeBufferText[i].second < 0.0)
                m_timeBufferText.erase(m_timeBufferText.begin() + i);
            else
                m_bufferText.push_back(m_timeBufferText[i].first);
        }
    }

     //(RUS) Отрисовка текстов из буферов, и возможно удаление из Count буфера
    //(ENG) Drawing texts from buffers, and possibly deleting from the Count buffer
    void PrintText::renderBuffer(){
        unsigned countChar = 0;

        for (Text t : m_bufferText) {
            countChar += t.ms_text.length();
        }

        if (countChar == 0) {
            return;
        }

        std::vector<glm::vec4> Position;
        std::vector<glm::vec3> Color;
        std::vector<glm::vec4> Texture;
        Position.reserve(countChar);
        Color.reserve(countChar);
        Texture.reserve(countChar);

        for (Text t : m_bufferText) {
             
            float posX = t.ms_position.x;

            for (char c : t.ms_text) {
                uint8_t index= c;

                Position.push_back(glm::vec4(posX*m_window.x, t.ms_position.y*m_window.y, t.ms_position.z, t.ms_scale*m_window.y));
                Color.push_back(t.ms_color);
                Texture.push_back(m_textureChar.at(index));
                 
                posX += m_advanceChar[index] * t.ms_scale * (m_window.y / m_window.x);
            } 
        }

        m_texture->bind();

        m_PositionVBO.update(&Position[0], Position.size() * sizeof(glm::vec4));
        m_ColorVBO.update(&Color[0], Color.size() * sizeof(glm::vec3));
        m_TextureVBO.update(&Texture[0], Texture.size() * sizeof(glm::vec4));

        RENDER_ENGINE::drawInstanced(*m_VAO, countChar);

        m_VAO->unbind();
        m_TextureVBO.unbind();
        glBindTexture(GL_TEXTURE_2D, 0);
        
        m_bufferText.clear();
        Position.clear();
        Color.clear();
        Texture.clear();
    }

    float PrintText::sizeText(std::string text, GLfloat scale){
        float size = 0;
        for (char c : text) {
            size += m_advanceChar[(uint8_t)c] * scale * (m_window.y / m_window.x);
        }
        return size;
    }

     //(RUS) Очистка и удаление
    //(ENG) Cleanup and removal
    void PrintText::terminate(){
        m_timeBufferText.clear();
        m_advanceChar.clear();
        m_texture.~shared_ptr();
    }

    void PrintText::setWindow(const glm::vec2& window){
        m_window = window;
        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_window.x), 0.f, static_cast<float>(m_window.y), -100.f, 100.f);
        m_texture->getShader()->use();
        m_texture->getShader()->setMatrix4("projection", projectionMatrix);
    }
}
