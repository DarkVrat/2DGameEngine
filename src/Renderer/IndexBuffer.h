#pragma once

#include <glad/glad.h>

namespace Renderer {

	class IndexBuffer {
	public:
		IndexBuffer();
		~IndexBuffer();

		void init(const void* data, const unsigned int count);
		void bind() const;
		void unbind() const;

		unsigned int getCount() const;

	private:
		GLuint m_id;
		unsigned int m_count;
	};

}