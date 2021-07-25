#pragma once
#include <vector>
namespace Impact
{
	class Entity;
	class Scene
	{
	public:
		Scene(){};
		~Scene();
		Entity* CreateEntity();

	private:
		std::vector<Entity*> m_Entities;
	};
}