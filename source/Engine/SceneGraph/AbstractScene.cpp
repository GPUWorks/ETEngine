#include "stdafx.hpp"
#include "AbstractScene.hpp"

#include "../Components/CameraComponent.hpp"
#include "../Components/LightComponent.hpp"
#include "../Prefabs\FreeCamera.hpp"
#include "../Base/Time.hpp"
#include "Entity.hpp"
#include "../Framebuffers\Gbuffer.hpp"
#include "../Prefabs/Skybox.hpp"
#include "../Prefabs/FreeCamera.hpp"
#include "../Framebuffers/PostProcessingRenderer.hpp"
#include "../GraphicsHelper/TextRenderer.hpp"
#include "../GraphicsHelper/RenderPipeline.hpp"

#define CONTEXT Context::GetInstance()

AbstractScene::AbstractScene(std::string name) 
	: m_Name(name)
	, m_IsInitialized(false)
{
}

AbstractScene::~AbstractScene()
{
	for (Entity* pEntity : m_pEntityVec)
	{
		SafeDelete(pEntity);
	}
	m_pEntityVec.clear();
	if (m_pSkybox)SafeDelete(m_pSkybox);

	SafeDelete(m_pConObj);
	SafeDelete(m_pTime);
}

void AbstractScene::AddEntity(Entity* pEntity)
{
	pEntity->m_pParentScene = this;
	pEntity->RootInitialize();
	m_pEntityVec.push_back(pEntity);
}

void AbstractScene::RemoveEntity(Entity* pEntity, bool deleteEntity)
{
	auto it = find(m_pEntityVec.begin(), m_pEntityVec.end(), pEntity);
	m_pEntityVec.erase(it);
	if (deleteEntity)
	{
		delete pEntity;
		pEntity = nullptr;
	}
	else pEntity->m_pParentScene = nullptr;
}

void AbstractScene::RootInitialize()
{
	if (m_IsInitialized)return;

	//Create SceneContext
	FreeCamera* freeCam = new FreeCamera();
	freeCam->GetTransform()->SetPosition(0, -1, -3.5);
	freeCam->GetTransform()->RotateEuler(glm::radians(20.f), 0, 0);
	AddEntity(freeCam);
	m_pDefaultCam = freeCam->GetComponent<CameraComponent>();
	m_pTime = new Time();
	m_pConObj = new ContextObjects();
	m_pConObj->pCamera = m_pDefaultCam;
	m_pConObj->pTime = m_pTime;
	m_pConObj->pScene = this;

	CONTEXT->SetContext(m_pConObj);

	Initialize();

	for (Entity* pEntity : m_pEntityVec)
	{
		pEntity->RootInitialize();
	}

	m_IsInitialized = true;

	m_pConObj->pTime->Start();
}

void AbstractScene::RootUpdate()
{
	m_pConObj->pTime->Update();

	PERFORMANCE->StartFrameTimer();

	m_pConObj->pCamera->Update();

	Update();
	if (INPUT->IsKeyboardKeyDown(SDL_SCANCODE_UP))
	{
		float exposure = RenderPipeline::GetInstance()->GetPostProcessor()->GetExposure();
		float newExp = exposure * 4.f;
		exposure += (newExp - exposure)*TIME->DeltaTime();
		LOGGER::Log("Exposure: " + to_string(exposure));
		RenderPipeline::GetInstance()->GetPostProcessor()->SetExposure(exposure);
	}
	if (INPUT->IsKeyboardKeyDown(SDL_SCANCODE_DOWN))
	{
		float exposure = RenderPipeline::GetInstance()->GetPostProcessor()->GetExposure();
		float newExp = exposure * 4.f;
		exposure -= (newExp - exposure)*TIME->DeltaTime();
		LOGGER::Log("Exposure: " + to_string(exposure));
		RenderPipeline::GetInstance()->GetPostProcessor()->SetExposure(exposure);
	}
	if (INPUT->IsKeyboardKeyDown(SDL_SCANCODE_LEFT) && m_UseSkyBox)
	{
		float r = min(max(m_pSkybox->GetRoughness() -TIME->DeltaTime(), 0.f), 1.f);
		LOGGER::Log("Roughness: " + to_string(r));
		m_pSkybox->SetRoughness(r);
	}
	if (INPUT->IsKeyboardKeyDown(SDL_SCANCODE_RIGHT) && m_UseSkyBox)
	{
		float r = min(max(m_pSkybox->GetRoughness() + TIME->DeltaTime(), 0.f), 1.f);
		LOGGER::Log("Roughness: " + to_string(r));
		m_pSkybox->SetRoughness(r);
	}

	for (Entity* pEntity : m_pEntityVec)
	{
		pEntity->RootUpdate();
	}
	if (m_UseSkyBox)
	{
		m_pSkybox->RootUpdate();
	}
}

void AbstractScene::RootOnActivated()
{
	CONTEXT->SetContext(m_pConObj);
	OnActivated();
}
void AbstractScene::RootOnDeactivated()
{
	OnDeactivated();
}

void AbstractScene::SetActiveCamera(CameraComponent* pCamera)
{
	m_pConObj->pCamera = pCamera;
}

std::vector<LightComponent*> AbstractScene::GetLights()
{
	vector<LightComponent*> ret;
	for (auto *pEntity : m_pEntityVec)
	{
		auto entityLights = pEntity->GetComponents<LightComponent>();
		ret.insert(ret.end(), entityLights.begin(), entityLights.end());
	}
	return ret;
}

void AbstractScene::SetSkybox(string assetFile)
{
	m_UseSkyBox = true;
	SafeDelete(m_pSkybox);
	m_pSkybox = new Skybox(assetFile);
	m_pSkybox->RootInitialize();
	m_pSkybox->SetRoughness(0.15f);
}

HDRMap* AbstractScene::GetEnvironmentMap()
{
	if (m_UseSkyBox)
	{
		return m_pSkybox->GetHDRMap();
	}
	return nullptr;
}