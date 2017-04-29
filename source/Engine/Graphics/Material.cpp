#include "stdafx.hpp"

#include "Material.hpp"

#include "ShaderData.hpp"
#include "MeshFilter.hpp"

Material::Material(std::string shaderFile) :
	m_ShaderFile(shaderFile)
{
}
Material::~Material()
{
}

void Material::Initialize()
{
	if (!m_IsInitialized)
	{
		//Load Shader
		m_Shader = ContentManager::Load<ShaderData>(m_ShaderFile);

		//Jup (maybe temporary with texture manager)
		LoadTextures();

		//Get Access to uniforms
		if(m_StandardTransform)
		{
			m_UniMatModel = glGetUniformLocation(m_Shader->GetProgram(), "model");
			m_UniMatWVP = glGetUniformLocation(m_Shader->GetProgram(), "worldViewProj");
		}
		AccessShaderAttributes();

		m_IsInitialized = true;
	}
}

void Material::UploadVariables(glm::mat4 matModel)
{
	STATE->SetShader(m_Shader);
	//Upload matrices
	if (m_StandardTransform)
	{
		glUniformMatrix4fv(m_UniMatModel, 1, GL_FALSE, glm::value_ptr(matModel));
		glUniformMatrix4fv(m_UniMatWVP, 1, GL_FALSE, glm::value_ptr(CAMERA->GetViewProj()));
	}

	UploadDerivedVariables();
}
void Material::UploadVariables(glm::mat4 matModel, const glm::mat4 &matWVP)
{
	STATE->SetShader(m_Shader);
	//Upload matrices
	if (m_StandardTransform)
	{
		glUniformMatrix4fv(m_UniMatModel, 1, GL_FALSE, glm::value_ptr(matModel));
		glUniformMatrix4fv(m_UniMatWVP, 1, GL_FALSE, glm::value_ptr(matWVP));
	}

	UploadDerivedVariables();
}

void Material::SpecifyInputLayout()
{
	unsigned stride = 0;
	for (auto it = MeshFilter::LayoutAttributes.begin(); it != MeshFilter::LayoutAttributes.end(); ++it)
	{
		if (m_LayoutFlags & it->first) stride += it->second.dataSize;
	}
	unsigned startPos = 0;
	for (auto it = MeshFilter::LayoutAttributes.begin(); it != MeshFilter::LayoutAttributes.end(); ++it)
	{
		if (m_LayoutFlags & it->first)
		{
			const char* name = it->second.name.c_str();
			GLint attrib = glGetAttribLocation(m_Shader->GetProgram(), name);
			if (attrib >= 0)
			{
				glEnableVertexAttribArray(attrib);
				glVertexAttribPointer(attrib, it->second.dataSize, it->second.dataType, GL_FALSE,
					stride * sizeof(GLfloat), (void*)(startPos * sizeof(GLfloat)));
				startPos += it->second.dataSize;
			}
			else LOGGER::Log("Could not bind attribute '" + string(name) + "' to shader: " + m_Shader->GetName(), LogLevel::Error);
		}
	}
}
