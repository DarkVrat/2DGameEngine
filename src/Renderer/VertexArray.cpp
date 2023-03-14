#include "VertexArray.h"

namespace Renderer {
	VertexArray::VertexArray(){
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray(){
		glDeleteVertexArrays(1, &m_id);
	}

	VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept {
		m_id = vertexArray.m_id;
		vertexArray.m_id = 0;
		return *this;
	}
	VertexArray::VertexArray(VertexArray&& vertexArray) noexcept {
		m_id = vertexArray.m_id;
		vertexArray.m_id = 0;
	}

	void VertexArray::addBuffer(const VertexBuffer& vertexBuffer, GLuint layoutID, GLuint size, bool divisor, GLvoid* pointer){
		bind();
		vertexBuffer.bind();

		glEnableVertexAttribArray(layoutID);
		glVertexAttribPointer(layoutID, size, GL_FLOAT, GL_FALSE, size * sizeof(float), pointer);
		

		if (divisor) {
			glVertexAttribDivisor(layoutID, 1);
		}
	}

	void VertexArray::bind() const {
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() const {
		glBindVertexArray(0);
	}
}