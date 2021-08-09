#pragma once

#include <Impact.h>
#include <random>
class TestLayer : public Impact::Layer
{
public:
	TestLayer() : m_Entities{} 
	{
		//struct V{DirectX::XMFLOAT3 pos;};
		//Impact::Primitive::Plane::CreateSubDivided<V>(2u,2u); 
	};
	virtual void OnAttach(Impact::Graphics& gfx) override;

	virtual void OnDetach() noexcept override {};
	virtual void Update(float dt) noexcept override;
	virtual void Render(Impact::Graphics& gfx) const noexcept override;

private:

	std::vector<Impact::Entity*> m_Entities;
	std::vector<Impact::RenderComponent*> m_Draw;
	
	std::unique_ptr<Impact::Scene> m_Scene;
};

