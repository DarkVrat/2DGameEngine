#include "ShaderProgram.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace Renderer {
	 //(RUS) Конструктор создающий шейдер
	//(ENG) Constructor creating shader
	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader){
		GLuint vertexShaderID;
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)) {
			std::cerr << "(!) VERTEX SHADER Compile-time error" << std::endl;
			return;
		}

		GLuint fragmentShaderID;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)) {
			std::cerr << "(!) FRAGMENT SHADER Compile-time error" << std::endl;
			glDeleteShader(vertexShaderID);
			return;
		}

		m_ID = glCreateProgram();				
		glAttachShader(m_ID, vertexShaderID);	
		glAttachShader(m_ID, fragmentShaderID);	
		glLinkProgram(m_ID);					

		GLint success;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar infoLog[1024];
			glGetShaderInfoLog(m_ID, 1024, nullptr, infoLog);
			std::cerr << "(!) ERROR::SHADER: Limk-time error: \n" << infoLog << std::endl;
		}
		else m_isCompiled = true;

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

	}

	 //(RUS) Компиляция шейдера
	//(ENG) Shader compilation
	bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID) {
		
		shaderID = glCreateShader(shaderType);					
		const char* code = source.c_str();						
		glShaderSource(shaderID, 1, std::move(&code), nullptr); 
		glCompileShader(shaderID);								
		
		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success); 
		
		if (!success) {
			GLchar infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog); 
			std::cerr << "(!) ERROR::SHADER: Compile-time error: \n" << infoLog << std::endl; 
			return false;
		}
		return true;
	}

	 //(RUS) Удаление шейдера
    //(ENG) Removing a shader
	ShaderProgram::~ShaderProgram(){
		glDeleteProgram(m_ID);
	}

	 //(RUS) Получение флага компиляции
	//(ENG) Getting the compilation flag
	bool ShaderProgram::isCompiled() { return m_isCompiled; }

	 //(RUS) использование шейдера
	//(ENG) shader usage
	void ShaderProgram::use() const{
		glUseProgram(m_ID);
	}

	 //(RUS) Установка определённых параметров в Uniform  переменные шейдеров
	//(ENG) Setting certain parameters in uniform shader variables
	void ShaderProgram::setInt(const std::string& name, const GLint value) { 
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value); 
	}
	void ShaderProgram::setFloat (const std::string& name, const GLfloat value) { 
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value); 
	}
	void ShaderProgram::setMatrix4(const std::string& name, const glm::mat4& matrix) { 
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()),1,GL_FALSE, glm::value_ptr(matrix));	
	}
	void ShaderProgram::setVec3(const std::string& name, const glm::vec3 vec3) {
		glUniform3f(glGetUniformLocation(m_ID, name.c_str()), vec3.x,vec3.y,vec3.z);
	}

	 //(RUS) Конструктор переноса шейдера
	//(ENG) Shader transfer constructor
	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept {
		glDeleteProgram(m_ID);
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;
		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
		return *this;
	}
	ShaderProgram::ShaderProgram(ShaderProgram && shaderProgram) noexcept {
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;
		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
	}
}