#include "stdafx.hpp"

#include "LightVolume.hpp"
#include "../Materials/LightMaterial.hpp"
#include "../Graphics/MeshFilter.hpp"
#include <gtx/transform.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/matrix_decompose.hpp>
#include <gtx/euler_angles.hpp>
#include "../Materials/NullMaterial.hpp"
#include "../Framebuffers/Gbuffer.hpp"
#include "../Graphics/ShaderData.hpp"
#include "../Graphics/TextureData.hpp"
#include "../Graphics/Frustum.hpp"
#include "ShadowRenderer.hpp"
#include "PrimitiveRenderer.hpp"
#include "RenderPipeline.hpp"

PointLightVolume::PointLightVolume()
{

}
PointLightVolume::~PointLightVolume()
{
	SafeDelete(m_pMaterial);
	SafeDelete(m_pNullMaterial);
}

void PointLightVolume::Initialize()
{
	m_pMaterial = new LightMaterial();
	m_pMaterial->Initialize();
	m_pNullMaterial = new NullMaterial();
	m_pNullMaterial->Initialize();

	IsInitialized = true;
}
void PointLightVolume::Draw(glm::vec3 pos, float radius, glm::vec3 col)
{
	//Make sure everything is set up
	if (!IsInitialized)
	{
		Initialize();
	}

	//Frustum culling
	Sphere objSphere = Sphere(pos, radius);
	if (CAMERA->GetFrustum()->ContainsSphere(objSphere) == VolumeCheck::OUTSIDE)
		return;

	glm::mat4 World = glm::translate(pos)*glm::scale(glm::vec3(radius));

	//Draw the null material in the stencil buffer
	//STATE->SetDepthEnabled(true);
	//STATE->SetCullEnabled(false);
	//glClear(GL_STENCIL_BUFFER_BIT);
	//glStencilFunc(GL_ALWAYS, 0, 0);
	//glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	//glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
	//m_pNullMaterial->UploadVariables(World);
	//PrimitiveRenderer::GetInstance()->Draw<primitives::IcoSphere<2> >();
	//Draw the Light material on the gbuffer
	//glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	//STATE->SetDepthEnabled(false);
	//STATE->SetBlendEnabled(true);
	//STATE->SetBlendEquation(GL_FUNC_ADD);
	//STATE->SetBlendFunction(GL_ONE, GL_ONE);
	//STATE->SetCullEnabled(true);
	//STATE->SetFaceCullingMode(GL_FRONT);

	m_pMaterial->SetLight(pos, col, radius);
	m_pMaterial->UploadVariables(World);

	PrimitiveRenderer::GetInstance()->Draw<primitives::IcoSphere<2> >();

	//STATE->SetFaceCullingMode(GL_BACK);
	//STATE->SetBlendEnabled(false);
}

