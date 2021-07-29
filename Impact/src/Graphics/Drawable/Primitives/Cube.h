#pragma once
#include "Graphics\Drawable\RenderableBase.h"

namespace Impact
{
	class Entity;
	class Cube : public RenderableBase<Cube>
	{
	public:
		Cube(Entity* pParent, Graphics& gfx, bool randomized = false);
		virtual void Update(float dt) noexcept override;

	private:

	};
}



