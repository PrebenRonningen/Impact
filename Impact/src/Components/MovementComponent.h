#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "Entity\Entity.h"
#include "Components\TransformComponent.h"
#include "Core\Keyboard.h"

namespace Impact
{

	// TODO: Combine This with CameraComponent 
	class Entity;
	class MovementComponent : public Component
	{
	public:
		MovementComponent(Entity* pParent)
			: Component(pParent)
			, m_IsActive{true}
			, m_pTransform{nullptr}
			, m_Forward{ DirectX::XMFLOAT3(0, 0, -1) }
			, m_Up{ DirectX::XMFLOAT3(0, 1, 0) }
			, m_Right{ DirectX::XMFLOAT3(-1, 0, 0) }
			, m_LookAt{}
			, m_MoveSpeed{100.f}
			, m_MoveSpeedMultiplyer{1.f}
			, m_RotationSpeed{1.f}
		{
			assert(pParent->GetComponent<TransformComponent>() != nullptr && "(MovementComponent) TransformComponent missing");
			m_pTransform = pParent->GetComponent<TransformComponent>();
		}

		virtual void Update(float dt) noexcept override
		{
			if(!m_IsActive) return;
			m_MoveSpeedMultiplyer = (Keyboard::Get().IsKeyDown(VK_SHIFT)) ? 5.f : 1.f;

			const float totalMovementSpeed = m_MoveSpeed * dt * m_MoveSpeedMultiplyer;

			if (Keyboard::Get().IsKeyDown('W'))
			{
				DirectX::XMStoreFloat3(&m_pTransform->Translation(), 
					DirectX::XMVectorMultiplyAdd(
						DirectX::XMLoadFloat3(&m_pTransform->ForwardVector()),
						DirectX::XMVectorReplicate(totalMovementSpeed),
						DirectX::XMLoadFloat3(&m_pTransform->Translation())));
			}

			if (Keyboard::Get().IsKeyDown('S'))
			{
				DirectX::XMStoreFloat3(&m_pTransform->Translation(),
					DirectX::XMVectorMultiplyAdd(
						DirectX::XMLoadFloat3(&m_pTransform->ForwardVector()),
						DirectX::XMVectorReplicate(-totalMovementSpeed),
						DirectX::XMLoadFloat3(&m_pTransform->Translation())));
			}

			if (Keyboard::Get().IsKeyDown('A'))
			{
				DirectX::XMStoreFloat3(&m_pTransform->Translation(),
					DirectX::XMVectorMultiplyAdd(
						DirectX::XMLoadFloat3(&m_pTransform->RightVector()),
						DirectX::XMVectorReplicate(totalMovementSpeed),
						DirectX::XMLoadFloat3(&m_pTransform->Translation())));
			}

			if (Keyboard::Get().IsKeyDown('D'))
			{
				DirectX::XMStoreFloat3(&m_pTransform->Translation(),
					DirectX::XMVectorMultiplyAdd(
						DirectX::XMLoadFloat3(&m_pTransform->RightVector()),
						DirectX::XMVectorReplicate(-totalMovementSpeed),
						DirectX::XMLoadFloat3(&m_pTransform->Translation())));
			}

			if (Keyboard::Get().IsKeyDown('Q'))
			{
				DirectX::XMStoreFloat3(&m_pTransform->Translation(),
					DirectX::XMVectorMultiplyAdd(
						DirectX::XMLoadFloat3(&m_pTransform->UpVector()),
						DirectX::XMVectorReplicate(totalMovementSpeed),
						DirectX::XMLoadFloat3(&m_pTransform->Translation())));
			}

			if (Keyboard::Get().IsKeyDown('E'))
			{
				DirectX::XMStoreFloat3(&m_pTransform->Translation(),
					DirectX::XMVectorMultiplyAdd(
						DirectX::XMLoadFloat3(&m_pTransform->UpVector()),
						DirectX::XMVectorReplicate(-totalMovementSpeed),
						DirectX::XMLoadFloat3(&m_pTransform->Translation())));
			}

		}

	private:
		bool m_IsActive;
		TransformComponent* m_pTransform;
		DirectX::XMFLOAT3 m_Forward;
		DirectX::XMFLOAT3 m_Up;
		DirectX::XMFLOAT3 m_Right;

		DirectX::XMFLOAT4X4 m_LookAt;
		float m_MoveSpeed;
		float m_MoveSpeedMultiplyer;

		float m_RotationSpeed;
	};
}