DirectLightVolume::DirectLightVolume(){}
DirectLightVolume::~DirectLightVolume() 
{
}
void DirectLightVolume::Initialize()
{
	m_pShader = ContentManager::Load<ShaderData>("Shaders/FwdLightDirectionalShader.glsl");
	m_pShaderShadowed = ContentManager::Load<ShaderData>("Shaders/FwdLightDirectionalShadowShader.glsl");

	m_uCol = glGetUniformLocation(m_pShader->GetProgram(), "Color");
	m_uDir = glGetUniformLocation(m_pShader->GetProgram(), "Direction");
	m_uCamPos = glGetUniformLocation(m_pShader->GetProgram(), "camPos");

	IsInitialized = true;
}
void DirectLightVolume::Draw(glm::vec3 dir, glm::vec3 col)
{
	if (!IsInitialized) Initialize();

	// #todo: avoid getting all the uniform info again and again

	STATE->SetShader(m_pShader);

	glUniform1i(glGetUniformLocation(m_pShader->GetProgram(), "texGBufferA"), 0);
	glUniform1i(glGetUniformLocation(m_pShader->GetProgram(), "texGBufferB"), 1);
	glUniform1i(glGetUniformLocation(m_pShader->GetProgram(), "texGBufferC"), 2);
	auto gbufferTex = RenderPipeline::GetInstance()->GetGBuffer()->GetTextures();
	for (size_t i = 0; i < gbufferTex.size(); i++)
	{
		STATE->LazyBindTexture(i, GL_TEXTURE_2D, gbufferTex[i]->GetHandle());
	}
	//for position reconstruction
	glUniform1f(glGetUniformLocation(m_pShader->GetProgram(), "projectionA"), CAMERA->GetDepthProjA());
	glUniform1f(glGetUniformLocation(m_pShader->GetProgram(), "projectionB"), CAMERA->GetDepthProjB());
	glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetProgram(), "viewProjInv"), 1, GL_FALSE, glm::value_ptr(CAMERA->GetStatViewProjInv()));
	glUniform3fv(m_uCamPos, 1, glm::value_ptr(CAMERA->GetTransform()->GetPosition()));

	glUniform3fv(m_uDir, 1, glm::value_ptr(dir));
	glUniform3fv(m_uCol, 1, glm::value_ptr(col));

	PrimitiveRenderer::GetInstance()->Draw<primitives::Quad>();
}
void DirectLightVolume::DrawShadowed(glm::vec3 dir, glm::vec3 col, DirectionalShadowData *pShadow)
{
	if (!IsInitialized) Initialize();

	STATE->SetShader(m_pShaderShadowed);

	//Upload gbuffer
	glUniform1i(glGetUniformLocation(m_pShaderShadowed->GetProgram(), "texGBufferA"), 0);
	glUniform1i(glGetUniformLocation(m_pShaderShadowed->GetProgram(), "texGBufferB"), 1);
	glUniform1i(glGetUniformLocation(m_pShaderShadowed->GetProgram(), "texGBufferC"), 2);
	auto gbufferTex = RenderPipeline::GetInstance()->GetGBuffer()->GetTextures();
	for (size_t i = 0; i < gbufferTex.size(); i++)
	{
		STATE->LazyBindTexture(i, GL_TEXTURE_2D, gbufferTex[i]->GetHandle());
	}
	//for position reconstruction
	glUniform1f(glGetUniformLocation(m_pShaderShadowed->GetProgram(), "projectionA"), CAMERA->GetDepthProjA());
	glUniform1f(glGetUniformLocation(m_pShaderShadowed->GetProgram(), "projectionB"), CAMERA->GetDepthProjB());
	glUniformMatrix4fv(glGetUniformLocation(m_pShaderShadowed->GetProgram(), "viewProjInv"), 1, GL_FALSE, glm::value_ptr(CAMERA->GetStatViewProjInv()));
	glUniform3fv(glGetUniformLocation(m_pShaderShadowed->GetProgram(), "camPos"), 1, glm::value_ptr(CAMERA->GetTransform()->GetPosition()));

	//Camera info
	glUniformMatrix4fv(glGetUniformLocation(m_pShaderShadowed->GetProgram(), "CameraView")
		, 1, GL_FALSE, glm::value_ptr(CAMERA->GetView()));

	//light info
	glUniform3fv(glGetUniformLocation(m_pShaderShadowed->GetProgram(), "Direction"), 1, glm::value_ptr(dir));
	glUniform3fv(glGetUniformLocation(m_pShaderShadowed->GetProgram(), "Color"), 1, glm::value_ptr(col));

	//shadow info
	glUniform1i(glGetUniformLocation(m_pShaderShadowed->GetProgram(), "PcfSamples"), GRAPHICS.NumPCFSamples);
	glUniform1f(glGetUniformLocation(m_pShaderShadowed->GetProgram(), "Bias"), pShadow->m_Bias);

	std::string ligStr = "cascades[";
	for (size_t i = 0; i < pShadow->m_Cascades.size(); i++)
	{
		//Light Projection
		glUniformMatrix4fv(glGetUniformLocation(m_pShaderShadowed->GetProgram(),
			(ligStr + std::to_string(i) + "].LightVP").c_str()), 
			1, GL_FALSE, glm::value_ptr(pShadow->m_Cascades[i].lightVP));

		//Shadow map
		glUniform1i(glGetUniformLocation(m_pShaderShadowed->GetProgram(),
			(ligStr + std::to_string(i) + "].ShadowMap").c_str()), 3+i);
		STATE->LazyBindTexture(3+i, GL_TEXTURE_2D, pShadow->m_Cascades[i].pTexture->GetHandle());

		//cascade distance
		glUniform1f(glGetUniformLocation(m_pShaderShadowed->GetProgram(),
			(ligStr + std::to_string(i) + "].Distance").c_str()), pShadow->m_Cascades[i].distance);
	}

	PrimitiveRenderer::GetInstance()->Draw<primitives::Quad>();
}