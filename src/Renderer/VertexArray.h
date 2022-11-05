#pragma once

#include <glad/glad.h>
#include"VertexBuffer.h"
#include"VertexBufferLayout.h"

namespace Renderer {

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&& vertexArray) noexcept;
		VertexArray(VertexArray&& indexArray) noexcept;

		void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& Layout);
		void bind() const;
		void unbind() const;

	private:
		GLuint m_id=0;
		size_t m_elementsCount =0;
	};

}