#pragma once

#include <glad/glad.h>
#include "VertexBuffer.h"

namespace Renderer {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray& operator=(VertexArray&& vertexArray) noexcept;
		VertexArray(VertexArray&& indexArray) noexcept;

		void addBuffer(const VertexBuffer& vertexBuffer, GLuint layoutID, GLuint size, bool divisor=false, GLvoid* pointer=(void*)0);
		void bind() const;
		void unbind() const;

	private:
		GLuint m_id=0;
	};

}