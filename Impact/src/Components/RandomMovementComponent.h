#pragma once
#include "Component.h"
#include <DirectXMath.h>

namespace Impact
{
	class Entity;
	class RandomMovementComponent : public Component
	{
	public:
		RandomMovementComponent(Entity* pParent);
			
		virtual void Update(float dt) noexcept override;

		private:
			//	will be local/world TransformComponent
			//pos
			float r;
			float roll = 0;
			float pitch = 0;
			float yaw = 0;
			float theta;
			float phi;
			float chi;
			// speed
			float droll;
			float dpitch;
			float dyaw;
			float dtheta;
			float dphi;
			float dchi;
	};
}