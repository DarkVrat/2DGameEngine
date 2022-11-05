#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/mat4x4.hpp>

namespace Renderer {
	class ShaderProgram {
	public:
		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		//Конструкторы и деструкторы
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderProgram& operator=(ShaderProgram&& shaderProgram)noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram)noexcept;
		~ShaderProgram();

		//Проверка компиляции
		bool isCompiled() const {return m_isCompiled;}

		//передача данных и использование шейдера
		void use() const;
		void setInt(const std::string& name, const GLint value);
		void setFloat(const std::string& name, const GLfloat value);
		void setMatrix4(const std::string& name, const glm::mat4& matrix);

	private:
		GLuint m_ID = 0; //ID шейдера
		bool m_isCompiled = false; //Статус

		//Компиляция шейдеров
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID); 
	};
}