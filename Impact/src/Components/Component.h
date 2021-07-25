#pragma once
namespace Impact
{
	class Entity;
	class Component
	{
	public:
		Component(Entity* pParent)
			: m_ParentEntity{ pParent }
			{};
		virtual	~Component(){};
		virtual void Update(float dt) noexcept {dt;};
		Entity* GetParent() const noexcept {return m_ParentEntity;};
	private:
		Entity* m_ParentEntity = nullptr;
	};
}