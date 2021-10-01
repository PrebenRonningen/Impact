#include "Core\ImpactWin.h"
#include "MovementComponent.h"
#include "Entity\Entity.h"

#include "Core\Keyboard.h"
#include "Components\TransformComponent.h"

#include "Core\QuaternionToEuler.h"

namespace Impact {
	MovementComponent::MovementComponent(Entity* pParent)
		: Component(pParent)
		, m_pTransform{ nullptr }
		, m_WorldUp{ 0,1,0 }
		, m_Up{ pParent->GetComponent<TransformComponent>()->UpVector() }
		, m_Right{ pParent->GetComponent<TransformComponent>()->RightVector() }
		, m_Forward{ pParent->GetComponent<TransformComponent>()->ForwardVector() }
		, m_MoveSpeed{ 10.f }
		, m_MoveSpeedMultiplyer{ 1.f }
		, m_RotationSpeed{ 1.f }
		, m_RotationSpeedMultiplyer{ 10.f }
		, m_Moved{ nullptr }
	{
		assert(pParent->GetComponent<TransformComponent>() != nullptr && "(MovementComponent) TransformComponent missing");
		m_pTransform = pParent->GetComponent<TransformComponent>();

		Reset();
	}

	void MovementComponent::Update(float dt) noexcept
	{
		if (!m_IsActive) return;

		m_MoveSpeedMultiplyer = (shift) ? 5.f : 1.f;
		m_RotationSpeedMultiplyer = (shift) ? 2.5f : 1.f;

		const float totalMovementSpeed = m_MoveSpeed * m_MoveSpeedMultiplyer * dt;
		const float totalRotationSpeed = m_RotationSpeed * m_RotationSpeedMultiplyer * dt;

		DirectX::XMFLOAT3 modUp = (ctrl) ? m_Up : m_WorldUp;

		if (Keyboard::Get().IsKeyDown('W'))
		{
			Move(m_Forward, totalMovementSpeed, true);
		}

		if (Keyboard::Get().IsKeyDown('S'))
		{
			Move(m_Forward, totalMovementSpeed, false);
		}

		if (Keyboard::Get().IsKeyDown('D'))
		{
			Move(m_Right, totalMovementSpeed, true);
		}

		if (Keyboard::Get().IsKeyDown('A'))
		{
			Move(m_Right, totalMovementSpeed, false);
		}

		if (Keyboard::Get().IsKeyDown('E')) // Up
		{
			Move(modUp, totalMovementSpeed, true);
		}

		if (Keyboard::Get().IsKeyDown('Q')) // Down
		{
			Move(modUp, totalMovementSpeed, false);
		}

		if (Keyboard::Get().IsKeyDown('K'))
		{
			Rotate(m_Right, totalRotationSpeed, true);
		}
		if (Keyboard::Get().IsKeyDown('I'))
		{
			Rotate(m_Right, totalRotationSpeed, false);
		}
		if (Keyboard::Get().IsKeyDown('L'))
		{
			Rotate(m_Up, totalRotationSpeed, true);
		}
		if (Keyboard::Get().IsKeyDown('J'))
		{
			Rotate(m_Up, totalRotationSpeed, false);
		}
		if (Keyboard::Get().IsKeyDown('U'))
		{
			Rotate(m_Forward, totalRotationSpeed, true);
		}
		if (Keyboard::Get().IsKeyDown('O'))
		{
			Rotate(m_Forward, totalRotationSpeed, false);
		}

		if (!*m_Moved) return;
		if (freeLook)
			CalculateDirectionVectors(GetParent()->GetComponent<TransformComponent>()->GetWorldTransform());

		//*m_Moved = false;
	}
	void MovementComponent::Reset()
	{
		GetParent()->GetComponent<TransformComponent>()->Translation() = { 0, 0, 150 };
		GetParent()->GetComponent<TransformComponent>()->LocalTranslation() = { 0, 0, 0 };
		GetParent()->GetComponent<TransformComponent>()->Rotation() = { 0, 0, 0 };
		GetParent()->GetComponent<TransformComponent>()->LocalRotation() = { 0, 0, 0 };
		GetParent()->GetComponent<TransformComponent>()->RightVector() = { -1,0,0 };
		GetParent()->GetComponent<TransformComponent>()->UpVector() = { 0,1,0 };
		GetParent()->GetComponent<TransformComponent>()->ForwardVector() = { 0,0,-1 };

		shift = false;
		ctrl = false;
		freeLook = false;
		if (m_Moved != nullptr)
			*m_Moved = true;
	}
	void MovementComponent::AssignMovementUpdate(bool& refMovement)
	{
		m_Moved = &refMovement;
	}
	bool MovementComponent::OnKeyEvent(Event& e)
	{
		auto keyEvent = static_cast<KeyEvent&>(e);
		bool handled = false;

		if (!m_IsActive)
			return handled;

		switch (keyEvent.GetType())
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

	void MovementComponent::Move(DirectX::XMFLOAT3& localAxis, float speed, bool alongAxisDirection)
	{
		DirectX::XMStoreFloat3(&GetParent()->GetComponent<TransformComponent>()->Translation(), //store the transformed vector
			DirectX::XMVector3Transform(
				DirectX::XMLoadFloat3(&GetParent()->GetComponent<TransformComponent>()->Translation()),	//transform this vector
				DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&localAxis), ((alongAxisDirection) ? 1.f : -1.f) * speed)))); // transformation matrix

		*m_Moved |= true;
	}
	void MovementComponent::Rotate(DirectX::XMFLOAT3 localAxis, float speed, bool alongAxisDirection)
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

		*m_Moved |= true;
	}
	void MovementComponent::CalculateDirectionVectors(const DirectX::XMFLOAT4X4& rotationMatrix)
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
}