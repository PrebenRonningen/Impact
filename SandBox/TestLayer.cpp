#include "TestLayer.h"
#include <Impact.h>
#include <imgui.h>
#include "Graphics\Bindable\TransformCbuf.h"
#include "Events\EventHandler.h"


std::vector<Impact::Entity*> m_Cameras;
int currentCameraIdx = 0;
static int recursionLevel = 4;
TestLayer::TestLayer() : m_Entities{}, m_Earth{nullptr}, m_pCamera{nullptr}
{
	Impact::EventHandler::RegisterEvent(this, 1, Impact::Event::EventType::KeyPressed | Impact::Event::EventType::KeyReleased, [this](auto&&... args) -> decltype(auto)
		{
			return this->TestLayer::OnKeyEvent(std::forward<decltype(args)>(args)...);
		});
}

void TestLayer::OnAttach(Impact::Graphics& gfx)
{
	m_Scene = std::make_unique<Impact::Scene>();

	// planet
	Impact::Entity* entity = m_Scene->CreateEntity();
	m_Draw.push_back(entity->AddComponent<Impact::IcoSphere>(gfx));
	icoTransform = entity->GetComponent<Impact::TransformComponent>();
	m_Entities.push_back(entity);
	entity->GetComponent<Impact::IcoSphere>()->UpdateModel(recursionLevel);
	m_Earth = entity;

	// moon
	entity = m_Scene->CreateEntity();
	m_Draw.push_back(entity->AddComponent<Impact::IcoSphere>(gfx));
	moonTransform = entity->GetComponent<Impact::TransformComponent>();
	moonTransform->LocalScale() = {1.f/4.f, 1.f / 4.f , 1.f / 4.f };
	moonTransform->LocalTranslation() = {0,0,-480};
	m_Entities.push_back(entity);

	// camera 1
	entity = m_Scene->CreateEntity();
//	m_Draw.push_back(entity->AddComponent<Impact::Cube>(gfx));
	entity->AddComponent<Impact::MovementComponent>();
	entity->AddComponent<Impact::CameraComponent>();
	//entity->GetComponent<Impact::MovementComponent>()->AssignMovementUpdate(entity->GetComponent<Impact::CameraComponent>()->GetNeedsUpdate());
	//entity->GetComponent<Impact::TransformComponent>()->Translation() = DirectX::XMFLOAT3(0,-130,62);
	//entity->GetComponent<Impact::TransformComponent>()->Rotation() = DirectX::XMFLOAT3(45,0,0);
	m_Entities.push_back(entity);
	m_Cameras.push_back(entity);

	// camera 2
	entity = m_Scene->CreateEntity();
//	m_Draw.push_back(entity->AddComponent<Impact::Cube>(gfx));
	entity->AddComponent<Impact::MovementComponent>();
	entity->AddComponent<Impact::CameraComponent>();
	//entity->GetComponent<Impact::MovementComponent>()->AssignMovementUpdate(entity->GetComponent<Impact::CameraComponent>()->GetNeedsUpdate());
	m_Entities.push_back(entity);
	m_Cameras.push_back(entity);

	//// camera 3
	//entity = m_Scene->CreateEntity();
	//m_Draw.push_back(entity->AddComponent<Impact::Cube>(gfx));
	//entity->AddComponent<Impact::MovementComponent>();
	//entity->AddComponent<Impact::CameraComponent>();
	//entity->GetComponent<Impact::MovementComponent>()->AssignMovementUpdate(entity->GetComponent<Impact::CameraComponent>()->GetNeedsUpdate());
	//m_Entities.push_back(entity);
	//m_Cameras.push_back(entity);

	//// camera 4
	//entity = m_Scene->CreateEntity();
	//m_Draw.push_back(entity->AddComponent<Impact::Cube>(gfx));
	//entity->AddComponent<Impact::MovementComponent>();
	//entity->AddComponent<Impact::CameraComponent>();
	//entity->GetComponent<Impact::MovementComponent>()->AssignMovementUpdate(entity->GetComponent<Impact::CameraComponent>()->GetNeedsUpdate());
	//m_Entities.push_back(entity);
	//m_Cameras.push_back(entity);

	m_pCamera = m_Cameras[0];
	m_pCamera->GetComponent<Impact::CameraComponent>()->SetActive();
	m_pCamera->GetComponent<Impact::MovementComponent>()->SetActive();

	// Light 1
	entity = m_Scene->CreateEntity();
	entity->AddComponent<Impact::LightComponent>(gfx);
	entity->GetComponent<Impact::LightComponent>()->SetDirectionalLight();
	m_Entities.push_back(entity);
	// Light 2
	entity = m_Scene->CreateEntity();
	entity->AddComponent<Impact::LightComponent>(gfx);
	entity->GetComponent<Impact::LightComponent>()->SetPointLight();
	entity->GetComponent<Impact::TransformComponent>()->Translation().z = 65.f;
	entity->GetComponent<Impact::LightComponent>()->LightPosition() = entity->GetComponent<Impact::TransformComponent>()->Translation();
	m_Light = entity;

	m_Entities.push_back(entity);
}

