#include "PrintText.h"

#include "ft2build.h"
#include <GLFW/glfw3.h>
#include "../Managers/ResourceManager.h"
#include FT_FREETYPE_H

std::vector<uint8_t> Renderer::PrintText::m_advanceChar;
std::shared_ptr<Renderer::Texture2D> Renderer::PrintText::m_texture;
std::vector<std::pair<Renderer::PrintText::Text, double>> Renderer::PrintText::m_timeBufferText;
std::vector<Renderer::PrintText::Text> Renderer::PrintText::m_bufferText;
std::shared_ptr<Renderer::ShaderProgram> Renderer::PrintText::m_shader;
Renderer::VertexArray* Renderer::PrintText::m_vertexArray;
Renderer::VertexBuffer Renderer::PrintText::m_vertexBuffer;
Renderer::IndexBuffer Renderer::PrintText::m_indexBuffer;
int Renderer::PrintText::m_fontSize=32;

static double m_duration=0;
static double shaderset=0;
static double rendertext = 0;

namespace Renderer {
     //(RUS) Создание и заполнение начальными параметрами
    //(ENG) Creation and filling with initial parameters
    PrintText::PrintText() {
        m_advanceChar.clear();
        m_timeBufferText.clear();
        m_vertexArray=nullptr;
        m_texture = nullptr;
        m_shader = nullptr;
    }

