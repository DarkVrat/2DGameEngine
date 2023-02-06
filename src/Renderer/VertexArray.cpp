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

	void VertexArray::addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& Layout){
		bind();
		vertexBuffer.bind();
		const auto& layoutElements = Layout.getLayoutElements();
		GLbyte* offset = nullptr;

		for (unsigned int i = 0; i < layoutElements.size(); i++) {
			const auto& currentLayoutElement = layoutElements[i];
			GLuint currentAttribIndex = m_elementsCount + i;
			glEnableVertexAttribArray(currentAttribIndex);
			glVertexAttribPointer(currentAttribIndex, currentLayoutElement.m_count, currentLayoutElement.m_type, currentLayoutElement.m_normalized, Layout.getStride(), offset);
			offset += currentLayoutElement.m_size;
		}
		m_elementsCount += layoutElements.size();
	}

	void VertexArray::bind() const {
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() const {
		glBindVertexArray(0);
	}
}