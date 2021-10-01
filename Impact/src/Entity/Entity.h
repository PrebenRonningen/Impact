#pragma once
#include <typeinfo>
#include <vector>
namespace Impact
{
	class Scene;
	class Component;
	class Entity
	{
	public:
		Entity(Scene* pParentScene);
		~Entity();
		Entity(const Entity& other) = delete;
		Entity(Entity&& other);
		Entity& operator=(const Entity& other) = default;
		Entity& operator=(Entity&& other);
		template <typename T, typename... ARGS>
		T* AddComponent(ARGS&&... args)
		{
			Entity* t = this;
			T* component = new T(t, args...);
			m_Components.push_back(component);
			return component;
		};

		template <typename T>
		T* GetComponent()
		{
			const type_info& ti = typeid(T);
			for (auto* component : m_Components)
			{
				if (component && typeid(*component) == ti)
					return static_cast<T*>(component);
			}
			return nullptr;
		};
		void Update(float dt);

	private:
		std::vector<Component*> m_Components;
		Scene* m_pParentScene = nullptr;
	};
}