     //(RUS) Создание символов, и установка параметров
    //(ENG) Creating Symbols and Setting Parameters
    void PrintText::init(const std::shared_ptr<ShaderProgram>& shader, const std::string& fontPath, const std::shared_ptr<Texture2D>& texture) {
        m_shader=shader;
        m_texture = texture;

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
            std::cerr << "(!) ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

        FT_Face face;
        if (FT_New_Face(ft, (RESOURCE_MANAGER::getExecutablePath() + fontPath).c_str(), 0, &face))
            std::cerr << "(!) ERROR::FREETYPE: Failed to load font" << std::endl;

        FT_Set_Pixel_Sizes(face, 0, m_fontSize);

        m_advanceChar.reserve(255);

        for (unsigned int c=0; c<256; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cerr << "(!) ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            m_advanceChar.push_back(face->glyph->advance.x >> 6);
        }
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

    void PrintText::printTextWrappingTime( Text text, const int& size, const bool& centr, const double& Time) {
        std::map<int, int> words;
        int sizeWord = 0;
        int sizeSpace = m_advanceChar[(uint8_t)' '] * text.ms_scale / 32.0;
        for (int i = 0; i < text.ms_text.length(); i++) {
            if (text.ms_text[i] != ' ') {
                sizeWord += m_advanceChar[(uint8_t)text.ms_text[i]] * text.ms_scale / 32.0;
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

        int sizeText = 0;
        int lastIndex = 0;
        int lastTextRender = 0;
        for (auto& word : words) {
            if (sizeText + word.second > size && sizeText != 0) {
                if (!centr) {
                    printTextTime(Text( text.ms_text.substr(lastTextRender, lastIndex - lastTextRender), 
                                        text.ms_position, 
                                        text.ms_scale, 
                                        text.ms_color), 
                                  Time);
                }
                else { 
                    printTextTime(Text( text.ms_text.substr(lastTextRender, lastIndex - lastTextRender), 
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
            printTextTime(Text(text.ms_text.substr(lastTextRender, lastIndex - lastTextRender),
                text.ms_position,
                text.ms_scale,
                text.ms_color),
                Time);
        }
        else {
            printTextTime(Text(text.ms_text.substr(lastTextRender, lastIndex - lastTextRender),
                glm::vec3(text.ms_position.x + (size - sizeText) / 2, text.ms_position.y, text.ms_position.z),
                text.ms_scale,
                text.ms_color),
                Time);
        }
    }

    void PrintText::printTextWrapping(Text text, const int& size, const bool& centr) {
        std::map<int, int> words;
        int sizeWord = 0;
        int sizeSpace = m_advanceChar[(uint8_t)' '] * text.ms_scale / 32.0;
        for (int i = 0; i < text.ms_text.length(); i++) {
            if (text.ms_text[i] != ' ') {
                sizeWord += m_advanceChar[(uint8_t)text.ms_text[i]] * text.ms_scale / 32.0;
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

        int sizeText = 0;
        int lastIndex = 0;
        int lastTextRender = 0;
        for (auto& word : words) {
            if (sizeText + word.second > size && sizeText != 0) {
                if (!centr) {
                    printText(Text(text.ms_text.substr(lastTextRender, lastIndex - lastTextRender),
                                   text.ms_position,
                                   text.ms_scale,
                                   text.ms_color));
                }
                else {
                    printText(Text(text.ms_text.substr(lastTextRender, lastIndex - lastTextRender),
                                   glm::vec3(text.ms_position.x + (size - sizeText) / 2, text.ms_position.y, text.ms_position.z),
                                   text.ms_scale,
                                   text.ms_color));
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
            printText(Text(text.ms_text.substr(lastTextRender, lastIndex - lastTextRender),
                text.ms_position,
                text.ms_scale,
                text.ms_color));
        }
        else {
            printText(Text(text.ms_text.substr(lastTextRender, lastIndex - lastTextRender),
                glm::vec3(text.ms_position.x + (size - sizeText) / 2, text.ms_position.y, text.ms_position.z),
                text.ms_scale,
                text.ms_color));
        }
    }

     //(RUS) Добавление текста в буфер на какое то время
    //(ENG) Adding text to the buffer for a while
    void PrintText::printTextTime(const Text& text, const double& Time) {
        std::vector<std::pair<Text, double>>::iterator It;
        for (It = m_timeBufferText.begin(); It != m_timeBufferText.end(); It++) {
            if (It->first.ms_text == text.ms_text && It->first.ms_position == text.ms_position) {
                It->second = Time;
                return;
            }
        }
        m_timeBufferText.push_back(std::make_pair<>(text,Time));
    }

     //(RUS) Добавление текста в буфер на определённое количество отрисовок
    //(ENG) Adding text to the buffer for a certain number of renders
    void PrintText::printText(const Text& text){
        m_bufferText.push_back(text);
    }

     //(RUS) Отрисовка текстов из буферов, и возможно удаление из Count буфера
    //(ENG) Drawing texts from buffers, and possibly deleting from the Count buffer
    void PrintText::renderBuffer(){
        GLint scale = 32;
        GLfloat sizeText = m_fontSize / 512.0;
        GLfloat size = scale;

        m_shader->use();

        glActiveTexture(GL_TEXTURE0);
        m_vertexArray->bind();
        m_indexBuffer.bind();
        m_texture->bind();

        for (Text t : m_bufferText) {

            m_shader->setVec3("textColor", t.ms_color);
            m_shader->setFloat("layer", t.ms_position.z);
            size = scale;

            glm::vec2 position(t.ms_position.x, t.ms_position.y);

            for (char c : t.ms_text) {
                uint8_t ch = c;
                glm::vec2 texture(sizeText * (ch % 16), sizeText * (ch / 16));
                GLfloat vertices[4][4] = {
                    { position.x,        position.y + size,   texture.x,            texture.y },
                    { position.x,        position.y,          texture.x,            texture.y + sizeText },
                    { position.x + size, position.y,          texture.x + sizeText, texture.y + sizeText },
                    { position.x + size, position.y + size,   texture.x + sizeText, texture.y }
                };
                m_vertexBuffer.update(vertices, sizeof(vertices));
                m_vertexBuffer.unbind();
                glDrawElements(GL_TRIANGLES, m_indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
                position.x += m_advanceChar[ch] * size / (float)m_fontSize;
            }
        }
        m_vertexArray->unbind();

        glBindTexture(GL_TEXTURE_2D, 0);

        m_bufferText.clear();
    }

     //(RUS) обновление Time буфера, возможно удаление
    //(ENG) updating Time buffer, possibly deleting
    void PrintText::updateBuffer(const double& duration){
        for (int i = m_timeBufferText.size() - 1; i >= 0; i--) {
            m_timeBufferText[i].second -= duration;
            if (m_timeBufferText[i].second < 0.0)
                m_timeBufferText.erase(m_timeBufferText.begin() + i);
            else
                m_bufferText.push_back(m_timeBufferText[i].first);
        }
        m_duration += duration;
    }

     //(RUS) Очистка и удаление
    //(ENG) Cleanup and removal
    void PrintText::terminate(){
        m_timeBufferText.clear();
        m_advanceChar.clear();
        m_shader.~shared_ptr();
        m_texture.~shared_ptr();
        delete m_vertexArray;
    }
}
