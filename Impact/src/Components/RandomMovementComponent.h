#pragma once
#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "Entity\Entity.h"
#include "Components\TransformComponent.h"

#include <random>
namespace Impact
{
	class Entity;
	class RandomMovementComponent : public Component
	{
	public:
		RandomMovementComponent(Entity* pParent)
			:Component(pParent)
			{
				std::mt19937 rng(std::random_device{}());
				std::uniform_real_distribution<float> adist(0, 0);//180 * 2.0f);
				std::uniform_real_distribution<float> ddist(5, 20);
				std::uniform_real_distribution<float> odist(0,0 );
				std::uniform_real_distribution<float> rdist(0, 0);
				r = rdist(rng);
				droll = ddist(rng);
				dpitch = ddist(rng);
				dyaw = ddist(rng);
				dphi = odist(rng);
				dtheta = odist(rng);
				dchi = odist(rng);
				chi = adist(rng);
				theta = adist(rng);
				phi = adist(rng);
				//roll = 45;
				//chi = -23.44f;
				chi = 0;
			}
			
		virtual void Update(float dt) noexcept override
		{
			//roll += droll * dt;
			//pitch += dpitch * dt;
			yaw += dyaw * dt;
			//theta += dtheta * dt;;
			//phi += dphi * dt;
			//chi += dchi * dt;

			auto e = GetParent()->GetComponent<TransformComponent>();
			e->Rotation().x = pitch;
			e->Rotation().y = yaw;
			e->Rotation().z = roll;
			e->Translation().x = r;
			e->WorldRotation().x = theta;
			e->WorldRotation().y = phi;
			e->WorldRotation().z = chi;
			e->WorldTranslation().z = 0;
		}

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