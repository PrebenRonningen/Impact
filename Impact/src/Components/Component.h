#pragma once
namespace Impact
{
	class Entity;
	class Component
	{
	public:
		Component(Entity* pParent)
			: m_ParentEntity{ pParent }
			, m_IsActive{false}
			{};
		virtual	~Component(){};
		virtual void Update(float dt) noexcept {dt;};
		virtual void Refresh() noexcept {};
		Entity* GetParent() const noexcept {return m_ParentEntity;};
		virtual void SetActive()
		{
			m_IsActive = true;
		}
		virtual void SetInactive()
		{
			m_IsActive = false;
		}
		bool& GetIsActive()
		{
			return m_IsActive;
		}
	protected:
		bool m_IsActive;
	private:
		Entity* m_ParentEntity = nullptr;
	};
}