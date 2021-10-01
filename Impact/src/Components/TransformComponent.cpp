#include "TransformComponent.h"

namespace Impact
{
	TransformComponent::TransformComponent(Entity* pParent)
		: Component(pParent)
		, m_LocalTranslation{ 0.0f, 0.0f, 0.0f }
		, m_LocalRotation{ 0.0f, 0.0f, 0.0f }
		, m_LocalScale{ 1.0f, 1.0f, 1.0f }
		, m_WorldTranslation{ 0.0f, 0.0f, 0.0f }
		, m_WorldRotation{ 0.0f, 0.0f, 0.0f }
		, m_WorldScale{ 1.0f, 1.0f, 1.0f }
		, m_LocalTransform{}
		, m_WorldTransform{}
		, m_Base{}
		, m_RightVector{ -1.f, 0.f, 0.f }
		, m_UpVector{ 0.f, 1.f, 0.f }
		, m_ForwardVector{ 0.f, 0.f, -1.f }
	{
		DirectX::XMStoreFloat4x4(&m_Base, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&m_LocalTranslation), DirectX::XMLoadFloat3(&m_ForwardVector), DirectX::XMLoadFloat3(&m_UpVector)));
	}
	DirectX::XMFLOAT3& TransformComponent::LocalTranslation() noexcept
	{
		return m_LocalTranslation;
	}
	DirectX::XMFLOAT3& TransformComponent::LocalRotation() noexcept
	{
		return m_LocalRotation;
	}
	DirectX::XMFLOAT3& TransformComponent::LocalScale() noexcept
	{
		return m_LocalScale;
	}
	DirectX::XMFLOAT3& TransformComponent::Translation() noexcept
	{
		return m_WorldTranslation;
	}
	DirectX::XMFLOAT3& TransformComponent::Rotation() noexcept
	{
		return m_WorldRotation;
	}
	DirectX::XMFLOAT3& TransformComponent::Scale() noexcept
	{
		return m_WorldScale;
	}
	DirectX::XMFLOAT3& TransformComponent::ForwardVector() noexcept
	{
		return m_ForwardVector;
	}
	DirectX::XMFLOAT3& TransformComponent::UpVector() noexcept
	{
		return m_UpVector;
	}
	DirectX::XMFLOAT3& TransformComponent::RightVector() noexcept
	{
		return m_RightVector;
	}
	DirectX::XMFLOAT4X4& TransformComponent::GetWorldTransform() noexcept
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

	DirectX::XMFLOAT4X4& TransformComponent::GetLocalTransform() noexcept
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

	DirectX::XMFLOAT4X4 TransformComponent::GetTransform() noexcept
	{
		DirectX::XMFLOAT4X4 localTrans = GetLocalTransform();
		DirectX::XMFLOAT4X4 worldTrans = GetWorldTransform();

		DirectX::XMFLOAT4X4 trans;
		DirectX::XMStoreFloat4x4(&trans, DirectX::XMLoadFloat4x4(&m_Base) * DirectX::XMLoadFloat4x4(&localTrans) * DirectX::XMLoadFloat4x4(&worldTrans));
		return trans;
	}

	DirectX::XMFLOAT4X4 TransformComponent::GetInverseTransform() noexcept
	{
		DirectX::XMFLOAT4X4 mat = GetTransform();
		DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&mat)));
		return mat;
	}
}