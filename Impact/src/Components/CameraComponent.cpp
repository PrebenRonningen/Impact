#include "CameraComponent.h"
#include "Components\TransformComponent.h"
#include "Graphics\Bindable\TransformCbuf.h"
#include "Entity\Entity.h"

namespace Impact
{
	CameraComponent::CameraComponent(Entity* pParent)
		: Component(pParent)
		, m_NeedsUpdate{ false }
	{
		Impact::TransformCbuf::SetCamera(GetParent()->GetComponent<TransformComponent>()->GetInverseTransform());
	}

	void CameraComponent::Update(float) noexcept
	{
		if (!m_IsActive || !m_NeedsUpdate) return;

		Impact::TransformCbuf::SetCamera(GetParent()->GetComponent<TransformComponent>()->GetInverseTransform());
		m_NeedsUpdate = false;
	}

	void CameraComponent::UpdateView() const noexcept
	{
		Impact::TransformCbuf::SetCamera(GetParent()->GetComponent<TransformComponent>()->GetInverseTransform());
	}

	void CameraComponent::SetActive() noexcept
	{
		m_IsActive = true;

		Impact::TransformCbuf::SetCamera(GetParent()->GetComponent<TransformComponent>()->GetInverseTransform());
	}

	bool&  CameraComponent::GetNeedsUpdate() noexcept
	{
		return m_NeedsUpdate;
	}
}