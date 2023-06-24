#include "DebugRender.h"

#include "../Objects/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

std::shared_ptr<Renderer::ShaderProgram> DebugRender::m_shader;
std::shared_ptr<Renderer::VertexArray> DebugRender::m_VAO;
Renderer::VertexBuffer DebugRender::m_pointsVBO;

void DebugRender::init(){
	std::string vertex = R"(
		#version 330 core
		layout (location = 0) in vec2 point; 

		uniform mat4 projection;
		uniform vec3 view;
		uniform vec4 color;

		out vec4 colorLine;

		void main(){
			gl_Position = projection * vec4(point.x - view.x, point.y-view.y, 99.0, 1.0);
			colorLine=color;
		}
	)";

	std::string fragment = R"(
		#version 330 core
		in vec4 colorLine;
		out vec4 outputColor; 

		void main(){
		    outputColor = colorLine;   
		}
	)";

	m_shader = std::make_shared<Renderer::ShaderProgram>(vertex, fragment);

	m_VAO = std::make_shared<Renderer::VertexArray>();
	m_VAO->bind();

	m_pointsVBO.init(nullptr, 0);
	m_VAO->addBuffer(m_pointsVBO, 0, 2);

	m_VAO->unbind();

}

void DebugRender::updateUniform(){
	glm::vec2 size = CAMERA::getSize();
	glm::vec2 pos = CAMERA::getCoords();
	glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(size.x), 0.f, static_cast<float>(size.y), -100.f, 100.f);

	m_shader->use();
	m_shader->setMatrix4("projection", projectionMatrix);
	m_shader->setVec3("view", glm::vec3(pos.x - size.x / 2, pos.y - size.y / 2, 0));
}

void DebugRender::drawShape(const Shape shape, const glm::vec4& color){
	std::vector<glm::vec2> points = shape.Render();

	m_VAO->bind();

	m_pointsVBO.bind();
	m_pointsVBO.update(points.data(), points.size()*sizeof(glm::vec2));

	m_shader->use();
	m_shader->setVec4("color", color);

	glDrawArrays(GL_LINE_LOOP, 0, points.size());
	m_VAO->unbind();
}

