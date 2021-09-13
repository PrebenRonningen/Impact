#pragma once
#include "Component.h"
#include <DirectXMath.h>
namespace Impact 
{
	class TransformComponent : public Component
	{
	public:
		
		TransformComponent(Entity* pParent)
			: Component(pParent)
			, m_LocalTranslation { 0.0f, 0.0f, 0.0f }
			, m_LocalRotation { 0.0f, 0.0f, 0.0f }
			, m_LocalScale { 1.0f, 1.0f, 1.0f }
			, m_WorldTranslation { 0.0f, 0.0f, 0.0f }
			, m_WorldRotation { 0.0f, 0.0f, 0.0f }
			, m_WorldScale { 1.0f, 1.0f, 1.0f }
			, m_LocalTransform{}
			, m_WorldTransform{}
			, m_RightVector{ -1.f, 0.f, 0.f }
			, m_UpVector{ 0.f, 1.f, 0.f }
			, m_ForwardVector { 0.f, 0.f, -1.f }
		{
		
			DirectX::XMStoreFloat4x4(&m_Base, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&m_LocalTranslation), DirectX::XMLoadFloat3(&m_ForwardVector), DirectX::XMLoadFloat3(&m_UpVector)));

		}

		DirectX::XMFLOAT3& LocalTranslation() noexcept
		{
			return m_LocalTranslation;
		}
		DirectX::XMFLOAT3& LocalRotation() noexcept
		{
			return m_LocalRotation;
		}
		DirectX::XMFLOAT3& LocalScale() noexcept
		{
			return m_LocalScale;
		}
		DirectX::XMFLOAT3& Translation() noexcept
		{
			return m_WorldTranslation;
		}
		DirectX::XMFLOAT3& Rotation() noexcept
		{
			return m_WorldRotation;
		}
		DirectX::XMFLOAT3& Scale() noexcept
		{
			return m_WorldScale;
		}
		DirectX::XMFLOAT3& ForwardVector() noexcept
		{
			return m_ForwardVector;
		}
		DirectX::XMFLOAT3& UpVector() noexcept
		{
			return m_UpVector;
		}
		DirectX::XMFLOAT3& RightVector() noexcept
		{
			return m_RightVector;
		}
		DirectX::XMFLOAT4X4& GetWorldTransform() noexcept
		{
			DirectX::XMMATRIX mat =
				DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&m_WorldScale)) *
				DirectX::XMMatrixRotationRollPitchYaw(
					DirectX::XMConvertToRadians(m_WorldRotation.x),
					DirectX::XMConvertToRadians(m_WorldRotation.y),
					DirectX::XMConvertToRadians(m_WorldRotation.z)) *
				DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&m_WorldTranslation));

			DirectX::XMStoreFloat4x4(&m_WorldTransform, mat);
			return m_WorldTransform;
		}

		DirectX::XMFLOAT4X4& GetLocalTransform() noexcept
		{
			DirectX::XMMATRIX mat =
				DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&m_LocalScale)) *
				DirectX::XMMatrixRotationRollPitchYaw(
					DirectX::XMConvertToRadians(m_LocalRotation.x),
					DirectX::XMConvertToRadians(m_LocalRotation.y),
					DirectX::XMConvertToRadians(m_LocalRotation.z)) *
				DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&m_LocalTranslation));
			
			DirectX::XMStoreFloat4x4(&m_LocalTransform, mat);
			return m_LocalTransform;
		}
		DirectX::XMFLOAT4X4 GetTransform() noexcept
		{
			DirectX::XMFLOAT4X4 localTrans = GetLocalTransform();
			DirectX::XMFLOAT4X4 worldTrans = GetWorldTransform();

			DirectX::XMFLOAT4X4 trans;
			DirectX::XMStoreFloat4x4(&trans, DirectX::XMLoadFloat4x4(&m_Base) * DirectX::XMLoadFloat4x4(&localTrans) * DirectX::XMLoadFloat4x4(&worldTrans));
			return trans;
		}
		DirectX::XMFLOAT4X4 GetInverseTransform() noexcept
		{
			DirectX::XMFLOAT4X4 mat = GetTransform();
			DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&mat)));
			return mat;
		}

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