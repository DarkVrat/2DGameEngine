#pragma once

#include "Shape.h"
#include "../../Renderer/ShaderProgram.h"
#include "../../Renderer/VertexArray.h"
#include <glm/vec4.hpp>
#include <memory>

class DebugRender {
public:
	static void init();
	static void updateUniform();
	static void drawShape(const Shape shape, const glm::vec4& color=glm::vec4(1,1,1,1));
private:
	static std::shared_ptr<Renderer::ShaderProgram> m_shader;
	static std::shared_ptr<Renderer::VertexArray> m_VAO;
	static Renderer::VertexBuffer m_pointsVBO;
};