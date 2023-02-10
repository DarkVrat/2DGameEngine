#pragma once

#include <glad/glad.h>

 //(RUS) ������ ��� �������� � ������ � �������� ��������, ��� ��������� � openGL 
//(ENG) an object for storing and working with an array of indices, for rendering in openGL

namespace Renderer {
	class IndexBuffer {
	public:
		IndexBuffer();
		~IndexBuffer();

		void init(const void* data, const unsigned& count);
		void bind()const;
		void unbind() const;

		unsigned int getCount() const;

	private:
		GLuint m_id;
		unsigned int m_count;
	};

}