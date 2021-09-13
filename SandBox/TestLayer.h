#pragma once

#include <Impact.h>
#include <random>
class TestLayer : public Impact::Layer
{
public:
	TestLayer();
	virtual void OnAttach(Impact::Graphics& gfx) override;

	virtual void OnDetach() noexcept override {};
	virtual void Update(float dt) noexcept override;
	virtual void Render(Impact::Graphics& gfx) noexcept override;
	virtual void ImGuiRender() noexcept override;
private:
// For testing
	Impact::TransformComponent* icoTransform = nullptr;
	DirectX::XMFLOAT3 m_WorldRotationSpeed{ 0.f, 0.f, 0.f };
	DirectX::XMFLOAT3 m_LocalRotationSpeed{ 0.f, 0.f, 0.f };
// For testing

	std::vector<Impact::Entity*> m_Entities;
	std::vector<Impact::RenderComponent*> m_Draw;

	Impact::Entity* m_pCamera;
	
	std::unique_ptr<Impact::Scene> m_Scene;
};

