#pragma once
#include "Component.h"
#include <DirectXMath.h>

namespace Impact
{
	class Entity;
	class TransformComponent;
	class Event;
	class MovementComponent : public Component
	{
	public:
		MovementComponent(Entity* pParent);

		virtual void Update(float dt) noexcept override;

		void Reset();
		void AssignMovementUpdate(bool& refMovement); 

		bool OnKeyEvent(Event& e);
		
	private:
		void Move(DirectX::XMFLOAT3& localAxis, float speed, bool alongAxisDirection);
		void Rotate(DirectX::XMFLOAT3 localAxis, float speed, bool alongAxisDirection);

		// for resetting FreeLook
		void CalculateDirectionVectors(const DirectX::XMFLOAT4X4& rotationMatrix);

	private:

		bool shift;
		bool ctrl;
		bool freeLook;

		bool* m_Moved;

		TransformComponent* m_pTransform;
		DirectX::XMFLOAT3 m_WorldUp;

		DirectX::XMFLOAT3& m_Up;
		DirectX::XMFLOAT3& m_Right;
		DirectX::XMFLOAT3& m_Forward;

		float m_MoveSpeed;
		float m_MoveSpeedMultiplyer;

		float m_RotationSpeed;
		float m_RotationSpeedMultiplyer;
	};
}