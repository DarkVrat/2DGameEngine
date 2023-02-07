#pragma once

#include <vector>
#include <glad/glad.h>

namespace Renderer {

	struct VertexBufferLayoutElement{
		GLint m_count;
		GLenum m_type;
		GLboolean m_normalized;
		unsigned int m_size;

		VertexBufferLayoutElement(GLint count, GLenum type, GLboolean normalized, unsigned int size):m_count(count),m_type(type),m_normalized(normalized),m_size(size) {}
	};

	class VertexBufferLayout {
	public:
		VertexBufferLayout();

		void reserveElements(const size_t count);
		void addElementLayoutFloat(const unsigned int count, const bool normalized);

		unsigned int getStride() const { return m_stride; }
		const std::vector<VertexBufferLayoutElement>& getLayoutElements() const { return m_layoutElements; }

	private:
		std::vector<VertexBufferLayoutElement> m_layoutElements;
		unsigned int m_stride;
	};
}