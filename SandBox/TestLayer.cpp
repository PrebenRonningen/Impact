#include "TestLayer.h"
#include <Impact.h>

void TestLayer::OnAttach(Impact::Graphics& gfx)
{

	m_Scene = std::make_unique<Impact::Scene>();

	Impact::Entity* entity = m_Scene->CreateEntity();
	entity->AddComponent<Impact::RandomMovementComponent>();

	m_Draw.push_back(entity->AddComponent<Impact::IcoSphere>(gfx));


	m_Entities.push_back(entity);
}
void TestLayer::Update(float dt) noexcept
{
	for (auto* e : m_Entities)
		e->Update(dt);
}

void TestLayer::Render(Impact::Graphics& gfx) const noexcept
{
	for (auto& d : m_Draw)
	{
		d->Draw(gfx);
	}
};