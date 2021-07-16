#pragma once

#include "RenderComponent.h"
#include <random>
namespace Impact
{
	class TestCube : public RenderComponent
	{
	public:
		TestCube(Graphics& gfx, std::mt19937& rng,
			std::uniform_real_distribution<float>& adist,
			std::uniform_real_distribution<float>& ddist,
			std::uniform_real_distribution<float>& odist,
			std::uniform_real_distribution<float>& rdist);
		virtual void Update(float dt) noexcept override;
		virtual DirectX::XMFLOAT4X4 GetTransform() const noexcept override;

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