void TestLayer::Update(float dt) noexcept
{
	icoTransform->LocalRotation().x = fmod(icoTransform->LocalRotation().x + m_eLocalRotationSpeed.x * dt, 360.f);
	icoTransform->LocalRotation().y = fmod(icoTransform->LocalRotation().y + m_eLocalRotationSpeed.y * dt, 360.f);
	icoTransform->LocalRotation().z = fmod(icoTransform->LocalRotation().z + m_eLocalRotationSpeed.z * dt, 360.f);

	icoTransform->Rotation().x = fmod(icoTransform->Rotation().x + m_eWorldRotationSpeed.x * dt, 360.f);
	icoTransform->Rotation().y = fmod(icoTransform->Rotation().y + m_eWorldRotationSpeed.y * dt, 360.f);
	icoTransform->Rotation().z = fmod(icoTransform->Rotation().z + m_eWorldRotationSpeed.z * dt, 360.f);

	moonTransform->LocalRotation().x = fmod(moonTransform->LocalRotation().x + m_mLocalRotationSpeed.x * dt, 360.f);
	moonTransform->LocalRotation().y = fmod(moonTransform->LocalRotation().y + m_mLocalRotationSpeed.y * dt, 360.f);
	moonTransform->LocalRotation().z = fmod(moonTransform->LocalRotation().z + m_mLocalRotationSpeed.z * dt, 360.f);

	moonTransform->Rotation().x = fmod(moonTransform->Rotation().x + m_mWorldRotationSpeed.x * dt, 360.f);
	moonTransform->Rotation().y = fmod(moonTransform->Rotation().y + m_mWorldRotationSpeed.y * dt, 360.f);
	moonTransform->Rotation().z = fmod(moonTransform->Rotation().z + m_mWorldRotationSpeed.z * dt, 360.f);
	m_Light->GetComponent<Impact::LightComponent>()->LightPosition() = DirectX::XMFLOAT3(moonTransform->GetTransform().m[3]);
	for (auto* e : m_Entities)
		e->Update(dt);
}

void TestLayer::Render(Impact::Graphics& gfx) noexcept
{
	// Lights goes here

	for (auto& e : m_Entities)
	{
		if(const auto& light = e->GetComponent<Impact::LightComponent>())
		{
			light->Bind(gfx);
			break;
		}
	}

	for (auto& d : m_Draw)
	{
		d->Draw(gfx);
	}
}

