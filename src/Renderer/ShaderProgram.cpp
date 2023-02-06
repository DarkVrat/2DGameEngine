#include "ShaderProgram.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace Renderer {
	//создание и компиляция шейдерной программы
	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader){
		//Комтиляция вертексного шейдера в и сохранение ID в vertexShaderID
		GLuint vertexShaderID;
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)) {
			std::cerr << "(!) VERTEX SHADER Compile-time error" << std::endl;
			return;
		}

		//Комтиляция фрагментного шейдера в и сохранение ID в fragmentShaderID
		GLuint fragmentShaderID;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)) {
			std::cerr << "(!) FRAGMENT SHADER Compile-time error" << std::endl;
			glDeleteShader(vertexShaderID);
			return;
		}

		m_ID = glCreateProgram();				//создание адреса шейдерной программы
		glAttachShader(m_ID, vertexShaderID);	//Присоединение к программе вертексного шейдера
		glAttachShader(m_ID, fragmentShaderID);	//Присоединение к программе фрагментного шейдера
		glLinkProgram(m_ID);					//Линковка программы шейдера по m_ID адресу

		//Проверка успешности линковки, и вывод ифо об ошибке, или установка m_isCompiled на истину
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

	//компиляция шейдеров из передаваемой строки взятой из файла, указаного типа, и сохранение ID шейдера в shaderID
	bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID) {
		
		shaderID = glCreateShader(shaderType);					//создание ID для шейдера определённого типа
		const char* code = source.c_str();						//получение кода шейдера из строки
		glShaderSource(shaderID, 1, std::move(&code), nullptr); //получает ID куда записать, кол-во элементов(У нас один массив символов), передача кода, длинна строк
		glCompileShader(shaderID);								//компиляция шейдера
		
		//Сохранение информации об успехе/провале компиляции
		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success); 
		
		//проверка, и вывод сообщения об ошибке, или успешное завершение компиляции
		if (!success) {
			GLchar infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog); 
			std::cerr << "(!) ERROR::SHADER: Compile-time error: \n" << infoLog << std::endl; 
			return false;
		}
		return true;
	}

	//Удаление шейдера
	ShaderProgram::~ShaderProgram(){
		glDeleteProgram(m_ID);
	}

	bool ShaderProgram::isCompiled(){
		return m_isCompiled;
	}

	//установка таргета при отрисовке на наш шейдер
	void ShaderProgram::use() const{
		glUseProgram(m_ID);
	}


	void ShaderProgram::setInt(const std::string& name, const GLint value){
		//Получение адреса юниформ переменной шейдера по name, и установка ей значения
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value); 
	}

	void ShaderProgram::setFloat (const std::string& name, const GLfloat value) {
		//Получение адреса юниформ переменной шейдера по name, и установка ей значения
 		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void ShaderProgram::setMatrix4(const std::string& name, const glm::mat4& matrix){
		//Получение адреса юниформ переменной шейдера по name, указание количества mat4 матриц, указание о ненадобности транспоритрования матриц, и загрузка матрицы в переменную
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()),1,GL_FALSE, glm::value_ptr(matrix));
	}

	void ShaderProgram::setVec3(const std::string& name, const glm::vec3 vec3) {
		//Получение адреса юниформ переменной шейдера по name, и установка ей значения
		glUniform3f(glGetUniformLocation(m_ID, name.c_str()), vec3.x,vec3.y,vec3.z);
	}

	//Передача данных програмного шейдера с удалением передаваемого
	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept {
		glDeleteProgram(m_ID);
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;
		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
		return *this;
	}

	//Передача данных програмного шейдера с удалением передаваемого
	ShaderProgram::ShaderProgram(ShaderProgram && shaderProgram) noexcept {
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;
		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
	}
}