#pragma once
#include <Impact.h>
class TestLayer : public Impact::Layer
{
public:
	TestLayer() : m_Boxes{} 
	{
		//struct V{DirectX::XMFLOAT3 pos;};
		//Impact::Primitive::Plane::CreateSubDivided<V>(2u,2u); 
		
	};
	virtual void OnAttach(Impact::Graphics& gfx) override
	{
		m_Scene = std::make_unique<Impact::Scene>();
		for (int i{}; i < 10; ++i) {

			Impact::Entity* entity = m_Scene->CreateEntity();

			entity->AddComponent<Impact::RandomMovementComponent>();
			entity->AddComponent<Impact::TestCube>(gfx);
			m_Boxes.push_back(entity);
		}
	};
	virtual void OnDetach() noexcept override {};
	virtual void Update(float dt) noexcept override
	{
		for (auto* b : m_Boxes)
			b->Update(dt);
	};
	virtual void Render(Impact::Graphics& gfx) const noexcept override
	{
		for (auto* b : m_Boxes)
			b->GetComponent<Impact::TestCube>()->Draw(gfx);
	};

private:

	std::vector<Impact::Entity*> m_Boxes;

	std::unique_ptr<Impact::Scene> m_Scene;
};

