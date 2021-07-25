#include "Entity.h"
#include "Components\Component.h"
#include "Scene\Scene.h"

namespace Impact 
{
	Entity::Entity(Scene* pParentScene)
		: m_pParentScene{pParentScene}
	{}

	Entity::~Entity()
	{
		for (Component* c : m_Components) {
			if (c != nullptr)
			{
				delete c;
				c = nullptr;
			}
		}
	}


	Entity::Entity(Entity&& other)
		: m_Components{}
		, m_pParentScene{nullptr}
	{
		this->m_Components.insert(this->m_Components.begin(), std::make_move_iterator(other.m_Components.begin()), std::make_move_iterator(other.m_Components.end()));
		this->m_pParentScene = other.m_pParentScene;
		other.m_pParentScene = nullptr;
		other.m_Components = std::vector<Impact::Component*>();
	}

	Entity& Entity::operator=(Entity&& other)
	{
		if (this != &other)
		{
			for (auto c : this->m_Components) {
				if (c != nullptr) {
					delete c;
					c = nullptr;
				}
			}
			this->m_Components.insert(this->m_Components.begin(), std::make_move_iterator(other.m_Components.begin()), std::make_move_iterator(other.m_Components.end()));
			this->m_pParentScene = other.m_pParentScene;
			other.m_pParentScene = nullptr;
			other.m_Components = std::vector<Impact::Component*>();
		}
		return *this;
	}
	void Entity::Update(float dt)
	{
		for (auto* c : m_Components)
			c->Update(dt);
	};
}