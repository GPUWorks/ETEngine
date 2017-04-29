#pragma once
#include "../StaticDependancies/glad/glad.h"

class AbstractScene;
class ShaderData;

class RenderState
{
public:
	RenderState();
	~RenderState();

	void Initialize();

	void SetDepthEnabled(bool enabled) { EnOrDisAble(m_DepthTestEnabled, enabled, GL_DEPTH_TEST); }
	void SetBlendEnabled(bool enabled) { EnOrDisAble(m_BlendEnabled, enabled, GL_BLEND); }
	void SetStencilEnabled(bool enabled) { EnOrDisAble(m_StencilTestEnabled, enabled, GL_STENCIL_TEST); }
	void SetCullEnabled(bool enabled) { EnOrDisAble(m_CullFaceEnabled, enabled, GL_CULL_FACE); }

	void SetFaceCullingMode(GLenum cullMode);
	void SetBlendEquation(GLenum equation);
	void SetBlendFunction(GLenum sFactor, GLenum dFactor);

	void SetViewport(glm::ivec2 pos, glm::ivec2 size);

	void SetClearColor(glm::vec4 col);

	void SetShader(ShaderData* pShader);

	void BindFramebuffer(GLuint handle);
	void BindReadFramebuffer(GLuint handle);
	void BindDrawFramebuffer(GLuint handle);

private:

	void EnOrDisAble(bool &state, bool enabled, GLenum glState);

	bool m_DepthTestEnabled = false;

	bool m_CullFaceEnabled = false;
	GLenum m_CullFaceMode = GL_BACK;
	
	bool m_StencilTestEnabled = false;

	bool m_BlendEnabled = false;
	GLenum m_BlendEquationRGB = GL_FUNC_ADD;
	GLenum m_BlendEquationAlpha = GL_FUNC_ADD;
	GLenum m_BlendFuncSFactor = GL_ONE;
	GLenum m_BlendFuncDFactor = GL_ZERO;

	glm::ivec2 m_ViewportPosition = glm::ivec2(0);
	glm::ivec2 m_ViewportSize; //initialize with values used during context creation

	glm::vec4 m_ClearColor = glm::vec4(0);

	GLuint m_ReadFramebuffer = 0;
	GLuint m_DrawFramebuffer = 0;

	ShaderData* m_pBoundShader = nullptr;
};