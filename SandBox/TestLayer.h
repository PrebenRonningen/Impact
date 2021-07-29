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
		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int>randomize(0,9);
		std::uniform_int_distribution<int>type(0,3);

		m_Scene = std::make_unique<Impact::Scene>();
		m_Draw.resize(180);
		for (int i{}; i < 180; ++i) 
		{
			Impact::Entity* entity = m_Scene->CreateEntity();
			entity->AddComponent<Impact::RandomMovementComponent>();

			switch (type(rng))
			{
			case 0:
				m_Draw[i] = entity->AddComponent<Impact::Cube>(gfx, (randomize(rng) % 2 == 0));
			break;
			case 1:
				m_Draw[i] = entity->AddComponent<Impact::UVSphere>(gfx, (randomize(rng) % 2 == 0));
			break;
			case 2:
				m_Draw[i] = entity->AddComponent<Impact::Plane>(gfx, (randomize(rng) % 2 == 0));
			break;
			case 3:
				m_Draw[i] = entity->AddComponent<Impact::IcoSphere>(gfx, (randomize(rng) % 2 == 0));
			break;
			default:
				break;
			}
			
			m_Entities.push_back(entity);
		}
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

