#include "RenderEngine.h"

namespace Renderer {
	void RenderEngine::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader){
		shader.use();
		vertexArray.bind();
		indexBuffer.bind();
		
		glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void RenderEngine::setClearColor(float r, float g, float b, float a){
		glClearColor(r, g, b, a);
	}

	void RenderEngine::setDetphTest(bool flag){
		if (flag) {
			glEnable(GL_DEPTH_TEST);
		}
		else{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void RenderEngine::clear() {
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}

	void RenderEngine::setViewport(unsigned int width, unsigned int height, unsigned intleftOffset, unsigned int bottomOffset){
		glViewport(intleftOffset, bottomOffset, width, height);
	}
}
