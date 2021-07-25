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
		{}

		DirectX::XMFLOAT3& Translation() noexcept
		{
			return m_Translation;
		}
		DirectX::XMFLOAT3& Rotation() noexcept
		{
			return m_Rotation;
		}
		DirectX::XMFLOAT3& Scale() noexcept
		{
			return m_Scale;
		}
		DirectX::XMFLOAT3& WorldTranslation() noexcept
		{
			return m_WorldTranslation;
		}
		DirectX::XMFLOAT3& WorldRotation() noexcept
		{
			return m_WorldRotation;
		}
		DirectX::XMFLOAT4X4 GetWorldTransform() const noexcept
		{
			DirectX::XMFLOAT4X4 trans;
			DirectX::XMMATRIX mat =
				DirectX::XMMatrixIdentity() *
				DirectX::XMMatrixRotationRollPitchYaw(
					DirectX::XMConvertToRadians(m_WorldRotation.x),
					DirectX::XMConvertToRadians(m_WorldRotation.y),
					DirectX::XMConvertToRadians(m_WorldRotation.z)) *
				DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&m_WorldTranslation));

			DirectX::XMStoreFloat4x4(&trans, mat);
			return trans;
		}

		DirectX::XMFLOAT4X4 GetLocalTransform() const noexcept
		{

			DirectX::XMFLOAT4X4 trans;
			DirectX::XMMATRIX mat =
				DirectX::XMMatrixIdentity() *
				DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&m_Scale)) *
				DirectX::XMMatrixRotationRollPitchYaw(
					DirectX::XMConvertToRadians(m_Rotation.x),
					DirectX::XMConvertToRadians(m_Rotation.y),
					DirectX::XMConvertToRadians(m_Rotation.z)) *
				DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&m_Translation));
			
			DirectX::XMStoreFloat4x4(&trans, mat);
			return trans;
		}
		DirectX::XMFLOAT4X4 GetTransform() const noexcept
		{
			DirectX::XMFLOAT4X4 localTrans = GetLocalTransform();
			DirectX::XMFLOAT4X4 worldTrans = GetWorldTransform();
			DirectX::XMMATRIX mat = DirectX::XMLoadFloat4x4(&localTrans) * DirectX::XMLoadFloat4x4(&worldTrans);
			DirectX::XMFLOAT4X4 trans;
			DirectX::XMStoreFloat4x4(&trans, mat);
			return trans;
		}
	private:
		DirectX::XMFLOAT3 m_Translation = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 m_Scale = { 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT3 m_WorldRotation = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 m_WorldTranslation = { 0.0f, 0.0f, 0.0f };






	};
}