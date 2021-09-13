#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "Entity\Entity.h"
#include "Components\TransformComponent.h"
#include "Graphics/Bindable/TransformCbuf.h"

#include "Core\QuaternionToEuler.h"

namespace Impact
{
	class Entity;
	class CameraComponent : public Component
	{
	public:
		CameraComponent(Entity* pParent)
			: Component(pParent)
			, m_IsActive{false}
			, m_Moved{false}
			, m_LocalUp{false}
			, m_WorldUp{0,1,0}
			, m_Up{ GetParent()->GetComponent<TransformComponent>()->UpVector() }
			, m_Right { GetParent()->GetComponent<TransformComponent>()->RightVector() }
			, m_Forward{ GetParent()->GetComponent<TransformComponent>()->ForwardVector() }
			, m_LookAt{ }
		{

			GetParent()->GetComponent<TransformComponent>()->Translation() = { 0, 0, 850 };
			//Reset();
		}

		virtual void Update(float dt) noexcept override
		{
			if(!m_IsActive) return;

			const float moveSpeedMultiplyer = (Keyboard::Get().IsKeyDown(VK_SHIFT)) ? 500.f : 0.f;
			const float totalMovementSpeed = 1 + moveSpeedMultiplyer * dt;
			if(Keyboard::Get().IsKeyDown(VK_CONTROL))
				m_LocalUp = !m_LocalUp;
			DirectX::XMFLOAT3 modUp = (m_LocalUp) ? m_Up : m_WorldUp;


			if (Keyboard::Get().IsKeyDown('W'))
			{
				Move(m_Forward, totalMovementSpeed, true);
			}

			if (Keyboard::Get().IsKeyDown('S'))
			{
				Move(m_Forward, totalMovementSpeed, false);
			}

			if ( Keyboard::Get().IsKeyDown('D'))
			{
				Move(m_Right, totalMovementSpeed, true);
			}

			if ( Keyboard::Get().IsKeyDown('A'))
			{
				Move(m_Right, totalMovementSpeed, false);
			}

			if ( Keyboard::Get().IsKeyDown('E')) // Up
			{
				Move(modUp, totalMovementSpeed, true);
			}

			if ( Keyboard::Get().IsKeyDown('Q')) // Down
			{
				Move(modUp, totalMovementSpeed, false);
			}

			if (Keyboard::Get().IsKeyDown('K'))
			{
				Rotate(m_Right, dt, true);
			}
			if (Keyboard::Get().IsKeyDown('I'))
			{
				Rotate(m_Right, dt, false);
			}
			if (Keyboard::Get().IsKeyDown('L'))
			{
				Rotate(m_Up, dt, true);
			}
			if (Keyboard::Get().IsKeyDown('J'))
			{
				Rotate(m_Up, dt, false);
			}
			if (Keyboard::Get().IsKeyDown('O'))
			{
				Rotate(m_Forward, dt, true);
				m_LocalUp=true;
			}
			if (Keyboard::Get().IsKeyDown('U'))
			{
				Rotate(m_Forward, dt, false);
				m_LocalUp = true;
			}

			DirectX::XMFLOAT4X4 mat = GetParent()->GetComponent<TransformComponent>()->GetInverseTransform();
			Impact::TransformCbuf::SetCamera(mat);
			if (!m_Moved) return;


		//	DirectX::XMStoreFloat3(&GetParent()->GetComponent<TransformComponent>()->Translation(), DirectX::XMVector3TransformCoord(DirectX::XMVECTOR(), DirectX::XMLoadFloat4x4(&m_LookAt)));
		//
		//	DirectX::XMVECTOR vp;
		//	DirectX::XMVECTOR vr;
		//	DirectX::XMVECTOR vs; 
		//
		//	//DirectX::XMMatrixDecompose(&vs, &vr, &vp, DirectX::XMLoadFloat4x4(&m_LookAt));
		//
		//	DirectX::XMFLOAT4 Qang;
		//	DirectX::XMStoreFloat4(&Qang, vr);
		//	DirectX::XMFLOAT3 ang = QuaternionToEuler(Qang);
		//	ang.x = DirectX::XMConvertToDegrees(ang.x);
		//	ang.y = DirectX::XMConvertToDegrees(ang.y)+180.f;
		//	ang.z = DirectX::XMConvertToDegrees(ang.z);
		//	GetParent()->GetComponent<TransformComponent>()->Rotation() = ang;
		//
		//	DirectX::XMFLOAT4X4 mat = GetParent()->GetComponent<TransformComponent>()->GetTransform();
			//DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&mat)));
			m_Moved = false;
		}

		void UpdatePosition() noexcept
		{
		//	DirectX::XMFLOAT3 pos = GetParent()->GetComponent<TransformComponent>()->Translation();
		//
		//	m_LookAt.m[3][0] = pos.x;
		//	m_LookAt.m[3][1] = pos.y;
		//	m_LookAt.m[3][2] = pos.z;
			m_Moved = true;
		}

