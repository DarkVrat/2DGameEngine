#include "IndexBuffer.h"

namespace Renderer {
	 //(RUS) ����������� �������� ��������������������� �������. ���������� ��� �������� ������. 
	//(ENG) constructor creating an uninitialized object. destructor to remove the buffer.
	IndexBuffer::IndexBuffer() :m_id(0),m_count(0) {}
	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_id);	
	}


	 //(RUS) ������������� ������ � OpenGL, ��� count ���������� � ��������� data 
	//(ENG) buffer initialization in OpenGL, for count variables and putting data
	void IndexBuffer::init(const void* data, const unsigned& count) {
		m_count = count;
		glGenBuffers(1, &m_id); 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(GLuint), data, GL_STATIC_DRAW);
	}

	 //(RUS) bind � unbind ������, ��� �������������
	//(ENG) bind to unbind buffer, to use
	void IndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}
	void IndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	 //(RUS) ��������� ���������� ��������� � ������
	//(ENG) getting the number of elements in the buffer
	unsigned int IndexBuffer::getCount() const { return m_count; }
}