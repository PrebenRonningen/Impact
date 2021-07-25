#include "Scene.h"
#include "Entity\Entity.h"
#include "Components\TransformComponent.h"

namespace Impact {
	Scene::~Scene()
	{
		for (auto e : m_Entities) {
			if (e != nullptr) {
				delete e;
				e = nullptr;
			}
		}
	}
	Entity* Scene::CreateEntity()
	{
		Entity* entity = new Entity{this};
		entity->AddComponent<TransformComponent>();

		m_Entities.push_back(std::move(entity));
		return m_Entities[m_Entities.size()-1];
	}
}
