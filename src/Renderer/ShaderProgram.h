#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

namespace Renderer {
	class ShaderProgram {
	public:
		//Конструкторы и деструкторы
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderProgram& operator=(ShaderProgram&& shaderProgram)noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram)noexcept;
		~ShaderProgram();

		//Проверка компиляции
		bool isCompiled();

		//передача данных и использование шейдера
		void use() const;

		void setInt(const std::string& name, const GLint value);
		void setFloat(const std::string& name, const GLfloat value);
		void setMatrix4(const std::string& name, const glm::mat4& matrix);
		void setVec3(const std::string& name, const glm::vec3 vec3);

	private:
		GLuint m_ID = 0; //ID шейдера
		bool m_isCompiled = false; //Статус

		//Компиляция шейдеров
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID); 
	};
}