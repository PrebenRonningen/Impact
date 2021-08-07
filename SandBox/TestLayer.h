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
	virtual void OnAttach(Impact::Graphics& gfx) override
	{

		m_Scene = std::make_unique<Impact::Scene>();

		Impact::Entity* entity = m_Scene->CreateEntity();
		entity->AddComponent<Impact::RandomMovementComponent>();

		m_Draw.push_back(entity->AddComponent<Impact::IcoSphere>(gfx));

		
		m_Entities.push_back(entity);
	};
	virtual void OnDetach() noexcept override {};
	virtual void Update(float dt) noexcept override
	{
		for (auto* e : m_Entities)
			e->Update(dt);
	};
	virtual void Render(Impact::Graphics& gfx) const noexcept override
	{
		for (auto& d : m_Draw)
		{
			d->Draw(gfx);
		}
	};

private:

	std::vector<Impact::Entity*> m_Entities;
	std::vector<Impact::RenderComponent*> m_Draw;
	
	std::unique_ptr<Impact::Scene> m_Scene;
};

