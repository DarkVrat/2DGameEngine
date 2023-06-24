#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

 //(RUS) Образец источника звука, содержит параметры, для их установки в Audio::SoundEffectPlayer
//(ENG) Sound source sample, contains options to set them in Audio::SoundEffectPlayer

namespace Renderer {
	class ShaderProgram {
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderProgram& operator=(ShaderProgram&& shaderProgram)noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram)noexcept;
		~ShaderProgram();

		bool isCompiled();

		void use() const;

		void setInt(const std::string& name, const GLint& value);
		void setFloat(const std::string& name, const GLfloat& value);
		void setMatrix4(const std::string& name, const glm::mat4& matrix);
		void setVec3(const std::string& name, const glm::vec3& vec3);
		void setVec4(const std::string& name, const glm::vec4& vec4);

		uint8_t getSettings() { return m_settings; }

	private:
		GLuint m_ID = 0; 
		bool m_isCompiled = false; 

		bool createShader(const std::string& source, const GLenum& shaderType, GLuint& shaderID); 
		uint8_t m_settings;
	};
}