void TestLayer::ImGuiRender() noexcept
{
	if (ImGui::Begin("Earth Layer"))
	{
		ImGui::DragFloat3("Earth Local Translation", &icoTransform->LocalTranslation().x);
		ImGui::DragFloat3("Earth Local Rotation", &icoTransform->LocalRotation().x);
		ImGui::DragFloat3("Earth Local Scale", &icoTransform->LocalScale().x, 0.001f);
		ImGui::DragFloat3("Earth World Translation", &icoTransform->Translation().x);
		ImGui::DragFloat3("Earth World Rotation", &icoTransform->Rotation().x);
		ImGui::DragFloat3("Earth World Scale", &icoTransform->Scale().x, 0.001f);

		ImGui::Text("Earth Rotation Speed");
		
		ImGui::InputFloat3("Earth Local Rotation Speed", &m_eLocalRotationSpeed.x);
		ImGui::InputFloat3("Earth World Rotation Speed", &m_eWorldRotationSpeed.x);
	}

	ImGui::End();

	if (ImGui::Begin("Moon Layer"))
	{
		ImGui::DragFloat3("Moon Local Translation", &moonTransform->LocalTranslation().x);
		ImGui::DragFloat3("Moon Local Rotation", &moonTransform->LocalRotation().x);
		ImGui::DragFloat3("Moon Local Scale", &moonTransform->LocalScale().x, 0.001f);
		ImGui::DragFloat3("Moon World Translation", &moonTransform->Translation().x);
		ImGui::DragFloat3("Moon World Rotation", &moonTransform->Rotation().x);
		ImGui::DragFloat3("Moon World Scale", &moonTransform->Scale().x, 0.001f);

		ImGui::Text("Moon Rotation Speed");

		ImGui::InputFloat3("Moon Local Rotation Speed", &m_mLocalRotationSpeed.x);
		ImGui::InputFloat3("Moon World Rotation Speed", &m_mWorldRotationSpeed.x);
	}

	ImGui::End();

	if (ImGui::Begin("Camera"))
	{
		if (ImGui::DragFloat3("Camera Rotation", &m_pCamera->GetComponent<Impact::TransformComponent>()->Rotation().x))
		{
			m_pCamera->GetComponent<Impact::CameraComponent>()->UpdateView();
		}
		
		if (ImGui::DragFloat3("Camera Transform", &m_pCamera->GetComponent<Impact::TransformComponent>()->Translation().x))
		{
			m_pCamera->GetComponent<Impact::CameraComponent>()->UpdateView();
		}

		if (ImGui::Button("Reset"))
		{
			m_pCamera->GetComponent<Impact::MovementComponent>()->Reset();

			m_pCamera->GetComponent<Impact::CameraComponent>()->UpdateView();
		}
	}
	ImGui::End();

	if(ImGui::Begin("Cameras"))
	{
		if (ImGui::Button("NextCamera")) {
			currentCameraIdx++;
			currentCameraIdx = currentCameraIdx % (m_Cameras.size());
			
			m_pCamera->GetComponent<Impact::CameraComponent>()->SetInactive();
			m_pCamera->GetComponent<Impact::MovementComponent>()->SetInactive();
			
			m_pCamera = m_Cameras[currentCameraIdx];
			m_pCamera->GetComponent<Impact::MovementComponent>()->SetActive();
			m_pCamera->GetComponent<Impact::CameraComponent>()->SetActive();
		}
	}
	ImGui::End();

	if (ImGui::Begin("Recursion")) 
	{
		if (ImGui::Button("Next Level"))
		{
			recursionLevel++;
			recursionLevel %= 7;

			m_Earth->GetComponent<Impact::IcoSphere>()->UpdateModel(recursionLevel);
		}

		if (ImGui::Button("Previous Level"))
		{
			recursionLevel--;
			if (recursionLevel < 0)
				recursionLevel = 6;

			m_Earth->GetComponent<Impact::IcoSphere>()->UpdateModel(recursionLevel);
		}
	}
	ImGui::End();

	if (ImGui::Begin("Light"))
	{
		auto& lightData = m_Light->GetComponent<Impact::LightComponent>()->UIWindow();
		
		if (ImGui::Button("Deactivate Lights"))
		{
			Impact::LightComponent::ToggleLights();
		}

		ImGui::DragFloat3("Light Direction", &lightData.lightDir.x);
		ImGui::DragFloat3("Light Color", &lightData.lightColor.x);
		ImGui::DragFloat("Light Range", &lightData.lightRange);
		ImGui::DragFloat("Light Intensity", &lightData.lightIntensity, 0.01f);
		ImGui::DragFloat("Light AttConst", &lightData.attConst);
		ImGui::DragInt("Light FallOffType", &lightData.fallOffType, 1, 0, 2);
		ImGui::DragInt("Light Type", &lightData.lightType, 1, 0, 1);
	}
	ImGui::End();
}

bool TestLayer::OnKeyEvent(Impact::Event& e)
{
	auto& keyEvent = static_cast<Impact::KeyEvent&>(e);
	bool handled = false;

	switch (keyEvent.GetType())
	{
	case Impact::Event::EventType::KeyPressed:
	case Impact::Event::EventType::KeyReleased:
		{
			handled |= m_pCamera->GetComponent<Impact::MovementComponent>()->OnKeyEvent(e);
			break;
		}
	default:
		break;
	}

	return handled;
}
