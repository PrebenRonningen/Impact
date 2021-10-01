#pragma once
#include "Component.h"
#include <DirectXMath.h>

namespace Impact 
{
	class TransformComponent : public Component
	{
	public:
		
		TransformComponent(Entity* pParent);

		DirectX::XMFLOAT3& LocalTranslation() noexcept;
		DirectX::XMFLOAT3& LocalRotation() noexcept;
		DirectX::XMFLOAT3& LocalScale() noexcept;

		DirectX::XMFLOAT3& Translation() noexcept;
		DirectX::XMFLOAT3& Rotation() noexcept;
		DirectX::XMFLOAT3& Scale() noexcept;
	
		DirectX::XMFLOAT3& ForwardVector() noexcept;
		DirectX::XMFLOAT3& UpVector() noexcept;
		DirectX::XMFLOAT3& RightVector() noexcept;

		DirectX::XMFLOAT4X4& GetWorldTransform() noexcept;
		DirectX::XMFLOAT4X4& GetLocalTransform() noexcept;

		DirectX::XMFLOAT4X4 GetTransform() noexcept;
		DirectX::XMFLOAT4X4 GetInverseTransform() noexcept;

	private:
		DirectX::XMFLOAT3 m_LocalTranslation;
		DirectX::XMFLOAT3 m_LocalRotation;
		DirectX::XMFLOAT3 m_LocalScale;

		DirectX::XMFLOAT3 m_WorldTranslation;
		DirectX::XMFLOAT3 m_WorldRotation;
		DirectX::XMFLOAT3 m_WorldScale;

		DirectX::XMFLOAT4X4 m_LocalTransform; 
		DirectX::XMFLOAT4X4 m_WorldTransform;
		DirectX::XMFLOAT4X4 m_Base;

		DirectX::XMFLOAT3 m_RightVector;
		DirectX::XMFLOAT3 m_UpVector;
		DirectX::XMFLOAT3 m_ForwardVector;
	};
}