		void UpdateRotation()
		{
		//DirectX::XMFLOAT3 Drot = GetParent()->GetComponent<TransformComponent>()->Rotation();
		//DirectX::XMFLOAT3 Rrot = {DirectX::XMConvertToRadians(Drot.x),DirectX::XMConvertToRadians(Drot.y+180.f), DirectX::XMConvertToRadians(Drot.z) };
		//DirectX::XMVECTOR rot = DirectX::XMLoadFloat3(&Rrot);
		//
		//DirectX::XMFLOAT3 f{0,0,1};
		//DirectX::XMFLOAT3 u{0,1,0};
		//DirectX::XMFLOAT3 r{-1,0,0};
		//
		//DirectX::XMStoreFloat4x4(&m_LookAt, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&m_LookAt), DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&GetParent()->GetComponent<TransformComponent>()->Translation()), -1.f))));
		//
		//DirectX::XMStoreFloat4x4(&m_LookAt, DirectX::XMMatrixMultiply(DirectX::XMMatrixIdentity(), DirectX::XMMatrixRotationRollPitchYawFromVector(rot)));
		//
		//DirectX::XMStoreFloat3(&m_Forward, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&f), DirectX::XMMatrixRotationRollPitchYawFromVector(rot)));
		//DirectX::XMStoreFloat3(&m_Up, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&u), DirectX::XMMatrixRotationRollPitchYawFromVector(rot)));
		//DirectX::XMStoreFloat3(&m_Right, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&r), DirectX::XMMatrixRotationRollPitchYawFromVector(rot)));
		//
		//DirectX::XMStoreFloat4x4(&m_LookAt, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&m_LookAt), DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&GetParent()->GetComponent<TransformComponent>()->Translation()))));

			m_Moved = true;
		}

		void Reset()
		{
			GetParent()->GetComponent<TransformComponent>()->Translation() = { 0, 0, 850 };
			GetParent()->GetComponent<TransformComponent>()->LocalTranslation() = { 0, 0, 0 };
			GetParent()->GetComponent<TransformComponent>()->Rotation() = { 0, 0, 0 };
			GetParent()->GetComponent<TransformComponent>()->LocalRotation() = { 0, 0, 0 };
			GetParent()->GetComponent<TransformComponent>()->RightVector() = {-1,0,0};
			GetParent()->GetComponent<TransformComponent>()->UpVector() = {0,1,0};
			GetParent()->GetComponent<TransformComponent>()->ForwardVector()= {0,0,-1};
			

			//UpdatePosition();
			//UpdateRotation();
		//	Impact::TransformCbuf::SetCamera(m_LookAt);
			m_Moved = true;
		}

		void SetActive()
		{
			m_IsActive = true;
		}
		void SetInactive()
		{
			m_IsActive = false;
		}
		bool& GetIsActive()
		{
			return m_IsActive;
		}

	private:
		void Move(DirectX::XMFLOAT3& localAxis, float speed, bool alongAxisDirection)
		{
			DirectX::XMStoreFloat3(&GetParent()->GetComponent<TransformComponent>()->Translation(), //store the transformed vector
				DirectX::XMVector3Transform(
					DirectX::XMLoadFloat3(&GetParent()->GetComponent<TransformComponent>()->Translation()),	//transform this vector
					DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&localAxis), ((alongAxisDirection) ? 1.f : -1.f) * speed)))); // transformation matrix
			
			m_Moved |= true;
		}
		void Rotate(DirectX::XMFLOAT3 localAxis, float speed, bool alongAxisDirection)
		{
		//	DirectX::XMStoreFloat4x4(&m_LookAt, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&m_LookAt), DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&GetParent()->GetComponent<TransformComponent>()->Translation()), -1.f))));

			DirectX::XMMATRIX M;

			M.r[0] = DirectX::XMVectorSet(m_Right.x, m_Right.y, m_Right.z, 0);
			M.r[1] = DirectX::XMVectorSet(m_Up.x, m_Up.y, m_Up.z, 0);
			M.r[2] = DirectX::XMVectorSet(m_Forward.x, m_Forward.y, m_Forward.z, 0);
			M.r[3] = DirectX::g_XMIdentityR3;


			//M = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&localAxis), ((alongAxisDirection) ? 1.f : -1.f) * speed), M);

			auto bitch = GetParent()->GetComponent<TransformComponent>()->Rotation();
			auto rot = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(bitch.x), DirectX::XMConvertToRadians(bitch.y), DirectX::XMConvertToRadians(bitch.z));
			auto eem = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&localAxis), ((alongAxisDirection) ? 1.f : -1.f) * speed);
			M = DirectX::XMMatrixMultiply(M,eem);
			auto mod = DirectX::XMMatrixMultiply(rot, eem);


			DirectX::XMStoreFloat3(&m_Right, M.r[0]);
			DirectX::XMStoreFloat3(&m_Up, M.r[1]);
			DirectX::XMStoreFloat3(&m_Forward, M.r[2]);

			DirectX::XMFLOAT4 qRotation;
			DirectX::XMStoreFloat4(&qRotation, DirectX::XMQuaternionRotationMatrix(mod));
			auto newRotaion = QuaternionToEulerDeg(qRotation);
			GetParent()->GetComponent<TransformComponent>()->Rotation() = { newRotaion.x, newRotaion.y, newRotaion.z };



			//DirectX::XMStoreFloat3(&m_Forward, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_Forward), DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&localAxis), ((alongAxisDirection) ? 1.f : -1.f) * speed)));
			//DirectX::XMStoreFloat3(&m_Up, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_Up), DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&localAxis), speed))); // don't remember why this one is like this
			//DirectX::XMStoreFloat3(&m_Right, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_Right), DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&localAxis), ((alongAxisDirection) ? 1.f : -1.f) * speed)));

			m_Moved |= true;
		}
	private:
		bool m_IsActive;
		bool m_Moved;
		bool m_LocalUp;
		DirectX::XMFLOAT3 m_WorldUp;
		
		DirectX::XMFLOAT3& m_Up;
		DirectX::XMFLOAT3& m_Right;
		DirectX::XMFLOAT3& m_Forward;

		DirectX::XMFLOAT4X4 m_LookAt;
	};
}