#pragma once

#include "RenderableBase.h"
#include <random>
namespace Impact
{
	class Entity;
	class TestCube : public RenderableBase<TestCube>
	{
	public:
		TestCube(Entity* pParent, Graphics& gfx);
		virtual void Update(float dt) noexcept override;

	private:
		
	};
}

