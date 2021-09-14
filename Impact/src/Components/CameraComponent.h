#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "Entity\Entity.h"
#include "Components\TransformComponent.h"
#include "Graphics/Bindable/TransformCbuf.h"

#include "Core\QuaternionToEuler.h"
#include "Events\EventHandler.h"
#include "Core\Keyboard.h"

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
			, m_WorldUp{0,1,0}
			, m_Up{ GetParent()->GetComponent<TransformComponent>()->UpVector() }
			, m_Right { GetParent()->GetComponent<TransformComponent>()->RightVector() }
			, m_Forward{ GetParent()->GetComponent<TransformComponent>()->ForwardVector() }
		{
			EventHandler::RegisterEvent(this, 1, Event::EventType::KeyPressed | Event::EventType::KeyReleased, [this](auto&&... args) -> decltype(auto)
				{
					return this->CameraComponent::OnKeyEvent(std::forward<decltype(args)>(args)...);
				});

			Reset();
		}

	
		virtual void Update(float dt) noexcept override
		{
			if(!m_IsActive) return;

			const float moveSpeedMultiplyer = (shift) ? 500.f : 0.f;
			const float totalMovementSpeed = 1 + moveSpeedMultiplyer * dt;

			DirectX::XMFLOAT3 modUp = (ctrl) ? m_Up : m_WorldUp;

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
			if (Keyboard::Get().IsKeyDown('U'))
			{
				Rotate(m_Forward, dt, true);
			}
			if (Keyboard::Get().IsKeyDown('O'))
			{
				Rotate(m_Forward, dt, false);
			}

			if (!m_Moved) return;
			if(freeLook)
				CalculateDirectionVectors(GetParent()->GetComponent<TransformComponent>()->GetWorldTransform());

				Impact::TransformCbuf::SetCamera(GetParent()->GetComponent<TransformComponent>()->GetInverseTransform());

			m_Moved = false;
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
			
			shift = false;
			ctrl = false;
			freeLook = false;
			Impact::TransformCbuf::SetCamera(GetParent()->GetComponent<TransformComponent>()->GetInverseTransform());
			m_Moved = true;
		}

		void SetActive()
		{
			m_IsActive = true;

			Impact::TransformCbuf::SetCamera(GetParent()->GetComponent<TransformComponent>()->GetInverseTransform());
		}
		void SetInactive()
		{
			m_IsActive = false;
		}
		bool& GetIsActive()
		{
			return m_IsActive;
		}

		bool OnKeyEvent(Event& e)
		{
			auto k = static_cast<KeyEvent&>(e);
			bool handled = false;

			if(!m_IsActive)
				return handled;

			switch (k.GetType())
			{
			case Event::EventType::KeyPressed:
				{
					if (Keyboard::Get().IsKeyDown(VK_SHIFT))
					{
						shift = true;
						handled |= true;
					}
					if (Keyboard::Get().IsKeyDown(VK_CONTROL))
					{
						// toggle
						ctrl = !ctrl;
						handled |= true;
					}
					if (Keyboard::Get().IsKeyDown('Z'))
					{
						freeLook = false;
						handled |= true;
					}
					break;
				}
			case Event::EventType::KeyReleased:
				{
					if (Keyboard::Get().IsKeyUp(VK_SHIFT))
					{
						shift = false;
						handled |= true;
					}
					//if (Keyboard::Get().IsKeyUp(VK_CONTROL))
					//{
					//	m_Modifiers.ctrl = false;
					//	handled |= true;
					//}
					if (Keyboard::Get().IsKeyUp('Z'))
					{
						freeLook = true;
						handled |= true;
					}
					break;
				}
			default:
				break;
			}
			return handled;
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
			// Delta Rotation
			auto eem = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&localAxis), ((alongAxisDirection) ? 1.f : -1.f) * speed);
			// modified Rotation Matrix
			auto mod = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&GetParent()->GetComponent<TransformComponent>()->GetWorldTransform()), eem);
		
			if (freeLook)
			{
				DirectX::XMFLOAT4X4 rotationMatrix;
				DirectX::XMStoreFloat4x4(&rotationMatrix, mod);
				CalculateDirectionVectors(rotationMatrix);
			}

			DirectX::XMFLOAT4 qRotation;
			DirectX::XMStoreFloat4(&qRotation, DirectX::XMQuaternionRotationMatrix(mod));

			GetParent()->GetComponent<TransformComponent>()->Rotation() = QuaternionToEulerDeg(qRotation);

			m_Moved |= true;
		}

		// for resetting FreeLook
		void CalculateDirectionVectors(const DirectX::XMFLOAT4X4& rotationMatrix)
		{
			DirectX::XMMATRIX M;

			M.r[0] = DirectX::g_XMNegIdentityR0;
			M.r[1] = DirectX::g_XMIdentityR1;
			M.r[2] = DirectX::g_XMNegIdentityR2;
			M.r[3] = DirectX::g_XMIdentityR3;

			// rotate Direction Vectors
			M = DirectX::XMMatrixMultiply(M, DirectX::XMLoadFloat4x4(&rotationMatrix));

			// store modified Direction Vectors
			DirectX::XMStoreFloat3(&m_Right, M.r[0]);
			DirectX::XMStoreFloat3(&m_Up, M.r[1]);
			DirectX::XMStoreFloat3(&m_Forward, M.r[2]);
		}

	private:
		bool shift;
		bool ctrl;
		bool freeLook;

		bool m_IsActive;
		bool m_Moved;

		DirectX::XMFLOAT3 m_WorldUp;
		
		DirectX::XMFLOAT3& m_Up;
		DirectX::XMFLOAT3& m_Right;
		DirectX::XMFLOAT3& m_Forward;

	};
}