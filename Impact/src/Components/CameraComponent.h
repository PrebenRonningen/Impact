#pragma once
#include "Component.h"
#include <DirectXMath.h>

namespace Impact
{
	class Entity;
	class CameraComponent : public Component
	{
	public:
		CameraComponent(Entity* pParent);
		
		virtual void Update(float dt) noexcept override;
		void UpdateView() const noexcept;
		virtual void SetActive() noexcept override;
		bool& GetNeedsUpdate() noexcept;

	private:
		bool m_NeedsUpdate;
	};
}