#include "RandomMovementComponent.h"

#include "Entity\Entity.h"
#include "Components\TransformComponent.h"

#include <random>

namespace Impact {
	RandomMovementComponent::RandomMovementComponent(Entity* pParent)
		: Component(pParent)
	{
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> adist(0, 180 * 2.0f);
		std::uniform_real_distribution<float> ddist(0, 90);
		std::uniform_real_distribution<float> odist(0, 15);
		std::uniform_real_distribution<float> rdist(6, 20);
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
	}
	void RandomMovementComponent::Update(float dt) noexcept
	{
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;;
		phi += dphi * dt;
		chi += dchi * dt;

		auto e = GetParent()->GetComponent<TransformComponent>();
		e->LocalRotation().x = pitch;
		e->LocalRotation().y = yaw;
		e->LocalRotation().z = roll;
		e->LocalTranslation().x = r;
		e->Rotation().x = theta;
		e->Rotation().y = phi;
		e->Rotation().z = chi;
		e->Translation().z = 0;
	}

}