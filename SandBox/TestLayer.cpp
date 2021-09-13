#include "TestLayer.h"
#include <Impact.h>
#include <imgui.h>

TestLayer::TestLayer() : m_Entities{}
{

}

void TestLayer::OnAttach(Impact::Graphics& gfx)
{

	m_Scene = std::make_unique<Impact::Scene>();

	Impact::Entity* entity = m_Scene->CreateEntity();
	//entity->AddComponent<Impact::RandomMovementComponent>();
	m_Draw.push_back(entity->AddComponent<Impact::IcoSphere>(gfx));

	icoTransform = entity->GetComponent<Impact::TransformComponent>();

	m_Entities.push_back(entity);

	entity = m_Scene->CreateEntity();
//	entity->AddComponent<Impact::MovementComponent>();
	//entity->AddComponent<Impact::CameraComponent>();
	entity->AddComponent<Impact::CameraComponent>()->SetActive();
	m_Entities.push_back(entity);
	m_pCamera = entity;

}

void TestLayer::Update(float dt) noexcept
{
	for (auto* e : m_Entities)
		e->Update(dt);

	icoTransform->LocalRotation().x = fmod(icoTransform->LocalRotation().x + m_LocalRotationSpeed.x * dt, 360.f);
	icoTransform->LocalRotation().y = fmod(icoTransform->LocalRotation().y + m_LocalRotationSpeed.y * dt, 360.f);
	icoTransform->LocalRotation().z = fmod(icoTransform->LocalRotation().z + m_LocalRotationSpeed.z * dt, 360.f);

	icoTransform->Rotation().x = fmod(icoTransform->Rotation().x + m_WorldRotationSpeed.x * dt, 360.f);
	icoTransform->Rotation().y = fmod(icoTransform->Rotation().y + m_WorldRotationSpeed.y * dt, 360.f);
	icoTransform->Rotation().z = fmod(icoTransform->Rotation().z + m_WorldRotationSpeed.z * dt, 360.f);


}

void TestLayer::Render(Impact::Graphics& gfx) noexcept
{

	for (auto& d : m_Draw)
	{
		d->Draw(gfx);
	}
}

#include "Core\QuaternionToEuler.h"

void TestLayer::ImGuiRender() noexcept
{
	if (ImGui::Begin("Test Layer"))
	{
		ImGui::DragFloat3("LocalTranslation", &icoTransform->LocalTranslation().x);
		ImGui::DragFloat3("LocalRotation", &icoTransform->LocalRotation().x);
		ImGui::DragFloat3("Local Scale", &icoTransform->LocalScale().x);
		ImGui::DragFloat3("World Translation", &icoTransform->Translation().x);
		ImGui::DragFloat3("World Rotation", &icoTransform->Rotation().x);
		ImGui::DragFloat3("World Scale", &icoTransform->Scale().x);

		ImGui::Text("Rotation Speed");
		
		ImGui::InputFloat3("Local Rotation Speed", &m_LocalRotationSpeed.x);
		ImGui::InputFloat3("World Rotation Speed", &m_WorldRotationSpeed.x);
	}

	ImGui::End();

	if (ImGui::Begin("Camera"))
	{
		static bool& cameraIsActive = m_pCamera->GetComponent<Impact::CameraComponent>()->GetIsActive();
		if (ImGui::Checkbox("ActiveCamera", &cameraIsActive))
		{
			if(cameraIsActive)
				m_pCamera->GetComponent<Impact::CameraComponent>()->SetActive();
			else
				m_pCamera->GetComponent<Impact::CameraComponent>()->SetInactive();
		}

		if (ImGui::DragFloat3("Camera Rotation", &m_pCamera->GetComponent<Impact::TransformComponent>()->Rotation().x))
		{
			//m_pCamera->GetComponent<Impact::CameraComponent>()->UpdateRotation();
		}
		
		if (ImGui::DragFloat3("Camera Transform", &m_pCamera->GetComponent<Impact::TransformComponent>()->Translation().x))
		{
			//m_pCamera->GetComponent<Impact::CameraComponent>()->UpdatePosition();
		}
		if (ImGui::Button("Reset"))
		{
			m_pCamera->GetComponent<Impact::CameraComponent>()->Reset();
		}
	}
	ImGui::End();

};